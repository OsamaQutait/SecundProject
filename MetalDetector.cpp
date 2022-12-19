#include "local.h"

const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;

int generate_waiting_time(int lower, int upper);

int main(int argc, char *argv[]) {
    unordered_map<string, int> data;
    // read file
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
    // end reading
    type = argv[1];
    if (!strcmp(argv[1], "mail")){
        if (mail_queue.size() > data["queues_mail"]){

        } else {

        }
    } else if (!strcmp(argv[1], "female")){
        if (female_queue.size() > data["queues_mail"]){

        } else {

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