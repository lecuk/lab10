#ifndef MENU_H
#define MENU_H

#include "student.h"
#include "group.h"

const char* get_input(char* input, size_t maxSize);

int menu_group(Group* group);
int menu_group_addStudent(Group* group);
int menu_group_sort(Group* group);
int menu_group_rename(Group* group);

int menu_student(Group* group, Student* student);
int menu_student_edit(Group* group, Student* student);
int menu_student_info(Group* group, Student* student);
int menu_student_remove(Group* group, Student* student);
int menu_student_addGrade(Group* group, Student* student);

#endif