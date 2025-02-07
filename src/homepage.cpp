#include "homepage.h"
#include "./ui_homepage.h"
#include "cipher/encrypt.h"
#include "hash/hash_function.h"
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QMessageBox>
#include <QClipboard>
#include <regex>
#include <json/json.h>
#include <iostream>
#include <fstream>

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
    QObject::connect(ui->dump, &QPushButton::clicked, this, &HomePage::dumpPassword);
    next->hide();
}

HomePage::~HomePage()
{
    delete ui;
    delete next;
}

Json::Value HomePage::writePasswordtoFile(QString fileName)
{
    /* Get the File Path */
    std::filesystem::path filePath = passwordDir->filesystemAbsolutePath()/fileName.toStdString();

    /* Get the JSON Object from JSON doc */
    Json::Value jsondoc;
    std::ifstream in;
    in.open(filePath);
    Json::CharReaderBuilder readbuilder;
    JSONCPP_STRING errs;
    if (!Json::parseFromStream(readbuilder, in, &jsondoc, &errs)) {
        std::cout << errs << std::endl;
        return Json::Value(Json::nullValue);
    }
    in.close();

    QFile::remove(QString::fromStdString(filePath.string()));

    /* Decrypt the ciphertext */
    QString decryptedPassword = QString::fromStdString(decrypt(jsondoc, globalVariables->keyHash.data()));

    jsondoc["plaintext"] = Json::Value(decryptedPassword.toStdString());
    return jsondoc;
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
    if (inputHash == globalVariables->keyHash) {
        /*
         * Launch the Main Window
         */
        next->show();
        ui->centralwidget->hide();
    }
}

void HomePage::dumpPassword()
{
    QStringList filters;
    filters << "*.json";
    passwordDir->setNameFilters(filters);
    QStringList fileList = passwordDir->entryList(QDir::Filter::Files|QDir::Filter::NoDotAndDotDot);
    if(fileList.isEmpty()) {
        std::cerr << "[!] No password files found" << std::endl;
        return;
    }

    std::ofstream out;
    out.open(QDir::homePath().toStdString()+"\\"+"dumped_passwords.txt", std::ios::out|std::ios::trunc);

    for (auto it=fileList.begin();it!=fileList.end();++it) {
        // dump password
        Json::Value details = writePasswordtoFile(*it);
        out << "Website:\t"
            << details["website"].asString()
            << "\tUsername:\t"
            << details["username"].asString()
            << "\tpassword:\t"
            << details["plaintext"]
            << "\n";
    }
    out.close();
    hashFile->remove();
}

