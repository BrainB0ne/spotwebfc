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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::initialize()
{
    QStringList languages;
    languages << "English" << "Nederlands";

    ui->languageComboBox->insertItems(0, languages);
    ui->languageComboBox->setCurrentIndex(ui->languageComboBox->findText(m_Language));
}

void SettingsDialog::accept()
{
    if(m_Language != ui->languageComboBox->currentText())
    {
        QMessageBox::information(this, tr("Language changed"),
                                 tr("Please restart Spotweb Filter Creator to activate the new language"));
    }

    m_Language = ui->languageComboBox->currentText();

    QDialog::accept();
}
