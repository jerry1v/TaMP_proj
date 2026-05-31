#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerButton_clicked()
{
    ui->resultText->append("Запрос на регистрацию");
}

void MainWindow::on_loginButton_clicked()
{
    ui->resultText->append("Запрос на авторизацию");
}

void MainWindow::on_aesButton_clicked()
{
    ui->resultText->append("AES шифрование");
}

void MainWindow::on_sha1Button_clicked()
{
    ui->resultText->append("SHA1 хэширование");
}

void MainWindow::on_newtonButton_clicked()
{
    ui->resultText->append("Метод Ньютона");
}

void MainWindow::on_stegoButton_clicked()
{
    ui->resultText->append("Стеганография");
}