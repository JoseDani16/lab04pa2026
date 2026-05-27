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
