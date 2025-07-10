#include "AntMessage.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QSvgRenderer>
#include <QFontMetrics>
#include <QFont>

AntMessage::AntMessage(QWidget* parent, Type type, const QString& message, int duration)
	: QWidget(parent),
	m_type(type),
	m_message(message),
	m_duration(duration),
	m_customOpacity(0.0)
{
	setAttribute(Qt::WA_TranslucentBackground);
	initResources();
	setupAnimations();
}

AntMessage::~AntMessage() {}

void AntMessage::initResources()
{
	int padding = 18;
	int iconWidth = 20;
	int spacing = 10;

	// 使用字体测量文字宽度
	QFont font;
	font.setPointSizeF(10.5);
	QFontMetrics fm(font);
	int textWidth = fm.horizontalAdvance(m_message);

	// 总宽度 = 左右 padding + 图标 + 间距 + 文字
	int totalWidth = padding + iconWidth + spacing + textWidth + padding;

	// 最小宽度保障
	totalWidth = qMax(totalWidth, 130);
	int height = 40;

	setFixedSize(totalWidth, height);

	// 添加阴影
	auto shadow = new QGraphicsDropShadowEffect(this);
	shadow->setBlurRadius(20);
	shadow->setOffset(0, 2);
	shadow->setColor(QColor(0, 0, 0, 80));
	setGraphicsEffect(shadow);

	// 判断使用哪个图
	switch (m_type)
	{
	case AntMessage::Info:
		m_svgPath = QString(":/Imgs/info.svg");
		break;
	case AntMessage::Success:
		m_svgPath = QString(":/Imgs/true.svg");
		break;
	case AntMessage::Error:
		m_svgPath = QString(":/Imgs/error.svg");
		break;
	case AntMessage::Warning:
		m_svgPath = QString(":/Imgs/warning.svg");
		break;
	default:
		m_svgPath = QString(":/Imgs/true.svg");
		break;
	}
}

void AntMessage::setupAnimations()
{
	m_opacityInAnim = new QPropertyAnimation(this, "customOpacity");
	m_opacityInAnim->setDuration(400);
	m_opacityInAnim->setEasingCurve(QEasingCurve::OutCubic);

	m_posInAnim = new QPropertyAnimation(this, "pos");
	m_posInAnim->setDuration(400);
	m_posInAnim->setEasingCurve(QEasingCurve::OutCubic);

	m_animInGroup = new QParallelAnimationGroup(this);
	m_animInGroup->addAnimation(m_opacityInAnim);
	m_animInGroup->addAnimation(m_posInAnim);

	m_opacityOutAnim = new QPropertyAnimation(this, "customOpacity");
	m_opacityOutAnim->setDuration(300);
	m_opacityOutAnim->setEasingCurve(QEasingCurve::InCubic);

	connect(m_opacityOutAnim, &QPropertyAnimation::finished, this, [this]()
		{
			emit destroySelf(this);
		});

	m_closeTimer = new QTimer(this);
	m_closeTimer->setSingleShot(true);
	connect(m_closeTimer, &QTimer::timeout, this, [this]()
		{
			emit closed(this);
		});
}

void AntMessage::animateIn(QPoint startPos, QPoint endPos, bool isFirst)
{
	m_animInGroup->stop();
	m_opacityInAnim->stop();
	m_posInAnim->stop();
	setCustomOpacity(0.0f);

	move(startPos);
	show();

	// 设置动画初始值
	m_opacityInAnim->setStartValue(0.0f);
	m_opacityInAnim->setEndValue(1.0f);

	m_posInAnim->setStartValue(startPos);
	m_posInAnim->setEndValue(endPos);

	if (isFirst)
	{
		connect(m_animInGroup, &QParallelAnimationGroup::finished, this, [this]()
			{
				m_closeTimer->start(m_duration);
			});
	}

	m_animInGroup->start();
}

void AntMessage::startCloseTimer()
{
	m_closeTimer->start(m_duration);
}

void AntMessage::animateOut()
{
	// 动态调整退场动画速度：越晚显示的消息，退场越快
	// 但仍保持合理区间，防止动画突兀或完全看不清
	int outDuration = qBound(100, static_cast<int>(m_duration * 0.7), 300);
	m_opacityOutAnim->setDuration(outDuration);

	m_opacityOutAnim->setStartValue(1.0f);
	m_opacityOutAnim->setEndValue(0.0f);
	m_opacityOutAnim->start();
}

void AntMessage::moveTo(QPoint target)
{
	QPropertyAnimation* moveAnim = new QPropertyAnimation(this, "pos");
	int outDuration = qBound(100, static_cast<int>(m_duration * 0.4), 200);
	moveAnim->setDuration(outDuration);
	moveAnim->setStartValue(pos());
	moveAnim->setEndValue(target);
	moveAnim->setEasingCurve(QEasingCurve::InOutCubic);
	moveAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void AntMessage::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setOpacity(m_customOpacity);

	// 背景圆角绘制
	painter.setBrush(QColor(255, 255, 255));
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(rect(), 8, 8);

	// 图标绘制
	int padding = 18;
	int iconWidth = 20;
	int spacing = 10;
	QSvgRenderer svgRenderer(m_svgPath);
	QRect iconRect(padding, (height() - iconWidth) / 2, iconWidth, iconWidth);
	svgRenderer.render(&painter, iconRect);

	// 文本绘制
	QRect textRect = rect().adjusted(padding + iconWidth + spacing, 0, -padding, 0);
	QFont font;
	font.setPointSizeF(10.5);
	painter.setFont(font);
	painter.setPen(QColor(30, 30, 30));
	painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_message);
}

void AntMessage::setCustomOpacity(qreal opacity)
{
	m_customOpacity = opacity;
	update();
}

qreal AntMessage::getCustomOpacity()
{
	return m_customOpacity;
}

void AntMessage::closeMessage()
{
	m_closeTimer->stop();
}