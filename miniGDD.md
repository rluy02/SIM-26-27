# Archery — Tiro con Arco

## 1. Proyectiles y generadores de fuerzas

### Proyectiles
- Diferentes tipos de flechas con propiedades físicas distintas:
  - Masa.
  - Superficie expuesta al aire.
  - Resistencia aerodinámica.
  - Otras propiedades que puedan afectar a su trayectoria.

### Generadores de fuerzas

- **Gravedad:**
  - Aplicable a los proyectiles y/o a diferentes entidades del escenario.
- **Viento:**
  - Modificar la trayectoria de los proyectiles y/o el movimiento de otras entidades.
- **Torbellino:**
  - Similar al viento, pero con un campo de fuerzas rotacional.
  - Valorar si se utiliza como elemento de dificultad adicional, si puede afectar a los proyectiles o si se limita a efectos visuales.
- **Explosiones:**
  - Generar fuerzas como consecuencia de determinados eventos, como la colisión de proyectiles con elementos del entorno.

## 2. PhysX: cuerpos físicos y colisiones

### Cuerpos físicos
- Diana.
- Proyectiles.
- Planos y superficies del escenario.

### Diana
- Posibles geometrías:
  - Cilindro.
  - Rectángulo.
  - Círculo.
- División en anillos o zonas de puntuación.
- La colisión de la flecha con las distintas zonas puede producir diferentes interacciones:
  - Balanceo.
  - Giro.
  - Destrucción.
  - Otras reacciones físicas.

## 3. UI / ImGui

Interfaz destinada tanto a proporcionar información al jugador como a facilitar el desarrollo y las pruebas del proyecto.

Posibles elementos:
- Sistema de puntuación.
- Número de disparos disponibles.
- Información sobre los proyectiles.
- Distancia en metros hasta la diana.
- Feedback de los disparos y colisiones.
- Menús de interacción y selección para el usuario.
- Controles de depuración y visualización de información física.

## 4. Inputs — Por decidir

**Importante:** mantener un sistema de controles sencillo e intuitivo.

### Opción A: Solo ratón
- Movimiento de cámara mediante clic derecho.
- Botón o sección de interfaz para cambiar el tipo de flecha.
- Control de potencia mediante:
  - Slider modificable.
  - Mantener pulsado para aumentar la tensión y soltar para disparar.

### Opción B: Teclado y ratón
- WASD para modificar el ángulo o la dirección de disparo.
- Mantener pulsada una tecla o botón para aplicar la potencia del disparo.
- Tecla para cambiar el tipo de proyectil.
- Ratón para controlar la cámara y/o apuntar.

## 5. Jugador — Por decidir

Definir el funcionamiento del jugador y la experiencia de juego en función del sistema de controles seleccionado.

Aspectos pendientes:
- Sistema de apuntado.
- Control de potencia.
- Selección de proyectiles.
- Mecánica de disparo.
- Posibilidad de movimiento del jugador o posición fija. //MUY PROBABLEMENTE, fija tal cual el juego de Wii

---

## 6. Preguntas para los profesores

### Física y proyectiles

1. **Geometría de los proyectiles:**
   - ¿Deberíamos simular la flecha utilizando un cuerpo físico diferente de una esfera?
   - Por ejemplo, un rectángulo u otra geometría que represente mejor su forma.

2. **Resistencia aerodinámica:**
   - ¿Deberían tener estos cuerpos resistencia aerodinámica para conseguir una simulación más realista?
   - Por ejemplo, ¿sería conveniente ampliar el modelo actual de resistencia del aire (k1), que actualmente esta con la implementación de Stokes.

3. **Fuerza de lanzamiento:**
   - ¿Cómo sería más adecuado modelar la fuerza del disparo?
   - ¿Sería conveniente aplicar dicha fuerza durante un intervalo de tiempo, partiendo de una velocidad inicial nula, para que la velocidad resultante se obtenga mediante la simulación física? o de forma inmediata, como si fuese una explosión.
   - ¿sería preferible utilizar otro modelo de lanzamiento, como un impulso o tensión?

### Diseño y jugabilidad

4. **Control de potencia:**
   - ¿Un slider de 0 a 100 o mantener pulsado un botón para tensar el arco? Depende del control que se vaya a utilizar. En principio me gustaría el mas sencillo posible para el prototipo, p.e un slider mencionado
   - Tambien puede ser una vez accionado cierto input (cancelable) se inicia el  tick de un slider que sube y cuando llega al tope superior baja y cuando llega al tope inferior sube y asi sucesivamente.
