#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPainterPath>

class Win11CheckButton : public QRadioButton
{
	Q_OBJECT
		Q_PROPERTY(qreal progress READ progress WRITE setProgress)
public:
	Win11CheckButton(QWidget* parent);
	~Win11CheckButton();

	qreal progress() const { return m_progress; }
	void setProgress(qreal p) {
		m_progress = p;
		update();
	}
protected:
	void paintEvent(QPaintEvent* event) override;
	QSize sizeHint() const;
private:
	qreal approximatePathLength(const QPainterPath& path);
	QPainterPath strokePathPortion(const QPainterPath& path, qreal length);
private:
	qreal m_progress;
	QPropertyAnimation* m_anim;
	QColor themeBlue = QColor(22, 119, 255);

	// 可调属性
	int boxSize = 16;
	int boxRadius = 3;
	int boxSpacing = 6;
	int boxMargin = 4;
};
