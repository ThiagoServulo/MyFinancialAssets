#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splashScreen = new QSplashScreen();
    splashScreen->setPixmap(QPixmap(":/images/splash.jfif"));
    splashScreen->show();
    QThread::msleep(500);

    MainWindow *mainWindow = new MainWindow();
    QThread::msleep(500);
    splashScreen->close();
    mainWindow->show();

    return a.exec();
}
