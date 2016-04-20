# cs354r-final-game-project
Repo for game technology final game project.

### Contributors
| Name           | Github         |
|----------------|----------------|
| Phoenix Bishea | phoenixbishea  |
| Ryan Lorey     | RyFry          |
| Adam Miller    | Zin20          |
| Luben Popov    | lpopov101      |

### Dependencies
Currently this project only runs on Unix systems.
In order to build and run the project, the following dependencies are required:
```
GLFW (Included with the project in the lib/ folder)
OpenGL
GLU
GLEW
pthread
Xrandr
Xxf86vm
Xi
Xinerama
X11
dl
Xcursor
```
All of these dependencies are what GLFW depend on, and most of them will be preincluded in a Unix install.

### Building and Running
To build and run:
```
cd path/to/repository
make run
```
If all of the dependencies are met, the project should load and run.

### Known issues
```
/usr/bin/ld: cannot find -lglfw3
```
If you get this error, type the following commands from the root directory
```
cd lib
ln -s libglfw.so.3.1 libglfw3.so
```
This will create a symbolic link to the proper GLFW library that your linker will be able to detect.
This is already a known issue on Arch Linux.

### Easy-to-fix errors
If you see this:
```
terminate called after throwing an instance of 'std::runtime_error'
  what():  ***** src/Model.cpp, line 71, process_mesh() : mesh->mVertices is NULL *****
```
Then you probably forgot to export your normals when you exported your Blender file as a .obj!
When exporting on the left-hand side make sure you check the "Include Normals" option, otherwise
you will get this error.