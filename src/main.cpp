#include "Factory.h"
#include <exception>
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

static void esperarContinuar() {
    cout << "Presione Enter para continuar...";
    cin.get();
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

static bool leerIntEnRango(const string& etiqueta, int min, int max, int& valor) {
    while (true) {
        valor = leerInt(etiqueta + " (0 Cancelar): ");
        if (valor == 0) return false;
        if (valor >= min && valor <= max) return true;
        cout << "Opcion invalida. Debe ingresar un valor entre " << min << " y " << max << "." << endl;
    }
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

static bool leerContraseniaValida(string& contrasenia) {
    while (true) {
        contrasenia = leerLinea("Contrasenia: ");
        if (contrasenia.size() >= 6) return true;
        cout << "La contrasenia debe tener al menos 6 caracteres." << endl;
        if (!leerBool("Desea reintentar la contrasenia")) return false;
    }
}

static bool leerNicknameDisponible(ISistema* sistema, string& nickname) {
    while (true) {
        nickname = leerLinea("Nickname: ");
        if (!sistema->existeUsuario(nickname)) return true;
        cout << "Ya existe un usuario con ese nickname." << endl;
        if (!leerBool("Desea ingresar otro nickname")) return false;
    }
}

static DTFecha leerFecha(const string& etiqueta) {
    cout << etiqueta << endl;
    int dia = leerInt("Dia: ");
    int mes = leerInt("Mes: ");
    int anio = leerInt("Anio: ");
    return DTFecha(dia, mes, anio);
}

static bool leerTipoPublicacion(TipoPublicacion& tipo) {
    int opcion;
    if (!leerIntEnRango("Tipo (1 Venta, 2 Alquiler)", 1, 2, opcion)) return false;
    tipo = opcion == 2 ? TipoPublicacion::Alquiler : TipoPublicacion::Venta;
    return true;
}

static bool leerTipoTecho(TipoTecho& tipo) {
    int opcion;
    if (!leerIntEnRango("Tipo techo (1 Liviano, 2 Dos aguas, 3 Plano)", 1, 3, opcion)) return false;
    if (opcion == 2) tipo = TipoTecho::DosAguas;
    else if (opcion == 3) tipo = TipoTecho::Plano;
    else tipo = TipoTecho::Liviano;
    return true;
}

static bool leerFiltroTipoInmueble(FiltroTipoInmueble& filtro) {
    int opcion;
    if (!leerIntEnRango("Filtro inmueble (1 Todos, 2 Casas, 3 Apartamentos)", 1, 3, opcion)) return false;
    if (opcion == 2) filtro = FiltroTipoInmueble::Casas;
    else if (opcion == 3) filtro = FiltroTipoInmueble::Apartamentos;
    else filtro = FiltroTipoInmueble::Todos;
    return true;
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

static void altaInmuebleParaPropietario(ISistema* sistema, const string& nickPropietario) {
    string error;
    int codigo = 0;
    int tipo;
    if (!leerIntEnRango("Tipo inmueble (1 Casa, 2 Apartamento)", 1, 2, tipo)) {
        cout << "Operacion cancelada." << endl;
        return;
    }
    string direccion = leerLinea("Direccion: ");
    int puerta = leerInt("Numero de puerta: ");
    float superficie = leerFloat("Superficie: ");
    int anio = leerInt("Anio construccion: ");
    Status status = Status::ERROR;
    error = "Tipo de inmueble invalido.";

    if (tipo == 1) {
        bool ph = leerBool("Propiedad horizontal");
        TipoTecho techo;
        if (!leerTipoTecho(techo)) {
            cout << "Operacion cancelada." << endl;
            return;
        }
        status = sistema->altaCasa(nickPropietario, direccion, puerta, superficie, anio, ph, techo, codigo, error);
    } else {
        int piso = leerInt("Piso: ");
        bool ascensor = leerBool("Tiene ascensor");
        float gastos = leerFloat("Gastos comunes: ");
        status = sistema->altaApartamento(nickPropietario, direccion, puerta, superficie, anio,
                                          piso, ascensor, gastos, codigo, error);
    }

    if (status == Status::OK) {
        cout << "Inmueble creado con codigo " << codigo << "." << endl;
    } else {
        cout << error << endl;
    }
}

static void agregarPropietariosRepresentados(ISistema* sistema, const string& nickInmobiliaria) {
    string error;
    do {
        cout << "Propietarios disponibles:" << endl;
        vector<string> propietarios = sistema->listarPropietarios();
        imprimirLineas(propietarios);
        if (propietarios.empty()) {
            cout << "No hay propietarios registrados para representar." << endl;
            return;
        }

        string nickPropietario = leerLinea("Nickname propietario a representar: ");
        Status status = sistema->representarPropietario(nickInmobiliaria, nickPropietario, error);
        cout << (status == Status::OK ? "Representacion creada." : error) << endl;
    } while (leerBool("Desea agregar otro propietario representado"));
}

static void altaUsuario(ISistema* sistema) {
    string error;
    int tipo;
    if (!leerIntEnRango("Tipo usuario (1 Cliente, 2 Propietario, 3 Inmobiliaria)", 1, 3, tipo)) {
        cout << "Operacion cancelada." << endl;
        esperarContinuar();
        return;
    }
    string nickname;
    if (!leerNicknameDisponible(sistema, nickname)) {
        cout << "Operacion cancelada." << endl;
        esperarContinuar();
        return;
    }
    string contrasenia;
    if (!leerContraseniaValida(contrasenia)) {
        cout << "Operacion cancelada." << endl;
        esperarContinuar();
        return;
    }
    string nombre = leerLinea("Nombre: ");
    string email = leerLinea("Email: ");
    Status status = Status::ERROR;
    error = "Tipo de usuario invalido.";

    if (tipo == 1) {
        string apellido = leerLinea("Apellido: ");
        string documento = leerLinea("Documento: ");
        status = sistema->altaCliente(nickname, contrasenia, nombre, email, apellido, documento, error);
    } else if (tipo == 2) {
        string cuenta = leerLinea("Cuenta bancaria: ");
        string telefono = leerLinea("Telefono: ");
        status = sistema->altaPropietario(nickname, contrasenia, nombre, email, cuenta, telefono, error);
        if (status == Status::OK) {
            cout << "Usuario creado." << endl;
            while (leerBool("Desea ingresar un inmueble para este propietario")) {
                altaInmuebleParaPropietario(sistema, nickname);
            }
            esperarContinuar();
            return;
        }
    } else {
        string direccion = leerLinea("Direccion: ");
        string telefono = leerLinea("Telefono: ");
        string url = leerLinea("URL: ");
        status = sistema->altaInmobiliaria(nickname, contrasenia, nombre, email, direccion, telefono, url, error);
        if (status == Status::OK) {
            cout << "Usuario creado." << endl;
            if (leerBool("Desea agregar propietarios representados para esta inmobiliaria")) {
                agregarPropietariosRepresentados(sistema, nickname);
            }
            esperarContinuar();
            return;
        }
    }
    cout << (status == Status::OK ? "Usuario creado." : error) << endl;
    esperarContinuar();
}

static void altaInmueble(ISistema* sistema) {
    imprimirLineas(sistema->listarPropietarios());
    string nickPropietario = leerLinea("Nickname propietario: ");
    altaInmuebleParaPropietario(sistema, nickPropietario);
}

static void representarPropietario(ISistema* sistema) {
    string error;
    cout << "Inmobiliarias:" << endl;
    imprimirLineas(sistema->listarInmobiliarias());
    cout << "Propietarios:" << endl;
    imprimirLineas(sistema->listarPropietarios());
    string nickInmo = leerLinea("Nickname inmobiliaria: ");
    string nickProp = leerLinea("Nickname propietario: ");
    Status status = sistema->representarPropietario(nickInmo, nickProp, error);
    cout << (status == Status::OK ? "Representacion creada." : error) << endl;
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
    Status status = sistema->altaAdministracion(nickInmo, codigo, error);
    cout << (status == Status::OK ? "Administracion creada." : error) << endl;
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
    TipoPublicacion tipo;
    if (!leerTipoPublicacion(tipo)) {
        cout << "Operacion cancelada." << endl;
        return;
    }
    string texto = leerLinea("Texto: ");
    float precio = leerFloat("Precio: ");
    Status status = sistema->altaPublicacion(nickInmo, codigoInmueble, tipo, texto, precio,
                                             codigoPublicacion, error);
    if (status == Status::OK) {
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
    Status status = sistema->altaAgenda(nickCliente, codigoPublicacion, fecha, contacto, error);
    cout << (status == Status::OK ? "Agenda creada." : error) << endl;
}

static void consultaPublicaciones(ISistema* sistema) {
    TipoPublicacion tipo;
    if (!leerTipoPublicacion(tipo)) {
        cout << "Operacion cancelada." << endl;
        return;
    }
    float min = leerFloat("Precio minimo: ");
    float max = leerFloat("Precio maximo: ");
    FiltroTipoInmueble filtro;
    if (!leerFiltroTipoInmueble(filtro)) {
        cout << "Operacion cancelada." << endl;
        return;
    }
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
    Status status = sistema->eliminarInmueble(codigo, error);
    cout << (status == Status::OK ? "Inmueble eliminado." : error) << endl;
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

        try {
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
        } catch (const exception& e) {
            cout << "Error inesperado: " << e.what() << endl;
        }
    }

    return 0;
}
