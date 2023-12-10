#include "solve.h"

Entity_p ExistsStudentNotChosen(Entity_p *students, int nb_students){
    if (students == NULL){
        test_pointer(students, "Invalid parameters or null pointer\n");
    }
    for(int i = 0 ; i < nb_students; i++){
        if (students[i]->chosen == -1){
            return students[i];
        }
    }
    return NULL;
}



int solve(Entity_p *schools, int nb_schools, Entity_p *students, int nb_students){
    // Initialize all students and schools as unassigned
    for(int i = 0; i < nb_students; i++){
        students[i]->chosen = -1;
    }
    for(int i = 0; i < nb_schools; i++){
        schools[i]->chosen = -1;
    }

    Entity_p s = NULL;
    int student_indx = 0;
    while ( (s = ExistsStudentNotChosen(students, nb_students)) != NULL){
        for(int i = 0; i < s->total; i++){
            Entity_p school = s->wishes[i];
            if (school->chosen == -1){
                school->chosen = student_indx;
                s->chosen = i;
                break;
            } else {
                Entity_p current_match = school->wishes[school->chosen];
                printf("the current student match is %s\n", current_match->name);
                if (prefersToChosen(school, s)){
                    current_match->chosen = -1;
                    school->chosen = student_indx;
                    s->chosen = i;
                    break;
                }
            }
        }
        student_indx++;
    }
    return EXIT_SUCCESS;
}