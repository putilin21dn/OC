#ifndef MAPPED_FILE_HPP
#define MAPPED_FILE_HPP

// constants
#define _MAPPED_SIZE 8192
#define _SHM_OPEN_MODE S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH   
#define _BUFFER_NAME "buffer"
#define _MSG_SEP '#'

std:: string on = "ON";
std:: string to = "TO";

struct MappedFile {
    int fd;
    char *data;
};

#endif