#pragma once

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class AntMessage : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(qreal customOpacity READ getCustomOpacity WRITE setCustomOpacity)
public:
	enum Type { Info, Success, Error, Warning };

	explicit AntMessage(QWidget* parent, Type type, const QString& message, int duration = 1000);
	~AntMessage();

	// 进入动画
	void animateIn(QPoint startPos, QPoint endPos, bool isFirst);
	// 退出动画
	void animateOut();
	void closeMessage();
	void startCloseTimer();
	qreal getCustomOpacity();
	void setCustomOpacity(qreal opacity);

	void setTargetPos(const QPoint& pos) { m_targetPos = pos; }
	QPoint targetPos() const { return m_targetPos; }
	QPoint startPos() const { return m_startPos; }
	QPoint endPos() const { return m_endPos; }
	void setStartPos(const QPoint& pos) { m_startPos = pos; }
	void setEndPos(const QPoint& pos) { m_endPos = pos; }
public:
	int m_duration;
signals:
	void closed(AntMessage* self);
	void destroySelf(AntMessage* self);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void initResources();
	void setupAnimations();

	bool m_isExit;
	bool m_isFirst;
	QPoint m_targetPos;
	QPoint m_startPos;
	QPoint m_endPos;
	Type m_type;
	QString m_message;
	QString m_svgPath;
	qreal m_customOpacity;

	QTimer* m_closeTimer;
	QPropertyAnimation* m_posAnim;
	QPropertyAnimation* m_opacityAnim;
	QParallelAnimationGroup* m_animGroup;
};
