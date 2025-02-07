#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include "mainwidget.h"
#include "global_variables.h"
#include <json/json.h>

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
    MainWidget* next;
    Singleton* globalVariables;

    Json::Value writePasswordtoFile(QString fileName);

private slots:
    void authenticate();
    void dumpPassword();
};
#endif // HOMEPAGE_H
