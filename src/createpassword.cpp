#include "createpassword.h"
#include "ui_createpassword.h"

CreatePassword::CreatePassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePassword)
{
    ui->setupUi(this);
}

CreatePassword::~CreatePassword()
{
    delete ui;
}
