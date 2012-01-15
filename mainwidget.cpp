#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "aboutdialog.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtXml/QDomDocument>

#define SPOTWEB_AVAILABLE_FILTER_FILE "spotwebfc.xml"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    connectSignalsSlots();
    initialize();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::connectSignalsSlots()
{
    connect(ui->openButton,         SIGNAL(clicked()), this, SLOT(slotOpenButtonClicked()));
    connect(ui->saveButton,         SIGNAL(clicked()), this, SLOT(slotSaveButtonClicked()));
    connect(ui->saveAsButton,       SIGNAL(clicked()), this, SLOT(slotSaveAsButtonClicked()));
    connect(ui->aboutButton,        SIGNAL(clicked()), this, SLOT(slotAboutButtonClicked()));
    connect(ui->addToolButton,      SIGNAL(clicked()), this, SLOT(slotAddToolButtonClicked()));
    connect(ui->removeToolButton,   SIGNAL(clicked()), this, SLOT(slotRemoveToolButtonClicked()));
    connect(ui->clearAllToolButton, SIGNAL(clicked()), this, SLOT(slotClearAllToolButtonClicked()));
}

void MainWidget::initialize()
{
    QList<int> sizeList;
    sizeList << 1 << 1;
    ui->splitter->setSizes(sizeList);

    ui->filtersTreeWidget->setRootIsDecorated(false);

    loadAvailableFilters();
}

void MainWidget::loadAvailableFilters()
{
    QDomDocument doc("spotwebfc");

    QString filePath = qApp->applicationDirPath() + "/" + QString(SPOTWEB_AVAILABLE_FILTER_FILE);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", QString("Contents file: %1 could not be opened").arg(SPOTWEB_AVAILABLE_FILTER_FILE));
        return;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        QMessageBox::critical(this, "Internal Error", QString("Contents data could not be read correctly").arg(SPOTWEB_AVAILABLE_FILTER_FILE));
        return;
    }

    file.close();

    QDomElement docElem = doc.documentElement();

    QTreeWidgetItem *item = 0;
    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
        {
            item = new QTreeWidgetItem(ui->contentsTreeWidget);
            item->setCheckState(0, Qt::Unchecked);
            item->setText(0, e.attribute("name"));
        }

        n = n.nextSibling();
    }
}

void MainWidget::slotOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spotweb Filter File"),
                                                    qApp->applicationDirPath(),
                                                    tr("Spotweb Filter Files (*.xml)"));
    if(!fileName.isEmpty())
    {
        ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
    }
}

void MainWidget::slotSaveButtonClicked()
{

}

void MainWidget::slotSaveAsButtonClicked()
{

}

void MainWidget::slotAboutButtonClicked()
{
    AboutDialog* pAboutDlg = new AboutDialog(this);
    if(pAboutDlg)
    {
        pAboutDlg->exec();

        delete pAboutDlg;
        pAboutDlg = 0;
    }
}

void MainWidget::slotAddToolButtonClicked()
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->filtersTreeWidget);
    item->setText(0, "Name");
}

void MainWidget::slotRemoveToolButtonClicked()
{

}

void MainWidget::slotClearAllToolButtonClicked()
{

}
