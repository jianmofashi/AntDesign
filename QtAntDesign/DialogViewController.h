#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include "MaterialDialog.h"
#include "MaskWidget.h"

class DialogViewController : public QGraphicsView
{
	Q_OBJECT
		Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor)
public:
	DialogViewController(bool loginState, int parentW, int parentH, QWidget* parent);
	~DialogViewController();
signals:
	void playMask(bool isAddMask);
	void successLogin(bool loginState);
public slots:
	void updateDialogPositionAndSize(MaterialDialog::PageIndex index);
	void showAnim(MaterialDialog::PageIndex index);
	void hideAnim();
	void buildStandardDialog(int parentW, int parentH, int dialogW, int dialogH, QString title, QString text);
	void getParentSize(int parentW, int parentH);
private:
	void onMaskClicked();
private:
	QGraphicsScene* scene = nullptr;
	QGraphicsProxyWidget* proxy = nullptr;
	QPropertyAnimation* scaleAnim = nullptr;
	QPropertyAnimation* opacityAnim = nullptr;
	QParallelAnimationGroup* groupAnim = nullptr;
	MaterialDialog* dialog = nullptr;

	int contentDialogW;
	int contentDialogH;
	int standardDialogW;
	int standardDialogH;
	QSize mainWindowSize;

	//遮罩组件
	MaskWidget* mask = nullptr;
	bool m_isHide = false;
private:
	qreal m_scaleFactor = 1.0;

public:
	qreal scaleFactor() const { return m_scaleFactor; }
	void setScaleFactor(qreal factor) {
		m_scaleFactor = factor;
		QTransform t;
		t.scale(m_scaleFactor, m_scaleFactor);
		this->setTransform(t);
	}
};
