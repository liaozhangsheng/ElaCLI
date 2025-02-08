#include <QApplication>

#include "ElaApplication.h"

#include "../ui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    eApp->init();

    MainWindow w;
    w.show();

    return QApplication::exec();
}
