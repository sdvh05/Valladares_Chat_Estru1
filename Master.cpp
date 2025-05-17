#include "Master.h"

// Constructor
Master::Master() {
    cargarUsuarios();
}

// Destructor - liberar memoria de usuarios
Master::~Master() {
    for (Usuario* u : todosLosUsuarios) {
        delete u;
    }
    todosLosUsuarios.clear();
}

// Validar credenciales leyendo allaccs.txt
bool Master::validateCredentials(const QString &username, const QString &password) {
    QFile file("allaccs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(',');
        if (parts.size() >= 9 && parts[0] == username && parts[3] == password) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Buscar datos del usuario por username en allaccs.txt
QStringList Master::findUserData(const QString &username) {
    QFile file("allaccs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return {};

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(',');
        if (parts.size() >= 9 && parts[0] == username) {
            file.close();
            return parts;
        }
    }
    file.close();
    return {};
}

// Cargar currentUser desde datos
void Master::loadExistingUser(const QStringList &userData) {
    currentUser = std::make_shared<Usuario>(
        userData[0], // username
        userData[1], // nombre
        userData[2], // email
        userData[3], // contraseña
        userData[4].toInt(), // edad
        userData[5], // avatar
        userData[6], // pregunta
        userData[7], // respuesta
        userData[8].toInt(), // online
        false // no registrar
        );
}

// Cargar todos los usuarios desde allaccs.txt
void Master::cargarUsuarios() {
    for (Usuario* u : todosLosUsuarios) {
        delete u;
    }
    todosLosUsuarios.clear();

    QFile file("allaccs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(',');
        if (parts.size() >= 9) {
            Usuario* nuevo = new Usuario(
                parts[0], //Username
                parts[1], //Nombre
                parts[2], //Correo
                parts[3], //Password
                parts[4].toInt(), //Edad
                parts[5], //Avatar
                parts[6], // Pregunta
                parts[7], // Respuesta
                parts[8].toInt(), //0= Ofline 1 =Online
                false
                );
            todosLosUsuarios.append(nuevo);
        }
    }
    file.close();
}



/*
void Master::cargarUsuarios() {
    for (Usuario* u : todosLosUsuarios) {
        delete u;
    }
    todosLosUsuarios.clear();

    QFile file("allaccs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split(',');
        if (parts.size() >= 9) {
            Usuario* nuevo = new Usuario(
                parts[0], parts[1], parts[2], parts[3],
                parts[4].toInt(), parts[5],
                parts[6], parts[7], parts[8].toInt(), false
                );
            todosLosUsuarios.append(nuevo);

            // DEBUG: Mostrar los datos cargados
            qDebug() << "-------Usuario------";
            qDebug() << "Username:" << parts[0];
            qDebug() << "Nombre:" << parts[1];
            qDebug() << "Correo:" << parts[2];
            qDebug() << "Password:" << parts[3];
            qDebug() << "Edad" << parts[4].toInt();
            qDebug() << "Avatar (ruta foto):" << parts[5];
            qDebug() << "Pregunta" << parts[6];
            qDebug() << "Respuesta" << parts[7];
            qDebug() << "Activo:" << parts[8].toInt(); // 0= Ofline 1 =Online
            qDebug() << "";
        } else {
            qDebug() << "Línea inválida o incompleta en allaccs.txt";
        }
    }
    file.close();
}
*/

// Login
bool Master::login(const QString &username, const QString &password) {
    if (validateCredentials(username, password)) {
        QStringList userData = findUserData(username);
        if (userData.size() >= 9) {
            loadExistingUser(userData);
            setOnline();
            return true;
        }
    }
    return false;
}

// Crear nuevo usuario (signIn)
void Master::signIn(const QString &username, const QString &nombre,
                    const QString &email, const QString &contra,
                    int edad, const QString &rutaImagen,
                    const QString &pregunta, const QString &respuesta) {

    //ver si ya Existe
    if (buscarUsuarioPorNombre(username) != nullptr) {
        QMessageBox::warning(nullptr, "Aviso", "El usuario ya existe.");
        return;
    }

    try {
        currentUser = std::make_shared<Usuario>(
            username,
            nombre,
            email,
            contra,
            edad,
            rutaImagen,
            pregunta,
            respuesta,
            1 // Online
            );
        // Añadir a lista local para evitar recarga innecesaria
        todosLosUsuarios.append(currentUser.get());


    } catch(const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", e.what());
    }
}

// Logout
void Master::logout() {
    if(currentUser) {
        Usuario::setOfline(currentUser->getUsername());
        currentUser.reset();
    }
}

// Set online y offline
void Master::setOnline() {
    if(currentUser) Usuario::setOnline(currentUser->getUsername());
}

void Master::setOffline() {
    if(currentUser) Usuario::setOfline(currentUser->getUsername());
}

// Getters usuario actual
QString Master::getUsername() const {
    return currentUser ? currentUser->getUsername() : "";
}

QString Master::getEmail() const {
    return currentUser ? currentUser->getEmail() : "";
}

QString Master::getAvatar() const {
    return currentUser ? currentUser->getAvatar() : "";
}

int Master::getOnlineStatus() const {
    return currentUser ? currentUser->getOnline() : 0;
}

QString Master::getTXTEstados() const {
    return "Estados/" + getUsername() + ".txt";
}

std::shared_ptr<Usuario> Master::getUser() const {
    return currentUser;
}

Usuario* Master::getUsuarioActual() const {
    return currentUser.get();
}

QList<Usuario*>& Master::getUsuarios() {
    if (todosLosUsuarios.isEmpty()) {
        cargarUsuarios();
    }
    return todosLosUsuarios;
}

Usuario* Master::buscarUsuarioPorNombre(const QString &username) const {
    for (Usuario* u : todosLosUsuarios) {
        if (u->getUsername() == username) {
            return u;
        }
    }
    return nullptr;
}

// Filtrado usuarios por estado leyendo archivo de estados -- AmigosManejo
QList<Usuario*> Master::getUsersPorEstado(int estadoBuscado) const {
    QList<Usuario*> resultado;
    QString archivoEstados = getTXTEstados();

    QFile file(archivoEstados);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return resultado;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(',');

        if (partes.size() != 2) continue;

        QString username = partes[0].trimmed();
        QString estadoStr = partes[1].trimmed();

        bool ok;
        int estado = estadoStr.toInt(&ok);
        if (!ok) continue;

        if (estado == estadoBuscado) {
            Usuario* u = buscarUsuarioPorNombre(username);
            if (u != nullptr) {
                resultado.append(u);
            }
        }
    }

    file.close();
    return resultado;
}

QList<Usuario*> Master::getUsers0() const { return getUsersPorEstado(0); } //Get No Amigos
QList<Usuario*> Master::getUsers1() const { return getUsersPorEstado(1); } //Get a Quienes Envie Soli
QList<Usuario*> Master::getUsers2() const { return getUsersPorEstado(2); } //Get Quienes me enviaron Soli
QList<Usuario*> Master::getUsers3() const { return getUsersPorEstado(3); } //Get Amigos



/*
QQList<Usuario*> Master::cargarAmigos() {
    cargarUsuarios(); // Aseguramos que todosLosUsuarios está actualizado

    QList<QString> amigosUsernames = getUsernamesPorEstado(3); // Solo los usernames con estado 3
    QList<Usuario*> listaCompletaDeAmigos;

    for (const QString& usernameAmigo : amigosUsernames) {
        for (Usuario* usuarioCompleto : todosLosUsuarios) {
            if (usuarioCompleto->getUsername() == usernameAmigo) {
                listaCompletaDeAmigos.append(usuarioCompleto);
                break;
            }
        }
    }

    return listaCompletaDeAmigos;
}
}
*/

QList<QString> Master::getUsernamesPorEstado(int estadoBuscado) const {
    QList<QString> resultado;
    QString archivoEstados = getTXTEstados();

    QFile file(archivoEstados);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return resultado;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(',');

        if (partes.size() != 2) continue;

        QString username = partes[0].trimmed();
        QString estadoStr = partes[1].trimmed();

        bool ok;
        int estado = estadoStr.toInt(&ok);
        if (!ok) continue;

        if (estado == estadoBuscado) {
            resultado.append(username);
        }
    }

    file.close();
    return resultado;
}

QList<Usuario*> Master::cargarAmigos() {
    cargarUsuarios(); // Asegura que todosLosUsuarios está actualizado

    QList<QString> amigosUsernames = getUsernamesPorEstado(3);
    QList<Usuario*> listaCompletaDeAmigos;

   // qDebug() << "Usernames encontrados con estado 3:" << amigosUsernames;
    //qDebug() << "Cantidad total de usuarios cargados:" << todosLosUsuarios.size();

    for (const QString& usernameAmigo : amigosUsernames) {
        bool encontrado = false;
        for (Usuario* usuarioCompleto : todosLosUsuarios) {
            if (usuarioCompleto->getUsername() == usernameAmigo) {
                listaCompletaDeAmigos.append(usuarioCompleto);
                //qDebug() << "Amigo agregado:" << usernameAmigo;
                //qDebug() <<usuarioCompleto->getNombreCompleto();
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            qDebug() << "Usuario no encontrado en todosLosUsuarios:" << usernameAmigo;
        }
    }

    //qDebug() << "Total amigos agregados:" << listaCompletaDeAmigos.size();
    return listaCompletaDeAmigos;
}




