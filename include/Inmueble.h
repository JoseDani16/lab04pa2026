#ifndef INMUEBLE_H
#define INMUEBLE_H

#include <string>
#include "../ICollection/interfaces/ICollectible.h"
#include "../ICollection/interfaces/ICollection.h"

class Administracion;
class Propietario;

class Inmueble : public ICollectible {
private:
    int codigo;
    std::string direccion;
    int numeroPuerta;
    float superficie;
    int anioConstruccion;
    Propietario* propietario;
    ICollection* administraciones;

public:
    Inmueble(int codigo, const std::string& direccion, int numeroPuerta,
             float superficie, int anioConstruccion, Propietario* propietario);
    virtual ~Inmueble();

    int getCodigo() const;
    std::string getDireccion() const;
    int getNumeroPuerta() const;
    float getSuperficie() const;
    int getAnioConstruccion() const;
    Propietario* getPropietario() const;

    void agregarAdministracion(Administracion* administracion);
    void quitarAdministracion(Administracion* administracion);
    ICollection* getAdministraciones() const;

    virtual std::string getTipo() const = 0;
    virtual std::string getDetalle() const;
};

#endif
