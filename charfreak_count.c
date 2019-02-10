/* Copyright 2019 Flora Canou | V. 1.1.0 | This file is part of CharFreak. 
 * CharFreak is free software, licensed under the GNU General Public License, v. 3 or later. 
 * If you have not received a copy of the license, visit https://www.gnu.org/licenses/. */

#include "charfreak.h"

int charfreakCount (char filename[], int exclSetting, int caseSetting, Counter counter[], int *distinct, int *total)
{
	//read file
	FILE *fpr;
	if ((fpr = fopen (filename, "r")) == NULL)
		return 1; //fails to read
	
	//counting
	clearCounter (counter, distinct, total);
	wint_t current; //current character
	bool distinctMark = true; //mark for distinct character
	int bomState = 0; //fvck Windows Notepad
	while ((current = fgetwc (fpr)) != WEOF)
	{
		if (bomState != 4) //checks UTF-8 BOM and excludes it if present
		{
			if (bomState == 0 && current == 0xEF || bomState == 1 && current == 0xBB || bomState == 2 && current == 0xBF)
				bomState++; //match
			else
				bomState = 4; //turns out BOM isn't present
			if (bomState == 3) //turns out BOM is present
			{
				clearCounter (counter, distinct, total);
				bomState = 4;
				continue;
			}
		}
		if (excl (exclSetting, current)) continue; //character exclusion
		current = caseSetting ? current : caseConv (current); //case conversion
		
		++*total;
		distinctMark = true;
		for (int i = 0; i < *distinct; i++)
		{
			if (current == counter[i].name)
			{
				counter[i].num++;
				distinctMark = false;
			}
		}
		if (distinctMark)
		{
			if (*distinct < sizec)
			{
				counter[*distinct].name = current;
				counter[*distinct].num = 1;
				++*distinct;
			}
			else
			{
				fclose (fpr);
				return 2; //exceeds size of counter
			}	
		}
	}
	fclose (fpr);
	return 0;
}

/* Character exclusion */
bool excl (int exclSetting, wint_t current)
{
	switch (exclSetting)
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
wint_t caseConv (wint_t current)
{
	return (towlower (towupper (current)));
}

/* Clear counter */
void clearCounter (Counter *counter, int *distinct, int *total)
{
	*total = 0;
	*distinct = 0;
	for (int i = 0; i < sizec; i++)
	{
		counter[i].name = '\0';
		counter[i].num = 0;
	}
}

