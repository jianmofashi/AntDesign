#include "MaterialDialog.h"
#include "StyleSheet.h"
#include <QLayout>
#include <QGraphicsOpacityEffect>
#include "AntMessageManager.h"

MaterialDialog::MaterialDialog(bool loginState, QWidget* parent)
	: QWidget(parent), m_loginState(loginState)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	// 字体
	QFont font;
	font.setPointSizeF(13);

	// 布局
	QVBoxLayout* bgLayout = new QVBoxLayout(this);
	bgLayout->setContentsMargins(0, 0, 0, 0);
	bgLayout->setSpacing(0);

	// 管理页面
	stackedWidget = new SlideStackedWidget(this);
	stackedWidget->setObjectName("myStacked");
	stackedWidget->setStyleSheet(StyleSheet::myStackedQss());
	bgLayout->addWidget(stackedWidget);

	// 登录页
	loginPage = new LoginPageWidget(stackedWidget);
	stackedWidget->addWidget(loginPage);

	// 注册页
	registerPage = new RegisterPageWidget(stackedWidget);
	stackedWidget->addWidget(registerPage);

	// 标准对话框页
	standardPage = new StandardDialogPage(stackedWidget);
	stackedWidget->addWidget(standardPage);

	connect(this, &MaterialDialog::setStandardDialogText, standardPage, &StandardDialogPage::setText);

	// 默认显示登录页
	stackedWidget->setCurrentWidget(loginPage);

	// 绑定注册登录按钮信号切换页面
	connect(loginPage->registerButton, &QPushButton::clicked, this, [this]()
		{
			// 登录页 → 注册页（向左滑动）
			stackedWidget->slideToPage(registerPage, SlideStackedWidget::RightToLeft, 300);
		});
	connect(registerPage->loginButton, &QPushButton::clicked, this, [this]()
		{
			// 注册页 → 登录页（向右滑动）
			stackedWidget->slideToPage(loginPage, SlideStackedWidget::LeftToRight, 300);
		});

	// 成功登录
	connect(loginPage, &LoginPageWidget::loginSuccess, this, [this]()
		{
			AntMessageManager::instance()->showMessage(AntMessage::Success, "成功登录");
			m_loginState = true;
			emit successLogin(m_loginState);
		});
	// 成功注册
	connect(registerPage, &RegisterPageWidget::registerSuccess, this, [this]()
		{
			// 注册页 → 登录页（向右滑动）
			AntMessageManager::instance()->showMessage(AntMessage::Success, "成功注册");
			stackedWidget->slideToPage(loginPage, SlideStackedWidget::LeftToRight, 300);
		});

	// 用户个人信息页面
	profilePage = new ProfilePage(this);
	stackedWidget->addWidget(profilePage);

	// 根据用户初始登录状态决定显示什么样的界面
	showIndexPage(loginState ? Login : Profile);
}

MaterialDialog::~MaterialDialog()
{
}

void MaterialDialog::showIndexPage(PageIndex index)
{
	switch (index)
	{
	case MaterialDialog::Login:
		stackedWidget->setCurrentWidget(loginPage);
		break;
	case MaterialDialog::Profile:
		stackedWidget->setCurrentWidget(profilePage);
		break;
	case MaterialDialog::Standard:
		stackedWidget->setCurrentWidget(standardPage);
		break;
	default:
		stackedWidget->setCurrentWidget(standardPage);
		break;
	}
}