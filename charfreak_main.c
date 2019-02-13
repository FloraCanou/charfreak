/* Copyright 2019 Flora Canou | V. 1.1.0 | This file is part of CharFreak. 
 * CharFreak is free software, licensed under the GNU General Public License, v. 3 or later. 
 * If you have not received a copy of the license, visit https://www.gnu.org/licenses/. */

#include "charfreak.h"

int main (int argc, char *argv[])
{
	setlocale (LC_ALL, "C.UTF-8");
	show_header ();
	show_order_list ();
	
	char order[SIZE_FILE];
	int total, distinct; //number of characters, number of distinct characters
	Counter *counter;
	size_counter = SIZE_DEFAULT; //size of counter
	if ((counter = malloc (size_counter * sizeof (Counter))) == NULL) //allocates memory
		return 1;
	int setting[3]; // [0] character filter, [1] case sensitivity, [2] sorting method
	if (load_setting (setting))
	{
		printf ("\nCannot access charfreak.conf for saved settings. Loading default. \n");
		setting[0] = 2;
		setting[1] = 0;
		setting[2] = 1;
	}	
	printf ("\n");
	show_setting (setting);
	
	while (1)
	{
		printf ("\n> ");
		scanf ("%s", order); //input
		if (order[0] == '/' && strlen (order) == 2) //orders other than to read a file
		{
			switch (order[1])
			{
				case 'q':
					return 0;
				case 'e':
					setting[0] = (setting[0] + 1) % 4;
					show_setting (setting);
					break;
				case 'r':
					setting[1] = (setting[1] + 1) % 2;
					show_setting (setting);
					break;
				case 't':
					setting[2] = (setting[2] + 1) % 3;
					show_setting (setting);
					break;
				case 's':
					if (save_setting (setting) == 0)
						printf ("Saved to charfreak.conf. \n");
					else
						printf ("Cannot access charfreak.conf. \n");
					break;
				case 'z':
					show_setting (setting);
					break;
				default:
					show_order_list ();
			}
		}
		else //order to read a file
		{
			startCount:
			switch (charfreak_count (order, setting[0], setting[1], counter, &distinct, &total)) //counting
			{
				case 0:
					printf ("Analysis successful. \n");
					charfreak_sort (setting[2], counter, distinct); //sorting
					show_counter (counter, distinct, total); //output
					break;
				case 1:
					printf ("Cannot open this file. \n");
					break;
				case 2:
					printf ("Analysis terminated. Too many distinct characters. \nResizing counter... \n");
					size_counter *= 8;
					if ((counter = realloc (counter, size_counter * sizeof (Counter))) != NULL)
					{
						printf ("Resize successful. Re-analysing... \n");
						goto startCount;
					}
					else
						printf ("Resize failed. Analysis canceled. \n");
			}
		}
		if (size_counter != SIZE_DEFAULT) //resets counter
		{
			printf ("Resetting counter... \n");
			size_counter = SIZE_DEFAULT;
			if ((counter = realloc (counter, size_counter * sizeof (Counter))) != NULL)
				printf ("Reset successful. \n");
			else
			{
				printf ("Error: Reset failed. \n");
				return 2;
			}
		}
		order[0] = '\0'; //clears order
	}
}

/* Load settings from charfreak.conf */
int load_setting (int setting[])
{
	FILE *fpr;
	if ((fpr = fopen ("charfreak.conf", "r")) == NULL)
		return 1; //fails to read
	
	int temp;
	fscanf (fpr, "%x", &temp);
	setting[0] = temp % 4;
	setting[1] = temp / 4 % 4;
	setting[2] = temp / 16 % 4;
	fclose (fpr);
	return 0;
}

/* Save settings to charfreak.conf */
int save_setting (int setting[])
{
	FILE *fpw;
	if ((fpw = fopen ("charfreak.conf", "w")) == NULL)
		return 1; //fails to write
	
	fprintf (fpw, "%x\nDo not edit this unless you know what you're doing. \n", (setting[0]+4*(setting[1]+4*setting[2])));
	fclose (fpw);
	return 0;
}

/* Two-column insertion sort with respect to name or number */
void charfreak_sort (int sortMethod, Counter counter[], int distinct)
{
	int i, j;
	Counter key;
	switch (sortMethod)
	{
		case 1: //by name
			for (i = 1; i < distinct; i++)
			{
				key = counter[i];
				for (j = i-1; j >= 0 && counter[j].name > key.name; j--)
					counter[j+1] = counter[j];
				counter[j+1] = key;
			}
			break;
		case 2: //by frequency
			for (i = 1; i < distinct; i++)
			{
				key = counter[i];
				for (j = i-1; j >= 0 && counter[j].num > key.num; j--)
					counter[j+1] = counter[j];
				counter[j+1] = key;
			}
	}
}

/* Show counter in console */
void show_counter (Counter counter[], int distinct, int total)
{
	printf ("The sample contains %d distinct character(s) in %d character(s). \n", distinct, total);
	printf ("  Code\t  Name\t  Num.\t  Freq.\n");
	for (int i = 0; i < distinct; i++)
	{
		printf ("%6lx\t   %lc \t%5d\t%6.3f%%\n", counter[i].name, counter[i].name, counter[i].num, 100*(float)counter[i].num/total);
	}
}

void show_setting (int setting[])
{
	printf ("Current settings: \n");
	switch (setting[0])
	{
		case 3: 
			printf ("Filter out:\tanything but alphabet\n");
			break;
		case 2: 
			printf ("Filter out:\tpunctuation marks\n");
			break;
		case 1: 
			printf ("Filter out:\tspaces\n");
			break;
		case 0:
			printf ("Filter out:\tnone\n");
			break;
		default: 
			printf ("Filter out:\t[not recognized]\n");
	}
	switch (setting[1])
	{
		case 1:
			printf ("Case sense:\ton\n");
			break;
		case 0:
			printf ("Case sense:\toff\n");
			break;
		default:
			printf ("Case sense:\t[not recognized]\n");
	}
	switch (setting[2])
	{
		case 2: 
			printf ("Sorting:\tby frequency\n");
			break;
		case 1: 
			printf ("Sorting:\tby name\n");
			break;
		case 0:
			printf ("Sorting:\tnone\n");
			break;
		default: 
			printf ("Sorting:\t[not recognized]\n");
	}
}

void show_order_list ()
{
	printf ("filename\tRead text sample, *UTF-8 only*\n"
		"\"/e\"\t\tToggle character filter policy\n"
		"\"/r\"\t\tToggle case sensitivity\n"
		"\"/t\"\t\tToggle sorting method\n"
		"\"/s\"\t\tSave current settings as default\n"
		"\"/z\"\t\tShow current settings\n"
		"\"/q\"\t\tQuit\n"
		"\"/\" + any other single character to show this list\n");
}

void show_header ()
{
	printf ("\nCharFreak - character frequency counter v. 1.1.0\n"
		"Copyright 2019 Flora Canou (floracanou@qq.com)\n"
 		"This program is licensed under the GNU General Public License, v. 3 or later. \n"
 		"To view a copy of the license, visit https://www.gnu.org/licenses/. \n\n");
}
