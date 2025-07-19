<img width="2500" height="587" alt="BasiCAD Banner" src="https://github.com/user-attachments/assets/36da34cd-57e4-4566-bfdd-b6900b62aaaf" />

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

<img width="1284" height="938" alt="image" src="https://github.com/user-attachments/assets/51a1f49e-41a1-4fde-9340-0ad065889c20" />

You can add and manipulate shapes using the keyboard shortcuts listed on the Controls section below. The camera can be manipulated using the mouse controls.

<img width="1280" height="936" alt="image" src="https://github.com/user-attachments/assets/2794be0d-92eb-4feb-bdac-1bbfa571d1cf" />

If you need a 4 views mode, you can toggle it with the `V` key.

<img width="1283" height="942" alt="image" src="https://github.com/user-attachments/assets/05320992-b8bf-4bb2-a158-bb552ae12ffd" />

The application also supports loading .obj files from the Resources folder using the number keys `1 - 6`.

<img width="1286" height="937" alt="image" src="https://github.com/user-attachments/assets/37a57fa8-fbee-40df-b793-005eb9e10544" />

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
