#include "StandardDialogPage.h"
#include "StyleSheet.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QScrollArea>
#include <QFrame>
#include <QApplication>

StandardDialogPage::StandardDialogPage(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("standardDialog");
	setStyleSheet(StyleSheet::standardDialogQss());

	// 主垂直布局
	QVBoxLayout* mainLay = new QVBoxLayout(this);
	mainLay->setContentsMargins(18, 28, 18, 28);
	mainLay->setSpacing(0);

	// ========== 1. 标题栏 ==========
	titleLabel = new QLabel("TITLE");
	QFont font;
	font.setPointSize(14);
	font.setBold(true);
	titleLabel->setFont(font);
	titleLabel->setFixedHeight(30);
	mainLay->addWidget(titleLabel);

	// ========== 2. 内容区（可滚动） ==========
	contentLabel = new QLabel("?");
	font.setPointSize(12.5);
	font.setBold(false);
	mainLay->addWidget(contentLabel);

	// ========== 3. 底部操作区 ==========
	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(); // 推到右侧
	font.setPointSize(11.5);
	font.setBold(false);
	QPushButton* cancelBtn = new QPushButton("取消");
	QPushButton* confirmBtn = new QPushButton("确定");
	cancelBtn->setFixedSize(70, 36);
	confirmBtn->setFixedSize(70, 36);

	cancelBtn->setFont(font);
	confirmBtn->setFont(font);

	cancelBtn->setObjectName("CancelButton");
	confirmBtn->setObjectName("ConfirmButton");
	cancelBtn->setStyleSheet(StyleSheet::standardDialogBtnQss());
	confirmBtn->setStyleSheet(StyleSheet::standardDialogBtnQss());

	buttonLayout->addWidget(cancelBtn);
	buttonLayout->addSpacing(10);
	buttonLayout->addWidget(confirmBtn);

	mainLay->addLayout(buttonLayout);

	// ===== 连接信号和槽：点击“确定”退出程序 =====
	connect(confirmBtn, &QPushButton::clicked, qApp, &QApplication::quit);
	connect(cancelBtn, &QPushButton::clicked, this, &StandardDialogPage::exitDialog);
}

StandardDialogPage::~StandardDialogPage()
{
}

void StandardDialogPage::setText(QString title, QString text)
{
	if (titleLabel && contentLabel)
	{
		titleLabel->setText(title);
		contentLabel->setText(text);
	}
}