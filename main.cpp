#include "ConsoleWindow.h"
#include <QApplication>
#include "ConsoleView.h"
#include "GameController.h"
#include "ConsoleView.h"
#include "LoginDialog.h"
#include "RoomDialog.h"
#include "GraphicsUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GameController::controller()->start();
    return a.exec();
}