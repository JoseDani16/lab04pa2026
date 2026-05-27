#include "Usuario.h"

Usuario::Usuario(const std::string& nickname, const std::string& contrasenia,
                 const std::string& nombre, const std::string& email):
        nickname(nickname), contrasenia(contrasenia), nombre(nombre), email(email) {
}

Usuario::~Usuario() {
}

std::string Usuario::getNickname() const { return nickname; }
std::string Usuario::getNombre() const { return nombre; }
std::string Usuario::getEmail() const { return email; }
