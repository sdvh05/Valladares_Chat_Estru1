#include "Usuario.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <algorithm>

// Constructor
Usuario::Usuario(QString username, QString nombreCompleto, QString email, QString contrasena,
                 int edad, QString avatar, QString pregunta, QString respuesta, int Online, bool shouldRegister)
    : username(username),
    nombreCompleto(nombreCompleto),
    email(email),
    contrasena(contrasena),
    edad(edad),
    avatar(avatar),
    preguntaSeguridad(pregunta),
    respuestaSeguridad(respuesta),
    Online(Online)
{
    if (shouldRegister) {
        crearCarpetas();
        crearArchivosIniciales();
        registrarUsuario();
    }

    qDebug() << "Ruta de allaccs.txt:" << QFileInfo("allaccs.txt").absoluteFilePath();
    qDebug() << "Ruta de Estados:" << QDir("Estados").absolutePath();
}

// Métodos estáticos
bool Usuario::validarUsername(const QString &username) {
    QFile archivo("allaccs.txt");
    if (!archivo.exists()) return true;

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            if (linea.startsWith(username + ",")) {
                archivo.close();
                return false;
            }
        }
        archivo.close();
    }
    return true;
}

void Usuario::crearCarpetas() {
    QDir dir;
    if (!dir.exists("Estados")) {
        if (!dir.mkpath("Estados")) {
            qDebug() << "Error creando carpeta Estados";
            throw std::runtime_error("Error creando directorios");
        }
    }
    if (!dir.exists("Chats")) {
        if (!dir.mkpath("Chats")) {
            qDebug() << "Error creando carpeta Chats";
            throw std::runtime_error("Error creando directorios");
        }
    }
}

void Usuario::crearArchivosIniciales() {
    QStringList usuariosExistentes = leerUsuariosRegistrados();
    actualizarEstadosExistentes(usuariosExistentes);
    crearEstadoNuevoUsuario(usuariosExistentes);
}

QStringList Usuario::leerUsuariosRegistrados() {
    QStringList usuarios;
    QFile archivo("allaccs.txt");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            if (!linea.isEmpty()) {
                QStringList partes = linea.split(',');
                if (!partes.isEmpty()) usuarios << partes[0];
            }
        }
        archivo.close();
    }
    return usuarios;
}

void Usuario::actualizarEstadosExistentes(const QStringList &usuarios) {
    foreach (QString usuario, usuarios) {
        QString ruta = QString("Estados/%1.txt").arg(usuario);
        QFile archivo(ruta);

        if (archivo.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << username << ",0\n";
            archivo.close();
        }
    }
}

void Usuario::crearEstadoNuevoUsuario(const QStringList &usuarios) {
    QString ruta = QString("Estados/%1.txt").arg(username);
    QFile archivo(ruta);

    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&archivo);
        foreach (QString usuario, usuarios) {
            out << usuario << ",0\n";
        }
        archivo.close();
    }
}

void Usuario::actualizarEstado(const QString &usuario, const QString &objetivo, int estado) {
    QString ruta = QString("Estados/%1.txt").arg(usuario);
    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error abriendo archivo:" << ruta;
        return;
    }

    QStringList lineas;
    bool encontrado = false;
    QTextStream in(&archivo);

    // Leer y modificar líneas
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        QStringList partes = linea.split(',');

        if (partes.size() >= 1 && partes[0] == objetivo) {
            linea = QString("%1,%2").arg(objetivo).arg(estado);
            encontrado = true;
        }
        lineas << linea;
    }

    // Si no se encontró, añadir nueva entrada
    if (!encontrado) {
        lineas << QString("%1,%2").arg(objetivo).arg(estado);
    }

    // Reescribir archivo
    archivo.resize(0);
    QTextStream out(&archivo);
    foreach (const QString &linea, lineas) {
        out << linea << "\n";
    }

    archivo.close();
}

int Usuario::obtenerEstado(const QString &objetivo) {
    QString ruta = QString("Estados/%1.txt").arg(username);
    QFile archivo(ruta);

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            QStringList partes = linea.split(',');
            if (partes.size() >= 2 && partes[0] == objetivo) {
                archivo.close();
                return partes[1].toInt();
            }
        }
        archivo.close();
    }
    return 0; // Estado por defecto si no se encuentra
}

bool Usuario::usuarioExiste(const QString &username) {
    QFile archivo("allaccs.txt");
    if (!archivo.exists()) return false;

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine().trimmed();
            if (linea.startsWith(username + ",")) {
                archivo.close();
                return true;
            }
        }
        archivo.close();
    }
    return false;
}

void Usuario::actualizarEstadoEnArchivo(const QString &username, int nuevoEstado) {
    QFile archivo("allaccs.txt");
    if (!archivo.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Error abriendo archivo para actualizar estado";
        return;
    }

    QStringList lineas;
    QTextStream in(&archivo);
    bool encontrado = false;

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(',');

        if (partes.size() >= 9 && partes[0] == username) {
            partes[8] = QString::number(nuevoEstado);
            linea = partes.join(',');
            encontrado = true;
        }
        lineas << linea;
    }

    if (encontrado) {
        archivo.resize(0);
        QTextStream out(&archivo);
        foreach (const QString &linea, lineas) {
            out << linea << "\n";
        }
    }
    else {
        qDebug() << "Usuario no encontrado para actualizar estado";
    }

    archivo.close();
}

void Usuario::registrarUsuario() {
    QFile archivo("allaccs.txt");
    if (!archivo.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error abriendo allaccs.txt";
        throw std::runtime_error("Error registrando usuario");
    }

    QTextStream out(&archivo);
    out << username << ","
        << nombreCompleto << ","
        << email << ","
        << contrasena << ","
        << edad << ","
        << avatar << ","
        << preguntaSeguridad << ","
        << respuestaSeguridad << ","
        << Online << "\n";
    archivo.close();
}

void Usuario::setOnline(const QString &username) {
    actualizarEstadoEnArchivo(username, 1);
}

void Usuario::setOfline(const QString &username) {
    actualizarEstadoEnArchivo(username, 0);
}

void Usuario::crearChat(const QString &otroUsuario) {
    QStringList usuarios = {username, otroUsuario};
    std::sort(usuarios.begin(), usuarios.end());
    QString nombreArchivo = QString("Chats/%1-%2.txt").arg(usuarios[0]).arg(usuarios[1]);

    QFile archivo(nombreArchivo);
    if (!archivo.exists()) {
        if (archivo.open(QIODevice::WriteOnly)) {
            archivo.close();
        }
    }
}

void Usuario::enviarSolicitud(const QString &receptor) {
    if (receptor == username) {
        QMessageBox::warning(nullptr, "Error", "No puedes enviarte una solicitud a ti mismo");
        return;
    }

    if (!usuarioExiste(receptor)) {
        QMessageBox::warning(nullptr, "Error", "El usuario receptor no existe");
        return;
    }

    // Actualizar estado del remitente
    actualizarEstado(username, receptor, 1);
    // Actualizar estado del receptor
    actualizarEstado(receptor, username, 2);

    QMessageBox::information(nullptr, "Éxito", "Solicitud enviada correctamente");
}

void Usuario::aceptarSolicitud(const QString &solicitante) {
    if (solicitante == username) {
        QMessageBox::warning(nullptr, "Error", "No puedes aceptar tu propia solicitud");
        return;
    }

    if (obtenerEstado(solicitante) != 2) {
        QMessageBox::warning(nullptr, "Error", "No hay solicitud pendiente de este usuario");
        return;
    }

    actualizarEstado(username, solicitante, 3);
    actualizarEstado(solicitante, username, 3);
    crearChat(solicitante);

    QMessageBox::information(nullptr, "Éxito", "Solicitud aceptada correctamente");
}

void Usuario::NegarSolicitud(const QString &solicitante) {
    if (solicitante == username) {
        QMessageBox::warning(nullptr, "Error", "No puedes negar tu propia solicitud");
        return;
    }

    if (obtenerEstado(solicitante) != 2) {
        QMessageBox::warning(nullptr, "Error", "No hay solicitud pendiente de este usuario");
        return;
    }

    actualizarEstado(username, solicitante, 0);
    actualizarEstado(solicitante, username, 0);

    QMessageBox::information(nullptr, "Éxito", "Solicitud negada correctamente");
}

    QString Usuario::getUsername() const { return username; }
    QString Usuario::getNombreCompleto() const { return nombreCompleto; }
    QString Usuario::getEmail() const { return email; }
    QString Usuario::getContrasena() const { return contrasena; }
    int Usuario::getEdad() const { return edad; }
    QString Usuario::getAvatar() const { return avatar; }
    QString Usuario::getPreguntaSeguridad() const { return preguntaSeguridad; }
    QString Usuario::getRespuestaSeguridad() const { return respuestaSeguridad; }
    bool Usuario::isOnline() const { return Online == 1; }
    int Usuario::getOnline() const { return Online; }

        QStringList Usuario::obtenerAmigos() {
        QStringList amigos;
        QString ruta = QString("Estados/%1.txt").arg(username);
        QFile archivo(ruta);

        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            while (!in.atEnd()) {
                QString linea = in.readLine().trimmed();
                QStringList partes = linea.split(',');
                if (partes.size() >= 2 && partes[1] == "3") {  // estado 3 = amigos
                    amigos << partes[0];
                }
            }
            archivo.close();
        }
        return amigos;
}
