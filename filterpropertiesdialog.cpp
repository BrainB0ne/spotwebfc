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

#include "filterpropertiesdialog.h"
#include "ui_filterpropertiesdialog.h"

#include "filtertreewidgetitem.h"

#include <QMessageBox>

FilterPropertiesDialog::FilterPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterPropertiesDialog)
{
    ui->setupUi(this);

    m_pFilterItem = 0;

    ui->iconComboBox->addItem(QIcon(":/images/eye.png"), "eye");
    ui->iconComboBox->addItem(QIcon(":/images/film.png"), "film");
    ui->iconComboBox->addItem(QIcon(":/images/picture.png"), "picture");
    ui->iconComboBox->addItem(QIcon(":/images/controller.png"), "controller");
    ui->iconComboBox->addItem(QIcon(":/images/application.png"), "application");
    ui->iconComboBox->addItem(QIcon(":/images/music.png"), "music");
    ui->iconComboBox->addItem(QIcon(":/images/fav.png"), "fav");
    ui->iconComboBox->addItem(QIcon(":/images/today.png"), "today");
    ui->iconComboBox->addItem(QIcon(":/images/dvd.png"), "dvd");
    ui->iconComboBox->addItem(QIcon(":/images/pda.png"), "pda");
    ui->iconComboBox->addItem(QIcon(":/images/bluray.png"), "bluray");
    ui->iconComboBox->addItem(QIcon(":/images/book.png"), "book");
    ui->iconComboBox->addItem(QIcon(":/images/custom.png"), "custom");
    ui->iconComboBox->addItem(QIcon(":/images/divx.png"), "divx");
    ui->iconComboBox->addItem(QIcon(":/images/female.png"), "female");
    ui->iconComboBox->addItem(QIcon(":/images/hd.png"), "hd");
    ui->iconComboBox->addItem(QIcon(":/images/home.png"), "home");
    ui->iconComboBox->addItem(QIcon(":/images/ipod.png"), "ipod");
    ui->iconComboBox->addItem(QIcon(":/images/linux.png"), "linux");
    ui->iconComboBox->addItem(QIcon(":/images/mac.png"), "mac");
    ui->iconComboBox->addItem(QIcon(":/images/mpg.png"), "mpg");
    ui->iconComboBox->addItem(QIcon(":/images/nintendo_ds.png"), "nintendo_ds");
    ui->iconComboBox->addItem(QIcon(":/images/nintendo_wii.png"), "nintendo_wii");
    ui->iconComboBox->addItem(QIcon(":/images/phone.png"), "phone");
    ui->iconComboBox->addItem(QIcon(":/images/playstation.png"), "playstation");
    ui->iconComboBox->addItem(QIcon(":/images/tag.png"), "tag");
    ui->iconComboBox->addItem(QIcon(":/images/tv.png"), "tv");
    ui->iconComboBox->addItem(QIcon(":/images/vista.png"), "vista");
    ui->iconComboBox->addItem(QIcon(":/images/windows.png"), "windows");
    ui->iconComboBox->addItem(QIcon(":/images/wmv.png"), "wmv");
    ui->iconComboBox->addItem(QIcon(":/images/xbox.png"), "xbox");
}

FilterPropertiesDialog::~FilterPropertiesDialog()
{
    delete ui;
}

void FilterPropertiesDialog::initialize()
{
    if(m_pFilterItem)
    {
        ui->nameLineEdit->setText(m_pFilterItem->getName());
        ui->iconComboBox->setCurrentIndex(ui->iconComboBox->findText(m_pFilterItem->getIconName()));
        ui->enableNotifyCheckBox->setChecked(m_pFilterItem->getEnableNotify() == "1" ? true : false);
        ui->tagLineEdit->setText(m_pFilterItem->getTag());
        ui->posterLineEdit->setText(m_pFilterItem->getPoster());
        ui->contentsListWidget->insertItems(0, m_pFilterItem->getContents());
    }

    ui->nameLineEdit->setFocus();
}

void FilterPropertiesDialog::accept()
{
    if(!ui->nameLineEdit->text().isEmpty())
    {
        if(m_pFilterItem)
        {
            m_pFilterItem->setName(ui->nameLineEdit->text());
            m_pFilterItem->setText(FILTER_COLUMN_NAME, ui->nameLineEdit->text());
            m_pFilterItem->setIconName(ui->iconComboBox->currentText());
            m_pFilterItem->setIcon(FILTER_COLUMN_NAME, ui->iconComboBox->itemIcon(ui->iconComboBox->currentIndex()));
            m_pFilterItem->setEnableNotify(ui->enableNotifyCheckBox->isChecked() ? "1" : "0");
            m_pFilterItem->setTag(ui->tagLineEdit->text());
            m_pFilterItem->setPoster(ui->posterLineEdit->text());
        }

        QDialog::accept();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Filter Name can't be empty!"));
        ui->nameLineEdit->setFocus();
    }
}
