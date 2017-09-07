#include "ConsoleWindow.h"
#include "ui_consoleui.h"

ConsoleWindow::ConsoleWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ConsoleWindow) {
    ui->setupUi(this);
}

ConsoleWindow::~ConsoleWindow() {
    delete ui;
}

