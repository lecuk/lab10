//student.c

#include "student.h"

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>

static inline int s_sign(float a)
{
	return (a > 0) ? 1 : (a < 0) ? -1 : 0;
}
static inline int s_compareGradesUnsorted(const void *a, const void *b)
{
	return -1;
}
Student *student_new(const char *firstName, const char *lastName, const bool gender, struct tm *dateOfBirth)
{
	if (!firstName || !lastName || !dateOfBirth) return NULL;

	Student *student = (Student*)malloc(sizeof(Student));
	if (student)
	{
		strncpy_s(student->firstName, STUDENT_MAX_FIRST_NAME, firstName, STUDENT_MAX_FIRST_NAME);
		strncpy_s(student->lastName, STUDENT_MAX_LAST_NAME, lastName, STUDENT_MAX_LAST_NAME);
		student->gender = gender;
		student->dateOfBirth = dateOfBirth;
		student->grades = llist_create(s_compareGradesUnsorted, ByValue, sizeof(int));
		student_calculateAge(student);
		student_calculateSumAndMean(student);
	}
	return student;
}

void student_addGrade(Student *student, const float grade)
{
	llist_addValue(student->grades, &grade);
	student_calculateSumAndMean(student);
}

void student_calculateSumAndMean(Student *student)
{
	float sum = 0;
	int count = student->grades->count;
	llist_foreach(student->grades, grade)
	{
		sum += *(float*)grade_node->item;
	}
	if (count)
	{
		student->sumGrade = sum;
		student->meanGrade = sum / count;
	}
	else
	{
		student->sumGrade = 0;
		student->meanGrade = 0;
	}
}

void student_addGrades(Student *student, const float *grades, const size_t count)
{
	for (unsigned i = 0; i < count; i++)
	{
		student_addGrade(student, grades[i]);
	}
}

void student_calculateAge(Student *student)
{
	time_t now = time(NULL);
	time_t date = mktime(student->dateOfBirth);
	student->age = (float)(difftime(now, date) / (365.2468 * 86400));
}

void student_freeMemory(Student *student)
{
	llist_dispose(student->grades);
	free(student);
}

int student_compareFirstNameDescending(const Student *student1, const Student *student2)
{
	return -strcmp(student1->firstName, student2->firstName);
}

int student_compareLastNameDescending(const void *student1, const void *student2)
{
	return -strcmp(((Student*)student1)->lastName, ((Student*)student2)->lastName);
}

int student_compareAgeDescending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student1)->age - ((Student*)student2)->age);
}

int student_compareMeanDescending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student1)->meanGrade - ((Student*)student2)->meanGrade);
}

int student_compareSumDescending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student1)->sumGrade - ((Student*)student2)->sumGrade);
}

int student_compareNameDescending(const void *student1, const void *student2)
{
	int lastNameComparision = student_compareLastNameDescending(student1, student2);
	return (lastNameComparision) ? lastNameComparision : student_compareFirstNameDescending(student1, student2);
}

int student_compareFirstNameAscending(const Student *student1, const Student *student2)
{
	return strcmp(student1->firstName, student2->firstName);
}

int student_compareLastNameAscending(const void *student1, const void *student2)
{
	return strcmp(((Student*)student1)->lastName, ((Student*)student2)->lastName);
}

int student_compareAgeAscending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student2)->age - ((Student*)student1)->age);
}

int student_compareMeanAscending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student2)->meanGrade - ((Student*)student1)->meanGrade);
}

int student_compareSumAscending(const void *student1, const void *student2)
{
	return s_sign(((Student*)student2)->sumGrade - ((Student*)student1)->sumGrade);
}

int student_compareNameAscending(const void *student1, const void *student2)
{
	int lastNameComparision = student_compareLastNameAscending(student1, student2);
	return (lastNameComparision) ? lastNameComparision : student_compareFirstNameAscending(student1, student2);
}
