#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QtSql>

QString setText, text_final;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query;
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    db.setHostName("127.0.0.1");
    db.setPort(3309);
    db.setUserName("root");
    db.setPassword("12345678");

    if(db.open())
        {
            QSqlQuery query;
            query.exec("SHOW DATABASES");
            while(query.next())
            {
                qDebug()<<query.value(0).toString();
            }

            db.close();
        }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "/desktop",
                                                      tr("TextFiles (*.txt)"));
    QFile file(fileName);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly))
            return;
    data = file.readAll();
        qDebug() << QString(data);

    QString search = ui->lineEdit->text();//шукане слово = слово з lineEdit

    QStringList lst = QString(data).replace(QRegExp("(\\.)"), ",")
                                     .split(",", QString::SkipEmptyParts);//розбиваємо на строки
    QString arr[999];

    for(int i = 0; i < lst.size(); i++){
       arr[i] = lst.at(i);
    }//розбиті строки записуємо в масив

    for(int i = 0; i < lst.size(); i++){
       if(arr[i].contains(search)){
       setText += arr[i];
       }
    }//вибираємо речення з словами які шукаємо

    for(int i = setText.size(); i >= 0; i--){
       text_final += setText[i];
    }// присвоюємо шукані речення в строку яка буде виводитись та зберігати всі попередні речення


    query.prepare("INSERT INTO  mydb (text_value) VALUES(?)");

    for(int i = 0; i <= text_final.size(); i++){
       query.addBindValue(text_final);
    }

    ui->textEdit->setText(text_final);

}
