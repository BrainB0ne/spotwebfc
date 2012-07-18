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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILTERTREEWIDGETITEM_H
#define FILTERTREEWIDGETITEM_H

#include <QTreeWidgetItem>

#define FILTER_COLUMN_NAME  0

class FilterTreeWidgetItem : public QTreeWidgetItem
{
public:
    FilterTreeWidgetItem(QTreeWidget *parent, int type = Type);
    FilterTreeWidgetItem(QTreeWidgetItem * parent, int type = Type);
    ~FilterTreeWidgetItem();

    void setName(const QString& name) {m_Name = name;}
    QString getName() {return m_Name;}

    void setID(const QString& id) {m_ID = id;}
    QString getID() {return m_ID;}

    void setParentID(const QString& parentID) {m_ParentID = parentID;}
    QString getParentID() {return m_ParentID;}

    void setOrder(const QString& order) {m_Order = order;}
    QString getOrder() {return m_Order;}

    void setIconName(const QString& iconName) {m_IconName = iconName;}
    QString getIconName() {return m_IconName;}

    void setEnableNotify(const QString& enableNotify) {m_EnableNotify = enableNotify;}
    QString getEnableNotify() {return m_EnableNotify;}

    void setTag(const QString& tag) {m_Tag = tag;}
    QString getTag() {return m_Tag;}

    void setPoster(const QString& poster) {m_Poster = poster;}
    QString getPoster() {return m_Poster;}

    void setFilters(const QStringList& filters) {m_Filters = filters;}
    QStringList getFilters() {return m_Filters;}
    void appendFilter(const QString& filter) {m_Filters.append(filter);}
    void removeFilter(const QString& filter) {m_Filters.removeAll(filter);}
    void clearFilters() {m_Filters.clear();}

    void setContents(const QStringList& contents) {m_Contents = contents;}
    QStringList getContents() {return m_Contents;}
    void appendContent(const QString& content) {m_Contents.append(content);}
    void removeContent(const QString& content) {m_Contents.removeAll(content);}
    void clearContents() {m_Contents.clear();}

private:
    QString m_Name;
    QString m_ID;
    QString m_Order;
    QString m_ParentID;
    QString m_IconName;
    QString m_EnableNotify;
    QString m_Tag;
    QString m_Poster;
    QStringList m_Filters;
    QStringList m_Contents;

};

#endif // FILTERTREEWIDGETITEM_H
