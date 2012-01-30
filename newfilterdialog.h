#ifndef NEWFILTERDIALOG_H
#define NEWFILTERDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class NewFilterDialog;
}

class NewFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFilterDialog(QWidget *parent = 0);
    ~NewFilterDialog();

    QString getName() {return m_Name;}
    QString getIconName() {return m_IconName;}
    QIcon getIcon() {return m_Icon;}

private slots:
    void accept();

private:
    Ui::NewFilterDialog *ui;

    QString m_Name;
    QString m_IconName;
    QIcon m_Icon;
};

#endif // NEWFILTERDIALOG_H
