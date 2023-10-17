#ifndef GOP_H
#define GOP_H

void setup_GOP();
void vprintf(const unsigned char* str, unsigned int color, unsigned int *start);
void vprintchar(unsigned char character, unsigned int color, unsigned int* start);
void clearscreen(unsigned int color);

#endif