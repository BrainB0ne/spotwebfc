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

private slots:
    void accept();

private:
    Ui::NewFilterDialog *ui;

    QString m_Name;
};

#endif // NEWFILTERDIALOG_H
