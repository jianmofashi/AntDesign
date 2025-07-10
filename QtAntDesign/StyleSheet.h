#pragma once
#include <QString>
#include <QColor>

namespace StyleSheet
{
	inline QString mainQss()
	{
		return "#main_widget{ "
			"border-top-left-radius: 8px; "		// 左上
			"border-top-right-radius: 8px; "	// 右上角圆角
			"border-bottom-left-radius: 8px;"	// 左下
			"border-bottom-right-radius: 8px;"  // 右下
			"background-color: white;}";
	}

	inline QString naviQss()
	{
		return R"(#navi_widget{background-color: rgb(245,245,245);
					border-top-left-radius: 8px;
					border-bottom-left-radius: 8px;})";
	}

	inline QString titleBarQss()
	{
		return R"(#titleBar{background-color: rgb(255,255,255);
					border-top-right-radius: 8px;})";
	}

	inline QString centralQss()
	{
		return R"(#central{background-color: rgb(255,255,255);
					border-bottom-right-radius: 8px;})";
	}

	inline QString gradientQss()
	{
		return R"(
		#gradientWidget {
		background: qlineargradient(
        x1: 0, y1: 0,
        x2: 1, y2: 1,
        stop: 0 #dce8ff,
        stop: 0.4 #e8e6ff,
        stop: 1 #f3e7fe
    );
		border-radius: 12px;})";
	}

	inline QString antMsgQss()
	{
		return R"(#AntMsg{background-color: gray;
					border-radius: 8px;})";
	}

	inline QString signInBtnQss()
	{
		return R"(
        #signInButton {
            background-color: #d0e9c6; /* 非常浅的薄荷绿 */
            border: none; /* 无边框 */
            border-radius: 6px; /* 现代圆角 */
            color: #388e3c; /* 深绿色文字 */
        }
        #signInButton:hover {
            background-color: #a5d6a7; /* 悬停时稍深的绿色 */
        }
        #signInButton:pressed {
            background-color: #81c784; /* 按压状态更深的绿色 */
        }
    )";
	}

	inline QString errorBtnQss()
	{
		return R"(/* 错误按钮 (红色系) */
					#errorButton {
						background-color: #E74C3C; /* 扁平红色 */
						border-color: #C0392B;
					}
					#errorButton:hover {
						background-color: #CB4335;
						border-color: #A93226;
					}
					#errorButton:pressed {
						background-color: #B03A2E;
					})";
	}

	inline QString infoBtnQss()
	{
		return R"(/* 信息按钮 (蓝色系 - 可以用基础样式，或略作调整) */
					#infoButton {
						background-color: #3498DB; /* 默认蓝色，也可以选一个特定信息蓝 */
						border-color: #2D709C;
					}
					#infoButton:hover {
						background-color: #2980B9;
						border-color: #2471A3;
					}
					#infoButton:pressed {
						background-color: #1F618D;
					})";
	}

	inline QString warningBtnQss()
	{
		return R"(/* 警告按钮 (橙色/黄色系) */
					#warningButton {
						background-color: #F39C12; /* 扁平橙色 */
						border-color: #D35400;
					}
					#warningButton:hover {
						background-color: #E67E22;
						border-color: #BA4A00;
					}
					#warningButton:pressed {
						background-color: #CA6F1E;
					})";
	}

	inline QString logoutBtnQss()
	{
		return R"(
        #logoutButton {
			color: rgb(80,80,80);
            background-color: #e2e6eb; /* 柔和的浅灰色 */
            border: none;
            border-radius: 6px;
        }
        #logoutButton:hover {
            background-color: #cbd5e0; /* 中等浅灰 */
        }
        #logoutButton:pressed {
            background-color: #a0aec0; /* 中灰色 */
        })";
	}

	inline QString myStackedQss()
	{
		return R"(#myStacked{background-color: rgb(255,255,255);
					border-radius: 8px;})";
	}

	inline QString lineEditQss(const QColor& borderColor)
	{
		return QString(R"(
        QLineEdit {
            border: 2px solid rgb(%1, %2, %3);
            border-radius: 5px;
            padding-top: 12px;
        }
    )").arg(borderColor.red()).arg(borderColor.green()).arg(borderColor.blue());
	}

	inline QString noBorderBtnQss(const QColor& color)
	{
		return QString(R"(
        QPushButton {
            border: none;
			background-color: transparent;
            color: rgb(%1, %2, %3);
        })").arg(color.red()).arg(color.green()).arg(color.blue());
	}

	inline QString titleBottomLineQss()
	{
		return QString(R"(
        #dialogSubTitle {
            border: none;
            border-bottom: 2px solid rgb(200,200,200); })");
	}

	inline QString widgetLeftLineQss()
	{
		return QString(R"(
        #rightWidget {
            border: none;
            border-left: 2px solid rgb(200,200,200); })");
	}

	// 水平方向的标签Qss
	inline QString hTabQss()
	{
		return QString(R"(
		 QTabWidget::pane {
			border: none;          /* 去除tabWidget面板边框 */
		}
		QTabBar {
			border: none;          /* 去除tabBar边框 */
			padding-left: 0px;
			padding-right: 0px;
		}
		QTabBar::tab {
			border: none;
			background: transparent;
		}
		QTabBar::tab:hover {
					color: #1890FF; /* Ant Design 蓝色 */
        }
		QTabBar::tab:selected {
            color: #1890FF; /* 选中标签蓝色 */
        })");
	}

	inline QString toolBtnQss(const QColor& defaultColor = QColor("#BFBFBF"), const QColor& hoverColor = QColor("#1677FF"))
	{
		return QString(R"(
        QToolButton {
            color: %1;
            background-color: transparent;
            border: none;
        }
        QToolButton:hover {
            color: %2;
        } )").arg(defaultColor.name(), hoverColor.name());
	}

	inline QString standardDialogQss()
	{
		return "#standardDialog{ "
			"border-radius: 8px; "
			"background-color: white;}";
	}

	inline QString standardDialogBtnQss()
	{
		return R"(
        QPushButton#CancelButton {
            background-color: white;
            color: black;
            border-radius: 4px;
			border:1px solid darkGray;
        }

        QPushButton#CancelButton:hover {
            color: #1677ff;
			background-color: white;
			border:1px solid #1677ff;
        }

        QPushButton#ConfirmButton {
            background-color: #1677ff;
            color: white;
            border: none;
            border-radius: 4px;
        }

        QPushButton#ConfirmButton:hover {
            background-color: #4096ff;
        })";
	}
}
