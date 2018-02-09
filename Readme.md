# Hangman

Hangman is all about guessing a certain word with a limited number of
guesses. If you run out of guesses you get "hanged", if you get the word
correct then you get to stay alive. This implementation requires you to
have access to a C compiler and be able to run the executable. 

### File Setup

Be sure that hangman.c, hangman.h, dictionary.txt, and Makefile are all in
the same directory and all named appropriately. If the file names are
changed, you will have to alter some of the code yourself. To compile,
type 'make' into the terminal, there should be no errors or warnings.
You can start the game by typing './hangman' into the command line. All
commands from now on are given by typing in an answer and pressing the
'Enter' key.

## Playing The Game

'''
./hangman
'''

### Single Player

To play by yourself, enter 'y'. The computer will choose a word from 
the dictionary and random and give you an appropriate amount of
guesses. Type letters one by one until the word has been solved or
you have run out of guesses.

### Multiplayer

When playing with multiple people, you get to choose how many tries you
get and are not limited to dictionary words. Anything goes, but try to 
make it fun. Enter 'n' once the game is running to choose multiplayer
mode. You will be able to choose the number of tries the guesser(s) 
get, then enter the chosen word. No text will appear while typing to 
keep it a secret. Letters can be entered until the entire word is 
guessed or the guesser(s) run out of tries.

## Troubleshooting

Make sure the program is compiled and all files have their original
names (as listed above). If you are still having issues, feel free to
email me at lukerindels983@gmail.com. Thanks for playing!
