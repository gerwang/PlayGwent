#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
    class RoomDialog;
}

class RoomDialog : public QDialog {
Q_OBJECT

public:
    explicit RoomDialog(const QJsonObject &content, QWidget *parent = 0);

    ~RoomDialog() override;

    QString getSelectedPlayer();

    QString getSelectedGame();

private:
    Ui::RoomDialog *ui;
};

#endif // ROOMDIALOG_H
