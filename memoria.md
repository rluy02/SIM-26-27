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
