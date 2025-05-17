#ifndef USUARIO_H
#define USUARIO_H

#include <QString>
#include <QStringList>

class Usuario {
protected:
    QString username;
    QString nombreCompleto;
    QString email;
    QString contrasena;
    int edad;
    QString avatar;
    QString preguntaSeguridad;
    QString respuestaSeguridad;
    int Online; // 0 Offline | 1 Online

public:
    Usuario(QString username, QString nombreCompleto, QString email, QString contrasena,
            int edad, QString avatar, QString pregunta, QString respuesta, int Online = 1, bool shouldRegister = true);

    static bool validarUsername(const QString &username);

    void registrarUsuario();

    static void setOnline(const QString &username);
    static void setOfline(const QString &username);

    void crearChat(const QString &otroUsuario);

    //0=No Amigos | 1=Envie Solicitud | 2= Me Enviaron Solicitud | 3=Amigos
    void enviarSolicitud(const QString &receptor);
    void aceptarSolicitud(const QString &solicitante);
    void NegarSolicitud(const QString &solicitante);
    void EliminarAmigo(const QString &amigo);

    QString getUsername() const;
    QString getNombreCompleto() const;
    QString getEmail() const;
    QString getContrasena() const;
    int getEdad() const;
    QString getAvatar() const;
    QString getPreguntaSeguridad() const;
    QString getRespuestaSeguridad() const;
    bool isOnline() const;
    int getOnline() const;

    QStringList obtenerAmigos();

private:
    void crearCarpetas();
    void crearArchivosIniciales();

    QStringList leerUsuariosRegistrados();
    void actualizarEstadosExistentes(const QStringList &usuarios);
    void crearEstadoNuevoUsuario(const QStringList &usuarios);

    void actualizarEstado(const QString &usuario, const QString &objetivo, int estado);
    int obtenerEstado(const QString &objetivo);

    static bool usuarioExiste(const QString &username);
    static void actualizarEstadoEnArchivo(const QString &username, int nuevoEstado);
};

#endif // USUARIO_H
