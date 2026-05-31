#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "client.h"
#include <QString>

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
    QString result =
		Client::getInstance().sendNewton();

	ui->resultText->append(result);
}

void MainWindow::on_stegoButton_clicked()
{
    QString result =
		Client::getInstance().sendStego("test");

	ui->resultText->append(result);
}