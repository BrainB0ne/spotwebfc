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

#ifndef FILTERPROPERTIESDIALOG_H
#define FILTERPROPERTIESDIALOG_H

#include <QDialog>

class FilterTreeWidgetItem;

namespace Ui {
class FilterPropertiesDialog;
}

class FilterPropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FilterPropertiesDialog(QWidget *parent = 0);
    ~FilterPropertiesDialog();

    void initialize();

    void setFilterItem(FilterTreeWidgetItem* item) {m_pFilterItem = item;}
    
private slots:
    void accept();

private:
    Ui::FilterPropertiesDialog *ui;

    FilterTreeWidgetItem* m_pFilterItem;
};

#endif // FILTERPROPERTIESDIALOG_H
