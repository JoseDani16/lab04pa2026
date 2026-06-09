# Resumen de Defensa - Lab 4: Plataforma de Bienes Raices

## Tabla de contenidos

- [1. Que hace el proyecto](#1-que-hace-el-proyecto)
- [2. Arquitectura general](#2-arquitectura-general)
- [3. Patrones de diseno](#3-patrones-de-diseno)
  - [3.1 Singleton](#31-singleton-sistema)
  - [3.2 Factory](#32-factory-factory)
  - [3.3 Facade](#33-facade-isistema--sistema)
  - [3.4 Herencia polimorfica](#34-herencia-polimorfica)
- [4. Modelo de dominio](#4-modelo-de-dominio)
  - [4.1 Clases y sus responsabilidades](#41-clases-y-sus-responsabilidades)
  - [4.2 Relaciones](#42-relaciones)
  - [4.3 Clase asociativa: Administracion](#43-clase-asociativa-administracion)
- [5. Funciones del sistema](#5-funciones-del-sistema-detalle)
  - [5.1 Altas de usuarios](#51-altas-de-usuarios)
  - [5.2 Altas de inmuebles](#52-altas-de-inmuebles)
  - [5.3 Representacion](#53-representacion)
  - [5.4 Administracion](#54-administracion)
  - [5.5 Publicacion](#55-publicacion)
  - [5.6 Agenda](#56-agenda)
  - [5.7 Eliminacion](#57-eliminacion)
  - [5.8 Consultas](#58-consultas)
  - [5.9 Datos de prueba](#59-datos-de-prueba)
- [6. Colecciones propias](#6-colecciones-propias-jerarquia-de-interfaces-implementaciones-y-patrones)
  - [6.1 Jerarquia de interfaces](#61-jerarquia-de-interfaces)
  - [6.2 ICollectible](#62-icollectible-el-marcador)
  - [6.3 Sistema de claves](#63-sistema-de-claves-ikey-y-orderedkey)
  - [6.4 ICollection](#64-icollection-la-interfaz-de-toda-coleccion)
  - [6.5 List](#65-list-implementacion-con-lista-enlazada)
  - [6.6 IDictionary](#66-idictionary-interfaz-de-diccionario)
  - [6.7 OrderedDictionary](#67-ordereddictionary-implementacion-con-arbol-binario-de-busqueda-bst)
  - [6.8 IIterator](#68-iiterator-el-patron-iterator)
  - [6.9 Uso en el dominio](#69-como-se-usan-las-colecciones-en-el-dominio)
  - [6.10 Preguntas frecuentes sobre colecciones](#610-puntos-clave-para-preguntas-del-profesor)
- [7. Mecanismos de C++](#7-mecanismos-de-c-utilizados)
  - [7.1 dynamic_cast](#71-dynamic_cast-convertir-tipos-en-tiempo-de-ejecucion)
  - [7.2 Status enum](#72-status-enum)
  - [7.3 Try/Catch](#73-trycatch)
  - [7.4 Forward declarations](#74-forward-declarations)
- [8. Ownership de memoria](#8-ownership-de-memoria)
- [9. Preguntas frecuentes del profesor](#9-preguntas-frecuentes-del-profesor)
- [10. Flujo de demo recomendado](#10-flujo-de-demo-recomendado)
- [11. Diagrama de navegacion](#11-diagrama-de-navegacion-para-buscar-una-publicacion)

---

## 1. Que hace el proyecto

Sistema de consola para gestionar una plataforma de bienes raices. Permite administrar usuarios (clientes, propietarios, inmobiliarias), inmuebles (casas, apartamentos), publicaciones de venta/alquiler, administraciones de propiedades y agendas de visita.


---

## 2. Arquitectura general

```
┌─────────────────────────────────────────────────────────┐
│                      main.cpp                           │
│                  (Menu de consola)                      │
│              Solo conoce Factory e ISistema             │
└──────────────────────┬──────────────────────────────────┘
                       │ Factory::getSistema()
                       ▼
┌─────────────────────────────────────────────────────────┐
│                    ISistema                             │
│              (Interfaz / Contrato)                      │
│   Define TODAS las operaciones del sistema              │
│   No conoce clases de dominio                           │
└──────────────────────┬──────────────────────────────────┘
                       │ implementada por
                       ▼
┌─────────────────────────────────────────────────────────┐
│                    Sistema                              │
│              (Singleton + Facade)                       │
│   Orquesta todo. Conoce el dominio completo.            │
│   Nunca devuelve objetos de dominio al exterior.        │
└──────┬──────────┬──────────┬──────────┬─────────────────┘
       │          │          │          │
       ▼          ▼          ▼          ▼
  ┌────────┐ ┌────────┐ ┌──────────┐ ┌──────────┐
  │Usuario │ │Inmueble│ │Administr.│ │Publicac. │ ...
  └────────┘ └────────┘ └──────────┘ └──────────┘
```


---

## 3. Patrones de diseno

### 3.1 Singleton (`Sistema`)

**Que es:** Garantiza que exista una unica instancia de una clase en todo el programa.


**Implementacion:**

```cpp
class Sistema : public ISistema {
private:
    static Sistema* instance;   // Puntero estatico: pertenece a la clase
    Sistema();                  // Constructor PRIVADO: nadie puede hacer new Sistema()
public:
    static Sistema* getInstance();
};

Sistema* Sistema::instance = nullptr;

Sistema* Sistema::getInstance() {
    if (instance == nullptr)
        instance = new Sistema();   // Se crea solo la primera vez (lazy)
    return instance;
}
```

**Por que aca:** Todo el estado del sistema (usuarios, inmuebles, publicaciones) debe vivir en un unico lugar. Si hubiera dos instancias, podrias crear un usuario en una y no encontrarlo en la otra.

**Responsabilidad del Singleton:**
- Mantener el estado global (diccionarios de usuarios e inmuebles).
- Ser el unico punto de acceso a los datos del dominio.
- Garantizar consistencia: todos los operaciones ven los mismos datos.

---

### 3.2 Factory (`Factory`)

**Que es:** Encapsula la logica de creacion de objetos. El cliente no necesita saber como se crea lo que usa.


**Implementacion:**

```cpp
class Factory {
public:
    static ISistema* getSistema();
};

ISistema* Factory::getSistema() {
    return Sistema::getInstance();
}
```

**Uso en main.cpp:**

```cpp
#include "Factory.h"    // Solo importa la Factory y la interfaz
// NO incluye Sistema.h

ISistema* sistema = Factory::getSistema();
```

**Por que aca:**
- El `main` no conoce `Sistema`. Si manana cambiamos la implementacion (por ejemplo, una version con base de datos), solo tocamos `Factory.cpp`.
- Principio de **inversion de dependencias**: el codigo de alto nivel depende de abstracciones (`ISistema`), no de implementaciones concretas (`Sistema`).

**Responsabilidad de la Factory:**
- Saber como crear la instancia del sistema.
- Ocultar los detalles de construccion (Singleton, inyeccion de dependencias, etc.).

---

### 3.3 Facade (`ISistema` + `Sistema`)

**Que es:** Provee una interfaz simplificada a un subsistema complejo. El cliente no necesita conocer las clases internas ni como se relacionan.


**Implementacion:**

```cpp
// ISistema: la interfaz que ve el mundo exterior
class ISistema {
    virtual Status altaPublicacion(...) = 0;
    virtual Status eliminarInmueble(...) = 0;
    virtual vector<string> consultarPublicaciones(...) = 0;
    // ... ~20 metodos que cubren todos los casos de uso
};
```

**Que oculta internamente `altaPublicacion`:**
1. Buscar la inmobiliaria por nickname.
2. Buscar el inmueble por codigo.
3. Encontrar la administracion que los conecta.
4. Validar que no exista otra publicacion del mismo tipo en la misma fecha.
5. Generar un codigo incremental unico.
6. Crear la publicacion.
7. Recalcular cual es la publicacion activa por tipo.

El `main` solo ve: `Status resultado = sistema->altaPublicacion(...);`

**Responsabilidad de la Facade:**
- Orquestar las operaciones del dominio.
- Validar reglas de negocio.
- Devolver solo datos simples (Status, strings, vectores), nunca objetos de dominio.
- Proteger el encapsulamiento del sistema.

---

### 3.4 Herencia polimorfica

**Que es:** Clases abstractas que definen un contrato, y clases concretas que lo implementan de forma distinta.


**En el proyecto:**

```
Usuario (abstracta)              Inmueble (abstracta)
├── Cliente                      ├── Casa
│   (apellido, documento)        │   (tipoTecho, propiedadHorizontal)
├── Propietario                  └── Apartamento
│   (cuentaBancaria, telefono)       (piso, ascensor, gastosComunes)
└── Inmobiliaria
    (direccion, telefono, url)
```

**Metodo clave:** `virtual string getTipo() const = 0;`

Esto obliga a cada subclase a decir que tipo es. Asi, cuando iteramos una coleccion mixta de usuarios, podemos saber que es cada uno sin necesidad de `if/else` encadenados.

```cpp
// En vez de esto (poco elegante):
if (dynamic_cast<Cliente*>(u))       cout << "Es cliente";
else if (dynamic_cast<Propietario*>(u)) cout << "Es propietario";

// Usamos polimorfismo:
cout << u->getTipo();   // Cada clase sabe que devolver
```

---

## 4. Modelo de dominio

### 4.1 Clases y sus responsabilidades

| Clase | Responsabilidad |
|-------|----------------|
| `Cliente` | Quien busca inmuebles y agenda visitas |
| `Propietario` | Dueno de inmuebles |
| `Inmobiliaria` | Representa propietarios, administra inmuebles, genera publicaciones |
| `Casa` / `Apartamento` | Tipos de inmuebles con atributos especificos |
| `Administracion` | Conecta Inmobiliaria con Inmueble (clase asociativa) |
| `Publicacion` | Anuncio de venta o alquiler de un inmueble administrado |
| `Agenda` | Coordina una visita entre un Cliente y una Publicacion |

### 4.2 Relaciones

```
Propietario 1 ──── * Inmueble
    │                    │
    │                    │ 1
    │                    │
    │                    ▼ *
    │              Administracion
    │                    │
    │                    │ *
    ▼                    │
Inmobiliaria ────── * ───┘
    │
    │ 1
    ▼ *
Publicacion
    │
    │ 1
    ▼ *
  Agenda ──── * 1 Cliente
```


### 4.3 Clase asociativa: `Administracion`

**Que es una clase asociativa:** Resuelve una relacion muchos a muchos entre dos clases. En vez de tener una relacion directa Inmobiliaria-Inmueble (que seria caos), creamos una clase intermedia que representa el vinculo.


```cpp
class Administracion {
    DTFecha fechaComienzo;
    Inmobiliaria* inmobiliaria;   // Quien administra
    Inmueble* inmueble;           // Que administra
    ICollection* publicaciones;   // Historial de publicaciones
};
```

---

## 5. Funciones del sistema (detalle)

### 5.1 Altas de usuarios

#### `altaCliente(nickname, contrasenia, nombre, email, apellido, documento, error) -> Status`

**Flujo:**
1. Validar que ningun campo de texto este vacio.
2. Validar que la contrasena tenga al menos 6 caracteres.
3. Validar que el nickname no exista ya.
4. Crear el objeto `Cliente` y agregarlo al diccionario `usuarios` con clave = nickname.


#### `altaPropietario(...)` y `altaInmobiliaria(...)`

Mismo patron que `altaCliente`, pero con campos especificos:
- Propietario: cuenta bancaria, telefono.
- Inmobiliaria: direccion, telefono, URL.

---

### 5.2 Altas de inmuebles

#### `altaCasa(nickPropietario, direccion, numeroPuerta, superficie, anioConstruccion, propiedadHorizontal, tipoTecho, codigoGenerado, error) -> Status`

**Flujo:**
1. Validar campos de texto no vacios.
2. Validar numeros positivos (puerta, superficie, anio).
3. Buscar al propietario por nickname. Si no existe, error.
4. Generar codigo incremental (`proxCodigoInmueble++`).
5. Crear objeto `Casa`.
6. Agregar la casa a la coleccion del propietario.
7. Agregar la casa al diccionario global de inmuebles (clave = codigo).
8. Devolver el codigo generado.


#### `altaApartamento(...)`

Mismo patron, pero agrega: piso (>= 0), ascensor (bool), gastos comunes (>= 0).

---

### 5.3 Representacion

#### `representarPropietario(nickInmobiliaria, nickPropietario, error) -> Status`

**Flujo:**
1. Validar textos no vacios.
2. Buscar inmobiliaria y propietario.
3. Verificar que la inmobiliaria no represente ya al propietario (si ya lo representa, retorna `ADVERTENCIA`).
4. Agregar el propietario a la coleccion de representados de la inmobiliaria.


**Regla de negocio importante:** Para que una inmobiliaria administre un inmueble, primero debe representar al dueno de ese inmueble.

---

### 5.4 Administracion

#### `altaAdministracion(nickInmobiliaria, codigoInmueble, error) -> Status`

**Flujo:**
1. Validar texto y codigo.
2. Buscar inmobiliaria e inmueble.
3. **Validar que la inmobiliaria represente al propietario del inmueble.** (Regla clave)
4. Validar que no exista ya una administracion entre ambos.
5. Crear `Administracion` con la fecha del sistema.
6. Agregar la administracion a la inmobiliaria Y al inmueble (doble enlace).


---

### 5.5 Publicacion

#### `altaPublicacion(nickInmobiliaria, codigoInmueble, tipo, texto, precio, codigoGenerado, error) -> Status`

**Flujo:**
1. Validar textos y precio > 0.
2. Buscar la administracion que conecta inmobiliaria + inmueble.
3. Recorrer las publicaciones de esa administracion: si ya existe una del mismo tipo en la misma fecha, error.
4. Generar codigo incremental (`proxCodigoPublicacion++`).
5. Crear la `Publicacion` y agregarla a la administracion.
6. **Recalcular la publicacion activa por tipo:** recorrer todas las publicaciones del mismo tipo, desactivar todas, activar la de mayor fecha.

**Regla de negocio clave:** La publicacion activa es la de mayor fecha por tipo. Si hoy publico en venta, esa es la activa. Si manana publico otra en venta, la nueva pasa a ser la activa y la anterior deja de serlo.


---

### 5.6 Agenda

#### `altaAgenda(nickCliente, codigoPublicacion, fechaVisita, formaContacto, error) -> Status`

**Flujo:**
1. Validar textos, codigo > 0, fecha valida.
2. Buscar cliente y publicacion.
3. Validar que la publicacion este activa.
4. Crear `Agenda` con los datos.
5. Agregar la agenda al cliente Y a la publicacion (doble enlace).


---

### 5.7 Eliminacion

#### `eliminarInmueble(codigoInmueble, error) -> Status`

**Flujo (borrado en cascada):**
1. Buscar el inmueble.
2. Recorrer sus administraciones y para cada una:
   a. Recorrer sus publicaciones y para cada una:
      - Quitar la publicacion de la administracion.
      - Eliminar la publicacion (que a su vez elimina sus agendas).
   b. Quitar la administracion de la inmobiliaria y del inmueble.
   c. Eliminar la administracion.
3. Quitar el inmueble de su propietario.
4. Quitar el inmueble del diccionario global.
5. Eliminar el inmueble.


**Punto clave para la defensa:** El borrado en cascada debe ser cuidadoso con los punteros. Se usa la tecnica de "primero recolectar en un vector, despues borrar" para evitar modificar una coleccion mientras se itera.

```cpp
// Primero recolectar
vector<Administracion*> aBorrar;
IIterator* it = inmueble->getAdministraciones()->getIterator();
while (it->hasCurrent()) {
    aBorrar.push_back(dynamic_cast<Administracion*>(it->getCurrent()));
    it->next();
}
delete it;

// Despues borrar
for (Administracion* adm : aBorrar) {
    borrarAdministracion(adm);
}
```

---

### 5.8 Consultas

#### `consultarPublicaciones(tipo, precioMin, precioMax, filtro, error) -> vector<string>`

**Flujo:**
1. Recorrer todas las inmobiliarias.
2. Para cada inmobiliaria, recorrer sus administraciones.
3. Para cada administracion, verificar si el tipo de inmueble coincide con el filtro (Todos / Casas / Apartamentos).
4. Si coincide, recorrer las publicaciones y filtrar por: activa, tipo, rango de precio.
5. Armar string con los datos y agregarlo al resultado.


#### `listarUsuarios()`, `listarInmuebles()`, etc.

Iteran las colecciones del sistema y devuelven vectores de strings formateados.

---

### 5.9 Datos de prueba

#### `cargarDatosPrueba(error) -> Status`

**Flujo:**
1. Limpiar todos los datos existentes (`limpiarDatos()`).
2. Resetear contadores y fecha del sistema.
3. Crear en orden:
   - 1 cliente (`cliente1`)
   - 2 propietarios (`prop1`, `prop2`)
   - 1 inmobiliaria (`inmo1`)
   - 2 inmuebles (casa para prop1, apartamento para prop2)
   - Representar ambos propietarios
   - 2 administraciones (inmo1 administra ambos inmuebles)
   - 2 publicaciones (venta para casa, alquiler para apartamento)
   - 1 agenda (cliente1 visita la publicacion 1)


---

## 6. Colecciones propias: jerarquia de interfaces, implementaciones y patrones

El proyecto NO usa STL. Las colecciones son una libreria propia disenada con fines didacticos
que implementa varios patrones de diseno. Entenderlas es **clave para la defensa**.

---

### 6.1 Jerarquia de interfaces

```
                    IKey (equals)
                      │
                 OrderedKey (compare: LESSER/EQUAL/GREATER)
                      │
              ┌───────┴────────┐
           String             Integer
   (clave por nickname)   (clave por codigo)


             ICollectible (marcador: "se puede guardar en colecciones")
                  │
    ┌─────────────┼─────────────────┐
    │             │                  │
 Usuario      Inmueble         Administracion
 Cliente       Casa             Publicacion
 Propietario   Apartamento       Agenda
 Inmobiliaria


              ICollection (add, remove, member, size, getIterator, isEmpty)
                  │
              ┌───┴───┐
            List  IDictionary
                      │
                   (add/remove/find usan IKey como parametro extra)
                      │
                 OrderedDictionary
                  (require claves OrderedKey, arbol binario de busqueda)


                   IIterator (hasCurrent, getCurrent, next, ~IIterator)
                        │
                  ListIterator
              (recorre ListNode*, mismo iterador para List y OrderedDictionary)
```

---

### 6.2 ICollectible: el marcador

```cpp
class ICollectible {
protected:
    ICollectible();             // No se puede instanciar directamente
public:
    virtual ~ICollectible();
};
```

Es una clase con constructor **protegido** y destructor virtual. Solo sirve como interfaz
para decir "este objeto se puede guardar en una coleccion". No tiene metodos propios.

**En nuestro proyecto**, TODAS las clases de dominio heredan de `ICollectible`:

```cpp
class Usuario : public ICollectible { ... };      // Clase abstracta
class Cliente : public Usuario { ... };           // Hereda ICollectible via Usuario
class Inmueble : public ICollectible { ... };     // Clase abstracta
class Publicacion : public ICollectible { ... };
class Agenda : public ICollectible { ... };
class Administracion : public ICollectible { ... };
```

**Por que es necesario:** `List` y `OrderedDictionary` trabajan internamente con `ICollectible*`.
Si una clase no hereda de `ICollectible`, no puede guardarse en estas colecciones.

---

### 6.3 Sistema de claves: `IKey` y `OrderedKey`

#### IKey: interfaz base para claves

```cpp
class IKey {
public:
    virtual bool equals(IKey *) const = 0;
    virtual ~IKey();
};
```


#### OrderedKey: agrega orden total

```cpp
enum ComparisonRes { LESSER, EQUAL, GREATER };

class OrderedKey : public IKey {
public:
    virtual ComparisonRes compare(OrderedKey *k) const = 0;
    bool equals(IKey *k) const;  // Implementado en base a compare()
    virtual ~OrderedKey();
};
```

**Implementacion de `equals`:** Delega en `compare` + `dynamic_cast`:

```cpp
bool OrderedKey::equals(IKey* k) const {
    OrderedKey *key = dynamic_cast<OrderedKey *>(k);
    if(key == NULL)
        throw invalid_argument("Se esperaba un OrderedKey");
    return compare(key) == EQUAL;
}
```


#### Claves concretas: `String` e `Integer`

Ambas heredan **simultaneamente** de `ICollectible` y `OrderedKey`:

```cpp
// String: clave de texto
class String : public ICollectible, public OrderedKey {
    char *s;
    ComparisonRes compare(OrderedKey *k) const {
        String *str = dynamic_cast<String *>(k);
        int cmp = strcmp(s, str->s);
        if(cmp == 0) return EQUAL;
        else if(cmp > 0) return GREATER;
        else return LESSER;
    }
};

// Integer: clave numerica
class Integer : public ICollectible, public OrderedKey {
    const int val;
    ComparisonRes compare(OrderedKey *k) const {
        Integer *i = dynamic_cast<Integer *>(k);
        if(i->val == val) return EQUAL;
        else if(val > i->val) return GREATER;
        else return LESSER;
    }
};
```

**Herencia multiple:** `String` e `Integer` heredan de `ICollectible` (para poder pasarse como valores)
y de `OrderedKey` (para usarse como claves ordenadas en el diccionario).

**Uso en `Sistema`:**

```cpp
// Clave string para usuarios
String key(nickname.c_str());
usuarios->find(&key);        // Busca usuario por nickname

// Clave integer para inmuebles
Integer key(codigo);
inmuebles->find(&key);       // Busca inmueble por codigo
```

---

### 6.4 ICollection: la interfaz de toda coleccion

```cpp
class ICollection {
public:
    virtual void add(ICollectible *) = 0;           // Agregar elemento
    virtual void remove(ICollectible *) = 0;         // Quitar por direccion de memoria
    virtual bool member(ICollectible *) const = 0;   // ¿Esta por direccion?
    virtual bool isEmpty() const = 0;
    virtual int getSize() const = 0;
    virtual IIterator *getIterator() = 0;            // Factory Method: crea iterador
    virtual ~ICollection();
};
```

**IMPORTANTE:** `add`, `remove` y `member` comparan por **direccion de memoria**, no por igualdad
de contenido. Dos objetos con los mismos atributos pero distintas direcciones son elementos
distintos. Esto es intencional: el diseno delegaba la comparacion semantica a `IDictionary`.

---

### 6.5 List: implementacion con lista enlazada

```cpp
class List : public ICollection {
    ListNode* head;    // Puntero al primer nodo
    int size;          // Cantidad de elementos
};
```

**Estructura interna:** `ListNode` es un nodo con:
- `ICollectible* elem` (el elemento guardado)
- `ListNode* next` (siguiente nodo)

**Operaciones:**
- `add`: recorre hasta el final o hasta encontrar duplicado (por direccion), inserta nodo.
- `remove`: busca por direccion, desenlaza el nodo, lo borra.
- `getIterator()`: devuelve `new ListIterator(head)`.

**Destructor:** Borra TODOS los nodos (`ListNode`) pero NO los elementos que contienen
(`ICollectible*`). Este es un punto de diseno fundamental: la coleccion no es duena
de los objetos que guarda.

**Complejidad:** O(n) para add, remove, member. O(1) para size, isEmpty.

---

### 6.6 IDictionary: interfaz de diccionario

```cpp
class IDictionary : public ICollection {
public:
    virtual void add(IKey *k, ICollectible *val) = 0;
    virtual bool member(IKey *k) const = 0;
    virtual void remove(IKey *k) = 0;
    virtual ICollectible *find(IKey *k) const = 0;
    // ... isEmpty, getSize, getIterator, ~IDictionary
};
```

**Diferencia clave con `ICollection`:** trabaja con **clave + valor**. La busqueda es por
clave, no por direccion del valor. El iterador devuelve los valores (no las claves).

**Nota de diseno:** `add` recibe `ICollectible* val` que **nunca se borra** al destruir
el diccionario. La clave `IKey* k` SI se borra al destruir el diccionario.

---

### 6.7 OrderedDictionary: implementacion con arbol binario de busqueda (BST)

```cpp
class OrderedDictionary : public IDictionary {
    OrderedDictionaryEntry* root;   // Raiz del arbol
    int size;
};
```

**Estructura interna:** `OrderedDictionaryEntry` es un nodo de arbol binario con:
- `OrderedKey* key` (la clave, se borra al destruir el diccionario)
- `ICollectible* val` (el valor, NO se borra)
- `OrderedDictionaryEntry* lesser` (subarbol izquierdo: claves menores)
- `OrderedDictionaryEntry* greater` (subarbol derecho: claves mayores)

**Operaciones principales:**

#### `add(IKey* k, ICollectible* val)`:
1. Hace `dynamic_cast<OrderedKey*>(k)` para asegurar que la clave es ordenable.
2. Recorre el arbol comparando con `key->compare(current->getKey())`:
   - Si `EQUAL`: reemplaza el valor y borra la clave duplicada.
   - Si `LESSER`: va por rama izquierda.
   - Si `GREATER`: va por rama derecha.
3. Si llega a una hoja, crea `OrderedDictionaryEntry` y lo inserta.

#### `find(IKey* k)`:
Recorre el arbol comparando claves hasta encontrar o llegar a hoja. Retorna `ICollectible*` o NULL.

#### `getIterator()`:
Recorre el arbol **in-order** (menores → nodo actual → mayores) para devolver los valores
ordenados de menor a mayor. Construye una `List` temporal con los valores y devuelve
un `ListIterator` sobre esa lista.

**Complejidad:** O(log n) promedio para add, find, remove (si el arbol esta balanceado).

**Destructor:** `root->deleteInDepth()` borra recursivamente TODAS las entradas y sus claves,
pero NO los valores (`ICollectible*`).

---

### 6.8 IIterator: el patron Iterator

```cpp
class IIterator {
public:
    virtual ICollectible *getCurrent() = 0;  // Elemento actual (tira excepcion si no hay)
    virtual bool hasCurrent() = 0;           // ¿Hay elemento actual?
    virtual void next() = 0;                 // Avanzar al siguiente
    virtual ~IIterator();
};
```

**Patron que implementa:** Iterator puro del GoF. Separa el algoritmo de recorrido de la
estructura de datos. El cliente no sabe si esta recorriendo una lista enlazada o un arbol.

**Unica implementacion:** `ListIterator` recorre `ListNode*`. Mantiene un puntero `current`
y avanza con `current = current->getNext()`. Funciona tanto para `List` como para
los iteradores de `OrderedDictionary` (que primero convierten el arbol a lista).

**Flujo estandar de uso (aparece en todo `Sistema.cpp`):**

```cpp
IIterator* it = coleccion->getIterator();  // 1. Crear iterador (con new)
while (it->hasCurrent()) {                 // 2. Mientras haya elementos
    Objeto* obj = dynamic_cast<Objeto*>(it->getCurrent());  // 3. Obtener y convertir
    // ... usar obj ...
    it->next();                             // 4. Avanzar
}
delete it;                                  // 5. Liberar el iterador
```

**Por que siempre `delete it`:** Los iteradores se crean con `new`. Si no se liberan,
hay memory leak. Esto es responsabilidad del que llama a `getIterator()`.

---

### 6.9 Como se usan las colecciones en el dominio

#### Diccionarios globales en `Sistema`

```cpp
class Sistema : public ISistema {
    IDictionary* usuarios;    // OrderedDictionary
    IDictionary* inmuebles;   // OrderedDictionary
};
```

**`usuarios`:** clave = `String` (nickname), valor = `Usuario*` (Cliente/Propietario/Inmobiliaria)

**Acceso tipico:**
```cpp
// Buscar
String key("cliente1");
Usuario* u = dynamic_cast<Usuario*>(usuarios->find(&key));

// Agregar
usuarios->add(new String("cliente1"), new Cliente(...));
```

**`inmuebles`:** clave = `Integer` (codigo), valor = `Inmueble*`

```cpp
// Buscar
Integer key(5);
Inmueble* inm = dynamic_cast<Inmueble*>(inmuebles->find(&key));

// Agregar
inmuebles->add(new Integer(codigoGenerado), new Casa(...));
```

#### Colecciones internas de las clases de dominio

Todas usan `List` (lista enlazada), accedidas via `ICollection*`:

| Clase | Coleccion | Contiene |
|-------|-----------|----------|
| `Propietario` | `ICollection* inmuebles` | `Inmueble*` |
| `Inmobiliaria` | `ICollection* propietariosRepr` | `Propietario*` |
| `Inmobiliaria` | `ICollection* administraciones` | `Administracion*` |
| `Inmueble` | `ICollection* administraciones` | `Administracion*` |
| `Administracion` | `ICollection* publicaciones` | `Publicacion*` |
| `Publicacion` | `ICollection* agendas` | `Agenda*` |
| `Cliente` | `ICollection* agendas` | `Agenda*` |

**Por que se usa `ICollection*` en vez de `List*` en los atributos:** Para programar contra
la interfaz, no contra la implementacion. Si manana se cambia `List` por otra estructura,
el codigo cliente no se rompe.

---

### 6.10 Puntos clave para preguntas del profesor

**"Como funciona el `add` en `OrderedDictionary` si la clave ya existe?"**
Si `compare` devuelve `EQUAL`, reemplaza el valor viejo por el nuevo y borra la clave
duplicada. No crea una nueva entrada.

**"Por que `List::add` compara por direccion de memoria?"**
`if(current->getElem() != c)` compara punteros. Dos objetos con identico contenido pero
distinta direccion son elementos distintos. Esto es una decision de diseno: la semantica
de igualdad la maneja `IDictionary` a traves de las claves.

**"Quien es dueno de la memoria de las claves en el diccionario?"**
El diccionario. Las claves se crean con `new` y el destructor de `OrderedDictionary`
las borra con `deleteInDepth()`. Los valores (`ICollectible*`) NO se borran.

**"Por que hay que hacer `delete it` despues de usar un iterador?"**
`getIterator()` crea el iterador con `new`. Si no se hace `delete`, hay memory leak.
Esto es responsabilidad del codigo cliente. En `Sistema.cpp` veras que incluso en
los `return` tempranos se libera el iterador antes de retornar.

**"Que pasa si no hago `dynamic_cast` al obtener de un iterador?"**
`getCurrent()` devuelve `ICollectible*`. Para acceder a metodos de la subclase
(como `getNickname()` de `Usuario`), tenes que convertirlo. Si no lo haces, no podes
llamar a esos metodos. Si te equivocas de tipo, `dynamic_cast` devuelve `nullptr`.

---

## 7. Mecanismos de C++ utilizados

### 7.1 dynamic_cast (convertir tipos en tiempo de ejecucion)

**Que hace:** Convierte un puntero de clase base a clase derivada, verificando en tiempo de ejecucion si la conversion es valida.


```cpp
Usuario* u = ...;  // Puede ser Cliente, Propietario o Inmobiliaria

Cliente* c = dynamic_cast<Cliente*>(u);
if (c != nullptr) {
    // u ES un Cliente, puedo usar metodos de Cliente
    cout << c->getDocumento();
}
```

**Requisito:** La clase base debe tener al menos un metodo `virtual` (en nuestro caso, `getTipo()` es virtual puro).

### 7.2 Status enum

```cpp
enum class Status { OK, ERROR, ADVERTENCIA };
```

Todas las operaciones que modifican el estado retornan `Status` + mensaje de error por referencia.


### 7.3 Try/Catch

Cada funcion publica de `Sistema` envuelve su logica en un `try/catch`:

```cpp
try {
    // Logica de negocio
} catch (const exception& e) {
    error = string("Error inesperado: ") + e.what();
    return Status::ERROR;
}
```


### 7.4 Forward declarations

```cpp
class Administracion;  // Declaracion adelantada
class Inmueble;
```

Se usan en los headers para evitar inclusiones circulares. El header solo necesita saber que la clase existe, no sus detalles.


---

## 8. Ownership de memoria

**Quien crea, quien destruye:**

| Objeto | Quien lo crea | Quien lo destruye |
|--------|--------------|-------------------|
| Usuarios | `Sistema` (en altaCliente, etc.) | `Sistema::limpiarDatos()` o destructor |
| Inmuebles | `Sistema` (en altaCasa, etc.) | `Sistema::eliminarInmueble()` o `limpiarDatos()` |
| Administraciones | `Sistema` (en altaAdministracion) | `Sistema::borrarAdministracion()` |
| Publicaciones | `Sistema` (en altaPublicacion) | `Sistema::borrarPublicacion()` |
| Agendas | `Sistema` (en altaAgenda) | Al borrar publicacion o limpiar datos |
| Iteradores | Quien los crea con `getIterator()` | El mismo con `delete it` |

**Regla general:** `Sistema` es el dueno de todo. Las clases de dominio solo tienen punteros a otros objetos, no los destruyen.

---

## 9. Preguntas frecuentes del profesor

### "Que patron usaron?"
Singleton (Sistema), Factory (Factory), Facade (ISistema/Sistema), y herencia polimorfica (Usuario/Inmueble).

### "Por que Singleton?"
Porque todo el estado del sistema debe ser unico. No tiene sentido tener dos registros de usuarios.

### "Por que Factory si solo llama a getInstance?"
Para desacoplar. El main no conoce Sistema. Si manana cambiamos la implementacion, solo tocamos Factory.

### "Que es ISistema?"
Una interfaz (clase abstracta con todos metodos puros). Define el contrato que debe cumplir el sistema. El main solo depende de esta interfaz.

### "Por que Sistema no devuelve objetos de dominio?"
Para proteger el encapsulamiento. Si devolviera un `Propietario*`, cualquiera podria modificarlo sin pasar por las validaciones del sistema.

### "Como funciona el borrado en cascada?"
Al borrar un inmueble, se borran sus administraciones, que a su vez borran sus publicaciones, que a su vez borran sus agendas. Se usa "recolectar y luego borrar" para no modificar colecciones mientras se iteran.

### "Que es la publicacion activa?"
La publicacion de mayor fecha por tipo (venta/alquiler) dentro de una administracion. Cuando se crea una nueva, se recalcula cual queda activa.

### "Que es Administracion?"
Una clase asociativa que conecta Inmobiliaria con Inmueble. Representa el contrato por el cual una inmobiliaria administra un inmueble de un propietario que representa.

### "Que pasa si la inmobiliaria no representa al propietario?"
No puede administrar su inmueble. Es una regla de negocio que se valida en `altaAdministracion`.

### "Como manejan los errores?"
Con el enum `Status` (OK, ERROR, ADVERTENCIA) y un parametro `string& error` que contiene el mensaje descriptivo. Todo envuelto en try/catch.

### "Que colecciones usan?"
Las del curso: `OrderedDictionary` para acceso por clave (usuarios por nickname, inmuebles por codigo), `List` para colecciones internas, e `IIterator` para recorrerlas.

### "Que es dynamic_cast y por que lo usan?"
Convierte un puntero de clase base a derivada en tiempo de ejecucion. Lo usamos porque las colecciones guardan `ICollectible*` y necesitamos acceder a metodos especificos de cada clase.

### "Como funciona la fecha del sistema?"
Es una fecha configurable dentro de `Sistema`. Por defecto es 27/5/2026. Se usa para las administraciones y publicaciones. Los datos de prueba la resetean.

---

## 10. Flujo de demo recomendado

```
1. Cargar datos de prueba       → Escenario completo listo
2. Listar usuarios              → Ver los 4 usuarios creados
3. Listar inmuebles             → Ver casa y apartamento
4. Consultar publicaciones      → Filtrar por tipo y precio
5. Alta de agenda               → Coordinar visita
6. Eliminar inmueble            → Ver borrado en cascada
7. Consultar publicaciones      → Verificar que se eliminaron
```

---

## 11. Diagrama de navegacion para buscar una publicacion

```
Sistema
  └─ usuarios (OrderedDictionary)
       └─ Inmobiliaria
            └─ administraciones (List)
                 └─ Administracion
                      ├─ inmobiliaria →
                      ├─ inmueble → Inmueble (Casa / Apartamento)
                      └─ publicaciones (List)
                           └─ Publicacion
                                ├─ codigo, fecha, tipo, texto, precio
                                ├─ activa (bool)
                                └─ agendas (List)
                                     └─ Agenda
                                          ├─ cliente →
                                          └─ publicacion →
```

Para buscar una publicacion por codigo, `Sistema` hace un recorrido de 3 niveles: usuarios → inmobiliarias → administraciones → publicaciones. Por eso `buscarPublicacion` tiene 3 iteradores anidados.
