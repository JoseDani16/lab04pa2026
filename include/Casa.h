#ifndef CASA_H
#define CASA_H

#include "Enums.h"
#include "Inmueble.h"

class Casa : public Inmueble {
private:
    bool propiedadHorizontal;
    TipoTecho tipoTecho;

public:
    Casa(int codigo, const std::string& direccion, int numeroPuerta,
         float superficie, int anioConstruccion, Propietario* propietario,
         bool propiedadHorizontal, TipoTecho tipoTecho);

    std::string getTipo() const override;
    std::string getDetalle() const override;
};

#endif
