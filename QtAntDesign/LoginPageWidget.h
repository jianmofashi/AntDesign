#pragma once

#include <QWidget>
#include <QPushButton>

class LoginPageWidget : public QWidget
{
	Q_OBJECT

public:
	LoginPageWidget(QWidget* parent = nullptr);
	~LoginPageWidget();
public:
	QPushButton* registerButton;
signals:
	void loginSuccess();
};
