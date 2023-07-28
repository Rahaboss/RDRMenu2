## Todo list for new features

Here is a list of future features that should ideally be implemented. These are listed in no particular order.

* Vulkan ImGui renderer
	* The game's D3D12 renderer has some bugs like an FPS cap when the window is in focus
	* Experimantal support is enabled with the ENABLE_VULKAN_RENDERER preprocessor
* JOAAT hash reverse lookup
	* A big dictionary mapping hashes to a string representation
* Full model database for peds, objects, etc.
	* Object pool contains unknown hashes that return false for STREAMING::\_IS\_MODEL\_AN\_OBJECT
* Freecam during cutscenes
	* The freecam currently only works during freeplay
* Disable all camera effects
	* ESP seems to be misaligned from the game view when effects are applied such as distortion or camera smoothing
* Debugging support
	* Experimantal support is enabled with the ENABLE_ANTI_ANTI_DEBUG preprocessor
* An object spooner / inspector
	* Keep a list of all entities spawned by user, also for use with peds, vehicles, etc.
* Enable MP objects in story mode
	* Peds from multiplayer can't be spawned properly
* Model appearance / clothing changer
	* Change player outfits and clothing / other than the preset outfits
	* Change player (facial) hair length and style
* General improvements to the menu
	* Custom styling, proper RDR font from game files
* More debugging tools / access to game features
	* YSC Script locals access, list of all running scripts and their management
	* Animation playback support
	* List of interiors
* Better logging of menu and game events
	* Logging to external file and log file rotation
	* More crash information like stack trace
* Full documented list of cutscene information
	* Also change out models during cutscenes if needed
* YSC script decompilation (not necessarily part of this project)
* Showing ingame messages on screen as a status text
* Better documentation of code components
* More experimentation with game structs like CPed
* Improve build times
	* Check includes (Project Properties -> C/C++ -> Advanced -> Show Includes)
	* Move function definitions into .cpp files
* Fix game bugs, possibly by changing game files
	* Remove the timer text overlay during five finger fillet minigame
	* Add back removed outfits (https://www.youtube.com/watch?v=VJ9yV-4tJj8)
