#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void errorCheck(int statusCode) {
  if (statusCode == -1) {
    perror("Error: \n");
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  int runLength = atoi(argv[3]);
  char *stringRemainder;
  long mode = strtol(argv[4], &stringRemainder, 0);
  int file = open(argv[1], O_RDONLY);
  int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
  char pattern[runLength];
  char value[runLength];
  int count, statusCode;

  //Error check-------------------------------
  if (argc != 5) {
    printf("There needs to be 4 inputs.\n Usage: rle <input file> <output file> <compression length> <mode>\n");
    exit(1);
  }

  if (runLength < 1) {
    printf("Compression length needs to greater or equal to 1.\n");
    exit(1);
  }

  if ((mode != 0) && (mode != 1)) {
    printf("Mode needs to be 0 or 1.\n");
    exit(1);
  }

  errorCheck(file);
  errorCheck(output);

  //End of error-check-------------------------

  //compress-----------------------------------
  if (mode == 0) {
    count = 1;
    statusCode = read(file, &pattern, runLength);
    errorCheck(statusCode);
    //Check if input file is empty
    if (statusCode == 0) {
      printf("Input file is empty.\n");
      exit(1);
    }
    while ((statusCode = read(file, &value, runLength)) != 0) {
      if (strncmp(pattern, value, runLength) == 0) {
        count++;
        if (count == 255) {
          statusCode = write(output, &count, 1);
          errorCheck(statusCode);
          statusCode = write(output, &pattern, runLength);
          errorCheck(statusCode);
          count = 0;
        }
      }     
      else {
        statusCode = write(output, &count, 1);
        errorCheck(statusCode);
        statusCode = write(output, &pattern, runLength);
        errorCheck(statusCode);
        count = 1;
        memcpy(pattern, value, runLength);
      }
    }
    errorCheck(statusCode);
    statusCode = write(output, &count, 1);
    errorCheck(statusCode);
    statusCode = write(output, &pattern, 1);
    errorCheck(statusCode);
  }

  //End of compress----------------------------

  //decompress---------------------------------
  else {
    char newLine[1] = "\n";
    while ((statusCode = read(file, &count, 1)) != 0) {
      statusCode = read(file, &value, runLength);
      errorCheck(statusCode);
      if (strncmp(&value[0], newLine, 1) == 0) {
        statusCode = write(output, &value, 1);
        errorCheck(statusCode);
      }
      else {
        for (int i = 1; i <= count; i++) { 
          statusCode = write(output, &value, runLength);
          errorCheck(statusCode);
        }
      }
    }
    errorCheck(statusCode);
  }
  
  //End of decompress---------------------------

  statusCode = close(file);
  errorCheck(statusCode);
  statusCode = close(output);
  errorCheck(statusCode);

  return 0;
}
