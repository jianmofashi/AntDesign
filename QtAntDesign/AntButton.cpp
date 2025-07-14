#include "AntButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QOverload>
#include "DesignSystem.h"

AntButton::AntButton(QString btnText, qreal textSize, QWidget* parent)
	:QPushButton(parent),
	m_radius(6),
	m_margin(8),
	m_hovered(false),
	m_pressed(false),
	baseColor(DesignSystem::instance()->primaryColor()) // Ant Design 默认主蓝色
{
	setCursor(Qt::PointingHandCursor);
	QFont font;
	font.setPointSizeF(textSize);
	setFont(font);
	setText(btnText);
}

AntButton::~AntButton()
{
}

void AntButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_clickTimer.isValid() || m_clickTimer.elapsed() >= m_clickIntervalMs)
		{
			m_clickTimer.restart();

			// 点击时创建一个新的波纹实例
			QRectF buttonRect = QRectF(m_margin, m_margin,
				width() - 2 * m_margin,
				height() - 2 * m_margin);	// 波纹起始矩形区域
			Ripple* ripple = new Ripple(buttonRect, m_radius, this);
			m_ripples.append(ripple);

			// 创建并配置并行动画组
			QParallelAnimationGroup* rippleAnimationGroup = new QParallelAnimationGroup(this);

			// 半径动画：从 0 → 1.0，后面乘以最大半径值
			QPropertyAnimation* offsetAnimation = new QPropertyAnimation(ripple, "m_offset");
			offsetAnimation->setDuration(animTime);
			offsetAnimation->setEasingCurve(QEasingCurve::InOutSine);

			// 透明度动画：从 0.8 → 0.0
			QPropertyAnimation* opacityAnimation = new QPropertyAnimation(ripple, "m_opacity");
			opacityAnimation->setDuration(animTime + 300);
			opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);

			rippleAnimationGroup->addAnimation(offsetAnimation);
			rippleAnimationGroup->addAnimation(opacityAnimation);

			// 仅当动画未运行时才重置
			if (rippleAnimationGroup->state() != QAbstractAnimation::Running)
			{
				ripple->setBeginValue(0, 0.5);
			}

			// 配置动画参数
			offsetAnimation->setStartValue(ripple->offset());
			offsetAnimation->setEndValue(m_margin - 2);

			opacityAnimation->setStartValue(ripple->opacity());
			opacityAnimation->setEndValue(0.0);

			// 属性变化时更新界面
			connect(ripple, &Ripple::offsetChanged, this, QOverload<>::of(&AntButton::update));
			connect(ripple, &Ripple::opacityChanged, this, QOverload<>::of(&AntButton::update));

			// 启动动画
			rippleAnimationGroup->start(QAbstractAnimation::DeleteWhenStopped);

			// 动画结束后移除波纹实例
			connect(rippleAnimationGroup, &QParallelAnimationGroup::finished, this, [this, ripple]()
				{
					m_ripples.removeOne(ripple);
					ripple->deleteLater();
				});

			m_pressed = true;
			update();
		}
	}
	QPushButton::mousePressEvent(event);
}

void AntButton::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_pressed = false;
		update();
	}
	QPushButton::mouseReleaseEvent(event);
}

void AntButton::enterEvent(QEnterEvent* event)
{
	m_hovered = true;
	update();
	QPushButton::enterEvent(event);
}

void AntButton::leaveEvent(QEvent* event)
{
	m_hovered = false;
	update();
	QPushButton::leaveEvent(event);
}

void AntButton::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// 1. 绘制按钮背景（圆角矩形）
	QRectF buttonRect = QRectF(m_margin, m_margin,
		width() - 2 * m_margin,
		height() - 2 * m_margin);
	QColor fillColor = baseColor;
	if (m_pressed)
		fillColor = baseColor.darker(120);	// 值越大越暗
	else if (m_hovered)
		fillColor = baseColor.lighter(110);	// 值越大越亮

	painter.setBrush(fillColor);
	painter.setPen(Qt::NoPen);
	painter.drawRoundedRect(buttonRect, m_radius, m_radius);

	// 2. 绘制文字
	painter.setPen(DesignSystem::instance()->currentTheme().textColor);
	painter.setFont(font());
	painter.drawText(buttonRect, Qt::AlignCenter, text());

	// 3. 如果正在播放波纹动画，就只绘制“外环”部分
	// 3.1 配置半透明的波纹颜色
	QColor rippleColor = baseColor;
	painter.setPen(Qt::NoPen);
	for (Ripple* ripple : m_ripples)
	{
		rippleColor.setAlphaF(ripple->opacity());
		painter.setBrush(rippleColor);

		// 3.2 计算“外部圆角矩形”和“内部圆角矩形”
		int rippleOffset = ripple->offset();
		QRectF outerRect = buttonRect.adjusted(
			-rippleOffset, -rippleOffset,
			+rippleOffset, +rippleOffset
		);

		QRectF innerRect = buttonRect;

		// 3.3 构造 QPainterPath 做路径相减：outer - inner 得到按钮本体外圈这部分
		QPainterPath outerPath;
		outerPath.addRoundedRect(outerRect, m_radius + 4, m_radius + 4);

		QPainterPath innerPath;
		innerPath.addRoundedRect(innerRect, m_radius, m_radius);

		QPainterPath ringPath = outerPath.subtracted(innerPath);

		// 3.4 填充这段“环形路径”
		painter.drawPath(ringPath);
	}
}