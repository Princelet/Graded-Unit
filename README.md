Graded Unit Devlog

26/4
- Essentially a week of bringing code over for an initial setup.
- Created plenty of starter enemy code to make specific enemies appear on specific waves as an introduction.
- Created the beginnings of a health system for the player and enemies, but no proper implementation.

27/4
- Created the basic player and enemy sprites for the game.
- Struggled to create some kind of animation so I've left it for now.

3/5
- Created the animated sprites for the character movement animations.
- Implemented these animations without a timer as a temporary test.
- Had trouble making a sideways walking animation that wasn't awkwardly shaped.
- Had trouble getting enemies to spawn and draw correctly.

4/5
- Fixed the sideways walk animation by adding a standing frame in between to show legs overlapping.
- Removed unnecessary jump code as the game uses 4 directional movement.
- Created item pickup sprites and a temporary square sprite for the game's block obstacles.
- Started working on block code but struggled with getting them to spawn correctly.

10/5
- Changed the early level enemy system to hardcoded numbers instead of a file for ease of access.
- Added enemy spawning at last and created an attack box class to begin the actual fighting mechanics.
- Struggled with getting the attack box to destroy itself after use.
- Enemies currently all go down to the left instead of randomly like I was hoping they would.

16/5
- Removed code around getting the attack box to disappear in favour of keeping it and teleporting it around.
- Added collision code to the enemies that instantly kills them for now.
- Had trouble getting the attack box to be central to the player in the attacking direction.

17/5
- Finally added a wave system that increments as soon as all enemies are killed.
- Noticed enemies do not follow the spawn timer as intended, which should be fixed soon but not urgently.
- Added an enemy health system but unfortunately the enemies dying causes the vector to error out.

18/5
- Added screen borders and a background to the arena.
- Couldn't find how to make the background transparent in order to make it less blinding to the user.
- Fixed the enemy health system.
- Enemy invulnerability currently doesn't exist which makes this useless as they take damage every frame.
- Player and enemies both currently vanish when touching the screen border.
- Fixed a memory leak related to defeating enemies where enemies did not get deleted until the next wave.

24/5
- Added new attack sprites.
- Fixed attack positions and set up attack box collision scales/offsets.
- Added enemy health and invulnerability.
- Gave enemy class an attack box for each enemy, currently they do not attack.
- Confused by the enemies that sometimes are at the top left and why they ignore the spawn timer code.

31/5
- Added some high score code to the game.
- Added a banner sprite for the UI at the top of the screen.
- Re-enabled the end panel code.
- Got enemies to attack but their attacks just float there and never vanish.
- Game over doesnt seem to do anything besides stop another wave from loading. The game generally keeps running?
- Added a visual cue to player invulnerability.
- Made enemy attack boxes different colours.
- Enemies seem to still be spawning out of bounds and from there just... sit there and don't exist? Wave can increment with a guy up in the corner.
- Adjusted enemy spawning to use the arena instead of the screen. Doesn't fix issue yet however...

1/6
- Created a block sprite.
- Made it so the player internally becomes invisible when on no health. This will be replaced when I figure out why game over isn't working.
- Fixed the old position value. The one on Object was private instead of protected, which means it wasn't overwriting and was always at 0, 0.
- Added a health item class and made it appear every 5 waves. It currently does nothing and never disappears.
- Tried to fix the enemies that spawn at the bottom and don't move, and now I have them stuck outside the arena.

6/6
- Attempted to change how animations work using clocks.
- Added clocks for game time and enemy spawning.
- Game SHOULD have timers now. However none of the text is displaying and I don't know why.
- Player animation clock seems to not restart when asked. Neither does the enemy one. According to breakpoints I'm hitting both but nothing seems to happen...

7/6
- Got the UI to appear and update correctly.
- Was a little confused when I forgot to "as string" the enemy size and it started removing letters!
- Player and enemies now can take varying damage.
- Added function to health item.
- Added power item and made it show on player when they have it.
- End panel can now appear and game over has a function.
- High scores currently don't update or display the word wave and have a weird character between the latter scores.
- Unsure how I'd set up a title screen or name entry, which I really need in order to do high score entry.
- Fixed the wave timer which didn't reset.

8/6
- Started setup for title screen. Currently doesn't work and nothing is drawn.
- A little lost on why New Wave is triggering when the level screen isn't updating.
- Solved the new wave issue! It was happening on construction instead of moving over.
- Title screen now draws, but nothing is really in the right place.

10/6
- Enemy attacks should really move with the enemy, they are also randomly directed despite having a set direction?
- Screen size differences don't work with banner or block spawning.
- Cooldowns aren't obvious to the player, found myself pressing the keys and nothing happens because I don't have a display of delay.
- Player attacks are barely visible which can cause confusion.

13/6
- Title screen generally complete.
- The icon doesn't move so Quit cannot currently be selected as far as I'm aware.
- Background image would be nice, but not a necessity to be honest.
- Controls info should be added.
- I'd like to figure out how to scale the game to the screen size, but I'm unsure how this would work. It isn't required but different resolutions make blocks useless and the game a lot easier due to how much space is available.
- Game over seems to print the new scores as it should but not before printing the direct text first...

14/6
- Completed the title screen.
- Fixed the score listings using a struct and vector.
- Struggled figuring out how to write to the score file.
- Set the game to always use 1920x1080 to semi-fix the screen resolution. Game difficulty no longer is affected by screen size.
- Created 3D title text in Blender.
- Confused as changing the attack timer doesn't seem to do anything to the instantly vanishing attack.
- Added enemy collision to player.
- Enemies could originally throw you out, but now the only collision is the player pushing enemies. While pushing them is possible, it is fair as the enemy can attack and the player ends up walking into it.
- Added controls for the other pad on the arcade machine. Player can use either and it doesn't really matter.
- Level screen can't go back to the title screen for some reason?
- Finished the score saving system.
- Fixed the title screen issue by adding a delay! As long as a key was still being pressed it skipped straight back to gameplay.
- Got a bit lost when it comes to audio at first, but music is in the game.