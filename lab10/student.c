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
	strncpy_s(student->firstName, STUDENT_MAX_FIRST_NAME, firstName, STUDENT_MAX_FIRST_NAME);
	strncpy_s(student->lastName, STUDENT_MAX_LAST_NAME, lastName, STUDENT_MAX_LAST_NAME);
	student->gender = gender;
	student->dateOfBirth = dateOfBirth;
	student->age = student_calculateAge(student);
	student->grades = llist_create(s_compareGradesUnsorted, ByValue, sizeof(int));
	student->sumGrade = 0;
	student->meanGrade = 0;
	return student;
}

int student_compareFirstName(const Student *student1, const Student *student2)
{
	return -strcmp(student1->firstName, student2->firstName);
}

int student_compareLastName(const Student *student1, const Student *student2)
{
	return -strcmp(student1->lastName, student2->lastName);
}

int student_compareAge(const Student *student1, const Student *student2)
{
	return s_sign((student_getAge(student1) - student_getAge(student2)));
}

int student_compareMean(const Student *student1, const Student *student2)
{
	return s_sign(student_getGradeMean(student1) - student_getGradeMean(student2));
}

int student_compareSum(const Student *student1, const Student *student2)
{
	return s_sign(student_getGradeSum(student1) - student_getGradeSum(student2));
}

int student_compareByAlphabet(const Student *student1, const Student *student2)
{
	int lastNameComparision = student_compareLastName(student1, student2);
	return (lastNameComparision) ? lastNameComparision : student_compareFirstName(student1, student2);
}

void student_addGrade(Student *student, const float grade)
{
	llist_addValue(student->grades, &grade);
	student_calculateSumAndMean(student);
}

void student_calculateSumAndMean(Student *student)
{
	float sum = 0;
	int count = 0;
	llist_foreach(student->grades, grade)
	{
		float grade = *(float*)grade_node->item;
		sum += grade;
		count++;
	}
	student->sumGrade = sum;
	student->meanGrade = sum / count;
}

void student_addGrades(Student *student, const float *grades, const size_t count)
{
	for (unsigned i = 0; i < count; i++)
	{
		student_addGrade(student, grades[i]);
	}
}

float student_getGradeMean(const Student *student)
{
	return student->meanGrade;
}

float student_getGradeSum(const Student *student)
{
	return student->sumGrade;
}

unsigned long student_getGradeCount(const Student *student)
{
	return student->grades->count;
}

float student_calculateAge(Student *student)
{
	time_t now = time(NULL);
	time_t date = mktime(student->dateOfBirth);
	return (float)(difftime(now, date) / (365.248 * 86400));
}

float student_getAge(const Student *student)
{
	return student->age;
}

void student_freeMemory(Student *student)
{
	llist_dispose(student->grades);
	free(student);
}
