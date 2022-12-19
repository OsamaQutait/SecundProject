#include "local.h"

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