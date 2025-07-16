#pragma once
#include <QWidget>
#include <QColor>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QList>
#include <QMap>
#include <QStandardItemModel>
#include "DesignSystem.h"
#include "PopupWidget.h"
#include "ComboBoxMask.h"

class AntComboBox : public QWidget
{
	Q_OBJECT
public:
	// enableMultiLevel 是否启用多级列表 poupHeight 弹出框高度
	explicit AntComboBox(QString showText, QStringList itemTextList, QWidget* parent = nullptr, int popupHeight = 240, bool enableMultiLevel = false,
		QMap<QString, QStringList> subItemMap = QMap<QString, QStringList>());

	~AntComboBox() override;

	void setCurrentText(const QString& text);
	// 设置是否启用多级列表
	void setEnableMultiLevel(bool enable);
	ComboBoxMask* getMask() { return mask; }
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	// 恢复初始状态
	void resetState();
public:
	QList<PopupWidget*> m_popups; // 用于存储所有弹出框
private:
	QString m_text;
	QString m_firstLevelSelectedText;
	QColor m_borderColor;
	QSvgRenderer* m_arrowRenderer;
	ComboBoxMask* mask = nullptr;
	PopupWidget* m_popup1 = nullptr;
	PopupWidget* m_popup2 = nullptr;
	QMap<QString, QStandardItemModel*> m_subModels;	// 全部二级列表模型

	bool m_isPressed;
	bool m_isChangeTextColor;
	bool m_enableMultiLevel = false;
};
