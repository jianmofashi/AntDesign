#include "HomePage.h"
#include <QVBoxLayout>
#include <QLabel>
#include "MaterialTabWidget.h"
#include "AntToggleButton.h"
#include "SlideStackedWidget.h"
#include "MaterialProgressBar.h"
#include <QList>
#include <QTimer>
#include "MaterialSpinner.h"
#include "AntRadioButton.h"
#include "AntSlider.h"
#include "NoDataWidget.h"
#include "AnimatedNumber.h"

HomePage::HomePage(QWidget* parent)
	: QWidget(parent)
{
	// 标签页
	MaterialTabWidget* tabWidget = new MaterialTabWidget(this);
	QWidget* w1 = new QWidget(this);
	tabWidget->addTab(w1, "常用控件");
	tabWidget->addTab(new QWidget(this), "数据统计");
	NoDataWidget* noData = new NoDataWidget(this);
	tabWidget->addTab(noData, "暂无数据");
	tabWidget->getWidget(1)->setStyleSheet("background-color: lightblue;");
	tabWidget->getWidget(2)->setStyleSheet("background-color: gray;");

	// 主布局
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(tabWidget);

	// 页面布局
	QVBoxLayout* w1Lay = new QVBoxLayout(w1);
	w1Lay->setSpacing(0);
	w1Lay->setContentsMargins(8, 8, 8, 8);

	// 添加标题 + 开关按钮
	QHBoxLayout* row1Layout = new QHBoxLayout();
	row1Layout->setSpacing(10); // label 和按钮之间的间距

	QHBoxLayout* row2Layout = new QHBoxLayout();
	row2Layout->setSpacing(10);

	QStringList nameList;
	QList<QLabel*>labelList;
	nameList << "开关按钮" << "进度条" << "水平滑动条" << "统计数值";
	for (auto& str : nameList)
	{
		QLabel* nameLabel = new QLabel(str, this);
		nameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
		nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		labelList.append(nameLabel);
	}
	// 开关按钮
	AntToggleButton* toggleBtn = new AntToggleButton(QSize(52, 26), this);
	toggleBtn->setShowText(true);

	// 进度条
	MaterialProgressBar* progress = new MaterialProgressBar(this);
	progress->setFixedHeight(10);
	// 启动测试模式
	progress->startTestPattern();
	// 圆形进度条
	MaterialSpinner* spinner = new MaterialSpinner(4, QColor(22, 119, 255), this);
	spinner->setFixedSize(42, 42);

	// 单选按钮
	AntRadioButton* radioBtn1 = new AntRadioButton(this);
	radioBtn1->setText("单选按钮1");
	AntRadioButton* radioBtn2 = new AntRadioButton(this);
	radioBtn2->setText("单选按钮2");

	// 水平滑动条
	AntSlider* slider = new AntSlider(0, 100, 30, this);
	slider->setFixedWidth(230);

	// 统计数值
	AnimatedNumber* animNum = new AnimatedNumber(this);
	animNum->setTextWidth(120);
	animNum->setFontSzie(16);
	animNum->animateTo(12345);

	// 添加到水平布局
	row1Layout->addWidget(labelList[0]);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(toggleBtn);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(labelList[1]);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(progress);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(spinner);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(radioBtn1);
	row1Layout->addWidget(radioBtn2);
	row1Layout->addSpacing(20);
	row1Layout->addWidget(labelList[2]);
	row1Layout->addWidget(slider);
	row1Layout->addStretch();  // 让它贴左边

	row2Layout->addWidget(labelList[3]);
	row2Layout->addWidget(animNum);
	row2Layout->addStretch();

	// 添加到页面布局
	w1Lay->addSpacing(20);
	w1Lay->addLayout(row1Layout);
	w1Lay->addSpacing(20);
	w1Lay->addLayout(row2Layout);
	w1Lay->addStretch();
}

HomePage::~HomePage()
{
}