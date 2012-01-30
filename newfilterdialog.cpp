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

#include "newfilterdialog.h"
#include "ui_newfilterdialog.h"

#include <QMessageBox>
#include <QCloseEvent>

NewFilterDialog::NewFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFilterDialog)
{
    ui->setupUi(this);

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

    ui->iconComboBox->setCurrentIndex(ui->iconComboBox->findText("custom"));
}

NewFilterDialog::~NewFilterDialog()
{
    delete ui;
}

void NewFilterDialog::accept()
{
    if(!ui->nameLineEdit->text().isEmpty())
    {
        m_Name = ui->nameLineEdit->text();
        m_IconName = ui->iconComboBox->currentText();
        m_Icon = ui->iconComboBox->itemIcon(ui->iconComboBox->currentIndex());
        QDialog::accept();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Filter Name can't be empty!"));
        ui->nameLineEdit->setFocus();
    }
}
