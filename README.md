# Videojuegos-5G
Repositorio orientado al desarrollo del trabajo de la asignatura de videojuegos

# Crear nuevo elemento en la lista de proyectos
1. Crea carpeta al nivel en el que se encuentra el archivo .sln
2. Dentro de ella, copia tu carpeta src y un archivo "premake5.lua"
3. De este archivo, solo cambia la siguiente línea: project (workspaceName .. "1")
workspaceName es una variable global del proceso.
<..> es el operador para concatenar strings.
4. Reiniciar Visual Studio
5. SI NO HA FUNCIONADO, volver a ejecutar "premake-VisualStudio.bat"
