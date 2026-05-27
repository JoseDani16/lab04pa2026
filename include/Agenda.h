#ifndef AGENDA_H
#define AGENDA_H

#include "DTFecha.h"
#include <string>
#include "../ICollection/interfaces/ICollectible.h"

class Cliente;
class Publicacion;

class Agenda : public ICollectible {
private:
    DTFecha fechaVisita;
    std::string formaContacto;
    Cliente* cliente;
    Publicacion* publicacion;

public:
    Agenda(const DTFecha& fechaVisita, const std::string& formaContacto,
           Cliente* cliente, Publicacion* publicacion);

    DTFecha getFechaVisita() const;
    std::string getFormaContacto() const;
    Cliente* getCliente() const;
    Publicacion* getPublicacion() const;
};

#endif
