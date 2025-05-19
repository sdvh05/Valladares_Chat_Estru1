/*#ifndef FUNCIONAMIGOS_H
#define FUNCIONAMIGOS_H

#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>

#include "Master.h"


class FuncionAmigos : public QDialog {
    Q_OBJECT

public:
    FuncionAmigos(Master* master, QWidget* parent = nullptr);

private slots:
    void actualizarListas();
    void buscarUsuarios();
    void manejarAceptarSolicitud();
    void manejarNegarSolicitud();
    void manejarCancelarSolicitud();

private:
    Master* master;
    QTabWidget* tabs;

    // Pestaña 1: Añadir amigos
    QLineEdit* inputBusqueda;
    QListWidget* listaUsuarios;

    // Pestaña 2: Solicitudes
    QListWidget* listaEnviadas;
    QListWidget* listaRecibidas;

    void crearUI();
    void cargarSolicitudesEnviadas();
    void cargarSolicitudesRecibidas();
    void cargarUsuariosNoAmigos();

    QString getUsernameActual() const;
};

#endif // FUNCIONAMIGOS_H

*/
