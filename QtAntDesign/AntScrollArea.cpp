#include "AntScrollArea.h"
#include "StyleSheet.h"
#include "DesignSystem.h"
#include <QScrollBar>

AntScrollArea::AntScrollArea(ScrollDirection direction, QWidget* parent)
	: QScrollArea(parent)
{
	setObjectName("AntScrollArea");

	// QScrollArea 会自动根据滚动区域尺寸 调整内容控件的尺寸
	setWidgetResizable(true);

	// 设置滚动策略
	setScrollPolicy(direction);

	// 样式表
	auto theme = DesignSystem::instance()->currentTheme();
	viewport()->setStyleSheet("background-color:transparent;");
	setStyleSheet(StyleSheet::antScrollAreaQss(theme.backgroundColor, theme.scrollAreaHandleColor, theme.scrollAreaHoverColor));
}

AntScrollArea::~AntScrollArea()
{
}

void AntScrollArea::addWidget(QWidget* w)
{
	setWidget(w);
}

void AntScrollArea::setScrollPolicy(int direction)
{
	setVerticalScrollBarPolicy(
		(direction & ScrollVertical) ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(
		(direction & ScrollHorizontal) ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}