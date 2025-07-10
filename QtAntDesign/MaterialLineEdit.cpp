#include "MaterialLineEdit.h"
#include <QPainter>
#include <QStyleOption>
#include <QFontMetrics>
#include <QToolButton>

MaterialLineEdit::MaterialLineEdit(QWidget* parent)
	: QLineEdit(parent),
	m_labelProgress(0.0),
	m_underlineProgress(0.0),
	m_animation(new QPropertyAnimation(this, "labelProgress", this)),
	m_underlineAnimation(new QPropertyAnimation(this, "underlineProgress", this))
{
	setFrame(false);
	setStyleSheet(StyleSheet::lineEditQss(QColor(210, 210, 210)));
	setPlaceholderText("");

	connect(this, &QLineEdit::textChanged, this, &MaterialLineEdit::onTextChanged);

	m_animation->setEasingCurve(QEasingCurve::InOutSine);
	m_underlineAnimation->setEasingCurve(QEasingCurve::InOutSine);

	if (!text().isEmpty()) {
		m_labelProgress = 1.0;
		m_underlineProgress = 1.0;
	}
}

MaterialLineEdit::~MaterialLineEdit()
{
	delete m_animation;
	delete m_underlineAnimation;
}

void MaterialLineEdit::setPasswordToggleEnabled(bool enabled)
{
	m_togglePasswordEnabled = enabled;
	// 右侧小眼睛
	m_togglePasswordButton = new QToolButton(this);
	if (enabled) {
		m_togglePasswordButton->show();
		setEchoMode(QLineEdit::Password);  // 默认启用密码模式
		m_togglePasswordButton->setCheckable(true);
		m_togglePasswordButton->setChecked(false);
		m_togglePasswordButton->setCursor(Qt::PointingHandCursor);
		m_togglePasswordButton->setIcon(QIcon(":/Imgs/eye-slash-filled.svg"));
		m_togglePasswordButton->setIconSize(QSize(40, 40));
		m_togglePasswordButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");

		connect(m_togglePasswordButton, &QToolButton::toggled, this, [this](bool checked) {
			m_passwordVisible = checked;
			setEchoMode(m_passwordVisible ? QLineEdit::Normal : QLineEdit::Password);
			m_togglePasswordButton->setIcon(QIcon(m_passwordVisible
				? ":/Imgs/eye-filled.svg"
				: ":/Imgs/eye-slash-filled.svg"));
			});
	}
	else {
		if (m_togglePasswordButton) {
			m_togglePasswordButton->hide();
		}
		setEchoMode(QLineEdit::Normal);
	}

	if (m_togglePasswordEnabled && m_togglePasswordButton) {
		int buttonWidth = m_togglePasswordButton->size().width();
		setTextMargins(0, 0, buttonWidth + 10, 0);
	}
	else {
		setTextMargins(0, 0, 0, 0);
	}
}

void MaterialLineEdit::setRightTextBtn(QString text)
{
	//右侧文本
	setCursor(Qt::PointingHandCursor);
	m_textBtn = new QPushButton(this);
	m_textBtn->setStyleSheet(StyleSheet::noBorderBtnQss(QColor(24, 144, 255)));
	m_textBtn->setText(text);
	m_textBtn->setMinimumSize(40, 40);
	m_textBtn->show();
}

void MaterialLineEdit::setLabelText(const QString& text)
{
	m_labelText = text;
	update();
}

QString MaterialLineEdit::labelText() const
{
	return m_labelText;
}

qreal MaterialLineEdit::labelProgress() const
{
	return m_labelProgress;
}

void MaterialLineEdit::setLabelProgress(qreal progress)
{
	if (qFuzzyCompare(m_labelProgress, progress))
		return;
	m_labelProgress = progress;
	update();
}

qreal MaterialLineEdit::underlineProgress() const
{
	return m_underlineProgress;
}

void MaterialLineEdit::setUnderlineProgress(qreal progress)
{
	if (qFuzzyCompare(m_underlineProgress, progress))
		return;
	m_underlineProgress = progress;
	update();
}

void MaterialLineEdit::focusInEvent(QFocusEvent* event)
{
	m_themeColor = QColor(24, 144, 255);
	m_underlineProgress = 0.0;
	QLineEdit::focusInEvent(event);
	if (!m_labelText.isEmpty())
	{
		animateLabel(true);
		animateUnderline(true);
	}
}

void MaterialLineEdit::focusOutEvent(QFocusEvent* event)
{
	QLineEdit::focusOutEvent(event);
	if (text().isEmpty())
	{
		animateLabel(false);
	}
	animateUnderline(false);
}

void MaterialLineEdit::onTextChanged(const QString& /*newText*/)
{
	if (!text().isEmpty())
	{
		animateLabel(true);
		animateUnderline(true);
	}
	else
	{
		if (!hasFocus())
		{
			animateLabel(false);
			animateUnderline(false);
		}
	}
}

void MaterialLineEdit::animateLabel(bool toUp)
{
	m_animation->stop();
	m_animation->setStartValue(m_labelProgress);
	m_animation->setEndValue(toUp ? 1.0 : 0.0);
	m_animation->start();
}

void MaterialLineEdit::animateUnderline(bool show)
{
	m_underlineAnimation->stop();

	qreal startValue = m_underlineProgress;  // 当前值

	if (show) {
		m_underlineAnimation->setStartValue(startValue);
		m_underlineAnimation->setEndValue(0.97);
		m_underlineAnimation->setDuration(300);
	}
	else {
		m_underlineAnimation->setStartValue(startValue);  // 改为当前值
		m_underlineAnimation->setEndValue(0.0);
		m_underlineAnimation->setDuration(300);
	}

	m_underlineAnimation->start();
}

void MaterialLineEdit::paintEvent(QPaintEvent* event)
{
	QLineEdit::paintEvent(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// --- 绘制标签动画文字 ---
	QFont baseFont = font();
	qreal startSize = baseFont.pointSizeF();
	qreal endSize = baseFont.pointSizeF() * 0.8;
	qreal currentSize = startSize + (endSize - startSize) * m_labelProgress;

	QFont labelFont = baseFont;
	labelFont.setPointSizeF(currentSize);
	painter.setFont(labelFont);

	QFontMetrics fmStart(baseFont);
	int startY = (height() + fmStart.ascent() - fmStart.descent()) / 2;
	QFontMetrics fmEnd(labelFont);
	int endY = fmEnd.ascent();
	qreal y = startY + (endY - startY) * m_labelProgress;

	int x = 6;

	QColor startColor(150, 150, 150);
	QColor endColor = m_themeColor;
	auto lerp = [](int a, int b, qreal t) { return int(a + (b - a) * t); };
	QColor currentColor(
		lerp(startColor.red(), endColor.red(), m_labelProgress),
		lerp(startColor.green(), endColor.green(), m_labelProgress),
		lerp(startColor.blue(), endColor.blue(), m_labelProgress)
	);
	painter.setPen(currentColor);
	painter.drawText(x, int(y), m_labelText);

	// --- 绘制下划线动画 ---
	if (m_underlineProgress > 0.0)  // 始终绘制（包括失去焦点阶段）
	{
		// 焦点在时使用主题色，失去焦点时使用灰色
		QColor underlineColor = m_themeColor;
		QPen pen(underlineColor, 3);  // 控制横线粗细
		painter.setPen(pen);

		int lineY = height() - 2;
		int fullLength = width();  // 横线可覆盖整个控件宽度
		int centerX = width() / 2;
		int halfLength = int((fullLength / 2.0) * m_underlineProgress);  // 根据动画进度设置长度

		painter.drawLine(centerX - halfLength, lineY, centerX + halfLength, lineY);
	}
}

void MaterialLineEdit::resizeEvent(QResizeEvent* event)
{
	QLineEdit::resizeEvent(event);
	// 右侧小眼睛
	if (m_togglePasswordButton)
	{
		int buttonWidth = m_togglePasswordButton->size().width();
		setTextMargins(0, 0, buttonWidth + 10, 0);
		m_togglePasswordButton->move(width() - buttonWidth - 10, (height() - m_togglePasswordButton->height()) / 2 + 2);
	}
	// 右侧文本
	if (m_textBtn)
	{
		int textButtonWidth = m_textBtn->size().width();
		setTextMargins(0, 0, textButtonWidth + 10, 0);
		m_textBtn->move(width() - textButtonWidth - 10, (height() - m_textBtn->height()) / 2);
	}
}