#include <bits/stdc++.h>
using namespace std;
int main(int argc,char *argv[])
{       
    string str[argc-1];
    string s="";
    for(int i=1;i<argc;i++)
    {
        str[i-1]=argv[i];
        s+=str[i-1];
        s+=" ";
    }
    string str3 = "g++ "+s+" -o a ";
    //string str2 = "-lglut -lGL -lGLU -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXcursor -lXext -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLEW -lGLU -lGL -lm -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp";
    string str2 = "-lassimp -lglut -lGL -lGLU -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXcursor -lXext -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLEW -lGLU -lGL -lm -ldl -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp";
    string str4 = str3+str2;
    const char *command = str4.c_str();
    system(command);
    return 0;
}
