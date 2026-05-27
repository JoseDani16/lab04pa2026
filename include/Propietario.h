#ifndef PROPIETARIO_H
#define PROPIETARIO_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

class Inmueble;

class Propietario : public Usuario {
private:
    std::string cuentaBancaria;
    std::string telefono;
    ICollection* inmuebles;

public:
    Propietario(const std::string& nickname, const std::string& contrasenia,
                const std::string& nombre, const std::string& email,
                const std::string& cuentaBancaria, const std::string& telefono);
    ~Propietario();

    std::string getTipo() const override;
    std::string getCuentaBancaria() const;
    std::string getTelefono() const;

    void agregarInmueble(Inmueble* inmueble);
    void quitarInmueble(Inmueble* inmueble);
    ICollection* getInmuebles() const;
};

#endif
