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

    /**
     * @brief load_files get all the stored passwords
     */
    void load_files();

    /**
     * @brief getPasswordFromFile get the decrypted password
     * @param fileName password name
     * @return returns if password is found and decrypted or not
     */
    bool getPasswordFromFile(QString fileName);

private slots:
    void getSelectedPassword();
    void editSelectedPassword();
    void clearSelection();
    void searchPasswords(QString text);

};

#endif // MAINWIDGET_H
