#include "AntDoubleInputNumber.h"
#include <QStyle>
#include <QStyleOptionSpinBox>
#include <QLineEdit>
#include "StyleSheet.h"

AntDoubleInputNumber::AntDoubleInputNumber(QWidget* parent)
	: QDoubleSpinBox(parent), m_buttonX(0)
{
	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setStyleSheet(StyleSheet::antDoubleInputNumberQss(theme.borderColor, theme.primaryColor));

	m_plusBtn = new QToolButton(this);
	m_plusBtn->setIcon(QIcon(":/Imgs/upArrow.svg"));
	m_plusBtn->setCursor(Qt::PointingHandCursor);
	m_plusBtn->setStyleSheet("QToolButton { border: none; background: transparent; }");
	connect(m_plusBtn, &QToolButton::clicked, this, [this]() { stepBy(1); });

	m_minusBtn = new QToolButton(this);
	m_minusBtn->setIcon(QIcon(":/Imgs/downArrow.svg"));
	m_minusBtn->setCursor(Qt::PointingHandCursor);
	m_minusBtn->setStyleSheet("QToolButton { border: none; background: transparent; }");
	connect(m_minusBtn, &QToolButton::clicked, this, [this]() { stepBy(-1); });

	m_animation = new QPropertyAnimation(this, "buttonX", this);
	m_animation->setDuration(300);
	m_animation->setEasingCurve(QEasingCurve::InOutCubic);
}

AntDoubleInputNumber::~AntDoubleInputNumber()
{
	// 资源由 Qt parent-child 系统自动清理
}

void AntDoubleInputNumber::enterEvent(QEnterEvent* event)
{
	if (m_animation->state() == QAbstractAnimation::Running)
		m_animation->stop();
	m_animation->setDirection(QAbstractAnimation::Forward);
	m_animation->start();

	QDoubleSpinBox::enterEvent(event);
}

void AntDoubleInputNumber::leaveEvent(QEvent* event)
{
	if (m_animation->state() == QAbstractAnimation::Running)
		m_animation->stop();
	m_animation->setDirection(QAbstractAnimation::Backward);
	m_animation->start();

	QDoubleSpinBox::leaveEvent(event);
}

void AntDoubleInputNumber::stepBy(int steps)
{
	QDoubleSpinBox::stepBy(steps);
	lineEdit()->deselect();
	lineEdit()->setCursorPosition(lineEdit()->text().length());
}

void AntDoubleInputNumber::resizeEvent(QResizeEvent* event)
{
	QDoubleSpinBox::resizeEvent(event);

	int btnHeight = height() / 2;
	int btnWidth = btnHeight;

	m_plusBtn->setFixedSize(btnWidth, btnHeight);
	m_plusBtn->setIconSize(QSize(btnWidth * 1.3, btnHeight * 1.3));

	m_minusBtn->setFixedSize(btnWidth, btnHeight);
	m_minusBtn->setIconSize(QSize(btnWidth * 1.3, btnHeight * 1.3));

	int startX = width() + btnWidth;
	int endX = width() - btnWidth - 2;

	if (m_animation->state() != QAbstractAnimation::Running)
	{
		m_buttonX = startX;
		m_animation->setStartValue(startX);
		m_animation->setEndValue(endX);
		updateButtonsPosition();
	}
}

void AntDoubleInputNumber::updateButtonsPosition()
{
	int btnWidth = m_plusBtn->width();
	int btnHeight = m_plusBtn->height();

	int btnYTop = 0;
	int btnYBottom = height() / 2;

	m_plusBtn->move(m_buttonX, btnYTop);
	m_minusBtn->move(m_buttonX, btnYBottom);
}

// 添加属性动画相关
int AntDoubleInputNumber::buttonX() const { return m_buttonX; }

void AntDoubleInputNumber::setButtonX(int x)
{
	m_buttonX = x;
	updateButtonsPosition();
}

void AntDoubleInputNumber::focusInEvent(QFocusEvent* event)
{
	QDoubleSpinBox::focusInEvent(event);
}

void AntDoubleInputNumber::focusOutEvent(QFocusEvent* event)
{
	QDoubleSpinBox::focusOutEvent(event);
}