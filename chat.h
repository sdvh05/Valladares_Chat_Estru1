#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QTimer>

//prueba
#include <QStack>

#include "master.h"

class Chat : public QWidget {
    Q_OBJECT

public:
    explicit Chat(Master* master, QWidget *parent = nullptr);
    void setLoginVentana(QWidget *ventana);

private slots:
    void LoginLogout();
    void seleccionarContacto(QListWidgetItem *item);
    void enviarMensaje();
    void deshacerMensaje();
    void mostrarStickersPopup();
    void Eliminar(QListWidgetItem *item);
    void ordenarContactos(int index);
    void refrescarPeriodicamente();

private:
    // Métodos
    void crearLayout();
    void mostrarContactosConAvatares();
    void mostrarContactosConAvatares(const QList<Usuario*>& amigos);
    void agregarMensajeWidget(const QString& mensaje, bool esPropio);
    int obtenerCantidadNotis(const QString& usuario);
    void mostrarOpcionesConfiguracion();

    // Punteros a objetos
    Master* master;
    QWidget* loginVentana;

    // Widgets
    QLabel *lblAvatar;
    QLabel *lblNombre;
    QLabel *lblUser;
    QLabel *lblTituloConfig;
    QLabel *lblNombreContacto;
    QListWidget *listaContactos;
    QLineEdit *inputMensaje;
    QPushButton *btnEnviar;
    QPushButton *btnDeshacer;
    QPushButton *btnAgregar;
    QPushButton *btnEliminar;
    QPushButton *btnConfig;
    QPushButton *btnCerrarSesion;
    QPushButton *btnStickers;
    QScrollArea *chatScrollArea;
    QWidget *chatContainer;
    QVBoxLayout *chatLayout;
    QComboBox *comboOrdenamiento;

    // Estado
    QString contactoActual;
    QString rutaArchivoChatActual;
    QStack<QString> pilaMensajes;
    QTimer* timerRefresco;
};

#endif // CHAT_H

