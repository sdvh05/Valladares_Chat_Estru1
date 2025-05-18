#ifndef CHAT_H
#define CHAT_H

#include "Master.h"
#include "login.h"
#include "signin.h"

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QStack>
#include <QQueue>
#include <QTimer>

class Login;
class SignIn;

class Chat : public QWidget {
    Q_OBJECT

public:
    Chat(Master* master, QWidget *parent = nullptr);

    void setLoginVentana(QWidget *ventana);
    void LoginLogout();
    void Eliminar(QListWidgetItem *item);

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
    QLineEdit *inputMensaje;
    QPushButton *btnEnviar;
    QPushButton *btnDeshacer;

    // Panel de perfil
    QLabel *lblAvatar;
    QLabel *lblNombre;
    QLabel *lblUser;

    // Área de mensajes estilo burbujas
    QScrollArea *chatScrollArea;
    QWidget *chatContainer;
    QVBoxLayout *chatLayout;

    // Pilas y colas
    QStack<QString> pilaMensajes;
    QQueue<QString> colaMensajesNoLeidos;

    // Métodos auxiliares
    void crearLayout();
    void mostrarContactosConAvatares();
    void agregarMensajeWidget(const QString& mensaje, bool esPropio);
    void mostrarStickersPopup();

};

#endif // CHAT_H
