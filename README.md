# EzAcc: EasyAccessibilityFramework
An accessibility library from scratch in c++ with the ultimate goal of being used by video game developers and thus preventing users with functional diversity from being excluded from the use of the final product. 

* [Github repository](https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework)

## Installation instructions (Framework)

Download the zip file and unzip it. Open the folder, obtain the files compiled from the last release of EzAcc. Specifically, the EzAcc.dll, EzAcc.lib files and all the library header files (.h). Once you have all the files in our project, load the library's .lib file from code and locate the file in your project.

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game engine._

## Installation instructions (Demo Project)

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy! You can find the audio and textures in the _assets_ folder.

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game engine._

## **Notes on performance**

There are one main known cases where the overall perfomance might decrease significantly:

* **Only the first time** you execute the engine the app will "freeze" as it's importing all the assets and generating its corresponding resources. The time will vary depending in the number and sizes of files that are being imported.
* If the z-buffer visualization is activated.
* When an animated model is loaded on scene **(especially if its playing its animation)** the engine perfomance might drop a lot. That's because the skinning process of deforming the mesh to match the bones positions of the corresponding skeleton needs to do a lot of matrix related operations multiple times per frame. As we don't use shaders in this engine all these operations are sended to the CPU (instead of the GPU) which is not prepared to handle matrix operations as well as the GPU does. 

## Controls in the Demo Project

* **Move Left:** Left keyboard arrow | gamepad
* **Move Right:** Right keyboard arrow | gamepad
* **Move Up:** Up keyboard arrow | gamepad
* **Move Down:** Down keyboard arrow | gamepad
* **Punch input:** 'P' key in keyboard | 'X' in gamepad
* **Move Left:** 'K' key in keyboard | 'B' in gamepad

### Gizmos

* **W/E/R:** Change gizmo to Translate, Rotation and Scale

### Assets controls

* **Load scene:** Double left clic

### Camera

* **Free look around:** Right Click Mouse Button
* **Orbit around object:** LALT + Left Click Mouse Button
* **Zoom in/out** Mouse wheel
* **Pan**: Middle mouse button
* **Duplicate movement speeed**: Hold Shift key
* **Focus camera on object**: F key

### UI 
* **Open config window**: 8
* **Open console**: 7 
* **Open Inspector**: I key
* **Show/Hide UI:** LALT + G key
* **Quit application:** ESC / Alt+F4

## Animations
* **Play punch animation**: 1
* **Play walk in place animation**: 2

_Click again on Window->(any option) to close that window_

## How to handle animations

* Left click in the the Warlock gameobject in the hierarchy which represents the character mesh.
* In the inspector scroll down until you find the animation component.
* In this component the animation can be played, paused, move frames forward and backward and you can move the timeline too if needed. * * Down below these controls you can change the animation speed and if you want it in loop and to interpolate between frames.
* Below this you'll find the available animations for this mesh which if clicked will be loaded into the character mesh. 
* Pressing 1 the character will perform a punch attack.
* Pressing 2 the character will blend to the walk in place animation (blend time is set to 1s). If 2 is released character will return to the idle animation.

## Innovations / Redesign

* Draw the z-buffer in grayscale option determined by the distance (Option in Config window -> Renderer3D options -> Switch Z- Buffer Visualization).

**_Warning: If this option is activated the overall performance might decrease significantly._**

Example for comparison:
![image](https://user-images.githubusercontent.com/25589509/46920311-ccccb780-cfec-11e8-9afa-fcbf64616042.png)


## Developed by [Guillermo García Subirana](https://github.com/Wilhelman)

## Tools used
* IDE: Microsoft Visual Studio 2019
* Language: C++
* Containers: STL
* Input and audio: SDL 2.0.8
* Graphics: OpenGL
* GUI: Deat ImGui 1.65
* Image loader: Developer's Image Library (DevIL) 1.8.0
* Data persistance: XML - Pugui:XML 2017
* File system: PHYSFS 3.0.1
* Code repository: GitHub
* Others: Adobe Photoshop CS6 / MS Paint / Aseprite
