# OctoBoom
OctoBoom - simple match-3 game
2016


###1 Package
Game is packed in the MacOS X application package. It is ready to run on MacOS X starting from 10.9
Game was created with XCode.

###2 How to Play
The goal is to get highest score as possible in one minute. 
To score you need arrange diamonds in line of 3 or more.
You can swap any diamond with neighbour. If this results in a match then matching diamonds are converted 
to score and disappear form the field. Then field collapses and new diamonds fall to fill it.

###3 Dependencies
You don't need to install anything to run this game.
Game uses pure SDL2 framework and is shipped with it built in.
The game was created on Mac so if you want to build it yourself I recommend to use Mac also.
To build the game you should have SDL2 framework installed in /Library/Frameworks/
To run the script that convert resources you need to python 2.7 (it might as well run on python 3)
and a PIL library.

###4 Resources
All resources are copied to the package during build process.
Game resources contain one atlas for all game elements, one atlas config in json,
and a pre-rendered font (it may look shabby but that is how it was ment to look)
Resuorces are created from the raw resources automatically with scripts (run manually only when need to change)
PackAtlas.py collects all images from 'RawResources/sprites' and arranges them in the atlas. 
The description of the atlas elements is stored in json file. There's no neat logic behind this script, it just gets
the job done. 
ConvertFonts.py converts all image files from 'RawResources/font' to the images of the same size but with white symbols on transparent background
The font image should contain 13 symbols per line, of size 46x70 px each. The symbols should be in the following order: a-z, A-Z, 0-9

###5 Code description and build
The game has no other dependencies except pure SDL2 framework. I've used own library vectry for vectors and matrices.
I had to parse json file so I've created tiny json parser. It may not be the most efficient of reliable but it passes
all test.
All manipulations with game field are done in Field class by corresponding FieldState:
![Field States](https://rawgit.com/Teivaz/OctoBoom/master/OctoBoom_FieldStates.svg)

To manipulate objects in the game I've created tree structure of Node classes similar to one in Cocos2d-x.
Memory management is mostly done manually except some shared textures that use refcount.
Drawing is done simply by blitting surfaces.
Tests are covering basic functionality of nodes transformation, json parsing and game field logic checks.
Xcode project has 3 targets: OctoBoom - for creating binary, OctoBoomTest - for test, OctoBoomApp - for building an application
