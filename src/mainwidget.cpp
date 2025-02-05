#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "global_variables.h"
#include "createpassword.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , createPasswordDialog(new CreatePassword(this))
{
    ui->setupUi(this);
    ui->displayLabel->
    QObject::connect(ui->create, &QPushButton::clicked, createPasswordDialog, &CreatePassword::open);
    ui->copy->setEnabled(false);

}

MainWidget::~MainWidget()
{
    delete ui;
    delete createPasswordDialog;
}

void MainWidget::load_files()
{
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
