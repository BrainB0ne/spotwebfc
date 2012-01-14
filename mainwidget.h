#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
private slots:
    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_saveAsButton_clicked();

    void on_addToolButton_clicked();

    void on_removeToolButton_clicked();

    void on_clearAllToolButton_clicked();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
