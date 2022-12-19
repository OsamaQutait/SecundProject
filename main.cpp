#include "local.h"

int main(int argc, char *argv[]) {
    int status = 0;
    pid_t pid, wpid, rolling_gate_man, rolling_gate_woman, metal_gate_man, metal_gate_woman;
    vector<pid_t> pid_array;
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
    //generate the rolling gate man and woman
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == -1 ){
            perror("Error in for the mail people");
            exit(-1);
        } else if (pid == 0 && i == 0) {
            rolling_gate_man = getpid();
            if (execl("./RollingGates", "RollingGates", "rolling_gate_man", (char *)NULL) == -1) {
                perror("Error in execlp the rolling gate man");
                exit(-2);
            }
        } else if (pid == 0 && i == 1){
            rolling_gate_woman = getpid();
            if (execl("./RollingGates", "RollingGates", "rolling_gate_woman", (char *)NULL) == -1) {
                perror("Error in execlp the rolling gate woman");
                exit(-2);
            }
        }
    }
    //generate the metal gate man and woman
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == -1 ){
            perror("Error in fork the metal gate man and woman");
            exit(-1);
        } else if (pid == 0 && i == 0) {
            metal_gate_man = getpid();
            char rolling_gate_man_s[10];
            sprintf(rolling_gate_man_s, "%d", rolling_gate_man);
            if (execl("./MetalDetector", "MetalDetector", "metal_gate_man", rolling_gate_man_s,(char *)NULL) == -1) {
                perror("Error in execlp the rolling gate man");
                exit(-2);
            }
        } else if (pid == 0 && i == 1){
            metal_gate_woman = getpid();
            char rolling_gate_woman_s[10];
            sprintf(rolling_gate_woman_s, "%d", rolling_gate_woman);
            if (execl("./MetalDetector", "MetalDetector", "metal_gate_woman", rolling_gate_woman_s, (char *)NULL) == -1) {
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
            if (execl("./RollingGates", "RollingGates", "mail", (char *)NULL) == -1){
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
            if (execl("./RollingGates", "RollingGates", "female", (char *)NULL) == -1){
                perror("Error in execlp the female people");
                exit(-2);
            }
        } else {
            pid_array.push_back(pid);
        }
    }
    //the father waits for all the child processes
    while ((wpid = wait(&status)) > 0);
    return 0;
}

