#include "Publicacion.h"
#include "Agenda.h"
#include "Cliente.h"
#include "../ICollection/collections/List.h"
#include "../ICollection/interfaces/IIterator.h"

using namespace std;

Publicacion::Publicacion(int codigo, const DTFecha& fecha, TipoPublicacion tipo,
                         const string& texto, float precio,
                         Administracion* administracion):
        codigo(codigo), fecha(fecha), tipo(tipo), texto(texto), precio(precio),
        activa(true), administracion(administracion), agendas(new List()) {
}

Publicacion::~Publicacion() {
    IIterator* it = agendas->getIterator();
    while (it->hasCurrent()) {
        Agenda* agenda = dynamic_cast<Agenda*>(it->getCurrent());
        if (agenda->getCliente() != nullptr) {
            agenda->getCliente()->quitarAgenda(agenda);
        }
        delete agenda;
        it->next();
    }
    delete it;
    delete agendas;
}

int Publicacion::getCodigo() const { return codigo; }
DTFecha Publicacion::getFecha() const { return fecha; }
TipoPublicacion Publicacion::getTipo() const { return tipo; }
string Publicacion::getTexto() const { return texto; }
float Publicacion::getPrecio() const { return precio; }
bool Publicacion::estaActiva() const { return activa; }
Administracion* Publicacion::getAdministracion() const { return administracion; }

void Publicacion::setActiva(bool activa) {
    this->activa = activa;
}

void Publicacion::agregarAgenda(Agenda* agenda) {
    agendas->add(agenda);
}

void Publicacion::quitarAgenda(Agenda* agenda) {
    agendas->remove(agenda);
}

ICollection* Publicacion::getAgendas() const {
    return agendas;
}
