#include "PopupWidget.h"
#include "StyleSheet.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>

PopupWidget::PopupWidget(int height, bool enableMultiLevel, QWidget* parent)
	: QListView(parent),
	m_enableMultiLevel(enableMultiLevel),
	m_popupHeight(height)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	auto theme = DesignSystem::instance()->currentTheme();
	setStyleSheet(StyleSheet::vListViewQss(theme.popupScrollBarColor));
	setStyle(new ListViewStyle(theme.popupBgColor, theme.popupScrollBarColor, theme.shadowColor, style()));

	// 自定义 Item Delegate
	setItemDelegate(new ListItemDelegate(36, enableMultiLevel, this));

	QFont font = this->font();
	font.setPointSize(11);   // 调大字体
	setFont(font);

	connect(this, &QListView::clicked, this, [this, enableMultiLevel](const QModelIndex& idx)
		{
			emit itemSelected(idx);
		});
}

void PopupWidget::setCurrentIndex(const QModelIndex& index)
{
	if (index.isValid())
	{
		setCurrentIndex(index);
	}
}