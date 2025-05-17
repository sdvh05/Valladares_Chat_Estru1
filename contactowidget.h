#ifndef CONTACTOWIDGET_H
#define CONTACTOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "usuario.h"

class ContactoWidget : public QWidget {
    Q_OBJECT
public:
    ContactoWidget(Usuario* usuario, QWidget* parent = nullptr);

    Usuario* getUsuario() const;

private:
    Usuario* usuario;
    QLabel* lblImagen;
    QLabel* lblNombreEstado;
};

#endif // CONTACTOWIDGET_H
