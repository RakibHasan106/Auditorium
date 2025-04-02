# Virtual Auditorium: 3D Auditorium Experience with OpenGL 3.30

## Overview
This project is a 3D virtual auditorium simulation built using OpenGL, GLFW, and GLM. It demonstrates advanced computer graphics techniques, including procedural generation, interactive elements, and realistic lighting. The scene includes a main hall, seating arrangements, a stage, a second-floor balcony, and dynamic components like rotating fans and interactive doors.


project [video](https://youtu.be/HnwnZD2MCEs?si=MmbQjxUY69W_cA_4)[report](https://drive.google.com/file/d/1x0qOlZ7DTbgnkuJnFqrU2YpvXDEpx-XH/view?usp=sharing)[presentation](https://docs.google.com/presentation/d/1fqUp17Vv382MAAyRAIplRBhUSdO33WJW/edit?usp=sharing&ouid=105724486781482342064&rtpof=true&sd=true)


## Features
- **3D Environment:** Floors, walls, stage, second-floor balcony with railings and stairs.
- **Dynamic Lighting:** Point lights, spotlights, and directional lighting with ambient, diffuse, and specular components.
- **Interactivity:** 
  - Fan rotation (activated via `R` key).
  - Door open/close functionality (`O/P` keys).
  - Camera movement using `WASD` keys.
- **Procedural Generation:** 
  - Fractal trees using recursive algorithms.
  - Bézier curves for curved structures like the roof.
- **Texture Mapping:** Applied to floors, curtains, and objects for realism.
- **Optimized Rendering:** Uses VAOs and VBOs for smooth performance.

## Technologies Used
- OpenGL 3.30
- GLFW (Window management and input handling)
- GLM (Mathematics library for 3D transformations)

## Implementation Details
### 1. Bézier Curves
Used for modeling smooth, curved surfaces like the auditorium roof. Defined by control points and interpolated using polynomial functions.

### 2. Fractal Trees
Generated recursively with adjustable branching angles, depth, and scaling factors for natural growth patterns.

### 3. Phong Lighting Model
Simulates realistic surface illumination with:
- **Ambient Light:** Constant illumination.
- **Diffuse Light:** Light scattering based on surface normals.
- **Specular Light:** Highlights from reflected light.

### 4. Texture Mapping
2D textures mapped onto 3D surfaces using UV coordinates to enhance visual fidelity.

### 5. Camera and Transformations
- **View Matrix:** Controls camera position and orientation.
- **Projection Matrix:** Defines perspective or orthographic viewing frustum.
- **Keyboard Navigation:** `WASD` for movement, mouse for orientation.

### 6. Rendering Optimization (VAO/VBO/EBO)
- **VAO (Vertex Array Object):** Configures vertex attributes.
- **VBO (Vertex Buffer Object):** Stores vertex data.
- **EBO (Element Buffer Object):** Optimizes rendering by reducing redundancy.

## Controls
| Action | Key |
|--------|-----|
| Move Camera | W, A, S, D |
| Rotate Fan | R |
| Open Door | O |
| Close Door | P |
| Toggle Lights | L |

## Outputs (Screenshots)
- **Stage and seating area**
- **View from the second floor**
- **Rotating fan and hanging lights**
- **Interactive doors (opened/closed states)**
- **Fractal trees and textured objects**

## Challenges & Future Enhancements
### Challenges
- Bézier curve vertex calculations required optimization.
- Balancing Phong lighting components to avoid over-saturation.
- Lack of shadows and collision detection.

### Future Enhancements
- **Shadow Mapping**: Improve realism with real-time shadows.
- **Collision Detection**: Prevent the camera from passing through walls.
- **Particle Effects**: Simulate realistic fire, smoke, or other visual effects.

## References
- [Learn OpenGL by Joey de Vries](https://learnopengl.com/)

---
This project serves as an educational tool for understanding OpenGL concepts, shaders, and 3D transformations. Contributions and feedback are welcome!
