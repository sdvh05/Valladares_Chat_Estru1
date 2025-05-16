#include "amigosmanejo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QScreen>
#include <QGuiApplication>

#include <QDebug>

#include "login.h"

AmigosManejo::AmigosManejo(Master* masterPtr, QWidget* parent)
    : QWidget(parent), master(masterPtr)
{
    // Para que la ventana tenga marco y botones
    setWindowFlags(Qt::Window);
    setWindowTitle("Gestión de Amigos");
    resize(400, 500); // tamaño de la ventana

    setupUI();

    // Mover la ventana a la derecha de la pantalla
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = screenGeometry.right() - width() - 50;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    int Estado;
}

void AmigosManejo::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    menuBar = new QMenuBar(this);
    menuBar->addAction("Añadir Amigos", this, &AmigosManejo::mostrarAnadir);
    menuBar->addAction("Solicitudes Enviadas", this, &AmigosManejo::mostrarEnviadas);
    menuBar->addAction("Solicitudes Recibidas", this, &AmigosManejo::mostrarRecibidas);

    stackedWidget = new QStackedWidget(this);

    // Sección: Añadir Amigos -- Todos los estados 0
    widgetAnadir = new QWidget(this);
    listaAnadir = new QListWidget(widgetAnadir);
    botonEnviarSolicitud = new QPushButton("Enviar Solicitud", widgetAnadir);
    QVBoxLayout* layoutAnadir = new QVBoxLayout(widgetAnadir);
    layoutAnadir->addWidget(listaAnadir);
    layoutAnadir->addWidget(botonEnviarSolicitud);
    stackedWidget->addWidget(widgetAnadir);

    // Sección: Solicitudes Enviadas -- Todos los Estados 1
    widgetEnviadas = new QWidget(this);
    listaEnviadas = new QListWidget(widgetEnviadas);
    QVBoxLayout* layoutEnviadas = new QVBoxLayout(widgetEnviadas);
    layoutEnviadas->addWidget(listaEnviadas);
    stackedWidget->addWidget(widgetEnviadas);

    // Sección: Solicitudes Recibidas -- Todos los Estados 2
    widgetRecibidas = new QWidget(this);
    listaRecibidas = new QListWidget(widgetRecibidas);
    botonAceptar = new QPushButton("Aceptar", widgetRecibidas);
    botonRechazar = new QPushButton("Rechazar", widgetRecibidas);
    QHBoxLayout* botones = new QHBoxLayout();
    botones->addWidget(botonAceptar);
    botones->addWidget(botonRechazar);
    QVBoxLayout* layoutRecibidas = new QVBoxLayout(widgetRecibidas);
    layoutRecibidas->addWidget(listaRecibidas);
    layoutRecibidas->addLayout(botones);
    stackedWidget->addWidget(widgetRecibidas);

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    //Funciones Botones
    connect(botonEnviarSolicitud, &QPushButton::clicked, this, &AmigosManejo::enviarSolicitud);
    connect(botonAceptar, &QPushButton::clicked, this, &AmigosManejo::aceptarSolicitud);
    connect(botonRechazar, &QPushButton::clicked, this, &AmigosManejo::rechazarSolicitud);

}

//Mostrar
//--------------------------------------------------------------------------------
void AmigosManejo::mostrarAnadir()
{
    stackedWidget->setCurrentWidget(widgetAnadir);
    cargarListaAnadir();
}

void AmigosManejo::mostrarEnviadas()
{
    stackedWidget->setCurrentWidget(widgetEnviadas);
    cargarListaEnviadas();
}

void AmigosManejo::mostrarRecibidas()
{
    stackedWidget->setCurrentWidget(widgetRecibidas);
    cargarListaRecibidas();
}

//Cargar
//--------------------------------------------------------------------------------
void AmigosManejo::cargarListaAnadir()
{
    listaAnadir->clear();
    const auto usuarios = master->getUsers0();
    for (Usuario* u : usuarios) {
        if (u) {
            listaAnadir->addItem(u->getUsername());
        }
    }
}

void AmigosManejo::cargarListaEnviadas()
{
    listaEnviadas->clear();
    const auto usuarios = master->getUsers1();
    for (Usuario* u : usuarios) {
        if (u) {
            listaEnviadas->addItem(u->getUsername());
        }
    }
}

void AmigosManejo::cargarListaRecibidas()
{
    listaRecibidas->clear();
    const auto usuarios = master->getUsers2();
    for (Usuario* u : usuarios) {
        if (u) {
            listaRecibidas->addItem(u->getUsername());
        }
    }
}


//Solicitudes
//--------------------------------------------------------------------------------
void AmigosManejo::enviarSolicitud()
{
    QListWidgetItem* item = listaAnadir->currentItem();
    if (!item) return;

    QString receptor = item->text();
    qDebug() << receptor;
    Usuario* actual = master->getUsuarioActual();
    if (!actual) return;

    actual->enviarSolicitud(receptor);
    cargarListaAnadir();
    cargarListaEnviadas();
}

void AmigosManejo::aceptarSolicitud()
{
    QListWidgetItem* item = listaRecibidas->currentItem();
    if (!item) return;

    QString solicitante = item->text();
    Usuario* actual = master->getUsuarioActual();
    if (!actual) return;

    actual->aceptarSolicitud(solicitante);
    cargarListaRecibidas();
}

void AmigosManejo::rechazarSolicitud()
{
    QListWidgetItem* item = listaRecibidas->currentItem();
    if (!item) return;

    QString solicitante = item->text();
    Usuario* actual = master->getUsuarioActual();
    if (!actual) return;

    actual->NegarSolicitud(solicitante);
    cargarListaRecibidas();
}
