/*#include "FuncionAmigos.h"
#include "Master.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QLineEdit>

FuncionAmigos::FuncionAmigos(Master* master, QWidget* parent)
    : QDialog(parent), master(master) {

    setWindowTitle("Gestión de Amigos");
    resize(600, 400);
    crearUI();
    actualizarListas();
}

void FuncionAmigos::crearUI() {
    tabs = new QTabWidget(this);

    // Pestaña 1: Añadir amigos
    QWidget* tabAnadir = new QWidget;
    inputBusqueda = new QLineEdit;
    inputBusqueda->setPlaceholderText("Buscar usuarios...");
    listaUsuarios = new QListWidget;
    QPushButton* btnActualizar = new QPushButton("Actualizar");

    QVBoxLayout* layoutAnadir = new QVBoxLayout(tabAnadir);
    QHBoxLayout* layoutBusqueda = new QHBoxLayout;
    layoutBusqueda->addWidget(inputBusqueda);
    layoutBusqueda->addWidget(btnActualizar);
    layoutAnadir->addLayout(layoutBusqueda);
    layoutAnadir->addWidget(listaUsuarios);

    // Pestaña 2: Solicitudes
    QWidget* tabSolicitudes = new QWidget;
    listaEnviadas = new QListWidget;
    listaRecibidas = new QListWidget;

    QVBoxLayout* layoutSolicitudes = new QVBoxLayout(tabSolicitudes);
    layoutSolicitudes->addWidget(new QLabel("Solicitudes Enviadas:"));
    layoutSolicitudes->addWidget(listaEnviadas);
    layoutSolicitudes->addWidget(new QLabel("Solicitudes Recibidas:"));
    layoutSolicitudes->addWidget(listaRecibidas);

    tabs->addTab(tabAnadir, "Añadir Amigos");
    tabs->addTab(tabSolicitudes, "Solicitudes");

    // Conexiones
    connect(btnActualizar, &QPushButton::clicked, this, &FuncionAmigos::actualizarListas);
    connect(inputBusqueda, &QLineEdit::textChanged, this, &FuncionAmigos::buscarUsuarios);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabs);
}

void FuncionAmigos::actualizarListas() {
    cargarUsuariosNoAmigos();
    cargarSolicitudesEnviadas();
    cargarSolicitudesRecibidas();
}

void FuncionAmigos::cargarUsuariosNoAmigos() {
    listaUsuarios->clear();

    QString archivoEstados = "Estados/" + getUsernameActual() + ".txt";

    QFile file(archivoEstados);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            QStringList partes = linea.split(',');

            if(partes.size() >= 2 && partes[1] == "0") {
                QString username = partes[0];
                if(username != getUsernameActual()) {
                    QListWidgetItem* item = new QListWidgetItem(username);
                    item->setData(Qt::UserRole, partes[1]); // Guardar estado
                    listaUsuarios->addItem(item);
                }
            }
        }
        file.close();
    }
}

void FuncionAmigos::cargarSolicitudesEnviadas() {
    listaEnviadas->clear();

    QString archivoEstados = "Estados/" + getUsernameActual() + ".txt";

    QFile file(archivoEstados);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            QStringList partes = linea.split(',');

            if(partes.size() >= 2 && partes[1] == "1") {
                QListWidgetItem* item = new QListWidgetItem(partes[0]);
                QWidget* widget = new QWidget;
                QHBoxLayout* layout = new QHBoxLayout(widget);

                QLabel* label = new QLabel(partes[0]);
                QPushButton* btnCancelar = new QPushButton("Cancelar");

                layout->addWidget(label);
                layout->addStretch();
                layout->addWidget(btnCancelar);
                widget->setLayout(layout);

                connect(btnCancelar, &QPushButton::clicked, [this, partes]() {
                    try {
                        master->getUser()->NegarSolicitud(partes[0]);
                        actualizarListas();
                    } catch(const std::exception& e) {
                        QMessageBox::warning(this, "Error", e.what());
                    }
                });

                listaEnviadas->addItem(item);
                listaEnviadas->setItemWidget(item, widget);
            }
        }
        file.close();
    }
}

void FuncionAmigos::cargarSolicitudesRecibidas() {
    listaRecibidas->clear();

    QString archivoEstados = "Estados/" + getUsernameActual() + ".txt";

    QFile file(archivoEstados);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            QStringList partes = linea.split(',');

            if(partes.size() >= 2 && partes[1] == "2") {
                QListWidgetItem* item = new QListWidgetItem;
                QWidget* widget = new QWidget;
                QHBoxLayout* layout = new QHBoxLayout(widget);

                QLabel* label = new QLabel(partes[0]);
                QPushButton* btnAceptar = new QPushButton("Aceptar");
                QPushButton* btnRechazar = new QPushButton("Rechazar");

                layout->addWidget(label);
                layout->addStretch();
                layout->addWidget(btnAceptar);
                layout->addWidget(btnRechazar);
                widget->setLayout(layout);

                connect(btnAceptar, &QPushButton::clicked, [this, partes]() {
                    try {
                        master->getUser()->aceptarSolicitud(partes[0]);
                        actualizarListas();
                    } catch(const std::exception& e) {
                        QMessageBox::warning(this, "Error", e.what());
                    }
                });

                connect(btnRechazar, &QPushButton::clicked, [this, partes]() {
                    try {
                        master->getUser()->NegarSolicitud(partes[0]);
                        actualizarListas();
                    } catch(const std::exception& e) {
                        QMessageBox::warning(this, "Error", e.what());
                    }
                });

                listaRecibidas->addItem(item);
                listaRecibidas->setItemWidget(item, widget);
            }
        }
        file.close();
    }
}

void FuncionAmigos::buscarUsuarios() {
    QString busqueda = inputBusqueda->text().trimmed().toLower();

    for(int i = 0; i < listaUsuarios->count(); ++i) {
        QListWidgetItem* item = listaUsuarios->item(i);
        bool coincide = item->text().toLower().contains(busqueda);
        item->setHidden(!coincide);
    }
}

QString FuncionAmigos::getUsernameActual() const {
    return master ? master->getUsername() : "";
}

*/
