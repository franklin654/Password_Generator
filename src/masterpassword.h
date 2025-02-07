#ifndef MASTERPASSWORD_H
#define MASTERPASSWORD_H

#include <QDialog>
#include "global_variables.h"

namespace Ui {
class MasterPassword;
}

class MasterPassword : public QDialog
{
    Q_OBJECT

public:
    explicit MasterPassword(QWidget *parent = nullptr);
    ~MasterPassword();

private:
    Ui::MasterPassword *ui;
    Singleton* globalVariables;

    void reEncryptPassword(std::vector<unsigned char>newHashKey, std::string filename);

private slots:
    void changeMasterPassword();
};

#endif // MASTERPASSWORD_H
