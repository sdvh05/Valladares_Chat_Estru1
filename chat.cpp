#include "Chat.h"
#include "amigosmanejo.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidgetItem>

Chat::Chat(Master* master, QWidget *parent)
    : QWidget(parent), master(master)  {
    crearLayout();
     setWindowTitle("Chat - " + master->getUsername());
}

void Chat::crearLayout() {
    // Frame izquierdo - Funciones
    QVBoxLayout *layoutBotones = new QVBoxLayout;
    lblTituloConfig = new QLabel("Opciones");
    btnAgregar = new QPushButton("Agregar Contacto");
    btnEliminar = new QPushButton("Eliminar Contacto");
    btnConfig = new QPushButton("Configuración");
    btnStickers = new QPushButton("Stickers");

    layoutBotones->addWidget(lblTituloConfig);
    layoutBotones->addWidget(btnAgregar);
    layoutBotones->addWidget(btnEliminar);
    layoutBotones->addWidget(btnStickers);
    layoutBotones->addWidget(btnConfig);
    layoutBotones->addStretch();



    connect(btnAgregar, &QPushButton::clicked, this, [this]() {
        AmigosManejo* ventanaAmigos = new AmigosManejo(master, this);
        ventanaAmigos->setAttribute(Qt::WA_DeleteOnClose); // se elimina al cerrar
        ventanaAmigos->setWindowTitle("Gestión de Amigos");
        ventanaAmigos->resize(400, 400); // tamaño opcional
        ventanaAmigos->show();
    });

    // Frame medio - Lista de usuarios
    listaContactos = new QListWidget;


    //agregarContacto("eZtIb", "conectado");


    connect(listaContactos, &QListWidget::itemClicked, this, &Chat::seleccionarContacto);

    // Frame derecho - Chat
    QVBoxLayout *layoutChat = new QVBoxLayout;
    lblNombreContacto = new QLabel("Selecciona un contacto");
    chatArea = new QTextEdit;
    chatArea->setReadOnly(true);

    inputMensaje = new QLineEdit;
    btnEnviar = new QPushButton("Enviar");
    btnDeshacer = new QPushButton("Deshacer");

    connect(btnEnviar, &QPushButton::clicked, this, &Chat::enviarMensaje);
    connect(btnDeshacer, &QPushButton::clicked, this, &Chat::deshacerMensaje);

    layoutChat->addWidget(lblNombreContacto);
    layoutChat->addWidget(chatArea);
    layoutChat->addWidget(inputMensaje);
    layoutChat->addWidget(btnEnviar);
    layoutChat->addWidget(btnDeshacer);

    // Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(layoutBotones, 1);
    mainLayout->addWidget(listaContactos, 2);
    mainLayout->addLayout(layoutChat, 4);

    setLayout(mainLayout);
    setWindowTitle("Chat - Estructura de Datos 1");
    resize(900, 600);
}

void Chat::agregarContacto(const QString &nombre, const QString &estado) {
    QListWidgetItem *item = new QListWidgetItem(nombre + " (" + estado + ")");
    listaContactos->addItem(item);
}

void Chat::seleccionarContacto(QListWidgetItem *item) {
    lblNombreContacto->setText(item->text());
    chatArea->append("Abriendo conversación con " + item->text() + "...");
}

void Chat::enviarMensaje() {
    QString mensaje = inputMensaje->text();
    if (!mensaje.isEmpty()) {

        chatArea->append(master->getUsername()+": " + mensaje);

        pilaMensajes.push(mensaje); // Guarda para deshacer
        inputMensaje->clear();
    }
}

void Chat::deshacerMensaje() {
    if (!pilaMensajes.isEmpty()) {
        QString ultimo = pilaMensajes.pop();
        // Elimina visualmente el último mensaje (opcionalmente más complejo con tracking)
        chatArea->append("[Mensaje eliminado: \"" + ultimo + "\"]");
    }
}
