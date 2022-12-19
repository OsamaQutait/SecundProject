#include "local.h"

pid_t rolling_gate_man, rolling_gate_woman;
const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;

int generate_waiting_time(int lower, int upper);
void handle_sigusr1(int sig);
void handle_sigusr2(int sig);

int main(int argc, char *argv[]) {
    type = argv[1];
    if (!strcmp(type, "rolling_gate_man")){
        rolling_gate_man = getpid();
        signal(SIGUSR1, &handle_sigusr1);
        while (1);
    }
    if (!strcmp(type, "rolling_gate_woman")){
        rolling_gate_woman = getpid();
        signal(SIGUSR2, &handle_sigusr2);
        while (1);
    }
    sleep(2);
    int wait = generate_waiting_time(1, 10);
    //his/her waiting time is grater pass at first to the queue
    sleep(11-wait);
    cout << "person with id : " << getpid() <<
    " reach the rolling gate with gender " << argv[1] << " with waiting time :"
    << wait  << " second" << endl;
    fflush(stdout);
    if (!strcmp(type, "mail")){
        mail_queue.push(getpid());
    } else if (!strcmp(type, "female")){
        female_queue.push(getpid());
    }
    while (1);
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