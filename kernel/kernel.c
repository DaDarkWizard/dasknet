#include "../drivers/screen.h"

void doSomething()
{

    char* video_memory = (char*) 0xb8000;
    *video_memory = 'W';
}

void main() {

    char* video_memory = (char*) 0xb8000;
    //*video_memory = 'X';
    //*(video_memory + 3) = 'Y';
    //test_screen();
    clear_screen();
    print("Hello world!\n");
    print("Finally got my screen working!\nNow maybe I can move on to more important things...\n");
}

void setMonitorText(char* text)
{
    char* video_memory = (char*) 0xb8000;
    for(int i = 0; text[i] != 0; i++)
    {
        video_memory[i] = text[i];
    }
}