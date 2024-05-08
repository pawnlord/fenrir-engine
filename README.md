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
 - Animations
    - Both generally, and specifically for graphics. This system should make making generally good looking animations for motions and such easy: simply plug in the coordinates and you should get a basic spline that looks "realistic", with some advanced tweaking for the things that matter.
 - Better graphics
    - Currently using the old "boxes and lines" prototype graphics, with ugly colors and no real style. We need sprites, we need shaders, and we need some tooling around these things for it to be usable.
 - UI tooling
    - Once this is done, it will be able to look more complete and pass the "early alpha" phase of development. Likely a goal for later
 - Debugging information and tools
    - Things like hot-reloading, savestates, and further to help make debugging this easier.
    - Also provides tools like serialization that would be helpful for further developments, such as networking and saving data.
    - List of currently needed tools:
        - Logging: As this is a game engine, exceptions are a good idea for some things. A mistake in a program should *never* crash in release. If the state is unrecoverable, pretend it isn't.
        - Hot reloading/save-states: Similar category of tools, which speed up prototype time drastically. It would be nice for the engine to be trivially serializable for most common use cases, which makes save-states a very tempting chioce. But, hot reloading is cool.

## Known bugs
 - Drawing glitch with polygon at angles: Unknown cause, looks like the line is segmented, with artifacts at the edges. Specifically was on the bottom line. Also seen: pentagonal stage had artifact at bottom.