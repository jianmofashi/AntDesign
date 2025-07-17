#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsScale>
#include "PopupWidget.h"

class PopupViewController : public QGraphicsView
{
	Q_OBJECT

public:
	PopupViewController(int height, bool enableMultiLevel, QWidget* parent);
	~PopupViewController();
	void showAnimated(const QPoint& pos, int width);
	void hideAnimated();
	bool getVisible() { return m_isVisible; }
	void updateSize(int width, int height);
signals:
	void itemSelected(const QModelIndex& index);
public:
	QPoint m_offset;
	PopupWidget* popup = nullptr;
private:
	bool m_isVisible;
	int m_height;
	QGraphicsScene* scene = nullptr;
	QGraphicsProxyWidget* proxy = nullptr;
	QPropertyAnimation* scaleAnim = nullptr;
	QPropertyAnimation* opacityAnim = nullptr;
	QParallelAnimationGroup* groupAnim = nullptr;
	QGraphicsScale* scaleTransform = nullptr;
};
