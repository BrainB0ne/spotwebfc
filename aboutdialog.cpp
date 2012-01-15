#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->versionLabel->setText("Version: 0.0.0.1-alpha");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
