#ifndef ADMINISTRACION_H
#define ADMINISTRACION_H

#include "DTFecha.h"
#include "../ICollection/interfaces/ICollectible.h"
#include "../ICollection/interfaces/ICollection.h"

class Inmobiliaria;
class Inmueble;
class Publicacion;

class Administracion : public ICollectible {
private:
    DTFecha fechaComienzo;
    Inmobiliaria* inmobiliaria;
    Inmueble* inmueble;
    ICollection* publicaciones;

public:
    Administracion(const DTFecha& fechaComienzo, Inmobiliaria* inmobiliaria,
                   Inmueble* inmueble);
    ~Administracion();

    DTFecha getFechaComienzo() const;
    Inmobiliaria* getInmobiliaria() const;
    Inmueble* getInmueble() const;

    void agregarPublicacion(Publicacion* publicacion);
    void quitarPublicacion(Publicacion* publicacion);
    ICollection* getPublicaciones() const;
    Publicacion* buscarPublicacionActivaPorTipo(int tipo) const;
};

#endif
