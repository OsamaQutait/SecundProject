#include "local.h"

int generate_waiting_time(int lower, int upper);

int main(int argc, char *argv[]) {
    int wait = generate_waiting_time(1, 5);
    sleep(wait);
    cout << "person with id : " << getpid() <<
    " reach the rolling gate with gender " << argv[1] << " with waiting time :"
    << wait << " second" << endl;
    fflush(stdout);
//    kill(getpid(), SIGKILL);
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