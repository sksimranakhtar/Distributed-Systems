
/*
This is the file to write the integers.

void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);

mmap() creates a new mapping in the virtual address space of the calling process.
The starting address for the new mapping is specified in addr. 
The length argument specifies the length of the mapping
The prot argument describes the desired memory protection of the mapping.
The flags argument determines whether updates to the mapping are visible to other processes mapping the same region, 
and whether updates are carried through to the underlying file.
MAP_SHARED: Share this mapping

int munmap(void *addr, size_t length);
The munmap() system call deletes the mappings for the specified address range, 
and causes further references to addresses within the range to generate invalid memory references
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int main(int argc, char *argv[])
{
    int i;
    int fd;
    int result;
    int *map;  /* mmapped array of int's */

    /* Open a file for writing.
 *     Creating the file if it doesn't exist.
 *     Truncating it to 0 size if it already exists. (not really needed)
 *     Note: "O_WRONLY" mode is not sufficient when mmaping.  */
   
   fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
	perror("Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints */
    result = lseek(fd, FILESIZE-1, SEEK_SET);   //set the pointer to the end of the stretched file
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
	exit(EXIT_FAILURE);
    }
    
    /* Something needs to be written at the end of the file to
 *     have the file actually have the new size.
 *     Just writing an empty string at the current file position will do.
 *     Note:
 *      - The current position in the file is at the end of the stretched 
 *      file due to the call to lseek().
 *      -An empty string is actually a single '\0' character, so a zero-byte
 *       will be written at the last byte of the file.
 *  */
 
    result = write(fd, "", 1);
    if (result != 1) {
	close(fd);
	perror("Error writing last byte of the file");
	exit(EXIT_FAILURE);
    }

    /* Now the file is ready to be mmapped.
 *      */
    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //map fails
	if (map == MAP_FAILED) {
	close(fd);
	perror("Error mmapping the file");
	exit(EXIT_FAILURE);
    }
    
    /* Now write int's to the file as if it were memory (an array of ints).
 *      */

	//map success
    for (i = 1; i <=500; ++i) {
	map[i] = 0; 
    }
	int j=0;
	while(1)
	{   //get user inputs to write in the shared memory
		scanf("%d ",&map[j]);
		j++;
	}

    /* Don't forget to free the mmapped memory*/
    if (munmap(map, FILESIZE) == -1) {
	perror("Error un-mmapping the file");
    }

    /* Decide here whether to close(fd) and exit() or not. Depends... 
	Un-mmaping doesn't close the file, so we still need to do that.
    */
    close(fd);
    return 0;
}
