#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginDialog) {
    ui->setupUi(this);

}

LoginDialog::~LoginDialog() {
    delete ui;
}

QString LoginDialog::getUserName() {
    return ui->usernameLineEdit->text();
}

QString LoginDialog::getPassword() {
    return ui->passwordLineEdit->text();
}
