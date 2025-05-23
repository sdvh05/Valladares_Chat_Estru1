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
#include <QComboBox>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

// Constructor
Chat::Chat(Master* master, QWidget *parent)
    : QWidget(parent), master(master) {
    crearLayout();
    setWindowTitle("Chat - " + master->getUsername());
    master->cargarAmigos();
    mostrarContactosConAvatares();

    //QTIMER
    QTimer* timerRefresco = new QTimer(this);
    connect(timerRefresco, &QTimer::timeout, this, &Chat::refrescarPeriodicamente);
    timerRefresco->start(1000); // cada 1 segundo

}

void Chat::setLoginVentana(QWidget *ventana) {
    loginVentana = ventana;
}

// Crea todo el layout de la ventana principal
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

    connect(btnCerrarSesion, &QPushButton::clicked, this, &Chat::LoginLogout);
    connect(btnEliminar, &QPushButton::clicked, this, [this]() {
        Eliminar(listaContactos->currentItem());
    });
    connect(btnStickers, &QPushButton::clicked, this, &Chat::mostrarStickersPopup);

    // COMBOBOX + LISTA DE CONTACTOS (panel central)
    QVBoxLayout* layoutCentral = new QVBoxLayout;
    comboOrdenamiento = new QComboBox;
    comboOrdenamiento->addItem("Default");
    comboOrdenamiento->addItem("A-Z");
    comboOrdenamiento->addItem("Chat Length");

    connect(comboOrdenamiento, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Chat::ordenarContactos);

    listaContactos = new QListWidget;
    connect(listaContactos, &QListWidget::itemClicked, this, &Chat::seleccionarContacto);

    layoutCentral->addWidget(comboOrdenamiento);
    layoutCentral->addWidget(listaContactos);

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
    connect(btnConfig, &QPushButton::clicked, this, &Chat::mostrarOpcionesConfiguracion);


    layoutChat->addWidget(lblNombreContacto);
    layoutChat->addWidget(chatScrollArea);
    layoutChat->addWidget(inputMensaje);
    layoutChat->addWidget(btnEnviar);
    layoutChat->addWidget(btnDeshacer);

    // LAYOUT PRINCIPAL
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(panelIzquierdo, 2);
    mainLayout->addLayout(layoutCentral, 2);
    mainLayout->addLayout(layoutChat, 4);

    setLayout(mainLayout);
    setWindowTitle("Chat - Estructura de Datos 1");
    resize(900, 600);
}


// ---------------- ORDENAMIENTO ---------------------

void Chat::ordenarContactos(int index) {
    QList<Usuario*> amigos;

    switch (index) {
    case 0:
        amigos = master->cargarAmigos(); break;
    case 1:
        amigos = master->CargarAmigosAlfabetico(); break;
    case 2:
        amigos = master->CargarAmigosLength(); break;
    default:
        amigos = master->cargarAmigos(); break;
    }

    mostrarContactosConAvatares(amigos);
}

void Chat::mostrarContactosConAvatares() {
    QList<Usuario*> amigos = master->cargarAmigos();
    mostrarContactosConAvatares(amigos);
}

void Chat::mostrarContactosConAvatares(const QList<Usuario*>& amigos) {
    listaContactos->clear();

    // Obtener las notificaciones pendientes
    QMap<QString, int> conteoNotificaciones;
    QList<QString> notificaciones = master->cargarNotificaciones();

    for (const QString& usuario : notificaciones) {
        conteoNotificaciones[usuario] = obtenerCantidadNotis(usuario);
    }

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

        // Si hay notificaciones para este usuario, añadir un contador visual
        QString usernameAmigo = amigo->getUsername();
        if (conteoNotificaciones.contains(usernameAmigo)) {
            int cantidad = conteoNotificaciones[usernameAmigo];

            QLabel* lblNotificacion = new QLabel(QString::number(cantidad));
            lblNotificacion->setFixedSize(24, 24);
            lblNotificacion->setAlignment(Qt::AlignCenter);
            lblNotificacion->setStyleSheet(
                "background-color: blue; color: white; border-radius: 12px; font-weight: bold;"
                );

            layout->addWidget(lblNotificacion);
        }

        QListWidgetItem* item = new QListWidgetItem(listaContactos);
        item->setSizeHint(itemWidget->sizeHint());
        item->setData(Qt::UserRole, amigo->getUsername());

        listaContactos->addItem(item);
        listaContactos->setItemWidget(item, itemWidget);
    }
}


// -------------------------------------

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
            QFile archivo(rutaArchivoChatActual);
            if (archivo.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&archivo);
                out << linea << "\n";
                archivo.close();
            }
            seleccionarContacto(listaContactos->currentItem());
            dialogo->accept();
        });

        grid->addWidget(botonSticker, i / 2, i % 2);
    }

    master->enviarNotificacion(contactoActual);
    layout->addLayout(grid);
    dialogo->setLayout(layout);
    dialogo->exec();
}

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
        label->setText(mensaje);

        QString colorFondo = esPropio ? "#cce5ff" : "#e0e0e0";
        QString colorTexto = "#000000";
        QString estilo = "padding: 8px; border-radius: 10px;";

        // Verificar si es un mensaje eliminado
        if (mensaje.trimmed() == "🗑️ Eliminaste este Mensaje") {
            estilo += " font-style: italic;";
        }

        label->setStyleSheet(QString("background-color: %1; color: %2; %3")
                                 .arg(colorFondo).arg(colorTexto).arg(estilo));
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

    QTimer::singleShot(0, [this]() {
        chatScrollArea->verticalScrollBar()->setValue(chatScrollArea->verticalScrollBar()->maximum());
    });
}


void Chat::seleccionarContacto(QListWidgetItem *item) {
    if (!item) return;

    contactoActual = item->data(Qt::UserRole).toString();
    lblNombreContacto->setText("Chat con: @" + contactoActual);

    QString user1 = master->getUsername();
    QString user2 = contactoActual;

    QString archivo1 = "Chats/" + user1 + "-" + user2 + ".txt";
    QString archivo2 = "Chats/" + user2 + "-" + user1 + ".txt";

    // Verifica cuál de los dos archivos existe
    if (QFile::exists(archivo1)) {
        rutaArchivoChatActual = archivo1;
    } else if (QFile::exists(archivo2)) {
        rutaArchivoChatActual = archivo2;
    } else {
        // Si ninguno existe, crea uno nuevo usando el orden user1-user2
        rutaArchivoChatActual = archivo1;
        QFile nuevoArchivo(rutaArchivoChatActual);
        nuevoArchivo.open(QIODevice::WriteOnly);
        nuevoArchivo.close();
    }

    // Limpia las notificaciones pendientes de este contacto
    master->leerNotificacion(contactoActual);

    // Limpia el layout del chat antes de cargar los nuevos mensajes
    QLayoutItem* itemLayout;
    while ((itemLayout = chatLayout->takeAt(0)) != nullptr) {
        if (itemLayout->widget()) itemLayout->widget()->deleteLater();
        delete itemLayout;
    }

    // Carga los mensajes desde el archivo correspondiente
    QFile archivo(rutaArchivoChatActual);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(": ");
            if (partes.size() >= 2) {
                QString autor = partes[0];
                QString contenido = partes.mid(1).join(": ");
                bool esPropio = (autor == user1);
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
        master->enviarNotificacion(contactoActual);
    }
}

void Chat::deshacerMensaje() {
    if (rutaArchivoChatActual.isEmpty()) return;

    QFile archivo(rutaArchivoChatActual);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&archivo);
    QStringList lineas;
    while (!in.atEnd()) {
        lineas << in.readLine();
    }
    archivo.close();

    QString usuario = master->getUsername();
    QString marcadorEliminado = usuario + ": 🗑️ Eliminaste este Mensaje";

    int indexAEliminar = -1;
    QString contenidoOriginal;

    for (int i = lineas.size() - 1; i >= 0; --i) {
        QString linea = lineas[i];

        if (linea.startsWith(usuario + ":")) {
            if (linea.trimmed() == marcadorEliminado) {
                continue;
            } else {
                indexAEliminar = i;
                contenidoOriginal = linea.section(": ", 1);
                break;
            }
        }
    }

    if (indexAEliminar != -1) {
        QMessageBox::StandardButton respuesta = QMessageBox::question(
            this,
            "Deshacer último mensaje",
            "¿Estás seguro que quieres eliminar tu último mensaje?\n\n\"" + contenidoOriginal + "\"",
            QMessageBox::Yes | QMessageBox::No
            );

        if (respuesta == QMessageBox::Yes) {
            // Reemplazar línea
            lineas[indexAEliminar] = marcadorEliminado;

            if (archivo.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                QTextStream out(&archivo);
                for (const QString& linea : lineas) {
                    out << linea << "\n";
                }
                archivo.close();
                seleccionarContacto(listaContactos->currentItem()); // Recargar chat visualmente
            }
        }
    } else {
        QMessageBox::information(this, "Deshacer", "No hay mensajes recientes tuyos para eliminar.");
    }
}



int Chat::obtenerCantidadNotis(const QString& usuario) {
    QString ruta = QString("Notificacion/%1.txt").arg(master->getUsername());
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        QStringList partes = linea.split(',');

        if (partes.size() == 2 && partes[0].trimmed() == usuario) {
            return partes[1].trimmed().toInt();
        }
    }

    return 0;
}

void Chat::refrescarPeriodicamente() {
    QString contactoAnterior = contactoActual;


    int index=comboOrdenamiento->currentIndex();
    ordenarContactos(index);



    for (int i = 0; i < listaContactos->count(); ++i) {
        QListWidgetItem* item = listaContactos->item(i);
        if (item->data(Qt::UserRole).toString() == contactoAnterior) {
            listaContactos->setCurrentItem(item);
            seleccionarContacto(item);
            break;
        }
    }
}

void Chat::mostrarOpcionesConfiguracion() {
    QStringList opciones = {"Cambiar Nombre", "Cambiar Edad", "Cambiar Email", "Cambiar Avatar"};
    bool ok;
    QString opcionSeleccionada = QInputDialog::getItem(this, "Configuración de Usuario","Selecciona una opción:", opciones, 0, false, &ok);
    if (!ok || opcionSeleccionada.isEmpty()) return;

    Usuario* actual = master->getUsuarioActual();

    if (opcionSeleccionada == "Cambiar Nombre") {
        QString nuevoNombre = QInputDialog::getText(this, "Cambiar Nombre","Ingresa tu nuevo nombre:", QLineEdit::Normal,actual->getNombreCompleto(), &ok);
        if (ok && !nuevoNombre.trimmed().isEmpty()) {
            auto confirmacion = QMessageBox::question(this, "Confirmar cambio","¿Deseas cambiar tu nombre a:\n\"" + nuevoNombre + "\"?",QMessageBox::Yes | QMessageBox::No);
            if (confirmacion == QMessageBox::Yes) {

                master->cambiarNombre(nuevoNombre.trimmed());
                lblNombre->setText(nuevoNombre.trimmed());
            }
        } else if (ok) {
            QMessageBox::warning(this, "Error", "El nombre no puede estar vacío.");
        }

    } else if (opcionSeleccionada == "Cambiar Edad") {
        QString nuevaEdadStr = QInputDialog::getText(this, "Cambiar Edad","Ingresa tu nueva edad:", QLineEdit::Normal,QString::number(actual->getEdad()), &ok);
        if (ok && !nuevaEdadStr.trimmed().isEmpty()) {
            bool esNumero;
            int nuevaEdad = nuevaEdadStr.toInt(&esNumero);
            if (!esNumero || nuevaEdad <= 0) {
                QMessageBox::warning(this, "Error", "La edad debe ser un número válido.");
                return;
            }

            auto confirmacion = QMessageBox::question(this, "Confirmar cambio","¿Deseas cambiar tu edad a:\n" + QString::number(nuevaEdad) + "?",QMessageBox::Yes | QMessageBox::No);
            if (confirmacion == QMessageBox::Yes) {
                master->cambiarEdad(nuevaEdad);
            }
        } else if (ok) {
            QMessageBox::warning(this, "Error", "La edad no puede estar vacía.");
        }

    } else if (opcionSeleccionada == "Cambiar Email") {
        QString nuevoEmail = QInputDialog::getText(this, "Cambiar Email","Ingresa tu nuevo email:", QLineEdit::Normal,actual->getEmail(), &ok);
        if (ok && !nuevoEmail.trimmed().isEmpty()) {
            if (!nuevoEmail.contains("@") || !nuevoEmail.contains(".")) {
                QMessageBox::warning(this, "Error", "El email ingresado no es válido.");
                return;
            }

            auto confirmacion = QMessageBox::question(this, "Confirmar cambio","¿Deseas cambiar tu email a:\n" + nuevoEmail + "?",QMessageBox::Yes | QMessageBox::No);
            if (confirmacion == QMessageBox::Yes) {
                master->cambiarEmail(nuevoEmail.trimmed());
            }
        } else if (ok) {
            QMessageBox::warning(this, "Error", "El email no puede estar vacío.");
        }

    } else if (opcionSeleccionada == "Cambiar Avatar") {
        QString rutaNueva = QFileDialog::getOpenFileName(this, "Seleccionar nuevo avatar", "", "Imágenes (*.png *.jpg *.jpeg *.bmp)");
        if (!rutaNueva.isEmpty()) {
            auto confirmacion = QMessageBox::question(this, "Confirmar cambio","¿Deseas cambiar tu avatar a esta imagen?\n" + rutaNueva,QMessageBox::Yes | QMessageBox::No);
            if (confirmacion == QMessageBox::Yes) {
                master->cambiarAvatar(rutaNueva);
                lblAvatar->setPixmap(QPixmap(rutaNueva));
            }
        }
    }
}
