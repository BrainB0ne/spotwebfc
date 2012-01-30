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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->buildDateLabel->setText(QString("Build Date: %1 - %2").arg(__DATE__).arg(__TIME__));
    ui->versionLabel->setText("Version: 0.0.0.1-alpha");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
