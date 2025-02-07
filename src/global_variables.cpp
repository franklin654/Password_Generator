#include "global_variables.h"
#include "Password_Utilities/password.h"
#include "hash/hash_function.h"
#include <fstream>
#include <QString>
#include <QMessageBox>
#include <QClipboard>
#include <QGuiApplication>
#include <iostream>

Singleton::Singleton()
{
    if (this->keyHash.empty())
        getKeyHash();
}

Singleton *Singleton::getInstance()
{
    if (instancePtr != nullptr)
        return instancePtr;

    instancePtr = new Singleton;
    return instancePtr;
}

void Singleton::getKeyHash() {
    if (!hashFile->exists()) {
        setKeyHash();
        return;
    }
    std::ifstream in(hashFile->filesystemFileName(), std::ios_base::in|std::ios_base::binary);
    unsigned char buff[32];
    in.read(reinterpret_cast<char*>(buff), 32*sizeof(char));
    if (in.gcount() != 32) {
        setKeyHash();
        return;
    }
    keyHash.assign(buff, buff+32);
    in.close();
    return;
}

void Singleton::setKeyHash() {
    QString Password = Pass::strong_password();
    QGuiApplication::clipboard()->setText(Password);
    for(auto i: hash_calculator(Password.toStdString().c_str()))
        keyHash.push_back(i);
    std::ofstream out(hashFile->filesystemFileName(), std::ios_base::out|std::ios_base::binary|std::ios_base::trunc);
    for(auto ch:keyHash)
        out << ch;
    out.close();
    QMessageBox msgBox(QMessageBox::Icon::Information,
                       "No Master Password Exist",
                       "Master Password Generated: "+Password,
                       QMessageBox::Ok);
    msgBox.setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);
    msgBox.exec();
}
