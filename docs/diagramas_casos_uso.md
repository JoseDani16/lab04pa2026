# Diagramas DSS y Comunicacion

Proyecto: Plataforma de bienes raices.

El objetivo de este documento es dejar una version presentable de los Diagramas de Secuencia del Sistema (DSS) y Diagramas de Comunicacion para los casos de uso del laboratorio.

## Convenciones

- En los DSS el sistema se trata como caja negra. Solo aparecen `Administrador` y `:Sistema`.
- Las operaciones del DSS devuelven `Status`, datatypes o colecciones de datatypes.
- El sistema no devuelve objetos de dominio.
- En los Diagramas de Comunicacion si aparecen objetos internos para mostrar colaboraciones.
- Los nombres de operaciones son tentativos y pueden ajustarse a la interfaz final.

---

# Diagramas de Secuencia del Sistema

## DSS - Alta de Usuario

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    alt tipo = Cliente
        Administrador->>Sistema: status = altaCliente(nickname: string, contrasenia: string, nombre: string, email: string, apellido: string, documento: string): Status
    else tipo = Propietario
        Administrador->>Sistema: status = altaPropietario(nickname: string, contrasenia: string, nombre: string, email: string, cuentaBancaria: string, telefono: string): Status
    else tipo = Inmobiliaria
        Administrador->>Sistema: status = altaInmobiliaria(nickname: string, contrasenia: string, nombre: string, email: string, direccion: string, telefono: string, url: string): Status
    end

    alt tipo = Propietario
        loop mientras existan inmuebles a ingresar
            alt tipoInmueble = Casa
                Administrador->>Sistema: status = altaCasa(nickPropietario: string, direccion: string, numPuerta: int, superficie: float, anioConstruccion: int, esPropiedadHorizontal: bool, tipoTecho: TipoTecho): Status
            else tipoInmueble = Apartamento
                Administrador->>Sistema: status = altaApartamento(nickPropietario: string, direccion: string, numPuerta: int, superficie: float, anioConstruccion: int, piso: int, ascensor: bool, gastosComunes: float): Status
            end
        end
    else tipo = Inmobiliaria
        loop mientras existan propietarios a representar
            Administrador->>Sistema: propietarios = listarPropietarios(): Set<DTPropietario>
            Administrador->>Sistema: status = representarPropietario(nickInmobiliaria: string, nickPropietario: string): Status
        end
    end
```

## DSS - Alta de Administracion de Propiedad

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    Administrador->>Sistema: inmobiliarias = listarInmobiliarias(): Set<DTInmobiliaria>
    Administrador->>Sistema: inmuebles = listarInmueblesAdministrables(nickInmobiliaria: string): Set<DTInmuebleAdministrable>
    Administrador->>Sistema: status = altaAdministracion(nickInmobiliaria: string, codigoInmueble: int): Status
```

## DSS - Alta de Publicacion

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    Administrador->>Sistema: inmobiliarias = listarInmobiliarias(): Set<DTInmobiliaria>
    Administrador->>Sistema: inmuebles = listarInmueblesAdministrados(nickInmobiliaria: string): Set<DTInmuebleAdministrado>
    Administrador->>Sistema: status = altaPublicacion(codigoInmueble: int, tipoPublicacion: TipoPublicacion, texto: string, precio: float): Status
```

Nota: si la interfaz final no conserva seleccion de contexto, la operacion puede quedar como:

```text
status = altaPublicacion(nickInmobiliaria: string, codigoInmueble: int, tipoPublicacion: TipoPublicacion, texto: string, precio: float): Status
```

## DSS - Consulta de Publicaciones

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    Administrador->>Sistema: publicaciones = consultarPublicaciones(tipoPublicacion: TipoPublicacion, precioMin: float, precioMax: float, tipoInmueble: string): Set<DTPublicacion>

    opt desea ver detalle de un inmueble
        Administrador->>Sistema: inmuebleDetalle = seleccionarInmueble(codigoPublicacion: int): DTInmuebleDetalle
    end
```

## DSS - Eliminar Inmueble

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    Administrador->>Sistema: inmueblesListados = listarInmuebles(): Set<DTInmuebleListado>
    Administrador->>Sistema: inmuebleDetalle = seleccionarInmueble(codigo: int): DTInmuebleDetalle

    opt confirma eliminacion
        Administrador->>Sistema: status = eliminarInmueble(codigo: int): Status
    end
```

## DSS - Alta de Agenda de Visita

Este caso aparece en el dominio del Lab 1. No estaba desarrollado como caso de uso principal en Lab 2, pero se incluye porque forma parte del ciclo funcional implementado.

```mermaid
sequenceDiagram
    actor Administrador
    participant Sistema as ":Sistema"

    Administrador->>Sistema: publicaciones = consultarPublicaciones(tipoPublicacion: TipoPublicacion, precioMin: float, precioMax: float, tipoInmueble: string): Set<DTPublicacion>
    Administrador->>Sistema: status = altaAgenda(nickCliente: string, codigoPublicacion: int, fechaVisita: DTFecha, formaContacto: string): Status
```

---

# Diagramas de Comunicacion

## Comunicacion - Alta Cliente

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaCliente(datosCliente)"| S[":Sistema"]
    S -->|"1.1: member(nickname)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: new Cliente(datosCliente)"| C[":Cliente"]
    S -->|"1.3: add(nickname, cliente)"| DU
    S -->|"1.4: status"| A
```

## Comunicacion - Alta Propietario

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaPropietario(datosPropietario)"| S[":Sistema"]
    S -->|"1.1: member(nickname)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: new Propietario(datosPropietario)"| P[":Propietario"]
    S -->|"1.3: add(nickname, propietario)"| DU
    S -->|"1.4: status"| A
```

## Comunicacion - Alta Inmobiliaria

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaInmobiliaria(datosInmobiliaria)"| S[":Sistema"]
    S -->|"1.1: member(nickname)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: new Inmobiliaria(datosInmobiliaria)"| I[":Inmobiliaria"]
    S -->|"1.3: add(nickname, inmobiliaria)"| DU
    S -->|"1.4: status"| A
```

## Comunicacion - Alta Inmueble

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaCasa/altaApartamento(nickPropietario, datosInmueble)"| S[":Sistema"]
    S -->|"1.1: find(nickPropietario)"| DU["usuarios: OrderedDictionary"]
    DU -->|"1.2: propietario"| P[":Propietario"]
    S -->|"1.3: generarCodigoInmueble()"| S
    S -->|"1.4: new Casa/Apartamento(datos)"| IN[":Inmueble"]
    S -->|"1.5: agregarInmueble(inmueble)"| P
    S -->|"1.6: add(codigo, inmueble)"| DI["inmuebles: OrderedDictionary"]
    S -->|"1.7: status"| A
```

## Comunicacion - Representar Propietario

```mermaid
flowchart LR
    A["Administrador"] -->|"1: representarPropietario(nickInmobiliaria, nickPropietario)"| S[":Sistema"]
    S -->|"1.1: find(nickInmobiliaria)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: find(nickPropietario)"| DU
    DU -->|"1.3: inmobiliaria"| I[":Inmobiliaria"]
    DU -->|"1.4: propietario"| P[":Propietario"]
    S -->|"1.5: agregarPropietarioRepresentado(propietario)"| I
    S -->|"1.6: status"| A
```

## Comunicacion - Listar Inmobiliarias

```mermaid
flowchart LR
    A["Administrador"] -->|"1: listarInmobiliarias()"| S[":Sistema"]
    S -->|"1.1: getIterator()"| DU["usuarios: OrderedDictionary"]
    DU -->|"1.2*: usuario"| U[":Usuario"]
    S -->|"1.3*: filtrar Inmobiliaria"| I[":Inmobiliaria"]
    S -->|"1.4: Set<DTInmobiliaria>"| A
```

## Comunicacion - Alta Administracion de Propiedad

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaAdministracion(nickInmobiliaria, codigoInmueble)"| S[":Sistema"]
    S -->|"1.1: find(nickInmobiliaria)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: find(codigoInmueble)"| DI["inmuebles: OrderedDictionary"]
    DU -->|"1.3: inmobiliaria"| I[":Inmobiliaria"]
    DI -->|"1.4: inmueble"| IN[":Inmueble"]
    S -->|"1.5: getPropietario()"| IN
    S -->|"1.6: representaA(propietario)"| I
    S -->|"1.7: new Administracion(fechaSistema, inmobiliaria, inmueble)"| AD[":Administracion"]
    S -->|"1.8: agregarAdministracion(administracion)"| I
    S -->|"1.9: agregarAdministracion(administracion)"| IN
    S -->|"1.10: status"| A
```

## Comunicacion - Listar Inmuebles Administrados

```mermaid
flowchart LR
    A["Administrador"] -->|"1: listarInmueblesAdministrados(nickInmobiliaria)"| S[":Sistema"]
    S -->|"1.1: find(nickInmobiliaria)"| DU["usuarios: OrderedDictionary"]
    DU -->|"1.2: inmobiliaria"| I[":Inmobiliaria"]
    S -->|"1.3: getAdministraciones()"| I
    I -->|"1.4*: administracion"| AD[":Administracion"]
    AD -->|"1.5*: getInmueble()"| IN[":Inmueble"]
    S -->|"1.6: Set<DTInmuebleAdministrado>"| A
```

## Comunicacion - Alta Publicacion

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaPublicacion(nickInmobiliaria, codigoInmueble, tipo, texto, precio)"| S[":Sistema"]
    S -->|"1.1: find(nickInmobiliaria)"| DU["usuarios: OrderedDictionary"]
    S -->|"1.2: find(codigoInmueble)"| DI["inmuebles: OrderedDictionary"]
    DU -->|"1.3: inmobiliaria"| I[":Inmobiliaria"]
    DI -->|"1.4: inmueble"| IN[":Inmueble"]
    S -->|"1.5: buscarAdministracion(inmobiliaria, inmueble)"| I
    I -->|"1.6*: getAdministraciones()"| AD[":Administracion"]
    S -->|"1.7: validar fecha y tipo"| AD
    S -->|"1.8: generarCodigoPublicacion()"| S
    S -->|"1.9: new Publicacion(codigo, fecha, tipo, texto, precio, administracion)"| P[":Publicacion"]
    S -->|"1.10: agregarPublicacion(publicacion)"| AD
    S -->|"1.11: actualizarActivaPorTipo(tipo)"| AD
    S -->|"1.12: status"| A
```

## Comunicacion - Consultar Publicaciones

```mermaid
flowchart LR
    A["Administrador"] -->|"1: consultarPublicaciones(tipo, precioMin, precioMax, tipoInmueble)"| S[":Sistema"]
    S -->|"1.1: getIterator()"| DU["usuarios: OrderedDictionary"]
    DU -->|"1.2*: inmobiliaria"| I[":Inmobiliaria"]
    I -->|"1.3*: getAdministraciones()"| AD[":Administracion"]
    AD -->|"1.4*: getPublicaciones()"| P[":Publicacion"]
    AD -->|"1.5*: getInmueble()"| IN[":Inmueble"]
    S -->|"1.6*: cumpleFiltros(publicacion, inmueble)"| S
    S -->|"1.7: Set<DTPublicacion>"| A
```

## Comunicacion - Ver Detalle de Inmueble

```mermaid
flowchart LR
    A["Administrador"] -->|"1: seleccionarInmueble(codigo)"| S[":Sistema"]
    S -->|"1.1: find(codigo)"| DI["inmuebles: OrderedDictionary"]
    DI -->|"1.2: inmueble"| IN[":Inmueble"]
    S -->|"1.3: crear DTInmuebleDetalle"| DT["DTInmuebleDetalle"]
    S -->|"1.4: DTInmuebleDetalle"| A
```

## Comunicacion - Alta Agenda

```mermaid
flowchart LR
    A["Administrador"] -->|"1: altaAgenda(nickCliente, codigoPublicacion, fechaVisita, formaContacto)"| S[":Sistema"]
    S -->|"1.1: find(nickCliente)"| DU["usuarios: OrderedDictionary"]
    DU -->|"1.2: cliente"| C[":Cliente"]
    S -->|"1.3: buscarPublicacion(codigoPublicacion)"| I[":Inmobiliaria"]
    I -->|"1.4*: getAdministraciones()"| AD[":Administracion"]
    AD -->|"1.5*: getPublicaciones()"| P[":Publicacion"]
    S -->|"1.6: estaActiva()"| P
    S -->|"1.7: new Agenda(fechaVisita, formaContacto, cliente, publicacion)"| AG[":Agenda"]
    S -->|"1.8: agregarAgenda(agenda)"| C
    S -->|"1.9: agregarAgenda(agenda)"| P
    S -->|"1.10: status"| A
```

## Comunicacion - Eliminar Inmueble

```mermaid
flowchart LR
    A["Administrador"] -->|"1: eliminarInmueble(codigo)"| S[":Sistema"]
    S -->|"1.1: find(codigo)"| DI["inmuebles: OrderedDictionary"]
    DI -->|"1.2: inmueble"| IN[":Inmueble"]
    IN -->|"1.3*: getAdministraciones()"| AD[":Administracion"]
    AD -->|"1.4*: getPublicaciones()"| P[":Publicacion"]
    P -->|"1.5*: getAgendas()"| AG[":Agenda"]
    AG -->|"1.6*: getCliente()"| C[":Cliente"]
    S -->|"1.7*: quitarAgenda(agenda)"| C
    S -->|"1.8*: delete agenda"| AG
    S -->|"1.9*: quitarPublicacion(publicacion)"| AD
    S -->|"1.10*: delete publicacion"| P
    S -->|"1.11*: quitarAdministracion(administracion)"| I[":Inmobiliaria"]
    S -->|"1.12*: quitarAdministracion(administracion)"| IN
    IN -->|"1.13: getPropietario()"| PR[":Propietario"]
    S -->|"1.14: quitarInmueble(inmueble)"| PR
    S -->|"1.15: remove(codigo)"| DI
    S -->|"1.16: delete inmueble"| IN
    S -->|"1.17: status"| A
```

