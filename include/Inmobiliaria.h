#ifndef INMOBILIARIA_H
#define INMOBILIARIA_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

class Administracion;
class Propietario;

class Inmobiliaria : public Usuario {
private:
    std::string direccion;
    std::string telefono;
    std::string url;
    ICollection* propietariosRepresentados;
    ICollection* administraciones;

public:
    Inmobiliaria(const std::string& nickname, const std::string& contrasenia,
                 const std::string& nombre, const std::string& email,
                 const std::string& direccion, const std::string& telefono,
                 const std::string& url);
    ~Inmobiliaria();

    std::string getTipo() const override;
    std::string getDireccionContacto() const;
    std::string getTelefono() const;
    std::string getUrl() const;

    void agregarPropietarioRepresentado(Propietario* propietario);
    bool representaA(Propietario* propietario) const;
    ICollection* getPropietariosRepresentados() const;

    void agregarAdministracion(Administracion* administracion);
    void quitarAdministracion(Administracion* administracion);
    ICollection* getAdministraciones() const;
};

#endif
