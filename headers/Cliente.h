#ifndef CLIENTE_H
#define CLIENTE_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

using namespace std;

class Agenda;

class Cliente : public Usuario {
private:
    string apellido;
    string documento;
    ICollection* agendas;

public:
    Cliente(const string& nickname, const string& contrasenia,
            const string& nombre, const string& email,
            const string& apellido, const string& documento);
    ~Cliente();

    string getTipo() const override;
    string getApellido() const;
    string getDocumento() const;

    void agregarAgenda(Agenda* agenda);
    void quitarAgenda(Agenda* agenda);
    ICollection* getAgendas() const;
};

#endif
