#ifndef _ENTITY_H_
#define _ENTITY_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entity_s Entity;
typedef Entity *Entity_p;
typedef unsigned int uint;

struct Entity_s {
    char *name;
    Entity_p *wishes;
    uint counter;
    uint total;
    int chosen;
};


void test_pointer(void* ptr, const char* msg);

Entity_p makeEntity(char *name, uint total);

int addEntity(Entity_p ent, Entity_p wish);

void freeEntity(Entity_p entity);

Entity_p getWish(Entity_p entity, uint index);

Entity_p getEntityByName(Entity_p *ents, int nb_ents, char *name);

char *getName(Entity_p ent);

Entity_p getChosen(Entity_p ent);

void setChosen(Entity_p ent, int index);

bool prefersToChosen(Entity_p ent, Entity_p choice);

void readStudentFromFile(FILE* file, Entity_p **students, int *nb_students_p, Entity_p **schools, int *nb_schools_p);



#endif