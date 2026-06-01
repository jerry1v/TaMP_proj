#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();

    void on_aesButton_clicked();
    void on_sha1Button_clicked();
    void on_newtonButton_clicked();
    void on_stegoButton_clicked();
	
	void on_showUsersButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif