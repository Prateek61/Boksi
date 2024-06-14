# Purposal for Boksi: Voxel Engine - Group 1

## Members
- Prateek Poudel
- Yubraj Basnet

## Objectives

The primary objective of this project is to develop a modular, efficient, and user-friendly voxel engine capable of rendering large, complex 3D worlds composed of voxels. The engine will support essential functionalities such as rendering, event handling, particle effects, lighting, collision detection, physics, and a user interface. It will be designed with modularity in mind, allowing easy extension and modification to add new features or improve existing ones. The engine should be optimized for performance to handle vast voxel terrains and dynamic environments smoothly. Additionally, it should provide an intuitive interface for developers to integrate their custom functionalities and for users to interact seamlessly with the voxel world. This project aims to create a versatile tool that can be used for various applications, from game development to simulations, ensuring flexibility and scalability.

</br>

Note: The project is divided into three groups, each responsible for a specific parts of the engine.

## Description
Let's start with the definition of a Pixel. We define a pixel as a point sample of a 2D signal of color and brightness, for our case, modelled as a tiny square. Similarly, a voxel is a point sample of a 3D signal of color and brightness, modelled as a tiny cube.

3D objects can be represented as a collection of voxels in a 3D grid. This is the basic idea behind a voxel engine. A voxel engine is a software that can load, render, and manipulate 3D objects represented as voxels.

The engine is divided into multiple components. Each component is responsible for a specific part of the engine. The components are:

- ### Core
    The core component will be responsible for initializing the engine, managing the main loop, and handling the overall control flow of the engine. It will provide the base structure for the engine and manage the interaction between different components.

- ### Renderer
    The renderer component will handle the rendering of the voxel world. It will be responsible for converting the voxel data into a visual representation on the screen. The renderer will be using path tracing to render the voxel world efficiently and accurately.
