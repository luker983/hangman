/**
 * @file hangman.c
 * @brief a hangman game for 1-2 players
 * @detail uses memory allocation to allow the computer to choose a random word,
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
#define DEF 5 //default number of tries if user data is out of range

int main()
{
        char buf[LEN];
        char *word;
        char *prompt = {""};
        char m;
        int guesses;
        srandom(time(NULL));
        struct diction_t *dictionary = file_open();
 
        printf("\n--**--Welcome to Hangman!--**--\n\n");
  LOOP: while(1) {
                puts("");
                printf("Are you playing by yourself? (enter 'q' to quit) [y/n]: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%c", &m);

                m = tolower(m);

                puts("");

                switch(m) {
                case('y'):
                        printf("We can play together! I'll pick a word.\n");
                        word = get_word(dictionary);
                        guesses = strlen(word) + 3;
                        printf("For the word I picked, you get %d guesses.\n", guesses);

                        make_hangman(word, guesses);
                        break;
                case('n'):
                        printf("Great! I'm tired of this game anyway.\n");
                        puts("");
                        printf("How many chances should the guesser get? [1-1024]: ");
                        fgets(buf, LEN, stdin);
                        sscanf(buf, "%d", &guesses);

                        if (guesses > 0 && guesses < LEN) {
                                printf("\nThe guesser will get %d tries.\n", guesses);
                        } else {
                                printf("\nThat wasn't one of the options...so the guesser will get the default, %d tries.\n", DEF);
                        }
                        printf("\nOne of you should enter a word here: ");
                        word = getpass(prompt);
                        //fgets(buf, LEN, stdin);

                        //word = buf;
                        word = rstrip(word);

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

                m = tolower(m);

                switch(m) {
                case('y'):
                        printf("\nOf course you do!\n");
                        break;
                case('n'):
                        free_mem(dictionary);
                        printf("\nI didn't want to play again anyway.\n");
                        return 0;
                default:
                        printf("\nUmmm....that wasn't one of the options, so I'm going to assume you want to play again.\n");
                }
        }

        return 0;
}

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
                if (c == len) {
                        printf("\nCongratulations! You guessed it!\n");
                        return;
                }
                guesses--;
                if (guesses) {
                        printf("\nYou guessed %c and revealed %d characters!\n", g, n);
                } 

        }
        printf("\nThe word was %s.\n", word);
        printf("You have no more guesses left. You lose!\n");
}

char *rstrip(char *s){
        int i;

        for (i = 0; s[i] != '\0'; i++) {
                if (isspace(s[i])) {
                        s[i] = '\0';
                }
        }

        return s;
}

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

void free_mem(struct diction_t *dictionary)
{
        int i;
             
        for (i = 0; i < dictionary->nval; i++) {
                                free(dictionary->words[i]);
                        }
                        free(dictionary->words);
                        free(dictionary);
}

char *get_word(struct diction_t *dictionary)
{
        int r = random() % dictionary->nval;
        return dictionary->words[r];
}
