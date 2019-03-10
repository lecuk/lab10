#include "menu.h"
#include "colors.h"
#include "student.h"
#include "date_time.h"
#include "group_queries.h"
#include <stdio.h>

const char* get_input(char* input, size_t maxSize)
{
	if (maxSize < 2) return NULL;

	printf(C_FG_GREEN);
	do
	{
		printf("> ");
		fgets(input, maxSize, stdin); //i don't know why, but gets_s doesn't work
		fseek(stdin, 0, SEEK_END); //seeking stdin to prevent further reading if user entered more characters than maxSize
	} while (!*input || input[0] == '\n');
	char* newLine;
	if (newLine = strrchr(input, '\n'))
	{
		*newLine = 0;
	}
	printf(C_RESET);
	return input;
}

int menu_group_addStudent(Group* group)
{
	char firstName[STUDENT_MAX_FIRST_NAME], lastName[STUDENT_MAX_LAST_NAME], genderInput[7], confirm[4], dateOfBirth[20], gradeInput[15];

	while (1) //first and last name input
	{
		printf("Now, enter first name of the student:\n");
		get_input(firstName, STUDENT_MAX_FIRST_NAME);
		if (strchr(firstName, ' ') || strchr(firstName, '\t') || strchr(firstName, '\r') || strchr(firstName, '\n'))
		{
			printf(C_FG_ORANGE "Name cannot contain whitespaces. Use underscores \"_\".\n" C_RESET);
			continue;
		}

		printf("Enter last name of the student:\n");
		get_input(lastName, STUDENT_MAX_LAST_NAME);
		if (strchr(lastName, ' ') || strchr(lastName, '\t') || strchr(lastName, '\r') || strchr(lastName, '\n'))
		{
			printf(C_FG_ORANGE "Name cannot contain whitespaces. Use underscores \"_\".\n" C_RESET);
			continue;
		}

		printf("Student is named " C_FG_YELLOW "%s %s" C_RESET ". Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", firstName, lastName);
		get_input(confirm, 4);
		if (strcmp(confirm, "yes"))
		{
			printf(C_FG_GRAY "OK. Try again.\n" C_RESET);
			continue;
		}
		break;
	}

	bool sex;
	while (1) //gender input
	{
		printf("Enter gender of the student (" C_FG_AQUA "male" C_RESET "/" C_FG_AQUA "female" C_RESET "):\n");
		get_input(genderInput, 7);
		if (!strcmp(genderInput, "male"))
		{
			sex = STUDENT_MALE;
		}
		else if (!strcmp(genderInput, "female"))
		{
			sex = STUDENT_FEMALE;
		}
		else
		{
			printf(C_FG_ORANGE "There are only two genders.\n" C_RESET);
			continue;
		}

		printf("Student is " C_FG_YELLOW "%s" C_RESET ". Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", genderInput);
		get_input(confirm, 4);
		if (strcmp(confirm, "yes"))
		{
			printf(C_FG_GRAY "OK. Try again.\n" C_RESET);
			continue;
		}
		break;
	}

	int d, m, y;
	struct tm* dateOfBirthTime;
	char dateOfBirthOutput[50];
	while (1) //date of birth input
	{
		printf("Now, enter the date of birth of " C_FG_YELLOW "%s %s" C_RESET ".\n"
			"Format: " C_FG_AQUA "DD.MM.YYYY" C_RESET "\n", firstName, lastName);

		get_input(dateOfBirth, 20);
		while (sscanf_s(dateOfBirth, "%d.%d.%d", &d, &m, &y) != 3 || !(dateOfBirthTime = dateTime_construct(d, m, y)))
		{
			printf(C_FG_ORANGE "Invalid input. Format: " C_FG_AQUA "DD.MM.YYYY" C_RESET "\n");
			get_input(dateOfBirth, 20);
		}

		strftime(dateOfBirthOutput, 50, "%b %d, %Y", dateOfBirthTime);
		printf("Students date of birth is " C_FG_YELLOW "%s" C_RESET ". Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", dateOfBirthOutput);
		get_input(confirm, 4);
		if (strcmp(confirm, "yes"))
		{
			printf(C_FG_GRAY "OK. Try again.\n" C_RESET);
			continue;
		}
		break;
	}

	Student* student = student_new(firstName, lastName, rand() % 2, dateOfBirthTime);
	if (!student)
	{
		printf(C_BG_DARKRED C_FG_YELLOW "Something went wrong. Can't create student.\n" C_RESET);
		return menu_group(group);
	}

	while (1)
	{
		float grade = 0;

		printf("Enter grades to add:\n"
			"(Type " C_FG_AQUA "stop" C_RESET " to... stop)\n");
		do
		{
			get_input(gradeInput, 15);

			if (!strcmp(gradeInput, "stop")) break;

			if (!sscanf_s(gradeInput, "%f", &grade))
			{
				printf(C_FG_ORANGE "Invalid input, need to be a floating-point number.\n" C_RESET);
			}
			else
			{
				student_addGrade(student, grade);
			}
		} while (1);

		printf("Students sum = " C_FG_YELLOW "%0.2f" C_RESET ", mean = " C_FG_YELLOW "%0.2f" C_RESET ". Is it right? Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", student->sumGrade, student->meanGrade);
		get_input(confirm, 4);
		if (strcmp(confirm, "yes"))
		{
			llist_clear(student->grades);
			printf(C_FG_GRAY "OK. Try again.\n" C_RESET);
			continue;
		}
		break;
	}

	printf("Successfully created student " C_FG_YELLOW "%s %s" C_RESET ".\n"
		"Date of birth: " C_FG_YELLOW "%s" C_RESET " (age - " C_FG_YELLOW "%0.1f" C_RESET " years)\n"
		"Sum: " C_FG_YELLOW "%0.2f" C_RESET "\n"
		"Mean: " C_FG_YELLOW "%0.2f" C_RESET "\n", student->firstName, student->lastName, dateOfBirthOutput, student->age, student->sumGrade, student->meanGrade);

	group_tryAddStudent(group, student);

	return menu_group(group);
}

int menu_group_rename(Group* group)
{
	printf("Current group name is " C_FG_YELLOW "%s" C_RESET ". Enter the new name of a group.\n", group->name);
	char newName[GROUP_NAME_MAX];
	get_input(newName, GROUP_NAME_MAX);
	strncpy_s(group->name, GROUP_NAME_MAX, newName, GROUP_NAME_MAX);
	printf("Successfully changed group name to " C_FG_YELLOW "%s" C_RESET ".\n", newName);
	return menu_group(group);
}

void group_show(Group* group)
{
	int index = 1;
	printf("|----------------------------------------------------------------------------------------------------------|\n");
	printf("| %-40s   %5d student(s)    Mean age = %-8.2f Mean grade = %-8.1f |\n", group->name, group->students->count, group_getMeanAge(group), group_getMeanGrade(group));
	printf("|-----+--------------------+------------------------------+--------+--------------------+---------+--------|\n");
	printf("|  #  |        Name        |            Surname           | Gender |   Date of birth    |   Sum   |  Mean  |\n");
	llist_foreach(group->students, student)
	{
		Student* student = (Student*)student_node->item;
		printf("|-----+--------------------+------------------------------+--------+--------------------+---------+--------|\n");
		const char* gender = (student->gender == STUDENT_MALE) ? "Male" : "Female";
		char buffer[50];
		strftime(buffer, 50, "%b %d, %Y", student->dateOfBirth);
		
		printf("|%-5d|%-20s|%-30s|%-8s|%-20s|%-9.2f|%-8.2f|\n", index, student->firstName, student->lastName, gender, buffer, student->sumGrade, student->meanGrade);
		index++;
	}
	printf("|-----+--------------------+------------------------------+--------+--------------------+---------+--------|\n");
}

int menu_group_sort(Group* group)
{
	printf("How to sort a group? (Example: " C_FG_AQUA "age-" C_RESET " sorts in descending order by age)\n"
		"\t" C_FG_AQUA "name+" C_RESET "/" C_FG_AQUA "-" C_RESET " - sort by last name, then by first name, then by mean.\n"
		"\t" C_FG_AQUA "age+" C_RESET "/" C_FG_AQUA "-" C_RESET " - sort by age, then by name.\n"
		"\t" C_FG_AQUA "sum+" C_RESET "/" C_FG_AQUA "-" C_RESET " - sort by sum, then by name.\n"
		"\t" C_FG_AQUA "mean+" C_RESET "/" C_FG_AQUA "-" C_RESET " - sort by mean, then by name.\n"
		"\t" C_FG_AQUA "back" C_RESET " - back to group menu.\n");

	char inputBuffer[10];

	while (1)
	{
		get_input(inputBuffer, 10);

		if (!strcmp(inputBuffer, "name-"))
		{
			printf(C_FG_GRAY "Sorted in alphabetical order (descending).\n" C_RESET);
			group_sort(group, SortedByName_Descending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "name+"))
		{
			printf(C_FG_GRAY "Sorted in alphabetical order (ascending).\n" C_RESET);
			group_sort(group, SortedByName_Ascending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "age-"))
		{
			printf(C_FG_GRAY "Sorted by age (descending).\n" C_RESET);
			group_sort(group, SortedByAge_Descending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "age+"))
		{
			printf(C_FG_GRAY "Sorted by age (ascending).\n" C_RESET);
			group_sort(group, SortedByAge_Ascending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "sum-"))
		{
			printf(C_FG_GRAY "Sorted by sum (descending).\n" C_RESET);
			group_sort(group, SortedBySum_Descending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "sum+"))
		{
			printf(C_FG_GRAY "Sorted by sum (ascending).\n" C_RESET);
			group_sort(group, SortedBySum_Ascending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "mean-"))
		{
			printf(C_FG_GRAY "Sorted by mean (descending).\n" C_RESET);
			group_sort(group, SortedByMean_Descending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "mean+"))
		{
			printf(C_FG_GRAY "Sorted by mean (ascending).\n" C_RESET);
			group_sort(group, SortedByMean_Ascending);
			return menu_group(group);
		}
		else if (!strcmp(inputBuffer, "back"))
		{
			return menu_group(group);
		}
		else
		{
			printf(C_FG_ORANGE "Unknown command. Try again.\n" C_RESET);
		}
	}
}

int menu_group_filter(Group* group)
{
	printf("Filter by:\n"
		"\t" C_FG_AQUA "firstname" C_RESET " - students which contain selected substring in their first name only.\n"
		"\t" C_FG_AQUA "lastname" C_RESET " - students which contain selected substring in their last name only.\n"
		"\t" C_FG_AQUA "name" C_RESET " - students which contain selected substring in either first or last name.\n"
		"\t" C_FG_AQUA "gender" C_RESET " - students which are males or females only.\n"
		"\t" C_FG_AQUA "age" C_RESET " - students which are this old in selected range.\n"
		"\t" C_FG_AQUA "dateofbirth" C_RESET " - students which were born in selected range.\n"
		"\t" C_FG_AQUA "sum" C_RESET " - students which have their sum grade in selected range.\n"
		"\t" C_FG_AQUA "age" C_RESET " - students which have their mean grade in selected range.\n"
		"\t" C_FG_AQUA "back" C_RESET " - back to main menu.\n");

	char inputBuffer[100];

	while (1)
	{
		get_input(inputBuffer, 100);

		if (!strcmp(inputBuffer, "firstname"))
		{
			printf("Enter substring of student's first name you want to find: \n");
			get_input(inputBuffer, STUDENT_MAX_FIRST_NAME);
			Group* result = query_findByFirstName(group, inputBuffer);
			return menu_group(result);
		}
		else if (!strcmp(inputBuffer, "firstname"))
		{
			printf("Enter substring of student's last name you want to find: \n");
			get_input(inputBuffer, STUDENT_MAX_LAST_NAME);
			Group* result = query_findByLastName(group, inputBuffer);
			return menu_group(result);
		}
		else if (!strcmp(inputBuffer, "back"))
		{
			return menu_group(group);
		}
		else
		{
			printf(C_FG_ORANGE "Unknown command. Try again.\n" C_RESET);
		}
	}
}

int menu_group(Group* group)
{
	group_show(group);

	printf("Select next option:\n"
		"\t" C_FG_AQUA "add" C_RESET " - add new student.\n"
		"\t" C_FG_AQUA "student " C_FG_YELLOW "N" C_RESET " - menu of " C_FG_YELLOW "N" C_RESET "-th student in the group.\n"
		"\t" C_FG_AQUA "rename" C_RESET " - rename group.\n"
		"\t" C_FG_AQUA "sort" C_RESET " - sort list by name, grades etc.\n"
		"\t" C_FG_AQUA "filter" C_RESET " - filter list to find specific students.\n"
		"\t" C_FG_AQUA "exit" C_RESET " - save data and exit the program.\n");

	char inputBuffer[100];

	while (1)
	{
		get_input(inputBuffer, 100);

		if (!strcmp(inputBuffer, "add"))
		{
			return menu_group_addStudent(group);
		}
		else if (!strcmp(inputBuffer, "rename"))
		{
			return menu_group_rename(group);
		}
		else if (strstr(inputBuffer, "student"))
		{
			size_t index;
			if (sscanf_s(inputBuffer, "%*s %d", &index) != 1)
			{
				//bad input, no argument
				printf(C_FG_ORANGE "Invalid input, should include " C_FG_AQUA "index" C_FG_ORANGE " of student.\n" C_RESET);
			}
			else
			{
				if (index < 1 || index > group->students->count)
				{
					printf(C_FG_ORANGE "There are no student with this index.\n" C_RESET);
					continue;
				}
				int currentIndex = 1;
				llist_foreach(group->students, student)
				{
					if (index == currentIndex)
					{
						Student* student = (Student*)student_node->item;
						return menu_student(group, student);
					}
					currentIndex++;
				}
			}
		}
		else if (!strcmp(inputBuffer, "sort"))
		{
			return menu_group_sort(group);
		}
		else if (!strcmp(inputBuffer, "filter"))
		{
			return menu_group_filter(group);
		}
		else if (!strcmp(inputBuffer, "exit"))
		{
			group_saveToFile(group, "data.txt");
			printf(C_FG_GRAY "Exiting...\n" C_RESET);
			return 0;
		}
		else
		{
			printf(C_FG_ORANGE "Unknown command. Try again.\n" C_RESET);
		}
	}
}

int menu_student(Group* group, Student* student)
{
	printf("What do you want to do with " C_FG_YELLOW "%s %s" C_RESET "?\n", student->firstName, student->lastName);
	printf("Select next option:\n"
		"\t" C_FG_AQUA "edit" C_RESET " - edit student data\n"
		"\t" C_FG_AQUA "addgrade" C_RESET " - add new grade to student's marks\n"
		"\t" C_FG_AQUA "info" C_RESET " - show all info about this student\n"
		"\t" C_FG_AQUA "remove" C_RESET " - remove student from group\n"
		"\t" C_FG_AQUA "back" C_RESET " - go to main menu\n");

	char inputBuffer[100];

	while (1)
	{
		get_input(inputBuffer, 100);
		if (!strcmp(inputBuffer, "edit"))
		{
			return menu_student_edit(group, student);
		}
		else if (!strcmp(inputBuffer, "info"))
		{
			return menu_student_info(group, student);
		}
		else if (!strcmp(inputBuffer, "remove"))
		{
			return menu_student_remove(group, student);
		}
		else if (!strcmp(inputBuffer, "addgrade"))
		{
			return menu_student_addGrade(group, student);
		}
		else if (!strcmp(inputBuffer, "back"))
		{
			return menu_group(group);
		}
		else
		{
			printf(C_FG_ORANGE "Unknown command. Try again.\n" C_RESET);
		}
	}

	return menu_group(group);
}

int menu_student_edit(Group* group, Student* student)
{
	char confirm[4];

	printf("Do you need to change " C_FG_YELLOW "%s's %s" C_RESET " name? Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", student->firstName, student->lastName);
	get_input(confirm, 4);
	if (!strcmp(confirm, "yes"))
	{
		char firstName[STUDENT_MAX_FIRST_NAME], lastName[STUDENT_MAX_LAST_NAME];
		while (1)
		{
			printf("Enter the new first name of the student:\n");
			get_input(firstName, STUDENT_MAX_FIRST_NAME);
			if (strchr(firstName, ' ') || strchr(firstName, '\t') || strchr(firstName, '\r') || strchr(firstName, '\n'))
			{
				printf(C_FG_ORANGE "Name cannot contain whitespaces. Use underscores \"_\".\n" C_RESET);
				continue;
			}

			printf("Enter the new last name of the student:\n");
			get_input(lastName, STUDENT_MAX_LAST_NAME);
			if (strchr(lastName, ' ') || strchr(lastName, '\t') || strchr(lastName, '\r') || strchr(lastName, '\n'))
			{
				printf(C_FG_ORANGE "Name cannot contain whitespaces. Use underscores \"_\".\n" C_RESET);
				continue;
			}

			printf("Students new name is " C_FG_YELLOW "%s %s" C_RESET ". Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", firstName, lastName);
			get_input(confirm, 4);
			if (!strcmp(confirm, "yes"))
			{
				break;
			}
		}

		strncpy_s(student->firstName, STUDENT_MAX_FIRST_NAME, firstName, STUDENT_MAX_FIRST_NAME);
		strncpy_s(student->lastName, STUDENT_MAX_LAST_NAME, lastName, STUDENT_MAX_LAST_NAME);
	}

	printf("Do you need to change " C_FG_YELLOW "%s's %s" C_RESET " date of birth? Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", student->firstName, student->lastName);
	get_input(confirm, 4);
	if (!strcmp(confirm, "yes"))
	{
		struct tm* dateOfBirthTime;
		char dateOfBirth[20], dateOfBirthOutput[50];
		int d, m, y;
		while (1)
		{
			printf("Enter the new date of birth of " C_FG_YELLOW "%s %s" C_RESET ".\n"
				"Format: " C_FG_AQUA "DD.MM.YYYY" C_RESET "\n", student->firstName, student->lastName);

			get_input(dateOfBirth, 20);
			while (sscanf_s(dateOfBirth, "%d.%d.%d", &d, &m, &y) != 3 || !(dateOfBirthTime = dateTime_construct(d, m, y)))
			{
				printf(C_FG_ORANGE "Invalid input. Format: " C_FG_AQUA "DD.MM.YYYY" C_RESET "\n");
				get_input(dateOfBirth, 20);
			}

			strftime(dateOfBirthOutput, 50, "%b %d, %Y", dateOfBirthTime);
			printf("Students date of birth is " C_FG_YELLOW "%s" C_RESET ". Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", dateOfBirthOutput);
			get_input(confirm, 4);
			if (!strcmp(confirm, "yes"))
			{
				break;
			}
		}

		student->dateOfBirth = dateOfBirthTime;

		student_calculateAge(student);
	}

	printf("Do you need to change " C_FG_YELLOW "%s's %s" C_RESET " grades? Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", student->firstName, student->lastName);
	get_input(confirm, 4);
	if (!strcmp(confirm, "yes"))
	{
		char gradeInput[15];
		float grade;

		llist_clear(student->grades);

		printf("Enter new grades to add:\n"
			"(Type " C_FG_AQUA "stop" C_RESET " to... stop)\n");
		do
		{
			get_input(gradeInput, 15);

			if (!strcmp(gradeInput, "stop")) break;

			if (!sscanf_s(gradeInput, "%f", &grade))
			{
				printf(C_FG_ORANGE "Invalid input, need to be a floating-point number.\n" C_RESET);
			}
			else
			{
				student_addGrade(student, grade);
			}
		} while (1);
	}

	return menu_student(group, student);
}

int menu_student_info(Group* group, Student* student)
{
	const char* gender = (student->gender == STUDENT_MALE) ? "Male" : "Female";

	char buffer[60];
	strftime(buffer, 59, "%d %b, %Y", student->dateOfBirth);

	printf("Student: " C_FG_YELLOW "%s %s" C_RESET " (" C_FG_YELLOW "%s" C_RESET ").\n"
		"Date of birth: " C_FG_YELLOW "%s" C_RESET " ( " C_FG_YELLOW "%0.1f" C_RESET " years)\n"
		"Sum = " C_FG_YELLOW "%0.2f" C_RESET ". Mean = " C_FG_YELLOW "%0.2f" C_RESET ". Grades:\n [ " C_FG_YELLOW,
		student->firstName, student->lastName, gender, buffer, student->age, student->sumGrade, student->meanGrade);
	llist_foreach(student->grades, grade)
	{
		float grade = *(float*)(grade_node->item);
		printf("%0.2g ", grade);
	}
	printf(C_RESET "]\n");

	return menu_student(group, student);
}

int menu_student_remove(Group* group, Student* student)
{
	char confirm[4];
	printf("Are you sure you want to remove " C_FG_YELLOW "%s %s" C_RESET " from the group? Type " C_FG_AQUA "yes" C_RESET " to confirm.\n", student->firstName, student->lastName);
	get_input(confirm, 4);
	if (!strcmp(confirm, "yes"))
	{
		group_removeStudent(group, student);
		printf(C_FG_GRAY "Successfully removed " C_FG_YELLOW "%s %s" C_FG_GRAY ".\n" C_RESET, student->firstName, student->lastName);
		student_freeMemory(student);
	}
	return menu_group(group);
}

int menu_student_addGrade(Group* group, Student* student)
{
	char gradeInput[15];
	float grade;
	printf("Enter grades to add:\n"
		"(Type " C_FG_AQUA "stop" C_RESET " to... stop)\n");
	do
	{
		get_input(gradeInput, 15);

		if (!strcmp(gradeInput, "stop")) break;

		if (!sscanf_s(gradeInput, "%f", &grade))
		{
			printf(C_FG_ORANGE "Invalid input, need to be a floating-point number.\n" C_RESET);
		}
		else
		{
			student_addGrade(student, grade);
		}
	} while (1);

	return menu_student(group, student);
}
