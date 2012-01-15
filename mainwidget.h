#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
    void initialize();
    void connectSignalsSlots();
    void loadContents();

private slots:
    void slotOpenButtonClicked();
    void slotSaveButtonClicked();
    void slotSaveAsButtonClicked();
    void slotAboutButtonClicked();
    void slotAddToolButtonClicked();
    void slotRemoveToolButtonClicked();
    void slotClearAllToolButtonClicked();

    void slotContentsTreeWidgetItemChanged(QTreeWidgetItem *item, int column);
    void slotContentsTreeWidgetItemCollapsed(QTreeWidgetItem *item);
    void slotContentsTreeWidgetItemExpanded(QTreeWidgetItem *item);

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
