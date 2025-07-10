#ifndef MATERIALLINEEDIT_H
#define MATERIALLINEEDIT_H

#include <QLineEdit>
#include <QPropertyAnimation>
#include <QColor>
#include <QPushButton>
#include "StyleSheet.h"

class MaterialLineEdit : public QLineEdit
{
	Q_OBJECT
		Q_PROPERTY(qreal labelProgress READ labelProgress WRITE setLabelProgress)
		Q_PROPERTY(qreal underlineProgress READ underlineProgress WRITE setUnderlineProgress)
public:
	explicit MaterialLineEdit(QWidget* parent = nullptr);
	~MaterialLineEdit() override;

	void setLabelText(const QString& text);
	QString labelText() const;

	// 控制浮动标签动画的进度
	qreal labelProgress() const;
	void setLabelProgress(qreal progress);

	// 控制下划线动画的进度
	qreal underlineProgress() const;
	void setUnderlineProgress(qreal progress);

	// 是否开启右侧的小眼睛(隐藏明文模式)
	void setPasswordToggleEnabled(bool enabled);

	// 设置右侧文本按钮启用
	void setRightTextBtn(QString text);

	// 当用户输入错误时候给予错误提示
	void errorHint()
	{
		m_themeColor = QColor(255, 77, 79);	// 经典红
		m_underlineProgress = 0.0;
		animateUnderline(true);
	}

	QString errorText() const
	{
		return m_errorText;
	}

	QPushButton* textBtn() { return m_textBtn; };

protected:
	void paintEvent(QPaintEvent* event) override;
	void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void resizeEvent(QResizeEvent* event);
private slots:
	void onTextChanged(const QString& text);

private:
	void animateLabel(bool toUp);
	void animateUnderline(bool toExpand);

	QString m_labelText;
	qreal m_labelProgress;
	qreal m_underlineProgress;

	QPropertyAnimation* m_labelAnimation;
	QPropertyAnimation* m_underlineAnimation;
	QPropertyAnimation* m_animation;

	QColor m_themeColor = QColor(22, 119, 255);	// 经典蓝

	QToolButton* m_togglePasswordButton = nullptr;
	QPushButton* m_textBtn = nullptr;
	bool m_passwordVisible = false;
	bool m_togglePasswordEnabled = false;

	QString m_errorText;            // 错误提示文本
};

#endif // MATERIALLINEEDIT_H
