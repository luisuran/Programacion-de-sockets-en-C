# Programacion-de-sockets-en-C
Para ejecutar los archivos seguir los siguientes pasos
<ol>
<li>Compilar ambos archivos con cc "nombrearchivo.c" -o "nombrearchivo"</li>
<li>Abrir dos terminales</li>
<li>Ejecutar ./server nro_puerto <br>
			En éste caso server es el nombre del archivo<br>
			nro_puerto puede ser cualquiera mayor a 1024</li>
<li>Ejecutar ./client direccion_ip nro_puerto<br>
			Si se van a ejecutar cliente y servidor en el mismo host la dirección ip es la de loopback (127.0.0.1)<br>
			El nro_puerto debe coincidir con el del servidor</li>
</ol>
