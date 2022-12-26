#include "local.h"

const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;
int metal_gate_man,metal_gate_woman, rolling_gate_man, rolling_gate_woman;
int generate_waiting_time(int lower, int upper);

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

    type = argv[1];
    if (!strcmp(argv[1], "metal_gate_man")){
        sleep(1);
        metal_gate_man = getpid();
        rolling_gate_man = stoi(argv[2]);

        kill(rolling_gate_man, SIGUSR1);


    } else if (!strcmp(argv[1], "metal_gate_woman")){
        sleep(1);
        metal_gate_woman = getpid();
        rolling_gate_woman = stoi(argv[2]);

        kill(rolling_gate_woman, SIGUSR2);


    } else if (!strcmp(argv[1], "mail")){
        int wait = generate_waiting_time(1, 3);
        sleep(wait);

        kill(stoi(argv[2]), SIGUSR1);
        if (execl("./hall", "hall", "male", (char *)NULL) == -1){
            perror("Error in execlp the male people");
            exit(-2);
        }
    } else if (!strcmp(argv[1], "female")){
        int wait = generate_waiting_time(1, 5);
        sleep(wait);

        kill(stoi(argv[2]), SIGUSR2);
        if (execl("./hall", "hall", "female", (char *)NULL) == -1){
            perror("Error in execlp the female people");
            exit(-2);
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
