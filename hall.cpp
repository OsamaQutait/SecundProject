#include "local.h"
static ushort   start_val[1] = {8};
static struct sembuf acquire = {0, -1, SEM_UNDO},
                     release = {0, 1, SEM_UNDO};
int generate_number(int lower, int upper);
int main(int argc, char *argv[]) {
    if (!strcmp(argv[1], "hall_process")){
        // hall semaphore
        union semun arg;
        int hall_sem = (semget(ftok(".", 'h'), 1, IPC_CREAT | IPC_EXCL | 0660));
        if (hall_sem == -1){
            perror("semget hall");
            exit(-4);
        }
        arg.array = start_val;
        if ( semctl(hall_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl initialization");
            exit(1);
        }
    } else if (!strcmp(argv[1], "male") || !strcmp(argv[1], "female")){
        string service_type[4] = {"Bx", "Ix", "Tx", "Rx"};
        string state_type[3] = {"Unserved", "Unhappy" , "Satisfied"};
        int service_num = generate_number(0, 3);
        int state_num = generate_number(0,2);
        /*
         * if state_num = 0 ---> Unserved
         * if state_num = 1 ---> Unhappy
         * if state_num = 2 ---> Satisfied
         *
         * if type_num = 0 ---> Bx
         * if type_num = 1 ---> Ix
         * if type_num = 2 ---> Tx
         * if type_num = 3 ---> Rx
         */
        if (state_num != 0){
            int sem = (semget(ftok(".", 'h'), 1, 0));
            if (sem == -1){
                perror("semget mail");
                exit(-2);
            }
            if ( semop(sem, &acquire, 1) == -1 ) {
                perror("semop2");
                exit(3);
            }
            cout << YELLOW << "process with id " << getpid() << " in the critical system, with gender " << argv[1] << " with service type "
            << service_type[service_num]  << " his states is " << state_type[state_num] << endl;
            fflush(stdout);
            sleep(1);

            if ( semop(sem, &release, 1) == -1 ) {
                perror("semop2");
                exit(3);
            }
        } else {
            cout << GREEN << "process with id " << getpid() << " in the critical system, with gender " << argv[1] << " with service type "
                             << service_type[service_num]  << " his states is " << state_type[state_num] << endl;
            fflush(stdout);
        }
    }

    return 0;
}

int generate_number(int lower, int upper){
    int s, numb;
    FILE *f;
    f = fopen("/dev/urandom", "rb");
    fread(&s, sizeof(int), 1, f);
    fclose(f);
    srand(s);
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}