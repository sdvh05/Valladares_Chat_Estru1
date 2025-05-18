#include "chat.h"
#include "amigosmanejo.h"
#include "login.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QPixmap>
#include <QDebug>
#include <QScrollBar>

Chat::Chat(Master* master, QWidget *parent)
    : QWidget(parent), master(master) {
    crearLayout();
    setWindowTitle("Chat - " + master->getUsername());
    master->cargarAmigos();
    mostrarContactosConAvatares();
}

void Chat::setLoginVentana(QWidget *ventana) {
    loginVentana = ventana;
}

void Chat::crearLayout() {
    // PANEL DE PERFIL
    QVBoxLayout *layoutPerfil = new QVBoxLayout;
    Usuario* actual = master->getUsuarioActual();

    lblAvatar = new QLabel;
    lblAvatar->setFixedSize(100, 100);
    lblAvatar->setScaledContents(true);
    lblAvatar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lblAvatar->setPixmap(QPixmap(actual->getAvatar()));

    lblNombre = new QLabel(actual->getNombreCompleto());
    lblNombre->setAlignment(Qt::AlignCenter);
    lblNombre->setStyleSheet("font-weight: bold; font-size: 14px;");

    lblUser = new QLabel("@" + actual->getUsername());
    lblUser->setAlignment(Qt::AlignCenter);
    lblUser->setStyleSheet("color: gray;");

    QHBoxLayout* avatarLayout = new QHBoxLayout;
    avatarLayout->addStretch();
    avatarLayout->addWidget(lblAvatar);
    avatarLayout->addStretch();
    layoutPerfil->addLayout(avatarLayout);

    layoutPerfil->addWidget(lblNombre);
    layoutPerfil->addWidget(lblUser);
    layoutPerfil->addSpacing(10);

    // BOTONES
    QVBoxLayout *layoutBotones = new QVBoxLayout;
    lblTituloConfig = new QLabel("Opciones");
    btnAgregar = new QPushButton("Agregar Contacto");
    btnEliminar = new QPushButton("Eliminar Contacto");
    btnConfig = new QPushButton("Configuración");
    btnStickers = new QPushButton("Stickers");
    btnCerrarSesion = new QPushButton("Cerrar Sesión");

    layoutBotones->addWidget(lblTituloConfig);
    layoutBotones->addWidget(btnAgregar);
    layoutBotones->addWidget(btnEliminar);
    layoutBotones->addWidget(btnStickers);
    layoutBotones->addWidget(btnConfig);
    layoutBotones->addWidget(btnCerrarSesion);
    layoutBotones->addStretch();

    // PANEL IZQUIERDO
    QVBoxLayout *panelIzquierdo = new QVBoxLayout;
    panelIzquierdo->addLayout(layoutPerfil);
    panelIzquierdo->addLayout(layoutBotones);
    panelIzquierdo->addStretch();

    connect(btnAgregar, &QPushButton::clicked, this, [this]() {
        AmigosManejo* ventanaAmigos = new AmigosManejo(master, this);
        ventanaAmigos->setAttribute(Qt::WA_DeleteOnClose);
        ventanaAmigos->setWindowTitle("Gestión de Amigos");
        ventanaAmigos->resize(400, 400);
        ventanaAmigos->show();
    });

    connect(btnCerrarSesion, &QPushButton::clicked, this, [this]() {
        LoginLogout();
    });

    connect(btnEliminar, &QPushButton::clicked, this, [this]() {
        Eliminar(listaContactos->currentItem());
    });
    connect(btnStickers, &QPushButton::clicked, this, &Chat::mostrarStickersPopup);


    // LISTA DE CONTACTOS
    listaContactos = new QListWidget;
    connect(listaContactos, &QListWidget::itemClicked, this, &Chat::seleccionarContacto);

    // PANEL DE CHAT
    QVBoxLayout *layoutChat = new QVBoxLayout;
    lblNombreContacto = new QLabel("Selecciona un contacto");

    chatScrollArea = new QScrollArea;
    chatContainer = new QWidget;
    chatLayout = new QVBoxLayout(chatContainer);
    chatLayout->setAlignment(Qt::AlignTop);
    chatScrollArea->setWidgetResizable(true);
    chatScrollArea->setWidget(chatContainer);

    inputMensaje = new QLineEdit;
    btnEnviar = new QPushButton("Enviar");
    btnDeshacer = new QPushButton("Deshacer");

    connect(btnEnviar, &QPushButton::clicked, this, &Chat::enviarMensaje);
    connect(btnDeshacer, &QPushButton::clicked, this, &Chat::deshacerMensaje);

    layoutChat->addWidget(lblNombreContacto);
    layoutChat->addWidget(chatScrollArea);
    layoutChat->addWidget(inputMensaje);
    layoutChat->addWidget(btnEnviar);
    layoutChat->addWidget(btnDeshacer);

    // LAYOUT PRINCIPAL
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(panelIzquierdo, 2);
    mainLayout->addWidget(listaContactos, 2);
    mainLayout->addLayout(layoutChat, 4);

    setLayout(mainLayout);
    setWindowTitle("Chat - Estructura de Datos 1");
    resize(900, 600);
}

//--------------------------------------------------------------------------------------------
void Chat::mostrarContactosConAvatares() {
    listaContactos->clear();

    QList<Usuario*> amigos = master->cargarAmigos();

    for (Usuario* amigo : amigos) {
        QWidget* itemWidget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        QLabel* avatarLabel = new QLabel;
        avatarLabel->setFixedSize(40, 40);
        avatarLabel->setPixmap(QPixmap(amigo->getAvatar()).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        avatarLabel->setScaledContents(true);

        QVBoxLayout* infoLayout = new QVBoxLayout;
        QLabel* lblUsername = new QLabel("@" + amigo->getUsername());
        QLabel* lblEstado = new QLabel(amigo->isOnline() ? "Online" : "Offline");
        lblEstado->setStyleSheet(amigo->isOnline() ? "color: green;" : "color: red;");

        infoLayout->addWidget(lblUsername);
        infoLayout->addWidget(lblEstado);

        layout->addWidget(avatarLabel);
        layout->addLayout(infoLayout);

        QListWidgetItem* item = new QListWidgetItem(listaContactos);
        item->setSizeHint(itemWidget->sizeHint());
        item->setData(Qt::UserRole, amigo->getUsername());

        listaContactos->addItem(item);
        listaContactos->setItemWidget(item, itemWidget);
    }
}

void Chat::mostrarStickersPopup() {
    if (contactoActual.isEmpty()) {
        QMessageBox::warning(this, "Selecciona un contacto", "Debes seleccionar un contacto antes de enviar un sticker.");
        return;
    }

    QDialog* dialogo = new QDialog(this);
    dialogo->setWindowTitle("Selecciona un sticker");
    dialogo->setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(dialogo);
    QGridLayout* grid = new QGridLayout;

    QStringList rutas = {
        "Stickers/StitchGum.jpeg",
        "Stickers/CoffeeCode.png",
        "Stickers/buen-trabajo.png",
        "Stickers/logro.png"
    };

    for (int i = 0; i < rutas.size(); ++i) {
        QString ruta = rutas[i];
        QPushButton* botonSticker = new QPushButton;
        botonSticker->setIcon(QIcon(ruta));
        botonSticker->setIconSize(QSize(100, 100));
        botonSticker->setFixedSize(110, 110);
        botonSticker->setStyleSheet("border: none;");

        connect(botonSticker, &QPushButton::clicked, this, [this, ruta, dialogo]() {
            QString linea = master->getUsername() + ": STICKER," + ruta;

            // Guardar en el archivo del chat
            QFile archivo(rutaArchivoChatActual);
            if (archivo.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&archivo);
                out << linea << "\n";
                archivo.close();
            }

            // Recargar chat para mostrarlo actualizado
            seleccionarContacto(listaContactos->currentItem());

            dialogo->accept();
        });

        grid->addWidget(botonSticker, i / 2, i % 2);  // 2 columnas
    }

    layout->addLayout(grid);
    dialogo->setLayout(layout);
    dialogo->exec();
}

//--------------------------------------------------------------------------------------------
void Chat::LoginLogout() {
    QMessageBox::StandardButton confirmacion;
    confirmacion = QMessageBox::question(this, "Cerrar sesión", "¿Estás seguro de que quieres cerrar sesión?", QMessageBox::Yes | QMessageBox::No);

    if (confirmacion == QMessageBox::Yes) {
        master->logout();
        Login *ventanaLogin = new Login();
        ventanaLogin->show();
        this->close();
    }
}

void Chat::Eliminar(QListWidgetItem *item) {
    if (!item) {
        QMessageBox::warning(this, "Error", "Selecciona un contacto para eliminar.");
        return;
    }

    QString usernameEliminar = item->data(Qt::UserRole).toString();

    QMessageBox::StandardButton confirmacion = QMessageBox::question(
        this,
        "Eliminar contacto",
        "¿Estás seguro de eliminar a @" + usernameEliminar + " de tus contactos?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirmacion == QMessageBox::Yes) {
        Usuario* usuarioActual = master->getUsuarioActual();
        usuarioActual->EliminarAmigo(usernameEliminar);
        delete listaContactos->takeItem(listaContactos->row(item));
        lblNombreContacto->setText("Selecciona un contacto");
    }
}

void Chat::agregarMensajeWidget(const QString& mensaje, bool esPropio) {
    QWidget* mensajeWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(mensajeWidget);

    QLabel* label = new QLabel;
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Detectar si es un sticker
    if (mensaje.startsWith("STICKER,")) {
        QString rutaSticker = mensaje.mid(QString("STICKER,").length());
        QPixmap pix(rutaSticker);
        if (!pix.isNull()) {
            label->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            label->setFixedSize(130, 130);
        } else {
            label->setText("[Sticker no encontrado]");
        }
    } else {
        // Mensaje de texto normal
        label->setText(mensaje);
        label->setStyleSheet(QString(
                                 "background-color: %1; color: %2; padding: 8px; border-radius: 10px;"
                                 ).arg(
                                     esPropio ? "#cce5ff" : "#e0e0e0"  // Fondo azul/gris
                                     ).arg(
                                     "#000000"  // Texto negro
                                     ));
    }

    if (esPropio) {
        layout->addStretch();
        layout->addWidget(label);
    } else {
        layout->addWidget(label);
        layout->addStretch();
    }

    layout->setContentsMargins(10, 5, 10, 5);
    chatLayout->addWidget(mensajeWidget);

    // Desplazar hacia abajo automáticamente
    QTimer::singleShot(0, [this]() {
        chatScrollArea->verticalScrollBar()->setValue(chatScrollArea->verticalScrollBar()->maximum());
    });
}


void Chat::seleccionarContacto(QListWidgetItem *item) {
    contactoActual = item->data(Qt::UserRole).toString();
    lblNombreContacto->setText("Chat con: @" + contactoActual);

    QString user1 = master->getUsername();
    QString user2 = contactoActual;

    QString archivo1 = "Chats/" + user1 + "-" + user2 + ".txt";
    QString archivo2 = "Chats/" + user2 + "-" + user1 + ".txt";

    if (QFile::exists(archivo1)) {
        rutaArchivoChatActual = archivo1;
    } else if (QFile::exists(archivo2)) {
        rutaArchivoChatActual = archivo2;
    } else {
        rutaArchivoChatActual = archivo1;
        QFile nuevoArchivo(rutaArchivoChatActual);
        nuevoArchivo.open(QIODevice::WriteOnly);
        nuevoArchivo.close();
    }

    QFile archivo(rutaArchivoChatActual);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);

        // Limpiar mensajes anteriores
        QLayoutItem* item;
        while ((item = chatLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(": ");
            if (partes.size() >= 2) {
                QString autor = partes[0];
                QString contenido = partes.mid(1).join(": ");
                bool esPropio = (autor == master->getUsername());
                agregarMensajeWidget(contenido, esPropio);
            } else {
                agregarMensajeWidget(linea, false);
            }
        }
        archivo.close();
    }
}

void Chat::enviarMensaje() {
    QString mensaje = inputMensaje->text().trimmed();
    if (!mensaje.isEmpty() && !rutaArchivoChatActual.isEmpty()) {
        QString linea = master->getUsername() + ": " + mensaje;

        QFile archivo(rutaArchivoChatActual);
        if (archivo.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << linea << "\n";
            archivo.close();
        }

        pilaMensajes.push(mensaje);
        inputMensaje->clear();
        agregarMensajeWidget(mensaje, true);
    }
}

void Chat::deshacerMensaje() {
    if (!pilaMensajes.isEmpty()) {
        QString ultimo = pilaMensajes.pop();
        agregarMensajeWidget("[Mensaje eliminado: \"" + ultimo + "\"]", true);
    }
}
