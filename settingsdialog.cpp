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

#include "mainwidget.h"

#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    m_pMainWidget = (MainWidget*)parent;

    connect(ui->languageComboBox, SIGNAL(activated(QString)),
            this, SLOT(slotLanguageComboBoxActivated(QString)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::initialize()
{
    QStringList languages;
    languages << "English" << "Deutsch" << "Nederlands";

    ui->languageComboBox->insertItems(0, languages);
    ui->languageComboBox->setCurrentIndex(ui->languageComboBox->findText(m_Language));
}

void SettingsDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);

        // retranslate other widgets which weren't added in designer
        retranslate();
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

void SettingsDialog::retranslate()
{
    // retranslate other widgets which weren't added in designer
}

void SettingsDialog::accept()
{
    m_Language = ui->languageComboBox->currentText();

    QDialog::accept();
}

void SettingsDialog::reject()
{
    QTranslator* translator = m_pMainWidget->getTranslator();

    if(m_pMainWidget && translator)
    {
        if(m_Language == "Nederlands")
        {
            translator->load("spotwebfc_nl");
            qApp->installTranslator(translator);
        }
        else if(m_Language == "Deutsch")
        {
            translator->load("spotwebfc_de");
            qApp->installTranslator(translator);
        }
        else if(m_Language == "English")
        {
            translator->load("");
            qApp->installTranslator(translator);
        }
    }

    QDialog::reject();
}

void SettingsDialog::slotLanguageComboBoxActivated(const QString& language)
{
    QTranslator* translator = m_pMainWidget->getTranslator();

    if(m_pMainWidget && translator)
    {
        if(language == "Nederlands")
        {
            translator->load("spotwebfc_nl");
            qApp->installTranslator(translator);
        }
        else if(language == "Deutsch")
        {
            translator->load("spotwebfc_de");
            qApp->installTranslator(translator);
        }
        else if(language == "English")
        {
            translator->load("");
            qApp->installTranslator(translator);
        }
    }
}
