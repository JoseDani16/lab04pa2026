#include "Apartamento.h"
#include <sstream>

Apartamento::Apartamento(int codigo, const std::string& direccion, int numeroPuerta,
                         float superficie, int anioConstruccion, Propietario* propietario,
                         int piso, bool tieneAscensor, float gastosComunes):
        Inmueble(codigo, direccion, numeroPuerta, superficie, anioConstruccion, propietario),
        piso(piso), tieneAscensor(tieneAscensor), gastosComunes(gastosComunes) {
}

std::string Apartamento::getTipo() const { return "Apartamento"; }

std::string Apartamento::getDetalle() const {
    std::ostringstream oss;
    oss << Inmueble::getDetalle() << "\n"
        << "Piso: " << piso << "\n"
        << "Ascensor: " << (tieneAscensor ? "Si" : "No") << "\n"
        << "Gastos comunes: " << gastosComunes;
    return oss.str();
}
