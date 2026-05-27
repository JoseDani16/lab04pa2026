# Analisis de Materiales Laboratorio 4

Material analizado desde `C:\Users\reyma\Downloads\Materiales Laboratorio 4 - Implementación.zip`.

## Que contiene

- `Ejemplo de Colecciones`: ejemplo C++ de una persona con mascotas usando una coleccion propia.
- `Ejemplo Singleton y Factory`: ejemplo C++ de acceso a un sistema unico mediante una factory.
- `ColDCD.png` y `DictDCD.png`: diagramas de clases para la coleccion y el diccionario.
- `Leame.txt`: descripcion de la implementacion de `ICollection`, `IDictionary`, `IKey`, `IIterator`, `List` y `OrderedDictionary`.

## Ideas reutilizables para el proyecto

1. Separar interfaces de implementaciones concretas.
   - `ICollection` define operaciones generales: `add`, `remove`, `member`, `isEmpty`, `getSize`, `getIterator`.
   - `IDictionary` define operaciones por clave: `add`, `member`, `remove`, `find`, `getIterator`.

2. Usar colecciones propias para relaciones entre objetos.
   - En el ejemplo, `Persona` tiene una `ICollection *mascotas`.
   - `agregarMascota` agrega el animal a la lista y tambien mantiene la asociacion inversa con la persona.
   - Esto sirve como modelo para relaciones tipo uno-a-muchos o muchos-a-muchos.

3. Recorrer colecciones mediante iteradores.
   - `listarMascotas` pide un `IIterator`, recorre con `hasCurrent`, `getCurrent` y `next`, y luego libera el iterador.
   - Es una forma consistente de recorrer sin depender de si abajo hay lista, arbol u otra estructura.

4. Usar `OrderedDictionary` cuando haga falta buscar por clave ordenada.
   - Implementa un ABB con claves derivadas de `OrderedKey`.
   - Permite buscar, agregar, eliminar, obtener minimo/maximo e iterar en orden ascendente o descendente.

5. Usar Factory/Singleton para centralizar el sistema.
   - `Factory::getSistema()` devuelve `Sistema::getInstance()`.
   - Sirve para que `main` y otras capas trabajen contra `ISistema`, no contra `Sistema` directamente.

## Precauciones importantes

- El codigo usa punteros crudos y manejo manual de memoria. Hay que definir claramente quien crea y quien destruye cada objeto.
- `ICollection` y `IDictionary` no borran los elementos guardados; solo borran nodos/entradas internas. Los objetos de dominio se liberan aparte.
- `OrderedDictionary` si borra sus claves, pero no sus valores.
- `OrderedDictionary::~OrderedDictionary()` asume que `root` no es `NULL`; si el diccionario esta vacio podria fallar. Conviene protegerlo antes de reutilizarlo.
- `Persona` crea `mascotas = new List()` pero no tiene destructor que haga `delete mascotas`; conviene agregarlo en un proyecto real.
- El `make clean` usa `rm`, por lo que en Windows falla si no hay entorno Unix. La compilacion con `make` y `g++` si funciono.

## Verificacion realizada

- `Ejemplo de Colecciones` compilo con MinGW y al correr imprimio:
  - `Mascota: Michi`
  - `Mascota: Chicho`
- `Ejemplo Singleton y Factory` compilo con MinGW y al correr imprimio:
  - `soy feliz`
  - `Nombre del Socio: Pepito`

## Recomendacion

Para el proyecto, conviene reutilizar primero la carpeta `ICollection` del ejemplo de colecciones como base, pero revisar destructores y ownership antes de integrarla. Luego se puede aplicar el patron `Factory + ISistema + Sistema singleton` si el proyecto requiere una unica fachada central para las operaciones.
