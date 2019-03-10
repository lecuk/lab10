#ifndef GROUP_QUERIES_H
#define GROUP_QUERIES_H

#include "group.h"

Group* query_findByFirstName(const Group* group, const char* name);
Group* query_findByLastName(const Group* group, const char* surname);
Group* query_findByGrades(const Group* group, const float* grades, const size_t size);
Group* query_findBySum(const Group* group, const float min, const float max);
Group* query_findByMean(const Group* group, const float min, const float max);

Group* group_loadFromFile(const char *path);
void group_saveToFile(const Group* group, const char* path);

#endif