# Desarrollo de un Sistema Operativo para Raspberry Pi 2

Este repositorio contiene el código fuente y las herramientas del Trabajo Final de Grado de tres alumnos de la Universidad Complutense de Madrid sobre el desarrollo de un Sistema Operativo totalmente funcional para la Raspberry Pi 2 con el fin de facilitar y agilizar el proceso de actualización y mejora del contenido y prácticas de asignaturas de las ingenierías de la Facultad de Informática de la UCM como Sistemas Operativos, Ampliación de Sistemas Operativos y Redes, Ingeniería del Software, Estructura de Datos y Algoritmos, etc. Además, se toman los primeros pasos para crear un ámbiente educativo entorno a este repositorio.

## 🏁 | Como empezar

Se recomienda trabajar en una distribución Linux.

1) Para emular la Raspberry Pi 2 con el Sistema Operativo se ha optado por usar QEMU: una serie emuladores open source de microprocesadores, en concreto el de ésta placa. 

```
$ sudo apt-get install qemu
```

2) Clonar el proyecto para tener las herramientas de depuración, el compilador y el Makefile para compilar y ejecutar el código fuente.

```
$ git clone https://github.com/dacya/tfg1920-raspiOS
```

3) Si se desea ejecutar el sistema en la Raspberry Pi 2 física debe instalarse primero en la tarjeta microSD el sistema operativo de código abierto Raspbian desarrollado por la comunidad para obtener los binarios del sistema de arranque.

    [Tutorial de instalación](https://www.raspberrypi.org/downloads/)

## 🏗️ | Compilación

Para compilar el SO para el emulador es tan fácil como ejecutar el Make con la regla build en la raíz del repositorio.

```
$ make build
```

En cambio, si se quiere ejecutar para la placa hardware se usa la siguiente regla:

```
$ make build_hard
```

## 🏃‍♀️ | Ejecución

Como ya se ha explicado, se puede usar QEMU para simular la Raspberry Pi 2. Para ello, se puede usar la siguiente regla:

```
$ make run
```

Para ejecutar el sistema en la placa física debe moverse el archivo generado en /build/kernel7.img a la tarjeta microSD sustituyendo así la imagen de Raspbian.

## 👷 | Autores

* **Alejandro Cancelo Correia** - [@AlexCCo](https://github.com/AlexCCo)
* **Tomás Golomb Durán** - [@ToGo347](https://github.com/ToGo347)
* **Raúl Sánchez Montaño** - [@Rasan98](https://github.com/Rasan98)

## 🎬 | Director

* **José Luis Risco Martín** - [@jlrisco](https://github.com/jlrisco)

## ✔️ | Licencia

Este proyecto está bajo la licencia MIT - visitar [LICENSE.md](LICENSE.md) para más detalles.
