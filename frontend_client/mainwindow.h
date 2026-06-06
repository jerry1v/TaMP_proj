#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QDialog>

class AdminPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onNewtonClicked();
    void onAesClicked();
    void onSha1Clicked();
    void onStegoClicked();
    void onShowUsersClicked();

private:
    void setupUi();
    void setupMenu();
    void setupConnections();
    void updateButtonStates(bool loggedIn, bool isAdmin);
    void appendResult(const QString& text);

    // ── Виджеты (точно как в QT Designer скриншоте) ──────────
    QWidget*      centralwidget;

    QPushButton*  aesButton;
    QLabel*       labelLogin;
    QLabel*       labelPassword;
    QPushButton*  loginButton;
    QLineEdit*    loginEdit;
    QPushButton*  newtonButton;
    QLineEdit*    passwordEdit;
    QPushButton*  registerButton;
    QTextEdit*    resultText;
    QPushButton*  sha1Button;
    QPushButton*  showUsersButton;
    QPushButton*  stegoButton;
    QTableWidget* tableUsers;

    QMenuBar*     menubar;
    QStatusBar*   statusbar;

    // ── Состояние ─────────────────────────────────────────────
    bool          isLoggedIn;
    bool          isAdmin;
    QString       currentLogin;
};
