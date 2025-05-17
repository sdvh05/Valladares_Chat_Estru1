#ifndef SIGNIN_H
#define SIGNIN_H

#include "Master.h"

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

class Login; // Para usar puntero

class SigninWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SigninWindow(Master* master, Login *ventanaLogin, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void seleccionarImagen();
    void registrarUsuario();

public:
    void ReturnLogin();

private:
    Master* master;

    QLineEdit *inputUsername;
    QLineEdit *inputNombreCompleto;
    QLineEdit *inputCorreo;
    QLineEdit *inputContrasena;
    QLineEdit *inputConfirmacion;
    QSpinBox *inputEdad;

    //Imagen
    QLabel *labelAvatar;
    QLabel *imagenAvatar;
    QPushButton *botonSeleccionarAvatar;
    QString rutaImagenSeleccionada;


    QComboBox *comboPreguntas;
    QLineEdit *inputRespuesta;

    Login *loginVentana;
};



#endif // SIGNIN_H

