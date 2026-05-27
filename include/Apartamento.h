#ifndef APARTAMENTO_H
#define APARTAMENTO_H

#include "Inmueble.h"

class Apartamento : public Inmueble {
private:
    int piso;
    bool tieneAscensor;
    float gastosComunes;

public:
    Apartamento(int codigo, const std::string& direccion, int numeroPuerta,
                float superficie, int anioConstruccion, Propietario* propietario,
                int piso, bool tieneAscensor, float gastosComunes);

    std::string getTipo() const override;
    std::string getDetalle() const override;
};

#endif
