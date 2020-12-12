#include <algorithm>
#include <iostream>
#include <cstring>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
// Needed to use read - https://bytes.com/topic/c/answers/433804-test-c-24-error-read-undeclared-first-use-function
#include <unistd.h>

static uintmax_t wc(char const *fname);

int main()
{
    uintmax_t m_numLines = 0;
    m_numLines = wc("test.cpp");
    std::cout << "m_numLines = " << m_numLines << "\n";
}

void handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}

static uintmax_t wc(char const *fname)
{
    static const auto BUFFER_SIZE = 16*8;
    int fd = open(fname, O_RDONLY);
    if(fd == -1)
        handle_error("open");

    /* Advise the kernel of our access pattern.  */
    //posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

    char buf[BUFFER_SIZE + 1];
    uintmax_t lines = 0;

    while(size_t bytes_read = read(fd, buf, BUFFER_SIZE))
    {
        int i = 0;
        char* currentLine = new char[i];

        for(size_t index=0; index<bytes_read; index++)
        {
            if(buf[index]=='\n')
            {
                std::cout << currentLine;
                std::cout << "\n";
                i = 0;
                memset(currentLine, 0, sizeof(currentLine)); 
            }
            else
            {
                currentLine[i] = buf[index];
                i ++;
            }

            lines ++;
        }
    }
    return lines;
}