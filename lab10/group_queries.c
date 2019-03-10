#include "group_queries.h"
#include "group.h"
#include "student.h"
#include "date_time.h"
#include "tools.h"
#include <stdio.h>
#include <ctype.h>

//thanks to this guy, i have this function below:
//https://stackoverflow.com/users/168986/clifford
char* stristr(const char* str1, const char* str2)
{
	const char* p1 = str1;
	const char* p2 = str2;
	const char* r = *p2 == 0 ? str1 : 0;

	while (*p1 != 0 && *p2 != 0)
	{
		if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
		{
			if (r == 0)
			{
				r = p1;
			}

			p2++;
		}
		else
		{
			p2 = str2;
			if (r != 0)
			{
				p1 = r + 1;
			}

			if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
			{
				r = p1;
				p2++;
			}
			else
			{
				r = 0;
			}
		}

		p1++;
	}

	return *p2 == 0 ? (char*)r : 0;
}

//-----------------------------------------------------------------------------------------

static const char* s_findByFirstName_substring;

static bool s_findByFirstName_condition(const void* item)
{
	return (stristr(((Student*)item)->firstName, s_findByFirstName_substring) != NULL);
}

Group* query_findByFirstName(const Group* group, const char* name)
{
	s_findByFirstName_substring = name;
	return group_fromAnotherGroup("First name search result", s_findByFirstName_condition, group);
}

//-----------------------------------------------------------------------------------------

static const char* s_findByLastName_substring;

static bool s_findByLastName_condition(const void* item)
{
	return (stristr(((Student*)item)->lastName, s_findByLastName_substring) != NULL);
}

Group* query_findByLastName(const Group* group, const char* name)
{
	s_findByLastName_substring = name;
	return group_fromAnotherGroup("Last name search result", s_findByLastName_condition, group);
}

//-----------------------------------------------------------------------------------------

static bool s_findByName_condition(const void* item)
{
	return s_findByFirstName_condition(item) || s_findByLastName_condition(item);
}

Group* query_findByName(const Group* group, const char* name)
{
	s_findByFirstName_substring = name;
	s_findByLastName_substring = name;
	return group_fromAnotherGroup("Name search result", s_findByLastName_condition, group);
}

//-----------------------------------------------------------------------------------------

static const float* s_findByGrades_gradesArray;
static size_t s_findByGrades_gradesCount;

static bool s_findByGrades_condition(const void* item)
{
	for (size_t i = 0; i < s_findByGrades_gradesCount; i++)
	{
		if (llist_containsValue(((Student*)item)->grades, s_findByGrades_gradesArray + i)) return true;
	}
	return false;
}

Group* query_findByGrades(const Group* group, const float* grades, const size_t size)
{
	s_findByGrades_gradesArray = grades;
	s_findByGrades_gradesCount = size;
	return group_fromAnotherGroup("Grades search result", s_findByGrades_condition, group);
}

//-----------------------------------------------------------------------------------------

static float s_findBySum_min;
static float s_findBySum_max;

static bool s_findBySum_condition(const void* item)
{
	return ((((Student*)item)->sumGrade > s_findBySum_min) && (((Student*)item)->sumGrade < s_findBySum_max));
}

Group* query_findBySum(const Group* group, const float min, const float max)
{
	s_findBySum_min = min;
	s_findBySum_max = max;
	return group_fromAnotherGroup("Sum search result", s_findBySum_condition, group);
}

//-----------------------------------------------------------------------------------------

static float s_findByMean_min;
static float s_findByMean_max;

static bool s_findByMean_condition(const void* item)
{
	return ((((Student*)item)->meanGrade > s_findByMean_min) && (((Student*)item)->meanGrade < s_findByMean_max));
}

Group* query_findByMean(const Group* group, const float min, const float max)
{
	s_findByMean_min = min;
	s_findByMean_max = max;
	return group_fromAnotherGroup("Mean search result", s_findByMean_condition, group);
}

//-----------------------------------------------------------------------------------------

Group* group_loadFromFile(const char *path)
{
	FILE *file;
	fopen_s(&file, path, "r");
	if (!file)
	{
		return NULL;
	}

	char groupName[GROUP_NAME_MAX];
	size_t studentCount;
	int sortType;
	if (fscanf_s(file, "%s%d%d", groupName, GROUP_NAME_MAX, &sortType, &studentCount) != 3)
	{
		fclose(file);
		return NULL;
	}

	Group* group = group_create(groupName, (StudentSort)sortType);
	if (!group)
	{
		fclose(file);
		return NULL;
	}

	for (size_t i = 0; i < studentCount; i++)
	{
		char firstName[STUDENT_MAX_FIRST_NAME], lastName[STUDENT_MAX_LAST_NAME];
		bool sex;
		int d, m, y;
		size_t gradeCount;
		if (fscanf_s(file, "%s%s%d%d%d%d%d", firstName, STUDENT_MAX_FIRST_NAME, lastName, STUDENT_MAX_LAST_NAME, &sex, &d, &m, &y, &gradeCount) != 7)
		{
			break;
		}

		Student *student = student_new(firstName, lastName, sex, dateTime_construct(d, m, y));
		if (!student) break; //some data was invalid

		bool errorReadingGrades = false;
		for (uint32 j = 0; j < gradeCount; j++)
		{
			float grade;
			if (fscanf_s(file, "%f", &grade))
			{
				student_addGrade(student, grade);
			}
			else
			{
				errorReadingGrades = true;
				break;
			}
		}
		if (errorReadingGrades) break;

		group_tryAddStudent(group, student);
	}
	fclose(file);
	return group;
}

void group_saveToFile(const Group* group, const char* path)
{
	FILE* file;
	fopen_s(&file, path, "w");
	if (!file) return; //just to be sure

	fprintf(file, "%s %d %d\n", group->name, (int)group->sort, group->students->count);
	llist_foreach(group->students, student)
	{
		Student* student = student_node->item;

		int d, m, y;
		dateTime_getTrueDMY(student->dateOfBirth, &d, &m, &y);
		fprintf(file, "%s %s %d %d %d %d %d\n", student->firstName, student->lastName, student->gender, d, m, y, student->grades->count);
		llist_foreach(student->grades, grade)
		{
			float grade = *((float*)grade_node->item);

			if (grade_node->nextNode) fprintf(file, "%0.2f ", grade);
			else fprintf(file, "%0.2f\n", grade);
		}
	}

	fclose(file);
}
