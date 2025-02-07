#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "createpassword.h"
#include <filesystem>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include "cipher/encrypt.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , globalVariables(Singleton::getInstance())
    , createPasswordDialog(new CreatePassword(this))
    , masterPasswordDialog(new MasterPassword(this))
{
    ui->setupUi(this);
    ui->copy->setEnabled(false);
    ui->pushButton->setEnabled(true);
    ui->displayLabel->hide();
    this->layout()->setAlignment(ui->pushButton, Qt::AlignmentFlag::AlignHCenter);
    QObject::connect(ui->create, &QPushButton::clicked, createPasswordDialog, &CreatePassword::open);
    QObject::connect(this->createPasswordDialog, &CreatePassword::passwordGenerated, this, [this](QString a){
        qDebug() << a;
        this->ui->passwordsListWidget->addItem(a);
    });
    QObject::connect(ui->view, &QPushButton::clicked, this, &MainWidget::getSelectedPassword);
    QObject::connect(ui->edit, &QPushButton::clicked, this, &MainWidget::editSelectedPassword);
    QObject::connect(ui->clear, &QPushButton::clicked, this, &MainWidget::clearSelection);
    QObject::connect(ui->searchBar, &QLineEdit::textEdited, this, &MainWidget::filterFiles);
    QObject::connect(ui->pushButton, &QPushButton::clicked, masterPasswordDialog, &MasterPassword::open);
}

MainWidget::~MainWidget()
{
    delete ui;
    delete createPasswordDialog;
}

void MainWidget::load_files()
{
    ui->passwordsListWidget->clear();
    QStringList filters;
    filters << "*.json";
    passwordDir->setNameFilters(filters);
    QStringList fileList = passwordDir->entryList(QDir::Filter::Files|QDir::Filter::NoDotAndDotDot);
    if(fileList.isEmpty())
        return;
    fileList.replaceInStrings(".json", "");
    ui->passwordsListWidget->addItems(fileList);
}

bool MainWidget::getPasswordFromFile(QString fileName)
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
        return false;
    }
    in.close();

    /* Decrypt the ciphertext */
    Password = QString::fromStdString(decrypt(jsondoc, globalVariables->keyHash.data()));
    if (Password.isEmpty())
        return false;
    userName = QString::fromStdString(jsondoc["username"].asString());
    WebSiteName = QString::fromStdString(jsondoc["website"].asString());

    return true;
}

void MainWidget::getSelectedPassword()
{
    QList<QListWidgetItem*> selectedItem = ui->passwordsListWidget->selectedItems();
    if (selectedItem.isEmpty())
        return;
    if (!this->getPasswordFromFile(selectedItem[0]->text()+".json"))
        return;
    ui->displayLabel->setText("Username: "+this->userName+" Password: "+this->Password);
    ui->displayLabel->show();
    ui->displayLabel->adjustSize();
    ui->copy->setEnabled(true);
}

void MainWidget::editSelectedPassword()
{
    QList<QListWidgetItem*> selectedItem = ui->passwordsListWidget->selectedItems();
    if (selectedItem.isEmpty())
        return;
    std::filesystem::path filePath = passwordDir->filesystemAbsolutePath()/(selectedItem[0]->text()+".json").toStdString();

    /* Get the JSON Object from JSON doc */
    Json::Value jsondoc;
    std::ifstream in;
    in.open(filePath);
    Json::CharReaderBuilder readbuilder;
    JSONCPP_STRING errs;
    if (!Json::parseFromStream(readbuilder, in, &jsondoc, &errs)) {
        std::cout << errs << std::endl;
        return;
    }
    in.close();

    this->createPasswordDialog->setUsernameInput(QString::fromStdString(jsondoc["username"].asString()));
    this->createPasswordDialog->setWebSiteNameInput(QString::fromStdString(jsondoc["website"].asString()));
    this->createPasswordDialog->open();
}

void MainWidget::clearSelection()
{
    ui->displayLabel->clear();
    this->Password.clear();
    this->WebSiteName.clear();
    this->userName.clear();
}

void MainWidget::filterFiles(QString text)
{
    ui->passwordsListWidget->clear();
    QStringList filters;
    filters << text+"*.json";
    passwordDir->setNameFilters(filters);
    QStringList fileList = passwordDir->entryList(QDir::Filter::Files|QDir::Filter::NoDotAndDotDot);
    if(fileList.isEmpty())
        return;
    fileList.replaceInStrings(".json", "");
    ui->passwordsListWidget->addItems(fileList);
}

void MainWidget::showEvent(QShowEvent *event)
{
    load_files();
    QMainWindow* coverWindow = qobject_cast<QMainWindow*>(this->parent());
    coverWindow->resize(this->size());
}
