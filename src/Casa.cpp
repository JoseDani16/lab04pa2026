#include "Casa.h"
#include <sstream>

Casa::Casa(int codigo, const std::string& direccion, int numeroPuerta,
           float superficie, int anioConstruccion, Propietario* propietario,
           bool propiedadHorizontal, TipoTecho tipoTecho):
        Inmueble(codigo, direccion, numeroPuerta, superficie, anioConstruccion, propietario),
        propiedadHorizontal(propiedadHorizontal), tipoTecho(tipoTecho) {
}

std::string Casa::getTipo() const { return "Casa"; }

std::string Casa::getDetalle() const {
    std::ostringstream oss;
    oss << Inmueble::getDetalle() << "\n"
        << "Propiedad horizontal: " << (propiedadHorizontal ? "Si" : "No") << "\n"
        << "Tipo de techo: " << tipoTechoToString(tipoTecho);
    return oss.str();
}
