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

QtAntDesign::QtAntDesign(QWidget* parent)
	: QWidget(parent),
	m_shadowRadius(15),
	m_offset(0, 0),
	m_color(150, 150, 150, 38),
	m_cornerRadius(10)
{
	ui.setupUi(this);

	setObjectName("QtAntDesign");
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setMinimumSize(1100, 750);

	ui.main_widget->setStyleSheet(StyleSheet::mainQss());

	// 获取主屏幕分辨率
	int w = 0, h = 0;
	QScreen* screen = QGuiApplication::primaryScreen();
	if (screen)
	{
		QSize screenSize = screen->availableGeometry().size();  // 可用屏幕大小，不包括任务栏
		w = int(screenSize.width() * 0.51);  // % 宽度
		h = int(screenSize.height() * 0.62); // % 高度
		resize(w, h);
	}
	setContentsMargins(15, 15, 15, 15);

	// 任务栏 内容区域 导航栏 布局调整
	ui.navi_widget->setFixedWidth(60); // 希望的宽度
	ui.navi_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);	// 水平方向缩放策略固定
	ui.navi_widget->setStyleSheet(StyleSheet::naviQss());
	ui.titleBar->setStyleSheet(StyleSheet::titleBarQss());
	ui.central->setStyleSheet(StyleSheet::centralQss());
	ui.titleBar->setFixedHeight(50);

	// 开启悬浮事件处理鼠标边界样式变化
	setAttribute(Qt::WA_Hover, true);
	installEventFilter(this);

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
	btnMin->setFixedSize(36, 36);
	btnMax->setFixedSize(36, 36);
	btnClose->setFixedSize(36, 36);
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
	HomePage* homePage = new HomePage(stackedWidget);
	SettingsPage* settingsPage = new SettingsPage(stackedWidget);
	AboutPage* aboutPage = new AboutPage(stackedWidget);
	stackedWidget->addWidget(homePage);
	stackedWidget->addWidget(settingsPage);
	stackedWidget->addWidget(aboutPage);
	// 导航按钮
	const int naviWidth = ui.navi_widget->width();  // 动态获取导航栏宽度
	const double iconSizeRatio = 0.56;  // 图标占按钮的比例
	const int buttonSize = naviWidth;
	const int iconSize = static_cast<int>(buttonSize * iconSizeRatio);
	CustomToolButton* btnHome = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnSettings = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	CustomToolButton* btnAbout = new CustomToolButton(QSize(iconSize, iconSize), ui.navi_widget);
	// 设置统一样式
	buttonInfos = {
	{btnHome, ":/Imgs/home.svg", ":/Imgs/home_active.svg", homePage},
	{btnSettings, ":/Imgs/settings.svg", ":/Imgs/settings_active.svg", settingsPage},
	{btnAbout, ":/Imgs/about.svg", ":/Imgs/about_active.svg", aboutPage}
	};
	// 设置统一样式和连接信号
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

	// 初始化全局消息管理器
	AntMessageManager::instance()->getMainWindow(ui.main_widget);	// 全局消息
	AntTooltipManager::instance()->getMainWindow(ui.main_widget);	// 全局提示

	// 对话框
	DialogViewController* mDialog = new DialogViewController(avatar->loginState(), w, h, ui.main_widget);	// 实际登录状态要服务器给予
	avatar->addDialog(mDialog);
	connect(this, &QtAntDesign::resized, mDialog, &DialogViewController::getParentSize);
	connect(mDialog, &DialogViewController::successLogin, avatar, &CircularAvatar::allowLogin);

	// 窗口动画
	initWindowAnimation();

	// 信号连接
	connect(btnMax, &QToolButton::clicked, this, [this]()
		{
			onMaximizeButtonClicked();
		});

	connect(btnMin, &QToolButton::clicked, this, [this]()
		{
			playMiniAnim();
		});

	connect(btnClose, &QToolButton::clicked, this, [this, w, h]()
		{
			emit showStandardDialog(ui.main_widget->width(), ui.main_widget->height(), w / 3, h / 4, "TITLE", "是否关闭应用程序?");
		});

	connect(this, &QtAntDesign::showStandardDialog, mDialog, &DialogViewController::buildStandardDialog);
}

QtAntDesign::~QtAntDesign()
{
}

void QtAntDesign::initWindowAnimation()
{
	// 最大化缩放
	maxScaleAnim = new QPropertyAnimation(this, "geometry");
	maxScaleAnim->setDuration(300);
	maxScaleAnim->setEasingCurve(QEasingCurve::InOutSine);
	// 最小化缩放
	miniScaleAnim = new QPropertyAnimation(this, "geometry");
	miniScaleAnim->setDuration(300);
	miniScaleAnim->setEasingCurve(QEasingCurve::InOutSine);

	connect(maxScaleAnim, &QPropertyAnimation::finished, this, [this]()
		{
			// 完成最大化窗口动画
			if (maxScaleAnim->direction() == QAbstractAnimation::Forward)
			{
				// 更新按钮图标
				btnMax->setIcon(QIcon(":/Imgs/Maximize-3.svg"));
				showMaximized();
			}
			// 完成还原窗口动画
			else
			{
				// 更新按钮图标
				btnMax->setIcon(QIcon(":/Imgs/Maximize-1.svg"));
				showNormal();
			}

			m_disableShadow = false;

			// 模拟 resizeEvent 以更新阴影缓存
			QResizeEvent event(size(), size());
			resizeEvent(&event);

			// 触发窗口重绘，让 paintEvent 执行绘制阴影
			update();
		});

	connect(miniScaleAnim, &QPropertyAnimation::finished, this, [this]()
		{
			setMinimumSize(1100, 750);
			m_disableShadow = false;

			// 第一步：先最小化，窗口立即隐藏
			showMinimized();
			// 因为他在屏幕外面 因此要把它拉回来
			setGeometry(m_windowStartRect);
		});
}

void QtAntDesign::playMiniAnim()
{
	if (isMinimized()) return;

	m_beforeMax = (windowState() & Qt::WindowMaximized);	//判断当前是否是最大化状态
	m_disableShadow = true;

	setMinimumSize(0, 0);
	qreal scaleFactor = 0.3; // 缩小比例
	QRect availableGeometry = screen()->availableGeometry();
	m_windowStartRect = geometry();
	QRect startRect = geometry();

	int endWidth = startRect.width() * scaleFactor;
	int endHeight = startRect.height() * scaleFactor;

	int endX = availableGeometry.center().x() - endWidth / 2;
	int endY = availableGeometry.bottom(); // 屏幕底部

	QRect endRect(endX, endY, endWidth, endHeight);

	miniScaleAnim->stop();
	miniScaleAnim->setStartValue(startRect);
	miniScaleAnim->setEndValue(endRect);
	miniScaleAnim->start();
}

// 你的最大化按钮的槽函数
void QtAntDesign::onMaximizeButtonClicked()
{
	// 如果当前已经是最大化，则还原。否则，请求最大化。
	if (isMaximized())
	{
		playRstoreAnim();
	}
	else
	{
		playMaxAnim();
	}
}

void QtAntDesign::playMaxAnim()
{
	if (isMaximized()) return;

	// 获取当前窗口和屏幕信息
	QRect startRect = this->geometry();
	QScreen* scr = this->screen();
	if (!scr) return;

	QRect availableGeometry = scr->availableGeometry(); // 不含任务栏

	// 以当前中心点为中心，构造最大化目标矩形
	QRect endRect = availableGeometry; // 左上角对齐，大小等于屏幕可用区域

	// 禁用阴影防止他使缩放动画抖动
	m_disableShadow = true;

	// 动画配置
	maxScaleAnim->setDirection(QAbstractAnimation::Forward);
	maxScaleAnim->stop();
	maxScaleAnim->setStartValue(startRect);
	maxScaleAnim->setEndValue(endRect);
	maxScaleAnim->start();
}

void QtAntDesign::playRstoreAnim()
{
	maxScaleAnim->stop();
	maxScaleAnim->setDirection(QAbstractAnimation::Backward);
	maxScaleAnim->start();
}

ResizeRegion QtAntDesign::hitTest(const QPoint& pos)
{
	const int broderWidth = 20; // 边界宽度
	int x = pos.x();
	int y = pos.y();
	int width = this->width();
	int height = this->height();

	bool onLeft = (x >= 0 && x < broderWidth);
	bool onRight = (x > width - broderWidth && x < width);
	bool onTop = (y >= 0 && y < broderWidth);
	bool onBottom = (y > height - broderWidth && y < height);

	if (onLeft && onTop) return TopLeft;
	if (onLeft && onBottom) return BottomLeft;
	if (onRight && onTop) return TopRight;
	if (onRight && onBottom) return BottomRight;
	if (onLeft) return Left;
	if (onRight) return Right;
	if (onTop) return Top;
	if (onBottom) return Bottom;

	return None;
}

void QtAntDesign::adjustCursorStyle(ResizeRegion region)
{
	switch (region)
	{
	case TopLeft:
	case BottomRight: setCursor(Qt::SizeFDiagCursor); break;
	case TopRight:
	case BottomLeft: setCursor(Qt::SizeBDiagCursor); break;
	case Left:
	case Right: setCursor(Qt::SizeHorCursor); break;
	case Top:
	case Bottom: setCursor(Qt::SizeVerCursor); break;
	default: setCursor(Qt::ArrowCursor); break;
	}
}

void QtAntDesign::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		// 保存初始几何状态
		m_initialGeometry = geometry();
		m_isLockCursor = true;
		m_resizeRegion = hitTest(e->pos());
		adjustCursorStyle(m_resizeRegion);
		m_dragStartPos = e->globalPosition().toPoint();

		// 标题栏拖动逻辑
		if (m_resizeRegion == None && e->pos().y() < ui.titleBar->height())
		{
			dragPos = e->globalPosition().toPoint() - frameGeometry().topLeft();
		}
	}
}

void QtAntDesign::mouseMoveEvent(QMouseEvent* e)
{
	// 处理窗口移动
	if ((e->buttons() & Qt::LeftButton) &&
		e->pos().y() < ui.titleBar->height() &&
		m_resizeRegion == None)
	{
		move(e->globalPosition().toPoint() - dragPos);
		return;
	}
	// 处理窗口缩放
	if ((e->buttons() & Qt::LeftButton) && m_resizeRegion != None)
	{
		QPoint offset = e->globalPosition().toPoint() - m_dragStartPos;
		QRect newGeom = m_initialGeometry;

		switch (m_resizeRegion)
		{
		case Left:
			newGeom.setLeft(newGeom.left() + offset.x());
			break;
		case Right:
			newGeom.setRight(newGeom.right() + offset.x());
			break;
		case Top:
			newGeom.setTop(newGeom.top() + offset.y());
			break;
		case Bottom:
			newGeom.setBottom(newGeom.bottom() + offset.y());
			break;
		case TopLeft:
			newGeom.setTopLeft(newGeom.topLeft() + offset);
			break;
		case TopRight:
			newGeom.setTopRight(newGeom.topRight() + offset);
			break;
		case BottomLeft:
			newGeom.setBottomLeft(newGeom.bottomLeft() + offset);
			break;
		case BottomRight:
			newGeom.setBottomRight(newGeom.bottomRight() + offset);
			break;
		default:
			break;
		}

		// 应用最小尺寸限制（带方向修正）
		if (newGeom.width() < minimumWidth())
		{
			if (m_resizeRegion == Left || m_resizeRegion == TopLeft || m_resizeRegion == BottomLeft)
			{
				newGeom.setLeft(newGeom.right() - minimumWidth());
			}
			else
			{
				newGeom.setWidth(minimumWidth());
			}
		}

		if (newGeom.height() < minimumHeight())
		{
			if (m_resizeRegion == Top || m_resizeRegion == TopLeft || m_resizeRegion == TopRight)
			{
				newGeom.setTop(newGeom.bottom() - minimumHeight());
			}
			else
			{
				newGeom.setHeight(minimumHeight());
			}
		}

		setGeometry(newGeom);
	}
}

void QtAntDesign::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) // 只处理左键释放
	{
		m_isLockCursor = false;
		ResizeRegion region = hitTest(event->pos());
		adjustCursorStyle(region);
	}
}

void QtAntDesign::paintEvent(QPaintEvent*)
{
	if (!m_disableShadow)
	{
		QPainter p(this);
		p.drawPixmap(0, 0, m_shadowCache);
	}
}

bool QtAntDesign::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::HoverMove && !m_isLockCursor)
	{
		QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
		QPoint pos = hoverEvent->position().toPoint();
		ResizeRegion region = hitTest(pos);
		adjustCursorStyle(region);
		return true; // 表示事件已处理
	}

	return QObject::eventFilter(obj, event);
}

void QtAntDesign::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if (!m_disableShadow)
	{
		updateShadowCache();
		// 调整对话框尺寸
		emit resized(ui.main_widget->width(), ui.main_widget->height());
	}
}

bool QtAntDesign::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
	{
		MSG* msg = reinterpret_cast<MSG*>(message);
		if (msg->message == WM_WINDOWPOSCHANGING)
		{
			WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(msg->lParam);
			if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
			{
				// 加上 SWP_NOCOPYBITS，减少抖动
				wp->flags |= SWP_NOCOPYBITS;

				// 调用系统默认窗口过程，得到默认结果
				*result = ::DefWindowProcW(msg->hwnd, msg->message, msg->wParam, msg->lParam);

				// 返回 1 表示消息已处理，Qt 不会再处理
				return true;
			}
			// 没满足条件时返回 0 表示未处理，让 Qt 处理
			*result = 0;
			return false;
		}
	}
	return QWidget::nativeEvent(eventType, message, result);
}

void QtAntDesign::changeEvent(QEvent* event)
{
	QWidget::changeEvent(event);

	if (event->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent* stateEvent = static_cast<QWindowStateChangeEvent*>(event);

		bool wasMinimized = (stateEvent->oldState() & Qt::WindowMinimized);
		bool isNowVisible = !(windowState() & Qt::WindowMinimized);

		if (wasMinimized && isNowVisible)
		{
			// 最小化之前是最大化的窗口状态
			if (m_beforeMax)
			{
				setWindowState(Qt::WindowMaximized);
				m_beforeMax = false;
			}
		}
	}
}

void QtAntDesign::updateShadowCache()
{
	if (width() <= 0 || height() <= 0) return;

	QPixmap pixmap(size());
	pixmap.fill(Qt::transparent);

	QPainter p(&pixmap);
	p.setRenderHint(QPainter::Antialiasing);

	for (int i = m_shadowRadius; i >= 1; --i)
	{
		QColor c = m_color;
		double factor = double(i) / m_shadowRadius;
		c.setAlphaF(m_color.alphaF() * factor * factor);
		p.setPen(Qt::NoPen);
		p.setBrush(c);

		QRectF r = rect().adjusted(i, i, -i, -i);
		r.translate(m_offset);

		p.drawRoundedRect(r, m_cornerRadius, m_cornerRadius);
	}

	m_shadowCache = pixmap;
}