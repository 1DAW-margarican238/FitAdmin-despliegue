Módulo de control de torno (ESP32 + PN532)

Este módulo gestiona el acceso físico al gimnasio mediante lectura de chapas NFC. Cuando un usuario acerca su chapa al lector, el ESP32 captura el UID, lo envía al servidor y este lo valida contra la base de datos. Si la chapa está registrada, se abre el servo y se registra el acceso con el usuario y gimnasio correspondientes.

> ⚠️ Nota para la defensa: Antes de la demostración será necesario modificar en el código del ESP32 el nombre y contraseña de la red WiFi, y la IP del servidor, para adaptarlo a la red del centro.
