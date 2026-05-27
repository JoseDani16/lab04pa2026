# Contexto acumulado del proyecto

Proyecto: plataforma digital de bienes raices para gestionar usuarios, inmuebles, publicaciones, administraciones y agendas de visita.

Fuentes revisadas:

- `C:\Users\reyma\Downloads\Letra Lab1.pdf`
- `C:\Users\reyma\Downloads\Letra Lab2.pdf`
- `C:\Users\reyma\Downloads\Letra Lab 4.pdf`
- UML compartido en la conversacion
- Material de ejemplo de colecciones y Singleton/Factory

## Dominio principal

El sistema trabaja con usuarios, inmuebles, inmobiliarias, publicaciones y agendas.

Usuarios:

- `Usuario` es abstracta.
- `Cliente`, `Propietario` e `Inmobiliaria` especializan `Usuario`.
- Todo usuario tiene nickname, contrasena de al menos seis caracteres, nombre y email.
- `Cliente` agrega apellido y documento.
- `Propietario` agrega cuenta bancaria y telefono.
- `Inmobiliaria` agrega direccion, telefono y URL.

Inmuebles:

- `Inmueble` es abstracta.
- `Casa` y `Apartamento` especializan `Inmueble`.
- Todo inmueble tiene codigo numerico, direccion, numero de puerta, superficie y ano de construccion.
- `Casa` agrega si es propiedad horizontal y `TipoTecho`: liviano, dos aguas o plano.
- `Apartamento` agrega piso, ascensor y gastos comunes.
- Cada inmueble pertenece a un propietario.

Administracion:

- Una inmobiliaria representa propietarios.
- Una inmobiliaria puede administrar inmuebles de propietarios que representa.
- Para cada administracion se registra fecha de comienzo.
- Lab4 agrega explicitamente el caso de uso `Alta de Administracion de Propiedad`.
- `Administracion` se modela como clase asociativa entre `Inmobiliaria` e `Inmueble`.
- `Administracion` colecciona las publicaciones generadas para ese inmueble administrado.

Publicaciones:

- Una inmobiliaria genera publicaciones sobre inmuebles que administra.
- En el modelo acordado, una publicacion pertenece a una `Administracion`.
- Cada publicacion tiene codigo incremental, fecha, tipo (`venta` o `alquiler`), texto y precio.
- El sistema mantiene historial de publicaciones.
- Para cada administracion solo puede haber una publicacion activa por tipo.
- La publicacion activa por tipo es la de mayor fecha.

Agendas:

- Un cliente puede coordinar visita a un inmueble publicado.
- La agenda referencia una publicacion, una fecha de visita y un texto con forma de contacto.
- En el modelo acordado, `Agenda` conoce a `Cliente` y `Publicacion`.
- `Cliente` colecciona sus agendas y `Publicacion` colecciona las agendas asociadas.

## Casos de uso requeridos

### Alta de Usuario

El administrador ingresa datos base y el tipo de usuario.

- Si el nickname ya existe, se informa error y no se crea.
- Si es cliente, se registran apellido y documento.
- Si es propietario, se registran cuenta bancaria y telefono.
- Si es inmobiliaria, se registran direccion, telefono y URL.
- Si se dio de alta un propietario, se permite registrar cero o mas inmuebles asociados.
- Si se dio de alta una inmobiliaria, se permite registrar cero o mas propietarios representados.

### Alta de Publicacion

Flujo esperado:

- Listar inmobiliarias por nickname y nombre.
- Elegir inmobiliaria.
- Listar inmuebles administrados por esa inmobiliaria con codigo, direccion y fecha de comienzo de administracion.
- Elegir inmueble.
- Ingresar tipo de publicacion, texto y precio.
- Crear publicacion con fecha del sistema y codigo incremental.
- Actualizar publicacion activa.
- Si ya existe publicacion con la misma fecha y tipo para ese contexto, informar error y no crear.

### Consulta de Publicaciones

Filtros:

- Tipo de publicacion: venta o alquiler.
- Rango de precio minimo y maximo.
- Tipo de inmueble: todos, solo casas o solo apartamentos.

Resultado:

- Listar solo publicaciones activas que cumplan los filtros.
- Mostrar codigo, fecha, texto, precio y nombre de la inmobiliaria.
- Opcionalmente, permitir ingresar codigo de publicacion y mostrar detalle completo del inmueble relacionado.

### Eliminar Inmueble

Flujo:

- Listar codigo, direccion y nombre del propietario de todos los inmuebles.
- Elegir inmueble.
- Mostrar detalle.
- Confirmar eliminacion.

Efectos:

- Si confirma, eliminar inmueble.
- Eliminar tambien todas sus publicaciones asociadas.
- Eliminar tambien las agendas asociadas a esas publicaciones.
- Si no confirma, no cambia el sistema.

### Alta de Administracion de Propiedad

Caso agregado en Lab4.

Flujo:

- Listar inmobiliarias por nickname y nombre.
- Elegir inmobiliaria.
- Listar inmuebles de propietarios representados por esa inmobiliaria, con codigo, direccion y nombre del propietario.
- Elegir inmueble.
- Crear administracion entre inmobiliaria e inmueble con fecha actual.

## Requisitos de implementacion Lab4

- Implementar completamente todos los casos de uso.
- Hacer menu de operaciones segun controladores e interfaces.
- Incluir opcion para inicializar/cargar datos de prueba.
- Entregar codigo fuente y Makefile.
- Debe compilar y ejecutar sin errores en Linux de la sede.

## Observaciones sobre el UML actual

El UML compartido contiene las clases centrales correctas:

- `Usuario`, `Cliente`, `Propietario`, `Inmobiliaria`
- `Inmueble`, `Casa`, `Apartamento`
- `Administracion`, `Publicacion`, `Agenda`
- `DTFecha`, `TipoPublicacion`, `TipoTecho`

Posibles ajustes necesarios:

- Agregar o confirmar asociaciones y multiplicidades:
  - `Propietario` 1 a muchos `Inmueble`.
  - `Inmobiliaria` muchos a muchos `Propietario` como representacion.
  - `Inmobiliaria` 1 a muchos `Administracion`.
  - `Inmueble` 1 a muchos `Administracion`.
  - `Administracion` 1 a muchos `Publicacion`.
  - `Publicacion` 1 a muchos `Agenda`.
  - `Cliente` 1 a muchos `Agenda`.
- `Administracion` es clase asociativa conceptualmente, pero en C++ se implementa como clase normal con punteros a `Inmobiliaria` e `Inmueble`.
- Agregar atributos principales a cada clase si el diagrama final debe ser completo.
- Considerar una clase controladora o fachada tipo `Sistema` / `ISistema`, siguiendo el ejemplo de Singleton y Factory.
- Usar colecciones propias (`ICollection`, `IDictionary`, `OrderedDictionary`, `List`) para guardar usuarios, inmuebles y relaciones internas.

## Recomendacion de arquitectura para implementar

- `Sistema` como singleton que implementa `ISistema`.
- `Factory` con `getSistema()`.
- Diccionarios por clave para acceso rapido:
  - usuarios por nickname.
  - inmuebles por codigo.
- `Sistema` no conoce directamente publicaciones ni agendas.
- `Sistema` mantiene los codigos incrementales de inmuebles y publicaciones.
- Listas para relaciones:
  - inmuebles de un propietario.
  - propietarios representados por inmobiliaria.
  - administraciones de una inmobiliaria.
  - administraciones de un inmueble.
  - publicaciones historicas de una administracion.
  - agendas de una publicacion.
  - agendas de un cliente.
- Separar clases de dominio de DTO/datatype usados por el menu.

## Modelo de relaciones acordado

`Sistema` conoce directamente:

- `usuarios`: diccionario por nickname.
- `inmuebles`: diccionario por codigo.
- `proxCodigoInmueble`.
- `proxCodigoPublicacion`.
- `fechaSistema`.

Relaciones:

- `Propietario` 1 a muchos `Inmueble`.
- `Inmueble` conoce a su `Propietario`.
- `Inmobiliaria` muchos a muchos `Propietario` como representacion. Por ahora alcanza con que `Inmobiliaria` coleccione propietarios representados.
- `Inmobiliaria` 1 a muchos `Administracion`.
- `Inmueble` 1 a muchos `Administracion`.
- `Administracion` conoce a una `Inmobiliaria`, a un `Inmueble` y colecciona `Publicacion`.
- `Publicacion` conoce a su `Administracion` y colecciona `Agenda`.
- `Agenda` conoce a su `Cliente` y a su `Publicacion`.
- `Cliente` colecciona sus `Agenda`.

Flujos de creacion:

- Alta de inmueble: `Sistema` busca propietario, genera codigo, crea `Casa` o `Apartamento`, lo agrega al propietario y lo registra en el diccionario global de inmuebles.
- Alta de administracion: `Sistema` busca inmobiliaria e inmueble, valida que la inmobiliaria represente al propietario del inmueble, crea `Administracion`, la agrega a la inmobiliaria y al inmueble.
- Alta de publicacion: `Sistema` busca la administracion que conecta inmobiliaria e inmueble, genera codigo, crea `Publicacion`, la agrega a la administracion y actualiza la publicacion activa por tipo dentro de esa administracion.
- Alta de agenda: `Sistema` busca cliente y publicacion navegando por administraciones, crea `Agenda`, la agrega al cliente y a la publicacion.

## Riesgos a cuidar

- Borrado en cascada de inmueble: hay que eliminar publicaciones y agendas relacionadas sin dejar punteros colgando.
- Publicacion activa: no alcanza con guardar historial; hay que poder determinar o actualizar cual queda activa por tipo.
- Ownership de memoria: las colecciones del ejemplo no borran los objetos guardados, por lo que el sistema debe decidir y documentar quien destruye cada objeto.
- Fecha del sistema: los casos asumen una fecha actual. Puede implementarse como una fecha fija configurable en datos de prueba o una funcion del sistema.
- Makefile: debe ser compatible con Linux; evitar comandos o rutas propias de Windows.
