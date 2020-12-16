#include <QApplication>

#include "interface/Application.hpp"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	Application app;
	app.resize(1920, 1080);
	app.show();
	 
	return a.exec();
}