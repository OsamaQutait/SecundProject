#include "local.h"

int generate_waiting_time(int lower, int upper);

int main(int argc, char *argv[]) {
//    sleep(15); // for real code , for test I will put it 2 ,to minimize waiting time
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