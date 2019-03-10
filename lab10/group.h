#ifndef GROUP_H
#define GROUP_H

#include "llist.h"
#include "student.h"

#define GROUP_NAME_MAX 35

typedef enum EStudentSort
{
	SortedByName_Ascending = 0,
	SortedByAge_Ascending,
	SortedBySum_Ascending,
	SortedByMean_Ascending,
	SortedByName_Descending,
	SortedByAge_Descending,
	SortedBySum_Descending,
	SortedByMean_Descending
} StudentSort;

typedef struct TGroup
{
	char name[GROUP_NAME_MAX];
	Condition* condition;
	LinkedList* students;
	StudentSort sort;
} Group;

Group *group_create(const char *name, const StudentSort sort);
Group *group_createWithCondition(const char *name, Condition *condition, const StudentSort sort);
Group *group_fromAnotherGroup(const char *name, Condition *condition, const Group *original);
void group_printInfo(const Group *group);
void group_tryAddStudent(Group *group, const Student *student);
void group_removeStudent(Group *group, const Student *student);
void group_sort(Group* group, StudentSort sort);
void group_changeCondition(Group *group, Condition *newCondition);
bool group_doesStudentMatchCondition(const Group *group, const Student *student);
void group_tryAddToAnotherGroup(const Group *source, Group *destination);
void group_rename(Group* group, const char* name);
float group_getMeanAge(Group* group);
float group_getMeanGrade(Group* group);

#endif