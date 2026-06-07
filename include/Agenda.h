#ifndef AGENDA_H
#define AGENDA_H

#include "DTFecha.h"
#include <string>
#include "../ICollection/interfaces/ICollectible.h"

using namespace std;

class Cliente;
class Publicacion;

class Agenda : public ICollectible {
private:
    DTFecha fechaVisita;
    string formaContacto;
    Cliente* cliente;
    Publicacion* publicacion;

public:
    Agenda(const DTFecha& fechaVisita, const string& formaContacto,
           Cliente* cliente, Publicacion* publicacion);

    DTFecha getFechaVisita() const;
    string getFormaContacto() const;
    Cliente* getCliente() const;
    Publicacion* getPublicacion() const;
};

#endif
