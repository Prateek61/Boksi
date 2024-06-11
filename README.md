# Voxel Engine

## Introduction
This document is a proposal draft for a voxel engine. The engine will be written in C++ and will use OpenGL for rendering. The engine will be designed to be modular and easy to use. The engine will be able to render large worlds with many voxels. 

## Features
- Rendering of voxels
- Loading of voxel data from files
- Event system : input, window events, etc.
- Camera system
- Collision detection (maybe)
- Particles (maybe)
- Post processing (maybe)
- Physics (maybe)
- Lighting (maybe)

## Design
The engine will be designed to be modular. The engine will be split into multiple modules. The modules will be:
- Core
- Renderer
- Event
- Camera
- Entity Component System
- Logging System

### Core
The core module will be responsible for initializing the other modules, running the main loop and cleaning up the engine.

### Renderer
This module will be responsible for rendering the voxels to the screen. Might architect the renderer to support multiple rendering apis but will use OpenGL for this project. Need to optimize the renderer to render large number of voxels.

#### Optimizations
- Use Chunking: Divide the world into chunks and only render the chunks that are visible to the camera.
- Use frustum culling: Only render the voxels that are visible to the camera.
- Use level of detail: Render the voxels with lower detail when they are far away from the camera.
- Use instancing: Use instancing to render multiple voxels with a single draw call.

### Event
This module will be responsible for handling events such as input events and window events.

### Camera
This module will be responsible for handling the camera. The camera will be able to move around the world and look around. The camera will be able to be controlled by the player.

## Implementation
The engine will be implemented in C++ and will use OpenGL for rendering. The engine will be designed to be modular and easy to use. The engine will be able to render large worlds with many voxels. The engine will be implemented in a way that is easy to extend and add new features to.

## Learn

### 3D Math
- Linear Algebra


### OpenGL
- https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
- https://www.youtube.com/watch?v=45MIykWJ-C4

### Voxel Engine
- https://sites.google.com/site/letsmakeavoxelengine/home
- https://voxely.net/blog/the-perfect-voxel-engine/

### Entity Component System
- https://voxely.net/blog/object-oriented-entity-component-system-design/

### Misc
- https://voxely.net/blog/
