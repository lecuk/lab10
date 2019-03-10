//student.h
#ifndef STUDENT_H
#define STUDENT_H

#include <time.h>
#include "llist.h"

#define STUDENT_MAX_FIRST_NAME 20
#define STUDENT_MAX_LAST_NAME 30

#define STUDENT_MALE 1
#define STUDENT_FEMALE 0

typedef struct TStudent
{
	char firstName[20];
	char lastName[30];
	bool gender;
	struct tm *dateOfBirth;
	float age;
	LinkedList *grades;
	float meanGrade;
	float sumGrade;
} Student;

Student *student_new(const char *firstName, const char *lastName, const bool gender, struct tm *dateOfBirth);
void student_addGrade(Student *student, const float grade);
void student_addGrades(Student *student, const float *grades, const size_t count);
void student_freeMemory(Student *student);
void student_calculateAge(Student *student);
void student_calculateSumAndMean(Student *student);

int student_compareFirstNameDescending(const void *student1, const void *student2);
int student_compareLastNameDescending(const void *student1, const void *student2);
int student_compareNameDescending(const void *student1, const void *student2);
int student_compareAgeDescending(const void *student1, const void *student2);
int student_compareMeanDescending(const void *student1, const void *student2);
int student_compareSumDescending(const void *student1, const void *student2);
int student_compareFirstNameAscending(const void *student1, const void *student2);
int student_compareLastNameAscending(const void *student1, const void *student2);
int student_compareNameAscending(const void *student1, const void *student2);
int student_compareAgeAscending(const void *student1, const void *student2);
int student_compareMeanAscending(const void *student1, const void *student2);
int student_compareSumAscending(const void *student1, const void *student2);

#endif