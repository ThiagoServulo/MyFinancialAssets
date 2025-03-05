#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QThread>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splashScreen = new QSplashScreen();
    splashScreen->setPixmap(QPixmap(":/images/splash.jfif"));
    splashScreen->show();
    QThread::msleep(500);

    // Get .ini file
    QString iniFilePath = QCoreApplication::applicationDirPath() + "/config.ini";

    // Check if the .ini file exists
    if (!QFile::exists(iniFilePath))
    {
        // If the file doesn't exist, show an error message box and close the application
        QMessageBox::critical(nullptr, "Error", "O arquivo de configuração (config.ini) não existe. Fcehando a aplicação.");
        QCoreApplication::quit();
        return 0;
    }

    MainWindow *mainWindow = new MainWindow();
    QThread::msleep(500);
    splashScreen->close();
    mainWindow->show();

    return a.exec();
}
