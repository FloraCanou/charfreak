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

int charfreakCount (char*, int, int, int*, int*, int*, int*);
bool excl (int, wint_t);
wint_t caseConv (wint_t);
void clearCounter (int*, int*, int*, int*);
int loadSetting (int*);
int saveSetting (int*);
void charfreakSort (int, int*, int*, int*);
void insertionSortPartial (int*, int*, int n);
void showCounter (int*, int*, int, int);
void showSetting (int*);
void showOrderList ();
void showHeader ();

#endif

