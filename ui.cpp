//// g++ test.cpp -o test.cpp -lglut -lGLU -lGL -lm


#include "local.h"


void renderBitMap(char *string, void *font, float x, float y) {
    char *c;
    glRasterPos2i(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);

    }
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    ///BORDER


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    glOrtho(0, w, 0, h, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // important
    glTranslatef(0.5, 0.5, 0);

    float offset = 40;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0 + offset, 0 + offset);
    glVertex2f(0 + offset, h - offset);

    glVertex2f(w - offset, h - offset);
    glVertex2f(w - offset, 0 + offset);
    glEnd();

    glutSwapBuffers();

    ////EXIT////

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(1360, 100);
    glVertex2f(1360, 200);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(1360, 100);
    glVertex2f(1380, 100);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(1360, 200);
    glVertex2f(1380, 200);
    glEnd();
    glFlush();


    ///WALL BETWEEN THE HALL AND ENTRANCE
    //white
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(600, 660);
    glVertex2f(600, 480);
    glEnd();
    glFlush();

////////UPPER METAL DETECTOR///////
    //red
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(650, 410);
    glVertex2f(650, 430);
    glEnd();
    glFlush();

    //red
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(650, 480);
    glVertex2f(650, 460);
    glEnd();
    glFlush();
/////////////////////////////

///WALL BETWEEN THE 2 METAL DETECTORS
    //white
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(600, 410);
    glVertex2f(600, 290);
    glEnd();
    glFlush();
////////////////////

/////// LOWER METAL DETECTOR/////
    //red
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(650, 240);
    glVertex2f(650, 220);
    glEnd();
    glFlush();

    //red
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(650, 290);
    glVertex2f(650, 270);
    glEnd();
    glFlush();
///////////////////

    //white
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(600, 220);
    glVertex2f(600, 40);
    glEnd();
    glFlush();

    ///MAMARRAT FOR MALES AND FEMALES
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 480);
    glVertex2f(650, 480);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 410);
    glVertex2f(650, 410);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 290);
    glVertex2f(650, 290);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 220);
    glVertex2f(650, 220);
    glEnd();
    glFlush();

    ///WALLS BETWEEN THE TWO ROLLING GATES/////
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 410);
    glVertex2f(300, 290);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 40);
    glVertex2f(300, 220);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(300, 660);
    glVertex2f(300, 480);
    glEnd();
    glFlush();


    /////ROLLING GATES
    //green
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex2f(300, 480);
    glVertex2f(300, 410);
    glEnd();
    glFlush();

    //green
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex2f(300, 290);
    glVertex2f(300, 220);
    glEnd();
    glFlush();
////////////////

    // read file
    unordered_map<string, int> data;
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
    data["Male"] = (int)((double)data["total_number_of_people"]*((double)data["Male"]/(double)100));
    data["Female"] = (int)((double)data["total_number_of_people"]*((double)data["Female"]/(double)100));

    // end reading

    ///texts
    glColor3f(1, 1, 1);
    char title[100];

    char FemaleStringNum[256];
    char MaleStringNum[256];

    char PeopleStringNum[256];



    int FemaleNumber = data["Female"];
    int MaleNumber = data["Male"];

    int PeopleNum = data["total_number_of_people"];



    sprintf(title, "OIM Simulation");
    renderBitMap(title, GLUT_BITMAP_HELVETICA_18, 700, 672);
    sprintf(FemaleStringNum, "Female Number: %d", FemaleNumber);
    renderBitMap(FemaleStringNum, GLUT_BITMAP_HELVETICA_18, 320, 440);
    sprintf(MaleStringNum, "Male Number: %d", MaleNumber);
    renderBitMap(MaleStringNum, GLUT_BITMAP_HELVETICA_18, 320, 250);

    sprintf(PeopleStringNum, "People waiting to enter: %d", PeopleNum);
    renderBitMap(PeopleStringNum, GLUT_BITMAP_HELVETICA_18, 60, 345);
////////////////////
    sprintf(title, "travel document");
    renderBitMap(title, GLUT_BITMAP_HELVETICA_18, 620, 610);

    sprintf(title, "birth certificate");
    renderBitMap(title, GLUT_BITMAP_HELVETICA_18, 810, 610);

    sprintf(title, "ID-related");
    renderBitMap(title, GLUT_BITMAP_HELVETICA_18, 1020, 610);

    sprintf(title, "family reunion");
    renderBitMap(title, GLUT_BITMAP_HELVETICA_18, 1200, 610);
///////////////////



    ///////////////////
    glEnd();
    glFlush();



    /////////shababeek///////
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(600, 600);
    glVertex2f(1360, 600);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(600, 500);
    glVertex2f(1360, 500);
    glEnd();
    glFlush();

    /////LINES BETWEEN SHABABEEK/////
    for (int i = 600; i <= 1360; i += 190) {

        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(i, 600);
        glVertex2f(i, 500);
        glEnd();
        glFlush();
    }


///////TX 1//////


    glBegin(GL_POLYGON);

    glColor3f(1, 1, 0);
    glVertex2f(625, 580);
    glVertex2f(625 + 40, 580);
    glVertex2f(625 + 40, 540);
    glVertex2f(625, 540);
    glEnd();
    glFlush();

    ///////TX 2//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(725, 580);
    glVertex2f(725 + 40, 580);
    glVertex2f(725 + 40, 540);
    glVertex2f(725, 540);
    glEnd();
    glFlush();


    ///////BX 1//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(815, 580);
    glVertex2f(815 + 40, 580);
    glVertex2f(815 + 40, 540);
    glVertex2f(815, 540);
    glEnd();
    glFlush();

    ///////BX 2//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(915, 580);
    glVertex2f(915 + 40, 580);
    glVertex2f(915 + 40, 540);
    glVertex2f(915, 540);
    glEnd();
    glFlush();

    ///////IX 1//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(1005, 580);
    glVertex2f(1005 + 40, 580);
    glVertex2f(1005 + 40, 540);
    glVertex2f(1005, 540);
    glEnd();
    glFlush();

    ///////IX 2//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(1105, 580);
    glVertex2f(1105 + 40, 580);
    glVertex2f(1105 + 40, 540);
    glVertex2f(1105, 540);
    glEnd();
    glFlush();

    ///////RX 1//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(1195, 580);
    glVertex2f(1195 + 40, 580);
    glVertex2f(1195 + 40, 540);
    glVertex2f(1195, 540);
    glEnd();
    glFlush();


    ///////RX 2//////
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    glVertex2f(1295, 580);
    glVertex2f(1295 + 40, 580);
    glVertex2f(1295 + 40, 540);
    glVertex2f(1295, 540);
    glEnd();
    glFlush();

    //////////CHAIRS//////
    for (int i = 700; i <= 1250; i += 50) {
        glColor3f(0, 1, 0);

        glBegin(GL_POLYGON);
        glVertex2f(i, 220);
        glVertex2f(i + 20, 220);
        glVertex2f(i + 20, 200);
        glVertex2f(i, 200);
        glEnd();
        glFlush();

    }

    for (int i = 700; i <= 1250; i += 50) {
        glColor3f(0, 1, 0);

        glBegin(GL_POLYGON);
        glVertex2f(i, 180);
        glVertex2f(i + 20, 180);
        glVertex2f(i + 20, 160);
        glVertex2f(i, 160);
        glEnd();
        glFlush();

    }

}

void sim() {
    int shmID = shmget(ftok(".", 'o') , sizeof(int)*3, 0666 | IPC_CREAT);
    if (shmID == -1){
        perror("error in generate the shared memory");
        exit(-1);
    }
    int *shm = (int *)shmat(shmID, NULL, 0);

    char UnservedStringNum[256];
    char UnhappyStringNum[256];
    char satisfiedStringNum[256];

    int unservednum = shm[0];
    int Unhappynum = shm[1];
    int satisfiedNum = shm[2];

    sprintf(UnservedStringNum, "Unserved People: %d", unservednum);
    renderBitMap(UnservedStringNum, GLUT_BITMAP_HELVETICA_18, 1150, 410);


    sprintf(UnhappyStringNum, "Unhappy People: %d", Unhappynum);
    renderBitMap(UnhappyStringNum, GLUT_BITMAP_HELVETICA_18, 1150, 380);

    sprintf(satisfiedStringNum, "Satisfied People: %d", satisfiedNum);
    renderBitMap(satisfiedStringNum, GLUT_BITMAP_HELVETICA_18, 1150, 350);
    glEnd();
    glFlush();

}



void display() {
    draw();
    while(1) {
        sim();
        sleep(1);
        glClear(GL_COLOR_BUFFER_BIT);
        draw();

    }
}
int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(150, 250);
    glutInitWindowSize(1400, 700);
    glutCreateWindow("WELCOME TO OUR PROGRAM");
    glutDisplayFunc(display);
    glutMainLoop();
}