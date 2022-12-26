//
// Created by osama on 12/26/22.
//
#include "local.h"
unordered_map <string, int> data;

int main(){
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

    cout << "data[\"Male\"] " << data["Male"] << endl;
    cout << "data[\"Female\"] " << data["Female"] << endl;


    data["Male"] = (int)((double)data["total_number_of_people"]*((double)data["Male"]/(double)100));
    data["Female"] = (int)((double)data["total_number_of_people"]*((double)data["Female"]/(double)100));
    // end reading

    cout << "data[\"Male\"] " << data["Male"] << endl;
    cout << "data[\"Female\"] " << data["Female"]<< endl;
//    cout << double ((double)40/(double)100 )<< endl;
    return 0;
}