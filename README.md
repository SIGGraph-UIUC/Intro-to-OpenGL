# Intro-to-OpenGL
SIGGRAPH@UIUC's intro to OpenGL

## Cloning and Building
This project relies on Git submodules, so use ``git clone --recursive`` when cloning this project.

Once cloned, CMake can be used to build the project:
```
mkdir build
cd build
cmake ..
```

If you are on Windows and using Visual Studio, the solution and project files will be generated. Otherwise, you 
can run 
`make` in the `build` directory. 
