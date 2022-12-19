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
//    sleep(15); // for real code , for test I will put it 2 ,to minimize waiting time
    sleep(2);
    int wait = generate_waiting_time(1, 10);
    //his/her waiting time is grater pass at first to the queue
    sleep(11-wait);
    cout << "person with id : " << getpid() <<
    " reach the rolling gate with gender " << argv[1] << " with waiting time :"
    << wait  << " second" << endl;
    fflush(stdout);
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