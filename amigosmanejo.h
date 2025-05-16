#ifndef AMIGOSMANEJO_H
#define AMIGOSMANEJO_H

#include <QWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

#include "Master.h"
#include "Usuario.h"


class AmigosManejo : public QWidget
{
    Q_OBJECT

private:
    Master* master;
    QMenuBar* menuBar;
    QStackedWidget* stackedWidget;

    // Widgets de cada sección
    QWidget* widgetAnadir;
    QWidget* widgetEnviadas;
    QWidget* widgetRecibidas;

    QListWidget* listaAnadir;
    QPushButton* botonEnviarSolicitud;

    QListWidget* listaEnviadas;

    QListWidget* listaRecibidas;
    QPushButton* botonAceptar;
    QPushButton* botonRechazar;

    void setupUI();
    void cargarListaAnadir();
    void cargarListaEnviadas();
    void cargarListaRecibidas();

private slots:
    void mostrarAnadir();
    void mostrarEnviadas();
    void mostrarRecibidas();

    void enviarSolicitud();
    void aceptarSolicitud();
    void rechazarSolicitud();

public:
    AmigosManejo(Master* masterPtr, QWidget* parent = nullptr);
};

#endif // AMIGOSMANEJO_H
