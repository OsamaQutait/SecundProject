#include "local.h"
static ushort   start_val[1] = {8};
static ushort   start_val1[1] = {1};
static struct sembuf acquire = {0, -1, SEM_UNDO},
                     release = {0, 1, SEM_UNDO};

static struct sembuf acquire1 = {0, -1, SEM_UNDO},
                     release1 = {0, 1, SEM_UNDO};
int generate_number(int lower, int upper);


int main(int argc, char *argv[]) {

    // read file
    unordered_map<string, int> data;
    string line, word, num;
    fstream data_file;
    data_file.open("inputData.txt", ios::in);
    if (!data_file){
        perror("File not created!");
        exit(-1);
    } else {
        while (getline(data_file, line)){
            stringstream ss(line);
            ss >> word;
            ss >> num;
            data[word] = stoi(num); // casting to integer
        }
        data_file.close();
    }
    data["Male"] = (int)((double)data["total_number_of_people"]*((double)data["Male"]/(double)100));
    data["Female"] = (int)((double)data["total_number_of_people"]*((double)data["Female"]/(double)100));
    // end reading

    if (!strcmp(argv[1], "hall_process")){
        // hall semaphore
        union semun arg;
        int hall_sem = (semget(ftok(".", 'h'), 1, IPC_CREAT | IPC_EXCL | 0660));
        if (hall_sem == -1){
            perror("semget hall (error in hall semaphore)");
            exit(-1);
        }
        arg.array = start_val;
        if ( semctl(hall_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl initialization (error in initialization the hall semaphore)");
            exit(-1);
        }
        // write in shared memory semaphore
        union semun arg1;
        int hall_sem1 = (semget(ftok(".", 'x'), 1, IPC_CREAT | IPC_EXCL | 0660));
        if (hall_sem1 == -1){
            perror("semget write (error in write semaphore)");
            exit(-1);
        }
        arg1.array = start_val1;
        if ( semctl(hall_sem1, 0, SETALL, arg1) == -1 ) {
            perror("semctl initialization (error in initialization the write semaphore)");
            exit(1);
        }

        // shared memory
        int shmID = shmget(ftok(".", 'o') , sizeof(int)*3, 0666 | IPC_CREAT);
        if (shmID == -1){
            perror("error in generate the shared memory");
            exit(-1);
        }
        int *shm = (int *)shmat(shmID, NULL, 0);
        shm[0] = 0;
        shm[1] = 0;
        shm[2] = 0;

    } else if (!strcmp(argv[1], "male") || !strcmp(argv[1], "female")){

        int shmID = shmget(ftok(".", 'o') , sizeof(int )*3, 0666 | IPC_CREAT);
        if (shmID == -1){
            perror("error in generate the shared memory");
            exit(-1);
        }
        int *shm = (int *)shmat(shmID, NULL, 0);

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

            // hall semaphore
            int sem = (semget(ftok(".", 'h'), 1, 0));
            if (sem == -1){
                perror("error in connect to the semaphore hale");
                exit(-1);
            }
            // write semaphore
            int sem1 = (semget(ftok(".", 'x'), 1, 0));
            if (sem1 == -1){
                perror("error in connect to the write semaphore");
                exit(-1);
            }
            if ( semop(sem, &acquire, 1) == -1 ) {
                perror("error in acquire the hall semaphore");
                exit(-1);
            }
            if ( semop(sem1, &acquire1, 1) == -1 ) {
                perror("error in acquire the write semaphore");
                exit(-1);
            }
            if (state_type[state_num] == "Unserved"){
                shm[0]++;
            } else if (state_type[state_num] == "Unhappy") {
                shm[1]++;
            } else if (state_type[state_num] == "Satisfied") {
                shm[2]++;
            }

            cout << YELLOW << "process with id " << getpid() << " in the critical section, with gender " << argv[1] << " with service type "
            << service_type[service_num]  << " his states is " << state_type[state_num] << endl
                    << "Unserved " << shm[0] << " Unhappy " << shm[1] << " Satisfied " << shm[2] << endl;
            fflush(stdout);

            if (shm[0] == data["Unserved"] || shm[1] == data["Unhappy"] || shm[2] == data["Satisfied"]){
                cout << "one of the condition happen so the program will terminate " << endl;
                kill(getppid(), SIGUSR1);

            }

            if (shm[0] + shm[1] + shm[2] == data["total_number_of_people"]){
                kill(getppid(), SIGUSR1);

            }

            if ( semop(sem1, &release1, 1) == -1 ) {
                perror("error in release the write semaphore");
                exit(-1);
            }
            sleep(1);

            if ( semop(sem, &release, 1) == -1 ) {
                perror("error in release the hall semaphore");
                exit(-1);
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