#include "data_generator.h"

const char* firstMaleNames[] = {
	"Rick", "Morty",
	"Homer", "Bart", "Milhouse",
	"Peter", "Brian", "Steve", "Chris",
	"Philip", "Hubert",
	"Phineaz", "Ferb", "Heinz",
	"Bob", "Patrick",
	"Scooby", "Shaggy", "Fred"
};
const char* firstFemaleNames[] = {
	"Marge", "Lisa",
	"Lois", "Meg",
	"Leela",
	"Candace",
	"Sandy",
	"Daphnie", "Velma"
};

const char* lastNames[] = {
	// Rick and Morty
	"Sanchez",
	// Simpsons
	"Simpson", "Flanders", "Van_Houten", "Skinner",
	// Family Guy
	"Griffin", "Quagmire", "Brown", "Swanson", "Goldman",
	// Futurama
	"Fry", "Fansworth",
	// Phineaz and Ferb
	"Flynn", "Fletcher", "Doofershmitz",
	// Sponge Bob
	"Squarepants", "Star",
	// Scooby-Doo
	"Doo", "Rogers", "Jones", "Blake", "Dinkley"
};

Group* generate_random_students(const size_t count)
{
	Group* group = group_create("Random", SortedByName_Descending);
	for (size_t i = 0; i < count; i++)
	{
		char* firstName;
		char* lastName;
		bool gender = rand() % 2;
		if (gender == STUDENT_MALE) //male
		{
			firstName = firstMaleNames[rand() % (sizeof(firstMaleNames) / sizeof(char*))];
		}
		else
		{
			firstName = firstFemaleNames[rand() % (sizeof(firstFemaleNames) / sizeof(char*))];
		}
		lastName = lastNames[rand() % (sizeof(lastNames) / sizeof(char*))];

		int y = rand() % 40 + 1971;
		int m = rand() % 12 + 1;
		int d = rand() % dateTime_getMonthDayCount(m, y) + 1;
		struct tm* dateOfBirth = dateTime_construct(d, m, y);
		Student* student = student_new(firstName, lastName, gender, dateOfBirth);

		size_t maxGrades = rand() % 10 + 3;
		for (int j = 0; j < maxGrades; j++)
		{
			float grade = rand() % 10 + (float)(rand() % 10) / 10;
			student_addGrade(student, grade);
		}

		group_tryAddStudent(group, student);
	}
	return group;
}
