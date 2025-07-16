# Skel
Skel is a small C++ framework for building real-time interactive applications, using OpenGL, GLFW, and ImGui.
---

## Features

- Pixel-perfect 2D surface rendering
- Minimal math types (`float2`, `int2`, etc.)
- Built-in console
- Input management
- Easy-to-extend game loop

![GIF](https://github.com/neokken/Skel/blob/main/exampleGif/Game_tRn5j2sCWa.gif)

## Installation

1. **Clone with submodules**:
   ```bash
   git clone --recurse-submodules https://github.com/neokken/Skel
   ```
2. **Generate the Visual Studio solution**<br>
   Simply run the batch script: GenerateProject.bat
   
3. **Open Skel.sln and build Game**.<br>
   Press F5 to run the application, and watch example in action

> Tip: Switch to the metalgearExample branch for a larger sample (warning: rough code ahead!).

> 
## Quickstart

### Extend GameBase
Gamebase is the core of your application.

Mention how EngineInitValues can be customized (window size, console enabled, etc.).
Show how Initialize, Update, and Render work.

### Surface Rendering
Why do i use surfaces (tmpl8 mention or something)
Explain what a Surface is.
Show how to create one (e.g., new Surface(width, height)).
Show how to clear, draw to, and blit it using the Renderer.


#### CPU vs GPU Usage
Why surfaces have the gputhingy there

#### Drawing primatives on a surface
line
box
bar

### Input Manager
IsKeyDown, WasKeyPressed, WasKeyReleased
ignore blocked?


### UI Panels
what are they?
Show how to register a custom panel class
maybe small example

### Console
Show how to register a custom command class
maybe small example

#### Logging
SKEL_LOG
how this is relevant to the console


