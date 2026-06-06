#include "mainwindow.h"
#include "client.h"
#include "adminpanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QDateTime>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QSplitter>

// ── Утилиты крипто ──────────────────────────────────────────
static QString simpleAesEncrypt(const QString& text, const QString& key)
{
    // Простая демо-реализация XOR (в учебных целях)
    QByteArray data = text.toUtf8();
    QByteArray k    = key.toUtf8();
    if (k.isEmpty()) k = "defaultkey";

    QByteArray result;
    for (int i = 0; i < data.size(); i++)
    {
        result.append(data[i] ^ k[i % k.size()]);
    }
    return result.toHex();
}

static QString sha1Hash(const QString& text)
{
    return QCryptographicHash::hash(
        text.toUtf8(),
        QCryptographicHash::Sha1).toHex();
}

static double newtonSqrt(double n, int iters = 10)
{
    if (n < 0) return -1;
    double x = n / 2.0;
    for (int i = 0; i < iters; i++)
        x = (x + n / x) / 2.0;
    return x;
}

// ── MainWindow ───────────────────────────────────────────────
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      isLoggedIn(false),
      isAdmin(false)
{
    setWindowTitle("Система авторизации");
    setMinimumSize(760, 560);
    resize(900, 650);

    setupUi();
    setupMenu();
    setupConnections();
    updateButtonStates(false, false);

    statusbar->showMessage("Не подключено к серверу");
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    centralwidget = new QWidget(this);
    setCentralWidget(centralwidget);

    // ── Левая колонка: авторизация ───────────────────────────
    QGroupBox* authGroup = new QGroupBox("Авторизация", centralwidget);

    labelLogin    = new QLabel("Логин:",    authGroup);
    loginEdit     = new QLineEdit(authGroup);
    loginEdit->setObjectName("loginEdit");
    loginEdit->setPlaceholderText("Введите логин");

    labelPassword = new QLabel("Пароль:",  authGroup);
    passwordEdit  = new QLineEdit(authGroup);
    passwordEdit->setObjectName("passwordEdit");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Введите пароль");

    loginButton    = new QPushButton("Войти",           authGroup);
    registerButton = new QPushButton("Регистрация",     authGroup);
    loginButton->setObjectName("loginButton");
    registerButton->setObjectName("registerButton");

    loginButton->setMinimumHeight(32);
    registerButton->setMinimumHeight(32);

    QGridLayout* authLayout = new QGridLayout(authGroup);
    authLayout->addWidget(labelLogin,    0, 0);
    authLayout->addWidget(loginEdit,     0, 1);
    authLayout->addWidget(labelPassword, 1, 0);
    authLayout->addWidget(passwordEdit,  1, 1);
    authLayout->addWidget(loginButton,   2, 0);
    authLayout->addWidget(registerButton,2, 1);

    // ── Левая колонка: функции ───────────────────────────────
    QGroupBox* funcGroup = new QGroupBox("Функции", centralwidget);

    aesButton       = new QPushButton("AES шифрование",  funcGroup);
    sha1Button      = new QPushButton("SHA1 хэш",        funcGroup);
    newtonButton    = new QPushButton("Метод Ньютона",   funcGroup);
    stegoButton     = new QPushButton("Стеганография",   funcGroup);
    showUsersButton = new QPushButton("Панель администратора", funcGroup);

    aesButton->setObjectName("aesButton");
    sha1Button->setObjectName("sha1Button");
    newtonButton->setObjectName("newtonButton");
    stegoButton->setObjectName("stegoButton");
    showUsersButton->setObjectName("showUsersButton");

    for (auto* btn : {aesButton, sha1Button, newtonButton,
                      stegoButton, showUsersButton})
    {
        btn->setMinimumHeight(32);
    }

    showUsersButton->setStyleSheet(
        "QPushButton { background-color: #c0392b; color: white; }"
        "QPushButton:hover { background-color: #e74c3c; }"
        "QPushButton:disabled { background-color: #888; }");

    QVBoxLayout* funcLayout = new QVBoxLayout(funcGroup);
    funcLayout->addWidget(aesButton);
    funcLayout->addWidget(sha1Button);
    funcLayout->addWidget(newtonButton);
    funcLayout->addWidget(stegoButton);
    funcLayout->addSpacing(10);
    funcLayout->addWidget(showUsersButton);

    // ── Левая колонка целиком ────────────────────────────────
    QWidget* leftPanel = new QWidget(centralwidget);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->addWidget(authGroup);
    leftLayout->addWidget(funcGroup);
    leftLayout->addStretch();
    leftPanel->setFixedWidth(260);

    // ── Правая колонка: результат + таблица ─────────────────
    QGroupBox* resultGroup = new QGroupBox("Результат", centralwidget);
    resultText = new QTextEdit(resultGroup);
    resultText->setObjectName("resultText");
    resultText->setReadOnly(true);
    resultText->setFont(QFont("Consolas", 9));

    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    resultLayout->addWidget(resultText);

    QGroupBox* tableGroup = new QGroupBox("Пользователи", centralwidget);
    tableUsers = new QTableWidget(tableGroup);
    tableUsers->setObjectName("tableUsers");
    tableUsers->setColumnCount(2);
    tableUsers->setHorizontalHeaderLabels({"Логин", "Роль"});
    tableUsers->horizontalHeader()->setStretchLastSection(true);
    tableUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableUsers->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableUsers->setVisible(false);

    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->addWidget(tableUsers);

    QWidget* rightPanel = new QWidget(centralwidget);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->addWidget(resultGroup, 2);
    rightLayout->addWidget(tableGroup,  1);

    // ── Главный layout ───────────────────────────────────────
    QHBoxLayout* mainLayout = new QHBoxLayout(centralwidget);
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    menubar  = menuBar();
    statusbar = statusBar();
}

void MainWindow::setupMenu()
{
    QMenu* fileMenu = menubar->addMenu("Файл");
    QAction* exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* helpMenu = menubar->addMenu("Справка");
    QAction* aboutAction = helpMenu->addAction("О программе");
    connect(aboutAction, &QAction::triggered, this, [this]()
    {
        QMessageBox::information(this, "О программе",
            "Система авторизации v1.0\n\n"
            "Функции:\n"
            "• Регистрация и авторизация пользователей\n"
            "• AES шифрование (XOR демо)\n"
            "• SHA1 хэширование\n"
            "• Метод Ньютона (извлечение корня)\n"
            "• Стеганография (LSB)\n"
            "• Панель администратора\n\n"
            "Данные хранятся в файле users.txt");
    });
}

void MainWindow::setupConnections()
{
    connect(loginButton,    &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(newtonButton,   &QPushButton::clicked, this, &MainWindow::onNewtonClicked);
    connect(aesButton,      &QPushButton::clicked, this, &MainWindow::onAesClicked);
    connect(sha1Button,     &QPushButton::clicked, this, &MainWindow::onSha1Clicked);
    connect(stegoButton,    &QPushButton::clicked, this, &MainWindow::onStegoClicked);
    connect(showUsersButton,&QPushButton::clicked, this, &MainWindow::onShowUsersClicked);

    // Enter в полях логина/пароля
    connect(passwordEdit, &QLineEdit::returnPressed,
            loginButton, &QPushButton::click);
    connect(loginEdit, &QLineEdit::returnPressed,
            [this]() { passwordEdit->setFocus(); });
}

void MainWindow::updateButtonStates(bool loggedIn, bool isAdm)
{
    aesButton->setEnabled(loggedIn);
    sha1Button->setEnabled(loggedIn);
    newtonButton->setEnabled(loggedIn);
    stegoButton->setEnabled(loggedIn);
    showUsersButton->setEnabled(loggedIn && isAdm);

    loginButton->setEnabled(!loggedIn);
    registerButton->setEnabled(!loggedIn);
    loginEdit->setEnabled(!loggedIn);
    passwordEdit->setEnabled(!loggedIn);
}

void MainWindow::appendResult(const QString& text)
{
    QString ts = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    resultText->append(ts + text);
}

// ── Слоты ────────────────────────────────────────────────────
void MainWindow::onLoginClicked()
{
    QString login = loginEdit->text().trimmed();
    QString pass  = passwordEdit->text();

    if (login.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    statusbar->showMessage("Подключение к серверу...");

    QString response = Client::getInstance().loginUser(login, pass);

    if (response == "CONNECTION_ERROR" || response == "SERVER_ERROR")
    {
        statusbar->showMessage("Ошибка соединения с сервером");
        QMessageBox::critical(this, "Ошибка",
            "Не удалось подключиться к серверу.\n"
            "Убедитесь, что сервер запущен (порт 8080).");
        return;
    }

    if (response.startsWith("LOGIN_SUCCESS"))
    {
        isLoggedIn   = true;
        currentLogin = login;
        isAdmin      = (Client::getInstance().getCurrentRole() == "ADMIN");

        updateButtonStates(true, isAdmin);

        QString roleStr = isAdmin ? "Администратор" : "Пользователь";
        setWindowTitle("Система авторизации — " + login + " (" + roleStr + ")");
        statusbar->showMessage("Авторизован: " + login + " | Роль: " + roleStr);

        appendResult("✓ Авторизация успешна. Роль: " + roleStr);
        QMessageBox::information(this, "Успех",
            "Добро пожаловать, " + login + "!\nРоль: " + roleStr);
    }
    else
    {
        statusbar->showMessage("Ошибка авторизации");
        appendResult("✗ Ошибка авторизации для: " + login);
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль.");
    }
}

void MainWindow::onRegisterClicked()
{
    QString login = loginEdit->text().trimmed();
    QString pass  = passwordEdit->text();

    if (login.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль.");
        return;
    }

    if (pass.length() < 3)
    {
        QMessageBox::warning(this, "Ошибка", "Пароль должен содержать минимум 3 символа.");
        return;
    }

    QString response = Client::getInstance().registerUser(login, pass);

    if (response == "CONNECTION_ERROR" || response == "SERVER_ERROR")
    {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось подключиться к серверу.");
        return;
    }

    if (response == "REGISTER_SUCCESS")
    {
        appendResult("✓ Пользователь зарегистрирован: " + login);
        QMessageBox::information(this, "Успех",
            "Пользователь \"" + login + "\" успешно зарегистрирован!\n"
            "Теперь вы можете войти.");
        statusbar->showMessage("Зарегистрирован: " + login);
    }
    else
    {
        appendResult("✗ Ошибка регистрации: " + login);
        QMessageBox::warning(this, "Ошибка",
            "Регистрация не удалась.\n"
            "Возможно, пользователь с таким логином уже существует.");
    }
}

void MainWindow::onAesClicked()
{
    bool ok;
    QString text = QInputDialog::getText(
        this, "AES шифрование",
        "Введите текст для шифрования:", QLineEdit::Normal, "", &ok);

    if (!ok || text.isEmpty()) return;

    QString key = QInputDialog::getText(
        this, "AES ключ", "Введите ключ:", QLineEdit::Normal, "secret", &ok);

    if (!ok) return;

    QString encrypted = simpleAesEncrypt(text, key);
    appendResult("─── AES шифрование ───");
    appendResult("Текст:      " + text);
    appendResult("Ключ:       " + key);
    appendResult("Результат:  " + encrypted);
    appendResult("Алгоритм:   XOR (демо-режим)");
    statusbar->showMessage("AES: зашифровано");
}

void MainWindow::onSha1Clicked()
{
    bool ok;
    QString text = QInputDialog::getText(
        this, "SHA1",
        "Введите текст для хэширования:", QLineEdit::Normal, "", &ok);

    if (!ok || text.isEmpty()) return;

    QString hash = sha1Hash(text);
    appendResult("─── SHA1 хэш ───");
    appendResult("Текст:  " + text);
    appendResult("SHA1:   " + hash);
    statusbar->showMessage("SHA1: вычислен");
}

void MainWindow::onNewtonClicked()
{
    bool ok;
    double n = QInputDialog::getDouble(
        this, "Метод Ньютона",
        "Введите число для извлечения корня:", 2.0, 0.0, 1e12, 4, &ok);

    if (!ok) return;

    double result = newtonSqrt(n);
    int iters = 10;

    appendResult("─── Метод Ньютона ───");
    appendResult(QString("Число:        %1").arg(n));
    appendResult(QString("√%1 ≈ %2").arg(n).arg(result, 0, 'f', 10));
    appendResult(QString("Итераций:     %1").arg(iters));
    appendResult(QString("Проверка:     %1² = %2").arg(result, 0, 'f', 6).arg(result * result, 0, 'f', 6));
    statusbar->showMessage("Метод Ньютона: вычислено");
}

// ── WAV LSB стеганография ────────────────────────────────────
// Формат WAV: 44 байта заголовок, затем PCM-сэмплы (8-bit или 16-bit)
// Прячем сообщение в младший бит каждого байта сэмпла после заголовка.
// Первые 32 бита = длина сообщения в байтах, затем биты текста.

static bool wavHideMessage(const QString& wavPath,
                           const QString& outPath,
                           const QString& message)
{
    QFile inFile(wavPath);
    if (!inFile.open(QIODevice::ReadOnly)) return false;

    QByteArray wavData = inFile.readAll();
    inFile.close();

    // Минимальная проверка WAV-заголовка
    if (wavData.size() < 44) return false;
    if (wavData.left(4) != "RIFF") return false;
    if (wavData.mid(8, 4) != "WAVE") return false;

    QByteArray msgBytes = message.toUtf8();
    quint32 msgLen = (quint32)msgBytes.size();

    // Нужно (4 + msgLen) байт * 8 бит, по 1 биту на сэмпл после заголовка
    int bitsNeeded = (4 + msgLen) * 8;
    int availableSamples = wavData.size() - 44;

    if (availableSamples < bitsNeeded)
        return false; // файл слишком маленький

    // Формируем битовый поток: сначала 4 байта длины, потом текст
    QByteArray payload;
    payload.append((char)((msgLen >> 24) & 0xFF));
    payload.append((char)((msgLen >> 16) & 0xFF));
    payload.append((char)((msgLen >>  8) & 0xFF));
    payload.append((char)( msgLen        & 0xFF));
    payload.append(msgBytes);

    QByteArray result = wavData;

    int bitIndex = 0;
    for (int byteIdx = 0; byteIdx < payload.size(); byteIdx++)
    {
        unsigned char b = (unsigned char)payload[byteIdx];
        for (int bit = 7; bit >= 0; bit--)
        {
            int samplePos = 44 + bitIndex;
            unsigned char sample = (unsigned char)result[samplePos];
            // Записываем бит в LSB сэмпла
            sample = (sample & 0xFE) | ((b >> bit) & 1);
            result[samplePos] = (char)sample;
            bitIndex++;
        }
    }

    QFile outFile(outPath);
    if (!outFile.open(QIODevice::WriteOnly)) return false;
    outFile.write(result);
    outFile.close();
    return true;
}

static QString wavExtractMessage(const QString& wavPath)
{
    QFile file(wavPath);
    if (!file.open(QIODevice::ReadOnly)) return {};

    QByteArray wavData = file.readAll();
    file.close();

    if (wavData.size() < 44) return {};
    if (wavData.left(4) != "RIFF") return {};

    // Читаем первые 32 бита (4 байта длины)
    int availableSamples = wavData.size() - 44;
    if (availableSamples < 32) return {};

    // Извлекаем длину сообщения
    quint32 msgLen = 0;
    for (int bit = 0; bit < 32; bit++)
    {
        int samplePos = 44 + bit;
        unsigned char sample = (unsigned char)wavData[samplePos];
        msgLen = (msgLen << 1) | (sample & 1);
    }

    if (msgLen == 0 || msgLen > 100000) return {}; // защита от мусора

    if (availableSamples < (int)(32 + msgLen * 8)) return {};

    // Извлекаем байты сообщения
    QByteArray msgBytes;
    for (quint32 i = 0; i < msgLen; i++)
    {
        unsigned char b = 0;
        for (int bit = 0; bit < 8; bit++)
        {
            int samplePos = 44 + 32 + (int)(i * 8 + bit);
            unsigned char sample = (unsigned char)wavData[samplePos];
            b = (b << 1) | (sample & 1);
        }
        msgBytes.append((char)b);
    }

    return QString::fromUtf8(msgBytes);
}

void MainWindow::onStegoClicked()
{
    QStringList options = {"Скрыть сообщение в WAV", "Извлечь сообщение из WAV"};

    bool ok;
    QString choice = QInputDialog::getItem(
        this, "Стеганография WAV",
        "Выберите операцию:", options, 0, false, &ok);

    if (!ok) return;

    if (choice == options[0])
    {
        // ── Скрыть ───────────────────────────────────────────
        QString wavPath = QFileDialog::getOpenFileName(
            this, "Выберите WAV файл-носитель",
            "", "WAV файлы (*.wav)");

        if (wavPath.isEmpty()) return;

        QString message = QInputDialog::getText(
            this, "Скрыть сообщение",
            "Введите секретное сообщение:", QLineEdit::Normal, "", &ok);

        if (!ok || message.isEmpty()) return;

        QString outPath = QFileDialog::getSaveFileName(
            this, "Сохранить стего-файл",
            "stego_output.wav", "WAV файлы (*.wav)");

        if (outPath.isEmpty()) return;

        bool success = wavHideMessage(wavPath, outPath, message);

        appendResult("─── Стеганография WAV: скрыть ───");
        appendResult("Носитель:  " + QFileInfo(wavPath).fileName());
        appendResult("Сообщение: " + message);
        appendResult("Длина:     " + QString::number(message.toUtf8().size()) + " байт");

        if (success)
        {
            appendResult("Результат: " + QFileInfo(outPath).fileName());
            appendResult("Статус:    ✓ Сообщение успешно скрыто (LSB)");
            QMessageBox::information(this, "Успех",
                "Сообщение скрыто в файле:\n" + outPath);
        }
        else
        {
            appendResult("Статус: ✗ Ошибка — файл слишком мал или не является WAV");
            QMessageBox::critical(this, "Ошибка",
                "Не удалось скрыть сообщение.\n"
                "Убедитесь что файл является корректным WAV\n"
                "и достаточно большим для данного сообщения.");
        }
    }
    else
    {
        // ── Извлечь ──────────────────────────────────────────
        QString wavPath = QFileDialog::getOpenFileName(
            this, "Выберите стего WAV файл",
            "", "WAV файлы (*.wav)");

        if (wavPath.isEmpty()) return;

        QString extracted = wavExtractMessage(wavPath);

        appendResult("─── Стеганография WAV: извлечь ───");
        appendResult("Файл: " + QFileInfo(wavPath).fileName());

        if (extracted.isEmpty())
        {
            appendResult("Статус: ✗ Сообщение не найдено или файл повреждён");
            QMessageBox::warning(this, "Не найдено",
                "Скрытое сообщение не обнаружено.");
        }
        else
        {
            appendResult("Сообщение: " + extracted);
            appendResult("Длина:     " + QString::number(extracted.toUtf8().size()) + " байт");
            appendResult("Статус:    ✓ Сообщение успешно извлечено");
            QMessageBox::information(this, "Сообщение найдено",
                "Извлечённое сообщение:\n\n" + extracted);
        }
    }

    statusbar->showMessage("Стеганография: выполнено");
}

void MainWindow::onShowUsersClicked()
{
    if (!isAdmin)
    {
        QMessageBox::warning(this, "Доступ запрещён",
            "Панель администратора доступна только администраторам.");
        return;
    }

    AdminPanel* panel = new AdminPanel(this);
    panel->exec();

    // После закрытия панели — обновляем таблицу
    QString response = Client::getInstance().getUsersList();
    if (!response.startsWith("CONNECTION_ERROR") &&
        !response.startsWith("SERVER_ERROR")    &&
        response != "USERS_EMPTY")
    {
        QStringList lines = response.split("\n", Qt::SkipEmptyParts);
        tableUsers->setVisible(true);
        tableUsers->setRowCount(lines.size());

        for (int i = 0; i < lines.size(); i++)
        {
            QStringList parts = lines[i].split("|");
            tableUsers->setItem(i, 0,
                new QTableWidgetItem(parts.value(0)));
            tableUsers->setItem(i, 1,
                new QTableWidgetItem(parts.value(1)));
        }
    }
}
