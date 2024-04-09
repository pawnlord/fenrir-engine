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

## physics layout

```
Component -> Physics
Entity -> PhysicsEntity
PhysicsEntity contains {Physics, private physics_tag}