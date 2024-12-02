[Project Webpage](https://ben256.com/projects-3d-shooter.html)

# shooter
3D shooter

## Building

### Shaders
#### Linux
Make sure you have the glsl compiler installed: ```sudo apt-get install glslc```.

To compile the shaders:```make shaders``` or ```make glsl_shaders```.
#### MacOS
Make sure you have the xcode *app* installed first, then
to compile the shaders:```make shaders``` or ```make metal_shaders```.

You can also run ```make glsl_shaders``` if you have the glsl compiler installed on mac.
#### Windows
TODO: Figure it out yourself or use WSL2

### Application
For release mode: ```make rel```

Debug mode: ```make dbg```

> This will automatically build and install SDL3 if you haven't already

You can add ```run```, ```gdb```, or ```lldb``` at the end like this ```make rel run``` to also
run or debug shooter.
