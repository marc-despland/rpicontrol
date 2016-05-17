#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtScript/QScriptEngine>
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scanButton_clicked()
{
    qDebug() << "Received a click event";
     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));

    QUrl url("http://127.0.0.1:8080/api/tracker/scan");
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply* currentReply = manager->get(request);  // GET
    if (currentReply->error()) {
        qDebug() << "Can't execute the request";
    }


}

void MainWindow::onResult(QNetworkReply* reply)
{
    qDebug() << "Received response";
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occurs";
        return;  // ...only in a blog post
    }

    QString data = (QString) reply->readAll();
    qDebug() << "data : " <<data;
    QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));

    // Get JSON object
    QJsonObject json = doc.object();
    QJsonObject left=json.value(QString("left")).toObject();
    ui->leftlux->setText(QString::number(left.value(QString("lux")).toDouble()));
    ui->leftangle->setText(QString::number(left.value(QString("angle")).toDouble()));
    QJsonObject right=json.value(QString("right")).toObject();
    ui->rightlux->setText(QString::number(right.value(QString("lux")).toDouble()));
    ui->rightangle->setText(QString::number(right.value(QString("angle")).toDouble()));
    QJsonObject total=json.value(QString("total")).toObject();
    ui->sumlux->setText(QString::number(total.value(QString("lux")).toDouble()));
    ui->sumangle->setText(QString::number(total.value(QString("angle")).toDouble()));

}
