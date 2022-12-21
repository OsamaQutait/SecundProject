#include "local.h"

pid_t rolling_gate_man, rolling_gate_woman;
const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;

int generate_waiting_time(int lower, int upper);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);

int main(int argc, char *argv[]) {
    int man_sem, woman_sem;
    key_t ipc_key1, ipc_key2;
    union semun arg;
    static ushort   start_val[1] = {1};
    static struct sembuf acquire = {0, -1, SEM_UNDO},
                         release = {0, 1, SEM_UNDO};
    type = argv[1];
    int wait;
    if (!strcmp(type, "rolling_gate_man")){
        rolling_gate_man = getpid();
        cout << BLUE << "rolling_gate_man id is : " << rolling_gate_man << endl;

        ipc_key1 = ftok(".", 'A');
        man_sem = (semget(ipc_key1, 1, IPC_CREAT | IPC_EXCL | 0660));
        if (man_sem == -1){
            perror("semget");
            exit(-4);
        }
        arg.array = start_val;
        if ( semctl(man_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl -- producer -- initialization");
            exit(1);
        }
//        signal(SIGUSR1, &handle_sigusr1);
//        while (1);
    }
    if (!strcmp(type, "rolling_gate_woman")){
        rolling_gate_woman = getpid();
        cout << BLUE << "rolling_gate_woman id is : " << rolling_gate_woman << endl;

        ipc_key2 = ftok(".", 'B');
        woman_sem = (semget(ipc_key2, 1, IPC_CREAT | IPC_EXCL | 0660));
        if (woman_sem == -1){
            perror("semget");
            exit(-4);
        }
        arg.array = start_val;
        if ( semctl(woman_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl initialization");
            exit(-1);
        }
//        signal(SIGUSR2, &handle_sigusr2);
//        while (1);
    }
    if (!strcmp(type, "female") || !strcmp(type, "mail")){
        wait = generate_waiting_time(1, 10);
        //his/her waiting time is grater pass at first to the queue
        sleep(11-wait);
    }

    if (!strcmp(type, "mail")){
        man_sem = (semget(ftok(".", 'A'), 1, 0));
        if (man_sem == -1){
            perror("semget mail");
            exit(-2);
        }
        acquire.sem_num = 0;
        if ( semop(man_sem, &acquire, 1) == -1 ) {
            perror("semop");
            exit(3);
        }
        cout << RED << "person with id : " << getpid() <<
             " reach the rolling gate with gender " << argv[1] << " with waiting time :"
             << wait  << " second" << endl;
        fflush(stdout);
        sleep(1);
        mail_queue.push(getpid());
        if (execl("./MetalDetector", "MetalDetector", "mail", (char *)NULL) == -1){
            perror("Error in execlp the mail people");
            exit(-2);
        }
        release.sem_num = 0;
        if ( semop(man_sem, &release, 1) == -1 ) {
            perror("semop");
            exit(3);
        }

    } else if (!strcmp(type, "female")){
        woman_sem = (semget(ftok(".", 'B'), 1, 0));
        if (man_sem == -1){
            perror("semget female");
            exit(-2);
        }
        acquire.sem_num = 0;
        if ( semop(woman_sem, &acquire, 1) == -1 ) {
            perror("semop");
            exit(3);
        }
        cout << YELLOW << "person with id : " << getpid() <<
             " reach the rolling gate with gender " << argv[1] << " with waiting time :"
             << wait  << " second" << endl;
        fflush(stdout);
        sleep(1);
        female_queue.push(getpid());
        if (execl("./MetalDetector", "MetalDetector", "female", (char *)NULL) == -1){
            perror("Error in execlp the female people");
            exit(-2);
        }
        release.sem_num = 0;
        if ( semop(woman_sem, &release, 1) == -1 ) {
            perror("semop");
            exit(3);
        }
    }
    return 0;
}

int generate_waiting_time(int lower, int upper){
    int s, numb;
    FILE *f;
    f = fopen("/dev/urandom", "rb");
    fread(&s, sizeof(int), 1, f);
    fclose(f);
    srand(s);
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void handle_sigusr1(int sig){
    kill(mail_queue.top(), SIGUSR1);
    if (!strcmp(type, "mail") && !mail_queue.empty()){
        mail_queue.pop();
        if (execl("./MetalDetector", "MetalDetector", "mail", (char *)NULL) == -1){
            perror("Error in execlp the mail people");
            exit(-2);
        }
    }
}

void handle_sigusr2(int sig){
    kill(female_queue.top(), SIGUSR2);
    if (!strcmp(type, "female") && !female_queue.empty()){
        female_queue.pop();
        if (execl("./MetalDetector", "MetalDetector", "female", (char *)NULL) == -1){
            perror("Error in execlp the female people");
            exit(-2);
        }
    }
}