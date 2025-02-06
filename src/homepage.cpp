#include "homepage.h"
#include "./ui_homepage.h"
#include "hash/hash_function.h"
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QMessageBox>
#include <QClipboard>
#include <regex>


HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
    , next(new MainWidget(this))
    , globalVariables(Singleton::getInstance())
{
    ui->setupUi(this);
    this->setWindowTitle("Password Manager");
    ui->inputMasterPasskey->setMaxLength(32);
    ui->inputMasterPasskey->setEchoMode(QLineEdit::Password);
    QObject::connect(ui->login, &QPushButton::clicked, this, &HomePage::authenticate);
    QObject::connect(ui->inputMasterPasskey, &QLineEdit::returnPressed, this, &HomePage::authenticate);
    next->hide();
}

HomePage::~HomePage()
{
    delete ui;
    delete next;
}

void HomePage::authenticate()
{
    QString passwd = ui->inputMasterPasskey->text();
    re->setPattern("^[ \t\n]+|[' '\t\n\f\r]+$");
    passwd.remove(*re);
    if (!std::regex_search(passwd.toStdString(), std::regex{"(?=.*[[:lower:]])(?=.*[[:upper:]])(?=.*[[:digit:]])(?=.*[[:punct:]])"}) || passwd.length() < 8) {
        QMessageBox msgBox(QMessageBox::Icon::Information,
                           "Invalid Password",
                           "Password must be Minimum 8 characters has at least one letter, one number and one special character",
                           QMessageBox::Ok);
        msgBox.exec();
        ui->inputMasterPasskey->clear();
        return;
    }
    std::vector<unsigned char> inputHash = hash_calculator((const char*)passwd.toStdString().c_str());
    if (1||inputHash == globalVariables->keyHash) {
        /*
         * Launch the Main Window
         */
        next->show();
        ui->centralwidget->hide();
    }
}
