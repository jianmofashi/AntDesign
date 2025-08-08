#pragma once

#include <QWidget>
#include <QPushButton>

class LoginPageWidget : public QWidget
{
	Q_OBJECT

public:
	LoginPageWidget(QWidget* parent = nullptr);
	~LoginPageWidget();

	void updateTheme();
public:
	QPushButton* registerButton;
signals:
	void loginSuccess();
private:
	QWidget* titleWidget;
	QPushButton* subBtn2;
};
