# ZPG (Basics of Computer Graphics)

This repository contains a project for the "Základy počítačové grafiky" (Basics of Computer Graphics) course at VSB. The project is focused on creating a 3D graphics application using OpenGL and C++.

## Project Overview

The main objectives of this project include:

- Implementing various 3D scenes with different types of objects, including basic shapes like triangles, squares, and complex models such as trees and bushes.
- Applying various shading techniques, including Phong shading, Lambert shading, and Blinn shading, to create realistic lighting effects.
- Using object-oriented programming principles to manage scenes, transformations, and shaders effectively.
- Implementing a camera system that allows for dynamic movement and multiple viewing perspectives.
- Supporting multiple light sources, including dynamic, moving lights that simulate 'will-o'-the-wisps' in a forest scene.
- Adding support for distance-based attenuation, ensuring light intensity decreases realistically with distance.
- Enabling dynamic rotations for certain objects and random translations to simulate natural movement, like rotating trees or moving lights.
- Incorporating a **skybox** feature, allowing realistic environmental mapping and adding an action to pause the skybox from following the camera to display it as a stationary cube.

## Features

### 1. **Multiple Scenes**
The project contains multiple predefined scenes, each showcasing different models and shaders, including:
- A basic scene with a single triangle.
- A forest scene with multiple trees and bushes, along with dynamic lighting.
- A Phong shading scene featuring spheres.
- A scene demonstrating different shading models (Phong, Lambert, Blinn, and Constant) on various shapes.

### 2. **Lighting and Shading**
- **Multiple Lights**:
  - Supports multiple lights with different colors and positions.
  - Dynamic, moving lights simulate natural phenomena like 'will-o'-the-wisps.'
- **Realistic Lighting**:
  - Implements ambient, diffuse, and specular reflections for more realistic rendering.
  - Supports attenuation to ensure light intensity diminishes with distance.
- **Shader Models**:
  - Implements various shading models using GLSL shaders, including Phong, Lambert, Blinn, and Constant shading.

### 3. **Dynamic Object Transformations**
- **Object Movement**:
  - Objects, like trees, rotate dynamically in specific scenes.
  - Lights randomly move, simulating natural, dynamic effects.
- **Skybox Feature**:
  - Realistic environmental cubemap mapping for the background.
  - Added action to toggle the skybox to pause its following behavior and display it as a stationary cube in the scene.

### 4. **Camera Controls**
- **Navigation**:
  - Camera movement is supported via WASD keys and mouse controls for free navigation.
- **Multiple Views**:
  - Different scenes utilize different camera setups to showcase various perspectives dynamically.

### 5. **Shader System**
- **Custom GLSL Shaders**:
  - Vertex and fragment shaders are used to implement lighting and material effects.
- **Real-Time Updates**:
  - Shaders react dynamically to changes in light sources, materials, and transformations.

### 6. **Transformation System**
- **Composite Transformations**:
  - Scaling, rotation, and translation transformations are handled through reusable components.
  - Composite transformations allow flexible and hierarchical object manipulation.

### 7. **Skybox Implementation**
- A skybox is used to add a realistic background to the scenes.
- Includes textures for environmental mapping, with the ability to stop the skybox from following the camera, turning it into a stationary cube.

## Running the Project
1. **Dependencies**:
   - OpenGL
   - GLFW
   - GLM
   - GLEW
   - SOIL (Simple OpenGL Image Library)
2. **Compilation**:
   - Use a C++ compiler with OpenGL, GLFW, GLM, GLEW and SOIL linked.
3. **Controls**:
   - `WASD`: Camera movement.
   - Mouse: Look around.
   - `T`: Toggle skybox following behavior.
---
This project is a foundation for exploring computer graphics principles and techniques, emphasizing hands-on implementation of realistic 3D scenes and dynamic effects.
