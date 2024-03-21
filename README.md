# engine layout

```
context
|-> Scene
    |-> Entity
    | |-> Component
    |...
    ...
|-> Scene
    |->...
```
Context: The window
Scene: The current set of objects to be displayed. This draws images
Entity: Contains the texture and transform, holds components..