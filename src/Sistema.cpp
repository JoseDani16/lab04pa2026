#include "Sistema.h"
#include "Agenda.h"
#include "Apartamento.h"
#include "Casa.h"
#include "../ICollection/Integer.h"
#include "../ICollection/String.h"
#include "../ICollection/collections/OrderedDictionary.h"
#include "../ICollection/interfaces/IIterator.h"
#include <sstream>
#include <vector>

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

bool Sistema::existeNickname(const std::string& nickname) const {
    String key(nickname.c_str());
    return usuarios->member(&key);
}

Usuario* Sistema::buscarUsuario(const std::string& nickname) const {
    String key(nickname.c_str());
    return dynamic_cast<Usuario*>(usuarios->find(&key));
}

Cliente* Sistema::buscarCliente(const std::string& nickname) const {
    return dynamic_cast<Cliente*>(buscarUsuario(nickname));
}

Propietario* Sistema::buscarPropietario(const std::string& nickname) const {
    return dynamic_cast<Propietario*>(buscarUsuario(nickname));
}

Inmobiliaria* Sistema::buscarInmobiliaria(const std::string& nickname) const {
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

bool Sistema::altaCliente(const std::string& nickname, const std::string& contrasenia,
                          const std::string& nombre, const std::string& email,
                          const std::string& apellido, const std::string& documento,
                          std::string& error) {
    if (contrasenia.size() < 6) {
        error = "La contrasenia debe tener al menos 6 caracteres.";
        return false;
    }
    if (existeNickname(nickname)) {
        error = "Ya existe un usuario con ese nickname.";
        return false;
    }
    usuarios->add(new String(nickname.c_str()),
                  new Cliente(nickname, contrasenia, nombre, email, apellido, documento));
    return true;
}

bool Sistema::altaPropietario(const std::string& nickname, const std::string& contrasenia,
                              const std::string& nombre, const std::string& email,
                              const std::string& cuentaBancaria, const std::string& telefono,
                              std::string& error) {
    if (contrasenia.size() < 6) {
        error = "La contrasenia debe tener al menos 6 caracteres.";
        return false;
    }
    if (existeNickname(nickname)) {
        error = "Ya existe un usuario con ese nickname.";
        return false;
    }
    usuarios->add(new String(nickname.c_str()),
                  new Propietario(nickname, contrasenia, nombre, email, cuentaBancaria, telefono));
    return true;
}

bool Sistema::altaInmobiliaria(const std::string& nickname, const std::string& contrasenia,
                               const std::string& nombre, const std::string& email,
                               const std::string& direccion, const std::string& telefono,
                               const std::string& url, std::string& error) {
    if (contrasenia.size() < 6) {
        error = "La contrasenia debe tener al menos 6 caracteres.";
        return false;
    }
    if (existeNickname(nickname)) {
        error = "Ya existe un usuario con ese nickname.";
        return false;
    }
    usuarios->add(new String(nickname.c_str()),
                  new Inmobiliaria(nickname, contrasenia, nombre, email, direccion, telefono, url));
    return true;
}

bool Sistema::altaCasa(const std::string& nickPropietario, const std::string& direccion,
                       int numeroPuerta, float superficie, int anioConstruccion,
                       bool propiedadHorizontal, TipoTecho tipoTecho,
                       int& codigoGenerado, std::string& error) {
    Propietario* propietario = buscarPropietario(nickPropietario);
    if (propietario == nullptr) {
        error = "No existe un propietario con ese nickname.";
        return false;
    }
    codigoGenerado = proxCodigoInmueble++;
    Casa* casa = new Casa(codigoGenerado, direccion, numeroPuerta, superficie,
                          anioConstruccion, propietario, propiedadHorizontal, tipoTecho);
    propietario->agregarInmueble(casa);
    inmuebles->add(new Integer(codigoGenerado), casa);
    return true;
}

bool Sistema::altaApartamento(const std::string& nickPropietario, const std::string& direccion,
                              int numeroPuerta, float superficie, int anioConstruccion,
                              int piso, bool tieneAscensor, float gastosComunes,
                              int& codigoGenerado, std::string& error) {
    Propietario* propietario = buscarPropietario(nickPropietario);
    if (propietario == nullptr) {
        error = "No existe un propietario con ese nickname.";
        return false;
    }
    codigoGenerado = proxCodigoInmueble++;
    Apartamento* apartamento = new Apartamento(codigoGenerado, direccion, numeroPuerta, superficie,
                                               anioConstruccion, propietario, piso,
                                               tieneAscensor, gastosComunes);
    propietario->agregarInmueble(apartamento);
    inmuebles->add(new Integer(codigoGenerado), apartamento);
    return true;
}

bool Sistema::representarPropietario(const std::string& nickInmobiliaria,
                                     const std::string& nickPropietario,
                                     std::string& error) {
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    Propietario* propietario = buscarPropietario(nickPropietario);
    if (inmobiliaria == nullptr) {
        error = "No existe una inmobiliaria con ese nickname.";
        return false;
    }
    if (propietario == nullptr) {
        error = "No existe un propietario con ese nickname.";
        return false;
    }
    inmobiliaria->agregarPropietarioRepresentado(propietario);
    return true;
}

bool Sistema::altaAdministracion(const std::string& nickInmobiliaria, int codigoInmueble,
                                 std::string& error) {
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    Inmueble* inmueble = buscarInmueble(codigoInmueble);
    if (inmobiliaria == nullptr) {
        error = "No existe una inmobiliaria con ese nickname.";
        return false;
    }
    if (inmueble == nullptr) {
        error = "No existe un inmueble con ese codigo.";
        return false;
    }
    if (!inmobiliaria->representaA(inmueble->getPropietario())) {
        error = "La inmobiliaria no representa al propietario de ese inmueble.";
        return false;
    }
    if (buscarAdministracion(inmobiliaria, inmueble) != nullptr) {
        error = "La inmobiliaria ya administra ese inmueble.";
        return false;
    }
    Administracion* administracion = new Administracion(fechaSistema, inmobiliaria, inmueble);
    inmobiliaria->agregarAdministracion(administracion);
    inmueble->agregarAdministracion(administracion);
    return true;
}

bool Sistema::altaPublicacion(const std::string& nickInmobiliaria, int codigoInmueble,
                              TipoPublicacion tipo, const std::string& texto, float precio,
                              int& codigoGenerado, std::string& error) {
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    Inmueble* inmueble = buscarInmueble(codigoInmueble);
    Administracion* administracion = buscarAdministracion(inmobiliaria, inmueble);
    if (administracion == nullptr) {
        error = "No existe una administracion para esa inmobiliaria e inmueble.";
        return false;
    }

    IIterator* it = administracion->getPublicaciones()->getIterator();
    while (it->hasCurrent()) {
        Publicacion* publicacion = dynamic_cast<Publicacion*>(it->getCurrent());
        if (publicacion->getTipo() == tipo && publicacion->getFecha() == fechaSistema) {
            delete it;
            error = "Ya existe una publicacion de ese tipo para la fecha actual.";
            return false;
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
            if (activa == nullptr || activa->getFecha() < publicacion->getFecha()) {
                activa = publicacion;
            }
        }
        it->next();
    }
    delete it;
    if (activa != nullptr) {
        activa->setActiva(true);
    }
    return true;
}

bool Sistema::altaAgenda(const std::string& nickCliente, int codigoPublicacion,
                         const DTFecha& fechaVisita, const std::string& formaContacto,
                         std::string& error) {
    Cliente* cliente = buscarCliente(nickCliente);
    Publicacion* publicacion = buscarPublicacion(codigoPublicacion);
    if (cliente == nullptr) {
        error = "No existe un cliente con ese nickname.";
        return false;
    }
    if (publicacion == nullptr) {
        error = "No existe una publicacion con ese codigo.";
        return false;
    }
    if (!publicacion->estaActiva()) {
        error = "La publicacion no esta activa.";
        return false;
    }
    Agenda* agenda = new Agenda(fechaVisita, formaContacto, cliente, publicacion);
    cliente->agregarAgenda(agenda);
    publicacion->agregarAgenda(agenda);
    return true;
}

std::vector<std::string> Sistema::listarUsuarios() const {
    std::vector<std::string> res;
    IIterator* it = usuarios->getIterator();
    while (it->hasCurrent()) {
        Usuario* usuario = dynamic_cast<Usuario*>(it->getCurrent());
        std::ostringstream oss;
        oss << usuario->getNickname() << " - " << usuario->getNombre()
            << " (" << usuario->getTipo() << ")";
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

std::vector<std::string> Sistema::listarInmuebles() const {
    std::vector<std::string> res;
    IIterator* it = inmuebles->getIterator();
    while (it->hasCurrent()) {
        Inmueble* inmueble = dynamic_cast<Inmueble*>(it->getCurrent());
        std::ostringstream oss;
        oss << inmueble->getCodigo() << " - " << inmueble->getDireccion()
            << " - Propietario: " << inmueble->getPropietario()->getNombre();
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

std::vector<std::string> Sistema::listarInmobiliarias() const {
    std::vector<std::string> res;
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

std::vector<std::string> Sistema::listarPropietarios() const {
    std::vector<std::string> res;
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

std::vector<std::string> Sistema::listarInmueblesAdministrables(const std::string& nickInmobiliaria) const {
    std::vector<std::string> res;
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
                std::ostringstream oss;
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

std::vector<std::string> Sistema::listarInmueblesAdministrados(const std::string& nickInmobiliaria) const {
    std::vector<std::string> res;
    Inmobiliaria* inmobiliaria = buscarInmobiliaria(nickInmobiliaria);
    if (inmobiliaria == nullptr) {
        return res;
    }
    IIterator* it = inmobiliaria->getAdministraciones()->getIterator();
    while (it->hasCurrent()) {
        Administracion* administracion = dynamic_cast<Administracion*>(it->getCurrent());
        Inmueble* inmueble = administracion->getInmueble();
        std::ostringstream oss;
        oss << inmueble->getCodigo() << " - " << inmueble->getDireccion()
            << " - Desde: " << administracion->getFechaComienzo().toString();
        res.push_back(oss.str());
        it->next();
    }
    delete it;
    return res;
}

std::vector<std::string> Sistema::consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                         float precioMax, FiltroTipoInmueble filtro) const {
    std::vector<std::string> res;
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
                            std::ostringstream oss;
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

std::string Sistema::detalleInmueble(int codigoInmueble) const {
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

    std::vector<Publicacion*> publicacionesABorrar;
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

bool Sistema::eliminarInmueble(int codigoInmueble, std::string& error) {
    Inmueble* inmueble = buscarInmueble(codigoInmueble);
    if (inmueble == nullptr) {
        error = "No existe un inmueble con ese codigo.";
        return false;
    }

    std::vector<Administracion*> administracionesABorrar;
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
    return true;
}

void Sistema::limpiarDatos() {
    std::vector<Administracion*> administracionesABorrar;
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
    std::string error;
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
