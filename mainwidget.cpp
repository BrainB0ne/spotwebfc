#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "aboutdialog.h"
#include "newfilterdialog.h"
#include "filterpropertiesdialog.h"
#include "filtertreewidgetitem.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtXml/QDomDocument>

#define CONTENTS_FILE "spotwebfc.xml"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    initialize();
    connectSignalsSlots();
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

    connect(ui->contentsTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(slotContentsTreeWidgetItemChanged(QTreeWidgetItem*)));
    connect(ui->contentsTreeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SLOT(slotContentsTreeWidgetItemCollapsed()));
    connect(ui->contentsTreeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SLOT(slotContentsTreeWidgetItemExpanded()));

    connect(ui->filtersTreeWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(slotFiltersTreeWidgetContextMenu(const QPoint&)));
    connect(ui->filtersTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(slotFiltersTreeWidgetItemDoubleClicked(QTreeWidgetItem*)));
}

void MainWidget::initialize()
{
    m_pCurrentFilterItem = 0;

    QList<int> sizeList;
    sizeList << 1 << 1;
    ui->splitter->setSizes(sizeList);

    ui->filtersTreeWidget->setRootIsDecorated(false);

    loadContents();
}

void MainWidget::loadContents()
{
    QDomDocument doc("spotwebfc");

    QString filePath = qApp->applicationDirPath() + "/" + QString(CONTENTS_FILE);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", QString("Contents file: %1 could not be opened").arg(CONTENTS_FILE));
        return;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        QMessageBox::critical(this, "Internal Error", QString("Contents data could not be read correctly").arg(CONTENTS_FILE));
        return;
    }

    file.close();

    QDomElement docElem = doc.documentElement();

    QTreeWidgetItem *catItem = 0;
    QTreeWidgetItem *typeItem = 0;
    QTreeWidgetItem *subcatItem = 0;
    QTreeWidgetItem *subfilterItem = 0;

    QDomNode nCat = docElem.firstChild();
    while(!nCat.isNull())
    {
        QDomElement eCat = nCat.toElement();
        if(!eCat.isNull())
        {
            catItem = new QTreeWidgetItem(ui->contentsTreeWidget);
            catItem->setFlags(catItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
            catItem->setCheckState(0, Qt::Unchecked);
            catItem->setText(0, eCat.attribute("name"));
            catItem->setText(1, eCat.attribute("filter"));
            catItem->setIcon(0, QIcon(QString(":/images/%1.png").arg(eCat.attribute("pixmap"))));
            catItem->setExpanded(true);

            QDomNode nType = eCat.firstChild();
            while (!nType.isNull())
            {
                QDomElement eType = nType.toElement();
                if(!eType.isNull())
                {
                    typeItem = new QTreeWidgetItem(catItem);
                    typeItem->setFlags(typeItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
                    typeItem->setCheckState(0, Qt::Unchecked);
                    typeItem->setText(0, eType.attribute("name"));
                    typeItem->setText(1, eType.attribute("filter"));

                    QDomNode nSubCat = eType.firstChild();
                    while (!nSubCat.isNull())
                    {
                        QDomElement eSubCat = nSubCat.toElement();
                        if(!eSubCat.isNull())
                        {
                            subcatItem = new QTreeWidgetItem(typeItem);
                            subcatItem->setFlags(subcatItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
                            subcatItem->setCheckState(0, Qt::Unchecked);
                            subcatItem->setText(0, eSubCat.attribute("name"));

                            QDomNode nSubFilter = eSubCat.firstChild();
                            while (!nSubFilter.isNull())
                            {
                                QDomElement eSubFilter = nSubFilter.toElement();
                                if(!eSubFilter.isNull())
                                {
                                    subfilterItem = new QTreeWidgetItem(subcatItem);
                                    subfilterItem->setFlags(subfilterItem->flags() | Qt::ItemIsUserCheckable);
                                    subfilterItem->setCheckState(0, Qt::Unchecked);
                                    subfilterItem->setText(0, eSubFilter.attribute("name"));
                                    subfilterItem->setText(1, eSubFilter.attribute("filter"));
                                }

                                nSubFilter = nSubFilter.nextSibling();
                            }
                        }

                        nSubCat = nSubCat.nextSibling();
                    }
                }

                nType = nType.nextSibling();
            }
        }

        nCat = nCat.nextSibling();
    }

    ui->contentsTreeWidget->resizeColumnToContents(0);
    ui->contentsTreeWidget->resizeColumnToContents(1);
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
    if(ui->fileLineEdit->text().isEmpty())
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spotweb Filter File"),
                                   qApp->applicationDirPath() + "/myfilters.xml",
                                   tr("Spotweb Filter Files (*.xml)"));

        if(!fileName.isEmpty())
        {
            ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
            //TODO: save filters to file from File Save Dialog

            QMessageBox::information(this, "Spotweb Filters saved", QString("Spotweb Filters saved to:\n%1").arg(QDir::convertSeparators(fileName)));
        }
    }
    else
    {
        //TODO: save filters to file specified in fileLineEdit

        QMessageBox::information(this, "Spotweb Filters saved", QString("Spotweb Filters saved to:\n%1").arg(ui->fileLineEdit->text()));
    }
}

void MainWidget::slotSaveAsButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spotweb Filter File As"),
                               qApp->applicationDirPath() + "/myfilters.xml",
                               tr("Spotweb Filter Files (*.xml)"));

    if(!fileName.isEmpty())
    {
        ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
        //TODO: save filters to file from File Save Dialog

        QMessageBox::information(this, "Spotweb Filters saved", QString("Spotweb Filters saved to:\n%1").arg(QDir::convertSeparators(fileName)));
    }
}

void MainWidget::slotAboutButtonClicked()
{
    AboutDialog* aboutDlg = new AboutDialog(this);
    if(aboutDlg)
    {
        aboutDlg->exec();

        delete aboutDlg;
        aboutDlg = 0;
    }
}

void MainWidget::slotAddToolButtonClicked()
{
    NewFilterDialog* newFilterDlg = new NewFilterDialog(this);

    if(newFilterDlg)
    {
        if(newFilterDlg->exec() == QDialog::Accepted)
        {
            FilterTreeWidgetItem* filterItem = new FilterTreeWidgetItem(ui->filtersTreeWidget);
            filterItem->setName(newFilterDlg->getName());
            filterItem->setText(0, newFilterDlg->getName());
            filterItem->setIconName("custom");
            filterItem->setIcon(0, QIcon(":/images/custom.png"));

            ui->filtersTreeWidget->clearSelection();
            ui->filtersTreeWidget->setCurrentItem(filterItem);
            filterItem->setSelected(true);
            ui->filtersTreeWidget->setFocus();
        }

        delete newFilterDlg;
        newFilterDlg = 0;
    }
}

void MainWidget::slotRemoveToolButtonClicked()
{
    QList<QTreeWidgetItem*> selItems = ui->filtersTreeWidget->selectedItems();
    QList<QTreeWidgetItem*>::iterator it = selItems.begin(),
                                      itEnd = selItems.end();

    while(it != itEnd)
    {
        delete (*it);
        ++it;
    }
}

void MainWidget::slotClearAllToolButtonClicked()
{
    QTreeWidgetItemIterator it(ui->contentsTreeWidget, QTreeWidgetItemIterator::All);
    while (*it)
    {
        (*it)->setCheckState(0, Qt::Unchecked);
        ++it;
    }

    ui->contentsTreeWidget->viewport()->update();
}

void MainWidget::slotContentsTreeWidgetItemChanged(QTreeWidgetItem* item)
{
    if(item)
    {
        QList<QTreeWidgetItem*> selItems = ui->filtersTreeWidget->selectedItems();

        if(selItems.count() == 1)
        {
            FilterTreeWidgetItem* selectedFilterItem = (FilterTreeWidgetItem*)(selItems.at(0));
            QString filter = item->text(1);

            if(!filter.isEmpty())
            {
                if(item->checkState(0) == Qt::Checked)
                {
                    QTreeWidgetItem* curItem = item;
                    QStringList contentList;
                    selectedFilterItem->appendFilter(filter);

                    while(curItem)
                    {
                        contentList.prepend(curItem->text(0));
                        curItem = curItem->parent();
                    }

                    selectedFilterItem->appendContent(contentList.join(" -> "));
                }
                else if(item->checkState(0) == Qt::Unchecked)
                {
                    QTreeWidgetItem* curItem = item;
                    QStringList contentList;
                    selectedFilterItem->removeFilter(filter);

                    while(curItem)
                    {
                        contentList.prepend(curItem->text(0));
                        curItem = curItem->parent();
                    }

                    selectedFilterItem->removeContent(contentList.join(" -> "));
                }
            }
        }

        ui->contentsTreeWidget->viewport()->update();
    }
}

void MainWidget::slotContentsTreeWidgetItemCollapsed()
{
    ui->contentsTreeWidget->resizeColumnToContents(0);
    ui->contentsTreeWidget->resizeColumnToContents(1);
}

void MainWidget::slotContentsTreeWidgetItemExpanded()
{
    ui->contentsTreeWidget->resizeColumnToContents(0);
    ui->contentsTreeWidget->resizeColumnToContents(1);
}

void MainWidget::slotFiltersTreeWidgetContextMenu(const QPoint &pos)
{
    m_pCurrentFilterItem = (FilterTreeWidgetItem*)(ui->filtersTreeWidget->itemAt(pos));
    QMenu menu(ui->filtersTreeWidget);

    if (m_pCurrentFilterItem == 0)
    {
        menu.addAction(tr("&Add"), this, SLOT(slotAddToolButtonClicked()));
    }
    else
    {
        menu.addAction(tr("&Remove"), this, SLOT(slotRemoveToolButtonClicked()));
        menu.addAction(tr("&Properties"), this, SLOT(slotShowFilterProperties()));
    }

    menu.exec(QCursor::pos());
}

void MainWidget::slotFiltersTreeWidgetItemDoubleClicked(QTreeWidgetItem* item)
{
    if(item)
    {
        m_pCurrentFilterItem = (FilterTreeWidgetItem*)item;
        slotShowFilterProperties();
    }
}

void MainWidget::slotShowFilterProperties()
{
    FilterPropertiesDialog* filterPropDlg = new FilterPropertiesDialog(this);

    if(filterPropDlg)
    {
        filterPropDlg->setFilterItem(m_pCurrentFilterItem);
        filterPropDlg->initialize();

        if(filterPropDlg->exec() == QDialog::Accepted)
        {
            // TODO: save properties to item
        }
    }

    m_pCurrentFilterItem = 0;
}
