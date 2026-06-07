#ifndef INMOBILIARIA_H
#define INMOBILIARIA_H

#include "Usuario.h"
#include <string>
#include "../ICollection/interfaces/ICollection.h"

using namespace std;

class Administracion;
class Propietario;

class Inmobiliaria : public Usuario {
private:
    string direccion;
    string telefono;
    string url;
    ICollection* propietariosRepresentados;
    ICollection* administraciones;

public:
    Inmobiliaria(const string& nickname, const string& contrasenia,
                 const string& nombre, const string& email,
                 const string& direccion, const string& telefono,
                 const string& url);
    ~Inmobiliaria();

    string getTipo() const override;
    string getDireccionContacto() const;
    string getTelefono() const;
    string getUrl() const;

    void agregarPropietarioRepresentado(Propietario* propietario);
    bool representaA(Propietario* propietario) const;
    ICollection* getPropietariosRepresentados() const;

    void agregarAdministracion(Administracion* administracion);
    void quitarAdministracion(Administracion* administracion);
    ICollection* getAdministraciones() const;
};

#endif
