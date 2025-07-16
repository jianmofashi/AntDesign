#pragma once

#include <QWidget>
#include <QListView>
#include <QPropertyAnimation>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include "DesignSystem.h"

// 自定义 Item Delegate 来控制 ListItem 的大小
class ListItemDelegate : public QStyledItemDelegate
{
public:
	explicit ListItemDelegate(int itemHieght = 36, bool enableMultiLevel = false, QObject* parent = nullptr)
		: QStyledItemDelegate(parent), m_itemHeight(itemHieght), m_enableMultiLevel(enableMultiLevel)
	{
		// 选中的背景颜色
		QColor color = DesignSystem::instance()->primaryColor();
		color.setAlpha(30);
		m_curIdxBgColor = color;
		// 获取默认的背景色
		m_bgColor = DesignSystem::instance()->currentTheme().popupItemBgColor;
		// 获取默认的文本颜色
		m_textColor = DesignSystem::instance()->currentTheme().popupTextColor;
	}

	// 重写 sizeHint 来指定每个 item 的大小
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		// 获取原始的建议大小
		QSize originalSize = QStyledItemDelegate::sizeHint(option, index);
		// 设置一个固定的、更大的高度，例如 36px。你可以根据需要调整。
		return QSize(originalSize.width(), m_itemHeight);
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		painter->save();

		QRectF rect = option.rect.adjusted(2, 2, -2, -2);
		int borderRadius = 6;

		const QListView* listView = qobject_cast<const QListView*>(option.widget);
		QModelIndex currentIndex = listView ? listView->currentIndex() : QModelIndex();

		bool isHovered = option.state & QStyle::State_MouseOver;
		bool isCurrent = index == currentIndex;

		// 画背景
		if (isCurrent) 
		{
			painter->setBrush(m_curIdxBgColor);
			painter->setPen(Qt::NoPen);
			painter->drawRoundedRect(rect, borderRadius, borderRadius);
		}
		else if (isHovered) 
		{
			painter->setBrush(m_bgColor);
			painter->setPen(Qt::NoPen);
			painter->drawRoundedRect(rect, borderRadius, borderRadius);
		}

		// 设置字体
		QFont font = option.font;
		if (isCurrent) {
			font.setBold(true);
		}
		painter->setFont(font);

		// 字体颜色
		painter->setPen(m_textColor);

		// 文本区域预留出右侧图标空间
		QRect textRect = option.rect.adjusted(10, 0, -10, 0);
		// 画文本
		QString text = index.data(Qt::DisplayRole).toString();
		int flags = m_enableMultiLevel ? (Qt::AlignVCenter | Qt::AlignLeft) : Qt::AlignCenter;
		painter->drawText(textRect, flags, text);

		// 画图标（右侧）
		if (m_enableMultiLevel)
		{
			QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
			if (!icon.isNull()) {
				int iconSize = 12;
				QRect iconRect(option.rect.right() - iconSize - 5,
					option.rect.top() + (option.rect.height() - iconSize) / 2,
					iconSize, iconSize);
				icon.paint(painter, iconRect);
			}
		}

		painter->restore();
	}

private:
	int m_itemHeight;			// 用于存储每个 item 的高度
	QColor m_bgColor;			// 默认的背景色
	QColor m_curIdxBgColor;		// 当前选中项的背景颜色
	QColor m_textColor;			// 文本颜色
	bool m_enableMultiLevel;	// 是否启用多级列表
};

class PopupWidget : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(int popupHeight READ popupHeight WRITE setPopupHeight)
public:
	explicit PopupWidget(int height, bool enableMultiLevel, QWidget* parent = nullptr);
	void setModel(QAbstractItemModel* model);
	void showAnimated(const QPoint& pos, int width);
	void hideAnimated();
	/* 属性存取器 */
	int  popupHeight() const { return height(); }
	void setPopupHeight(int h) { setFixedHeight(h); }
	// 添加这个函数用于设置当前项
	void setCurrentIndex(const QModelIndex& index);
	bool getVisible() { return m_isVisible; }
	void removeAppEventFilter();
signals:
	void itemSelected(const QModelIndex& index);
	void containsMousePos(bool isContains);
protected:
	void focusOutEvent(QFocusEvent* event) override;
public:
	QPoint m_offset;
private:
	QListView* m_listView;
	QPropertyAnimation* m_anim;
	bool m_isVisible;
	bool m_enableMultiLevel;	// 是否启用多级列表
	int m_popupHeight;			// 弹出框的高度
};
