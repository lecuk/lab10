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

#include <stdio.h>
#include <string.h>
#include <conio.h>

const char* firstMaleNames[] = { 
	"Rick", "Morty", 
	"Homer", "Bart", "Milhouse",
	"Peter", "Brian", "Steve", "Chris", 
	"Philip", "Hubert", 
	"Phineaz", "Ferb", "Heinz", 
	"Bob", "Patrick", 
	"Scooby", "Shaggy", "Fred" 
};
const char* firstFemaleNames[] = { 
	"Marge", "Lisa", 
	"Lois", "Meg", 
	"Leela", 
	"Candace", 
	"Sandy", 
	"Daphnie", "Velma" 
};

const char* lastNames[] = { 
	"Sanchez",												// Rick and Morty
	"Simpson", "Flanders", "Van_Houten", "Skinner",			// Simpsons
	"Griffin", "Quagmire", "Brown", "Swanson", "Goldman",	// Family Guy
	"Fry", "Fansworth",										// Futurama
	"Flynn", "Fletcher", "Doofershmitz",					// Phineaz and Ferb
	"Squarepants", "Star",									// Sponge Bob
	"Doo", "Rogers", "Jones", "Blake", "Dinkley"			// Scooby-Doo
};

Group* generate_random_students(const size_t count)
{
	Group* group = group_create("Random", SortedByName);
	for (size_t i = 0; i < count; i++)
	{
		char* firstName;
		char* lastName;
		bool sex = rand() % 2;
		if (sex == STUDENT_MALE) //male
		{
			firstName = firstMaleNames[rand() % (sizeof(firstMaleNames) / sizeof(char*))];
		}
		else
		{
			firstName = firstFemaleNames[rand() % (sizeof(firstFemaleNames) / sizeof(char*))];
		}
		lastName = lastNames[rand() % (sizeof(lastNames) / sizeof(char*))];

		int y = rand() % 40 + 1971;
		int m = rand() % 12 + 1;
		int d = rand() % dateTime_getMonthDayCount(m, y) + 1;
		struct tm* dateOfBirth = dateTime_construct(d, m, y);
		Student* student = student_new(firstName, lastName, sex, dateOfBirth);

		size_t maxGrades = rand() % 10 + 3;
		for (int j = 0; j < maxGrades; j++)
		{
			float grade = rand() % 10 + (float)(rand() % 10) / 10;
			student_addGrade(student, grade);
		}

		group_tryAddStudent(group, student);
	}
	return group;
}

int main()
{
	printf("Loading data...\n");
	Group* currentGroup = group_loadFromFile("data.txt");

	if (!currentGroup)
	{
		printf("Can't load file. Creating new file...\n");
		currentGroup = group_create("Unnamed_group", SortedByName);
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