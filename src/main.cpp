#include "mainwindow.h"


#include <QApplication>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>

#include "dms_horizontaldraglabel.h"
#include "dms_horizontaldragwidget.h"
#include "virtualwinfuncs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.resize(600, 480);
    w.showNormal();

    w.applyCurrentSources();
    w.resize(600, 480);
    w.show();

    return a.exec();
}
