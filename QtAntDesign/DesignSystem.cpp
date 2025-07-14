// DesignSystem.cpp
#include "DesignSystem.h"

DesignSystem* DesignSystem::m_instance = nullptr;

DesignSystem::DesignSystem()
	: m_mode(Light)
{
	loadThemes();
	m_currentTheme = m_lightTheme;
}

void DesignSystem::loadThemes()
{
	// Light Theme
	m_lightTheme.primaryColor = QColor("#1677ff");
	m_lightTheme.textColor = QColor("#ffffff");
	m_lightTheme.backgroundColor = QColor("#ffffff");
	m_lightTheme.borderColor = QColor(200, 200, 200);
	m_lightTheme.borderColorHover = QColor(22, 119, 255);
	m_lightTheme.msgTextColor= QColor(30, 30, 30);
	m_lightTheme.disabledColor = QColor(150, 150, 150);
	m_lightTheme.hintBgColor = QColor(0, 0, 0);
	m_lightTheme.slideBgColor = QColor(200, 200, 200);
	m_lightTheme.slideColor = QColor(66, 133, 244);
	m_lightTheme.ringColor = QColor(153, 204, 255);
	m_lightTheme.ringhoveredColor = QColor(22, 119, 255);
	m_lightTheme.toggleButtonColor = QColor(255, 255, 255);
	m_lightTheme.toggleButtonBgColor = QColor(200, 200, 200);
	m_lightTheme.noCheckedColor = QColor(200, 200, 200);
	m_lightTheme.toolTipBgColor = QColor(255, 255, 255);
	m_lightTheme.toolTipTextColor = QColor(30, 30, 30);
	m_lightTheme.userBubbleBgColor = QColor(255, 255, 255);
	m_lightTheme.toolBtnBgColor = QColor(24, 144, 255, 40);
	m_lightTheme.dialogBgColor = QColor(255, 255, 255);
	m_lightTheme.progressBarBgColor = QColor(224, 224, 224);
	m_lightTheme.tabTextColor = QColor(0, 0, 0);
	m_lightTheme.lineEditBorderColor = QColor(180, 180, 180);
	m_lightTheme.lineEditHintColor = QColor(150, 150, 150);
	m_lightTheme.checkBoxBgColor = QColor(255, 255, 255);
	m_lightTheme.checkBoxBorderEnableColor = QColor(180, 180, 180);
	m_lightTheme.checkBoxBorderDisableColor = QColor(200, 200, 200);
	m_lightTheme.checkBoxTextColor = QColor(0, 0, 0);
	m_lightTheme.checkBoxTextDisableColor = QColor(180, 180, 180);
	QFont lightFont;
	lightFont.setPointSizeF(12);

	// Dark Theme
}

void DesignSystem::setThemeMode(ThemeMode mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;
		m_currentTheme = (mode == Light) ? m_lightTheme : m_darkTheme;
		emit themeChanged();
	}
}

DesignSystem::ThemeMode DesignSystem::themeMode() const
{
	return m_mode;
}

const Theme& DesignSystem::currentTheme() const
{
	return m_currentTheme;
}

QColor DesignSystem::primaryColor() const
{
	return m_currentTheme.primaryColor;
}

QColor DesignSystem::textColor(TextColorMode color) const
{
	if (color == Black)
		return QColor(0, 0, 0);
	else if (color == White)
		return QColor(255, 255, 255);
	else if (color == DarkGray)
		return QColor(30, 30, 30);
	else
		return QColor(0, 0, 0);
}

QColor DesignSystem::backgroundColor() const
{
	return m_currentTheme.backgroundColor;
}

QColor DesignSystem::borderColor() const
{
	return m_currentTheme.borderColor;
}

QColor DesignSystem::borderColorHover() const
{
	return m_currentTheme.borderColorHover;
}

QColor DesignSystem::disabledColor() const
{
	return m_currentTheme.disabledColor;
}