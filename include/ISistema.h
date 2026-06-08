#ifndef ISISTEMA_H
#define ISISTEMA_H

#include "DTFecha.h"
#include "Enums.h"
#include "Status.h"
#include <string>
#include <vector>

using namespace std;

class ISistema {
public:
    virtual ~ISistema();

    virtual void setFechaSistema(const DTFecha& fecha) = 0;
    virtual DTFecha getFechaSistema() const = 0;
    virtual bool existeUsuario(const string& nickname) const = 0;
    virtual bool existeCliente(const string& nickname) const = 0;
    virtual bool existePropietario(const string& nickname) const = 0;
    virtual bool existeInmobiliaria(const string& nickname) const = 0;
    virtual bool existeInmueble(int codigo) const = 0;
    virtual bool existePublicacion(int codigo) const = 0;
    virtual bool existePublicacionActiva(int codigo) const = 0;

    virtual Status altaCliente(const string& nickname, const string& contrasenia,
                               const string& nombre, const string& email,
                               const string& apellido, const string& documento,
                               string& error) = 0;

    virtual Status altaPropietario(const string& nickname, const string& contrasenia,
                                   const string& nombre, const string& email,
                                   const string& cuentaBancaria, const string& telefono,
                                   string& error) = 0;

    virtual Status altaInmobiliaria(const string& nickname, const string& contrasenia,
                                    const string& nombre, const string& email,
                                    const string& direccion, const string& telefono,
                                    const string& url, string& error) = 0;

    virtual Status altaCasa(const string& nickPropietario, const string& direccion,
                            int numeroPuerta, float superficie, int anioConstruccion,
                            bool propiedadHorizontal, TipoTecho tipoTecho,
                            int& codigoGenerado, string& error) = 0;

    virtual Status altaApartamento(const string& nickPropietario, const string& direccion,
                                   int numeroPuerta, float superficie, int anioConstruccion,
                                   int piso, bool tieneAscensor, float gastosComunes,
                                   int& codigoGenerado, string& error) = 0;

    virtual Status representarPropietario(const string& nickInmobiliaria,
                                          const string& nickPropietario,
                                          string& error) = 0;

    virtual Status altaAdministracion(const string& nickInmobiliaria, int codigoInmueble,
                                      string& error) = 0;

    virtual Status altaPublicacion(const string& nickInmobiliaria, int codigoInmueble,
                                   TipoPublicacion tipo, const string& texto, float precio,
                                   int& codigoGenerado, string& error) = 0;

    virtual Status altaAgenda(const string& nickCliente, int codigoPublicacion,
                              const DTFecha& fechaVisita, const string& formaContacto,
                              string& error) = 0;

    virtual vector<string> listarUsuarios() const = 0;
    virtual vector<string> listarInmuebles() const = 0;
    virtual vector<string> listarInmobiliarias() const = 0;
    virtual vector<string> listarPropietarios() const = 0;
    virtual vector<string> listarInmueblesAdministrables(const string& nickInmobiliaria) const = 0;
    virtual vector<string> listarInmueblesAdministrados(const string& nickInmobiliaria) const = 0;
    virtual vector<string> consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                            float precioMax, FiltroTipoInmueble filtro) const = 0;
    virtual string detalleInmueble(int codigoInmueble) const = 0;

    virtual Status eliminarInmueble(int codigoInmueble, string& error) = 0;
    virtual Status cargarDatosPrueba(string& error) = 0;
};

#endif
