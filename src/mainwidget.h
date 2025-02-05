#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QResizeEvent>

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
    QDialog* createPasswordDialog;

    void load_files();

};

#endif // MAINWIDGET_H
