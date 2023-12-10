#include "solve.h"


void test_pointer(void* ptr, const char* msg) {
    if (ptr == NULL) {
        fprintf(stderr, "Error: %s is NULL\n", msg);
        exit(EXIT_FAILURE);
    }
}

Entity_p makeEntity(char *name, uint total){
    if (name == NULL){
        test_pointer(name, "Invalid name parameter in function makeEntity\n");
    }
    Entity_p res = malloc(sizeof(Entity));
    if (res == NULL){
        test_pointer(res, "memory allocation problem\n");
    }
    uint size = strlen(name);
    char *dest = malloc(size * sizeof(char));
    if (dest == NULL){
        test_pointer(dest, "memory allocation problem or invalid size\n");
    }
    res->name = strcpy(dest, name);
    if (res->name != dest){
        test_pointer(res->name, "error while copying the string\n");
    }
    res->total = total;
    res->chosen = -1;
    res->counter = 0;

    res->wishes = malloc(res->total * sizeof(Entity_p));
    if (res->wishes == NULL){
        test_pointer(res->wishes, "memory allocation problem for wishes array\n");
    }
    for(int i = 0; i < res->total; i++){
        res->wishes[i] = NULL;
    }
    return res;
}

int addEntity(Entity_p ent, Entity_p wish){
    if (ent == NULL || wish == NULL){
        test_pointer(ent, "Invalid ent or wish parameter in function addEntity\n");
    }
    if (ent->counter >= ent->total){
        fprintf(stderr, "Error: the entity %s has already %d wishes\n", ent->name, ent->total);
        return -1;
    }
    ent->wishes[ent->counter] = wish;
    ent->counter++;
    return 0;
}

void freeEntity(Entity_p entity){
    if (entity != NULL){
        free(entity->name);
        for(int i = 0; i < entity->total; i++){
            free(entity->wishes[i]);
        }
        free(entity->wishes);
    }
    free(entity);
}


Entity_p getWish(Entity_p entity, uint index){
    if (entity == NULL){
        test_pointer(entity, "Invalid entity parameter in function getWish\n");
    }
    if (index >= entity->total){
        fprintf(stderr, "Error: the entity %s has only %d wishes\n", entity->name, entity->total);
        return NULL;
    }
    return entity->wishes[index];
}


Entity_p getEntityByName(Entity_p *ents, int nb_ents, char *name){
    if (ents == NULL || name == NULL){
        test_pointer(ents, "Invalid ents or name parameter in function getEntityByName\n");
    }
    for(int i = 0; i < nb_ents; i++){
        if (strcmp(ents[i]->name, name) == 0){
            return ents[i];
        }
    }
    return NULL;
}

char *getName(Entity_p ent){
    if (ent == NULL){
        test_pointer(ent, "Invalid ent parameter in function getName\n");
    }
    return ent->name;
}

Entity_p getChosen(Entity_p ent){
    if (ent == NULL){
        test_pointer(ent, "Invalid ent parameter in function getChosen\n");
    }
    return ent->wishes[ent->chosen];
}

void setChosen(Entity_p ent, int index){
    if (ent == NULL || index >= ent->total){
        test_pointer(ent, "Invalid ent or index parameter in function setChosen\n");
    }
    ent->chosen = index;
}

bool prefersToChosen(Entity_p ent, Entity_p choice){
    if (ent == NULL || choice == NULL){
        test_pointer(ent, "Invalid parameters ent or choice\n");
    }
    if (ent->chosen == -1){
        return true;
    }
    for(int i = 0; i < ent->chosen; i++){
        if (ent->wishes[i] == choice){
            return true;
        }
    }
    return false;
}


void readStudentFromFile(FILE* file, Entity_p**students, int *nb_students_p, Entity_p **schools, int *nb_schools_p){
    if (file == NULL){
        test_pointer(file, "Invalid file in parameter\n");
    }
    int nb_students = 0;
    int nb_schools = 0;
    if (fscanf(file, "%d", &nb_students) != 1){
        fprintf(stderr, "Couldnt read from file\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(file, "%d", &nb_schools) != 1){
        fprintf(stderr, "Couldnt read from file\n");
        exit(EXIT_FAILURE);
    }

    Entity_p *students_array = malloc(nb_students * sizeof(Entity_p));
    test_pointer(students_array, "Memory allocation problem\n");

    Entity_p *schools_array = malloc(nb_schools * sizeof(Entity_p));
    test_pointer(schools_array, "Memory allocation problem\n");   


    char bufferStudentsNames[50];
    char bufferSchoolNames[50];

    for(int i = 0; i < nb_students; i++){
        fscanf(file, "%s", bufferStudentsNames);
        students_array[i] = makeEntity(bufferStudentsNames, nb_schools);
    }

    for(int i = 0; i < nb_schools; i++){
        fscanf(file, "%s", bufferSchoolNames);
        schools_array[i] = makeEntity(bufferSchoolNames, nb_students);
    }

    for (int i = 0 ; i < nb_students; i++){
        for (int j = 0; j < nb_schools; j++){
            fscanf(file, "%s", bufferSchoolNames);
            Entity_p school = getEntityByName(schools_array, nb_schools, bufferSchoolNames);
            addEntity(students_array[i], school);
        }
    }

    for(int i = 0; i < nb_schools; i++){
        for(int j = 0; j < nb_students; j++){
            fscanf(file, "%s", bufferStudentsNames);
            Entity_p student = getEntityByName(students_array, nb_students, bufferStudentsNames);
            addEntity(schools_array[i], student);
        }
    }

    *students = students_array;
    *nb_students_p = nb_students;
    *schools = schools_array;
    *nb_schools_p = nb_schools;
}


int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        fprintf(stderr, "Error: couldnt open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    Entity_p *students = NULL;
    Entity_p *schools = NULL;
    int nb_students = 0;
    int nb_schools = 0;
    readStudentFromFile(file, &students, &nb_students, &schools, &nb_schools);
    fclose(file);

    printf("Students:\n");
    for(int i = 0; i < nb_students; i++){
        printf("%s\n", getName(students[i])); // Assuming getName gets the name of an Entity
        printf("%s's Wishes:\n", getName(students[i]));
        for(int j = 0; j < nb_schools; j++){
            printf("%s\n", getName(getWish(students[i], j)));
        }
    }
    printf("Schools:\n");
    for(int i = 0; i < nb_schools; i++){
        printf("%s\n", getName(schools[i])); // Assuming getName gets the name of an Entity
        printf("College wishes:\n");
        for(int j = 0; j < nb_students; j++){
            printf("%s\n", getName(getWish(schools[i], j)));
        }
    }

    printf("\n\n");

    solve(schools, nb_schools, students, nb_students);

    // Print the result
    printf("Result:\n");
    for(int i = 0; i < nb_students; i++){
        printf("Student %s is assigned to %s\n", students[i]->name, getChosen(students[i])->name);
    }

    return 0;


    for(int i = 0; i < nb_students; i++){
        free(students[i]); // Assuming freeEntity frees an Entity
    }
    free(students); // Free the array of student pointers

    for(int i = 0; i < nb_schools; i++){
        free(schools[i]); // Assuming freeEntity frees an Entity
    }
    free(schools); // Free the array of school pointers

    return EXIT_SUCCESS;
}

