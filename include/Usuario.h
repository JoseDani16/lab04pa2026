#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "../ICollection/interfaces/ICollectible.h"

class Usuario : public ICollectible {
private:
    std::string nickname;
    std::string contrasenia;
    std::string nombre;
    std::string email;

public:
    Usuario(const std::string& nickname, const std::string& contrasenia,
            const std::string& nombre, const std::string& email);
    virtual ~Usuario();

    std::string getNickname() const;
    std::string getNombre() const;
    std::string getEmail() const;

    virtual std::string getTipo() const = 0;
};

#endif
