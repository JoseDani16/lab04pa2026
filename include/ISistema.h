#ifndef ISISTEMA_H
#define ISISTEMA_H

#include "DTFecha.h"
#include "Enums.h"
#include <string>
#include <vector>

class ISistema {
public:
    virtual ~ISistema();

    virtual void setFechaSistema(const DTFecha& fecha) = 0;
    virtual DTFecha getFechaSistema() const = 0;

    virtual bool altaCliente(const std::string& nickname, const std::string& contrasenia,
                             const std::string& nombre, const std::string& email,
                             const std::string& apellido, const std::string& documento,
                             std::string& error) = 0;

    virtual bool altaPropietario(const std::string& nickname, const std::string& contrasenia,
                                 const std::string& nombre, const std::string& email,
                                 const std::string& cuentaBancaria, const std::string& telefono,
                                 std::string& error) = 0;

    virtual bool altaInmobiliaria(const std::string& nickname, const std::string& contrasenia,
                                  const std::string& nombre, const std::string& email,
                                  const std::string& direccion, const std::string& telefono,
                                  const std::string& url, std::string& error) = 0;

    virtual bool altaCasa(const std::string& nickPropietario, const std::string& direccion,
                          int numeroPuerta, float superficie, int anioConstruccion,
                          bool propiedadHorizontal, TipoTecho tipoTecho,
                          int& codigoGenerado, std::string& error) = 0;

    virtual bool altaApartamento(const std::string& nickPropietario, const std::string& direccion,
                                 int numeroPuerta, float superficie, int anioConstruccion,
                                 int piso, bool tieneAscensor, float gastosComunes,
                                 int& codigoGenerado, std::string& error) = 0;

    virtual bool representarPropietario(const std::string& nickInmobiliaria,
                                        const std::string& nickPropietario,
                                        std::string& error) = 0;

    virtual bool altaAdministracion(const std::string& nickInmobiliaria, int codigoInmueble,
                                    std::string& error) = 0;

    virtual bool altaPublicacion(const std::string& nickInmobiliaria, int codigoInmueble,
                                 TipoPublicacion tipo, const std::string& texto, float precio,
                                 int& codigoGenerado, std::string& error) = 0;

    virtual bool altaAgenda(const std::string& nickCliente, int codigoPublicacion,
                            const DTFecha& fechaVisita, const std::string& formaContacto,
                            std::string& error) = 0;

    virtual std::vector<std::string> listarUsuarios() const = 0;
    virtual std::vector<std::string> listarInmuebles() const = 0;
    virtual std::vector<std::string> listarInmobiliarias() const = 0;
    virtual std::vector<std::string> listarPropietarios() const = 0;
    virtual std::vector<std::string> listarInmueblesAdministrables(const std::string& nickInmobiliaria) const = 0;
    virtual std::vector<std::string> listarInmueblesAdministrados(const std::string& nickInmobiliaria) const = 0;
    virtual std::vector<std::string> consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                            float precioMax, FiltroTipoInmueble filtro) const = 0;
    virtual std::string detalleInmueble(int codigoInmueble) const = 0;

    virtual bool eliminarInmueble(int codigoInmueble, std::string& error) = 0;
    virtual void cargarDatosPrueba() = 0;
};

#endif
