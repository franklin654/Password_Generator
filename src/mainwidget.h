#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QResizeEvent>
#include "global_variables.h"
#include "createpassword.h"

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
    QStringList fileList;
    CreatePassword* createPasswordDialog;
    QString Password;
    QString WebSiteName;
    QString userName;
    Singleton* globalVariables;

    void load_files();
    bool getPasswordFromFile(QString fileName);

private slots:
    void getSelectedPassword();
    void editSelectedPassword();
    void clearSelection();
    void filterFiles(QString text);

};

#endif // MAINWIDGET_H
