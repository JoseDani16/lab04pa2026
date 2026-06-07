#ifndef CASA_H
#define CASA_H

#include "Enums.h"
#include "Inmueble.h"

using namespace std;

class Casa : public Inmueble {
private:
    bool propiedadHorizontal;
    TipoTecho tipoTecho;

public:
    Casa(int codigo, const string& direccion, int numeroPuerta,
         float superficie, int anioConstruccion, Propietario* propietario,
         bool propiedadHorizontal, TipoTecho tipoTecho);

    string getTipo() const override;
    string getDetalle() const override;
};

#endif
