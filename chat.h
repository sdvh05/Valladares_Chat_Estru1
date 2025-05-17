#ifndef CHAT_H
#define CHAT_H

#include "Master.h"
#include "login.h"
#include "signin.h"

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QString>

#include <QTimer>

//Pilas Y colas, eliminar al crear las propias
#include <QStack>
#include <QQueue>

class Login;
class SignIn;

class Chat : public QWidget {
    Q_OBJECT

public:
    Chat(Master* master, QWidget *parent = nullptr);

    void setLoginVentana(QWidget *ventana);
    void LoginLogout();

    QString rutaArchivoChatActual;
    QString contactoActual;


private slots:
    void enviarMensaje();
    void deshacerMensaje();
    void seleccionarContacto(QListWidgetItem* item);

private:
    Master* master;
    QWidget* loginVentana = nullptr;

    // Panel de botones/configuración
    QPushButton *btnConfig;
    QPushButton *btnAgregar;
    QPushButton *btnEliminar;
    QPushButton *btnStickers;
    QPushButton *btnCerrarSesion;

    QLabel *lblTituloConfig;

    // Panel de usuarios
    QListWidget *listaContactos;

    // Panel de chat
    QLabel *lblNombreContacto;
    QTextEdit *chatArea;
    QLineEdit *inputMensaje;
    QPushButton *btnEnviar;
    QPushButton *btnDeshacer;

    // Almacenar último mensaje (para "deshacer")
    QStack<QString> pilaMensajes;

    // Cola para mensajes no leídos
    QQueue<QString> colaMensajesNoLeidos;

    //Mini Mi Perfil
    QLabel *lblAvatar;
    QLabel *lblNombre;
    QLabel *lblUser;

    void crearLayout();
    void agregarContacto(const QString &nombre, const QString &estado);
    void mostrarContactosConAvatares();



};

#endif // CHAT_H
