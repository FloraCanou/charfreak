/* This file is part of CharFreak. */

#ifndef CHARFREAK_H
#define CHARFREAK_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#define	sizef	256
#define	sizec	256

typedef struct Counter {int name; int num;} Counter; //stores name and number of each character
int charfreakCount (char*, int, int, Counter*, int*, int*);
bool excl (int, wint_t);
wint_t caseConv (wint_t);
void clearCounter (Counter*, int*, int*);
int loadSetting (int*);
int saveSetting (int*);
void charfreakSort (int, Counter*, int*);
void showCounter (Counter*, int, int);
void showSetting (int*);
void showOrderList ();
void showHeader ();

#endif
