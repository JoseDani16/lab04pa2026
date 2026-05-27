#ifndef ENUMS_H
#define ENUMS_H

#include <string>

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

std::string tipoPublicacionToString(TipoPublicacion tipo);
std::string tipoTechoToString(TipoTecho tipo);

#endif
