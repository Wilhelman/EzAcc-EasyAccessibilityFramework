# EzAcc: EasyAccessibilityFramework
An accessibility library from scratch in c++ with the ultimate goal of being used by video game developers and thus preventing users with functional diversity from being excluded from the use of the final product. 

[Github repository](https://github.com/Wilhelman/EzAcc-EasyAccessibilityFramework)

_Developed by [Guillermo García Subirana](https://github.com/Wilhelman)_

## Installation instructions (Framework)

Download the zip file and unzip it. Open the folder, obtain the files compiled from the last release of EzAcc. Specifically, the EzAcc.dll, EzAcc.lib files and all the library header files (.h). Once you have all the files in our project, load the library's .lib file from code and locate the file in your project.

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game engine._

## Installation instructions (Demo Project)

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy! You can find the audio and textures in the _assets_ folder.

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game engine._

## Controls in the Demo Project

* **Move Left:** Left keyboard arrow | gamepad
* **Move Right:** Right keyboard arrow | gamepad
* **Move Up:** Up keyboard arrow | gamepad
* **Move Down:** Down keyboard arrow | gamepad
* **Punch input:** 'P' key in keyboard | 'X' in gamepad
* **Move Left:** 'K' key in keyboard | 'B' in gamepad

## **Notes on performance**

There are one main known case where the overall perfomance might decrease significantly:

* **Perform pixel modification functionality** This feature has a high performance cost for two reasons:
* The calculations are done on the CPU instead of the GPU (as a shader calculation would do which is something left for future work).
* Depending on the size of the texture to render, the iteration can overload the logical process.
It is for these reasons that the use of this function is recommended in static elements of the screen, which require very few rendering updates. (E.g. interface or non-animated backgrounds)

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
