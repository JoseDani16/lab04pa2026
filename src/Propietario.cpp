#include "Propietario.h"
#include "Inmueble.h"
#include "../ICollection/collections/List.h"

Propietario::Propietario(const std::string& nickname, const std::string& contrasenia,
                         const std::string& nombre, const std::string& email,
                         const std::string& cuentaBancaria, const std::string& telefono):
        Usuario(nickname, contrasenia, nombre, email),
        cuentaBancaria(cuentaBancaria), telefono(telefono), inmuebles(new List()) {
}

Propietario::~Propietario() {
    delete inmuebles;
}

std::string Propietario::getTipo() const { return "Propietario"; }
std::string Propietario::getCuentaBancaria() const { return cuentaBancaria; }
std::string Propietario::getTelefono() const { return telefono; }

void Propietario::agregarInmueble(Inmueble* inmueble) {
    inmuebles->add(inmueble);
}

void Propietario::quitarInmueble(Inmueble* inmueble) {
    inmuebles->remove(inmueble);
}

ICollection* Propietario::getInmuebles() const {
    return inmuebles;
}
