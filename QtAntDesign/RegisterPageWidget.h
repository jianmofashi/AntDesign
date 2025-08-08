#pragma once

#include <QWidget>
#include <QPushbutton>

class RegisterPageWidget : public QWidget
{
	Q_OBJECT

public:
	RegisterPageWidget(QWidget* parent = nullptr);
	~RegisterPageWidget();

	void updateTheme();
public:
	QPushButton* loginButton;
signals:
	void registerSuccess();
private:
	QWidget* titleWidget;
};
