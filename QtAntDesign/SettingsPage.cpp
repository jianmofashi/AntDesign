#include "SettingsPage.h"
#include <QVBoxLayout>
#include <QLabel>

SettingsPage::SettingsPage(QWidget* parent)
    : QWidget(parent)
{
    QLabel* label = new QLabel("SETTINGS", this);
    QFont font = label->font();
    font.setPointSize(52);
    font.setBold(true);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setStyleSheet("background-color: #A0C4A0;");
}


SettingsPage::~SettingsPage()
{}

