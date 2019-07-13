/* Copyright 2019 Flora Canou | V. 1.2.0 | This file is part of CharFreak. 
 * CharFreak is free software, licensed under the GNU General Public License, v. 3 or later. 
 * If you have not received a copy of the license, visit https://www.gnu.org/licenses/. */

#include "charfreak.h"

int main (int argc, char *argv[])
{
	setlocale (LC_ALL, "C.UTF-8");
	show_header ();
	show_order_list_main ();
	
	char order[SIZE_FILE];
	int total, distinct; //number of characters, number of distinct characters
	Counter *counter;
	size_counter = SIZE_DEFAULT; //size of counter
	if ((counter = malloc (size_counter * sizeof (Counter))) == NULL) //allocates memory
		return 1;
	int setting[4] = {2, 0, 1, 0}; // [0] character filter, [1] case sensitivity, [2] sorting method, [3] output method
	if (load_setting (setting))
		printf ("\nCannot access charfreak.conf for saved settings. Loading default. \n");
	printf ("\nCurrent settings: \n");
	show_setting (setting, 1 + 2 + 4 + 8);
	
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
				case 'a':
					edit_setting (setting);
				case 'z':
					printf ("Current settings: \n");
					show_setting (setting, 1 + 2 + 4 + 8);
					break;
				default:
					show_order_list_main ();
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
					charfreak_output (order, setting[3], counter, distinct, total); //output
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
	for (int i = 0; i < 4; i++)
		setting[i] = (temp >> (2*i)) % 4;
	fclose (fpr);
	return 0;
}

/* Edit settings */
int edit_setting (int setting[])
{
	show_order_list_setting ();
	char order[SIZE_FILE];
	while (1)
	{
		printf ("\n> ");
		scanf ("%s", order);
		FILE *fpw;
		switch (order[1])
		{
			case 'q':
				return 0;
			case 'w':
				setting[0] = (setting[0] + 1) % 4;
				show_setting (setting, 1);
				break;
			case 'e':
				setting[1] = (setting[1] + 1) % 2;
				show_setting (setting, 2);
				break;
			case 'r':
				setting[2] = (setting[2] + 1) % 3;
				show_setting (setting, 4);
				break;
			case 't':
				setting[3] = (setting[3] + 1) % 3;
				show_setting (setting, 8);
				break;
			case 's': //save settings
				if ((fpw = fopen ("charfreak.conf", "w")) != NULL)
				{
					int temp = 0;
					for (int i = 0; i < 4; i++)
						temp = (temp << 2) + setting[i];
					fprintf (fpw, "%x\nDo not edit this unless you know what you're doing. \n", temp);
					fclose (fpw);
					printf ("Saved to charfreak.conf. \n");
					return 0;
				}
				else
					printf ("Cannot access charfreak.conf. \n"); //fails to write
				break;
			default:
				show_order_list_setting ();
		}
		order[0] = '\0'; //clears order
	}
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

/* Output */
void charfreak_output (char order[], int outputMethod, Counter counter[], int distinct, int total)
{
	if (outputMethod == 0 || outputMethod == 2)
	{
		printf ("The sample contains %d distinct character(s) in %d character(s). \n", distinct, total);
		printf ("  Code\t  Name\t  Num.\t  Freq.\n");
		for (int i = 0; i < distinct; i++)
			printf ("%6lx\t   %lc \t%5d\t%6.3f%%\n", counter[i].name, counter[i].name, counter[i].num, 100*(float)counter[i].num/total);
	}
	if (outputMethod == 1 || outputMethod == 2)
	{
		FILE *fpw;
		if ((fpw = fopen (strcat (order, "_charfreak.txt"), "w")) != NULL)
		{
			fprintf (fpw, "The sample contains %d distinct character(s) in %d character(s). \n", distinct, total);
			fprintf (fpw, "  Code\t  Name\t  Num.\t  Freq.\n");
			for (int i = 0; i < distinct; i++)
				fprintf (fpw, "%6lx\t   %lc \t%5d\t%6.3f%%\n", counter[i].name, counter[i].name, counter[i].num, 100*(float)counter[i].num/total);
			fclose (fpw);
			printf ("Saved to %s. \n", order);
		}
		else
			printf ("Cannot save to file. \n"); //fails to write
	}
}

void show_setting (int setting[], int show_setting_switch)
{
	if (show_setting_switch % 2)
	{
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
	}
	if ((show_setting_switch >> 1) % 2)
	{
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
	}
	if ((show_setting_switch >> 2) % 2)
	{
		switch (setting[2])
		{
			case 2: 
				printf ("Sort method:\tby frequency\n");
				break;
			case 1: 
				printf ("Sort method:\tby name\n");
				break;
			case 0:
				printf ("Sort method:\tnone\n");
				break;
			default: 
				printf ("Sort method:\t[not recognized]\n");
		}
	}
	if ((show_setting_switch >> 3) % 2)
	{
		switch (setting[3])
		{
			case 2:
				printf ("Output method:\tshow & save\n");
				break;
			case 1:
				printf ("Output method:\tsave to file\n");
				break;
			case 0:
				printf ("Output method:\tshow on screen\n");
				break;
			default:
				printf ("Output method:\t[not recognized]\n");
		}
	}
}

void show_order_list_main ()
{
	printf ("filename\tRead text sample, *UTF-8 only*\n"
		"\"/q\"\t\tQuit\n"
		"\"/a\"\t\tEdit settings\n"
		"\"/z\"\t\tShow current settings\n"
		"\"/\" + any other character(s) to show this list\n");
}

void show_order_list_setting ()
{
	printf ("\"/q\"\t\tDone editing\n"
		"\"/w\"\t\tToggle character filter policy\n"
		"\"/e\"\t\tToggle case sensitivity\n"
		"\"/r\"\t\tToggle sorting method\n"
		"\"/t\"\t\tToggle output method\n"
		"\"/s\"\t\tSave current settings as default\n"
		"\"/\" + any other character(s) to show this list\n");
}

void show_header ()
{
	printf ("\nCharFreak - character frequency counter v. 1.2.0\n"
		"Copyright 2019 Flora Canou (floracanou@qq.com)\n"
 		"This program is licensed under the GNU General Public License, v. 3 or later. \n"
 		"To view a copy of the license, visit https://www.gnu.org/licenses/. \n\n");
}
