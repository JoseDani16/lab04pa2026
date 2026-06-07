#include "Enums.h"

using namespace std;

string tipoPublicacionToString(TipoPublicacion tipo) {
    return tipo == TipoPublicacion::Venta ? "Venta" : "Alquiler";
}

string tipoTechoToString(TipoTecho tipo) {
    switch (tipo) {
        case TipoTecho::Liviano: return "Liviano";
        case TipoTecho::DosAguas: return "Dos aguas";
        case TipoTecho::Plano: return "Plano";
    }
    return "Desconocido";
}
