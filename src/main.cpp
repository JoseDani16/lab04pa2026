#include "Factory.h"
#include <iostream>
#include <limits>

using namespace std;

static void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string leerLinea(const string& etiqueta) {
    string valor;
    cout << etiqueta;
    getline(cin >> ws, valor);
    return valor;
}

static int leerInt(const string& etiqueta) {
    int valor;
    while (true) {
        cout << etiqueta;
        if (cin >> valor) {
            limpiarEntrada();
            return valor;
        }
        cout << "Debe ingresar un numero valido." << endl;
        limpiarEntrada();
    }
    return valor;
}

static float leerFloat(const string& etiqueta) {
    float valor;
    while (true) {
        cout << etiqueta;
        if (cin >> valor) {
            limpiarEntrada();
            return valor;
        }
        cout << "Debe ingresar un numero valido." << endl;
        limpiarEntrada();
    }
    return valor;
}

static bool leerBool(const string& etiqueta) {
    char valor;
    while (true) {
        cout << etiqueta << " (s/n): ";
        cin >> valor;
        limpiarEntrada();
        if (valor == 's' || valor == 'S') return true;
        if (valor == 'n' || valor == 'N') return false;
        cout << "Debe responder s o n." << endl;
    }
}

static DTFecha leerFecha(const string& etiqueta) {
    cout << etiqueta << endl;
    int dia = leerInt("Dia: ");
    int mes = leerInt("Mes: ");
    int anio = leerInt("Anio: ");
    return DTFecha(dia, mes, anio);
}

static TipoPublicacion leerTipoPublicacion() {
    int opcion = leerInt("Tipo (1 Venta, 2 Alquiler): ");
    return opcion == 2 ? TipoPublicacion::Alquiler : TipoPublicacion::Venta;
}

static TipoTecho leerTipoTecho() {
    int opcion = leerInt("Tipo techo (1 Liviano, 2 Dos aguas, 3 Plano): ");
    if (opcion == 2) return TipoTecho::DosAguas;
    if (opcion == 3) return TipoTecho::Plano;
    return TipoTecho::Liviano;
}

static FiltroTipoInmueble leerFiltroTipoInmueble() {
    int opcion = leerInt("Filtro inmueble (1 Todos, 2 Casas, 3 Apartamentos): ");
    if (opcion == 2) return FiltroTipoInmueble::Casas;
    if (opcion == 3) return FiltroTipoInmueble::Apartamentos;
    return FiltroTipoInmueble::Todos;
}

static void imprimirLineas(const vector<string>& lineas) {
    if (lineas.empty()) {
        cout << "No hay datos para mostrar." << endl;
        return;
    }
    for (const string& linea : lineas) {
        cout << "- " << linea << endl;
    }
}

static void altaUsuario(ISistema* sistema) {
    string error;
    int tipo = leerInt("Tipo usuario (1 Cliente, 2 Propietario, 3 Inmobiliaria): ");
    string nickname = leerLinea("Nickname: ");
    string contrasenia = leerLinea("Contrasenia: ");
    string nombre = leerLinea("Nombre: ");
    string email = leerLinea("Email: ");
    bool ok = false;

    if (tipo == 1) {
        string apellido = leerLinea("Apellido: ");
        string documento = leerLinea("Documento: ");
        ok = sistema->altaCliente(nickname, contrasenia, nombre, email, apellido, documento, error);
    } else if (tipo == 2) {
        string cuenta = leerLinea("Cuenta bancaria: ");
        string telefono = leerLinea("Telefono: ");
        ok = sistema->altaPropietario(nickname, contrasenia, nombre, email, cuenta, telefono, error);
    } else {
        string direccion = leerLinea("Direccion: ");
        string telefono = leerLinea("Telefono: ");
        string url = leerLinea("URL: ");
        ok = sistema->altaInmobiliaria(nickname, contrasenia, nombre, email, direccion, telefono, url, error);
    }
    cout << (ok ? "Usuario creado." : error) << endl;
}

static void altaInmueble(ISistema* sistema) {
    string error;
    int codigo = 0;
    imprimirLineas(sistema->listarPropietarios());
    string nickPropietario = leerLinea("Nickname propietario: ");
    int tipo = leerInt("Tipo inmueble (1 Casa, 2 Apartamento): ");
    string direccion = leerLinea("Direccion: ");
    int puerta = leerInt("Numero de puerta: ");
    float superficie = leerFloat("Superficie: ");
    int anio = leerInt("Anio construccion: ");
    bool ok = false;

    if (tipo == 1) {
        bool ph = leerBool("Propiedad horizontal");
        TipoTecho techo = leerTipoTecho();
        ok = sistema->altaCasa(nickPropietario, direccion, puerta, superficie, anio, ph, techo, codigo, error);
    } else {
        int piso = leerInt("Piso: ");
        bool ascensor = leerBool("Tiene ascensor");
        float gastos = leerFloat("Gastos comunes: ");
        ok = sistema->altaApartamento(nickPropietario, direccion, puerta, superficie, anio,
                                      piso, ascensor, gastos, codigo, error);
    }
    if (ok) {
        cout << "Inmueble creado con codigo " << codigo << "." << endl;
    } else {
        cout << error << endl;
    }
}

static void representarPropietario(ISistema* sistema) {
    string error;
    cout << "Inmobiliarias:" << endl;
    imprimirLineas(sistema->listarInmobiliarias());
    cout << "Propietarios:" << endl;
    imprimirLineas(sistema->listarPropietarios());
    string nickInmo = leerLinea("Nickname inmobiliaria: ");
    string nickProp = leerLinea("Nickname propietario: ");
    bool ok = sistema->representarPropietario(nickInmo, nickProp, error);
    cout << (ok ? "Representacion creada." : error) << endl;
}

static void altaAdministracion(ISistema* sistema) {
    string error;
    imprimirLineas(sistema->listarInmobiliarias());
    string nickInmo = leerLinea("Nickname inmobiliaria: ");
    cout << "Inmuebles administrables:" << endl;
    vector<string> administrables = sistema->listarInmueblesAdministrables(nickInmo);
    imprimirLineas(administrables);
    if (administrables.empty()) {
        cout << "No se puede continuar: no hay inmuebles administrables para esa inmobiliaria." << endl;
        return;
    }
    int codigo = leerInt("Codigo inmueble: ");
    bool ok = sistema->altaAdministracion(nickInmo, codigo, error);
    cout << (ok ? "Administracion creada." : error) << endl;
}

static void altaPublicacion(ISistema* sistema) {
    string error;
    int codigoPublicacion = 0;
    imprimirLineas(sistema->listarInmobiliarias());
    string nickInmo = leerLinea("Nickname inmobiliaria: ");
    cout << "Inmuebles administrados:" << endl;
    vector<string> administrados = sistema->listarInmueblesAdministrados(nickInmo);
    imprimirLineas(administrados);
    if (administrados.empty()) {
        cout << "No se puede continuar: no hay inmuebles administrados para esa inmobiliaria." << endl;
        return;
    }
    int codigoInmueble = leerInt("Codigo inmueble: ");
    TipoPublicacion tipo = leerTipoPublicacion();
    string texto = leerLinea("Texto: ");
    float precio = leerFloat("Precio: ");
    bool ok = sistema->altaPublicacion(nickInmo, codigoInmueble, tipo, texto, precio,
                                       codigoPublicacion, error);
    if (ok) {
        cout << "Publicacion creada con codigo " << codigoPublicacion << "." << endl;
    } else {
        cout << error << endl;
    }
}

static void altaAgenda(ISistema* sistema) {
    string error;
    string nickCliente = leerLinea("Nickname cliente: ");
    int codigoPublicacion = leerInt("Codigo publicacion: ");
    DTFecha fecha = leerFecha("Fecha visita");
    string contacto = leerLinea("Forma de contacto: ");
    bool ok = sistema->altaAgenda(nickCliente, codigoPublicacion, fecha, contacto, error);
    cout << (ok ? "Agenda creada." : error) << endl;
}

static void consultaPublicaciones(ISistema* sistema) {
    TipoPublicacion tipo = leerTipoPublicacion();
    float min = leerFloat("Precio minimo: ");
    float max = leerFloat("Precio maximo: ");
    FiltroTipoInmueble filtro = leerFiltroTipoInmueble();
    imprimirLineas(sistema->consultarPublicaciones(tipo, min, max, filtro));
    if (leerBool("Ver detalle de un inmueble")) {
        int codigo = leerInt("Codigo inmueble: ");
        cout << sistema->detalleInmueble(codigo) << endl;
    }
}

static void eliminarInmueble(ISistema* sistema) {
    string error;
    imprimirLineas(sistema->listarInmuebles());
    int codigo = leerInt("Codigo inmueble: ");
    string detalle = sistema->detalleInmueble(codigo);
    cout << detalle << endl;
    if (detalle == "No existe un inmueble con ese codigo.") {
        cout << "No se puede continuar con la eliminacion." << endl;
        return;
    }
    if (!leerBool("Confirma eliminar")) {
        cout << "Operacion cancelada." << endl;
        return;
    }
    bool ok = sistema->eliminarInmueble(codigo, error);
    cout << (ok ? "Inmueble eliminado." : error) << endl;
}

int main() {
    ISistema* sistema = Factory::getSistema();
    int opcion = -1;

    while (opcion != 0) {
        cout << "\n=== Plataforma de Bienes Raices ===" << endl;
        cout << "1. Cargar datos de prueba" << endl;
        cout << "2. Alta de usuario" << endl;
        cout << "3. Alta de inmueble" << endl;
        cout << "4. Representar propietario" << endl;
        cout << "5. Alta de administracion de propiedad" << endl;
        cout << "6. Alta de publicacion" << endl;
        cout << "7. Alta de agenda" << endl;
        cout << "8. Consulta de publicaciones" << endl;
        cout << "9. Eliminar inmueble" << endl;
        cout << "10. Listar usuarios" << endl;
        cout << "11. Listar inmuebles" << endl;
        cout << "0. Salir" << endl;
        opcion = leerInt("Opcion: ");

        switch (opcion) {
            case 1:
                sistema->cargarDatosPrueba();
                cout << "Datos de prueba cargados." << endl;
                break;
            case 2: altaUsuario(sistema); break;
            case 3: altaInmueble(sistema); break;
            case 4: representarPropietario(sistema); break;
            case 5: altaAdministracion(sistema); break;
            case 6: altaPublicacion(sistema); break;
            case 7: altaAgenda(sistema); break;
            case 8: consultaPublicaciones(sistema); break;
            case 9: eliminarInmueble(sistema); break;
            case 10: imprimirLineas(sistema->listarUsuarios()); break;
            case 11: imprimirLineas(sistema->listarInmuebles()); break;
            case 0: cout << "Fin." << endl; break;
            default: cout << "Opcion invalida." << endl; break;
        }
    }

    return 0;
}
