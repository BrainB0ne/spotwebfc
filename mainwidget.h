/*
 * Spotweb Filter Creator
 * Copyleft 2012
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * For more information about the GNU General Public License: <http://www.gnu.org/licenses/>.
 */

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
    void updateFilterItem(FilterTreeWidgetItem* selectedFilterItem);
    void updateContentsTree(FilterTreeWidgetItem* selectedFilterItem);
    QString findContentByFilter(const QString& filter);
    void createFilterIdentification();
    void filtersTreeEmptyCheck();
    int  saveFilterFile(const QString& fileName);
    FilterTreeWidgetItem* findFilterItemByParentID(const QString& parentID);

protected:
    void closeEvent(QCloseEvent* event);

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
