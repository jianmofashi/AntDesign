#include "AntInputNumber.h"
#include <QStyle>
#include <QStyleOptionSpinBox>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>
#include "StyleSheet.h"

AntInputNumber::AntInputNumber(QWidget* parent)
	: QSpinBox(parent), m_buttonX(0)
{
	setButtonSymbols(QAbstractSpinBox::NoButtons);

	setStyleSheet(StyleSheet::antInputNumberQss(theme.borderColor, theme.primaryColor));

	m_plusBtn = new QToolButton(this);
	m_plusBtn->setIcon(QIcon(":/Imgs/upArrow.svg"));
	m_plusBtn->setCursor(Qt::PointingHandCursor);
	m_plusBtn->setStyleSheet("QToolButton { border: none; background: transparent; }");
	connect(m_plusBtn, &QToolButton::clicked, this, [this]() { stepBy(1); });

	m_minusBtn = new QToolButton(this);
	m_minusBtn->setIcon(QIcon(":/Imgs/downArrow.svg"));
	m_minusBtn->setCursor(Qt::PointingHandCursor);
	m_minusBtn->setStyleSheet("QToolButton { border: none; background: ; }");
	connect(m_minusBtn, &QToolButton::clicked, this, [this]() { stepBy(-1); });

	m_animation = new QPropertyAnimation(this, "buttonX", this);
	m_animation->setDuration(300);
	m_animation->setEasingCurve(QEasingCurve::InOutCubic);
}

void AntInputNumber::enterEvent(QEnterEvent* event)
{
	if (m_animation->state() == QAbstractAnimation::Running)
		m_animation->stop();
	m_animation->setDirection(QAbstractAnimation::Forward);
	m_animation->start();

	QSpinBox::enterEvent(event);
}

void AntInputNumber::leaveEvent(QEvent* event)
{
	if (m_animation->state() == QAbstractAnimation::Running)
		m_animation->stop();
	m_animation->setDirection(QAbstractAnimation::Backward);
	m_animation->start();

	QSpinBox::leaveEvent(event);
}

void AntInputNumber::stepBy(int steps)
{
	QSpinBox::stepBy(steps);
	lineEdit()->deselect();  // 取消选中
	lineEdit()->setCursorPosition(lineEdit()->text().length());  // 光标移末尾
}

void AntInputNumber::resizeEvent(QResizeEvent* event)
{
	QSpinBox::resizeEvent(event);

	// 让按钮高度为控件高度的一半，宽度等于高度，做成正方形
	int btnHeight = height() / 2;
	int btnWidth = btnHeight;

	m_plusBtn->setFixedSize(btnWidth, btnHeight);
	m_plusBtn->setIconSize(QSize(btnWidth * 1.3, btnHeight * 1.3));  // 图标略大于按钮

	m_minusBtn->setFixedSize(btnWidth, btnHeight);
	m_minusBtn->setIconSize(QSize(btnWidth * 1.3, btnHeight * 1.3));

	// 更新动画目标位置
	int startX = width() + btnWidth;                // 完全在外面
	int endX = width() - btnWidth - 2;              // 靠近控件右侧

	if (m_animation->state() != QAbstractAnimation::Running)
	{
		m_buttonX = startX;
		m_animation->setStartValue(startX);
		m_animation->setEndValue(endX);
		updateButtonsPosition();
	}
}

void AntInputNumber::updateButtonsPosition()
{
	int btnWidth = m_plusBtn->width();
	int btnHeight = m_plusBtn->height();

	int btnYTop = 0;
	int btnYBottom = (height()) / 2;

	// 直接用 m_buttonX 定位按钮左上角
	m_plusBtn->move(m_buttonX, btnYTop);
	m_minusBtn->move(m_buttonX, btnYBottom);
}

void AntInputNumber::focusInEvent(QFocusEvent* event)
{
	QSpinBox::focusInEvent(event);
}

void AntInputNumber::focusOutEvent(QFocusEvent* event)
{
	QSpinBox::focusOutEvent(event);
}