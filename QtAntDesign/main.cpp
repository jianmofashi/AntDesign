#include<QApplication>
#include<QtAntDesign.h>
#include<QFontDatabase>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	// 加载google字体
	int fontId = QFontDatabase::addApplicationFont(":/fonts/NotoSansSC-Regular.ttf");
	if (fontId != -1)
	{
		QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
		QFont font(family);
		font.setHintingPreference(QFont::PreferNoHinting);	// 禁用字体微调渲染保证字体清晰
		QApplication::setFont(font);
	}
	else
	{
		qWarning("字体加载失败！");
	}

	QtAntDesign w;
	w.show();
	return a.exec();
}