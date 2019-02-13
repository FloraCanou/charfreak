/* Copyright 2019 Flora Canou | V. 1.1.0 | This file is part of CharFreak. 
 * CharFreak is free software, licensed under the GNU General Public License, v. 3 or later. 
 * If you have not received a copy of the license, visit https://www.gnu.org/licenses/. */

#include "charfreak.h"

int charfreak_count (char filename[], int filter_tp_setting, int case_setting, Counter counter[], int *distinct, int *total)
{
	//reading file
	FILE *fpr;
	if ((fpr = fopen (filename, "r")) == NULL)
		return 1; //fails to read
	
	//counting
	clear_counter (counter, distinct, total);
	wint_t current; //current character
	bool bom_state = false; //fvck Windows Notepad
	while ((current = fgetwc (fpr)) != WEOF)
	{
		if (!bom_state) //BOM exclusion
		{
			bom_state = true;
			if (current == 0xFEFF)
				continue;
		}
		if (filter_tp (filter_tp_setting, current)) //character filter on type
			continue;
		current = case_setting ? current : case_conv (current); //case conversion
		
		if (place_current (current, counter, distinct) == -1) //placing
		{
			fclose (fpr);
			return 2; //exceeds size of counter
		}
		++*total;
	}
	fclose (fpr);
	return 0;
}

/* Places the current character into the counter */
int place_current (wint_t current, Counter *counter, int *distinct)
{
	for (int i = 0; i < *distinct; i++)
	{
		if (current == counter[i].name)
		{
			counter[i].num++;
			return i; //into counter[i]
		}
	}
	if (*distinct < size_counter)
	{
		counter[*distinct].name = current;
		counter[*distinct].num = 1;
		++*distinct;
		return (*distinct - 1); //into a new place
	}
	return -1; //exceeds size of counter
}

/* Character exclusion */
bool filter_tp (int filter_tp_setting, wint_t current)
{
	switch (filter_tp_setting)
	{
		case 3:
			return (iswalpha (current) == 0);
		case 2:
			return (iswcntrl (current) || iswspace (current) || iswpunct (current));
		case 1:
			return (iswcntrl (current) || iswspace (current));
		case 0:
			return (iswcntrl (current));
	}
}

/* Case conversion */
wint_t case_conv (wint_t current)
{
	return (towlower (towupper (current)));
}

/* Clears the counter */
void clear_counter (Counter *counter, int *distinct, int *total)
{
	*total = 0;
	*distinct = 0;
	for (int i = 0; i < size_counter; i++)
	{
		counter[i].name = '\0';
		counter[i].num = 0;
	}
}
