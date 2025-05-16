#ifndef LOGIN_H
#define LOGIN_H

#include "Master.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);

private slots:
    void verificarCredenciales();

private:
    Master* master;

    QLabel *usuarioLabel;
    QLabel *contrasenaLabel;
    QLineEdit *usuarioInput;
    QLineEdit *contrasenaInput;
    QPushButton *botonLogin;
    QPushButton *botonOlvidaste;
    QPushButton *botonSignin;
};

#endif // LOGIN_H
