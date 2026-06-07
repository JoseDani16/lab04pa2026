#ifndef ENUMS_H
#define ENUMS_H

#include <string>

using namespace std;

enum class TipoPublicacion {
    Venta,
    Alquiler
};

enum class TipoTecho {
    Liviano,
    DosAguas,
    Plano
};

enum class FiltroTipoInmueble {
    Todos,
    Casas,
    Apartamentos
};

string tipoPublicacionToString(TipoPublicacion tipo);
string tipoTechoToString(TipoTecho tipo);

#endif
