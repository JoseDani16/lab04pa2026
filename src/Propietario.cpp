#include "Propietario.h"
#include "Inmueble.h"
#include "../ICollection/collections/List.h"

using namespace std;

Propietario::Propietario(const string& nickname, const string& contrasenia,
                         const string& nombre, const string& email,
                         const string& cuentaBancaria, const string& telefono):
        Usuario(nickname, contrasenia, nombre, email),
        cuentaBancaria(cuentaBancaria), telefono(telefono), inmuebles(new List()) {
}

Propietario::~Propietario() {
    delete inmuebles;
}

string Propietario::getTipo() const { return "Propietario"; }
string Propietario::getCuentaBancaria() const { return cuentaBancaria; }
string Propietario::getTelefono() const { return telefono; }

void Propietario::agregarInmueble(Inmueble* inmueble) {
    inmuebles->add(inmueble);
}

void Propietario::quitarInmueble(Inmueble* inmueble) {
    inmuebles->remove(inmueble);
}

ICollection* Propietario::getInmuebles() const {
    return inmuebles;
}
