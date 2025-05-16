#include "login.h"
#include "signin.h"
#include "Master.h"
#include "chat.h"
#include "recuperarpass.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QPixmap>
#include <QFont>

Login::Login(QWidget *parent)
    : QWidget(parent), master(new Master())
{
    // Widgets de entrada
    usuarioLabel = new QLabel("Usuario:");
    contrasenaLabel = new QLabel("Contraseña:");

    usuarioInput = new QLineEdit;
    contrasenaInput = new QLineEdit;
    contrasenaInput->setEchoMode(QLineEdit::Password);

    botonLogin = new QPushButton("Iniciar Sesión");

    botonOlvidaste = new QPushButton("¿Olvidaste tu contraseña?");
    botonOlvidaste->setFlat(true);
    botonOlvidaste->setStyleSheet("color: blue; text-decoration: underline;");

    botonSignin = new QPushButton("¿No tienes una cuenta? Crear Cuenta");
    botonSignin->setFlat(true);
    botonSignin->setStyleSheet("color: cyan; text-decoration: underline;");

    // Conexiones de Botones
    connect(botonLogin, &QPushButton::clicked, this, &Login::verificarCredenciales);

    connect(botonOlvidaste, &QPushButton::clicked, this, [=]() {
        RecuperarPass *ventanaRecuperar = new RecuperarPass();
        ventanaRecuperar->show();
        this->hide();
        connect(ventanaRecuperar, &RecuperarPass::destroyed, this, [=]() {
            this->show();
        });
    });

    connect(botonSignin, &QPushButton::clicked, this, [=]() {
        SigninWindow *signin = new SigninWindow(master, this);
        signin->show();
        this->hide();
        connect(signin, &SigninWindow::destroyed, this, [=]() {
            this->show();
        });
    });

    // Configuración de fuentes
    QFont font("Arial", 12);
    usuarioLabel->setFont(font);
    contrasenaLabel->setFont(font);
    usuarioInput->setFont(font);
    contrasenaInput->setFont(font);
    botonLogin->setFont(font);
    botonOlvidaste->setFont(font);
    botonSignin->setFont(font);

    // Header
    QFrame *headerFrame = new QFrame;
    headerFrame->setStyleSheet("background-color: #004080;");
    headerFrame->setFixedHeight(50);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *labelHeader = new QLabel("Bienvenido");
    labelHeader->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
    labelHeader->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(labelHeader);
    headerLayout->addStretch();
    headerFrame->setLayout(headerLayout);

    // Frame derecho (celeste) - más delgado (150px)
    QFrame *rightFrame = new QFrame;
    rightFrame->setStyleSheet("background-color: #cceeff;");
    rightFrame->setFixedWidth(150);  // Ancho reducido del frame celeste

    // Layout para el frame derecho
    QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
    rightLayout->addStretch();
    rightFrame->setLayout(rightLayout);

    // Layout izquierdo (formulario de login)
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addSpacing(20);
    leftLayout->addWidget(usuarioLabel);
    leftLayout->addWidget(usuarioInput);
    leftLayout->addSpacing(15);
    leftLayout->addWidget(contrasenaLabel);
    leftLayout->addWidget(contrasenaInput);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(botonLogin);
    leftLayout->addSpacing(15);
    leftLayout->addWidget(botonOlvidaste, 0, Qt::AlignCenter);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(botonSignin, 0, Qt::AlignCenter);
    leftLayout->addStretch();

    QWidget *leftWidget = new QWidget;
    leftWidget->setLayout(leftLayout);
    leftWidget->setFixedWidth(300);  // Ancho fijo para el formulario

    // Layout central que contiene izquierda y derecha
    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout->addWidget(leftWidget);
    centerLayout->addWidget(rightFrame);
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Configuración de la ventana (tamaño ajustado)
    setWindowTitle("Login");
    resize(500, 400);  // Tamaño más compacto: 500px ancho x 400px alto
}

void Login::verificarCredenciales()
{
    QString usuario = usuarioInput->text().trimmed();
    QString contrasena = contrasenaInput->text().trimmed();

    if(master->login(usuario, contrasena)) {
        // Pasar Master a Chat
        Chat *ventanaChat = new Chat(master);
        ventanaChat->show();


        this->close();
    } else {
        QMessageBox::warning(this, "Error", "Usuario o Contraseña Incorrecto");
        usuarioInput->clear();
        contrasenaInput->clear();
    }
}
