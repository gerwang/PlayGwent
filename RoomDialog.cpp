#include <QtCore/QJsonArray>
#include <QDebug>
#include "RoomDialog.h"
#include "ui_roomdialog.h"

RoomDialog::RoomDialog(const QJsonObject &content, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RoomDialog) {
    ui->setupUi(this);
    connect(ui->playerListWidget, &QListWidget::currentItemChanged, ui->gameListWidget, &QListWidget::clearSelection);
    connect(ui->gameListWidget, &QListWidget::currentItemChanged, ui->playerListWidget, &QListWidget::clearSelection);

    if (content["command"].toString() != "respond_list") {
        qWarning() << "not proper command!";
    }
    QJsonArray jsonHallPlayers = content["hallplayers"].toArray();
    for (int i = 0; i < jsonHallPlayers.size(); i++) {
        ui->playerListWidget->addItem(jsonHallPlayers[i].toString());
    }
    QJsonArray jsonGameArray = content["ongames"].toArray();
    for (int i = 0; i < jsonGameArray.size(); i++) {
        ui->gameListWidget->addItem(jsonGameArray[i].toString());
    }

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
