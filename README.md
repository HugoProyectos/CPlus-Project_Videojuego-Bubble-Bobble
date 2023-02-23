# Videojuegos-5G
Repositorio orientado al desarrollo del trabajo de la asignatura de videojuegos.

# Crear nuevo elemento en la lista de proyectos
1. Crea carpeta al nivel en el que se encuentra el archivo .sln.
2. Dentro de ella, copia tu carpeta src y un archivo "premake5.lua".
3. De este archivo, solo cambia la siguiente línea: project (workspaceName .. "1").
workspaceName es una variable global del proceso.
<..> es el operador para concatenar strings.
4. Reiniciar Visual Studio.
5. SI NO HA FUNCIONADO, volver a ejecutar "premake-VisualStudio.bat".

# Proyectos de VisualStudio
El único proyecto que debe permanecer en VisualStudio por precaución es el denominado "raylib". Este es el proyecto asociado
a la librería. Cada proyecto viene definido por el archivo "premake5.lua" contenido en su directorio raíz. El nombre del proyecto
viene dado por el fichero "premake5.lua" como se explica en la sección anterior.

Por ejemplo, el proyecto "Videojuegos-5G1" está asociado al contenido de la carpeta "game1".

Para añadir nuevos ficheros al proyecto (que es el entorno que permite depurar) hay que seguir los pasos indicados en la siguiente
sección.

# Al crear un nuevo archivo para el proyecto
1. Acceder a la vista de carpetas de VisualStudio (mirar imagen ComoAlternarVistas.png).
2. En la carpeta a la que se quiere añadir el nuevo archivo (main.cpp, por ejemplo) click derecho -> Agregar -> Nuevo archivo.
3. A continuación hay dos opciones:
	4.1 Si creamos muchos archivos:
		4.1.1 Cerrar VisualStudio.
		4.1.2 Ejecutar "premake-VisualStudio.bat".
		4.1.3 Volver a abrir VisualStudio.
	4.2 Si solo hemos añadido un archivo, o pocos:
		4.2.1 Volver a nuestro ".sln" ("Videojuegos5G.sln").
		4.3.2 Hacer click derecho en el proyecto al que lo queremos añadir -> Agregar -> Elemento existente.
		4.3.3 En el explorador de archivos que sale, elegir el archivo que hemos creado.

