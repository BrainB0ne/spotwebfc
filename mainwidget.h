#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

class FilterTreeWidgetItem;

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
    void slotContentsTreeWidgetItemCollapsed();
    void slotContentsTreeWidgetItemExpanded();

    void slotFiltersTreeWidgetContextMenu(const QPoint& pos);
    void slotShowFilterProperties();

private:
    Ui::MainWidget *ui;

    FilterTreeWidgetItem* m_pCurrentFilterItem;
};

#endif // MAINWIDGET_H
