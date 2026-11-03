Ricardo Luy - Proyecto Intermedio



\[InfoGeneral]: La escena con las pruebas del proyecto están en "MProject\_Scene.h" que se carga de forma predeterminada al iniciar el programa. También se puede acceder a través de la tecla numero '5'.



Requisito A.1) --> "ParticleWithForces.cpp" 29-34. En la linea 33 se llama al método real de integración "verlet()" 51-60.

Requisito A.2) --> "ParticleWithForces.cpp" 7-8 atributo masa y el resto de componentes que recibe la constructora. 

&#x20;              --> "MProject\_Scene.h" ~~18-25~~ se definen dos structs de particulas con atributos diferentes parametrizables para la función "apartadoA" 27.

Requisito A.3) --> "ParticleWithForces.cpp" 11 se calcula \_invMass: que almacena la inversa de la masa. En "verlet()" 53 se calcula la aceleración aplicando el principio de la masa.

&#x09;       --> "MProject\_Scene.h" ~~64-65~~ .H se aplica la misma fuerza para ambas particulas, y se deberia de observar en la demo como la de menor masa acelera más.



(Extra): Con la tecla 'i' o 'I' se puede ver información sobre las partículas generadas del apartado A



Requisito B.1) -->



