#pragma once

#include <QWidget>
#include <QEvent>
#include "AntScrollArea.h"
#include "AntProfileTable.h"
#include "TabContentWidget.h"

class HomePage : public QWidget
{
	Q_OBJECT

public:
	HomePage(QWidget* parent);
	~HomePage();
private:
	void initViewPage();
signals:
	void resized(int w, int h);				// 用于通知其他组件调整大小
	void windowMoved(QPoint globalPos);		// 窗口移动时发出信号
private:
	AntScrollArea* scrollArea1 = nullptr;
	AntScrollArea* scrollArea2 = nullptr;
	QVector<AntProfileTable::TableColumnItems> rowItems;
	AntProfileTable* table = nullptr;
	QStandardItemModel* tableModel = nullptr;
};
