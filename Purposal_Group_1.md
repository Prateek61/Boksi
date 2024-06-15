# Purposal for Boksi: Voxel Engine - Group 1

## Members
- Prateek Poudel
- Yubraj Basnet

## Objectives
- Develop a modular and efficient voxel engine capable of rendering large, complex 3D worlds composed of voxels.
- Support essential functionalities like rendering, event handling, physics, user interface, and more.
- Utilize optimizations like suitable data structures and rendering techniques to enhance performance.

</br>

Note: The project is divided into three groups, each responsible for a specific parts of the engine.

## Description
To understand a voxel engine, it's helpful to start with the concept of a pixel. A pixel, short for "picture element," is a point sample of a 2D signal of color and brightness, typically represented as a tiny square on digital screens. Pixels are the basic building blocks of digital images, forming the basis for rendering visuals in two dimensions.

Similarly, a voxel, or "volume element," is a point sample of a 3D signal of color and brightness, modeled as a tiny cube. Voxels are the three-dimensional counterparts to pixels and are used to construct 3D objects in a digital space. While pixels are used to represent 2D images, voxels are used to build and represent three-dimensional objects and environments.

In a voxel engine, 3D objects are represented as collections of these tiny cubes, arranged in a 3D grid. This approach allows for the creation of detailed and complex 3D worlds where each voxel can be individually manipulated. This provides a high level of control and flexibility in modeling, enabling the creation of intricate and dynamic environments. The voxel engine is the software responsible for loading, rendering, and manipulating these voxel-based 3D objects.

The voxel engine will be structured into multiple components, each responsible for a specific aspect of the engine’s functionality:


- ### Core
    The core component is fundamental to the engine's operation. It will initialize the engine, manage the main loop, and control the overall flow of the program. This component provides the base structure for the engine, ensuring smooth interaction between the various components.

- ### Renderer
    The renderer is crucial for visualizing the voxel world. It will convert the voxel data into a visual representation on the screen. To achieve small voxel sizes, the renderer will use path tracing, a rendering technique that simulates the behavior of light rays, instead of more traditional rasterization techniques to achieve better results for voxel rendering. This will ensure efficient and accurate rendering of the voxel world.

- ### Event System
    The event system will handle all interactions within the engine. This includes managing
    input from the user via keyboard, mouse, and other input devices, as well as handling window
    events such as resizing and closing. By managing these inputs and events, the event system
    will enable users to interact seamlessly with the voxel world.

- ### World Manager
    The world manager will be responsible for the manipulation and storage of voxel world data.
    It will provide functionalities to load, save, and modify the voxel world. This component
    will use efficient data structures, such as Sparse Voxel Octrees (SVO), to manage voxel
    data. These data structures will enhance the efficiency of rendering, collision detection, and
    physics calculations.

## Tools To Be Uses
- C++ for the core engine development
- OpenGL as the rendering API
- GLSL for shader programming
- GLFW for window management
- GLM for mathematics
- Dear ImGui for the user interface
- Git and Github for collaboration

## Output
    The completion of this project will result in a fully function voxel engine with advanced
    features and capabilities, including efficient rendering of large, complex 3D worlds using
    path tracing, and support for event handling, particle effects, collision detection, physics, and
    a user interface. Its modular design will ensure ease of extension and modification, allowing
    developers to add new features or improve existing ones. This versalite tool will be suitable
    for various applications, from game development to simulations, providing flexibility and
    scalability.
