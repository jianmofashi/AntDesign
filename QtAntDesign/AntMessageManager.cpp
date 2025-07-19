#include "AntMessageManager.h"
#include <QApplication>
#include "QtAntDesign.h"
#include "DesignSystem.h"

AntMessageManager* AntMessageManager::m_instance = nullptr;

AntMessageManager::AntMessageManager(QWidget* parent)
	: QWidget(parent)
{
}

AntMessageManager::~AntMessageManager()
{
}

QRect AntMessageManager::getScreenGeometry() const
{
	// Qt5: QDesktopWidget；Qt6: QScreen
#ifdef QT_VERSION_MAJOR
#if QT_VERSION_MAJOR >= 6
	QScreen* screen = QGuiApplication::primaryScreen();
	return screen->geometry();
#else
	QDesktopWidget* desktop = QApplication::desktop();
	return desktop->screenGeometry(desktop->primaryScreen());
#endif
#else
	QDesktopWidget* desktop = QApplication::desktop();
	return desktop->screenGeometry(desktop->primaryScreen());
#endif
}

AntMessageManager* AntMessageManager::instance()
{
	if (!m_instance)
	{
		m_instance = new AntMessageManager();
	}
	return m_instance;
}

int AntMessageManager::adjustMsgDuration(int baseDuration)
{
	const int index = m_messages.size() - 1;

	if (index <= 0) {
		// 首条消息，时间更长
		return baseDuration * 1.5;
	}

	// 从第2条开始衰减：指数下降(开方)，最小保持时长
	qreal ratio = qPow(0.45, index);		// x^1, x^2, x^3...
	ratio = qMax(ratio, 0.05);				// 不低于
	return static_cast<int>(baseDuration * ratio);
}

void AntMessageManager::showMessage(AntMessage::Type type, const QString& message, int baseDuration)
{
	static QElapsedTimer  s_timer;
	if (s_timer.isValid() && s_timer.elapsed() < 100) return;
	s_timer.restart();

	// 1. 创建消息
	QWidget* mainWindow = DesignSystem::instance()->getMainWindow();
	AntMessage* msg = new AntMessage(mainWindow, type, message, adjustMsgDuration(baseDuration));
	// 连接信号：关闭、销毁
	connect(msg, &AntMessage::closed, this, &AntMessageManager::onMessageClosed);
	connect(msg, &AntMessage::destroySelf, this, &AntMessageManager::clearMessage);

	// 2. 加入队列
	m_messages.append(msg);

	// 3. 计算 endPos
	int notifWidth = msg->width();
	int x = (mainWindow->width() - notifWidth) / 2;
	QPoint endPos;

	// 10是Y方向位移量
	if (m_messages.size() == 1)
	{
		endPos = QPoint(x, 10);
	}
	else
	{
		AntMessage* prev = m_messages.at(m_messages.size() - 2);
		QPoint prevEndPos = prev->targetPos();
		int newY = prevEndPos.y() + prev->height() + 10;
		endPos = QPoint(x, newY);
	}

	// 4. 计算 startPos
	QPoint startPos;
	if (m_messages.size() == 1)
	{
		startPos = QPoint(x, -msg->height());
	}
	else
	{
		AntMessage* prev = m_messages.at(m_messages.size() - 2);
		QPoint prevEndPos = prev->targetPos();
		startPos = prevEndPos;
	}

	// 5. 设置目标位置
	msg->setTargetPos(endPos);

	// 6. 开始动画
	msg->animateIn(startPos, endPos, m_messages.size() == 1); // 首个消息控制关闭流程
}

void AntMessageManager::onMessageClosed(AntMessage* msg)
{
	// 只有队列头部消息需要淡出，其余只移动
	if (m_messages.isEmpty()) return;

	if (m_messages.first() == msg)
	{
		m_messages.removeOne(msg);
		// 首条消息淡出
		msg->animateOut();
	}
}

void AntMessageManager::clearMessage(AntMessage* message)
{
	if (message) message->deleteLater();
	if (m_messages.isEmpty()) return;

	// 优化：使用 QParallelAnimationGroup 协调所有移动动画，确保同步
	QParallelAnimationGroup* moveGroup = new QParallelAnimationGroup(this);

	// 更新剩余消息位置（向上移动）
	int x = (DesignSystem::instance()->getMainWindow()->width() - m_messages[0]->width()) / 2;
	int y = 10;		// 首个消息 y=10
	for (AntMessage* msg : m_messages)
	{
		QPoint newPos(x, y);
		msg->setTargetPos(newPos);
		msg->setCustomOpacity(1.0);	// 动画的窗口时间竞争问题,导致透明度异常无法解决,先凑活这样

		// 向上移动填补动画
		QPropertyAnimation* moveAnim = new QPropertyAnimation(msg, "pos");
		int moveDuration = qBound(100, static_cast<int>(msg->m_duration), 250);  // 优化：统一延长
		moveAnim->setDuration(moveDuration);
		moveAnim->setStartValue(msg->pos());
		moveAnim->setEndValue(newPos);
		moveAnim->setEasingCurve(QEasingCurve::InOutSine);
		moveGroup->addAnimation(moveAnim);

		y += msg->height() + 10;
	}

	// 启动组动画
	moveGroup->start(QAbstractAnimation::DeleteWhenStopped);

	// 只有在组动画完成后，才启动下一个 closeTimer（避免连锁卡顿）
	connect(moveGroup, &QParallelAnimationGroup::finished, this, [this]()
		{
			if (!m_messages.isEmpty())
			{
				m_messages[0]->startCloseTimer();
			}
		});
}