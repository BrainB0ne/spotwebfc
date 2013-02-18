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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class MainWidget;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void initialize();
    void retranslate();
    void fillLanguages();

    void setLanguage(const QString& language) {m_Language = language;}
    QString getLanguage() {return m_Language;}
    
protected:
    void changeEvent(QEvent* event);

private slots:
    void accept();
    void reject();
    void slotLanguageComboBoxActivated(const QString& language);

private:
    Ui::SettingsDialog *ui;

    QString m_Language;
    MainWidget* m_pMainWidget;
};

#endif // SETTINGSDIALOG_H
