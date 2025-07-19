#include "QtAntDesign.h"
#include <qevent.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qscreen.h>
#include "StyleSheet.h"
#include "AntMessageManager.h"
#include "AntTooltipManager.h"
#include "DialogViewController.h"
#include "HomePage.h"
#include "SettingsPage.h"
#include "AboutPage.h"
#include <QToolButton>
#include <QLabel>
#include <QShowEvent>
#include "NotificationManager.h"
#include "DesignSystem.h"

QtAntDesign::QtAntDesign(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setObjectName("QtAntDesign");
	setWindowFlags(Qt::FramelessWindowHint);
	setMinimumSize(1100, 750);

	ui.main_widget->setStyleSheet(StyleSheet::mainQss());

	// 初始化全局设计系统 必须写在最前面 因为它会设置主题和主窗口指针
	DesignSystem::instance()->setThemeMode(DesignSystem::Light);	// 默认亮主题
	DesignSystem::instance()->setMainWindow(this);					// 获取主窗口指针

	// 获取主屏幕分辨率
	int w = 0, h = 0;
	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen)
	{
		QSize screenSize = screen->availableGeometry().size();		// 可用屏幕大小，不包括任务栏
		w = int(screenSize.width() * 0.50);  // % 宽度
		h = int(screenSize.height() * 0.60); // % 高度
		resize(w, h);
	}
	setContentsMargins(0, 0, 0, 0);

	// 任务栏 内容区域 导航栏 布局调整
	ui.navi_widget->setFixedWidth(m_naviWidth); // 希望的宽度
	ui.navi_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);	// 水平方向缩放策略固定
	ui.navi_widget->setStyleSheet(StyleSheet::naviQss());
	ui.titleBar->setStyleSheet(StyleSheet::titleBarQss());
	ui.central->setStyleSheet(StyleSheet::centralQss());
	ui.titleBar->setFixedHeight(m_titleBarHeight);

	// 标题栏
	QHBoxLayout* titleLay = new QHBoxLayout(ui.titleBar);
	titleLay->setContentsMargins(20, 0, 12, 0);
	titleLay->setSpacing(6);
	QFont font;
	font.setPointSizeF(14);
	font.setBold(true);
	QLabel* title = new QLabel("TITLE", ui.titleBar);
	title->setFont(font);
	// 创建按钮
	btnMin = new QToolButton(ui.titleBar);
	btnMax = new QToolButton(ui.titleBar);
	btnClose = new QToolButton(ui.titleBar);
	// 设置图标
	btnMin->setIcon(QIcon(":/Imgs/Minimize-2.svg"));
	btnMax->setIcon(QIcon(":/Imgs/Maximize-1.svg"));
	btnClose->setIcon(QIcon(":/Imgs/Shut down-2.svg"));
	// 设置QSS
	btnMin->setStyleSheet(StyleSheet::toolBtnQss());
	btnMax->setStyleSheet(StyleSheet::toolBtnQss());
	btnClose->setStyleSheet(StyleSheet::toolBtnQss());
	// 设置固定大小（根据图标适当调整）
	btnMin->setFixedSize(32, 32);
	btnMax->setFixedSize(32, 32);
	btnClose->setFixedSize(34, 34);
	int extraWidth = 100;	// 额外的间距宽度: 是控件间距以及标题栏两端的间距 自己根据标题栏所有控件的宽度调整
	m_widgetTotalWidth = btnMin->width() + btnMax->width() + btnClose->width() + extraWidth;

	// 将标题和按钮添加到布局
	titleLay->addWidget(title);
	titleLay->addStretch();
	titleLay->addWidget(btnMin);
	titleLay->addWidget(btnMax);
	titleLay->addWidget(btnClose);

	// 导航栏添加控件
	QVBoxLayout* naviLay = new QVBoxLayout(ui.navi_widget);;
	ui.navi_widget->layout()->setContentsMargins(0, 0, 0, 0);
	CircularAvatar* avatar = new CircularAvatar(QSize(42, 42), ":/Imgs/noLogin.svg", ":/Imgs/github.svg", ui.navi_widget);
	// 添加页面
	QVBoxLayout* contentLay = new QVBoxLayout(ui.central);
	contentLay->setContentsMargins(0, 0, 0, 0);
	contentLay->setSpacing(0);
	SlideStackedWidget* stackedWidget = new SlideStackedWidget(ui.central);
	contentLay->addWidget(stackedWidget);
	HomePage* homePage = new HomePage(ui.main_widget);
	SettingsPage* settingsPage = new SettingsPage(stackedWidget);
	AboutPage* aboutPage = new AboutPage(stackedWidget);
	stackedWidget->addWidget(homePage);
	stackedWidget->addWidget(settingsPage);
	stackedWidget->addWidget(aboutPage);
	stackedWidget->setCurrentIndex(0);				// 默认显示首页
	// 导航按钮
	const int naviWidth = ui.navi_widget->width();  // 动态获取导航栏宽度
	const double iconSizeRatio = 0.56;				// 图标占按钮的比例
	const int buttonSize = naviWidth;
	const int iconSize = static_cast<int>(buttonSize * iconSizeRatio);
	CustomToolButton* btnHome = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnSettings = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnAbout = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	// 设置导航按钮样式
	buttonInfos = {
	{btnHome, ":/Imgs/home.svg", ":/Imgs/home_active.svg", homePage},
	{btnSettings, ":/Imgs/settings.svg", ":/Imgs/settings_active.svg", settingsPage},
	{btnAbout, ":/Imgs/about.svg", ":/Imgs/about_active.svg", aboutPage}
	};
	// 设置统一样式和连接信号
	buttonInfos[stackedWidget->currentIndex()].button->setBtnChecked(true); // 设置当前页面按钮为选中状态
	for (const ButtonInfo& info : buttonInfos)
	{
		CustomToolButton* btn = info.button;
		btn->setSvgIcons(info.normalIcon, info.activeIcon);
		btn->setFixedSize(QSize(naviWidth - 4, naviWidth - 4));
		// 连接信号，捕获图标路径
		connect(btn, &QToolButton::clicked, [btn, stackedWidget, this]()
			{
				// 禁用所有按钮，防止视觉反馈+误点
				for (ButtonInfo& infos : buttonInfos)
					infos.button->setEnabled(false);

				if (stackedWidget->isAnimationRunning()) return;

				// 更新UI
				QWidget* nextPage = nullptr;
				btn->setBtnChecked(true);
				for (ButtonInfo& infos : buttonInfos)
				{
					if (infos.button == btn)
						nextPage = infos.page;
					else
						infos.button->setBtnChecked(false);
				}

				// 页面切换 + 结束后恢复按钮
				stackedWidget->slideFromBottomToTop(nextPage, 150, [this]()
					{
						for (ButtonInfo& infos : buttonInfos)
							infos.button->setEnabled(true);
					});
			});
	}
	//  布局
	naviLay->addSpacing(28);
	naviLay->addWidget(avatar, 0, Qt::AlignHCenter);
	naviLay->addSpacing(16);
	naviLay->addWidget(btnHome, 0, Qt::AlignHCenter);
	naviLay->addWidget(btnSettings, 0, Qt::AlignHCenter);
	naviLay->addWidget(btnAbout, 0, Qt::AlignHCenter);
	naviLay->addStretch();

	// 初始化全局管理器
	AntMessageManager::instance();	// 全局消息
	AntTooltipManager::instance();	// 全局提示

	// 初始化通知管理器
	NotificationManager::instance()->getMainWindow(ui.main_widget);

	// 调整消息框位置
	connect(this, &QtAntDesign::resized, this, [=](int w, int h)
		{
			NotificationManager::instance()->relayoutNotifications(w, h);
		});

	// 对话框
	DialogViewController* mDialog = new DialogViewController(avatar->loginState(), w, h, ui.main_widget);	// 实际登录状态要服务器给予
	avatar->addDialog(mDialog);
	connect(this, &QtAntDesign::resized, mDialog, &DialogViewController::getParentSize);
	connect(mDialog, &DialogViewController::successLogin, avatar, &CircularAvatar::allowLogin);

	// 信号连接
	connect(btnMax, &QToolButton::clicked, this, [this]()
		{
			// 每次使用时重新获取窗口句柄，可以确保你拿到的是当前最新、有效的窗口句柄。避免使用缓存失效的句柄造成崩溃。
			HWND hwnd = reinterpret_cast<HWND>(winId());

			// 如果当前最大化，点击恢复；否则最大化
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &wp);

			if (wp.showCmd == SW_MAXIMIZE)
			{
				ShowWindow(hwnd, SW_RESTORE);
				btnMax->setIcon(QIcon(":/Imgs/Maximize-1.svg"));
			}
			else
			{
				ShowWindow(hwnd, SW_MAXIMIZE);
				btnMax->setIcon(QIcon(":/Imgs/Maximize-3.svg"));
			}
		});

	connect(btnMin, &QToolButton::clicked, this, [this]()
		{
			HWND hwnd = reinterpret_cast<HWND>(winId());
			ShowWindow(hwnd, SW_MINIMIZE);
		});

	connect(btnClose, &QToolButton::clicked, this, [this, w, h]()
		{
			emit showStandardDialog(ui.main_widget->width(), ui.main_widget->height(), w / 3, h / 4, "TITLE", "是否关闭应用程序?");
		});

	connect(this, &QtAntDesign::showStandardDialog, mDialog, &DialogViewController::buildStandardDialog);

	// 主页的信号连接
	connect(this, &QtAntDesign::resized, homePage, &HomePage::resized);
	connect(this, &QtAntDesign::windowMoved, homePage, &HomePage::windowMoved);
}

QtAntDesign::~QtAntDesign()
{
}

void QtAntDesign::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	// 调整对话框尺寸
	emit resized(ui.main_widget->width(), ui.main_widget->height());
}

bool QtAntDesign::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
	MSG* msg = static_cast<MSG*>(message);

	switch (msg->message) {
	case WM_NCCALCSIZE: {
		// 去掉非客户区，让客户区占满整个窗口（无边框）
		if (msg->wParam) {
			*result = 0;
			return true;
		}
		break;
	}
	case WM_NCHITTEST: {
		// 处理拖拽和缩放区域
		const LONG borderWidth = 8; // 拖拽缩放边框宽度
		RECT winRect;
		GetWindowRect(HWND(winId()), &winRect);

		// 获取鼠标全局坐标
		const LONG x = GET_X_LPARAM(msg->lParam);
		const LONG y = GET_Y_LPARAM(msg->lParam);

		// 允许缩放的条件（根据窗口最小最大宽高判断）
		const bool canResizeWidth = minimumWidth() != maximumWidth();
		const bool canResizeHeight = minimumHeight() != maximumHeight();

		// 左上角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.left && x < winRect.left + borderWidth &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOPLEFT;
			return true;
		}
		// 右上角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.right - borderWidth && x < winRect.right &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOPRIGHT;
			return true;
		}
		// 左下角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.left && x < winRect.left + borderWidth &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOMLEFT;
			return true;
		}
		// 右下角
		if (canResizeWidth && canResizeHeight &&
			x >= winRect.right - borderWidth && x < winRect.right &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOMRIGHT;
			return true;
		}
		// 左边
		if (canResizeWidth &&
			x >= winRect.left && x < winRect.left + borderWidth) {
			*result = HTLEFT;
			return true;
		}
		// 右边
		if (canResizeWidth &&
			x >= winRect.right - borderWidth && x < winRect.right) {
			*result = HTRIGHT;
			return true;
		}
		// 上边
		if (canResizeHeight &&
			y >= winRect.top && y < winRect.top + borderWidth) {
			*result = HTTOP;
			return true;
		}
		// 下边
		if (canResizeHeight &&
			y >= winRect.bottom - borderWidth && y < winRect.bottom) {
			*result = HTBOTTOM;
			return true;
		}

		// 设置标题栏拖动区域 只有该区域内才允许拖动窗口
		if (x > winRect.left + m_naviWidth && x < winRect.right - m_widgetTotalWidth
			&& y > winRect.top && y < winRect.top + m_titleBarHeight)
		{
			*result = HTCAPTION;
			return true;
		}

		// 其余地方交给默认处理
		break;
	}
	default:
		break;
	}
#endif // Q_OS_WIN
	return QWidget::nativeEvent(eventType, message, result);
}

void QtAntDesign::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	m_hwnd = reinterpret_cast<HWND>(winId());
	if (!m_hwnd) return;

	LONG style = GetWindowLong(m_hwnd, GWL_STYLE);

	// 非客户区被隐藏了 但是他还有标题栏、有边框以及最大化最小化功能的标准窗口从而提供windows原生动画和交互
	style |= WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	SetWindowLong(m_hwnd, GWL_STYLE, style);

	MARGINS margins = { 5,5,5,5 };
	DwmExtendFrameIntoClientArea(m_hwnd, &margins);

	DWORD cornerPref = 2;
	DwmSetWindowAttribute(m_hwnd, 33, &cornerPref, sizeof(cornerPref));
}

void QtAntDesign::moveEvent(QMoveEvent* event)
{
	QWidget::moveEvent(event);
	// 发送窗口左上角全局坐标
	emit windowMoved(this->mapToGlobal(QPoint(0, 0)));
}