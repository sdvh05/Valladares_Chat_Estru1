// ContactoWidget.cpp
#include "ContactoWidget.h"
#include <QPixmap>

ContactoWidget::ContactoWidget(Usuario* usuario, QWidget* parent)
    : QWidget(parent), usuario(usuario)
{
    lblImagen = new QLabel;
    lblImagen->setFixedSize(40, 40);
    lblImagen->setScaledContents(true);
    QPixmap foto(usuario->getRutaFoto());
    if (!foto.isNull()) {
        lblImagen->setPixmap(foto);
    } else {
        lblImagen->setText("?");
    }

    lblNombreEstado = new QLabel(usuario->getUsername() +
                                 (usuario->estaActivo() ? " (Online)" : " (Offline)"));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lblImagen);
    layout->addWidget(lblNombreEstado);
    layout->addStretch();
    setLayout(layout);
}

Usuario* ContactoWidget::getUsuario() const {
    return usuario;
}
