#ifndef PUBLICACION_H
#define PUBLICACION_H

#include "DTFecha.h"
#include "Enums.h"
#include <string>
#include "../ICollection/interfaces/ICollectible.h"
#include "../ICollection/interfaces/ICollection.h"

using namespace std;

class Administracion;
class Agenda;

class Publicacion : public ICollectible {
private:
    int codigo;
    DTFecha fecha;
    TipoPublicacion tipo;
    string texto;
    float precio;
    bool activa;
    Administracion* administracion;
    ICollection* agendas;

public:
    Publicacion(int codigo, const DTFecha& fecha, TipoPublicacion tipo,
                const string& texto, float precio,
                Administracion* administracion);
    ~Publicacion();

    int getCodigo() const;
    DTFecha getFecha() const;
    TipoPublicacion getTipo() const;
    string getTexto() const;
    float getPrecio() const;
    bool estaActiva() const;
    Administracion* getAdministracion() const;

    void setActiva(bool activa);
    void agregarAgenda(Agenda* agenda);
    void quitarAgenda(Agenda* agenda);
    ICollection* getAgendas() const;
};

#endif
