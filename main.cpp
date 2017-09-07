#include "ConsoleWindow.h"
#include <QApplication>
#include "ConsoleView.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    ConsoleWindow w;
//    w.show();
    ConsoleView view;
    return a.exec();
}