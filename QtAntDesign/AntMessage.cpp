#include "AntMessage.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QSvgRenderer>
#include <QFontMetrics>
#include <QFont>
#include "DesignSystem.h"

AntMessage::AntMessage(QWidget* parent, Type type, const QString& message, int duration)
	: QWidget(parent),
	m_type(type),
	m_message(message),
	m_duration(duration),
	m_customOpacity(0.0),
	m_isExit(false),
	m_isFirst(false)
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
	// 动画
	m_opacityAnim = new QPropertyAnimation(this, "customOpacity");
	m_opacityAnim->setDuration(300);
	m_opacityAnim->setEasingCurve(QEasingCurve::InOutSine);

	m_posAnim = new QPropertyAnimation(this, "pos");
	m_posAnim->setDuration(300);
	m_posAnim->setEasingCurve(QEasingCurve::InOutSine);

	m_animGroup = new QParallelAnimationGroup(this);
	m_animGroup->addAnimation(m_opacityAnim);
	m_animGroup->addAnimation(m_posAnim);

	connect(m_animGroup, &QPropertyAnimation::finished, this, [this]()
		{
			// 退出动画完毕
			if (m_isExit)
			{
				emit destroySelf(this);
			}
			else
			{
				// 进入动画完毕
				if (m_isFirst) m_closeTimer->start(m_duration);
			}
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
	if (m_animGroup->state() == QAbstractAnimation::Running)
		m_animGroup->stop();

	m_isExit = false;
	m_isFirst = isFirst;
	move(startPos);
	show();

	// 设置动画初始值
	m_opacityAnim->setStartValue(0.0);
	m_opacityAnim->setEndValue(1.0);
	m_posAnim->setStartValue(startPos);
	m_posAnim->setEndValue(endPos);
	m_animGroup->start();
}

void AntMessage::startCloseTimer()
{
	m_closeTimer->start(m_duration);
}

void AntMessage::animateOut()
{
	if (m_animGroup->state() == QAbstractAnimation::Running)
		m_animGroup->stop();

	m_isExit = true;
	m_opacityAnim->setStartValue(1.0);
	m_opacityAnim->setEndValue(0.0);
	m_posAnim->setStartValue(pos());
	m_posAnim->setEndValue(pos() - QPoint(0, 8));
	m_animGroup->start();
}

void AntMessage::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setOpacity(m_customOpacity);

	// 背景圆角绘制
	painter.setBrush(DesignSystem::instance()->backgroundColor());
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
	painter.setPen(DesignSystem::instance()->currentTheme().msgTextColor);
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