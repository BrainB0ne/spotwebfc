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

#ifndef NEWFILTERDIALOG_H
#define NEWFILTERDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class NewFilterDialog;
}

class NewFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFilterDialog(QWidget *parent = 0);
    ~NewFilterDialog();

    QString getName() {return m_Name;}
    QString getIconName() {return m_IconName;}
    QIcon getIcon() {return m_Icon;}

private slots:
    void accept();

private:
    Ui::NewFilterDialog *ui;

    QString m_Name;
    QString m_IconName;
    QIcon m_Icon;
};

#endif // NEWFILTERDIALOG_H
