#include "PopupWidget.h"
#include "StyleSheet.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>

PopupWidget::PopupWidget(int height, bool enableMultiLevel, QWidget* parent)
	: QWidget(parent),
	m_enableMultiLevel(enableMultiLevel),
	m_popupHeight(height)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	m_listView = new QListView(this);
	m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	auto theme = DesignSystem::instance()->currentTheme();
	m_listView->setStyleSheet(StyleSheet::vListViewQss(theme.popupBgColor, theme.popupScrollBarColor));

	// 自定义 Item Delegate
	m_listView->setItemDelegate(new ListItemDelegate(36, enableMultiLevel, this));

	// 布局
	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(margin, margin, margin, margin);
	layout->addWidget(m_listView);

	QFont font = m_listView->font();
	font.setPointSize(11);   // 调大字体
	m_listView->setFont(font);

	connect(m_listView, &QListView::clicked, this, [this, enableMultiLevel](const QModelIndex& idx)
		{
			emit itemSelected(idx);
		});
}

void PopupWidget::setModel(QAbstractItemModel* model)
{
	m_listView->setModel(model);
}

void PopupWidget::setCurrentIndex(const QModelIndex& index)
{
	if (index.isValid())
	{
		m_listView->setCurrentIndex(index);
	}
}