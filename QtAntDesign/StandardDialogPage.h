#pragma once

#include <QWidget>
#include <QLabel>

class StandardDialogPage : public QWidget
{
	Q_OBJECT

public:
	StandardDialogPage(QWidget* parent);
	~StandardDialogPage();

	void setText(QString title, QString text);
signals:
	void exitDialog();
private:
	QLabel* titleLabel;
	QLabel* contentLabel;
};
