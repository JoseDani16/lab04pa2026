#include "DTFecha.h"
#include <sstream>

DTFecha::DTFecha(): dia(1), mes(1), anio(2026) {
}

DTFecha::DTFecha(int dia, int mes, int anio): dia(dia), mes(mes), anio(anio) {
}

int DTFecha::getDia() const { return dia; }
int DTFecha::getMes() const { return mes; }
int DTFecha::getAnio() const { return anio; }

std::string DTFecha::toString() const {
    std::ostringstream oss;
    oss << dia << "/" << mes << "/" << anio;
    return oss.str();
}

bool DTFecha::operator<(const DTFecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool DTFecha::operator==(const DTFecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}
