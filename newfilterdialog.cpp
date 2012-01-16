#include "newfilterdialog.h"
#include "ui_newfilterdialog.h"

#include <QMessageBox>
#include <QCloseEvent>

NewFilterDialog::NewFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFilterDialog)
{
    ui->setupUi(this);
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
        QDialog::accept();
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Filter Name can't be empty!"));
        ui->nameLineEdit->setFocus();
    }
}
