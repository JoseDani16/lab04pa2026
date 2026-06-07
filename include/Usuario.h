#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "../ICollection/interfaces/ICollectible.h"

using namespace std;

class Usuario : public ICollectible {
private:
    string nickname;
    string contrasenia;
    string nombre;
    string email;

public:
    Usuario(const string& nickname, const string& contrasenia,
            const string& nombre, const string& email);
    virtual ~Usuario();

    string getNickname() const;
    string getNombre() const;
    string getEmail() const;

    virtual string getTipo() const = 0;
};

#endif
