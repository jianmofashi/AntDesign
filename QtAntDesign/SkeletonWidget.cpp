#include "skeletonwidget.h"
#include <QPainter>
#include <QLinearGradient>

SkeletonWidget::SkeletonWidget(QSize size, int rectRadius, QWidget* parent)
	: QWidget(parent), m_rectRadius(rectRadius)
{
	setMinimumSize(size);

	int barWidth = 140;
	m_animation = new QPropertyAnimation(this, "gradientX");
	m_animation->setDuration(1000);
	m_animation->setStartValue(-barWidth);
	m_animation->setEndValue(width() + barWidth);
	m_animation->setLoopCount(-1);
}

void SkeletonWidget::setGradientX(int x)
{
	m_gradientX = x;
	update();
}

void SkeletonWidget::stopSkeleton()
{
	if (m_animation)
	{
		m_animation->stop();
	}
	this->hide();  // 隐藏骨架控件
}

void SkeletonWidget::startSkeleton()
{
	if (m_animation)
	{
		m_animation->stop();
		m_animation->start();
	}
	this->show();  // 隐藏骨架控件
}

void SkeletonWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// 背景色
	QRectF rect = this->rect();
	p.setBrush(QColor("#DCDCDC"));  // RGB(220, 220, 220)
	p.setPen(Qt::NoPen);
	p.drawRoundedRect(rect, m_rectRadius, m_rectRadius);

	// 高亮动画条
	QLinearGradient gradient(m_gradientX - 70, 0, m_gradientX + 70, 0);
	gradient.setColorAt(0.0, QColor(255, 255, 255, 0));
	gradient.setColorAt(0.5, QColor(255, 255, 255, 150));
	gradient.setColorAt(1.0, QColor(255, 255, 255, 0));

	p.setBrush(gradient);
	p.drawRoundedRect(rect, m_rectRadius, m_rectRadius);
}

void SkeletonWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if (m_animation) {
		int barWidth = 140;
		m_animation->setEndValue(width() + barWidth);
	}
}