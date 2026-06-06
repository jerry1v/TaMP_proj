#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Система авторизации");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("AuthApp");

    // Современный стиль
    app.setStyle("Fusion");

    MainWindow w;
    w.show();

    return app.exec();
}
