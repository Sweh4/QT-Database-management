#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>
#include "dialog.h"
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QLineEdit>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();
    QString mjesec;
    QString godina;
    QString serijski;

    void setMjesec(QString );
    void setGodina(QString );
    void setSerijski(QString );
    void setPocetnoStanje(int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog2 *ui;
};


#endif // DIALOG2_H
