# Guia de Uso

Proyecto: plataforma de bienes raices para Programacion Avanzada.

## Requisitos

- Compilador `g++`.
- `make`.
- Entorno Linux para la entrega final del laboratorio.

En Windows con MinGW tambien compila, pero el `clean` del Makefile esta pensado para Linux.

## Compilar

Desde la raiz del proyecto:

```bash
make
```

Esto genera el ejecutable:

```bash
./programa
```

## Ejecutar

```bash
make run
```

O directamente:

```bash
./programa
```

En Windows, si se compilo con MinGW:

```powershell
.\programa.exe
```

## Limpiar

```bash
make clean
```

## Flujo recomendado para probar

1. Ejecutar el programa.
2. Elegir `1. Cargar datos de prueba`.
3. Probar `10. Listar usuarios`.
4. Probar `11. Listar inmuebles`.
5. Probar `8. Consulta de publicaciones`.
6. Probar `7. Alta de agenda`.
7. Probar `9. Eliminar inmueble`.
8. Volver a consultar publicaciones para verificar que se eliminen las asociadas al inmueble.

## Alta de usuario

La opcion `2. Alta de usuario` sigue el flujo del caso de uso:

- Si se crea un `Cliente`, solo se cargan sus datos especificos.
- Si se crea un `Propietario`, el menu ofrece ingresar uno o mas inmuebles para ese propietario.
- Cada inmueble creado para un propietario se agrega a la coleccion del propietario y tambien al diccionario global de inmuebles del sistema.
- Si se crea una `Inmobiliaria`, el menu ofrece agregar uno o mas propietarios representados.
- El nickname se valida inmediatamente. Si ya existe, el menu permite ingresar otro nickname o cancelar el alta.
- La contrasenia se valida inmediatamente. Si tiene menos de seis caracteres, el menu permite reintentar la contrasenia o cancelar el ingreso.
- Los campos de texto obligatorios se validan en el momento. Si quedan vacios, el menu permite reintentar o cancelar.
- Los numeros con restricciones se validan en el momento: codigos, puertas, superficie, precio y anio deben ser positivos; piso y gastos comunes no pueden ser negativos.
- Las fechas se validan antes de continuar, incluyendo dias validos por mes y anios bisiestos.
- Los nicknames y codigos que referencian objetos existentes se verifican antes de seguir pidiendo datos del formulario.
- Las opciones enumeradas de los formularios permiten cancelar con `0` y reintentan si se ingresa un valor fuera de rango.

## Datos de prueba

La opcion `Cargar datos de prueba` crea:

- Un cliente: `cliente1`.
- Dos propietarios: `prop1`, `prop2`.
- Una inmobiliaria: `inmo1`.
- Dos inmuebles.
- Dos administraciones.
- Dos publicaciones.
- Una agenda asociada a una publicacion.

## Arquitectura

El acceso principal al sistema se hace mediante:

- `Factory`
- `ISistema`
- `Sistema`

`Sistema` funciona como fachada/controlador principal. No devuelve objetos de dominio hacia afuera; la idea de diseno es que devuelva `Status`, datatypes o colecciones de datatypes.

Las operaciones que modifican el sistema retornan `Status`, un enum con estos valores:

- `OK`
- `ERROR`
- `ADVERTENCIA`

Los mensajes de error se devuelven mediante un parametro `string& error`. Cuando una operacion genera un codigo, por ejemplo alta de inmueble o publicacion, ese codigo se devuelve mediante `int& codigoGenerado`.

La carga de datos de prueba tambien usa el mismo criterio:

```cpp
string error;
Status status = sistema->cargarDatosPrueba(error);
```

Para validaciones tempranas desde el menu, `ISistema` ofrece consultas booleanas como `existeCliente`, `existePropietario`, `existeInmobiliaria`, `existeInmueble`, `existePublicacion` y `existePublicacionActiva`. Estas operaciones no devuelven objetos de dominio.

Las operaciones publicas de `Sistema` capturan excepciones con `try/catch` y las transforman en `Status::ERROR` cargando el mensaje correspondiente en `error`. El menu tambien tiene un `try/catch` general para evitar que una excepcion no esperada cierre el programa.

Internamente:

- `Sistema` conoce directamente `usuarios` e `inmuebles`.
- `Usuario` es abstracta; derivan `Cliente`, `Propietario` e `Inmobiliaria`.
- `Inmueble` es abstracta; derivan `Casa` y `Apartamento`.
- `Administracion` conecta `Inmobiliaria` e `Inmueble`.
- `Administracion` colecciona `Publicacion`.
- `Publicacion` colecciona `Agenda`.
- `Agenda` conecta `Cliente` y `Publicacion`.

## Colecciones

El proyecto usa las colecciones provistas en el laboratorio:

- `ICollection`
- `List`
- `IDictionary`
- `OrderedDictionary`
- `IIterator`
- `Integer`
- `String`

Las clases de dominio heredan de `ICollectible` para poder almacenarse en esas colecciones.

## Documentacion

Los diagramas DSS y de comunicacion estan en:

```text
docs/diagramas_casos_uso.md
```

El contexto acumulado del proyecto esta en:

```text
CONTEXTO_PROYECTO_LABS.md
```
