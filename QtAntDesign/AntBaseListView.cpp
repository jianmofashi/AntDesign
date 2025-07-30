#include "AntBaseListView.h"

AntBaseListView::AntBaseListView(QWidget* parent)
	: QListView(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	// 自定义样式
	setStyle(new AntBaseListViewStyle(style()));

	// 自定义 Item Delegate
	setItemDelegate(new AntBaseListItemDelegate(this));
}

AntBaseListView::~AntBaseListView()
{
}