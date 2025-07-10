#pragma once

#include <QWidget>
#include <QPushbutton>

class RegisterPageWidget : public QWidget
{
	Q_OBJECT

public:
	RegisterPageWidget(QWidget* parent = nullptr);
	~RegisterPageWidget();
public:
	QPushButton* loginButton;
signals:
	void registerSuccess();
};
