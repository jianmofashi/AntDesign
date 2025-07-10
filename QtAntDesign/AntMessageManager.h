#pragma once

#include <QWidget>
#include <QQueue>
#include "AntMessage.h"

class AntMessageManager : public QWidget
{
	Q_OBJECT

public:
	static AntMessageManager* instance();
	// baseDuration 表示显示多久ms
	void showMessage(AntMessage::Type type, const QString& message, int baseDuration = 1000);
	// 关闭动画
	void onMessageClosed(AntMessage* message);
	// 删除处理
	void removeMessage(AntMessage* message);
	void getMainWindow(QWidget* mainWindow)
	{
		m_mainWindow = mainWindow;
	}
signals:
	// 播放下个消息框的退出动画
	void closedNext(AntMessage* message);
private:
	explicit AntMessageManager(QWidget* parent = nullptr);
	~AntMessageManager();
	QRect getScreenGeometry() const;
	int adjustMsgDuration(int baseDuration);
private:
	QList<AntMessage*> m_messages;
	static AntMessageManager* m_instance;
	QWidget* m_mainWindow = nullptr;
};
