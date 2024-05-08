# fenrir
A game engine for 2D games. Built using Raylib.

## engine layout

```
context
|-> Scene
    |-> Entity
    | |-> Component
    |...
    | |-> Tag
    |...
    ...
|-> Scene
    |->...
```
Context: The window
Scene: The current set of objects to be displayed. This draws images
Entity: Contains the texture and transform, holds components
Component: A behavior, something that runs every frame
Tag: A piece of information about the entity. Currently unused

## Physics
There is a naive physics engine, that can handle inelastic collision against "floating" (unmoving) objects. All collisions are handled through polygons, though currently only rectangles have been implemented.

## Input
Input is done through closures to store state. The current idiom will be returning a closure from an object that will then be able to act upon it.  
2 buttons cannot be mapped to the same thing currently.  
2 functions cannot be mapped to 2 buttons currently.  
The only things buttons need to know about the outside world is their current state and a delta-time for smooth control.  

## TODO
1. Better debug information
2. Polygonal collisions