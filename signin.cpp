#include "signin.h"
#include "login.h"
#include "chat.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QString>
#include <QChar>
#include <QFrame>
#include <QFont>

SigninWindow::SigninWindow(Master* master, Login *ventanaLogin, QWidget *parent)
    : QWidget(nullptr), loginVentana(ventanaLogin), master(master)  // Inicializar master
{
    // Header Azul
    QFrame *headerFrame = new QFrame;
    headerFrame->setStyleSheet("background-color: #004080;");
    headerFrame->setFixedHeight(50);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *labelHeader = new QLabel("Crear nueva cuenta");
    labelHeader->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
    labelHeader->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(labelHeader);
    headerLayout->addStretch();

    // Frame Celeste
    QFrame *rightFrame = new QFrame;
    rightFrame->setStyleSheet("background-color: #cceeff;");
    rightFrame->setFixedWidth(150);

    QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
    rightLayout->addStretch();

    // Controles del formulario
    QLabel *labelUsername = new QLabel("Nombre de usuario:");
    inputUsername = new QLineEdit;

    QLabel *labelNombreCompleto = new QLabel("Nombre completo:");
    inputNombreCompleto = new QLineEdit;

    QLabel *labelCorreo = new QLabel("Correo electrónico:");
    inputCorreo = new QLineEdit;

    QLabel *labelContrasena = new QLabel("Contraseña:");
    inputContrasena = new QLineEdit;
    inputContrasena->setEchoMode(QLineEdit::Password);

    QLabel *labelConfirmacion = new QLabel("Confirmar contraseña:");
    inputConfirmacion = new QLineEdit;
    inputConfirmacion->setEchoMode(QLineEdit::Password);

    QLabel *labelEdad = new QLabel("Edad:");
    inputEdad = new QSpinBox;
    inputEdad->setMinimum(0);
    inputEdad->setMaximum(120);

    QLabel *labelAvatarText = new QLabel("Avatar:");
    labelAvatar = new QLabel("Ninguno seleccionado");
    botonSeleccionarAvatar = new QPushButton("Seleccionar Imagen");
    connect(botonSeleccionarAvatar, &QPushButton::clicked, this, &SigninWindow::seleccionarImagen);

    QHBoxLayout *avatarLayout = new QHBoxLayout;
    avatarLayout->addWidget(labelAvatar);
    avatarLayout->addWidget(botonSeleccionarAvatar);

    QLabel *labelPregunta = new QLabel("Pregunta de seguridad:");
    comboPreguntas = new QComboBox;
    comboPreguntas->addItem("¿Nombre de tu primera mascota?");
    comboPreguntas->addItem("¿Ciudad de nacimiento?");
    comboPreguntas->addItem("¿Comida favorita?");
    comboPreguntas->addItem("¿Nombre de tu primer Escuela?");

    QLabel *labelRespuesta = new QLabel("Respuesta:");
    inputRespuesta = new QLineEdit;

    QPushButton *botonRegistrar = new QPushButton("Registrar");
    QPushButton *botonCancelar = new QPushButton("");
    botonCancelar->setIcon(QIcon("Imagenes/salida.png"));

    //Fuentes
    QFont font("Arial", 10);
    labelUsername->setFont(font);
    inputUsername->setFont(font);
    labelNombreCompleto->setFont(font);
    inputNombreCompleto->setFont(font);
    labelCorreo->setFont(font);
    inputCorreo->setFont(font);
    labelContrasena->setFont(font);
    inputContrasena->setFont(font);
    labelConfirmacion->setFont(font);
    inputConfirmacion->setFont(font);
    labelEdad->setFont(font);
    inputEdad->setFont(font);
    labelAvatarText->setFont(font);
    labelAvatar->setFont(font);
    botonSeleccionarAvatar->setFont(font);
    labelPregunta->setFont(font);
    comboPreguntas->setFont(font);
    labelRespuesta->setFont(font);
    inputRespuesta->setFont(font);
    botonRegistrar->setFont(font);
    botonCancelar->setFont(font);

    // Conexiones
    connect(botonRegistrar, &QPushButton::clicked, this, &SigninWindow::registrarUsuario);
    connect(botonCancelar, &QPushButton::clicked, this, [=]() {
        this->close();
    });

    // FRAME izquierda Formulario
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->setSpacing(8);

    formLayout->addSpacing(15);

    formLayout->addWidget(labelUsername);
    formLayout->addWidget(inputUsername);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelNombreCompleto);
    formLayout->addWidget(inputNombreCompleto);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelCorreo);
    formLayout->addWidget(inputCorreo);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelContrasena);
    formLayout->addWidget(inputContrasena);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelConfirmacion);
    formLayout->addWidget(inputConfirmacion);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelEdad);
    formLayout->addWidget(inputEdad);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelAvatarText);
    formLayout->addLayout(avatarLayout);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelPregunta);
    formLayout->addWidget(comboPreguntas);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelRespuesta);
    formLayout->addWidget(inputRespuesta);
    formLayout->addSpacing(20);

    // Botones
    QHBoxLayout *botonesLayout = new QHBoxLayout;
    botonesLayout->addStretch();
    botonesLayout->addWidget(botonRegistrar);
    botonesLayout->addSpacing(15);
    botonesLayout->addWidget(botonCancelar);
    botonesLayout->addStretch();

    formLayout->addLayout(botonesLayout);
    formLayout->addSpacing(15);

    QWidget *formWidget = new QWidget;
    formWidget->setLayout(formLayout);
    formWidget->setFixedWidth(350);

    // Layout central (Frames Form Celeste)
    QHBoxLayout *centerLayout = new QHBoxLayout;
    centerLayout->addWidget(formWidget);
    centerLayout->addWidget(rightFrame);
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(centerLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setWindowTitle("Crear nueva cuenta");
    resize(600, 650);
}

void SigninWindow::seleccionarImagen()
{
    QString ruta = QFileDialog::getOpenFileName(this, "Seleccionar Avatar", "", "Imágenes (*.png *.jpg *.bmp)");
    if (!ruta.isEmpty()) {
        rutaImagenSeleccionada = ruta;
        labelAvatar->setText(QFileInfo(ruta).fileName());
    }
}

void SigninWindow::registrarUsuario()
{
    QString usuario = inputUsername->text().trimmed();
    QString nombre = inputNombreCompleto->text().trimmed();
    QString correo = inputCorreo->text().trimmed();
    QString pass = inputContrasena->text().trimmed();
    QString confirm = inputConfirmacion->text().trimmed();
    int edad = inputEdad->value();
    QString pregunta = comboPreguntas->currentText();
    QString imagen = rutaImagenSeleccionada.isEmpty() ? "default_avatar.png" : rutaImagenSeleccionada;
    QString respuesta = inputRespuesta->text().trimmed();
    int IsOn =1;

    // Campos Vacios
    if (usuario.isEmpty() || nombre.isEmpty() || correo.isEmpty() ||
        pass.isEmpty() || confirm.isEmpty() || respuesta.isEmpty()) {
        QMessageBox::warning(this, "Registro", "Por favor, complete todos los campos.");
        return;
    }

    // Formato Correo
    if (!correo.contains("@") || !correo.contains(".")) {
        QMessageBox::warning(this, "Correo inválido", "Ingrese un correo electrónico válido.");
        return;
    }

    // Contrasena Segura
    if (pass != confirm) {
        QMessageBox::warning(this, "Error", "Las contraseñas no coinciden.");
        return;
    }

    if (pass.length() < 6) {
        QMessageBox::warning(this, "Contraseña débil", "La contraseña debe tener al menos 6 caracteres.");
        return;
    }

    bool tieneLetra = false;
    bool tieneNumero = false;
    bool tieneMayuscula = false;
    bool tieneMinuscula = false;

    for (const QChar &c : pass) {
        if (c.isLetter()) {
            tieneLetra = true;
            if (c.isUpper()) tieneMayuscula = true;
            else tieneMinuscula = true;
        }
        else if (c.isDigit()) tieneNumero = true;
    }

    if (!tieneLetra || !tieneNumero) {
        QMessageBox::warning(this, "Contraseña débil", "La contraseña debe contener letras y números.");
        return;
    }

    if (!tieneMayuscula || !tieneMinuscula) {
        QMessageBox::warning(this, "Contraseña débil", "La contraseña debe contener mayúsculas y minúsculas.");
        return;
    }

    // Validar unicidad del usuario
    if (!Usuario::validarUsername(usuario)) {
        QMessageBox::warning(this, "Error", "El nombre de usuario ya está registrado.");
        return;
    }

    try {
        // Usar Master para el registro
        master->signIn(
            usuario,
            nombre,
            correo,
            pass,
            edad,
            imagen,
            pregunta,
            respuesta
            );

        // Verificar creación de archivos
        QFileInfo checkAllaccs("allaccs.txt");
        QFileInfo checkEstado(QString("Estados/%1.txt").arg(usuario));

        if(!checkAllaccs.exists() || !checkEstado.exists()) {
            throw std::runtime_error("Error en creación de archivos");
        }

        // Pasar Master a Chat
        Chat *ventanaChat = new Chat(master);
        ventanaChat->show();
        this->close();

        QMessageBox::information(this, "Éxito", "Registro completado correctamente");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Error de registro: %1").arg(e.what()));
        qDebug() << "Error durante registro:" << e.what();
    }
}

void SigninWindow::closeEvent(QCloseEvent *event)
{
   // loginVentana->show();
    //QWidget::closeEvent(event);
}

