#include "local.h"

pid_t pid, wpid, rolling_gate_man, rolling_gate_woman, metal_gate_man, metal_gate_woman, hall_process;
vector<pid_t> pid_array;
void handle_sigusr1(int sig);

int main(int argc, char *argv[]) {
    signal(SIGUSR1, &handle_sigusr1);
    int status = 0;
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
    char mail[3], femail[3], bx[3], ix[3], tx[3],
         rx[3], unserved[3], unhappy[3], satisfied[3];

    sprintf(mail, "%d", data["Male"]);
    sprintf(femail, "%d", data["Female"]);
    sprintf(bx, "%d", data["Bx"]);
    sprintf(ix, "%d", data["Ix"]);
    sprintf(tx, "%d", data["Tx"]);
    sprintf(rx, "%d", data["Rx"]);
    sprintf(unserved, "%d", data["Unserved"]);
    sprintf(unhappy, "%d", data["Unhappy"]);
    sprintf(satisfied, "%d", data["Satisfied"]);

    // generate process for the ui
    pid = fork();
    if (pid == -1){
        perror("Error in for the hall");
        exit(-1);
    } else if (pid == 0){
        if (execl("./ui", "ui", (char *)NULL) == -1){
            perror("Error in execlp the ui process");
            exit(-2);
        }
    } else {
        pid_array.push_back(pid);
    }

    // generate process for the hall
    pid = fork();
    if (pid == -1){
        perror("Error in for the hall");
        exit(-1);
    } else if (pid == 0){
        if (execl("./hall", "hall", "hall_process", (char *)NULL) == -1){
            perror("Error in execlp the mail people");
            exit(-2);
        }
    } else {
        hall_process = pid;
        pid_array.push_back(pid);
    }
    //generate the rolling gate man and woman
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == -1 ){
            perror("Error in for the mail people");
            exit(-1);
        } else if (pid == 0 && i == 0) {
            if (execl("./RollingGates", "RollingGates", "rolling_gate_man", (char *)NULL) == -1) {
                perror("Error in execlp the rolling gate man");
                exit(-2);
            }
        } else if (pid == 0 && i == 1){
            if (execl("./RollingGates", "RollingGates", "rolling_gate_woman", (char *)NULL) == -1) {
                perror("Error in execlp the rolling gate woman");
                exit(-2);
            }
        } else if (pid != 0 && i == 0){
            rolling_gate_man = pid;
            pid_array.push_back(pid);
            cout << MAGENTA << "rolling_gate_man ID is : " << rolling_gate_man << endl;
            fflush(stdout);

        } else if (pid != 0 && i == 1){
            rolling_gate_woman = pid;
            pid_array.push_back(pid);
            cout << MAGENTA << "rolling_gate_woman ID is : " << rolling_gate_woman << endl;
            fflush(stdout);

        }
    }
    sleep(1);
    //generate the metal gate man and woman
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == -1 ){
            perror("Error in fork the metal gate man and woman");
            exit(-1);
        } else if (pid == 0 && i == 0) {
            metal_gate_man = getpid();
            pid_array.push_back(pid);
            cout << MAGENTA << "metal_gate_man ID is : " << metal_gate_man << endl;
            fflush(stdout);
            string ss = to_string(int(rolling_gate_man));
            char rolling_gate_man_s[ss.length()+1];
            strcpy(rolling_gate_man_s, ss.c_str());
            fflush(stdout);
            if (execl("./MetalDetector", "MetalDetector", "metal_gate_man", rolling_gate_man_s,(char *)NULL) == -1) {
                perror("Error in execlp the rolling gate man");
                exit(-2);
            }
        } else if (pid == 0 && i == 1){
            metal_gate_woman = getpid();
            pid_array.push_back(pid);
            cout << MAGENTA << "metal_gate_woman ID is : " << metal_gate_woman << endl;
            fflush(stdout);
            string ss = to_string(int(rolling_gate_woman));
            char rolling_gate_woman_s[ss.length()+1];
            strcpy(rolling_gate_woman_s, ss.c_str());
            fflush(stdout);if (execl("./MetalDetector", "MetalDetector", "metal_gate_woman", rolling_gate_woman_s, (char *)NULL) == -1) {
                perror("Error in execlp the rolling gate woman");
                exit(-2);
            }
        }
    }
    //generate the mail people
    for (int i = 0; i < data["Male"]; ++i) {
        pid = fork();
        if (pid == -1){
            perror("Error in for the mail people");
            exit(-1);
        } else if (pid == 0){
            string ss = to_string(int(rolling_gate_man));
            char rolling_gate_man_s[ss.length()+1];
            strcpy(rolling_gate_man_s, ss.c_str());
            if (execl("./RollingGates", "RollingGates", "mail", rolling_gate_man_s, (char *)NULL) == -1){
                perror("Error in execlp the mail people");
                exit(-2);
            }
        } else {
            pid_array.push_back(pid);
        }
    }
    //generate the female people
    for (int i = 0; i < data["Female"]; ++i) {
        pid = fork();
        if (pid == -1){
            perror("Error in for the Female people");
            exit(-1);
        } else if (pid == 0){
            string ss = to_string(int(rolling_gate_woman));
            char rolling_gate_woman_s[ss.length()+1];
            strcpy(rolling_gate_woman_s, ss.c_str());
            if (execl("./RollingGates", "RollingGates", "female", rolling_gate_woman_s,  (char *)NULL) == -1){
                perror("Error in execlp the female people");
                exit(-2);
            }
        } else {
//            sleep(1);
            pid_array.push_back(pid);
        }
    }

    //the father waits for all the child processes
    while ((wpid = wait(&status)) > 0);

    // delete the semaphore generated by childes
    int woman_sem = (semget(ftok(".", 'B'), 1, 0));
    if (semctl(woman_sem, 0, IPC_RMID, 0) == -1){
        perror("IPC_RMID faild");
    }

    int man_sem = (semget(ftok(".", 'A'), 1, 0));
    if (semctl(man_sem, 0, IPC_RMID, 0) == -1){
        cout << "man_sem " << man_sem << " mail_counter" << endl;
        perror("IPC_RMID faild");
    }

    int hall_sem = (semget(ftok(".", 'h'), 1, 0));
    if (semctl(hall_sem, 0, IPC_RMID, 0) == -1){
        cout << "man_sem " << hall_sem << " mail_counter" << endl;
        perror("IPC_RMID faild");
    }

    int write_sem = (semget(ftok(".", 'x'), 1, 0));
    if (semctl(write_sem, 0, IPC_RMID, 0) == -1){
        cout << "man_sem " << write_sem << " mail_counter" << endl;
        perror("IPC_RMID faild");
    }
    return 0;
}

void handle_sigusr1(int sig){
    for (int i = 0; i < pid_array.size(); i++){
        kill(pid_array[i], SIGINT);
    }
}