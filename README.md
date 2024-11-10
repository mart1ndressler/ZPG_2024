# ZPG (Basics of Computer Graphics)

This repository contains a project for the "Základy počítačové grafiky" (Basics of Computer Graphics) course at VSB. The project is focused on creating a 3D graphics application using OpenGL and C++.

## Project Overview

The main objectives of this project include:

- Implementing various 3D scenes with different types of objects, including basic shapes like triangles, squares, and complex models such as trees and bushes.
- Applying various shading techniques, including Phong shading, Lambert shading, and Blinn shading, to create realistic lighting effects.
- Using object-oriented programming principles to manage scenes, transformations, and shaders effectively.
- Implementing a camera system that allows for dynamic movement and multiple viewing perspectives.
- Supporting multiple light sources, including dynamic, moving lights that simulate 'will-o'-the-wisps' in a forest scene.
- Integrating attenuation for lights, ensuring that light intensity decreases with distance for a more realistic effect.
- Enabling dynamic rotations for certain objects and random translations to simulate natural movement, like rotating trees or moving lights.

## Features

- **Multiple Scenes**: The project contains multiple predefined scenes, each showcasing different models and shaders, including:
  - A basic scene with a single triangle.
  - A forest scene with multiple trees and bushes, along with dynamic lighting.
  - A Phong shading scene featuring spheres.
  - A scene demonstrating different shading models (Phong, Lambert, Blinn, and Constant) on various shapes.
- **Lighting and Shading**:
  - Supports multiple lights with different colors and positions.
  - Implements lighting models for realistic rendering, including ambient, diffuse, and specular reflections.
  - Uses distance-based attenuation to simulate realistic light fall-off.
- **Dynamic Object Transformations**:
  - Objects can rotate dynamically, such as rotating trees in the forest.
  - Moving lights simulate 'will-o'-the-wisps,' with random translation for natural movement.
- **Camera Controls**: Enables navigation through scenes with camera movements (WASD keys) and mouse controls. Different cameras are used in different scenes to showcase various perspectives.
- **Shaders**:
  - Uses GLSL shaders for custom lighting and material effects.
  - Implements various shading models, including Phong, Lambert, Blinn, and Constant shading.
- **Transformation System**:
  - Includes composite transformations for scaling, rotation, and translation of objects.
  - A composite transformation system allows for reusable and flexible transformations across objects.
