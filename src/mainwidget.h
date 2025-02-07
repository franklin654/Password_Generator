#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QResizeEvent>
#include "global_variables.h"
#include "createpassword.h"
#include "masterpassword.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::MainWidget *ui;
    CreatePassword* createPasswordDialog;
    Singleton* globalVariables;
    MasterPassword* masterPasswordDialog;
    QStringList fileList;
    QString Password;
    QString WebSiteName;
    QString userName;

    void load_files();
    bool getPasswordFromFile(QString fileName);

private slots:
    void getSelectedPassword();
    void editSelectedPassword();
    void clearSelection();
    void filterFiles(QString text);

};

#endif // MAINWIDGET_H
