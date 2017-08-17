#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWindow w;
    w.setGeometry(200, 200, 400, 500);
    w.show();

    return app.exec();
}
