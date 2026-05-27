#include "Agenda.h"

Agenda::Agenda(const DTFecha& fechaVisita, const std::string& formaContacto,
               Cliente* cliente, Publicacion* publicacion):
        fechaVisita(fechaVisita), formaContacto(formaContacto),
        cliente(cliente), publicacion(publicacion) {
}

DTFecha Agenda::getFechaVisita() const { return fechaVisita; }
std::string Agenda::getFormaContacto() const { return formaContacto; }
Cliente* Agenda::getCliente() const { return cliente; }
Publicacion* Agenda::getPublicacion() const { return publicacion; }
