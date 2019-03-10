#include <stdio.h>
#include "group.h"
#include "student.h"
#include "tools.h"

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

static inline bool _noCondition(const void *target)
{
	return 1;
}
Group *group_create(const char *name, const StudentSort sort)
{
	return group_createWithCondition(name, _noCondition, sort);
}

Group *group_createWithCondition(const char *name, Condition *condition, const StudentSort sort)
{
	Group *group = (Group*)malloc(sizeof(Group));
	if (group)
	{
		group_rename(group, name);
		switch (sort)
		{
		case SortedByAge:
			group->students = llist_create(student_compareAge, ByPointer, sizeof(Student));
			break;
		case SortedBySum:
			group->students = llist_create(student_compareSum, ByPointer, sizeof(Student));
			break;
		case SortedByMean:
			group->students = llist_create(student_compareMean, ByPointer, sizeof(Student));
			break;
		case SortedByName:
		default:
			group->students = llist_create(student_compareByAlphabet, ByPointer, sizeof(Student));
			break;
		}
		group->sort = sort;
		group->condition = condition;
	}
	return group;
}

Group *group_fromAnotherGroup(const char *name, Condition *condition, const Group *original)
{
	Group *group = group_createWithCondition(name, condition, original->sort);
	group_tryAddToAnotherGroup(original, group);
	return group;
}

void group_printInfo(const Group *group)
{
	printf("--------------Group \"%s\"------------\n", group->name);
	llist_foreach(group->students, student_printInfo);
	printf("--------------------------------------\n");
}

void group_tryAddStudent(Group *group, const Student *student)
{
	if (group_doesStudentMatchCondition(group, student))
	{
		llist_add(group->students, student);
	}
}

void group_removeStudent(Group *group, const Student *student)
{
	llist_remove(group->students, student);
}

void group_sortByName(Group *group)
{
	llist_rearrange(group->students, student_compareByAlphabet);
	group->sort = SortedByName;
}

void group_sortByAge(Group *group)
{
	llist_rearrange(group->students, student_compareAge);
	group->sort = SortedByAge;
}

void group_sortBySum(Group *group)
{
	llist_rearrange(group->students, student_compareSum);
	group->sort = SortedBySum;
}

void group_sortByMean(Group *group)
{
	llist_rearrange(group->students, student_compareMean);
	group->sort = SortedByMean;
}

static Group *_curGroup;
static void _removeIfNecessary(void *item)
{
	Student *student = (Student*)item;
	if (!group_doesStudentMatchCondition(_curGroup, student))
	{
		group_removeStudent(_curGroup, student);
	}
}
void group_changeCondition(Group *group, Condition *newCondition)
{
	group->condition = newCondition;
	_curGroup = group;
	llist_foreach(group->students, _removeIfNecessary);
}

bool group_doesStudentMatchCondition(const Group *group, const Student *student)
{
	return group->condition(student);
}

static Group *_curDestination;
static void _addIfNecessary(void *target)
{
	group_tryAddStudent(_curDestination, (Student*)target);
}
void group_tryAddToAnotherGroup(const Group *source, Group *destination)
{
	_curDestination = destination;
	llist_foreach(source->students, _addIfNecessary);
}

void group_rename(Group* group, const char* name)
{
	strncpy_s(group->name, GROUP_NAME_MAX, name, GROUP_NAME_MAX);
}