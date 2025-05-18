#include "recuperarpass.h"
#include "login.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFont>
#include <QIcon>
#include <QInputDialog>


RecuperarPass::RecuperarPass(QWidget *parent)
    : QWidget(parent)
{
    // Header (barra superior azul)
    QFrame *headerFrame = new QFrame;
    headerFrame->setStyleSheet("background-color: #004080;");
    headerFrame->setFixedHeight(50);

    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    QLabel *labelHeader = new QLabel("Recuperar Contraseña");
    labelHeader->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
    labelHeader->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(labelHeader);
    headerLayout->addStretch();

    // Frame derecho (celeste)
    QFrame *rightFrame = new QFrame;
    rightFrame->setStyleSheet("background-color: #cceeff;");
    rightFrame->setFixedWidth(150);

    QVBoxLayout *rightLayout = new QVBoxLayout(rightFrame);
    rightLayout->addStretch();

    // Controles del formulario
    QLabel *labelUsuario = new QLabel("Usuario:");
    inputUsername = new QLineEdit;

    QLabel *labelPregunta = new QLabel("Pregunta de seguridad:");
    comboPreguntas = new QComboBox;
    comboPreguntas->addItem("¿Nombre de tu primera mascota?");
    comboPreguntas->addItem("¿Ciudad de nacimiento?");
    comboPreguntas->addItem("¿Comida favorita?");
    comboPreguntas->addItem("¿Nombre de tu primer Escuela?");

    QLabel *labelRespuesta = new QLabel("Respuesta:");
    inputRespuesta = new QLineEdit;

    botonCambiar = new QPushButton("Recuperar contraseña");
    botonRegresar = new QPushButton("");
    //botonRegresar = new QPushButton("Regresar");


    //Fuente
    QFont font("Arial", 10);
    labelUsuario->setFont(font);
    inputUsername->setFont(font);
    labelPregunta->setFont(font);
    comboPreguntas->setFont(font);
    labelRespuesta->setFont(font);
    inputRespuesta->setFont(font);
    botonCambiar->setFont(font);
    botonRegresar->setFont(font);

    // Conexiones
    connect(botonCambiar, &QPushButton::clicked, this, &RecuperarPass::verificarRespuesta);
    connect(botonRegresar, &QPushButton::clicked, this, &RecuperarPass::cerrarVentana);

    // Layout del formulario (izquierda) con espaciado consistente
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->setSpacing(8);

    formLayout->addSpacing(15);

    formLayout->addWidget(labelUsuario);
    formLayout->addWidget(inputUsername);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelPregunta);
    formLayout->addWidget(comboPreguntas);
    formLayout->addSpacing(12);

    formLayout->addWidget(labelRespuesta);
    formLayout->addWidget(inputRespuesta);
    formLayout->addSpacing(20);

    formLayout->addWidget(botonCambiar);
    formLayout->addSpacing(10);

    //Boton Regresar
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addStretch();
    hLayout->addWidget(botonRegresar);
    formLayout->addLayout(hLayout);
    botonRegresar->setIcon(QIcon("Imagenes/salida.png"));

    formLayout->addSpacing(15);

    QWidget *formWidget = new QWidget;
    formWidget->setLayout(formLayout);
    formWidget->setFixedWidth(300);

    // Layout central (Form + Celeste)
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

    setWindowTitle("Cambiar contraseña");
    resize(500, 350);  // Tamaño ajustado
}

void RecuperarPass::verificarRespuesta()
{
    QString usuario = inputUsername->text();
    QString preguntaSeleccionada = comboPreguntas->currentText();
    QString respuestaIngresada = inputRespuesta->text();

    if (usuario.isEmpty() || respuestaIngresada.isEmpty()) {
        QMessageBox::warning(this, "Campos vacíos", "Llena todos los campos.");
        return;
    }

    // Solicita nueva contraseña
    bool ok;
    QString nuevaContrasena = QInputDialog::getText(this, "Nueva contraseña",
                                                    "Ingresa tu nueva contraseña:",
                                                    QLineEdit::Password,
                                                    "", &ok);
    if (!ok || nuevaContrasena.isEmpty()) {
        QMessageBox::information(this, "Cancelado", "No se cambió la contraseña.");
        return;
    }

    if (nuevaContrasena.length() < 4) {
        QMessageBox::warning(this, "Contraseña inválida", "La nueva contraseña debe tener al menos 4 caracteres.");
        return;
    }

    if (Master::cambiarContrasena(usuario, preguntaSeleccionada, respuestaIngresada, nuevaContrasena)) {
        QMessageBox::information(this, "Éxito", "Contraseña actualizada correctamente.");
        this->cerrarVentana();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar la contraseña.\nVerifica que la información sea correcta y que la nueva contraseña sea distinta.");
    }
}

void RecuperarPass::cerrarVentana()
{
    Login *ventanaLogin = new Login();
    ventanaLogin->show();
    this->close();
}
