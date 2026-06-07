#include "Casa.h"
#include <sstream>

using namespace std;

Casa::Casa(int codigo, const string& direccion, int numeroPuerta,
           float superficie, int anioConstruccion, Propietario* propietario,
           bool propiedadHorizontal, TipoTecho tipoTecho):
        Inmueble(codigo, direccion, numeroPuerta, superficie, anioConstruccion, propietario),
        propiedadHorizontal(propiedadHorizontal), tipoTecho(tipoTecho) {
}

string Casa::getTipo() const { return "Casa"; }

string Casa::getDetalle() const {
    ostringstream oss;
    oss << Inmueble::getDetalle() << "\n"
        << "Propiedad horizontal: " << (propiedadHorizontal ? "Si" : "No") << "\n"
        << "Tipo de techo: " << tipoTechoToString(tipoTecho);
    return oss.str();
}
