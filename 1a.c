#include <stdio.h> 
#include <unistd.h> 
#include <limits.h>
int main() {

printf("No. of clock ticks: %ld\n", sysconf(_SC_CLK_TCK));

printf("Max. no. of child processes: %ld\n", sysconf(_SC_CHILD_MAX));

printf("Max. path length: %ld\n", pathconf("/", _PC_PATH_MAX));

printf("Max. no. of characters in a file name: %ld\n", pathconf("/", _PC_NAME_MAX));

printf("Max. no. of open files/process: %ld\n", sysconf(_SC_OPEN_MAX));

return 0; 
}