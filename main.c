/*
**  main.c
**  Yet another wc
**
**  Created by Constantin Gubin on 27.04.2021.
**  Copyright © 2021 Constantin Gubin. All rights reserved.
**
**
**  Create a program in C that computes some statistics about a text file stored on disk.
**  Please include instructions for using it.
**  The statistics are:
**  whitespace-delimited word count,
**  line count,
**  average number of letters per word (to one decimal place) and most common letter.
**  We will check the accuracy of the output;
**  assess the program for style and readability;
**  and also look at how easy it is to extend your program to compute additional statistics.
**  The expectation is that you produce production-level code with all associated measures
**  --- testing, documentation etc.
**
**  It's supposed to distingush letters ['A'-'z'], symbols ['!'-'/'] || [':'-'?'] || '~'
**  and digits ['0'-'9'], thus the number of all letters is ['~'-'!'+1]. Notice 1 is added because '~' is included and the next character DEL is non-printing. A word consists of letters, digits and symbols. It derives from
**  the difinition of a whitespace-delimited word.
*/

#include <stdio.h>
#include <stdlib.h>

#define YES 1
#define NO  0
#define NLETTERS '~'-'!'+1  /* It could be 127-'!', but it looks unclear.
                            ** In any other cases of a narrower set of letters '~' should
                            ** be substuted by the last upper charecter included in
                            ** the set.
                            ** For example: only English Alpabet lettrs 'z'-'A'+1
                            */
#define ascii2i( ccode )    (ccode - (int) '!')     /* ASCII to index -- a charactar code minus the offset */
#define i2ascii( cindex )   (cindex + (int) '!')

typedef unsigned long   counter_t;
/* #define MAX_COUNTER -- ввести константу для проверки на переполнение */

counter_t   nwords;     /* Number of words */
counter_t   nlines;     /* Number of lines */
counter_t   nletters;   /* Number of letters */
int         ic;         /* The most common letter */
double      avword;     /* Average number of letters per a word. */

void cstat( FILE *fp );
void fmtprint( const char *fname );

int main(int argc, const char * argv[]) {
    FILE    *fp;
    
    if( argc == 1 ) {
        fprintf( stderr, "Usage: awc file [file ...]\n" );
        return( EXIT_FAILURE );
        /* Alternative: fp = stdin */
    }

    while( --argc > 0 )
        if( (fp = fopen(*++argv, "r")) == NULL) {
            fprintf(stderr, "awc: can't open %s\n", *argv);
            return( EXIT_FAILURE );
        } else {
            cstat( fp );
            fclose( fp );
            fmtprint( *argv );
        }
        
    return 0;
    
}

void fmtprint( const char *fname ) {

    printf( "%s\n", fname );
    printf( "words:\t\t%lu\n", nwords );
    printf( "lines:\t\t%lu\n", nlines );
    printf( "letters per word:%.1f\n", avword );
    printf( "the most common letter:\t\t%c\n\n", (char) ic );
    
}


/*
** cstat - Compute STATistics
*/
void cstat(FILE *fp) {
    register int i;     /* A bit old fashion but I like it. */
    int c;              /* Input buf */
    int newword;        /* Alternative might be as 'bool newword', but I like this one */
    counter_t   maxoccur; /* Max occurence of a letter */
    counter_t   occur[NLETTERS]; /* Occurence of each letter. */
    
    nwords = 0;
    nlines = 0;
    nletters = 0;
    
    newword = YES;
    for(i = 0; i < NLETTERS; i++)
        occur[i] = 0;
    
    while((c = fgetc(fp)) != EOF) {
        switch( c ) {
            case '\n':  ++nlines;
            case ' ' :
            case '\t':  newword = YES;
                        continue;
        }
        if( c > 126 )
            continue;   /* Exclude DEL and not ASCII characters. Just to be on the safe
                        ** side.
                        */
        ++nletters;
        ++occur[ascii2i(c)];
        if( newword ) {
            ++nwords;
            newword = NO;
        }
        /* Ввести проверку на переполнение. */
    }
    maxoccur = 0;
    for( i = 0, ic = 0; i < NLETTERS; i++)
        if( occur[i] > maxoccur) {
            maxoccur = occur[i];
            ic = i; /* предесмотреть если две и более букв имеют одинаковое вхождение */
        }
    ic = i2ascii( ic );
    avword = (double) nletters / (double) nwords;
}
