#ifndef APARTAMENTO_H
#define APARTAMENTO_H

#include "Inmueble.h"

using namespace std;

class Apartamento : public Inmueble {
private:
    int piso;
    bool tieneAscensor;
    float gastosComunes;

public:
    Apartamento(int codigo, const string& direccion, int numeroPuerta,
                float superficie, int anioConstruccion, Propietario* propietario,
                int piso, bool tieneAscensor, float gastosComunes);

    string getTipo() const override;
    string getDetalle() const override;
};

#endif
