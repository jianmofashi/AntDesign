#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtAntDesign.h"
#include <QHoverEvent>
#include "CircularAvatar.h"
#include "CustomToolButton.h"
#include <QLabel>
#include <QMainWindow>
#include <windows.h>

// 检测鼠标是否在边缘
enum ResizeRegion {
	None, Left, Right, Top, Bottom,
	TopLeft, TopRight, BottomLeft, BottomRight
};

class QtAntDesign : public QWidget
{
	Q_OBJECT
public:
	QtAntDesign(QWidget* parent = nullptr);
	~QtAntDesign();
protected:
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void paintEvent(QPaintEvent*) override;
	bool eventFilter(QObject* obj, QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
	void changeEvent(QEvent* event);
signals:
	void resized(int parentW, int parentH);
	void playMaximizeAnim();
	void showStandardDialog(int parentW, int parentH, int dialogW, int dialogH, QString title, QString text);
private:
	ResizeRegion hitTest(const QPoint& pos);
	void adjustCursorStyle(ResizeRegion region);
	void initWindowAnimation();
	// 最大化动画
	void playMaxAnim();
	// 还原动画
	void playRstoreAnim();
	// 最小化动画
	void playMiniAnim();
	void onMaximizeButtonClicked();
	// 缓存阴影图
	void updateShadowCache();
private:
	Ui::QtAntDesignClass ui;
	QPoint dragPos;
	QPixmap m_shadowCache;  // 缓存阴影图
	int     m_shadowRadius;    // 阴影厚度
	QPoint  m_offset;          // 阴影偏移
	QColor  m_color;           // 阴影颜色 & 初始 alpha
	qreal   m_cornerRadius;    // 圆角半径
	bool	m_disableShadow = false;
	bool	m_beforeMax = false;	// 最小化之前是最大化的窗口状态

	// 窗口缩放
	ResizeRegion m_resizeRegion = None;
	QPoint m_dragStartPos;
	QRect m_initialGeometry;
	bool m_isLockCursor = false;	// 锁定光标样式

	// 按钮数据结构
	struct ButtonInfo
	{
		CustomToolButton* button;
		QString normalIcon;
		QString activeIcon;
		QWidget* page;
	};
	QList<ButtonInfo> buttonInfos;

	// 窗口动画
	QPropertyAnimation* maxScaleAnim = nullptr;
	QPropertyAnimation* miniScaleAnim = nullptr;
	QRect m_windowStartRect;			// 动画开始前的位置
	// 最小化 最大化 关闭 按钮
	QToolButton* btnMin = nullptr;
	QToolButton* btnMax = nullptr;
	QToolButton* btnClose = nullptr;
};
