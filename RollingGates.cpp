#include "local.h"

pid_t rolling_gate_man, rolling_gate_woman;
const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;
int generate_waiting_time(int lower, int upper);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);
void handle_sigusr3(int sig);
void handle_sigusr4(int sig);
const char * rolling_gate_man_id;
const char* rolling_gate_woman_id;
struct message_buffer{
    pid_t pid;
};
int man_sem, woman_sem;
int man_msd_id, woman_msg_id;

int main(int argc, char *argv[]) {

    union semun arg;
    static ushort   start_val[1] = {1};
    static struct sembuf acquire = {0, -1, SEM_UNDO},
            release = {0, 1, SEM_UNDO};

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


    type = argv[1];
    int wait;
    if (!strcmp(argv[1], "rolling_gate_man")){
        rolling_gate_man = getpid();
        cout << BLUE << "rolling_gate_man id is : " << rolling_gate_man << endl;
        // semaphore
        man_sem = (semget(ftok(".", 'A'), 1, IPC_CREAT | IPC_EXCL | 0660));
        if (man_sem == -1){
            perror("semget osama");
            exit(-4);
        }
        arg.array = start_val;
        if ( semctl(man_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl initialization");
            exit(1);
        }
        // message queue
        man_msd_id = msgget(ftok(".", 'S'), 0666|IPC_CREAT);
        if (man_msd_id == -1){
            perror("Error in message create");
            exit(-1);
        }
        while (1){
            struct message_buffer message;
            int receiv_val = msgrcv(man_msd_id, &message, sizeof(message.pid), 0, !IPC_NOWAIT);
            mail_queue.push(message.pid);
            if (mail_queue.size() == data["Male"]){
                msgctl(man_msd_id, IPC_RMID, NULL);
                break;
            }
            if (receiv_val == -1 ){
                perror("Error in receive message1");
                break;
            }
        }
        signal(SIGUSR1, &handle_sigusr1);
        while (1){
            pause();
        }

    }

    if (!strcmp(type, "rolling_gate_woman")){
        sleep(1);
        rolling_gate_woman = getpid();
        cout << BLUE << "rolling_gate_woman id is : " << rolling_gate_woman << endl;
        // semaphore
        woman_sem = semget(ftok(".", 'B'), 1, IPC_CREAT | IPC_EXCL | 0660);
        if (woman_sem == -1){
            perror("semget");
            exit(-4);
        }

        arg.array = start_val;
        if ( semctl(woman_sem, 0, SETALL, arg) == -1 ) {
            perror("semctl initialization");
            exit(-1);
        }
        // message queue
        woman_msg_id = msgget(ftok(".", 'P'), 0666|IPC_CREAT);
        if (woman_msg_id == -1){
            perror("Error in message create");
            exit(-1);
        }
        while (1){
            struct message_buffer message2;
            int receiv_val = msgrcv(woman_msg_id, &message2, sizeof(message2.pid), 0, !IPC_NOWAIT);
            female_queue.push(message2.pid);
            if (female_queue.size() == data["Female"]){
                msgctl(woman_msg_id, IPC_RMID, NULL);
                break;
            }
            if (receiv_val == -1 ){
                perror("Error in receive message");
                break;
            }
        }

        signal(SIGUSR2, &handle_sigusr3);
        while (1){
            pause();
        }
    }

    if (!strcmp(type, "mail")){
        rolling_gate_man_id = argv[2];
//        wait = 2;
        // connect to semaphore
        man_sem = (semget(ftok(".", 'A'), 1, 0));
        if (man_sem == -1){
            perror("semget mail");
            exit(-2);
        }
//        sleep(generate_waiting_time(1, 3));
        if ( semop(man_sem, &acquire, 1) == -1 ) {
            perror("semop1");
            exit(3);
        }

        // send a message
        int msg_id = msgget(ftok(".", 'S'), 0666|IPC_CREAT);
        if (msg_id == -1){
            perror("Error in message connect");
            exit(-1);
        }
        struct message_buffer message;
        message.pid = getpid();
        int send_val = msgsnd(msg_id, &message, sizeof(message.pid), !IPC_NOWAIT);
        if (send_val == -1){
            perror("Error in send mail message");
            exit(-2);
        }
        if ( semop(man_sem, &release, 1) == -1 ) {
            perror("semop2");
            exit(3);
        }
        signal(SIGUSR1, &handle_sigusr2);
        pause();

    } else if (!strcmp(type, "female")){
        rolling_gate_woman_id = argv[2];
        //semaphore
        woman_sem = (semget(ftok(".", 'B'), 1, 0));
        if (woman_sem == -1){
            perror("semget female");
            exit(-2);
        }
//        sleep(generate_waiting_time(1, 3));
        if ( semop(woman_sem, &acquire, 1) == -1 ) {
            perror("semop3");
            exit(3);
        }
        // send message
        int msg_id1 = msgget(ftok(".", 'P'), 0666|IPC_CREAT);
        if (msg_id1 == -1){
            perror("Error in message connect");
            exit(-1);
        }
        struct message_buffer message1;
        message1.pid = getpid();
        int send_val1 = msgsnd(msg_id1, &message1, sizeof(message1.pid), !IPC_NOWAIT);
        if (send_val1 == -1){
            perror("Error in send female message");
            exit(-2);
        }

        if ( semop(woman_sem, &release, 1) == -1 ) {
            perror("semop4");
            exit(3);
        }
        signal(SIGUSR2, &handle_sigusr4);
        pause();
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
    if (mail_queue.empty()){
        kill(rolling_gate_man, SIGINT);
    } else {
        kill(mail_queue.top(), SIGUSR1);
        mail_queue.pop();
    }

}

void handle_sigusr2(int sig){
    if (execl("./MetalDetector", "MetalDetector", "mail", rolling_gate_man_id, (char *)NULL) == -1){
        perror("Error in execlp the mail people");
        exit(-2);
    }
}

void handle_sigusr3(int sig){
    if (female_queue.empty()){
        kill(rolling_gate_woman, SIGINT);
    }else {
        kill(female_queue.top(), SIGUSR2);
        female_queue.pop();
    }
}

void handle_sigusr4(int sig){
    if (execl("./MetalDetector", "MetalDetector", "female", rolling_gate_woman_id, (char *)NULL) == -1){
        perror("Error in execlp the female people");
        exit(-2);
    }
}