#ifndef CREATEPASSWORD_H
#define CREATEPASSWORD_H

#include <QDialog>

namespace Ui {
class CreatePassword;
}

class CreatePassword : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePassword(QWidget *parent = nullptr);
    ~CreatePassword();

private:
    Ui::CreatePassword *ui;
};

#endif // CREATEPASSWORD_H
