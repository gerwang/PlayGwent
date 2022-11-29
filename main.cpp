#include <QApplication>
#include "ConsoleView.h"
#include "GameController.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet("QPushButton:hover:!pressed{border-image:url(:assets/ui/menu_active2.tex.png)}"
                    "QPushButton:pressed{border-image:url(:assets/ui/menu_hit2.tex.png)}"
                    "QPushButton {border-image: url(:assets/ui/menu_inactive2.tex.png); padding: 5px; color: rgb(201, 199, 197)}"
    );
    GameController::controller()->start();
    return a.exec();
}