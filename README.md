# PonyGL
A Graphics engine that I made for messing around, so the code may not be quite good, I started this more than a year ago, but decided to make it publicly available.

## Compiling

* Have Qt 5.5 or later
* Gather any necessary libraries (see pne_config.h)
* Compile it, tested under MinGW 32bit.
* Download resouces [here](http://www.mediafire.com/download/2uy4zidgc7g33du/PonyGL_v0.0.1_Resources.zip) and put the res folder in the same directory as the executable.
* Run that frick.

## Using/Playing

### Controls

* WASD keys to move camera and mouse to look around
* Hold CTRL to move faster
* Hold R to move slower
* Use the arrow keys to move the static Rainbow Dash model around (locked to one axis).
* You can use the Insert (Z+), Page Up (Z-), Home (Y+), End (Y-), Delete (X+) and Page Down (X-) keys to rotate the Rainbow Dash model, though it's not quite good (glm's rotation function is confusing af).
