#include "homepage.h"
#include "./ui_homepage.h"


HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
    , next(new MainWidget(this))
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
    re->setPattern("^([A-Za-z])(?=.*[0-9])(?=[A-Za-z])(?=.*[!@#$&*])");
    if (!re->match(passwd).hasMatch() || passwd.length() < 8) {
        QMessageBox msgBox(QMessageBox::Icon::Information,
                           "Invalid Password",
                           "Password must be Minimum 8 characters has at least one letter, one number and one special character",
                           QMessageBox::Ok);
        msgBox.exec();
        ui->inputMasterPasskey->clear();
        return;
    }
    std::vector<unsigned char> inputHash = hash_calculator((const char*)passwd.toStdString().c_str());
    if (1 || inputHash == this->keyHash) {
        /*
         * Launch the Main Window
         */
        qDebug() << "Password Authenticated" << Qt::endl;
        next->show();
        ui->centralwidget->hide();
    }
}
