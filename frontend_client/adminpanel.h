#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QGroupBox>

class AdminPanel : public QDialog
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget* parent = nullptr);

private slots:
    void onRefresh();
    void onDeleteUser();
    void onPromoteUser();
    void onDemoteUser();
    void onAddUser();

private:
    void setupUi();
    void loadUsers();
    QString getSelectedLogin();

    QTableWidget* tableUsers;
    QPushButton*  btnRefresh;
    QPushButton*  btnDelete;
    QPushButton*  btnPromote;
    QPushButton*  btnDemote;
    QPushButton*  btnAdd;
    QLabel*       lblStats;
    QStatusBar*   statusBar;
};
