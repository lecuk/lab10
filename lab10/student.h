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

Student *student_new(const char *firstName, const char *lastName, const bool sex, struct tm *dateOfBirth);
int student_compareFirstName(const Student *student1, const Student *student2);
int student_compareLastName(const Student *student1, const Student *student2);
int student_compareAge(const Student *student1, const Student *student2);
int student_compareMean(const Student *student1, const Student *student2);
int student_compareSum(const Student *student1, const Student *student2);
int student_compareByAlphabet(const Student *student1, const Student *student2);
void student_addGrade(Student *student, const float grade);
void student_addGrades(Student *student, const float *grades, const size_t count);
float student_getGradeMean(const Student *student);
float student_getGradeSum(const Student *student);
unsigned long student_getGradeCount(const Student *student);
float student_getAge(const Student *student);
void student_freeMemory(Student *student);
float student_calculateAge(Student *student);
void student_calculateSumAndMean(Student *student);

#endif