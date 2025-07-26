#pragma once

#include <QWidget>

class HomePage : public QWidget
{
	Q_OBJECT

public:
	HomePage(QWidget* parent);
	~HomePage();

signals:
	void resized(int w, int h);				// 用于通知其他组件调整大小
	void windowMoved(QPoint globalPos);		// 窗口移动时发出信号
};
