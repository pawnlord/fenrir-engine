# engine
A graphics engine for 2d games

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
Tag: A piece of information about the entity

## TODO
1. Better debug information
2. Testing for Rectangle collisions
3. Optimize Rectangle collisions if need be
4. Polygonal collisions
5. Center rectangles