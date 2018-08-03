/**
 * @file hangman.c
 * @brief a hangman game for 1-2 players
 * @detail uses doubling memory allocation to allow the computer to choose a random word,
 * 2 players can take turns inputting words and number of tries to beat each other
 *
 * @author Luke Rindels
 * @date April 6, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include "hangman.h"

#define LEN 1024 //for character buffers
#define DEF 3 //added to string length of word if user data is out of range

int main()
{
        char buf[LEN];
        char *word;
        char *prompt = {""};
        char m;
        int i;
        int diff;
        int guesses;
        srandom(time(NULL));
        struct diction_t *dictionary = file_open();
    
        system("clear");
        printf("\n---------- Welcome to Hangman! ----------\n\n");

// game loop
  LOOP: while(1) {
                puts("");
                printf("Are you playing by yourself? (enter 'q' to quit) [y/n]: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%c", &m);

                system("clear");
                m = tolower(m);

                puts("");
			
		        // if playing alone, computer picks word
                switch(m) {
                case('y'):
                        // computer picks word
                        printf("Okay, I will play with you. I have a word in mind.\n");
                        word = get_word(dictionary);
                        // difficulty selection
                        printf("\nPick a difficult level. 1 is the easiest, 5 is the hardest: ");
                        fgets(buf, LEN, stdin);
                        sscanf(buf, "%d", &diff);

                        system("clear");
                        // error checking
                        if (diff < 1 || diff > 5) {
                            printf("\nThat's not an option, so you get the default difficulty level (3).\n");
                            diff = 3;
                        } else {
                            printf("\nYou picked difficulty level %d.\n", diff);
                        }
    
                        guesses = strlen(word) + 5 - diff;
                        printf("For the word I picked, you get %d guesses.\n", guesses);

                        make_hangman(word, guesses);
                        break;
                // playing with friend
                case('n'):
                        printf("Cool, the person choosing the word should enter it here (leave blank to choose random word): ");
                        puts("");
	                    // word is typed here
                        word = getpass(prompt); 
                        word = rstrip(word);
                        // blank entries get random word
                        if (strnlen(word, LEN) == 0) {
                            word = get_word(dictionary);
                        }
         
                        system("clear");
                        // if a character is not a letter a random word is chosen
                        for (i = 0; i < strnlen(word, LEN); i++) {
                            if (!isalpha(word[i])) {
                                printf("\nOnly letters are allowed, I will pick a random word.\n");
                                word = get_word(dictionary);
                            }
                        }
                        // gives number of guesses
                        printf("\nHow many chances should the guesser get? The word has %d letters. [1-1024]: ", (int) strnlen(word, LEN));
                        fgets(buf, LEN, stdin);
                        sscanf(buf, "%d", &guesses);
            
                        system("clear");
                        if (guesses > 0 && guesses < LEN) {
                                printf("\nThe guesser will get %d tries.\n", guesses);
                        } else {
                                printf("\nThat wasn't one of the options...so the guesser will get the default, %d tries.\n", (int) strnlen(word, LEN) + DEF);
                                guesses = strnlen(word, LEN) + DEF;
                        }
                        make_hangman(word, guesses);
                        break;
                case('q'):
                        free_mem(dictionary);
                        printf("\nBye! Come back soon!\n");
                        return 0;
                default:
                        printf("\nPlease enter 'y' for yes or 'n' for no. If you would like to quit, enter 'q'.\n");
                        goto LOOP;
                }

                printf("\nWould you like to play again? [y,n]: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%c", &m);

                system("clear");
                m = tolower(m);

                switch(m) {
                case('y'):
                        printf("\nAlright! Let's play again.\n");
                        break;
                case('n'):
                        free_mem(dictionary);
                        printf("\nThat's fine, come back soon!\n");
                        return 0;
                default:
                        printf("\nThat wasn't one of the options so I'm going to assume you want to play again.\n");
                }
        }

        return 0;
}

/**
 * Displays the game board to stdout
 *
 * @param word the word being guessed
 * @param guesses the number of tries the player gets
 */
void make_hangman(char *word, int guesses)
{
        int len = strlen(word);
        int used[26] = {0};
        char prog[len];
        char key[len];
        char buf[LEN];
        char g;
        int i;
        int n;
        int c;

        for (i = 0; i < len; i++) {
                prog[i] = '_';
                key[i] = tolower(word[i]);
        }

        while (guesses > 0) {
                n = 0;
                c = 0;
                puts("");
                printf("**************************************\n");
                puts(""); 
                printf("*****************************\\\\|//**\n");
                printf("**You have %d guesses left     \\|/\n", guesses);
                printf("**                             |\n");
                printf("**                            ()\n");
                printf("**                           /||\\\n");
                printf("**                            //\n");
                printf("** ");
                for (i = 0; i < len; i++) {
                        printf("%c ", prog[i]);
                }
                printf("\n*************************\n");
                printf("Guessed letters: ");
                for (i = 0; i < 26; i++) { 
                        if (used[i] > 0) {
                                printf("%c ", 'a' + i);
                        } else {
                                printf("- ");
                        }
                }
                puts("");
                printf("\nEnter a letter here: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%c", &g); 
                g = tolower(g);

                system("clear");
   
                // verifies letter is in alphabet
                if (!isalpha(g)) {
                    printf("\nThat is not a letter of the alphabet.\n");
                    continue;
                }
                // verifies letter has not been guessed
                if (used[g - 'a']) {
                    printf("\nYou have already guessed the letter %c.\n", g);
                    continue;  
                }
                // marks letter as used
                used[g - 'a']++;
                for (i = 0; i < len; i++) {
                        if (g == key[i]) {
                                prog[i] = g;
                                n++; 
                        } 
                        if (prog[i] != '_') {
                                c++;
                        }
                }
                // player wins
                if (c == len) {
                        printf("\nCongratulations! You guessed the word %s!\n", word);
                        return;
                } 
                // wrong guess
                if (n == 0) {
                    guesses--;
                }
                // player has guesses left
                if (guesses) {
                        printf("\nYou guessed %c and revealed %d characters!\n", g, n);
                } 

        }
        printf("\nThe word was %s.\n", word);
        printf("You have no more guesses left. You lose!\n");
}

/**
 * Strips whitespace from the right of a string
 *
 * @param s the string being stripped
 * @return the stripped string
 */
char *rstrip(char *s){
        int i;

        for (i = 0; s[i] != '\0'; i++) {
                if (isspace(s[i])) {
                        s[i] = '\0';
                }
        }

        return s;
}

/**
 * Opens and allocates space for the dictionary
 * 
 * @return the dictionary
 */
struct diction_t *file_open()
{
        int i = 0;

        char buf[LEN];
        FILE *fp = fopen("dictionary.txt", "r");
        struct diction_t *dictionary = malloc(sizeof(struct diction_t));
        assert(dictionary);

        dictionary->nval = INIT;
        dictionary->max = INIT;
        dictionary->words = NULL;
        /* uses doubling reallocation system */
        while (fgets(buf, LEN, fp)) {
                if (dictionary->words == NULL) {
                        dictionary->words = malloc(sizeof(char *));
                        assert(dictionary->words);
                } else if (dictionary->nval > dictionary->max) {
                        dictionary->words = realloc(dictionary->words, GROW * dictionary->max *sizeof(char *));
                        assert(dictionary->words);
                        dictionary->max = GROW * dictionary->max;
                }
                dictionary->words[i] = malloc(sizeof(char) * LEN);
                assert(dictionary->words[i]);

                strncpy(dictionary->words[i], strtok(buf, "\n"), LEN);
                i++;
                dictionary->nval++;
        }
        dictionary->nval--;
        fclose(fp);

        return dictionary;
}

/**
 * Frees the memory allocated for the dictionary
 *
 * @param dictionary the dictionary being freed
 */
void free_mem(struct diction_t *dictionary)
{
        int i;
             
        for (i = 0; i < dictionary->nval; i++) {
                                free(dictionary->words[i]);
                        }
                        free(dictionary->words);
                        free(dictionary);
}

/**
 * Pulls random word from dictionary
 *
 * @param dictionary the dictionary being pulled from
 * @return a random word
 */
char *get_word(struct diction_t *dictionary)
{
        int r = random() % dictionary->nval;
        return dictionary->words[r];
}
