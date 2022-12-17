#include "local.h"

void read_file(unordered_map<string, int> data);

int main() {
    unordered_map<string, int> data;
    read_file(data);
    int total_number_of_processes = data["Male"]+ data["Female"] + data["Bx"]
                                    + data["Ix"] + data["Tx"]+ data["Rx"];
    pid_t pid;

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
            data[word] = stoi(num);
            cout << endl << word << " " << stoi(num) << endl;
        }
        data_file.close();
    }
}
