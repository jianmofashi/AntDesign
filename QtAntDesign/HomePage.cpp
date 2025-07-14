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
#include "SkeletonWidget.h"
#include "AntButton.h"
#include "NotificationManager.h"
#include "AntInputNumber.h"

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

	// 页面内容布局
	QVBoxLayout* pageLay = new QVBoxLayout(w1);
	pageLay->setSpacing(0);
	pageLay->setContentsMargins(0, 0, 0, 0);

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
	MaterialSpinner* spinner = new MaterialSpinner(4, DesignSystem::instance()->primaryColor(), this);
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

	// 骨架屏
	AntButton* skeletonDescBtn = new AntButton("骨架屏启动", 12, w1);
	skeletonDescBtn->setFixedSize(120, 50);
	QHBoxLayout* row3Layout = new QHBoxLayout(w1);
	row3Layout->setSpacing(8);
	row3Layout->setContentsMargins(0, 0, 0, 0);

	QList<QIcon> icons = {
		QIcon(":/Imgs/undraw_book-lover_f1dq.svg"),
		QIcon(":/Imgs/undraw_developer-avatar_f6ac.svg"),
		QIcon(":/Imgs/undraw_loving-it_hspq.svg"),
		QIcon(":/Imgs/undraw_stock-prices_8nuz.svg")
	};

	QList<QLabel*> iconLabels;
	QList<SkeletonWidget*> skeletons;

	for (const QIcon& icon : icons)
	{
		SkeletonWidget* skeleton = new SkeletonWidget(QSize(280, 200), 8, this);
		row3Layout->addWidget(skeleton);
		skeletons.append(skeleton);  // 保存骨架指针

		QLabel* iconLabel = new QLabel(this);
		iconLabel->setAlignment(Qt::AlignCenter);
		iconLabel->setPixmap(icon.pixmap(300, 200));
		iconLabel->hide();  // 初始隐藏
		iconLabels.append(iconLabel);
		row3Layout->addWidget(iconLabel);
	}

	connect(skeletonDescBtn, &AntButton::clicked, this, [this, skeletonDescBtn, iconLabels, skeletons]()
		{
			skeletonDescBtn->setEnabled(false);  // 禁用按钮，防止重复点击
			// 启动骨架动画
			for (int i = 0; i < iconLabels.size(); ++i)
			{
				iconLabels[i]->hide();  // 隐藏真实图标
			}

			for (SkeletonWidget* skeleton : skeletons)
			{
				skeleton->startSkeleton();
			}

			// 延迟2秒后隐藏骨架，显示图标
			QTimer::singleShot(3000, this, [=]()
				{
					for (int i = 0; i < skeletons.size(); ++i)
					{
						skeletons[i]->stopSkeleton();   // 停止并隐藏骨架
					}

					for (int i = 0; i < iconLabels.size(); ++i)
					{
						iconLabels[i]->show();
					}
					skeletonDescBtn->setEnabled(true);
				});
		});

	// 任务通知
	QHBoxLayout* row4Layout = new QHBoxLayout(w1);
	row4Layout->setSpacing(12);
	row4Layout->setContentsMargins(0, 0, 0, 0);

	AntButton* taskBtn = new AntButton("任务通知", 12, w1);
	taskBtn->setFixedSize(120, 50);
	connect(taskBtn, &AntButton::clicked, this, [this]()
		{
			NotificationManager::instance()->showNotification("任务通知");
		});

	// 数字输入框
	AntInputNumber* spinBox = new AntInputNumber(this);
	spinBox->setFixedSize(85, 35); // 设置最小尺寸
	spinBox->setRange(0, 100);	// 调整数值范围
	spinBox->setValue(10);		// 设置初始值
	QLabel* spinBoxLabel = new QLabel("数字输入框", this);
	row4Layout->addWidget(taskBtn);
	row4Layout->addWidget(spinBoxLabel);
	row4Layout->addWidget(spinBox);
	row4Layout->addStretch();

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
	pageLay->addSpacing(20);
	pageLay->addLayout(row1Layout);
	pageLay->addSpacing(20);
	pageLay->addLayout(row2Layout);
	pageLay->addWidget(skeletonDescBtn);
	pageLay->addLayout(row3Layout);
	pageLay->addLayout(row4Layout);
	pageLay->addStretch();
}

HomePage::~HomePage()
{
}