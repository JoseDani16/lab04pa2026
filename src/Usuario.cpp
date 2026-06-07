#include "Usuario.h"

using namespace std;

Usuario::Usuario(const string& nickname, const string& contrasenia,
                 const string& nombre, const string& email):
        nickname(nickname), contrasenia(contrasenia), nombre(nombre), email(email) {
}

Usuario::~Usuario() {
}

string Usuario::getNickname() const { return nickname; }
string Usuario::getNombre() const { return nombre; }
string Usuario::getEmail() const { return email; }
