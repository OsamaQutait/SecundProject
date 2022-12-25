#include "local.h"
static ushort   start_val[1] = {8};
static struct sembuf acquire = {0, -1, SEM_UNDO},
                     release = {0, 1, SEM_UNDO};
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
        int sem = (semget(ftok(".", 'h'), 1, 0));
        if (sem == -1){
            perror("semget mail");
            exit(-2);
        }
        if ( semop(sem, &acquire, 1) == -1 ) {
            perror("semop2");
            exit(3);
        }
        cout << YELLOW << "process with id " << getpid() << " in the critical system, with gender " << argv[1] << endl;
        sleep(1);

        if ( semop(sem, &release, 1) == -1 ) {
            perror("semop2");
            exit(3);
        }

    }

    return 0;
}