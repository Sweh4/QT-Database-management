#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QObject>
#include "dialog.h"

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = 0);
    ~Dialog1();
    QString serijski_l;
    QSqlQuery qry;

public slots:
void setLabelTextS(QString podaci, int spin_pod);
void setLabelTextGM(int god, int mj);


private slots:
void on_pushButton_clicked();

private:
    Ui::Dialog1 *ui;

};

#endif // DIALOG1_H
