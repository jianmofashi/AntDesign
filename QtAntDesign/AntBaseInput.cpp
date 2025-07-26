#include "AntBaseInput.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "DesignSystem.h"
#include "StyleSheet.h"

AntBaseInput::AntBaseInput(QWidget* parent)
	: QLineEdit(parent),
	m_backgroundColor(DesignSystem::instance()->widgetBgColor()),
	m_hoverBackgroundColor(DesignSystem::instance()->widgetHoverBgColor()),
	m_focusBackgroundColor(DesignSystem::instance()->backgroundColor()),
	m_focusBorderColor(DesignSystem::instance()->primaryColor())
{
	setObjectName("AntBaseInput");
	auto style = new AntBaseInputStyle(
		m_backgroundColor,
		m_hoverBackgroundColor,
		m_focusBorderColor,
		m_focusBackgroundColor,
		DesignSystem::instance()->primaryColor(),
		1.5
	);

	this->setStyle(style);

	setContextMenuPolicy(Qt::NoContextMenu);
}