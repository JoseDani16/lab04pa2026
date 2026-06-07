#include "Inmobiliaria.h"
#include "Administracion.h"
#include "Propietario.h"
#include "../ICollection/collections/List.h"
#include "../ICollection/interfaces/IIterator.h"

using namespace std;

Inmobiliaria::Inmobiliaria(const string& nickname, const string& contrasenia,
                           const string& nombre, const string& email,
                           const string& direccion, const string& telefono,
                           const string& url):
        Usuario(nickname, contrasenia, nombre, email), direccion(direccion),
        telefono(telefono), url(url), propietariosRepresentados(new List()),
        administraciones(new List()) {
}

Inmobiliaria::~Inmobiliaria() {
    IIterator* it = administraciones->getIterator();
    while (it->hasCurrent()) {
        Administracion* administracion = dynamic_cast<Administracion*>(it->getCurrent());
        delete administracion;
        it->next();
    }
    delete it;
    delete propietariosRepresentados;
    delete administraciones;
}

string Inmobiliaria::getTipo() const { return "Inmobiliaria"; }
string Inmobiliaria::getDireccionContacto() const { return direccion; }
string Inmobiliaria::getTelefono() const { return telefono; }
string Inmobiliaria::getUrl() const { return url; }

void Inmobiliaria::agregarPropietarioRepresentado(Propietario* propietario) {
    propietariosRepresentados->add(propietario);
}

bool Inmobiliaria::representaA(Propietario* propietario) const {
    return propietariosRepresentados->member(propietario);
}

ICollection* Inmobiliaria::getPropietariosRepresentados() const {
    return propietariosRepresentados;
}

void Inmobiliaria::agregarAdministracion(Administracion* administracion) {
    administraciones->add(administracion);
}

void Inmobiliaria::quitarAdministracion(Administracion* administracion) {
    administraciones->remove(administracion);
}

ICollection* Inmobiliaria::getAdministraciones() const {
    return administraciones;
}
