#pragma once

#include <QDoubleSpinBox>
#include <QToolButton>
#include <QPropertyAnimation>

class AntDoubleInputNumber : public QDoubleSpinBox
{
    Q_OBJECT
        Q_PROPERTY(int buttonX READ buttonX WRITE setButtonX)

public:
    AntDoubleInputNumber(QWidget* parent = nullptr);
    ~AntDoubleInputNumber();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void stepBy(int steps) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateButtonsPosition();
    int buttonX() const;
    void setButtonX(int x);

private:
    QToolButton* m_plusBtn;
    QToolButton* m_minusBtn;
    QPropertyAnimation* m_animation;

    int m_buttonX;
};
