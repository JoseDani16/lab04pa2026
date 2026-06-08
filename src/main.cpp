#include "Factory.h"
#include <exception>
#include <iostream>
#include <limits>

using namespace std;

static bool leerBool(const string& etiqueta);

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

static bool textoVacio(const string& valor) {
    return valor.find_first_not_of(" \t\r\n") == string::npos;
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

static bool leerLineaNoVacia(const string& etiqueta, string& valor) {
    while (true) {
        valor = leerLinea(etiqueta);
        if (!textoVacio(valor)) return true;
        cout << "Este dato no puede quedar vacio." << endl;
        if (!leerBool("Desea reintentar")) return false;
    }
}

static bool leerIntEnRango(const string& etiqueta, int min, int max, int& valor) {
    while (true) {
        valor = leerInt(etiqueta + " (0 Cancelar): ");
        if (valor == 0) return false;
        if (valor >= min && valor <= max) return true;
        cout << "Opcion invalida. Debe ingresar un valor entre " << min << " y " << max << "." << endl;
    }
}

static bool leerIntPositivo(const string& etiqueta, int& valor) {
    while (true) {
        valor = leerInt(etiqueta);
        if (valor > 0) return true;
        cout << "Debe ingresar un numero mayor a cero." << endl;
        if (!leerBool("Desea reintentar")) return false;
    }
}

static bool leerIntNoNegativo(const string& etiqueta, int& valor) {
    while (true) {
        valor = leerInt(etiqueta);
        if (valor >= 0) return true;
        cout << "Debe ingresar un numero mayor o igual a cero." << endl;
        if (!leerBool("Desea reintentar")) return false;
    }
}

static bool leerFloatPositivo(const string& etiqueta, float& valor) {
    while (true) {
        valor = leerFloat(etiqueta);
        if (valor > 0) return true;
        cout << "Debe ingresar un numero mayor a cero." << endl;
        if (!leerBool("Desea reintentar")) return false;
    }
}

static bool leerFloatNoNegativo(const string& etiqueta, float& valor) {
    while (true) {
        valor = leerFloat(etiqueta);
        if (valor >= 0) return true;
        cout << "Debe ingresar un numero mayor o igual a cero." << endl;
        if (!leerBool("Desea reintentar")) return false;
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
        if (!leerLineaNoVacia("Contrasenia: ", contrasenia)) return false;
        if (contrasenia.size() >= 6) return true;
        cout << "La contrasenia debe tener al menos 6 caracteres." << endl;
        if (!leerBool("Desea reintentar la contrasenia")) return false;
    }
}

static bool leerNicknameDisponible(ISistema* sistema, string& nickname) {
    while (true) {
        if (!leerLineaNoVacia("Nickname: ", nickname)) return false;
        if (!sistema->existeUsuario(nickname)) return true;
        cout << "Ya existe un usuario con ese nickname." << endl;
        if (!leerBool("Desea ingresar otro nickname")) return false;
    }
}

static bool fechaValida(int dia, int mes, int anio) {
    if (anio <= 0 || mes < 1 || mes > 12 || dia < 1) return false;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    if (bisiesto) diasMes[1] = 29;
    return dia <= diasMes[mes - 1];
}

static bool leerFecha(const string& etiqueta, DTFecha& fecha) {
    while (true) {
        cout << etiqueta << endl;
        int dia = leerInt("Dia: ");
        int mes = leerInt("Mes: ");
        int anio = leerInt("Anio: ");
        if (fechaValida(dia, mes, anio)) {
            fecha = DTFecha(dia, mes, anio);
            return true;
        }
        cout << "La fecha ingresada no es valida." << endl;
        if (!leerBool("Desea reintentar la fecha")) return false;
    }
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

static void imprimirResultado(Status status, const string& mensajeOk, const string& mensaje, bool esperar = true) {
    if (status == Status::OK) {
        cout << mensajeOk << endl;
    } else if (status == Status::ADVERTENCIA) {
        cout << "Advertencia: " << mensaje << endl;
    } else {
        cout << mensaje << endl;
    }
    if (esperar) esperarContinuar();
}

static bool leerClienteExistente(ISistema* sistema, string& nickname) {
    while (true) {
        if (!leerLineaNoVacia("Nickname cliente: ", nickname)) return false;
        if (sistema->existeCliente(nickname)) return true;
        cout << "No existe un cliente con ese nickname." << endl;
        if (!leerBool("Desea ingresar otro cliente")) return false;
    }
}

static bool leerPropietarioExistente(ISistema* sistema, string& nickname) {
    while (true) {
        if (!leerLineaNoVacia("Nickname propietario: ", nickname)) return false;
        if (sistema->existePropietario(nickname)) return true;
        cout << "No existe un propietario con ese nickname." << endl;
        if (!leerBool("Desea ingresar otro propietario")) return false;
    }
}

static bool leerInmobiliariaExistente(ISistema* sistema, string& nickname) {
    while (true) {
        if (!leerLineaNoVacia("Nickname inmobiliaria: ", nickname)) return false;
        if (sistema->existeInmobiliaria(nickname)) return true;
        cout << "No existe una inmobiliaria con ese nickname." << endl;
        if (!leerBool("Desea ingresar otra inmobiliaria")) return false;
    }
}

static bool leerCodigoInmuebleExistente(ISistema* sistema, int& codigo) {
    while (true) {
        if (!leerIntPositivo("Codigo inmueble: ", codigo)) return false;
        if (sistema->existeInmueble(codigo)) return true;
        cout << "No existe un inmueble con ese codigo." << endl;
        if (!leerBool("Desea ingresar otro codigo")) return false;
    }
}

static bool leerCodigoPublicacionActiva(ISistema* sistema, int& codigo) {
    while (true) {
        if (!leerIntPositivo("Codigo publicacion: ", codigo)) return false;
        if (sistema->existePublicacionActiva(codigo)) return true;
        if (sistema->existePublicacion(codigo)) {
            cout << "La publicacion existe, pero no esta activa." << endl;
        } else {
            cout << "No existe una publicacion con ese codigo." << endl;
        }
        if (!leerBool("Desea ingresar otro codigo")) return false;
    }
}

static void altaInmuebleParaPropietario(ISistema* sistema, const string& nickPropietario) {
    string error;
    int codigo = 0;
    int tipo;
    if (!leerIntEnRango("Tipo inmueble (1 Casa, 2 Apartamento)", 1, 2, tipo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    string direccion;
    int puerta;
    float superficie;
    int anio;
    if (!leerLineaNoVacia("Direccion: ", direccion) ||
        !leerIntPositivo("Numero de puerta: ", puerta) ||
        !leerFloatPositivo("Superficie: ", superficie) ||
        !leerIntPositivo("Anio construccion: ", anio)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = Status::ERROR;
    error = "Tipo de inmueble invalido.";

    if (tipo == 1) {
        bool ph = leerBool("Propiedad horizontal");
        TipoTecho techo;
        if (!leerTipoTecho(techo)) {
            imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
            return;
        }
        status = sistema->altaCasa(nickPropietario, direccion, puerta, superficie, anio, ph, techo, codigo, error);
    } else {
        int piso;
        if (!leerIntNoNegativo("Piso: ", piso)) {
            imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
            return;
        }
        bool ascensor = leerBool("Tiene ascensor");
        float gastos;
        if (!leerFloatNoNegativo("Gastos comunes: ", gastos)) {
            imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
            return;
        }
        status = sistema->altaApartamento(nickPropietario, direccion, puerta, superficie, anio,
                                          piso, ascensor, gastos, codigo, error);
    }

    if (status == Status::OK) {
        imprimirResultado(status, "Inmueble creado con codigo " + to_string(codigo) + ".", error);
    } else {
        imprimirResultado(status, "", error);
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

        string nickPropietario;
        if (!leerPropietarioExistente(sistema, nickPropietario)) {
            imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
            return;
        }
        Status status = sistema->representarPropietario(nickInmobiliaria, nickPropietario, error);
        imprimirResultado(status, "Representacion creada.", error, false);
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
    string nombre;
    string email;
    if (!leerLineaNoVacia("Nombre: ", nombre) ||
        !leerLineaNoVacia("Email: ", email)) {
        cout << "Operacion cancelada." << endl;
        esperarContinuar();
        return;
    }
    Status status = Status::ERROR;
    error = "Tipo de usuario invalido.";

    if (tipo == 1) {
        string apellido;
        string documento;
        if (!leerLineaNoVacia("Apellido: ", apellido) ||
            !leerLineaNoVacia("Documento: ", documento)) {
            cout << "Operacion cancelada." << endl;
            esperarContinuar();
            return;
        }
        status = sistema->altaCliente(nickname, contrasenia, nombre, email, apellido, documento, error);
    } else if (tipo == 2) {
        string cuenta;
        string telefono;
        if (!leerLineaNoVacia("Cuenta bancaria: ", cuenta) ||
            !leerLineaNoVacia("Telefono: ", telefono)) {
            cout << "Operacion cancelada." << endl;
            esperarContinuar();
            return;
        }
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
        string direccion;
        string telefono;
        string url;
        if (!leerLineaNoVacia("Direccion: ", direccion) ||
            !leerLineaNoVacia("Telefono: ", telefono) ||
            !leerLineaNoVacia("URL: ", url)) {
            cout << "Operacion cancelada." << endl;
            esperarContinuar();
            return;
        }
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
    imprimirResultado(status, "Usuario creado.", error);
}

static void altaInmueble(ISistema* sistema) {
    imprimirLineas(sistema->listarPropietarios());
    string nickPropietario;
    if (!leerPropietarioExistente(sistema, nickPropietario)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    altaInmuebleParaPropietario(sistema, nickPropietario);
}

static void representarPropietario(ISistema* sistema) {
    string error;
    cout << "Inmobiliarias:" << endl;
    imprimirLineas(sistema->listarInmobiliarias());
    cout << "Propietarios:" << endl;
    imprimirLineas(sistema->listarPropietarios());
    string nickInmo;
    string nickProp;
    if (!leerInmobiliariaExistente(sistema, nickInmo) ||
        !leerPropietarioExistente(sistema, nickProp)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = sistema->representarPropietario(nickInmo, nickProp, error);
    imprimirResultado(status, "Representacion creada.", error);
}

static void altaAdministracion(ISistema* sistema) {
    string error;
    imprimirLineas(sistema->listarInmobiliarias());
    string nickInmo;
    if (!leerInmobiliariaExistente(sistema, nickInmo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    cout << "Inmuebles administrables:" << endl;
    vector<string> administrables = sistema->listarInmueblesAdministrables(nickInmo);
    imprimirLineas(administrables);
    if (administrables.empty()) {
        imprimirResultado(Status::ERROR, "", "No se puede continuar: no hay inmuebles administrables para esa inmobiliaria.");
        return;
    }
    int codigo;
    if (!leerCodigoInmuebleExistente(sistema, codigo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = sistema->altaAdministracion(nickInmo, codigo, error);
    imprimirResultado(status, "Administracion creada.", error);
}

static void altaPublicacion(ISistema* sistema) {
    string error;
    int codigoPublicacion = 0;
    imprimirLineas(sistema->listarInmobiliarias());
    string nickInmo;
    if (!leerInmobiliariaExistente(sistema, nickInmo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    cout << "Inmuebles administrados:" << endl;
    vector<string> administrados = sistema->listarInmueblesAdministrados(nickInmo);
    imprimirLineas(administrados);
    if (administrados.empty()) {
        imprimirResultado(Status::ERROR, "", "No se puede continuar: no hay inmuebles administrados para esa inmobiliaria.");
        return;
    }
    int codigoInmueble;
    if (!leerCodigoInmuebleExistente(sistema, codigoInmueble)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    TipoPublicacion tipo;
    if (!leerTipoPublicacion(tipo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    string texto;
    float precio;
    if (!leerLineaNoVacia("Texto: ", texto) ||
        !leerFloatPositivo("Precio: ", precio)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = sistema->altaPublicacion(nickInmo, codigoInmueble, tipo, texto, precio,
                                             codigoPublicacion, error);
    if (status == Status::OK) {
        imprimirResultado(status, "Publicacion creada con codigo " + to_string(codigoPublicacion) + ".", error);
    } else {
        imprimirResultado(status, "", error);
    }
}

static void altaAgenda(ISistema* sistema) {
    string error;
    string nickCliente;
    int codigoPublicacion;
    DTFecha fecha;
    string contacto;
    if (!leerClienteExistente(sistema, nickCliente) ||
        !leerCodigoPublicacionActiva(sistema, codigoPublicacion) ||
        !leerFecha("Fecha visita", fecha) ||
        !leerLineaNoVacia("Forma de contacto: ", contacto)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = sistema->altaAgenda(nickCliente, codigoPublicacion, fecha, contacto, error);
    imprimirResultado(status, "Agenda creada.", error);
}

static void consultaPublicaciones(ISistema* sistema) {
    TipoPublicacion tipo;
    if (!leerTipoPublicacion(tipo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    float min;
    float max;
    if (!leerFloatNoNegativo("Precio minimo: ", min) ||
        !leerFloatNoNegativo("Precio maximo: ", max)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    if (max < min) {
        imprimirResultado(Status::ERROR, "", "El precio maximo no puede ser menor al minimo.");
        return;
    }
    FiltroTipoInmueble filtro;
    if (!leerFiltroTipoInmueble(filtro)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    imprimirLineas(sistema->consultarPublicaciones(tipo, min, max, filtro));
    if (leerBool("Ver detalle de un inmueble")) {
        int codigo;
        if (!leerCodigoInmuebleExistente(sistema, codigo)) {
            imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
            return;
        }
        cout << sistema->detalleInmueble(codigo) << endl;
    }
}

static void eliminarInmueble(ISistema* sistema) {
    string error;
    imprimirLineas(sistema->listarInmuebles());
    int codigo;
    if (!leerCodigoInmuebleExistente(sistema, codigo)) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    string detalle = sistema->detalleInmueble(codigo);
    cout << detalle << endl;
    if (detalle == "No existe un inmueble con ese codigo.") {
        imprimirResultado(Status::ERROR, "", "No se puede continuar con la eliminacion.");
        return;
    }
    if (!leerBool("Confirma eliminar")) {
        imprimirResultado(Status::ADVERTENCIA, "", "Operacion cancelada.");
        return;
    }
    Status status = sistema->eliminarInmueble(codigo, error);
    imprimirResultado(status, "Inmueble eliminado.", error);
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
                    {
                        string error;
                        Status status = sistema->cargarDatosPrueba(error);
                        imprimirResultado(status, "Datos de prueba cargados.", error);
                    }
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
