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
#include <QtGui/QCloseEvent>
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

void MainWidget::closeEvent(QCloseEvent *event)
{
    if(ui->filtersTreeWidget->topLevelItemCount() == 0 && ui->fileLineEdit->text().isEmpty())
    {
        event->accept();
        return;
    }

    int reply = QMessageBox::warning(this, tr("Spotweb Filter Creator"),
                                   tr("Spotweb Filter Creator is closing, unsaved changes will be lost!\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                   QMessageBox::Save);

    if (reply == QMessageBox::Save)
    {
        // save filters and close the application.
        if(ui->fileLineEdit->text().isEmpty())
        {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spotweb Filter File"),
                                       qApp->applicationDirPath() + "/myfilters.xml",
                                       tr("Spotweb Filter Files (*.xml)"));

            if(!fileName.isEmpty())
            {
                if(saveFilterFile(fileName) == 0)
                {
                    ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
                    QMessageBox::information(this, "Spotweb Filters saved",
                                             QString("Spotweb Filters saved to:\n%1")
                                             .arg(ui->fileLineEdit->text()));
                    event->accept();
                }
                else
                {
                    QMessageBox::critical(this, "Spotweb Filters save failed",
                                          QString("Spotweb Filters could not be saved to:\n%1")
                                          .arg(QDir::convertSeparators(fileName)));
                    event->ignore();
                }
            }
            else
            {
                event->ignore();
            }
        }
        else
        {
            if(saveFilterFile(ui->fileLineEdit->text()) == 0)
            {
                QMessageBox::information(this, "Spotweb Filters saved",
                                         QString("Spotweb Filters saved to:\n%1")
                                         .arg(ui->fileLineEdit->text()));
                event->accept();
            }
            else
            {
                QMessageBox::critical(this, "Spotweb Filters save failed",
                                      QString("Spotweb Filters could not be saved to:\n%1")
                                      .arg(QDir::convertSeparators(ui->fileLineEdit->text())));
                event->ignore();
            }
        }
    }
    else if(reply == QMessageBox::Discard)
    {
        // just close the application.
        event->accept();
    }
    else if(reply == QMessageBox::Cancel)
    {
        // don't close the application.
        event->ignore();
    }
    else
    {
        // don't close the application.
        event->ignore();
    }
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

    ui->filtersTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->filtersTreeWidget->setDragEnabled(true);
    ui->filtersTreeWidget->viewport()->setAcceptDrops(true);
    ui->filtersTreeWidget->setDropIndicatorShown(true);
    ui->filtersTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);

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
        QMessageBox::critical(this, "Error",
                              QString("Contents file: %1 could not be opened").arg(CONTENTS_FILE));
        return;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        QMessageBox::critical(this, "Internal Error",
                              QString("Contents data could not be read correctly"));
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
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        ui->filtersTreeWidget->clearSelection();
        ui->filtersTreeWidget->clear();

        slotClearAllToolButtonClicked();
        m_pPreviousFilterItem = 0;

        QDomDocument doc("spotwebfilters");

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "Error",
                                  QString("Spotweb Filter file: %1 could not be opened")
                                  .arg(QDir::convertSeparators(fileName)));
            return;
        }

        if (!doc.setContent(&file))
        {
            QApplication::restoreOverrideCursor();

            file.close();
            QMessageBox::critical(this, "Internal Error",
                                  QString("Spotweb Filter data could not be read correctly"));
            return;
        }

        file.close();

        QDomElement docElem = doc.documentElement();

        FilterTreeWidgetItem* filterItem = 0;

        QString version;
        QString generator;

        QDomNode n = docElem.firstChild();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            if(!e.isNull())
            {
                if(e.tagName() == "version")
                {
                    version = e.text();
                }
                else if(e.tagName() == "generator")
                {
                    generator = e.text();
                }
                else if(e.tagName() == "filters")
                {
                    QDomNode nFilter = e.firstChild();
                    while (!nFilter.isNull())
                    {
                        QDomElement eFilter = nFilter.toElement();
                        if(!eFilter.isNull())
                        {
                            filterItem = new FilterTreeWidgetItem(ui->filtersTreeWidget);

                            QDomNode n1 = eFilter.firstChild();
                            while(!n1.isNull())
                            {
                                QDomElement e1 = n1.toElement();
                                if(!e1.isNull())
                                {
                                    if(e1.tagName() == "id")
                                    {
                                        filterItem->setID(e1.text());
                                    }
                                    else if(e1.tagName() == "title")
                                    {
                                        filterItem->setName(e1.text());
                                        filterItem->setText(FILTER_COLUMN_NAME, e1.text());
                                    }
                                    else if(e1.tagName() == "icon")
                                    {
                                        filterItem->setIconName(e1.text());
                                        filterItem->setIcon(FILTER_COLUMN_NAME, QIcon(QString(":/images/%1.png").arg(e1.text())));
                                    }
                                    else if(e1.tagName() == "parent")
                                    {
                                        filterItem->setParentID(e1.text());
                                    }
                                    else if(e1.tagName() == "order")
                                    {
                                        filterItem->setOrder(e1.text());
                                    }
                                    else if(e1.tagName() == "tree")
                                    {
                                        QDomNode nItem = e1.firstChild();
                                        while(!nItem.isNull())
                                        {
                                            QDomElement eItem = nItem.toElement();
                                            if(!eItem.isNull())
                                            {
                                                if(eItem.tagName() == "item")
                                                {
                                                    if(eItem.attribute("type") == "include")
                                                    {
                                                        filterItem->appendFilter(eItem.text());
                                                        filterItem->appendContent(findContentByFilter(eItem.text()));
                                                    }
                                                }
                                            }

                                            nItem = nItem.nextSibling();
                                        }
                                    }
                                }

                                n1 = n1.nextSibling();
                            }
                        }

                        nFilter = nFilter.nextSibling();
                    }
                }
            }

            n = n.nextSibling();
        }

        FilterTreeWidgetItem* firstFilterItem = (FilterTreeWidgetItem*)(ui->filtersTreeWidget->topLevelItem(0));
        if(firstFilterItem)
        {
            ui->filtersTreeWidget->setCurrentItem(firstFilterItem);
            firstFilterItem->setSelected(true);
            m_pCurrentFilterItem = firstFilterItem;
        }

        ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
        ui->filtersTreeWidget->setFocus();
        filtersTreeEmptyCheck();
    }

    QApplication::restoreOverrideCursor();
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
            if(saveFilterFile(fileName) == 0)
            {
                ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
                QMessageBox::information(this, "Spotweb Filters saved",
                                         QString("Spotweb Filters saved to:\n%1")
                                         .arg(ui->fileLineEdit->text()));
            }
            else
            {
                QMessageBox::critical(this, "Spotweb Filters save failed",
                                      QString("Spotweb Filters could not be saved to:\n%1")
                                      .arg(QDir::convertSeparators(fileName)));
            }
        }
    }
    else
    {
        if(saveFilterFile(ui->fileLineEdit->text()) == 0)
        {
            QMessageBox::information(this, "Spotweb Filters saved",
                                     QString("Spotweb Filters saved to:\n%1")
                                     .arg(ui->fileLineEdit->text()));
        }
        else
        {
            QMessageBox::critical(this, "Spotweb Filters save failed",
                                  QString("Spotweb Filters could not be saved to:\n%1")
                                  .arg(QDir::convertSeparators(ui->fileLineEdit->text())));
        }
    }
}

void MainWidget::createFilterIdentification()
{
    FilterTreeWidgetItem* item = 0;
    QTreeWidgetItemIterator itItem(ui->filtersTreeWidget, QTreeWidgetItemIterator::All);

    while (*itItem)
    {
        item = (FilterTreeWidgetItem*)(*itItem);

        int id = 0;
        FilterTreeWidgetItem* curItem = item;
        QString strID;

        while (curItem)
        {
            if(curItem->parent())
            {
                id = curItem->parent()->indexOfChild(curItem);
            }
            else
            {
                id = ui->filtersTreeWidget->indexOfTopLevelItem(curItem);
            }

            strID.prepend(QString::number(id+1));
            curItem = (FilterTreeWidgetItem*)curItem->parent();
        }

        // set the ID of the item
        item->setID(strID);

        // set order to the same value as ID
        item->setOrder(item->getID());

        if(item->parent())
        {
            // set parentID of the item to the ID of the parent item
            item->setParentID(((FilterTreeWidgetItem*)(item->parent()))->getID());
        }
        else
        {
            // top level item gets parentID of "0"
            item->setParentID("0");
        }

        ++itItem;
    }
}

int MainWidget::saveFilterFile(const QString& fileName)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    updateFilterItem(m_pCurrentFilterItem);
    createFilterIdentification();

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
    QDomElement filterElement,
                idElement,
                titleElement,
                iconElement,
                parentElement,
                orderElement,
                treeElement,
                itemElement;

    QStringList filtersList;
    QString filter;
    QStringList::iterator it, itEnd;

    QTreeWidgetItemIterator itItem(ui->filtersTreeWidget, QTreeWidgetItemIterator::All);
    while (*itItem)
    {
        item = (FilterTreeWidgetItem*)(*itItem);
        filterElement = doc.createElement("filter");
        filtersElement.appendChild(filterElement);

        idElement = doc.createElement("id");
        filterElement.appendChild(idElement);

        t = doc.createTextNode(item->getID());
        idElement.appendChild(t);

        titleElement = doc.createElement("title");
        filterElement.appendChild(titleElement);

        t = doc.createTextNode(item->getName());
        titleElement.appendChild(t);

        iconElement = doc.createElement("icon");
        filterElement.appendChild(iconElement);

        t = doc.createTextNode(item->getIconName());
        iconElement.appendChild(t);

        parentElement = doc.createElement("parent");
        filterElement.appendChild(parentElement);

        t = doc.createTextNode(item->getParentID());
        parentElement.appendChild(t);

        orderElement = doc.createElement("order");
        filterElement.appendChild(orderElement);

        t = doc.createTextNode(item->getOrder());
        orderElement.appendChild(t);

        treeElement = doc.createElement("tree");
        filterElement.appendChild(treeElement);

        filtersList = item->getFilters();

        it = filtersList.begin();
        itEnd = filtersList.end();

        while(it != itEnd)
        {
            filter = (*it);

            itemElement = doc.createElement("item");
            itemElement.setAttribute("type", "include");
            treeElement.appendChild(itemElement);

            t = doc.createTextNode(filter);
            itemElement.appendChild(t);

            ++it;
        }

        ++itItem;
    }

    QString xml = doc.toString();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QApplication::restoreOverrideCursor();
        return 1;
    }

    QTextStream out(&file);
    out << xml << "\n";

    file.close();

    QApplication::restoreOverrideCursor();
    return 0;
}

void MainWidget::slotSaveAsButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spotweb Filter File As"),
                               qApp->applicationDirPath() + "/myfilters.xml",
                               tr("Spotweb Filter Files (*.xml)"));

    if(!fileName.isEmpty())
    {
        if(saveFilterFile(fileName) == 0)
        {
            ui->fileLineEdit->setText(QDir::convertSeparators(fileName));
            QMessageBox::information(this, "Spotweb Filters saved",
                                     QString("Spotweb Filters saved to:\n%1")
                                     .arg(ui->fileLineEdit->text()));
        }
        else
        {
            QMessageBox::critical(this, "Spotweb Filters save failed",
                                  QString("Spotweb Filters could not be saved to:\n%1")
                                  .arg(QDir::convertSeparators(fileName)));
        }
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
            filterItem->setIconName(newFilterDlg->getIconName());
            filterItem->setIcon(FILTER_COLUMN_NAME, newFilterDlg->getIcon());

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

QString MainWidget::findContentByFilter(const QString& filter)
{
    QTreeWidgetItemIterator it(ui->contentsTreeWidget, QTreeWidgetItemIterator::All);

    while (*it)
    {
        if((*it)->text(CONTENTS_COLUMN_FILTER) == filter)
        {
            QTreeWidgetItem* curItem = (*it);
            QStringList contentList;

            while(curItem)
            {
                contentList.prepend(curItem->text(CONTENTS_COLUMN_TYPE));
                curItem = curItem->parent();
            }

            return contentList.join(" -> ");
        }

        ++it;
    }

    return QString("");
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
