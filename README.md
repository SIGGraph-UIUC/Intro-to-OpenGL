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

## Credits
Since we usually give this OpenGL tutorial in the fall semester and we start model / texture loading around September - October, we chose some free fall-related assets to use:
- [Fall Leaves Image from pxhere](https://pxhere.com/en/photo/1354873)
- [Skull Model by martinjario](https://sketchfab.com/3d-models/skull-downloadable-1a9db900738d44298b0bc59f68123393)
