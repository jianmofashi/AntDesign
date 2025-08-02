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
	m_lightTheme.primaryColor = QColor(22, 119, 255);	// Ant 蓝色
	m_lightTheme.primaryHoverColor = m_lightTheme.primaryColor.lighter(130);
	m_lightTheme.textColor = QColor(255, 255, 255);
	m_lightTheme.backgroundColor = QColor(255, 255, 255);
	m_lightTheme.widgetBgColor = QColor(240, 240, 240);
	m_lightTheme.widgetHoverBgColor = QColor(230, 230, 230);
	m_lightTheme.widgetSelectedBgColor = QColor(220, 220, 220);
	m_lightTheme.borderColor = QColor(200, 200, 200);
	m_lightTheme.borderColorHover = QColor(22, 119, 255);
	m_lightTheme.shadowColor = QColor(80, 80, 80);
	m_lightTheme.msgTextColor = QColor(30, 30, 30);
	m_lightTheme.disabledColor = QColor(150, 150, 150);
	m_lightTheme.slideBgColor = QColor(200, 200, 200);
	m_lightTheme.slideColor = QColor(66, 133, 244);
	m_lightTheme.ringColor = QColor(153, 204, 255);
	m_lightTheme.ringhoveredColor = QColor(22, 119, 255);
	m_lightTheme.toggleButtonColor = QColor(255, 255, 255);
	m_lightTheme.toggleButtonBgColor = QColor(200, 200, 200);
	m_lightTheme.noCheckedColor = QColor(200, 200, 200);
	m_lightTheme.toolTipBgColor = QColor(30, 30, 30);
	m_lightTheme.toolTipTextColor = QColor(245, 245, 245);
	m_lightTheme.userBubbleBgColor = QColor(255, 255, 255);
	m_lightTheme.toolBtnBgColor = QColor(24, 144, 255, 40);
	m_lightTheme.progressBarBgColor = QColor(224, 224, 224);
	m_lightTheme.tabTextColor = QColor(0, 0, 0);
	m_lightTheme.lineEditBorderColor = QColor(180, 180, 180);
	m_lightTheme.lineEditHintColor = QColor(150, 150, 150);
	m_lightTheme.checkBoxBgColor = QColor(255, 255, 255);
	m_lightTheme.checkBoxBorderEnableColor = QColor(180, 180, 180);
	m_lightTheme.checkBoxBorderDisableColor = QColor(200, 200, 200);
	m_lightTheme.checkBoxTextColor = QColor(0, 0, 0);
	m_lightTheme.checkBoxTextDisableColor = QColor(180, 180, 180);
	m_lightTheme.popupBgColor = QColor(255, 255, 255);
	m_lightTheme.popupScrollBarColor = QColor(220, 220, 220);
	m_lightTheme.popupItemBgColor = QColor(230, 230, 230, 180);
	m_lightTheme.popupTextColor = QColor(30, 30, 30);
	m_lightTheme.scrollAreaHandleColor = QColor(230, 230, 230);
	m_lightTheme.scrollAreaHoverColor = QColor(190, 190, 190);
	m_lightTheme.listTextColor = QColor(0, 0, 0);
	m_lightTheme.listItemDescColor = QColor(150, 150, 150);
	m_lightTheme.tableTextColor = QColor(0, 0, 0);
	m_lightTheme.tabBarBgColor = QColor(243, 243, 243);
	m_lightTheme.tabHoverColor = QColor(230, 230, 230);
	m_lightTheme.onTabBtnHoverColor = QColor(200, 200, 200);

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

void DesignSystem::setTransparentMask(TransparentMask* tpMask)
{
	m_tpMask = tpMask;
}

TransparentMask* DesignSystem::getTransparentMask()
{
	return m_tpMask;
}

void DesignSystem::setDarkMask(MaskWidget* mask)
{
	m_darkMask = mask;
}

MaskWidget* DesignSystem::getDarkMask()
{
	return m_darkMask;
}

const Theme& DesignSystem::currentTheme() const
{
	return m_currentTheme;
}

QColor DesignSystem::primaryColor() const
{
	return m_currentTheme.primaryColor;
}

QColor DesignSystem::textColor() const
{
	return m_currentTheme.textColor;
}

QColor DesignSystem::backgroundColor() const
{
	return m_currentTheme.backgroundColor;
}

QColor DesignSystem::widgetBgColor() const
{
	return m_currentTheme.widgetBgColor;
}

QColor DesignSystem::widgetHoverBgColor() const
{
	return m_currentTheme.widgetHoverBgColor;
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