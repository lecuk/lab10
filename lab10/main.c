/*
	Компілятору наш
	ти що єси на жосткому диску
	та збережеться ім'я твоє
	та прийде час твій
	та буде кінець програми твоєї очевидний як юзеру так і кодеру
	сурс код дай нам насущний
	і не веди нас до ерорів
	але визволи нас від ворнінгів
	return 0;
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "llist.h"
#include "student.h"
#include "group.h"
#include "colors.h"
#include "group_queries.h"
#include "date_time.h"
#include "menu.h"
#include "data_generator.h"

#include <stdio.h>
#include <string.h>
#include <conio.h>

int main()
{
	printf("Loading data...\n");
	Group* currentGroup = group_loadFromFile("data.txt");

	if (!currentGroup)
	{
		printf("Can't load file. Creating new file...\n");
		currentGroup = group_create("Unnamed_group", SortedByName_Descending);
		if (!currentGroup)
		{
			printf(C_BG_DARKRED C_FG_YELLOW "Something went wrong. Exiting...\n" C_RESET);
			return -1;
		}
	}
	menu_group(currentGroup);

	printf(C_FG_GRAY "Press " C_FG_AQUA "ENTER" C_FG_GRAY " to close the program...\n" C_RESET);
	getchar();

	return 0;
}