#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"
#include <QtCore>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Client::getInstance().connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerButton_clicked()
{
    QString result =
        Client::getInstance().registerUser(
            ui->loginEdit->text(),
            ui->passwordEdit->text());
    ui->resultText->append(result);
}

void MainWindow::on_loginButton_clicked()
{
    QString result =
        Client::getInstance().loginUser(
            ui->loginEdit->text(),
            ui->passwordEdit->text());
    ui->resultText->append(result);
}

void MainWindow::on_aesButton_clicked()
{
    QString result =
        Client::getInstance().sendAES("test");
    ui->resultText->append(result);
}

void MainWindow::on_sha1Button_clicked()
{
    QString result =
        Client::getInstance().sendSHA1("test");
    ui->resultText->append(result);
}

void MainWindow::on_newtonButton_clicked()
{
    // берём число из loginEdit, либо захардкодить 2.0
    double number = ui->loginEdit->text().toDouble();
    QString result =
        Client::getInstance().sendNewton(number);
    ui->resultText->append(result);
}

void MainWindow::on_stegoButton_clicked()
{
    QString result =
        Client::getInstance().sendStego("test");
    ui->resultText->append(result);
}

void MainWindow::on_showUsersButton_clicked()
{
    QString response =
        Client::getInstance().getUsers();
    QStringList rows = response.split("\n");
    ui->tableUsers->setColumnCount(2);
    ui->tableUsers->setHorizontalHeaderLabels({"Login", "Role"});
    ui->tableUsers->setRowCount(rows.size());
    for (int i = 0; i < rows.size(); i++)
    {
        QStringList parts = rows[i].split(" ");
        if (parts.size() < 2) continue;
        ui->tableUsers->setItem(i, 0, new QTableWidgetItem(parts[0]));
        ui->tableUsers->setItem(i, 1, new QTableWidgetItem(parts[1]));
    }
}