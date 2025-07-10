#pragma once

#include <QWidget>
#include <QEvent>
#include <QStackedWidget>
#include "MaskWidget.h"
#include "LoginPageWidget.h"
#include "RegisterPageWidget.h"
#include "SlideStackedWidget.h"
#include "ProfilePage.h"
#include "StandardDialogPage.h"

class MaterialDialog : public QWidget
{
	Q_OBJECT

public:
	enum PageIndex
	{
		Login,
		Profile,
		Standard
	};

	MaterialDialog(bool loginState, QWidget* parent);
	~MaterialDialog();
public:
	void showIndexPage(PageIndex index);
	StandardDialogPage* standardDialog() { return standardPage; };
signals:
	void setStandardDialogText(QString title, QString text);
	void successLogin(bool loginState);
private:
	// 用户登录状态
	bool m_loginState = false;
	// 页面管理
	SlideStackedWidget* stackedWidget;
	LoginPageWidget* loginPage;
	RegisterPageWidget* registerPage;
	ProfilePage* profilePage;
	StandardDialogPage* standardPage;
};
