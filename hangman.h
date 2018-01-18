/**
 * @file hangman.h
 * @brief header file for hangman.c
 *
 * @author Luke Rindels
 * @date April 6, 2017
 */

#ifndef HANG_H
#define HANG_H
struct diction_t {
        int nval;
        int max;
        char **words;
};

enum {INIT = 1, GROW = 2};
void make_hangman(char *word, int guesses);
void free_mem(struct diction_t *dictionary);
char *rstrip(char *s);
char *get_word(struct diction_t *dictionary);
struct diction_t *file_open();
#endif
