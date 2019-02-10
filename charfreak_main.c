/* Copyright 2019 Flora Canou | V. 1.1.0 | This file is part of CharFreak. 
 * CharFreak is free software, licensed under the GNU General Public License, v. 3 or later. 
 * If you have not received a copy of the license, visit https://www.gnu.org/licenses/. */

#include "charfreak.h"

int main (int argc, char *argv[])
{
	setlocale (LC_ALL, "C.UTF-8");
	showHeader ();
	showOrderList ();
	char order[sizef];
	int total, distinct; //number of characters, number of distinct characters
	
	Counter counter[sizec];
	int setting[3] = {2, 0, 1}; // [0] character exclusion, [1] case sensitivity, [2] sorting method
	if (loadSetting (setting))
		printf ("\nCannot access charfreak.conf for saved settings. Loading default. \n");
	printf ("\n");
	showSetting (setting);
	while (1)
	{
		printf ("\n> ");
		scanf ("%s", order); //input
		if (order[0] == '/' && strlen(order) == 2)
		{
			switch (order[1])
			{
				case 'q':
					return 0;
				case 'e':
					setting[0] = (setting[0] + 1) % 4;
					showSetting (setting);
					break;
				case 'r':
					setting[1] = (setting[1] + 1) % 2;
					showSetting (setting);
					break;
				case 't':
					setting[2] = (setting[2] + 1) % 3;
					showSetting (setting);
					break;
				case 's':
					if (saveSetting (setting) == 0)
						printf ("Saved to charfreak.conf. \n");
					else
						printf ("Cannot access charfreak.conf. \n");
					break;
				case 'z':
					showSetting (setting);
					break;
				case 'x': 
					showOrderList ();
					break;
				default: 
					printf ("Order not recognized. \n");
			}
		}
		else
		{
			switch (charfreakCount (order, setting[0], setting[1], counter, &distinct, &total)) //counting
			{
				case 0:
					printf ("Analysis successful. \n");
					charfreakSort (setting[2], counter, &distinct); //sorting
					showCounter (counter, distinct, total); //output
					break;
				case 1:
					printf ("Cannot open this file. \n");
					break;
				case 2:
					printf ("Analysis terminated. Too many distinct characters. \n");
			}
		}
		order[0] = '\0'; //clear order
	}
}

/* Load settings from charfreak.conf */
int loadSetting (int setting[])
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
int saveSetting (int setting[])
{
	FILE *fpw;
	if ((fpw = fopen ("charfreak.conf", "w")) == NULL)
		return 1; //fails to write
	
	fprintf (fpw, "%x\nDo not edit this unless you know what you're doing. \n", (setting[0]+4*(setting[1]+4*setting[2])));
	fclose (fpw);
	return 0;
}

/* Two-column insertion sort with respect to name or number */
void charfreakSort (int sortMethod, Counter counter[], int *distinct)
{
	int i, j;
	Counter key;
	switch (sortMethod)
	{
		case 1: //by name
			for (i = 1; i < *distinct; i++)
			{
				key = counter[i];
				for (j = i-1; j >= 0 && counter[j].name > key.name; j--)
					counter[j+1] = counter[j];
				counter[j+1] = key;
			}
			break;
		case 2: //by frequency
			for (i = 1; i < *distinct; i++)
			{
				key = counter[i];
				for (j = i-1; j >= 0 && counter[j].num > key.num; j--)
					counter[j+1] = counter[j];
				counter[j+1] = key;
			}
	}
}

/* Show counter in console */
void showCounter (Counter counter[], int distinct, int total)
{
	printf ("The sample contains %d distinct character(s) in %d character(s). \n", distinct, total);
	printf ("  Code\t  Name\t  Num.\t  Freq.\n");
	for (int i = 0; i < distinct; i++)
	{
		printf ("%6lx\t   %lc \t%5d\t%6.3f%%\n", counter[i].name, counter[i].name, counter[i].num, 100*(float)counter[i].num/total);
	}
}

void showSetting (int setting[])
{
	printf ("Current settings: \n");
	switch (setting[0])
	{
		case 3: 
			printf ("Exclusion:\tanything but alphabet\n");
			break;
		case 2: 
			printf ("Exclusion:\tpunctuation marks\n");
			break;
		case 1: 
			printf ("Exclusion:\tspaces\n");
			break;
		case 0:
			printf ("Exclusion:\tnone\n");
			break;
		default: 
			printf ("Exclusion:\t[not recognized]\n");
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

void showOrderList ()
{
	printf ("List of orders: \n"
		"filename\tRead text sample. Please make sure it is in UTF-8. \n"
		"\"/e\"\t\tToggle character exclusion policy\n"
		"\"/r\"\t\tToggle case sensitivity\n"
		"\"/t\"\t\tToggle sorting method\n"
		"\"/s\"\t\tSave current settings as default\n"
		"\"/z\"\t\tShow current settings\n"
		"\"/x\"\t\tShow this list\n"
		"\"/q\"\t\tQuit\n");
}

void showHeader ()
{
	printf ("\nCharFreak - character frequency counter v. 1.0.0\n"
		"Copyright 2019 Flora Canou (floracanou@qq.com)\n"
 		"This program is licensed under the GNU General Public License, v. 3 or later. \n"
 		"To view a copy of the license, visit https://www.gnu.org/licenses/. \n\n");
}
