//
// Created by osama on 12/17/22.
//
#ifndef SECUNDPROJECT_LOCAL_H
#define SECUNDPROJECT_LOCAL_H
#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <cstdlib>
#include <queue>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <sys/ipc.h>
#include <sys/shm.h>
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */

#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLACK   "\033[30m"      /* Black */
#define MAGENTA "\033[35m"      /* Magenta */
union semun {
    int              val;
    struct semid_ds *buf;
    ushort          *array;
};
#endif //SECUNDPROJECT_LOCAL_H