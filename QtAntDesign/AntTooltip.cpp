#include "AntTooltip.h"
#include "QVBoxLayout"
#include <QLabel>
#include <QLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QPainter>
#include "DesignSystem.h"

AntTooltip::AntTooltip(QString text, ArrowDir dir, QWidget* parent)
	: QWidget(parent), m_arrowDirection(dir), m_text(text)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

	QFont font;
	font.setPointSize(11);
	m_font = font;
	QFontMetrics metrics(m_font);

	// 测量文本尺寸（不换行）
	int textWidth = metrics.horizontalAdvance(m_text);
	int textHeight = metrics.height();

	// 加额外 padding 整体更美观
	int Padding = 16;
	int paddedTextWidth = textWidth + Padding * 2;
	int paddedTextHeight = textHeight + Padding * 2;

	// 计算总宽高
	int tiptoolWidth = paddedTextWidth + (m_arrowDirection == ArrowLeft || m_arrowDirection == ArrowRight ? arrowWidth : 0);
	int tiptoolHeight = paddedTextHeight + (m_arrowDirection == ArrowTop || m_arrowDirection == ArrowBottom ? arrowHeight : 0);

	// 设置控件尺寸
	resize(tiptoolWidth, tiptoolHeight);

	// 阴影
	auto* shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setBlurRadius(20);            // 模糊半径，越大越模糊
	shadowEffect->setOffset(0, 0.5);              // 阴影偏移量（向下偏移）
	shadowEffect->setColor(QColor(50, 50, 50, 120)); // 阴影颜色 + 透明度

	this->setGraphicsEffect(shadowEffect);

	// 几何动画
	geoAnim = new QPropertyAnimation(this, "geometry");
	opcaAnim = new QPropertyAnimation(this, "windowOpacity");
	groupAnim = new QParallelAnimationGroup(this);
	groupAnim->addAnimation(geoAnim);
	groupAnim->addAnimation(opcaAnim);

	// 信号槽
	connect(groupAnim, &QParallelAnimationGroup::finished, this, [this]()
		{
			if (isHide)
			{
				emit destroySelf(this);
			}
		});
}

AntTooltip::~AntTooltip()
{
}

void AntTooltip::showAnimated(QPoint globalPos)
{
	isHide = false;

	const QSize finalSize = size();
	const qreal scaleFactor = 0.93;

	// 目标最终的显示矩形
	QRect endRect(globalPos, finalSize);

	// 缩放动画的中心点
	QPoint center = endRect.center();

	// 起始大小
	QSize startSize(finalSize.width() * scaleFactor, finalSize.height() * scaleFactor);

	// 计算起始左上角，使中心一致
	QPoint startTopLeft(
		center.x() - startSize.width() / 2,
		center.y() - startSize.height() / 2
	);
	QRect startRect(startTopLeft, startSize);

	setGeometry(startRect);
	show();

	groupAnim->stop();
	geoAnim->setStartValue(startRect);
	geoAnim->setEndValue(endRect);
	geoAnim->setEasingCurve(QEasingCurve::OutQuart);
	geoAnim->setDuration(200);
	opcaAnim->setStartValue(0.0f);
	opcaAnim->setEndValue(1.0f);
	opcaAnim->setDuration(200);
	opcaAnim->setEasingCurve(QEasingCurve::OutQuart);

	groupAnim->setDirection(QAbstractAnimation::Forward);
	groupAnim->start();
}

void AntTooltip::hideAnimated()
{
	isHide = true;
	groupAnim->setDirection(QAbstractAnimation::Backward);
	groupAnim->start();
}

QPoint AntTooltip::arrowTipOffset() const
{
	switch (m_arrowDirection) {
	case ArrowLeft:
		return QPoint(0, height() / 2);
	case ArrowRight:
		return QPoint(width(), height() / 2);
	case ArrowTop:
		return QPoint(width() / 2, 0);
	case ArrowBottom:
		return QPoint(width() / 2, height());
	default:
		return QPoint(width() / 2, height() / 2);
	}
}

void AntTooltip::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(Qt::NoPen);

	const int radius = 6;
	QRect rectBubble;  // 气泡矩形区域（不包括箭头）

	QPainterPath path;

	// 判断三角形的宽高
	if (m_arrowDirection == ArrowDir::ArrowLeft || m_arrowDirection == ArrowDir::ArrowRight)
	{
		arrowHeight = 14;
		arrowWidth = 7;
	}
	else
	{
		arrowHeight = 7;
		arrowWidth = 14;
	}

	switch (m_arrowDirection) {
	case ArrowDir::ArrowLeft: {
		rectBubble = QRect(arrowWidth, 0, width() - arrowWidth, height());
		QRectF r = rectBubble.adjusted(margin, margin, -margin, -margin);

		int ax = r.left();
		int ay = (height() - arrowHeight) / 2;
		int ah = arrowHeight;

		path.moveTo(ax, ay);
		path.lineTo(ax - arrowWidth, ay + ah / 2);
		path.lineTo(ax, ay + ah);

		// 左下圆角
		path.lineTo(ax, r.bottom() - radius);
		path.quadTo(r.left(), r.bottom(), r.left() + radius, r.bottom());

		// 右下圆角
		path.lineTo(r.right() - radius, r.bottom());
		path.quadTo(r.right(), r.bottom(), r.right(), r.bottom() - radius);

		// 右上圆角
		path.lineTo(r.right(), r.top() + radius);
		path.quadTo(r.right(), r.top(), r.right() - radius, r.top());

		// 左上圆角
		path.lineTo(r.left() + radius, r.top());
		path.quadTo(r.left(), r.top(), r.left(), r.top() + radius);

		path.lineTo(ax, ay);
		break;
	}
	case ArrowDir::ArrowRight: {
		rectBubble = QRect(0, 0, width() - arrowWidth, height());
		QRectF r = rectBubble.adjusted(margin, margin, -margin, -margin);

		int ax = r.right();
		int ay = (rectBubble.height() - arrowHeight) / 2;
		int ah = arrowHeight;

		path.moveTo(ax, ay);
		path.lineTo(ax + arrowWidth, ay + ah / 2);
		path.lineTo(ax, ay + ah);

		// 右下圆角
		path.lineTo(ax, r.bottom() - radius);
		path.quadTo(r.right(), r.bottom(), r.right() - radius, r.bottom());

		// 左下圆角
		path.lineTo(r.left() + radius, r.bottom());
		path.quadTo(r.left(), r.bottom(), r.left(), r.bottom() - radius);

		// 左上圆角
		path.lineTo(r.left(), r.top() + radius);
		path.quadTo(r.left(), r.top(), r.left() + radius, r.top());

		// 右上圆角
		path.lineTo(r.right() - radius, r.top());
		path.quadTo(r.right(), r.top(), r.right(), r.top() + radius);

		path.lineTo(ax, ay);
		break;
	}
	case ArrowDir::ArrowTop: {
		rectBubble = QRect(0, arrowHeight, width(), height() - arrowHeight);
		QRectF r = rectBubble.adjusted(margin, margin, -margin, -margin);

		int ax = (r.width() / 2);
		int ay = r.top();

		path.moveTo(ax, ay);
		path.lineTo(ax + arrowWidth / 2, ay - arrowHeight);
		path.lineTo(ax + arrowWidth, ay);

		// 右上圆角
		path.lineTo(r.right() - radius, ay);
		path.quadTo(r.right(), ay, r.right(), ay + radius);

		// 右下圆角
		path.lineTo(r.right(), r.bottom() - radius);
		path.quadTo(r.right(), r.bottom(), r.right() - radius, r.bottom());

		// 左下圆角
		path.lineTo(r.left() + radius, r.bottom());
		path.quadTo(r.left(), r.bottom(), r.left(), r.bottom() - radius);

		// 左上圆角
		path.lineTo(r.left(), ay + radius);
		path.quadTo(r.left(), ay, r.left() + radius, ay);

		path.lineTo(ax, ay);
		break;
	}
	case ArrowDir::ArrowBottom: {
		rectBubble = QRect(0, 0, width(), height() - arrowHeight);
		QRectF r = rectBubble.adjusted(margin, margin, -margin, -margin);

		int ax = (r.width() / 2);
		int ay = r.bottom();

		path.moveTo(ax, ay);
		path.lineTo(ax + arrowWidth / 2, ay + arrowHeight);
		path.lineTo(ax + arrowWidth, ay);

		// 右下圆角
		path.lineTo(r.right() - radius, ay);
		path.quadTo(r.right(), ay, r.right(), ay - radius);

		// 右上圆角
		path.lineTo(r.right(), r.top() + radius);
		path.quadTo(r.right(), r.top(), r.right() - radius, r.top());

		// 左上圆角
		path.lineTo(r.left() + radius, r.top());
		path.quadTo(r.left(), r.top(), r.left(), r.top() + radius);

		// 左下圆角
		path.lineTo(r.left(), ay - radius);
		path.quadTo(r.left(), ay, r.left() + radius, ay);

		path.lineTo(ax, ay);
		break;
	}
	}

	// 填充气泡主体
	p.setBrush(DesignSystem::instance()->currentTheme().toolTipBgColor);
	p.drawPath(path);

	// 设置字体与颜色
	p.setFont(m_font);
	p.setPen(DesignSystem::instance()->currentTheme().toolTipTextColor);  // 深灰色文本，更柔和

	// 计算文本区域（在 rectBubble 内部减去 margin）
	QRect textRect = rectBubble.adjusted(margin, margin, -margin, -margin);
	// 绘制文本，不支持自动换行
	p.drawText(textRect, Qt::TextSingleLine | Qt::AlignCenter, m_text);
}