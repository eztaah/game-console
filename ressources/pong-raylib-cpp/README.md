<div align="center">

# pong

*Developed in August 2023* 

<br>

"A classic Pong game where your goal is to score as high as possible against an unbeatable bot."  
Use `Z`, `S` and `Space` keys to move inside the menu.

<br undefined>

Made with the C++ programming language and the [raylib](https://www.raylib.com/) graphics library. 

<br undefined>

## Download the game

<a href="https://github.com/EzTaah/pong/releases/download/v1.1/pong-for-windows.zip"> <img src="https://github.com/EzTaah/pong/blob/main/docs/other/windows.png?raw=true" height=70 hspace=1> </a>
<br undefined>
<br undefined>
<br undefined>
</div>

---

# Building and Development

**This game was built on Windows with [CMake](https://cmake.org/download/) and the [MinGW](https://winlibs.com/) compiler.**

<br undefined>

**Here are the compilation instructions :**


1. Open a PowerShell terminal and navigate to the project's directory.

2. Create a build directory : 
```bash
mkdir build
```

3. Navigate to the build directory : 
```bash
cd build 
```

4. Generate the makefiles (Choose between the debug and the release version) :
```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
```
```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
```

5. Compile the project :
``` bash
mingw32-make 
```

5. Run the game (Choose between the debug and the release version) :   
```bash
.\debug.exe
```
```bash
.\release.exe
```
 
