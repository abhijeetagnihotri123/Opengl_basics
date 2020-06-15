#include <bits/stdc++.h>
using namespace std;
int main(int argc,char *argv[])
{   
    string str1 = argv[1];
    string str3 = "g++ "+str1+" -o a ";
    string str2 = "-lglut -lGL -lGLU -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXcursor -lXext -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLEW -lGLU -lGL -lm -ldl -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp";
    string str4 = str3+str2;
    const char *command = str4.c_str();
    system(command);
    return 0;
}