#include "local.h"

void read_file(unordered_map<string, int> data);

int main(int argc, char *argv[]) {
    vector<pid_t> pid_array;
    unordered_map<string, int> data;
    read_file(data);
    int total_number_of_processes = data["Male"]+ data["Female"] + data["Bx"]
                                    + data["Ix"] + data["Tx"]+ data["Rx"];
    pid_t pid = fork();
    //generate the mail people
    for (int i = 0; i < data["Male"]; ++i) {
        if (pid == -1){
            perror("Error in for the mail people");
            exit(-1);
        } else if (pid == 0){
            if (execlp("./RollingGates", "RollingGates", "mail", (char *)NULL) == -1){
                perror("Error in execlp the mail people");
                exit(-2);
            }
        } else {
            pid_array.push_back(pid);
        }
    }
    //generate the female people
    for (int i = 0; i < data["Female"]; ++i) {
        if (pid == -1){
            perror("Error in for the Female people");
            exit(-1);
        } else if (pid == 0){
            if (execlp("./RollingGates", "RollingGates", "female", (char *)NULL) == -1){
                perror("Error in execlp the female people");
                exit(-2);
            }
        } else {
            pid_array.push_back(pid);
        }
    }
    sleep(1);

    return 0;
}

void read_file(unordered_map<string, int> data){
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
            cout << word << num << endl;
        }
        data_file.close();
    }
}
