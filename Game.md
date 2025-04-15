                                Shakiraliev ELdar 11417 Project 1 evaluation
        Added: level class, player class, enemy class, dynamic level loading, exception handling.

level class:
First things first I rewrote all the functions from the initial level.h and made them object-oriented. Thus, all the calling functions were changed into Level::function.

player class:
Rewrote every function and made them object-oriented and changed the calling functions into Player::function.

enemy class:
Rewrote every function and made them object-oriented and changed the calling functions into Enemy::function. 

Some functions from one header file needed to be called in header files of other class, by making them static. 
Problems faced:
There were no problems with rewriting the player, enemy and level header files into classes. But, I faced the problem of making the levels dynamic. It took so much time to create lots of vectors, to check where were errors. Most of the time levels were loaded without a single texture. The things that helped me were vectors of chars, vectors of int and vectors of my own struct level. 

Error handling:
Added error handling in assets.h to always make sure that all the images and all the sounds are loaded correctly.
Added error handling in dynamic level logic to make sure that I have a proper amount of objects.


