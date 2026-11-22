An implementation of Conway's Game of Life

Made for hackClub terra 26/27  

### How to use:

Please make sure you have raylib installed on your machine

Hit space to step one generation
Drag view with middle mouse button
Scroll to zoom  
Press Q to reset view  

### What it do?

terraLife uses a self-scaling chunk system to allow for more expansive simulations  
Because that took quite sometime, user interactions are purely visual for this ship

### How it do dat?

Using a custom glsl shader, pixels are colored in directly from a gray-scale texture.  
This texture in turn is fed directly from the memory buffer used by the simulation  

### Spying on the futur

Placing pixels
Stepping faster
Custom rule-set
Switching to a compute-shader

