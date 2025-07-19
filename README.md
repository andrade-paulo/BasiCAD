-banner-

**BasiCAD** is a simple 3D CAD program written in C++ using the Direct3D API. This system was created as the second assignment for the course Computer Graphics at the Universidade Federal Rural do Semi-Árido.

## Features
- Creation of 3D basic shapes (cube, sphere, geosphere, cylinder, grid)
- Manipulation of the shapes (translate, rotate, scale)
- Interactable Camera
- Multiple view mode (orthographic and perspective)
- Loading objects from OBJ files

## Technologies Used
- C++
- Direct3D
- Windows API

## Build and Run Instructions
To use BasiCAD, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/andrade-paulo/BasiCAD
   ```

2. Open the solution file in Visual Studio.

3. Build the solution.

4. Run the application.

## Usage
The application will start with a grid in the center of the screen.

- start image -

You can add shapes using the keyboard shortcuts listed on the Controls section below. The camera can be manipulated using the mouse and keyboard controls.

- scene example -

If you need a 4 views mode, you can toggle it with the `V` key.

- 4 views mode -

The application also supports loading .obj files from the Resources folder using the number keys `1 - 6`.

- loaded obj example -

### Controls
- **Mouse Controls:**
  - `Left Click`: Rotate the camera
  - `Right Click` / `Scroll`: Zoom

- **Keyboard Controls:**
  - `H`: Show or hide the help menu.
  - `V`: Toggle between single and multiple view.
  - `ESC`: Close the application.

  - `B`: Add a Box.
  - `C`: Add a Cylinder.
  - `S`: Add a Sphere.
  - `P`: Add a Grid.
  - `G`: Add a Geosphere.
  - `1 - 6`: Load .obj models from the Resources folder.

  - `TAB`: Select the next object.
  - `DELETE`: Remove the selected object.

  - `Arrow Up/Down`: Move in depth (Z).
  - `Arrow Left/Right`: Move laterally (X).
  - `[`: Move up (Y).
  - `]`: Move down (Y).

  - `CTRL + Arrow Up/Down`: Rotate around the X axis.
  - `CTRL + Arrow Left/Right`: Rotate around the Y axis.
  - `CTRL + [`: Rotate around the Z axis (clockwise).
  - `CTRL + ]`: Rotate around the Z axis (counter-clockwise).

  - `+`: Scale up the object.
  - `-`: Scale down the object.