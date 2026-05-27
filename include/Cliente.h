#ifndef CLIENTE_H
#define CLIENTE_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

class Agenda;

class Cliente : public Usuario {
private:
    std::string apellido;
    std::string documento;
    ICollection* agendas;

public:
    Cliente(const std::string& nickname, const std::string& contrasenia,
            const std::string& nombre, const std::string& email,
            const std::string& apellido, const std::string& documento);
    ~Cliente();

    std::string getTipo() const override;
    std::string getApellido() const;
    std::string getDocumento() const;

    void agregarAgenda(Agenda* agenda);
    void quitarAgenda(Agenda* agenda);
    ICollection* getAgendas() const;
};

#endif
