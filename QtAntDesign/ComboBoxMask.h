#pragma once

#include <QWidget>

class ComboBoxMask : public QWidget
{
	Q_OBJECT
public:
	explicit ComboBoxMask(QWidget* parent);

signals:
	void clickedOutside(); // 点击遮罩区域发出信号

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event);
};
