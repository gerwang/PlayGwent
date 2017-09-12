#include "roomdialog.h"
#include "ui_roomdialog.h"

RoomDialog::RoomDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RoomDialog) {
    ui->setupUi(this);
    for (int i = 0; i < 30; i++) {
        ui->playerListWidget->addItem(new QListWidgetItem(QString::number(i), ui->playerListWidget));
        ui->gameListWidget->addItem(new QListWidgetItem(QString::number(i), ui->gameListWidget));
    }
    connect(ui->playerListWidget, &QListWidget::currentItemChanged, ui->gameListWidget, &QListWidget::clearSelection);
    connect(ui->gameListWidget, &QListWidget::currentItemChanged, ui->playerListWidget, &QListWidget::clearSelection);

}

RoomDialog::~RoomDialog() {
    delete ui;
}

QString RoomDialog::getSelectedPlayer() {
    const QList<QListWidgetItem *> &list = ui->playerListWidget->selectedItems();
    if (!list.empty()) {
        return list.first()->text();
    } else {
        return "";
    }
}

QString RoomDialog::getSelectedGame() {
    const QList<QListWidgetItem *> &list = ui->gameListWidget->selectedItems();
    if (!list.empty()) {
        return list.first()->text();
    } else {
        return "";
    }
}
