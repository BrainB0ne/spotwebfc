#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "aboutdialog.h"
#include "newfilterdialog.h"
#include "filterpropertiesdialog.h"
#include "filtertreewidgetitem.h"

#include <QtCore/QTextStream>
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
    connect(ui->filtersTreeWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotFiltersTreeWidgetItemSelectionChanged()));
}

void MainWidget::initialize()
{
    m_pCurrentFilterItem = 0;
    m_pPreviousFilterItem = 0;

    QList<int> sizeList;
    sizeList << 1 << 1;
    ui->splitter->setSizes(sizeList);

    ui->filtersTreeWidget->setRootIsDecorated(false);

    filtersTreeEmptyCheck();
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
            catItem->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Unchecked);
            catItem->setText(CONTENTS_COLUMN_TYPE, eCat.attribute("name"));
            catItem->setText(CONTENTS_COLUMN_FILTER, eCat.attribute("filter"));
            catItem->setIcon(CONTENTS_COLUMN_TYPE, QIcon(QString(":/images/%1.png").arg(eCat.attribute("pixmap"))));
            catItem->setExpanded(true);

            QDomNode nType = eCat.firstChild();
            while (!nType.isNull())
            {
                QDomElement eType = nType.toElement();
                if(!eType.isNull())
                {
                    typeItem = new QTreeWidgetItem(catItem);
                    typeItem->setFlags(typeItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
                    typeItem->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Unchecked);
                    typeItem->setText(CONTENTS_COLUMN_TYPE, eType.attribute("name"));
                    typeItem->setText(CONTENTS_COLUMN_FILTER, eType.attribute("filter"));

                    QDomNode nSubCat = eType.firstChild();
                    while (!nSubCat.isNull())
                    {
                        QDomElement eSubCat = nSubCat.toElement();
                        if(!eSubCat.isNull())
                        {
                            subcatItem = new QTreeWidgetItem(typeItem);
                            subcatItem->setFlags(subcatItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
                            subcatItem->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Unchecked);
                            subcatItem->setText(CONTENTS_COLUMN_TYPE, eSubCat.attribute("name"));

                            QDomNode nSubFilter = eSubCat.firstChild();
                            while (!nSubFilter.isNull())
                            {
                                QDomElement eSubFilter = nSubFilter.toElement();
                                if(!eSubFilter.isNull())
                                {
                                    subfilterItem = new QTreeWidgetItem(subcatItem);
                                    subfilterItem->setFlags(subfilterItem->flags() | Qt::ItemIsUserCheckable);
                                    subfilterItem->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Unchecked);
                                    subfilterItem->setText(CONTENTS_COLUMN_TYPE, eSubFilter.attribute("name"));
                                    subfilterItem->setText(CONTENTS_COLUMN_FILTER, eSubFilter.attribute("filter"));
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

    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_TYPE);
    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_FILTER);
}

void MainWidget::slotOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spotweb Filter File"),
                                                    qApp->applicationDirPath(),
                                                    tr("Spotweb Filter Files (*.xml)"));
    if(!fileName.isEmpty())
    {
        ui->fileLineEdit->setText(QDir::convertSeparators(fileName));

        ui->filtersTreeWidget->clear();
        slotClearAllToolButtonClicked();
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

            saveFilterFile(ui->fileLineEdit->text());
            QMessageBox::information(this, "Spotweb Filters saved", QString("Spotweb Filters saved to:\n%1").arg(QDir::convertSeparators(fileName)));
        }
    }
    else
    {
        saveFilterFile(ui->fileLineEdit->text());
        QMessageBox::information(this, "Spotweb Filters saved", QString("Spotweb Filters saved to:\n%1").arg(ui->fileLineEdit->text()));
    }
}

void MainWidget::saveFilterFile(const QString& fileName)
{
    QDomDocument doc("spotwebfilters");
    QDomElement root = doc.createElement("spotwebfilter");
    doc.appendChild(root);

    QDomElement versionElement = doc.createElement("version");
    root.appendChild(versionElement);

    QDomText t = doc.createTextNode("1.0");
    versionElement.appendChild(t);

    QDomElement generatorElement = doc.createElement("generator");
    root.appendChild(generatorElement);

    t = doc.createTextNode("spotwebfc");
    generatorElement.appendChild(t);

    QDomElement filtersElement = doc.createElement("filters");
    root.appendChild(filtersElement);

    FilterTreeWidgetItem* item = 0;
    QDomElement filterElement, titleElement;

    QTreeWidgetItemIterator it(ui->filtersTreeWidget, QTreeWidgetItemIterator::All);
    while (*it)
    {
        item = (FilterTreeWidgetItem*)(*it);
        filterElement = doc.createElement("filter");
        filtersElement.appendChild(filterElement);

        titleElement = doc.createElement("title");
        filterElement.appendChild(titleElement);

        t = doc.createTextNode(item->getName());
        titleElement.appendChild(t);

        // TODO: Save other elements

        ++it;
    }

    QString xml = doc.toString();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << xml << "\n";

    file.close();
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
            filterItem->setText(FILTER_COLUMN_NAME, newFilterDlg->getName());
            filterItem->setIconName("custom");
            filterItem->setIcon(FILTER_COLUMN_NAME, QIcon(":/images/custom.png"));

            ui->filtersTreeWidget->clearSelection();
            ui->filtersTreeWidget->setCurrentItem(filterItem);
            filterItem->setSelected(true);
            ui->filtersTreeWidget->setFocus();

            filtersTreeEmptyCheck();
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

    filtersTreeEmptyCheck();
}

void MainWidget::slotClearAllToolButtonClicked()
{
    QTreeWidgetItemIterator it(ui->contentsTreeWidget, QTreeWidgetItemIterator::All);
    while (*it)
    {
        (*it)->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Unchecked);
        ++it;
    }

    ui->contentsTreeWidget->viewport()->update();
}

void MainWidget::slotContentsTreeWidgetItemChanged(QTreeWidgetItem *item)
{
    if(item)
    {
        ui->contentsTreeWidget->viewport()->update();
    }
}

void MainWidget::slotContentsTreeWidgetItemCollapsed()
{
    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_TYPE);
    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_FILTER);
}

void MainWidget::slotContentsTreeWidgetItemExpanded()
{
    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_TYPE);
    ui->contentsTreeWidget->resizeColumnToContents(CONTENTS_COLUMN_FILTER);
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
        updateFilterItem(m_pCurrentFilterItem);
        filterPropDlg->setFilterItem(m_pCurrentFilterItem);
        filterPropDlg->initialize();

        if(filterPropDlg->exec() == QDialog::Accepted)
        {
            // TODO: save properties to item
        }
    }
}

void MainWidget::slotFiltersTreeWidgetItemSelectionChanged()
{
    QList<QTreeWidgetItem*> selItems = ui->filtersTreeWidget->selectedItems();
    if(selItems.count() == 1)
    {
        updateFilterItem(m_pPreviousFilterItem);
        m_pCurrentFilterItem = (FilterTreeWidgetItem*)(selItems.at(0));
        updateContentsTree(m_pCurrentFilterItem);
        m_pPreviousFilterItem = m_pCurrentFilterItem;
    }
}

void MainWidget::updateFilterItem(FilterTreeWidgetItem* selectedFilterItem)
{
    if(selectedFilterItem)
    {
        QTreeWidgetItemIterator it(ui->contentsTreeWidget, QTreeWidgetItemIterator::Checked);

        selectedFilterItem->clearFilters();
        selectedFilterItem->clearContents();

        while (*it)
        {
            QString filter = (*it)->text(CONTENTS_COLUMN_FILTER);

            if(!filter.isEmpty())
            {
                if((*it)->checkState(CONTENTS_COLUMN_TYPE) == Qt::Checked)
                {
                    QTreeWidgetItem* curItem = (*it);
                    QStringList contentList;
                    selectedFilterItem->appendFilter(filter);

                    while(curItem)
                    {
                        contentList.prepend(curItem->text(CONTENTS_COLUMN_TYPE));
                        curItem = curItem->parent();
                    }

                    selectedFilterItem->appendContent(contentList.join(" -> "));
                }
            }

            ++it;
        }
    }
}

void MainWidget::updateContentsTree(FilterTreeWidgetItem *selectedFilterItem)
{
    slotClearAllToolButtonClicked();

    if(selectedFilterItem)
    {
        QStringList filtersList = selectedFilterItem->getFilters();
        QStringList::iterator it = filtersList.begin(),
                              itEnd = filtersList.end();

        while(it != itEnd)
        {
            QString filter = (*it);
            QTreeWidgetItemIterator itItem(ui->contentsTreeWidget, QTreeWidgetItemIterator::All);

            while(*itItem)
            {
                if((*itItem)->text(CONTENTS_COLUMN_FILTER) == filter)
                {
                    (*itItem)->setCheckState(CONTENTS_COLUMN_TYPE, Qt::Checked);
                }

                ++itItem;
            }

            ++it;
        }
    }
}

void MainWidget::filtersTreeEmptyCheck()
{
    if(ui->filtersTreeWidget->topLevelItemCount() == 0)
    {
        slotClearAllToolButtonClicked();
        m_pPreviousFilterItem = 0;

        ui->contentsTreeWidget->clearSelection();
        ui->contentsTreeWidget->clearFocus();
        ui->contentsTreeWidget->setEnabled(false);
        ui->removeToolButton->setEnabled(false);
        ui->clearAllToolButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->saveAsButton->setEnabled(false);
    }
    else if(ui->filtersTreeWidget->topLevelItemCount() > 0)
    {
        ui->contentsTreeWidget->setEnabled(true);
        ui->removeToolButton->setEnabled(true);
        ui->clearAllToolButton->setEnabled(true);
        ui->saveButton->setEnabled(true);
        ui->saveAsButton->setEnabled(true);
    }
}
