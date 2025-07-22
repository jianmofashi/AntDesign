// DesignSystem.h
#ifndef DESIGN_SYSTEM_H
#define DESIGN_SYSTEM_H

#include <QObject>
#include <QColor>
#include <QFont>

struct Theme
{
	// 样式颜色
	QColor primaryColor;
	QColor textColor;
	QColor backgroundColor;
	// 单选按钮相关颜色
	QColor borderColor;
	QColor borderColorHover;
	QColor disabledColor;
	// 全局消息相关颜色
	QColor msgTextColor;
	// 滑动条相关颜色
	QColor slideColor;
	QColor slideBgColor;
	QColor ringColor;
	QColor ringhoveredColor;
	// 开关按钮滑块颜色
	QColor toggleButtonColor;
	QColor toggleButtonBgColor;
	QColor noCheckedColor;
	// 提示框相关颜色
	QColor toolTipBgColor;
	QColor toolTipTextColor;
	// 用户信息气泡颜色
	QColor userBubbleBgColor;
	// 自定义图标按钮颜色
	QColor toolBtnBgColor;
	// 自定义对话框颜色
	QColor dialogBgColor;
	// 进度条相关颜色
	QColor progressBarBgColor;
	// tab相关颜色
	QColor tabTextColor;
	// line edit 颜色
	QColor lineEditBorderColor;
	QColor lineEditHintColor;
	// 多选按钮相关颜色
	QColor checkBoxBgColor;
	QColor checkBoxBorderEnableColor;
	QColor checkBoxBorderDisableColor;
	QColor checkBoxTextColor;
	QColor checkBoxTextDisableColor;
	// 下拉弹出框相关颜色
	QColor popupBgColor;
	QColor popupScrollBarColor;
	QColor popupItemBgColor;
	QColor popupTextColor;
	// 滚动区相关颜色
	QColor scrollAreaHandleColor;
	QColor scrollAreaHoverColor;
};

class DesignSystem : public QObject
{
	Q_OBJECT

public:
	enum ThemeMode { Light, Dark };
	enum TextColorMode { Black, White, DarkGray };

	static DesignSystem* instance()
	{
		if (!m_instance)
		{
			m_instance = new DesignSystem();
		}
		return m_instance;
	}

	// 当前主题访问
	const Theme& currentTheme() const;

	// 获取主窗口指针
	void setMainWindow(QWidget* mainWindow) { m_mainWindow = mainWindow; }
	QWidget* getMainWindow() const { return m_mainWindow; }

	// 快捷获取颜色等
	QColor primaryColor() const;
	QColor textColor() const;
	QColor backgroundColor() const;
	QColor borderColor() const;
	QColor borderColorHover() const;
	QColor disabledColor() const;

	// 设置主题
	void setThemeMode(ThemeMode mode);
	ThemeMode themeMode() const;

signals:
	void themeChanged();

private:
	DesignSystem();
	DesignSystem(const DesignSystem&) = delete;
	DesignSystem& operator=(const DesignSystem&) = delete;

	void loadThemes();

	static DesignSystem* m_instance;
	Theme m_lightTheme;
	Theme m_darkTheme;
	Theme m_currentTheme;
	ThemeMode m_mode;
	QWidget* m_mainWindow = nullptr; // 用于获取主窗口
};

#endif // DESIGN_SYSTEM_H
