#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtAntDesign.h"
#include <QHoverEvent>
#include "CircularAvatar.h"
#include "CustomToolButton.h"
#include <QLabel>
#include <QTimer>

// 判断是否在windows平台
#ifdef Q_OS_WIN

#include <windows.h>
#include <dwmapi.h>
#include <windowsx.h>

#pragma comment(lib, "Dwmapi.lib")

#endif  // Q_OS_WIN

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
	void resizeEvent(QResizeEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
	void showEvent(QShowEvent* event) override;
	void moveEvent(QMoveEvent* event) override;
signals:
	void resized(int parentW, int parentH);
	void playMaximizeAnim();
	void showStandardDialog(int parentW, int parentH, int dialogW, int dialogH, QString title, QString text);
	void windowMoved(QPoint globalPos); // 窗口移动时发出信号
private:
	Ui::QtAntDesignClass ui;
	HWND m_hwnd;
	QPoint dragPos;
	int m_titleBarHeight = 54;	// 标题栏高度
	int m_naviWidth = 62;		// 导航栏宽度
	int m_widgetTotalWidth = 0;		// 标题栏上控件总宽度
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

	QRect m_minRect;
	QRect m_maxRect;
	QRect m_closeRect;
};
