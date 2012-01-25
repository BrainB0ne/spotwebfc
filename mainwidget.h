#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

#define CONTENTS_COLUMN_TYPE   0
#define CONTENTS_COLUMN_FILTER 1

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
    void updateFilterItem(FilterTreeWidgetItem *selectedFilterItem);
    void updateContentsTree(FilterTreeWidgetItem *selectedFilterItem);
    void filtersTreeEmptyCheck();

private slots:
    void slotOpenButtonClicked();
    void slotSaveButtonClicked();
    void slotSaveAsButtonClicked();
    void slotAboutButtonClicked();
    void slotAddToolButtonClicked();
    void slotRemoveToolButtonClicked();
    void slotClearAllToolButtonClicked();

    void slotContentsTreeWidgetItemChanged(QTreeWidgetItem* item);
    void slotContentsTreeWidgetItemCollapsed();
    void slotContentsTreeWidgetItemExpanded();

    void slotFiltersTreeWidgetContextMenu(const QPoint& pos);
    void slotFiltersTreeWidgetItemDoubleClicked(QTreeWidgetItem* item);
    void slotFiltersTreeWidgetItemSelectionChanged();
    void slotShowFilterProperties();

private:
    Ui::MainWidget *ui;

    FilterTreeWidgetItem* m_pCurrentFilterItem;
    FilterTreeWidgetItem* m_pPreviousFilterItem;
};

#endif // MAINWIDGET_H
