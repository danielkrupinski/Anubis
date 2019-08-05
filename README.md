# Anubis [![C](https://img.shields.io/badge/language-C-%23f34b7d.svg)](https://en.wikipedia.org/wiki/C) [![CS:GO](https://img.shields.io/badge/game-CS%3AGO-yellow.svg)](https://store.steampowered.com/app/730/CounterStrike_Global_Offensive/) [![Windows](https://img.shields.io/badge/platform-Windows-0078d7.svg)](https://en.wikipedia.org/wiki/Microsoft_Windows) [![x86](https://img.shields.io/badge/arch-x86-red.svg)](https://en.wikipedia.org/wiki/X86) [![License](https://img.shields.io/github/license/danielkrupinski/Anubis.svg)](LICENSE)

Free open-source training software / cheat for **Counter-Strike: Global Offensive** game. Designed as internal cheat - [Dynamic-link library](https://en.wikipedia.org/wiki/Dynamic-link_library) (DLL) loadable into game process. Compatible with the latest version of the game on Steam.

## Features

* **Glow** - render glow effect on entities
* **Misc** - miscellaneous features
* **Config** - JSON-based configuration system

<details>
<summary>Features in depth</summary>
* **Misc** - miscellaneous features
    * **Auto strafe** - automatically strafe in air following mouse movement
    * **Bunny hop** - automatically simulate space bar press / release while jump button is being held; increases movement speed
</details>

## Getting started

### Prerequisites
Microsoft Visual Studio 2019 (preferably latest version i.e. 16.0.3), platform toolset v142 and Windows SDK 10.0 are required in order to compile Anubis. If you don't have ones, you can download VS [here](https://visualstudio.microsoft.com/) (Windows SDK is installed during Visual Studio Setup).

### Cloning
The very first step in order to compile Anubis is to clone this repo from GitHub to your local computer. Git is required to step futher, if not installed download it [here](https://git-scm.com). Open git bash / git cmd / cmd and enter following command:
```
git clone https://github.com/danielkrupinski/Anubis.git
```
`Anubis` folder should have been succesfully created, containing all the source files.

### Compiling from source

When you have equiped a copy of source code, next step is opening **Anubis.sln** in Microsoft Visual Studio 2017.

Then change build configuration to `Release | x86` and simply press **Build solution**.

If everything went right you should receive `Anubis.dll`  binary file.

### Loading / Injecting into game process

Open your favorite [DLL injector](https://en.wikipedia.org/wiki/DLL_injection) and just inject `Anubis.dll` into `csgo.exe` process.

When injected, menu is openable under `INSERT` key.

