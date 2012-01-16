#ifndef FILTERTREEWIDGETITEM_H
#define FILTERTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class FilterTreeWidgetItem : public QTreeWidgetItem
{
public:
    FilterTreeWidgetItem(QTreeWidget *parent, int type = Type);
    FilterTreeWidgetItem(QTreeWidgetItem * parent, int type = Type);
    ~FilterTreeWidgetItem();

    void setName(const QString& name) {m_Name = name;}
    QString getName() {return m_Name;}

    void setFilters(const QStringList& filters) {m_Filters = filters;}
    QStringList getFilters() {return m_Filters;}

    void appendFilter(const QString& filter) {m_Filters.append(filter);}
    void removeFilter(const QString& filter) {m_Filters.removeAll(filter);}
    void clearFilters() {m_Filters.clear();}

private:
    QString m_Name;
    QStringList m_Filters;

};

#endif // FILTERTREEWIDGETITEM_H
