#include "Agenda.h"

using namespace std;

Agenda::Agenda(const DTFecha& fechaVisita, const string& formaContacto,
               Cliente* cliente, Publicacion* publicacion):
        fechaVisita(fechaVisita), formaContacto(formaContacto),
        cliente(cliente), publicacion(publicacion) {
}

DTFecha Agenda::getFechaVisita() const { return fechaVisita; }
string Agenda::getFormaContacto() const { return formaContacto; }
Cliente* Agenda::getCliente() const { return cliente; }
Publicacion* Agenda::getPublicacion() const { return publicacion; }
