#ifndef CHAT_H
#define CHAT_H

#include "Master.h"

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>

#include <QTimer>

//Pilas Y colas, eliminar al crear las propias
#include <QStack>
#include <QQueue>

class Chat : public QWidget {
    Q_OBJECT

public:
    Chat(Master* master, QWidget *parent = nullptr);

private slots:
    void enviarMensaje();
    void deshacerMensaje();
    void seleccionarContacto(QListWidgetItem* item);

private:
    Master* master;

    // Panel de botones/configuración
    QPushButton *btnConfig;
    QPushButton *btnAgregar;
    QPushButton *btnEliminar;
    QPushButton *btnStickers;
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

    void crearLayout();
    void agregarContacto(const QString &nombre, const QString &estado);
};

#endif // CHAT_H
