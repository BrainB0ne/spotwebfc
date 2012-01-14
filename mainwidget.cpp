#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_loadButton_clicked()
{

}

void MainWidget::on_saveButton_clicked()
{

}

void MainWidget::on_saveAsButton_clicked()
{

}

void MainWidget::on_addToolButton_clicked()
{

}

void MainWidget::on_removeToolButton_clicked()
{

}

void MainWidget::on_clearAllToolButton_clicked()
{

}
