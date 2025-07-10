// antradiobutton.h
#pragma once
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QEnterEvent>

class AntRadioButton : public QRadioButton
{
	Q_OBJECT
		Q_PROPERTY(qreal innerRatio READ innerRatio WRITE setInnerRatio)

public:
	explicit AntRadioButton(QWidget* parent = nullptr);

	QSize sizeHint() const override;

	qreal innerRatio() const { return m_innerRatio; }
	void setInnerRatio(qreal ratio);

protected:
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
private slots:
	void onToggled(bool checked);

private:
	qreal m_innerRatio = 0.0;  // 内圆半径比例（0~0.35）
	QPropertyAnimation* m_animation = nullptr;

	// 可调参数
	int m_marginLeft = 8;
	int m_spacing = 8;
	int m_circleDiameter = 18;

	QColor m_borderColorDefault = QColor(200, 200, 200); // 浅灰色默认边框
	QColor m_borderColorHover = QColor(22, 119, 255);    // 悬浮边框色

	QColor m_fillColor = QColor(22, 119, 255);           // 主题填充色

	bool m_hovered = false;

	qreal m_fillOpacity = 1.0;
	QPropertyAnimation* m_opacityAnimation = nullptr;
};
