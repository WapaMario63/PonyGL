# PonyGL
Graphics engine that I am making just for learning 3D graphics, it might as well be the graphics pipeline for PoneNgine if it goes anywhere. Keep in mind the code may be messy.

## Compiling

* Have Qt 5.7 or later
* Link the following libraries in the PonyGL.pro file:
  * GLFW
  * glew
  * SOIL2
  * SFML (for audio)
  * Assimp
  * Freetype (currently not used, but it will be in the future)
* Load your own stuff (models, sounds, shaders, etc.) in the run() function and load them in the game loop inside PonyGL.cpp.
* Compile it (tested under MinGW 32bit)
* If you want, you can use the old PonyGL resources and load them, which are available [here](http://www.mediafire.com/download/2uy4zidgc7g33du/PonyGL_v0.0.1_Resources.zip) 
* Run that frick.

## Using/Playing

### Controls

* WASD keys to move camera and mouse to look around
* Hold CTRL to move faster
* Hold R to move slower
