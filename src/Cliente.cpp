#include "Cliente.h"
#include "Agenda.h"
#include "../ICollection/collections/List.h"

using namespace std;

Cliente::Cliente(const string& nickname, const string& contrasenia,
                 const string& nombre, const string& email,
                 const string& apellido, const string& documento):
        Usuario(nickname, contrasenia, nombre, email), apellido(apellido), documento(documento),
        agendas(new List()) {
}

Cliente::~Cliente() {
    delete agendas;
}

string Cliente::getTipo() const { return "Cliente"; }
string Cliente::getApellido() const { return apellido; }
string Cliente::getDocumento() const { return documento; }

void Cliente::agregarAgenda(Agenda* agenda) {
    agendas->add(agenda);
}

void Cliente::quitarAgenda(Agenda* agenda) {
    agendas->remove(agenda);
}

ICollection* Cliente::getAgendas() const {
    return agendas;
}
