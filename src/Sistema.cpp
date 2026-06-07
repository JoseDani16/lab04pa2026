#include "Sistema.h"
#include "Agenda.h"
#include "Apartamento.h"
#include "Casa.h"
#include "../ICollection/Integer.h"
#include "../ICollection/String.h"
#include "../ICollection/collections/OrderedDictionary.h"
#include "../ICollection/interfaces/IIterator.h"
#include <exception>
#include <sstream>
#include <vector>

using namespace std;

Sistema* Sistema::instance = nullptr;

Sistema::Sistema():
        usuarios(new OrderedDictionary()), inmuebles(new OrderedDictionary()),
        proxCodigoInmueble(1), proxCodigoPublicacion(1), fechaSistema(27, 5, 2026) {
}

Sistema::~Sistema() {
    limpiarDatos();
    delete usuarios;
    delete inmuebles;
}

Sistema* Sistema::getInstance() {
    if (instance == nullptr) {
        instance = new Sistema();
    }
    return instance;
}

void Sistema::setFechaSistema(const DTFecha& fecha) {
    fechaSistema = fecha;
}

DTFecha Sistema::getFechaSistema() const {
    return fechaSistema;
}

bool Sistema::existeUsuario(const string& nickname) const {
    return existeNickname(nickname);
}

bool Sistema::existeNickname(const string& nickname) const {
    String key(nickname.c_str());
    return usuarios->member(&key);
}

Usuario* Sistema::buscarUsuario(const string& nickname) const {
    String key(nickname.c_str());
    return dynamic_cast<Usuario*>(usuarios->find(&key));
}

Cliente* Sistema::buscarCliente(const string& nickname) const {
    return dynamic_cast<Cliente*>(buscarUsuario(nickname));
}

Propietario* Sistema::buscarPropietario(const string& nickname) const {
    return dynamic_cast<Propietario*>(buscarUsuario(nickname));
}

Inmobiliaria* Sistema::buscarInmobiliaria(const string& nickname) const {
    return dynamic_cast<Inmobiliaria*>(buscarUsuario(nickname));
}

Inmueble* Sistema::buscarInmueble(int codigo) const {
    Integer key(codigo);
    return dynamic_cast<Inmueble*>(inmuebles->find(&key));
}

Administracion* Sistema::buscarAdministracion(Inmobiliaria* inmobiliaria, Inmueble* inmueble) const {
    if (inmobiliaria == nullptr || inmueble == nullptr) {
        return nullptr;
    }
    IIterator* it = inmobiliaria->getAdministraciones()->getIterator();
    while (it->hasCurrent()) {
        Administracion* administracion = dynamic_cast<Administracion*>(it->getCurrent());
        if (administracion->getInmueble() == inmueble) {
            delete it;
            return administracion;
        }
        it->next();
    }
    delete it;
    return nullptr;
}

Publicacion* Sistema::buscarPublicacion(int codigoPublicacion) const {
    IIterator* itUsuarios = usuarios->getIterator();
    while (itUsuarios->hasCurrent()) {
        Inmobiliaria* inmobiliaria = dynamic_cast<Inmobiliaria*>(itUsuarios->getCurrent());
        if (inmobiliaria != nullptr) {
            IIterator* itAdm = inmobiliaria->getAdministraciones()->getIterator();
            while (itAdm->hasCurrent()) {
                Administracion* administracion = dynamic_cast<Administracion*>(itAdm->getCurrent());
                IIterator* itPub = administracion->getPublicaciones()->getIterator();
                while (itPub->hasCurrent()) {
                    Publicacion* publicacion = dynamic_cast<Publicacion*>(itPub->getCurrent());
                    if (publicacion->getCodigo() == codigoPublicacion) {
                        delete itPub;
                        delete itAdm;
                        delete itUsuarios;
                        return publicacion;
                    }
                    itPub->next();
                }
                delete itPub;
                itAdm->next();
            }
            delete itAdm;
        }
        itUsuarios->next();
    }
    delete itUsuarios;
    return nullptr;
}

Status Sistema::altaCliente(const string& nickname, const string& contrasenia,
                            const string& nombre, const string& email,
                            const string& apellido, const string& documento,
                            string& error) {
    try {
        if (contrasenia.size() < 6) {
            error = "La contrasenia debe tener al menos 6 caracteres.";
            return Status::ERROR;
        }
        if (existeNickname(nickname)) {
            error = "Ya existe un usuario con ese nickname.";
            return Status::ERROR;
        }
        usuarios->add(new String(nickname.c_str()),
                      new Cliente(nickname, contrasenia, nombre, email, apellido, documento));
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaPropietario(const string& nickname, const string& contrasenia,
                                const string& nombre, const string& email,
                                const string& cuentaBancaria, const string& telefono,
                                string& error) {
    try {
        if (contrasenia.size() < 6) {
            error = "La contrasenia debe tener al menos 6 caracteres.";
            return Status::ERROR;
        }
        if (existeNickname(nickname)) {
            error = "Ya existe un usuario con ese nickname.";
            return Status::ERROR;
        }
        usuarios->add(new String(nickname.c_str()),
                      new Propietario(nickname, contrasenia, nombre, email, cuentaBancaria, telefono));
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaInmobiliaria(const string& nickname, const string& contrasenia,
                                 const string& nombre, const string& email,
                                 const string& direccion, const string& telefono,
                                 const string& url, string& error) {
    try {
        if (contrasenia.size() < 6) {
            error = "La contrasenia debe tener al menos 6 caracteres.";
            return Status::ERROR;
        }
        if (existeNickname(nickname)) {
            error = "Ya existe un usuario con ese nickname.";
            return Status::ERROR;
        }
        usuarios->add(new String(nickname.c_str()),
                      new Inmobiliaria(nickname, contrasenia, nombre, email, direccion, telefono, url));
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaCasa(const string& nickPropietario, const string& direccion,
                         int numeroPuerta, float superficie, int anioConstruccion,
                         bool propiedadHorizontal, TipoTecho tipoTecho,
                         int& codigoGenerado, string& error) {
    try {
        Propietario* propietario = buscarPropietario(nickPropietario);
        if (propietario == nullptr) {
            error = "No existe un propietario con ese nickname.";
            return Status::ERROR;
        }
        codigoGenerado = proxCodigoInmueble++;
        Casa* casa = new Casa(codigoGenerado, direccion, numeroPuerta, superficie,
                              anioConstruccion, propietario, propiedadHorizontal, tipoTecho);
        propietario->agregarInmueble(casa);
        inmuebles->add(new Integer(codigoGenerado), casa);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaApartamento(const string& nickPropietario, const string& direccion,
                                int numeroPuerta, float superficie, int anioConstruccion,
                                int piso, bool tieneAscensor, float gastosComunes,
                                int& codigoGenerado, string& error) {
    try {
        Propietario* propietario = buscarPropietario(nickPropietario);
        if (propietario == nullptr) {
            error = "No existe un propietario con ese nickname.";
            return Status::ERROR;
        }
        codigoGenerado = proxCodigoInmueble++;
        Apartamento* apartamento = new Apartamento(codigoGenerado, direccion, numeroPuerta, superficie,
                                                   anioConstruccion, propietario, piso,
                                                   tieneAscensor, gastosComunes);
        propietario->agregarInmueble(apartamento);
        inmuebles->add(new Integer(codigoGenerado), apartamento);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::representarPropietario(const string& nickInmobiliaria,
                                       const string& nickPropietario,
                                       string& error) {
    try {
        Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
        Propietario* propietario = buscarPropietario(nickPropietario);
        if (inmobiliaria == nullptr) {
            error = "No existe una inmobiliaria con ese nickname.";
            return Status::ERROR;
        }
        if (propietario == nullptr) {
            error = "No existe un propietario con ese nickname.";
            return Status::ERROR;
        }
        inmobiliaria->agregarPropietarioRepresentado(propietario);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaAdministracion(const string& nickInmobiliaria, int codigoInmueble,
                                   string& error) {
    try {
        Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
        Inmueble* inmueble = buscarInmueble(codigoInmueble);
        if (inmobiliaria == nullptr) {
            error = "No existe una inmobiliaria con ese nickname.";
            return Status::ERROR;
        }
        if (inmueble == nullptr) {
            error = "No existe un inmueble con ese codigo.";
            return Status::ERROR;
        }
        if (!inmobiliaria->representaA(inmueble->getPropietario())) {
            error = "La inmobiliaria no representa al propietario de ese inmueble.";
            return Status::ERROR;
        }
        if (buscarAdministracion(inmobiliaria, inmueble) != nullptr) {
            error = "La inmobiliaria ya administra ese inmueble.";
            return Status::ERROR;
        }
        Administracion* administracion = new Administracion(fechaSistema, inmobiliaria, inmueble);
        inmobiliaria->agregarAdministracion(administracion);
        inmueble->agregarAdministracion(administracion);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaPublicacion(const string& nickInmobiliaria, int codigoInmueble,
                                TipoPublicacion tipo, const string& texto, float precio,
                                int& codigoGenerado, string& error) {
    try {
        Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
        Inmueble* inmueble = buscarInmueble(codigoInmueble);
        Administracion* administracion = buscarAdministracion(inmobiliaria, inmueble);
        if (administracion == nullptr) {
            error = "No existe una administracion para esa inmobiliaria e inmueble.";
            return Status::ERROR;
        }

        IIterator* it = administracion->getPublicaciones()->getIterator();
        while (it->hasCurrent()) {
            Publicacion* publicacion = dynamic_cast<Publicacion*>(it->getCurrent());
            if (publicacion->getTipo() == tipo && publicacion->getFecha() == fechaSistema) {
                delete it;
                error = "Ya existe una publicacion de ese tipo para la fecha actual.";
                return Status::ERROR;
            }
            it->next();
        }
        delete it;

        codigoGenerado = proxCodigoPublicacion++;
        Publicacion* nueva = new Publicacion(codigoGenerado, fechaSistema, tipo, texto, precio, administracion);
        administracion->agregarPublicacion(nueva);

        Publicacion* activa = nullptr;
        it = administracion->getPublicaciones()->getIterator();
        while (it->hasCurrent()) {
            Publicacion* publicacion = dynamic_cast<Publicacion*>(it->getCurrent());
            if (publicacion->getTipo() == tipo) {
                publicacion->setActiva(false);
                if (activa == nullptr || activa->getFecha() < publicacion->getFecha()) activa = publicacion;
            }
            it->next();
        }
        delete it;
        if (activa != nullptr) activa->setActiva(true);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

Status Sistema::altaAgenda(const string& nickCliente, int codigoPublicacion,
                           const DTFecha& fechaVisita, const string& formaContacto,
                           string& error) {
    try {
        Cliente* cliente = buscarCliente(nickCliente);
        Publicacion* publicacion = buscarPublicacion(codigoPublicacion);
        if (cliente == nullptr) {
            error = "No existe un cliente con ese nickname.";
            return Status::ERROR;
        }
        if (publicacion == nullptr) {
            error = "No existe una publicacion con ese codigo.";
            return Status::ERROR;
        }
        if (!publicacion->estaActiva()) {
            error = "La publicacion no esta activa.";
            return Status::ERROR;
        }
        Agenda* agenda = new Agenda(fechaVisita, formaContacto, cliente, publicacion);
        cliente->agregarAgenda(agenda);
        publicacion->agregarAgenda(agenda);
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

vector<string> Sistema::listarUsuarios() const {
    vector<string> res;
    IIterator* it = usuarios->getIterator();
    while (it->hasCurrent()) {
        Usuario* usuario = dynamic_cast<Usuario*>(it->getCurrent());
        ostringstream oss;
        oss << usuario->getNickname() << " - " << usuario->getNombre()
            << " (" << usuario->getTipo() << ")";
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

vector<string> Sistema::listarInmuebles() const {
    vector<string> res;
    IIterator* it = inmuebles->getIterator();
    while (it->hasCurrent()) {
        Inmueble* inmueble = dynamic_cast<Inmueble*>(it->getCurrent());
        ostringstream oss;
        oss << inmueble->getCodigo() << " - " << inmueble->getDireccion()
            << " - Propietario: " << inmueble->getPropietario()->getNombre();
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

vector<string> Sistema::listarInmobiliarias() const {
    vector<string> res;
    IIterator* it = usuarios->getIterator();
    while (it->hasCurrent()) {
        Inmobiliaria* inmobiliaria = dynamic_cast<Inmobiliaria*>(it->getCurrent());
        if (inmobiliaria != nullptr) {
            res.push_back(inmobiliaria->getNickname() + " - " + inmobiliaria->getNombre());
        }
        it->next();
    }
    delete it;
    return res;
}

vector<string> Sistema::listarPropietarios() const {
    vector<string> res;
    IIterator* it = usuarios->getIterator();
    while (it->hasCurrent()) {
        Propietario* propietario = dynamic_cast<Propietario*>(it->getCurrent());
        if (propietario != nullptr) {
            res.push_back(propietario->getNickname() + " - " + propietario->getNombre());
        }
        it->next();
    }
    delete it;
    return res;
}

vector<string> Sistema::listarInmueblesAdministrables(const string& nickInmobiliaria) const {
    vector<string> res;
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    if (inmobiliaria == nullptr) {
        return res;
    }
    IIterator* itProp = inmobiliaria->getPropietariosRepresentados()->getIterator();
    while (itProp->hasCurrent()) {
        Propietario* propietario = dynamic_cast<Propietario*>(itProp->getCurrent());
        IIterator* itInm = propietario->getInmuebles()->getIterator();
        while (itInm->hasCurrent()) {
            Inmueble* inmueble = dynamic_cast<Inmueble*>(itInm->getCurrent());
            if (buscarAdministracion(inmobiliaria, inmueble) == nullptr) {
                ostringstream oss;
                oss << inmueble->getCodigo() << " - " << inmueble->getDireccion()
                    << " - Propietario: " << propietario->getNombre();
                res.push_back(oss.str());
            }
            itInm->next();
        }
        delete itInm;
        itProp->next();
    }
    delete itProp;
    return res;
}

vector<string> Sistema::listarInmueblesAdministrados(const string& nickInmobiliaria) const {
    vector<string> res;
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    if (inmobiliaria == nullptr) {
        return res;
    }
    IIterator* it = inmobiliaria->getAdministraciones()->getIterator();
    while (it->hasCurrent()) {
        Administracion* administracion = dynamic_cast<Administracion*>(it->getCurrent());
        Inmueble* inmueble = administracion->getInmueble();
        ostringstream oss;
        oss << inmueble->getCodigo() << " - " << inmueble->getDireccion()
            << " - Desde: " << administracion->getFechaComienzo().toString();
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

vector<string> Sistema::consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                         float precioMax, FiltroTipoInmueble filtro) const {
    vector<string> res;
    IIterator* itUsuarios = usuarios->getIterator();
    while (itUsuarios->hasCurrent()) {
        Inmobiliaria* inmobiliaria = dynamic_cast<Inmobiliaria*>(itUsuarios->getCurrent());
        if (inmobiliaria != nullptr) {
            IIterator* itAdm = inmobiliaria->getAdministraciones()->getIterator();
            while (itAdm->hasCurrent()) {
                Administracion* administracion = dynamic_cast<Administracion*>(itAdm->getCurrent());
                Inmueble* inmueble = administracion->getInmueble();
                bool coincideTipoInmueble =
                    filtro == FiltroTipoInmueble::Todos ||
                    (filtro == FiltroTipoInmueble::Casas && dynamic_cast<Casa*>(inmueble) != nullptr) ||
                    (filtro == FiltroTipoInmueble::Apartamentos && dynamic_cast<Apartamento*>(inmueble) != nullptr);

                if (coincideTipoInmueble) {
                    IIterator* itPub = administracion->getPublicaciones()->getIterator();
                    while (itPub->hasCurrent()) {
                        Publicacion* publicacion = dynamic_cast<Publicacion*>(itPub->getCurrent());
                        if (publicacion->estaActiva() && publicacion->getTipo() == tipo &&
                            publicacion->getPrecio() >= precioMin && publicacion->getPrecio() <= precioMax) {
                            ostringstream oss;
                            oss << publicacion->getCodigo() << " - " << publicacion->getFecha().toString()
                                << " - " << publicacion->getTexto()
                                << " - $" << publicacion->getPrecio()
                                << " - Inmobiliaria: " << inmobiliaria->getNombre()
                                << " - Inmueble: " << inmueble->getCodigo();
                            res.push_back(oss.str());
                        }
                        itPub->next();
                    }
                    delete itPub;
                }
                itAdm->next();
            }
            delete itAdm;
        }
        itUsuarios->next();
    }
    delete itUsuarios;
    return res;
}

string Sistema::detalleInmueble(int codigoInmueble) const {
    Inmueble* inmueble = buscarInmueble(codigoInmueble);
    return inmueble == nullptr ? "No existe un inmueble con ese codigo." : inmueble->getDetalle();
}

void Sistema::borrarPublicacion(Publicacion* publicacion) {
    if (publicacion == nullptr) {
        return;
    }
    Administracion* administracion = publicacion->getAdministracion();
    if (administracion != nullptr) {
        administracion->quitarPublicacion(publicacion);
    }
    delete publicacion;
}

void Sistema::borrarAdministracion(Administracion* administracion) {
    if (administracion == nullptr) {
        return;
    }

    vector<Publicacion*> publicacionesABorrar;
    IIterator* itPub = administracion->getPublicaciones()->getIterator();
    while (itPub->hasCurrent()) {
        publicacionesABorrar.push_back(dynamic_cast<Publicacion*>(itPub->getCurrent()));
        itPub->next();
    }
    delete itPub;

    for (Publicacion* publicacion : publicacionesABorrar) {
        borrarPublicacion(publicacion);
    }
    if (administracion->getInmobiliaria() != nullptr) {
        administracion->getInmobiliaria()->quitarAdministracion(administracion);
    }
    if (administracion->getInmueble() != nullptr) {
        administracion->getInmueble()->quitarAdministracion(administracion);
    }
    delete administracion;
}

Status Sistema::eliminarInmueble(int codigoInmueble, string& error) {
    try {
        Inmueble* inmueble = buscarInmueble(codigoInmueble);
        if (inmueble == nullptr) {
            error = "No existe un inmueble con ese codigo.";
            return Status::ERROR;
        }

        vector<Administracion*> administracionesABorrar;
        IIterator* itAdm = inmueble->getAdministraciones()->getIterator();
        while (itAdm->hasCurrent()) {
            administracionesABorrar.push_back(dynamic_cast<Administracion*>(itAdm->getCurrent()));
            itAdm->next();
        }
        delete itAdm;

        for (Administracion* administracion : administracionesABorrar) {
            borrarAdministracion(administracion);
        }
        if (inmueble->getPropietario() != nullptr) {
            inmueble->getPropietario()->quitarInmueble(inmueble);
        }
        Integer key(codigoInmueble);
        inmuebles->remove(&key);
        delete inmueble;
        return Status::OK;
    } catch (const exception& e) {
        error = string("Error inesperado: ") + e.what();
        return Status::ERROR;
    }
}

void Sistema::limpiarDatos() {
    vector<Administracion*> administracionesABorrar;
    IIterator* itUsuarios = usuarios->getIterator();
    while (itUsuarios->hasCurrent()) {
        Inmobiliaria* inmobiliaria = dynamic_cast<Inmobiliaria*>(itUsuarios->getCurrent());
        if (inmobiliaria != nullptr) {
            IIterator* itAdm = inmobiliaria->getAdministraciones()->getIterator();
            while (itAdm->hasCurrent()) {
                administracionesABorrar.push_back(dynamic_cast<Administracion*>(itAdm->getCurrent()));
                itAdm->next();
            }
            delete itAdm;
        }
        itUsuarios->next();
    }
    delete itUsuarios;

    for (Administracion* administracion : administracionesABorrar) {
        borrarAdministracion(administracion);
    }

    IIterator* itInmuebles = inmuebles->getIterator();
    while (itInmuebles->hasCurrent()) {
        delete dynamic_cast<Inmueble*>(itInmuebles->getCurrent());
        itInmuebles->next();
    }
    delete itInmuebles;
    delete inmuebles;
    inmuebles = new OrderedDictionary();

    itUsuarios = usuarios->getIterator();
    while (itUsuarios->hasCurrent()) {
        delete dynamic_cast<Usuario*>(itUsuarios->getCurrent());
        itUsuarios->next();
    }
    delete itUsuarios;
    delete usuarios;
    usuarios = new OrderedDictionary();
}

void Sistema::cargarDatosPrueba() {
    string error;
    int codigo = 0;
    int codigoPublicacion = 0;

    limpiarDatos();
    proxCodigoInmueble = 1;
    proxCodigoPublicacion = 1;
    fechaSistema = DTFecha(27, 5, 2026);

    altaCliente("cliente1", "123456", "Ana", "ana@mail.com", "Perez", "12345678", error);
    altaPropietario("prop1", "123456", "Carlos", "carlos@mail.com", "UY00BROU", "099111222", error);
    altaPropietario("prop2", "123456", "Beatriz", "bea@mail.com", "UY00ITAU", "098333444", error);
    altaInmobiliaria("inmo1", "123456", "Raices Centro", "info@raices.com",
                     "18 de Julio 1000", "29000000", "https://raices.example", error);

    altaCasa("prop1", "Rivera", 1234, 120, 1998, false, TipoTecho::Plano, codigo, error);
    altaApartamento("prop2", "Colonia", 555, 64, 2015, 7, true, 8500, codigo, error);
    representarPropietario("inmo1", "prop1", error);
    representarPropietario("inmo1", "prop2", error);
    altaAdministracion("inmo1", 1, error);
    altaAdministracion("inmo1", 2, error);
    altaPublicacion("inmo1", 1, TipoPublicacion::Venta, "Casa luminosa con patio", 150000, codigoPublicacion, error);
    altaPublicacion("inmo1", 2, TipoPublicacion::Alquiler, "Apartamento centrico", 32000, codigoPublicacion, error);
    altaAgenda("cliente1", 1, DTFecha(3, 6, 2026), "Coordinar por telefono", error);
}
