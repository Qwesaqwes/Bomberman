# Bomberman
42 project made by Jimmy Chen Ma, Fabien Del Carmen, Vivien Klaousen and Claudio Mutti.

# Objective
Create our own game engine in C++ and create a Bomberman-like game which uses the engine.

### In-Game screenshots


# Project's Constraints
The project mus follow the following constraints:
- No external library nor asset is to be pushed on the repository, a script must be provided in order to retrieve them.
- Both the engine and the game must be in C++.
- The use of full game engines (such as Ogre) are prohibited.
- The engine must use OpenGL for anything related to graphics.
- The visual assets must be 3D, but the gameplay must stay 2D.
- Provide a way to animate the 3D assets.
- Provide a way to play music and sounds.
- Provide a way to change at runtime: screen resolution, windowed or full screen mode, key bindings, music and sounds volumes.
- There must be a campaign mecanic, quitting the game won't lose any progress.
- This project is about actually finishing a credible product, not toying with OpenGL.

# Requirements
The "install.sh" script is to be launched right after you clone the repo in order to install all dependencies and download all assets.
The script will work for both MacOS and Linux, you just have to check that the "curl" and "unzip" commands runs on your computer.

Additionally, for Linux the following libraries may need to be installed:
* libxinerama-dev
* libopenal-dev
* libxcursor-dev
* libvorbis-dev
* libflac-dev

# How to launch
- If you run "install.sh" without problems the game will launch by itself at the end of the script.
- If you want to play a second time after having quit the game run this command from the root of the repository: "$> ./buld/super_bomberman".
- If you had to fix some dependecies and have to build the game then launch "$> make -C build  && ./build/super_bomberman".
- A "uninstall.sh" script is also provided if you want to clean up all dependencies and assets.

# How to play
Your goal is to clear all the levels that we created, some obtacles are destructibles using bombs and they can be used to kill enemies to. In order to win a level you must find a portal, it can be dropped by destroying a particular obstacle or enemy depending on the level.
Here is the key mapping:
- WASD keys to move around, they can be configured otherwise in the Settings panel of the main menu.
- Spacebar to put a bomb down, you can put a maximum of 3 bombs at the same time.
- Left-shift to kick bombs, you have to find the perk that gives you this ability in order to be able to do it.
- To trigger a portal or pick-up a perk just walk over it.
- Pressing the "esc" key will pop-up the pause menu.
- Pressing the "\`" key (backquote) will put you in debug mode, press the key again to resume game.

# Disclaimer
We do not own any of the assets used for this game, all ownership and credit goes to the respective creators.
