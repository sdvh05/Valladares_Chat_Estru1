#include "Master.h"

// Constructor
Master::Master() {
    cargarUsuarios();
}

// Destructor
Master::~Master() {
    for (Usuario* u : todosLosUsuarios) {
        delete u;
    }
    todosLosUsuarios.clear();
}

// Validar Login leyendo allaccs.txt
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



//--------------------------------------------------------------------------------------------------

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

//Amigos
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

//CARGAR AMIGOS
//----------------------------------------------------------------------------------------------------------------------------------------
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

////ORDENAMIENTO
QList<Usuario*> Master::CargarAmigosAlfabetico() {
    QList<Usuario*> amigos = cargarAmigos();
    int n = amigos.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            QString nombreA = amigos[j]->getUsername().toLower();
            QString nombreB = amigos[j + 1]->getUsername().toLower();

            if (nombreA > nombreB) {
                Usuario* temp = amigos[j];
                amigos[j] = amigos[j + 1];
                amigos[j + 1] = temp;
            }
        }
    }

    return amigos;
}


QList<Usuario*> Master::CargarAmigosLength() {
    QList<Usuario*> amigos = cargarAmigos();
    int n = amigos.size();
    QString user1 = getUsername();

    QVector<int> lineCounts(n, 0);

    // Llenamos los conteos
    for (int i = 0; i < n; ++i) {
        QString user2 = amigos[i]->getUsername();

        QString archivo1 = "Chats/" + user1 + "-" + user2 + ".txt";
        QString archivo2 = "Chats/" + user2 + "-" + user1 + ".txt";
        QString ruta = QFile::exists(archivo1) ? archivo1 :
                           (QFile::exists(archivo2) ? archivo2 : archivo1);

        if (!QFile::exists(ruta)) {
            QFile nuevoArchivo(ruta);
            nuevoArchivo.open(QIODevice::WriteOnly);
            nuevoArchivo.close();
            lineCounts[i] = 0;
            continue;
        }

        QFile file(ruta);
        int count = 0;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                in.readLine();
                count++;
            }
            file.close();
        }
        lineCounts[i] = count;
    }

    // Bubble sort
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (lineCounts[j] < lineCounts[j + 1]) {
                // Intercambiamos líneas
                int tempLine = lineCounts[j];
                lineCounts[j] = lineCounts[j + 1];
                lineCounts[j + 1] = tempLine;

                // Intercambiamos usuarios
                Usuario* tempUser = amigos[j];
                amigos[j] = amigos[j + 1];
                amigos[j + 1] = tempUser;
            }
        }
    }

    return amigos;
}



//CAMBIOS
//----------------------------------------------------------------------------------------------------------------------------------------
bool Master::cambiarContrasena(const QString& username, const QString& pregunta, const QString& respuesta, const QString& nuevaContrasena) {
    QFile file("allaccs.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QStringList lineas;
    QTextStream in(&file);
    bool actualizado = false;

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList parts = linea.split(',');

        if (parts.size() >= 9 && parts[0] == username && parts[6] == pregunta && parts[7] == respuesta) {
            // Validar que la nueva contraseña sea diferente
            if (parts[3] == nuevaContrasena) {
                QMessageBox::warning(nullptr, "Error", "La nueva contraseña no puede ser igual a la anterior.");
                file.close();
                return false;
            }

            parts[3] = nuevaContrasena; // Cambiar solo la contraseña
            linea = parts.join(",");
            actualizado = true;
        }

        lineas << linea;
    }

    file.close();

    if (actualizado) {
        QFile outFile("allaccs.txt");
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;

        QTextStream out(&outFile);
        for (const QString& l : lineas) {
            out << l << "\n";
        }

        outFile.close();
        return true;
    }

    QMessageBox::warning(nullptr, "Error", "Usuario o datos de seguridad incorrectos.");
    return false;
}

//NOTIFICACIONES
//--------------------------------------------------------------------------------------------------------------
void Master::enviarNotificacion(const QString& receptor) {
    QString ruta = QString("Notificacion/%1.txt").arg(receptor);
    QFile archivo(ruta);
    QString username= getUsername();

    if (!archivo.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de notificaciones de" << receptor;
        return;
    }

    QStringList lineas;
    QTextStream in(&archivo);
    bool encontrado = false;

    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        QStringList partes = linea.split(',');

        if (partes.size() == 2 && partes[0] == username) {
            int conteo = partes[1].toInt();
            partes[1] = QString::number(conteo + 1);
            linea = partes.join(',');
            encontrado = true;
        }

        lineas << linea;
    }

    if (!encontrado) {
        lineas << username + ",1";
    }

    archivo.resize(0); // Limpiar archivo antes de reescribir
    QTextStream out(&archivo);
    for (const QString& l : lineas) {
        out << l << "\n";
    }

    archivo.close();
}

void Master::leerNotificacion(const QString& receptor) {
    QString username= getUsername();
    QString ruta = QString("Notificacion/%1.txt").arg(username);
    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de notificaciones del usuario actual";
        return;
    }

    QStringList lineas;
    QTextStream in(&archivo);

    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        QStringList partes = linea.split(',');

        if (partes.size() == 2 && partes[0] == receptor) {
            partes[1] = "0";
            linea = partes.join(',');
        }

        lineas << linea;
    }

    archivo.resize(0); // Vaciar archivo
    QTextStream out(&archivo);
    for (const QString& l : lineas) {
        out << l << "\n";
    }

    archivo.close();
}

QList<QString> Master::cargarNotificaciones() const {
    QList<QString> notificaciones;
    QString ruta = QString("Notificacion/%1.txt").arg(getUsername());
    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[DEBUG] No se pudo abrir el archivo de notificaciones de" << getUsername();
        return notificaciones;
    }

    QTextStream in(&archivo);
    QMap<QString, int> conteo;

    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (!linea.isEmpty()) {
            QStringList partes = linea.split(",");
            if (partes.size() == 2) {
                QString usuario = partes[0].trimmed();
                int cantidad = partes[1].trimmed().toInt();

                if (cantidad > 0) {
                    conteo[usuario] = cantidad;
                    notificaciones << usuario;
                }
            } else {
                qDebug() << "[DEBUG] Línea malformada en notificaciones:" << linea;
            }
        }
    }

    archivo.close();

    for (auto it = conteo.begin(); it != conteo.end(); ++it) {
        qDebug() << " - Usuario:" << it.key() << "| Notificaciones:" << it.value();
    }

    return notificaciones;
}







