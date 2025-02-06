#ifndef CREATEPASSWORD_H
#define CREATEPASSWORD_H

#include "global_variables.h"
#include <QDialog>
#include <QLineEdit>
#include <json/json.h>

namespace Ui {
class CreatePassword;
}

class CreatePassword : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePassword(QWidget *parent = nullptr);
    ~CreatePassword();
    void clearContents();
    void setUsernameInput(QString tmp);
    void setWebSiteNameInput(QString tmp);


signals:
    void passwordGenerated(QString PasswordName);

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::CreatePassword *ui;
    QString passwordName;
    QString userName;
    QString password;
    Singleton* globVar;
    Json::Value details;

    void createNewPassword();
    void storePassword();

private slots:
    void generatePassword();
    void hidePassword();

};

#endif // CREATEPASSWORD_H
