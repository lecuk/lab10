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
		group->students = llist_create(NULL, ByPointer, sizeof(Student));
		group_rename(group, name);
		group->condition = condition;
		group_sort(group, sort);
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

void group_changeCondition(Group *group, Condition *newCondition)
{
	group->condition = newCondition;
	llist_foreach(group->students, student)
	{
		if (!group_doesStudentMatchCondition(group, student_node->item))
		{
			group_removeStudent(group, (Student*)(student_node->item));
		}
	}
}

bool group_doesStudentMatchCondition(const Group *group, const Student *student)
{
	return group->condition(student);
}

void group_tryAddToAnotherGroup(const Group *source, Group *destination)
{
	llist_foreach(source->students, student)
	{
		if (group_doesStudentMatchCondition(destination, student_node->item))
		{
			group_tryAddStudent(destination, (Student*)(student_node->item));
		}
	}
}

void group_rename(Group* group, const char* name)
{
	strncpy_s(group->name, GROUP_NAME_MAX, name, GROUP_NAME_MAX);
}

void group_sort(Group* group, StudentSort sort)
{
	switch (sort)
	{
	case SortedByAge_Descending:
		llist_rearrange(group->students, student_compareAgeDescending);
		break;
	case SortedBySum_Descending:
		llist_rearrange(group->students, student_compareSumDescending);
		break;
	case SortedByMean_Descending:
		llist_rearrange(group->students, student_compareMeanDescending);
		break;
	case SortedByName_Descending:
		llist_rearrange(group->students, student_compareNameDescending);
		break;
	case SortedByAge_Ascending:
		llist_rearrange(group->students, student_compareAgeAscending);
		break;
	case SortedBySum_Ascending:
		llist_rearrange(group->students, student_compareSumAscending);
		break;
	case SortedByMean_Ascending:
		llist_rearrange(group->students, student_compareMeanAscending);
		break;
	case SortedByName_Ascending:
	default:
		llist_rearrange(group->students, student_compareNameAscending);
		break;
	}

	group->sort = sort;
}

float group_getMeanAge(Group* group)
{
	float sum = 0;
	int count = group->students->count;
	llist_foreach(group->students, student)
	{
		sum += ((Student*)(student_node->item))->age;
	}
	return (count) ? (sum / count) : 0;
}

float group_getMeanGrade(Group* group)
{
	float sum = 0;
	int count = group->students->count;
	llist_foreach(group->students, student)
	{
		sum += ((Student*)(student_node->item))->meanGrade;
	}
	return (count) ? (sum / count) : 0;
}