#include "adminpanel.h"
#include "client.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>
#include <QStatusBar>
#include <QLabel>
#include <QFont>

AdminPanel::AdminPanel(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Панель администратора");
    setMinimumSize(600, 450);
    resize(700, 500);
    setModal(true);

    setupUi();
    loadUsers();
}

void AdminPanel::setupUi()
{
    // ── Заголовок ────────────────────────────────────────────
    QLabel* title = new QLabel("Управление пользователями", this);
    QFont f = title->font();
    f.setPointSize(13);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #c0392b; padding: 8px;");

    // ── Статистика ───────────────────────────────────────────
    lblStats = new QLabel("Загрузка...", this);
    lblStats->setAlignment(Qt::AlignCenter);

    // ── Таблица ──────────────────────────────────────────────
    tableUsers = new QTableWidget(this);
    tableUsers->setColumnCount(2);
    tableUsers->setHorizontalHeaderLabels({"Логин", "Роль"});
    tableUsers->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableUsers->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableUsers->setAlternatingRowColors(true);
    tableUsers->verticalHeader()->setVisible(false);

    // ── Кнопки действий ─────────────────────────────────────
    btnRefresh = new QPushButton("🔄 Обновить",       this);
    btnAdd     = new QPushButton("➕ Добавить",        this);
    btnPromote = new QPushButton("⬆ Сделать ADMIN",   this);
    btnDemote  = new QPushButton("⬇ Сделать USER",    this);
    btnDelete  = new QPushButton("🗑 Удалить",         this);

    btnDelete->setStyleSheet(
        "QPushButton { background-color: #c0392b; color: white; }"
        "QPushButton:hover { background-color: #e74c3c; }");

    btnPromote->setStyleSheet(
        "QPushButton { background-color: #27ae60; color: white; }"
        "QPushButton:hover { background-color: #2ecc71; }");

    for (auto* btn : {btnRefresh, btnAdd, btnPromote, btnDemote, btnDelete})
        btn->setMinimumHeight(30);

    // ── Кнопки в строку ─────────────────────────────────────
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnRefresh);
    btnLayout->addWidget(btnAdd);
    btnLayout->addStretch();
    btnLayout->addWidget(btnPromote);
    btnLayout->addWidget(btnDemote);
    btnLayout->addWidget(btnDelete);

    // ── Закрыть ──────────────────────────────────────────────
    QPushButton* btnClose = new QPushButton("Закрыть", this);
    btnClose->setMinimumHeight(30);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnClose);

    // ── Главный layout ───────────────────────────────────────
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addWidget(lblStats);
    mainLayout->addWidget(tableUsers, 1);
    mainLayout->addLayout(btnLayout);
    mainLayout->addLayout(bottomLayout);

    // ── Соединения ───────────────────────────────────────────
    connect(btnRefresh, &QPushButton::clicked, this, &AdminPanel::onRefresh);
    connect(btnDelete,  &QPushButton::clicked, this, &AdminPanel::onDeleteUser);
    connect(btnPromote, &QPushButton::clicked, this, &AdminPanel::onPromoteUser);
    connect(btnDemote,  &QPushButton::clicked, this, &AdminPanel::onDemoteUser);
    connect(btnAdd,     &QPushButton::clicked, this, &AdminPanel::onAddUser);
}

void AdminPanel::loadUsers()
{
    QString response = Client::getInstance().getUsersList();

    if (response == "CONNECTION_ERROR" || response == "SERVER_ERROR")
    {
        lblStats->setText("❌ Ошибка соединения с сервером");
        return;
    }

    if (response == "USERS_EMPTY")
    {
        tableUsers->setRowCount(0);
        lblStats->setText("Список пользователей пуст");
        return;
    }

    QStringList lines = response.split("\n", Qt::SkipEmptyParts);
    tableUsers->setRowCount(lines.size());

    int adminCount = 0;
    int userCount  = 0;

    for (int i = 0; i < lines.size(); i++)
    {
        QStringList parts = lines[i].split("|");
        QString login = parts.value(0).trimmed();
        QString role  = parts.value(1).trimmed();

        auto* loginItem = new QTableWidgetItem(login);
        auto* roleItem  = new QTableWidgetItem(role);

        if (role == "ADMIN")
        {
            roleItem->setForeground(Qt::red);
            roleItem->setFont([]{ QFont f; f.setBold(true); return f; }());
            adminCount++;
        }
        else
        {
            userCount++;
        }

        tableUsers->setItem(i, 0, loginItem);
        tableUsers->setItem(i, 1, roleItem);
    }

    lblStats->setText(
        QString("Всего пользователей: %1  |  Администраторов: %2  |  Пользователей: %3")
        .arg(lines.size()).arg(adminCount).arg(userCount));
}

QString AdminPanel::getSelectedLogin()
{
    auto items = tableUsers->selectedItems();
    if (items.isEmpty())
    {
        QMessageBox::warning(this, "Внимание",
            "Выберите пользователя в таблице.");
        return {};
    }
    int row = tableUsers->currentRow();
    return tableUsers->item(row, 0)->text();
}

void AdminPanel::onRefresh()
{
    loadUsers();
}

void AdminPanel::onDeleteUser()
{
    QString login = getSelectedLogin();
    if (login.isEmpty()) return;

    if (login == "admin")
    {
        QMessageBox::warning(this, "Запрещено",
            "Нельзя удалить главного администратора.");
        return;
    }

    auto btn = QMessageBox::question(
        this, "Удаление",
        QString("Удалить пользователя \"%1\"?").arg(login),
        QMessageBox::Yes | QMessageBox::No);

    if (btn != QMessageBox::Yes) return;

    QString response = Client::getInstance().deleteUser(login);

    if (response == "DELETE_SUCCESS")
    {
        QMessageBox::information(this, "Успех",
            QString("Пользователь \"%1\" удалён.").arg(login));
        loadUsers();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось удалить пользователя.");
    }
}

void AdminPanel::onPromoteUser()
{
    QString login = getSelectedLogin();
    if (login.isEmpty()) return;

    QString response = Client::getInstance().changeUserRole(login, "ADMIN");

    if (response == "ROLE_SUCCESS")
    {
        QMessageBox::information(this, "Успех",
            QString("Пользователь \"%1\" теперь ADMIN.").arg(login));
        loadUsers();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось изменить роль (нельзя изменить главного admin).");
    }
}

void AdminPanel::onDemoteUser()
{
    QString login = getSelectedLogin();
    if (login.isEmpty()) return;

    if (login == "admin")
    {
        QMessageBox::warning(this, "Запрещено",
            "Нельзя понизить главного администратора.");
        return;
    }

    QString response = Client::getInstance().changeUserRole(login, "USER");

    if (response == "ROLE_SUCCESS")
    {
        QMessageBox::information(this, "Успех",
            QString("Пользователь \"%1\" теперь USER.").arg(login));
        loadUsers();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось изменить роль.");
    }
}

void AdminPanel::onAddUser()
{
    bool ok;
    QString login = QInputDialog::getText(
        this, "Добавить пользователя",
        "Логин нового пользователя:", QLineEdit::Normal, "", &ok);
    if (!ok || login.trimmed().isEmpty()) return;

    QString pass = QInputDialog::getText(
        this, "Добавить пользователя",
        "Пароль:", QLineEdit::Password, "", &ok);
    if (!ok || pass.isEmpty()) return;

    QString response = Client::getInstance().registerUser(login.trimmed(), pass);

    if (response == "REGISTER_SUCCESS")
    {
        QMessageBox::information(this, "Успех",
            QString("Пользователь \"%1\" создан.").arg(login));
        loadUsers();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось создать пользователя.\n"
            "Возможно, такой логин уже существует.");
    }
}
