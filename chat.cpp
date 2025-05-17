#include "Chat.h"
#include "amigosmanejo.h"
#include "login.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QPixmap>
#include <QDebug>

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
    // ========== PANEL DE PERFIL ========== //
    QVBoxLayout *layoutPerfil = new QVBoxLayout;
    Usuario* actual = master->getUsuarioActual();

    lblAvatar = new QLabel;
    lblAvatar->setFixedSize(100, 100);
    lblAvatar->setScaledContents(true);
    lblAvatar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lblAvatar->setPixmap(QPixmap(actual->getAvatar()));
    qDebug() << actual->getAvatar();
    qDebug() << master->getAvatar();

    QString ruta = actual->getAvatar();
    qDebug() << "Ruta:" << ruta;
    qDebug() << "¿Existe el archivo?" << QFile::exists(ruta); //No Existe mis fotos, por el problema del Backup

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

    // ========== PANEL DE OPCIONES (BOTONES) ========== //
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

    // ========== AGRUPAR PERFIL + BOTONES ========== //
    QVBoxLayout *panelIzquierdo = new QVBoxLayout;
    panelIzquierdo->addLayout(layoutPerfil);
    panelIzquierdo->addLayout(layoutBotones);
    panelIzquierdo->addStretch();

    //Abrir Frame Gestion de Amigos
    connect(btnAgregar, &QPushButton::clicked, this, [this]() {
        AmigosManejo* ventanaAmigos = new AmigosManejo(master, this);
        ventanaAmigos->setAttribute(Qt::WA_DeleteOnClose);
        ventanaAmigos->setWindowTitle("Gestión de Amigos");
        ventanaAmigos->resize(400, 400);
        ventanaAmigos->show();
    });

    // Conexión para cerrar sesión
    connect(btnCerrarSesion, &QPushButton::clicked, this, [this]() {
        LoginLogout();
    });

    // ========== PANEL CENTRAL - Lista de contactos ========== //
    listaContactos = new QListWidget;
    connect(listaContactos, &QListWidget::itemClicked, this, &Chat::seleccionarContacto);

    // ========== PANEL DERECHO - Chat ========== //
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

    // ========== LAYOUT PRINCIPAL ========== //
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(panelIzquierdo, 2);
    mainLayout->addWidget(listaContactos, 2);
    mainLayout->addLayout(layoutChat, 4);

    setLayout(mainLayout);
    setWindowTitle("Chat - Estructura de Datos 1");
    resize(900, 600);
}

void Chat::mostrarContactosConAvatares() {
    listaContactos->clear();  // Limpiar antes de agregar

    QList<Usuario*> amigos = master->cargarAmigos();

    for (Usuario* amigo : amigos) {
        // Crear widget para mostrar en el QListWidget
        QWidget* itemWidget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Imagen de perfil
        QLabel* avatarLabel = new QLabel;
        avatarLabel->setFixedSize(40, 40);
        avatarLabel->setPixmap(QPixmap(amigo->getAvatar()).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        avatarLabel->setScaledContents(true);

        // Información textual
        QVBoxLayout* infoLayout = new QVBoxLayout;
        QLabel* lblUsername = new QLabel("@" + amigo->getUsername());
        QLabel* lblEstado = new QLabel(amigo->isOnline() ? "Online" : "Offline");
        lblEstado->setStyleSheet(amigo->isOnline() ? "color: green;" : "color: red;");

        infoLayout->addWidget(lblUsername);
        infoLayout->addWidget(lblEstado);

        layout->addWidget(avatarLabel);
        layout->addLayout(infoLayout);

        // Crear QListWidgetItem para contener el widget
        QListWidgetItem* item = new QListWidgetItem(listaContactos);
        item->setSizeHint(itemWidget->sizeHint());

        // Guardar el username en el item (para obtenerlo al seleccionar)
        item->setData(Qt::UserRole, amigo->getUsername());

        listaContactos->addItem(item);
        listaContactos->setItemWidget(item, itemWidget);
    }
}

void Chat::LoginLogout() {
    master->logout();
    Login *ventanaLogin = new Login();
    ventanaLogin->show();
    this->close();
}

void Chat::agregarContacto(const QString &nombre, const QString &estado) {
    QListWidgetItem *item = new QListWidgetItem(nombre + " (" + estado + ")");
    listaContactos->addItem(item);
}

void Chat::seleccionarContacto(QListWidgetItem *item) {
    contactoActual = item->data(Qt::UserRole).toString();
    lblNombreContacto->setText("Chat con: @" + contactoActual);

    QString user1 = master->getUsername();
    QString user2 = contactoActual;

    QString archivo1 = "Chats/"+user1 + "-" + user2 + ".txt";
    QString archivo2 = "Chats/"+user2 + "-" + user1 + ".txt";

    if (QFile::exists(archivo1)) {
        rutaArchivoChatActual = archivo1;
    } else if (QFile::exists(archivo2)) {
        rutaArchivoChatActual = archivo2;
    } else {
        // Si ninguno existe, creamos uno nuevo
        rutaArchivoChatActual = archivo1;
        QFile nuevoArchivo(rutaArchivoChatActual);
        nuevoArchivo.open(QIODevice::WriteOnly);  // lo crea vacío
        nuevoArchivo.close();
    }

    // Leer el contenido del archivo
    QFile archivo(rutaArchivoChatActual);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        chatArea->clear();
        while (!in.atEnd()) {
            QString linea = in.readLine();
            chatArea->append(linea);
        }
        archivo.close();
    }
}


void Chat::enviarMensaje() {
    QString mensaje = inputMensaje->text().trimmed();
    if (!mensaje.isEmpty() && !rutaArchivoChatActual.isEmpty()) {
        QString linea = master->getUsername() + ": " + mensaje;

        // Escribir en archivo
        QFile archivo(rutaArchivoChatActual);
        if (archivo.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << linea << "\n";
            archivo.close();
        }

        pilaMensajes.push(mensaje);
        inputMensaje->clear();

        // Refrescar chatArea
        QFile archivoLectura(rutaArchivoChatActual);
        if (archivoLectura.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivoLectura);
            chatArea->clear();
            while (!in.atEnd()) {
                chatArea->append(in.readLine());
            }
            archivoLectura.close();
        }
    }
}

void Chat::deshacerMensaje() {
    if (!pilaMensajes.isEmpty()) {
        QString ultimo = pilaMensajes.pop();
        chatArea->append("[Mensaje eliminado: \"" + ultimo + "\"]");
    }
}
