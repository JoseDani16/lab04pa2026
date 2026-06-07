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

using namespace std;

class Sistema : public ISistema {
private:
    static Sistema* instance;

    IDictionary* usuarios;
    IDictionary* inmuebles;
    int proxCodigoInmueble;
    int proxCodigoPublicacion;
    DTFecha fechaSistema;

    Sistema();

    bool existeNickname(const string& nickname) const;
    Usuario* buscarUsuario(const string& nickname) const;
    Cliente* buscarCliente(const string& nickname) const;
    Propietario* buscarPropietario(const string& nickname) const;
    Inmobiliaria* buscarInmobiliaria(const string& nickname) const;
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
    bool existeUsuario(const string& nickname) const override;

    Status altaCliente(const string& nickname, const string& contrasenia,
                       const string& nombre, const string& email,
                       const string& apellido, const string& documento,
                       string& error) override;

    Status altaPropietario(const string& nickname, const string& contrasenia,
                           const string& nombre, const string& email,
                           const string& cuentaBancaria, const string& telefono,
                           string& error) override;

    Status altaInmobiliaria(const string& nickname, const string& contrasenia,
                            const string& nombre, const string& email,
                            const string& direccion, const string& telefono,
                            const string& url, string& error) override;

    Status altaCasa(const string& nickPropietario, const string& direccion,
                    int numeroPuerta, float superficie, int anioConstruccion,
                    bool propiedadHorizontal, TipoTecho tipoTecho,
                    int& codigoGenerado, string& error) override;

    Status altaApartamento(const string& nickPropietario, const string& direccion,
                           int numeroPuerta, float superficie, int anioConstruccion,
                           int piso, bool tieneAscensor, float gastosComunes,
                           int& codigoGenerado, string& error) override;

    Status representarPropietario(const string& nickInmobiliaria,
                                  const string& nickPropietario,
                                  string& error) override;

    Status altaAdministracion(const string& nickInmobiliaria, int codigoInmueble,
                              string& error) override;

    Status altaPublicacion(const string& nickInmobiliaria, int codigoInmueble,
                           TipoPublicacion tipo, const string& texto, float precio,
                           int& codigoGenerado, string& error) override;

    Status altaAgenda(const string& nickCliente, int codigoPublicacion,
                      const DTFecha& fechaVisita, const string& formaContacto,
                      string& error) override;

    vector<string> listarUsuarios() const override;
    vector<string> listarInmuebles() const override;
    vector<string> listarInmobiliarias() const override;
    vector<string> listarPropietarios() const override;
    vector<string> listarInmueblesAdministrables(const string& nickInmobiliaria) const override;
    vector<string> listarInmueblesAdministrados(const string& nickInmobiliaria) const override;
    vector<string> consultarPublicaciones(TipoPublicacion tipo, float precioMin,
                                                    float precioMax, FiltroTipoInmueble filtro) const override;
    string detalleInmueble(int codigoInmueble) const override;

    Status eliminarInmueble(int codigoInmueble, string& error) override;
    void cargarDatosPrueba() override;
};

#endif
