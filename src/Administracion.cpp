#include "Administracion.h"
#include "Publicacion.h"
#include "../ICollection/collections/List.h"
#include "../ICollection/interfaces/IIterator.h"

Administracion::Administracion(const DTFecha& fechaComienzo, Inmobiliaria* inmobiliaria,
                               Inmueble* inmueble):
        fechaComienzo(fechaComienzo), inmobiliaria(inmobiliaria), inmueble(inmueble),
        publicaciones(new List()) {
}

Administracion::~Administracion() {
    IIterator* it = publicaciones->getIterator();
    while (it->hasCurrent()) {
        Publicacion* publicacion = dynamic_cast<Publicacion*>(it->getCurrent());
        delete publicacion;
        it->next();
    }
    delete it;
    delete publicaciones;
}

DTFecha Administracion::getFechaComienzo() const { return fechaComienzo; }
Inmobiliaria* Administracion::getInmobiliaria() const { return inmobiliaria; }
Inmueble* Administracion::getInmueble() const { return inmueble; }

void Administracion::agregarPublicacion(Publicacion* publicacion) {
    publicaciones->add(publicacion);
}

void Administracion::quitarPublicacion(Publicacion* publicacion) {
    publicaciones->remove(publicacion);
}

ICollection* Administracion::getPublicaciones() const {
    return publicaciones;
}

Publicacion* Administracion::buscarPublicacionActivaPorTipo(int tipo) const {
    IIterator* it = publicaciones->getIterator();
    while (it->hasCurrent()) {
        Publicacion* publicacion = dynamic_cast<Publicacion*>(it->getCurrent());
        if (static_cast<int>(publicacion->getTipo()) == tipo && publicacion->estaActiva()) {
            delete it;
            return publicacion;
        }
        it->next();
    }
    delete it;
    return nullptr;
}
