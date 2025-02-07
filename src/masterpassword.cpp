#include "masterpassword.h"
#include "./ui_masterpassword.h"
#include "cipher/encrypt.h"
#include "hash/hash_function.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <QMessageBox>
#include <regex>

MasterPassword::MasterPassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MasterPassword)
    , globalVariables(Singleton::getInstance())
{
    ui->setupUi(this);
    ui->label->setText("Enter Master Password: ");
    this->layout()->setAlignment(ui->Submit, Qt::AlignmentFlag::AlignHCenter);
    QObject::connect(ui->Submit, &QPushButton::clicked, this, &MasterPassword::changeMasterPassword);
}

MasterPassword::~MasterPassword()
{
    delete ui;
}

void MasterPassword::reEncryptPassword(std::vector<unsigned char> newHashKey, std::string filename)
{
    std::filesystem::path filepath = passwordDir->filesystemAbsolutePath()/filename;

    /* Get the JSON Object from JSON doc */
    Json::Value jsondoc;
    std::ifstream in;
    in.open(filepath);
    Json::CharReaderBuilder readbuilder;
    JSONCPP_STRING errs;
    if (!Json::parseFromStream(readbuilder, in, &jsondoc, &errs)) {
        std::cout << errs << std::endl;
        return;
    }
    in.close();

    /* Decrypt the ciphertext */
    std::string password = decrypt(jsondoc, this->globalVariables->keyHash.data());

    /* Encrypt the plaintext using new key */
    Json::Value details = encrypt(password.c_str(), newHashKey.data());
    details["username"] = jsondoc["username"];
    details["website"] = jsondoc["website"];

    /* store them again in the file */
    Json::StreamWriterBuilder writebuilder;
    std::unique_ptr<Json::StreamWriter> writer(writebuilder.newStreamWriter());
    std::ofstream out;
    out.open(filename, std::ofstream::out|std::ofstream::trunc|std::ofstream::binary);
    writer->write(details, &out);
    out.close();
}

void MasterPassword::changeMasterPassword()
{
    /* Get Password and remove trailing spaces and blanks */
    QString passwd = ui->passkeyInput->text();
    re->setPattern("^[ \t\n]+|[' '\t\n\f\r]+$");
    passwd.remove(*re);

    /* validate the password */
    if (!std::regex_search(passwd.toStdString(), std::regex{"(?=.*[[:lower:]])(?=.*[[:upper:]])(?=.*[[:digit:]])(?=.*[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~])"}) || passwd.length() < 8) {
        QMessageBox msgBox(QMessageBox::Icon::Information,
                           "Invalid Password",
                           "Password must be Minimum 8 characters has at least one letter, one number and one special character",
                           QMessageBox::Ok);
        msgBox.exec();
        ui->passkeyInput->clear();
        return;
    }
    std::vector<unsigned char> inputHash = hash_calculator((const char*)passwd.toStdString().c_str());
    if (inputHash == globalVariables->keyHash) {
        // throw password cannot be the same as previous one error
        std::cerr << "[!] You cannot use the same password" << std::endl;
        return;
    }

    /* Store the validated Password in file */
    std::ofstream out(hashFile->filesystemFileName(), std::ios_base::out|std::ios_base::binary|std::ios_base::trunc);
    for(auto ch:inputHash)
        out << ch;
    out.close();
    ui->passkeyInput->clear();
    globalVariables->keyHash.clear();
    globalVariables->keyHash = inputHash;

    /* Update the ciphertext stored */
    QStringList filters;
    filters << "*.json";
    passwordDir->setNameFilters(filters);
    QStringList fileList = passwordDir->entryList(QDir::Filter::Files|QDir::Filter::NoDotAndDotDot);
    if(fileList.isEmpty()) {
        std::cerr << "[!] No password files found" << std::endl;
        return;
    }
    for (auto it=fileList.begin();it != fileList.end();++it) {
        reEncryptPassword(inputHash, it->toStdString());
    }
}
