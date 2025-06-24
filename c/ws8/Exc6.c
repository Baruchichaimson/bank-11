#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1) /* because we need to print the data to the file and we can't do a padding */

typedef struct {
    float literature;
    float sociology;
    float psychology;
} HumanisticGrades;

typedef struct {
    float physics;
    float math;
} RealGrades;

typedef struct {
    HumanisticGrades humanistic;
    RealGrades real;
    float sports;
} Grade;

typedef struct {
    char first_name[32];
    char last_name[32];
    Grade grades;
} Student;

#pragma pack(pop) /* release the define that dont do a padding */

void saveStudentToFile(const char* filename, const Student* s)
{
    FILE* f = fopen(filename, "wb");
    if (!f)
    {
        perror("Failed to open file for writing");
        exit(1);
    }
    fwrite(s, sizeof(Student), 1, f);
    fclose(f);
}

void loadStudentFromFile(const char* filename, Student* s)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
    {
        perror("Failed to open file for reading");
        exit(1);
    }
    fread(s, sizeof(Student), 1, f);
    fclose(f);
}

void printStudent(const Student* s)
{
    printf("Student: %s %s\n", s->first_name, s->last_name);
    printf("Humanistic Grades:\n");
    printf("  Literature: %.2f\n", s->grades.humanistic.literature);
    printf("  Sociology: %.2f\n", s->grades.humanistic.sociology);
    printf("  Psychology: %.2f\n", s->grades.humanistic.psychology);
    printf("Real Grades:\n");
    printf("  Physics: %.2f\n", s->grades.real.physics);
    printf("  Math: %.2f\n", s->grades.real.math);
    printf("Sports Grade: %.2f\n", s->grades.sports);
}

int main()
{
    Student original = 
    {
        "Alice",
        "Smith",
        {
            {95.5, 88.0, 91.0},  
            {98.0, 87.5},       
            92.0               
        }
    };
	Student loaded;
    const char* filename = "student.bin";
    
    saveStudentToFile(filename, &original);
    
    loadStudentFromFile(filename, &loaded);

    printf("Original Student:\n");
    printStudent(&original);
    printf("\nLoaded Student:\n");
    printStudent(&loaded);

    return 0;
}

