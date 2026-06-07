#ifndef PROPIETARIO_H
#define PROPIETARIO_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

using namespace std;

class Inmueble;

class Propietario : public Usuario {
private:
    string cuentaBancaria;
    string telefono;
    ICollection* inmuebles;

public:
    Propietario(const string& nickname, const string& contrasenia,
                const string& nombre, const string& email,
                const string& cuentaBancaria, const string& telefono);
    ~Propietario();

    string getTipo() const override;
    string getCuentaBancaria() const;
    string getTelefono() const;

    void agregarInmueble(Inmueble* inmueble);
    void quitarInmueble(Inmueble* inmueble);
    ICollection* getInmuebles() const;
};

#endif
