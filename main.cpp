#include "local.h"

int main(int argc, char *argv[]) {
    int status = 0;
    pid_t pid, wpid;
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

