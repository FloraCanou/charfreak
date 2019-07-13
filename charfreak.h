/* This file is part of CharFreak. */

#ifndef CHARFREAK_H
#define CHARFREAK_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#define	SIZE_FILE	256
#define SIZE_DEFAULT	128

int size_counter; //size of counter
typedef struct Counter {int name; int num;} Counter; //stores name and number of each character
int charfreak_count (char*, int, int, Counter*, int*, int*);
int place_current (wint_t, Counter*, int*);
bool filter_tp (int, wint_t);
wint_t case_conv (wint_t);
void clear_counter (Counter*, int*, int*);
int load_setting (int*);
int edit_setting (int*);
void charfreak_sort (int, Counter*, int);
void charfreak_output (char*, int, Counter*, int, int);
void show_setting (int*, int);
void show_order_list_main ();
void show_order_list_setting ();
void show_header ();

#endif
