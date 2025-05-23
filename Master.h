#ifndef MASTER_H
#define MASTER_H

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <QList>
#include <QString>
#include <exception>

#include "Usuario.h"

class Master
{
private:
    std::shared_ptr<Usuario> currentUser;
    QFile usersFile{"allaccs.txt"};
    QList<Usuario*> todosLosUsuarios;

    bool validateCredentials(const QString &username, const QString &password);
    QStringList findUserData(const QString &username);
    void loadExistingUser(const QStringList &userData);

public:
    void cargarUsuarios();

public:
    Master();
    ~Master();

    // Login, SignIn, Logout
    bool login(const QString &username, const QString &password);
    void signIn(const QString &username, const QString &nombre,
                const QString &email, const QString &contra,
                int edad, const QString &rutaImagen,
                const QString &pregunta, const QString &respuesta);
    void logout();

    // Online / Offline
    void setOnline();
    void setOffline();

    // Getters de información del usuario
    QString getUsername() const;
    QString getEmail() const;
    QString getAvatar() const;
    int getOnlineStatus() const;

    QString getTXTEstados() const;

    std::shared_ptr<Usuario> getUser() const;
    Usuario* getUsuarioActual() const;

    // Manejo de usuarios
    QList<Usuario*>& getUsuarios();
    Usuario* buscarUsuarioPorNombre(const QString &username) const;

    // Filtrado por estado
    QList<Usuario*> getUsersPorEstado(int estadoBuscado) const;
    QList<Usuario*> getUsers0() const;
    QList<Usuario*> getUsers1() const;
    QList<Usuario*> getUsers2() const;
    QList<Usuario*> getUsers3() const;



    QList<QString> getUsernamesPorEstado(int estado) const;
    QList<Usuario*> cargarAmigos();

    //Ordenamientos
    QList<Usuario*> CargarAmigosAlfabetico();
    QList<Usuario*> CargarAmigosLength();

    //Notificaciones
    void enviarNotificacion(const QString& receptor);
    void leerNotificacion(const QString& receptor);
    QList<QString> cargarNotificaciones() const;


    //Cambios
    void actualizarLineaUsuario(const QStringList& nuevaLinea);
    static bool cambiarContrasena(const QString& username, const QString& pregunta, const QString& respuesta, const QString& nuevaContrasena);
    void cambiarNombre(const QString& nuevo);
    void cambiarEmail(const QString& nuevo);
    void cambiarEdad(int nuevaEdad);
    void cambiarAvatar(const QString& nuevaRuta);
};

#endif // MASTER_H
