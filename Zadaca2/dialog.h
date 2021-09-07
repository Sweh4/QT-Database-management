#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QDebug>
#include <QObject>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QSqlQueryModel>
#include "dialog2.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QSqlDatabase mydb;
    QString podaci_tab;
    int podaci_tab_spin;
    int godina;
    int mjesec;
    double cijena;

    double ukupnoCijena;
    void updateTable();
    void otvorenprozor();

    QPushButton *zavrsi_unos;
    QGridLayout *grid;
    QString spin;
    QString mj_s, god_s, serijski_s;


private slots:

    void spremanje();
    void on_tableView_doubleClicked(const QModelIndex &index1);
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    void provjera();

};

#endif // DIALOG_H
