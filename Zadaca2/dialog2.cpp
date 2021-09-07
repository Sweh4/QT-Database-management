#include "dialog2.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::setMjesec(QString mjesec)
{
     ui->label_mjesec->setText(mjesec);
}

void Dialog2::setGodina(QString godina)
{
    ui->label_godina->setText(godina);
}

void Dialog2::setSerijski(QString serijski)
{
     ui->label_serijski->setText(serijski);
}

void Dialog2::setPocetnoStanje(int pocetnoStanje)
{
    int mjesec = ui->label_mjesec->text().toInt();
    QString serijskia = ui->label_serijski->text();
    QSqlQuery glavniQry;
    QSqlQueryModel *sqlmodel = new QSqlQueryModel(this);

    glavniQry.prepare("SELECT * FROM STAMPA WHERE Serijski = '"+serijskia+"'");
    if(glavniQry.exec())
    {
     sqlmodel->setQuery(glavniQry);
     if(sqlmodel->rowCount() < 1)
     {
       ui->spinBox->setMinimum(pocetnoStanje);
     }else{
       QSqlQuery stranice;
       stranice.prepare("SELECT SUM(Stranice) FROM STAMPA WHERE Serijski = '"+serijskia+"'");
       if(stranice.exec())
       {
             while(stranice.next())
             {
              ui->spinBox->setMinimum(stranice.value(0).toInt());
             }
       }
    }
   }


}

void Dialog2::on_pushButton_clicked()
{
    QString serijskia = ui->label_serijski->text();
    QString godine = ui->label_godina->text();
    QString mjeseci = ui->label_mjesec->text();
    QString stranice = QString::number(ui->spinBox->value());

    QSqlQuery qry;
    qry.prepare("INSERT INTO Stampa(Serijski, Godina, Mjesec, Stranice) VALUES('"+serijskia+"', '"+godine+"', '"+mjeseci+"', '"+stranice+"')");
    if(qry.exec()){

         QSqlQuery qry2;
         qry2.prepare("UPDATE PRINTERI SET Ispunjeno = 1 WHERE Serijski = '"+serijskia+"'");
         if(qry2.exec())
         {
             qDebug() << "Upisano";
             this->close();
         }else{
         QMessageBox::critical(this,tr("error::"),qry.lastError().text());
     }

 }
}
