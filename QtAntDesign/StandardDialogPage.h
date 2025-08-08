#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class StandardDialogPage : public QWidget
{
	Q_OBJECT

public:
	StandardDialogPage(QWidget* parent);
	~StandardDialogPage();

	void setText(QString title, QString text);
	void updateTheme();
signals:
	void exitDialog();
private:
	QLabel* titleLabel;
	QLabel* contentLabel;
	QPushButton* cancelBtn;
	QPushButton* confirmBtn;
};
