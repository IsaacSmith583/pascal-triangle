# About

This is a 'continuous' Pascal's triangle visualiser. It usees the gamma function to calculate n choose r for non-integer values, and then uses that to make up Pascal's triangle. The brightness of the pixel corresponds to the value of that area, brighter meaning a higher value. Red pixels are positive values, green are negative, and black is zero.

## Controls
- WASD to move camera
- +/- to zoom the camera
- Spacebar to switch from integer values to continuous values
- 'n' to show negative values outside the triangle
- 'l' to view the values on a logarithmic scale
- q/e to adjust the hard limit on the maximum value (if it outweighs the gradient too much)

## Build
This project uses SDL2 for rendering, so you will need that to run it. Otherwise, the makefile is included, so you should need to do is run 'make' in the directory and the run the executable with './pascal' (not sure if this works with windows...).
