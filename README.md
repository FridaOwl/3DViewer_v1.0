# 3DViewer v1.0

### Implementation of 3DViewer v1.0, a program to view 3D wireframe models in the C programming language. The models themselves must be loaded from .obj files and be viewable on the screen with the ability to rotate, scale and translate.

## Important notes

- The program developed in `C` language of C11 standard using gcc compiler with Qt framework and additional `Qt` libraries and modules for GUI part(this is my first experience with `C++`).
- The program code located in the src folder
- The program built with Makefile which contains standard set of targets for GNU-programs: `all`, `install`, `uninstall`, `clean`, `dvi`, `dist`, `tests`, `gcov_report`.
- Written code follows the Google style. Legacy and outdated functions are not used according to POSIX.1-2017 standard(see `clang` goal in Makefile).
- Integration tests covered all of the library's functions by unit-tests using the `check.h` library(see `test` goal in Makefile).
- Unit tests coverage checked using gcov. It provides a gcov report in the form of an html page(see `gcov_report` goal in Makefile).
- The programs developed according to the principles of structured programming, duplication in the code is avoided

## Supported operations and functions

- There is one model on the screen at a time
- The program provide the ability to:
    - Load a wireframe model from an obj file (vertices and surfaces list support only).
    - Affine transformations
      - Translate the model by a given distance in relation to the X, Y, Z axes.
      - Rotate the model by a given angle relative to its X, Y, Z axes.
      - Scale the model by a given value.
- The graphical user interface contains:
    - A button to select the model file and a field to output its name.
    - A visualisation area for the wireframe model.
    - Button/buttons and input fields for translating the model.
    - Button/buttons and input fields for rotating the model.
    - Button/buttons and input fields for scaling the model.
    - Information about the uploaded model - file name, number of vertices and edges.
- The program correctly processes and allows user to view models with details up to 100, 1000, 10,000, 100,000, 1,000,000  vertices without freezing (a freeze is an interface inactivity of more than 0.5 seconds)

### Other 

- The program allows to customize the type of projection (parallel and central)
- The program allows setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices
- The program allows choosing the background color
- Settings are saved between program restarts
- The program allows saving the captured (rendered) images as bmp and jpeg files.
- The program allows recording small screencasts by a special button - the current custom affine transformation of the loaded object into gif-animation (640x480, 10fps, 5s)
