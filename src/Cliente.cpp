#include "Cliente.h"
#include "Agenda.h"
#include "../ICollection/collections/List.h"

Cliente::Cliente(const std::string& nickname, const std::string& contrasenia,
                 const std::string& nombre, const std::string& email,
                 const std::string& apellido, const std::string& documento):
        Usuario(nickname, contrasenia, nombre, email), apellido(apellido), documento(documento),
        agendas(new List()) {
}

Cliente::~Cliente() {
    delete agendas;
}

std::string Cliente::getTipo() const { return "Cliente"; }
std::string Cliente::getApellido() const { return apellido; }
std::string Cliente::getDocumento() const { return documento; }

void Cliente::agregarAgenda(Agenda* agenda) {
    agendas->add(agenda);
}

void Cliente::quitarAgenda(Agenda* agenda) {
    agendas->remove(agenda);
}

ICollection* Cliente::getAgendas() const {
    return agendas;
}
