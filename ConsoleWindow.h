#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <QMainWindow>
#include "AbstractUI.h"

namespace Ui {
    class ConsoleWindow;
}

class ConsoleWindow : public QMainWindow {
Q_OBJECT

public:
    explicit ConsoleWindow(QWidget *parent = 0);

    ~ConsoleWindow() override;

    Ui::ConsoleWindow *ui;
};

#endif // CONSOLEUI_H
