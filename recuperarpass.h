#ifndef RECUPERARPASS_H
#define RECUPERARPASS_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class RecuperarPass : public QWidget
{
    Q_OBJECT

public:
    explicit RecuperarPass(QWidget *parent = nullptr);

private slots:
    void verificarRespuesta();
    void cerrarVentana();

private:
    QLineEdit *inputUsername;
    QComboBox *comboPreguntas;
    QLineEdit *inputRespuesta;
    QPushButton *botonCambiar;
    QPushButton *botonRegresar;
};

#endif // RECUPERARPASS_H
