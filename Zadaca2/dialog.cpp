#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    mydb=QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/hp/Desktop/fax/OOP Zadace/Zadaca2-pp/CLCHARGE.db");
    mydb.open();
    if(!mydb.open()){
        qDebug("Info: Baza podataka nije otvorena!");
    }
    else{
        qDebug("Info: Baza podataka je uspješno otvorena!");
    }

    QSqlQueryModel *fakture = new QSqlQueryModel(this);
    QSqlQuery loadFakture;
    if(loadFakture.exec("SELECT * FROM FAKTURA"))
    {
        fakture->setQuery(loadFakture);
        ui->tableView_3->setModel(fakture);
    }

    updateTable();
    provjera();
}

Dialog::~Dialog()
{
    delete ui;    
}

void Dialog::updateTable()
{
    QSqlQueryModel *sqlmodel = new QSqlQueryModel(this);
    QSqlQuery loadQuery;
    if(loadQuery.exec("SELECT * FROM PRINTERI"))
    {
        sqlmodel->setQuery(loadQuery);
        ui->tableView->setModel(sqlmodel);
    }
    QSqlQueryModel *fakture = new QSqlQueryModel(this);
    QSqlQuery loadFakture;
    if(loadFakture.exec("SELECT * FROM FAKTURA"))
    {
        fakture->setQuery(loadFakture);
        ui->tableView_3->setModel(fakture);
    }
    QSqlQueryModel *stampa = new QSqlQueryModel(this);
    QSqlQuery loadStampa;
    if(loadStampa.exec("SELECT * FROM STAMPA"))
    {
        stampa->setQuery(loadStampa);
        ui->tableView_2->setModel(stampa);
    }
}

void Dialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    //Obiljezavanje klika
    QString val = ui->tableView->model()->index(index.row(),0).data().toString();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM PRINTERI WHERE Serijski = '"+val+"'");
    if(qry.exec())
    {
        while(qry.next())
        {
            QString mjesec = QString::number(ui->dateEdit->date().month());
            QString godina = QString::number(ui->dateEdit->date().year());

            QString pocenoStanje = qry.value(1).toString();
            int pocetnoStanjeInt = pocenoStanje.toInt();
            QString serijski = qry.value(0).toString();

            Dialog2 dialog2;
            dialog2.setModal(true);
            dialog2.setSerijski(serijski);
            dialog2.setMjesec(mjesec);
            dialog2.setGodina(godina);
            dialog2.setPocetnoStanje(pocetnoStanjeInt);
            dialog2.exec();


            provjera();

        }
    }

/*
   QSqlQuery query;
    query.prepare("Delete from STAMPA");
     if(!query.exec() )
        qDebug() << "deletion failed";

     query.clear();
     query.prepare("DELETE FROM SQLITE_SEQUENCE WHERE name='FAKTURA'");
     if(!query.exec() )
        qDebug() << "deletion failed";
*/

    updateTable();
    provjera();

}

//Provjerava ispunjeno, next button
void Dialog::provjera()
{
    QSqlQueryModel *modelAll = new QSqlQueryModel(this);
    QSqlQueryModel *nextBtnModel = new QSqlQueryModel(this);
    QSqlQuery loadAllQry;
    if(loadAllQry.exec("SELECT * FROM PRINTERI"))
    {
        modelAll->setQuery(loadAllQry);

        QSqlQuery nextButtonQry;
        if(nextButtonQry.exec("SELECT * FROM PRINTERI WHERE Ispunjeno = 1"))
        {
          nextBtnModel->setQuery(nextButtonQry);
          if(nextBtnModel->rowCount() == modelAll->rowCount())
          {
              ui->pushButton->setEnabled(true);
          }else {
              ui->pushButton->setEnabled(false);
          }
        }
    }
}



void Dialog::spremanje()
{
        QSqlQuery qry5;
        QSqlQuery qry6;
        qry5.prepare("insert into STAMPA(Serijski,Godina,Mjesec,Stranice) values ('"+serijski_s+"', '"+god_s+"', '"+mj_s+"', '"+spin+"') ");
        //qry5.prepare("update STAMPA set Serijski='"+serijski_s+"', Godina= '"+god_s+"', Mjesec='"+mj_s+"', Stranice='"+spin+"' ");
        if(qry5.exec()){
         qDebug() << "Podaci su spremljeni!";
         //Stavlja ispunjeno na 1 kada se klikne na next
         QString a=QString::number(0);
         qry6.prepare("update PRINTERI set Ispunjeno='"+a+"' where Serijski='"+serijski_s+"' ");
         qry6.exec();
        }else {
            qDebug() << "Podaci nisu spremljeni";
        }
}

//Zavrsi unos button
void Dialog::on_pushButton_clicked()
{
    int trenutniMjesec = ui->dateEdit->date().month();
    int trenutnaGodina = ui->dateEdit->date().year();

    int rezultatPrviMjesec;
    int rezultatDrugiMjesec;
    double cijena;
    float ukupnaCijena;
    QDate datum;
    int ukupanBrojStranica;
    int prosliMjesec = trenutniMjesec-1;

    if(prosliMjesec < 1)
    {
        prosliMjesec = 12;

    }

    QSqlQuery tmqry;
    tmqry.prepare("SELECT SUM(Stranice) FROM STAMPA WHERE Mjesec = :trenutniMjesec");
    tmqry.bindValue(":trenutniMjesec", trenutniMjesec);
    if(tmqry.exec())
    {
        while(tmqry.next())
        {
            rezultatPrviMjesec =  tmqry.value(0).toInt();
        }
        QSqlQuery pmqry;
        pmqry.prepare("SELECT SUM(Stranice) FROM STAMPA WHERE Mjesec = :prosliMjesec");
        pmqry.bindValue(":prosliMjesec", prosliMjesec);
        if(pmqry.exec())
        {
            while(pmqry.next())
            {
                rezultatDrugiMjesec = pmqry.value(0).toInt();
            }
        }
        ukupanBrojStranica = rezultatPrviMjesec - rezultatDrugiMjesec;


        QSqlQuery faktura;
        faktura.prepare("INSERT INTO FAKTURA  (Godina, Mjesec, Cijena, UkupnaCijena, Stranice) VALUES (:Godina, :Mjesec, :Cijena, :UkupnaCijena, :Stranice)");
        faktura.bindValue(":Godina", trenutnaGodina);
        faktura.bindValue(":Mjesec", trenutniMjesec);

        cijena = ui->lineEdit->text().toDouble();

        faktura.bindValue(":Cijena", cijena);
        ukupnaCijena = cijena * rezultatPrviMjesec;

        faktura.bindValue(":UkupnaCijena", ukupnaCijena);
        faktura.bindValue(":Stranice", ukupanBrojStranica);
        if(faktura.exec()){
                qDebug() << "Upisana faktura!";
            }

        if(trenutniMjesec == 12)
        {
            trenutniMjesec = 1;
            trenutnaGodina++;

            datum.setDate(trenutnaGodina,trenutniMjesec,1);
            ui->dateEdit->setDate(datum);
        }else{
            trenutniMjesec++;
            datum.setDate(trenutnaGodina,trenutniMjesec,1);
            ui->dateEdit->setDate(datum);

        }
        ui->pushButton->setEnabled(false);


       QSqlQuery printeri;
       printeri.prepare("UPDATE PRINTERI SET Ispunjeno = 0 WHERE Ispunjeno = 1");
       printeri.exec();

    }

    updateTable();
}

