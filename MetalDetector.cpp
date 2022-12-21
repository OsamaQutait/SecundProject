#include "local.h"

const char *type;
priority_queue<pid_t> mail_queue;
priority_queue<pid_t> female_queue;
int metal_gate_man,metal_gate_woman, rolling_gate_man, rolling_gate_woman;
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
    if (!strcmp(argv[1], "metal_gate_man")){
        metal_gate_man = getpid();
        rolling_gate_man = stoi(argv[2]);
        cout << GREEN << "rolling_gate_man id is : " << argv[2] << endl;
        fflush(stdout);
    } else if (!strcmp(argv[1], "metal_gate_woman")){
        metal_gate_woman = getpid();
        rolling_gate_woman = stoi(argv[2]);
        cout << GREEN << "rolling_gate_woman id is : " << argv[2] << endl;
        fflush(stdout);
    }
//    else {
//        cout << YELLOW << "person with id : " << getpid() <<
//             " reach the metal detector gate with gender " << argv[1] << endl;
//    }
//    while (1){
//        if (!strcmp(argv[1], "female")){
//            female_queue.push(getpid());
//            // added 2 because the female spending time more than mail
//            int wait = generate_waiting_time(1, 5) + 2;
//            sleep(wait);
//            cout << "process with id : " << getpid() << " spending " << wait << " second the gender is "<< argv[1] << endl;
//            female_queue.pop();
//            break;
//        }
//        if (!strcmp(argv[1], "mail")){
//            mail_queue.push(getpid());
//            int wait = generate_waiting_time(1, 5);
//            sleep(wait);
//            cout << "process with id : " << getpid() << " spending " << wait << " second the gender is "<< argv[1] << endl;
//            female_queue.pop();
//            break;
//        }
//        if (!strcmp(argv[1], "metal_gate_man") && mail_queue.size() <= 2){
//            kill(rolling_gate_man, SIGUSR1);
//        }
//        if (!strcmp(argv[1], "metal_gate_woman") && mail_queue.size() <= 2){
//            kill(rolling_gate_woman, SIGUSR2);
//        }
//    }


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