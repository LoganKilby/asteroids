/* date = May 11th 2021 2:59 am */

#ifndef UTILITY_H
#define UTILITY_H

struct file_buffer
{
    char* buffer;
    int size;
};

static int
LoadFile(file_buffer*, char*);

#endif //UTILITY_H
