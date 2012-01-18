#ifndef FILTERPROPERTIESDIALOG_H
#define FILTERPROPERTIESDIALOG_H

#include <QDialog>

class FilterTreeWidgetItem;

namespace Ui {
class FilterPropertiesDialog;
}

class FilterPropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FilterPropertiesDialog(QWidget *parent = 0);
    ~FilterPropertiesDialog();

    void initialize();

    void setFilterItem(FilterTreeWidgetItem* item) {m_pFilterItem = item;}
    
private slots:
    void accept();

private:
    Ui::FilterPropertiesDialog *ui;

    FilterTreeWidgetItem* m_pFilterItem;
};

#endif // FILTERPROPERTIESDIALOG_H
