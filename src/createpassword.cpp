#include "createpassword.h"
#include "ui_createpassword.h"
#include "Password_Utilities/password.h"
#include "cipher/encrypt.h"
#include <QDialogButtonBox>
#include <json/json.h>
#include <fstream>



CreatePassword::CreatePassword(QWidget *parent)
    :QDialog(parent)
    , ui(new Ui::CreatePassword)
    , globVar(Singleton::getInstance())
{
    ui->setupUi(this);
    ui->nameLabel->setText(QString("WebSite or Application Name"));
    ui->usernameLabel->setText(QString("Username"));
    ui->lengthLabel->setText("Password Length");
    ui->strengthLabel->setText("Password Strength");
    ui->passkeyLabel->setText("");
    ui->stregnthInput->addItem("Weak Password", QVariant('w'));
    ui->stregnthInput->addItem("Strong Password", QVariant('s'));
    ui->stregnthInput->addItem("Very Strong Password", QVariant('v'));
    ui->stregnthInput->setCurrentIndex(2);
    ui->specialCharacter->setChecked(true);
    ui->number->setChecked(true);
    QObject::connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=, this](){
        storePassword();
        accept();
        QString passwdName(this->passwordName);
        clearContents();
        emit passwordGenerated(passwdName);
    });
    QObject::connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QObject::connect(ui->createButton, &QPushButton::clicked, this, &CreatePassword::generatePassword);
}

CreatePassword::~CreatePassword() {delete ui;}

void CreatePassword::clearContents()
{
    this->password.clear();
    this->passwordName.clear();
    this->userName.clear();
}

void CreatePassword::setUsernameInput(QString tmp)
{
    ui->usernameInput->setText(tmp);
}

void CreatePassword::setWebSiteNameInput(QString tmp)
{
    ui->nameInput->setText(tmp);
}

void CreatePassword::showEvent(QShowEvent *event) { this->adjustSize(); }

void CreatePassword::createNewPassword()
{
    switch(ui->stregnthInput->currentIndex()) {
    case 0:
        this->password = Pass::weak_password(ui->lengthInput->value(), ui->number->isChecked(), ui->specialCharacter->isChecked());
        break;
    case 1:
        this->password = Pass::strong_password(ui->lengthInput->value(), ui->number->isChecked(), ui->specialCharacter->isChecked());
        break;
    case 2:
        this->password = Pass::very_strong_password(ui->lengthInput->value());
        break;
    default:
        qDebug() << "I don't know what the fuck is going on" << Qt::endl;
    }
}

void CreatePassword::generatePassword()
{
    createNewPassword();
    this->passwordName = ui->nameInput->text();
    this->userName = ui->usernameInput->text();
    ui->passkeyLabel->setText(this->password);

    details = encrypt(this->password.toStdString().c_str(), globVar->keyHash.data());
    details["username"] = Json::Value(this->userName.toStdString());
    details["website"] = Json::Value(this->passwordName.toStdString());
}

void CreatePassword::hidePassword()
{
    ui->passkeyLabel->clear();
    ui->copyButton->setDisabled(true);
}

void CreatePassword::storePassword()
{
    std::filesystem::path fileName = passwordDir->filesystemAbsolutePath()/QString(this->passwordName+".json").toStdString();
    Json::StreamWriterBuilder writebuilder;
    std::unique_ptr<Json::StreamWriter> writer(writebuilder.newStreamWriter());
    std::ofstream out;
    out.open(fileName, std::ofstream::out|std::ofstream::trunc|std::ofstream::binary);
    writer->write(details, &out);
    out.close();
}



