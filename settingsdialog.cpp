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

LanguageItem::LanguageItem(const QString& name, const QString& locale)
{
    m_Name = name;
    m_Locale = locale;
}

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
    QString languageLocale;
    QStringList languages;

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
        {
            languageName = e.attribute("name");
            languageLocale = e.attribute("locale");

            languages << languageName;

            m_LanguageItems.append(LanguageItem(languageName, languageLocale));
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
    if(m_pMainWidget)
    {
        QTranslator* translator = m_pMainWidget->getTranslator();

        if(translator)
        {
            QString locale = findLocaleByLanguageName(m_Language);

            if(!locale.isEmpty())
            {
                translator->load(QString("%1/spotwebfc_%2").arg(qApp->applicationDirPath()).arg(locale));
                qApp->installTranslator(translator);
            }
            else
            {
                translator->load("");
                qApp->installTranslator(translator);
            }
        }
    }

    QDialog::reject();
}

void SettingsDialog::slotLanguageComboBoxActivated(const QString& language)
{
    if(m_pMainWidget)
    {
        QTranslator* translator = m_pMainWidget->getTranslator();

        if(translator)
        {
            QString locale = findLocaleByLanguageName(language);

            if(!locale.isEmpty())
            {
                translator->load(QString("%1/spotwebfc_%2").arg(qApp->applicationDirPath()).arg(locale));
                qApp->installTranslator(translator);
            }
            else
            {
                translator->load("");
                qApp->installTranslator(translator);
            }
        }
    }
}

QString SettingsDialog::findLocaleByLanguageName(const QString& languageName)
{
    QList<LanguageItem>::iterator it = m_LanguageItems.begin(),
                                  itEnd = m_LanguageItems.end();

    while (it != itEnd)
    {
        if( (*it).getName() == languageName )
        {
            return (*it).getLocale();
        }

        ++it;
    }

    return "";
}

QString SettingsDialog::findLanguageNameByLocale(const QString& locale)
{
    QList<LanguageItem>::iterator it = m_LanguageItems.begin(),
                                  itEnd = m_LanguageItems.end();

    while (it != itEnd)
    {
        if( (*it).getLocale() == locale )
        {
            return (*it).getName();
        }

        ++it;
    }

    return "";
}
