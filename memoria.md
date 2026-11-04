# Ricardo Luy - Proyecto Intermedio

## Información general

La escena con las pruebas del proyecto se encuentra en `MProject_Scene.h`. Se carga de forma predeterminada al iniciar el programa y también se puede acceder a ella mediante la tecla `5`.

## Requisito A

**A.1)** → `ParticleWithForces.cpp`, líneas 29-34. En la línea 33 se llama al método de integración `verlet()`, implementado en las líneas 51-60.

**A.2)** → `ParticleWithForces.cpp`, líneas 7-8: se define el atributo masa y el resto de componentes que recibe el constructor.

→ `MProject_Scene.h`, líneas 18-25: se define el struct `Particle`, del que se crean dos instancias con diferentes parámetros para `apartadoA()` (línea 27).

**A.3)** → `ParticleWithForces.cpp`, línea 11: se calcula `_invMass`, que almacena la inversa de la masa. En `verlet()`, línea 53, se calcula la aceleración aplicando la segunda ley de Newton.

→ `MProject_Scene.h`, líneas 64-65: se aplica la misma fuerza a ambas partículas. En la demostración debería observarse cómo la partícula de menor masa adquiere una aceleración mayor.

**Controles:**
- `I`: mostrar información de las partículas generadas.

## Requisito B

→ `ProjectileWithForces.cpp`, líneas 3-4: adaptación de la clase `Projectile` de la prácticas anteriores (`Projectile.cpp` y `P2S_Scene.h`). Se mantiene la lógica del proyectil, pero se sustituye la aceleración como atributo por la aplicación de fuerzas mediante `ParticleWithForces`.

→ `MProject_Scene.h`, líneas 100-170: se definen las estructuras auxiliares para configurar los tipos de proyectil y calcular sus parámetros simulados, incluyendo masa, gravedad y fuerza. También se define `ActiveProjectile`, que almacena cada proyectil disparado y su fuerza gravitatoria.

El método `shoot()` genera los proyectiles desde la cámara utilizando la velocidad y la masa simuladas.

**Controles:**
- `Espacio`: disparar.
- `C`: cambiar el tipo de proyectil.
- `P`: mostrar información del tipo seleccionado.

**Extra:** Se omite el ajuste dinámico de la masa o velocidad del proyectil para simplificar los controles de la escena. Una posible implementación sería similar a la realizada en `P2S_Scene.h`, líneas 79-95.

**Consideración adicional:** Los proyectiles disparados no se eliminan durante la ejecución, por lo que el vector que los almacena puede crecer indefinidamente a medida que se dispara.

## Requisito C

→ `ParticleSystemWithForces.cpp`, se encarga en su totalidad de crear, destruir y actualizar un conjunto de partículas (su ciclo de vida). También gestiona el generador de partículas y el registro de generadores de fuerzas. (Su funcionamiento se explica con más detalle en el apartado D)
→ `MProject_Scene.h`, líneas 194-223: en el `apartadoC()` se reutilizan las configuraciones de las partículas definidas en el apartado A como referencia para configurar los emisores mediante `EmitterConfig`. Se establecen los parámetros iniciales y sus variaciones, incluyendo posición, velocidad y tiempo de vida.

## Requisito D

**D.1)** → `ParticleGenerator.h`: la clase ParticleGenerator almacena un EmitterConfig como modelo de configuración del emisor. Este contiene los parámetros base de las partículas que se generarán.

→ `ParticleGenerator.cpp`, líneas 25-32 método `sample()`: se genera una muestra con posición y velocidad variables, además de un tiempo de vida aleatorio mediante una distribución uniforme en el intervalo configurado.

**D.2)** → `ParticleGenerator.cpp`, líneas 8-15 método `gaussian()`: se utiliza una distribución normal para obtener variaciones aleatorias e independientes en los componentes X, Y y Z de la posición y velocidad.

→ `ParticleGenerator.cpp`, líneas 17-23 método `howMany()`: se determina la cantidad de partículas que se generarán por fotograma mediante una distribución uniforme.

**D.3)**→ `ParticleSystemWithForces.cpp`: el sistema solicita muestras al generador y crea las partículas correspondientes. De este modo, el sistema controla cuándo se generan partículas y cuándo deja de hacerlo. Obsérvese en: `generateParticles()` líneas 18-25; `spawn(muestra)` líneas 27-42; `kill()` líneas 44-58 ;`update()` línea 60-81.

**Controles:**
- `V`: cambiar el tipo sistema.
- `G`: apagar/encender el generador de partículas para el sistema en selección.
- `O`: mostrar información del sistema seleccionado.

## Requisito E