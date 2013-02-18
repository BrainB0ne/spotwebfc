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
#include <QtXml/QDomDocument>
#include <QFile>

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

void SettingsDialog::fillLanguages()
{
    QString filePath = qApp->applicationDirPath() + "/" + QString(LANGUAGES_FILE);

    QDomDocument doc("spotwebfc_languages");
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }

    file.close();

    QDomElement docElem = doc.documentElement();

    QString languageName;
    QStringList languages;

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
        {
            languageName = e.attribute("name");
            languages << languageName;
        }

        n = n.nextSibling();
    }

    ui->languageComboBox->insertItems(0, languages);
    ui->languageComboBox->setCurrentIndex(ui->languageComboBox->findText(m_Language));
}

void SettingsDialog::initialize()
{
    fillLanguages();
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
        else if(m_Language == "Français")
        {
            translator->load("spotwebfc_fr");
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
        else if(language == "Français")
        {
            translator->load("spotwebfc_fr");
            qApp->installTranslator(translator);
        }
        else if(language == "English")
        {
            translator->load("");
            qApp->installTranslator(translator);
        }
    }
}
