#include "mainwindow.h"
#include "mapwidget.h"
#include <QApplication>
#include <QDebug>
#include <QPixmap>
#include <QSplashScreen>
#include<iostream>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/images/IMG/logo.jpg");  //Create QPixmap, Set Startup Logo
    QSplashScreen splash(pixmap);  //Create QSplashScreen
    splash.show();  //Show Startup Logo
    a.processEvents();  //Keep Application Responsive
    qDebug() << "Program is running";
    MainWindow w;
    w.show();
    splash.finish(&w);  //Stop Showing Startup Logo
    return a.exec();
}
