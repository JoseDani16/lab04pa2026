#include "Inmueble.h"
#include "Propietario.h"
#include "Administracion.h"
#include "../ICollection/collections/List.h"
#include <sstream>

Inmueble::Inmueble(int codigo, const std::string& direccion, int numeroPuerta,
                   float superficie, int anioConstruccion, Propietario* propietario):
        codigo(codigo), direccion(direccion), numeroPuerta(numeroPuerta),
        superficie(superficie), anioConstruccion(anioConstruccion), propietario(propietario),
        administraciones(new List()) {
}

Inmueble::~Inmueble() {
    delete administraciones;
}

int Inmueble::getCodigo() const { return codigo; }
std::string Inmueble::getDireccion() const { return direccion; }
int Inmueble::getNumeroPuerta() const { return numeroPuerta; }
float Inmueble::getSuperficie() const { return superficie; }
int Inmueble::getAnioConstruccion() const { return anioConstruccion; }
Propietario* Inmueble::getPropietario() const { return propietario; }

void Inmueble::agregarAdministracion(Administracion* administracion) {
    administraciones->add(administracion);
}

void Inmueble::quitarAdministracion(Administracion* administracion) {
    administraciones->remove(administracion);
}

ICollection* Inmueble::getAdministraciones() const {
    return administraciones;
}

std::string Inmueble::getDetalle() const {
    std::ostringstream oss;
    oss << "Codigo: " << codigo << "\n"
        << "Tipo: " << getTipo() << "\n"
        << "Direccion: " << direccion << " " << numeroPuerta << "\n"
        << "Superficie: " << superficie << "\n"
        << "Anio construccion: " << anioConstruccion;
    if (propietario != nullptr) {
        oss << "\nPropietario: " << propietario->getNombre() << " (" << propietario->getNickname() << ")";
    }
    return oss.str();
}
