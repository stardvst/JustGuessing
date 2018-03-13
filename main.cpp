#include "src/JustGuessing.h"
#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app { argc, argv };
	app.setFont(QFont("Segoe UI"));

	// create the game instance
	static auto pGame = &JustGuessing::getInstance();
	pGame->show();

	return app.exec();
}
