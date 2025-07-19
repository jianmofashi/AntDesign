#pragma once
#include <QWidget>
#include <QColor>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QList>
#include <QMap>
#include <QStandardItemModel>
#include "DesignSystem.h"
#include "PopupViewController.h"
#include "TransparentMask.h"

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
	TransparentMask* getMask() { return mask; }
protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
private:
	// 恢复初始状态
	void resetState();
signals:
	void resized(int width, int height); 
public:
	QList<PopupViewController*> m_popups; // 用于存储所有弹出框
private:
	QString m_text;
	QString m_firstLevelSelectedText;
	QColor m_borderColor;
	QSvgRenderer* m_arrowRenderer;
	TransparentMask* mask = nullptr;
	PopupViewController* m_popup1 = nullptr;
	PopupViewController* m_popup2 = nullptr;
	QMap<QString, QStandardItemModel*> m_subModels;	// 全部二级列表模型

	bool m_isPressed;
	bool m_isChangeTextColor;
	bool m_enableMultiLevel = false;
};
