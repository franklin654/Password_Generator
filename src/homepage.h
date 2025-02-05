#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include "global_variables.h"
#include "hash/hash_function.h"
#include "mainwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class HomePage;
}
QT_END_NAMESPACE

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:

    /* attributes */

    Ui::HomePage *ui;
    std::vector<unsigned char> keyHash;
    MainWidget* next;

private slots:
    void authenticate();
};
#endif // HOMEPAGE_H
