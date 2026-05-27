#ifndef SISTEMA_H
#define SISTEMA_H

#include "ISistema.h"
#include "Usuario.h"
#include "Inmueble.h"
#include "Inmobiliaria.h"
#include "Propietario.h"
#include "Cliente.h"
#include "Administracion.h"
#include "Publicacion.h"
#include "../ICollection/interfaces/IDictionary.h"

class Sistema : public ISistema {
private:
    static Sistema* instance;

    IDictionary* usuarios;
    IDictionary* inmuebles;
    int proxCodigoInmueble;
    int proxCodigoPublicacion;
    DTFecha fechaSistema;

    Sistema();

    bool existeNickname(const std::string& nickname) const;
    Usuario* buscarUsuario(const std::string& nickname) const;
    Cliente* buscarCliente(const std::string& nickname) const;
    Propietario* buscarPropietario(const std::string& nickname) const;
    Inmobiliaria* buscarInmobiliaria(const std::string& nickname) const;
    Inmueble* buscarInmueble(int codigo) const;
    Administracion* buscarAdministracion(Inmobiliaria* inmobiliaria, Inmueble* inmueble) const;
    Publicacion* buscarPublicacion(int codigoPublicacion) const;

    void limpiarDatos();
    void borrarPublicacion(Publicacion* publicacion);
    void borrarAdministracion(Administracion* administracion);

public:
    static Sistema* getInstance();
    ~Sistema();

    void setFechaSistema(const DTFecha& fecha) override;
    DTFecha getFechaSistema() const override;

    bool altaCliente(const std::string& nickname, const std::string& contrasenia,
                     const std::string& nombre, const std::string& email,
                     const std::string& apellido, const std::string& documento,
                     std::string& error) override;

    bool altaPropietario(const std::string& nickname, const std::string& contrasenia,
                         const std::string& nombre, const std::string& email,
                         const std::string& cuentaBancaria, const std::string& telefono,
                         std::string& error) override;

    bool altaInmobiliaria(const std::string& nickname, const std::string& contrasenia,
                          const std::string& nombre, const std::string& email,
                          const std::string& direccion, const std::string& telefono,
                          const std::string& url, std::string& error) override;

    bool altaCasa(const std::string& nickPropietario, const std::string& direccion,
                  int numeroPuerta, float superficie, int anioConstruccion,
                  bool propiedadHorizontal, TipoTecho tipoTecho,
                  int& codigoGenerado, std::string& error) override;

    bool altaApartamento(const std::string& nickPropietario, const std::string& direccion,
                         int numeroPuerta, float superficie, int anioConstruccion,
                         int piso, bool tieneAscensor, float gastosComunes,
                         int& codigoGenerado, std::string& error) override;

    bool representarPropietario(const std::string& nickInmobiliaria,
                                const std::string& nickPropietario,
                                std::string& error) override;

    bool altaAdministracion(const std::string& nickInmobiliaria, int codigoInmueble,
                            std::string& error) override;

    bool altaPublicacion(const std::string& nickInmobiliaria, int codigoInmueble,
                         TipoPublicacion tipo, const std::string& texto, float precio,
                         int& codigoGenerado, std::string& error) override;

    bool altaAgenda(const std::string& nickCliente, int codigoPublicacion,
                    const DTFecha& fechaVisita, const std::string& formaContacto,
                    std::string& error) override;

    std::vector<std::string> listarUsuarios() const override;
    std::vector<std::string> listarInmuebles() const override;
    std::vector<std::string> listarInmobiliarias() const override;
    std::vector<std::string> listarPropietarios() const override;
    std::vector<std::string> listarInmueblesAdministrables(const std::string& nickInmobiliaria) const override;
    std::vector<std::string> listarInmueblesAdministrados(const std::string& nickInmobiliaria) const override;
    std::vector<std::string> consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                    float precioMax, FiltroTipoInmueble filtro) const override;
    std::string detalleInmueble(int codigoInmueble) const override;

    bool eliminarInmueble(int codigoInmueble, std::string& error) override;
    void cargarDatosPrueba() override;
};

#endif
