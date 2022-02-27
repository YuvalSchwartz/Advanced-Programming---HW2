
#ifndef DEFS_H_
#define DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 300

typedef enum {me, others} which_to_enter;

typedef enum {add_failure, remove_failure, memory_failure, print_failure, null_pointer_failure, success} status;

typedef struct Type Type;

typedef struct BiologicalInformation BiologicalInformation;

typedef struct Pokemon Pokemon;

#endif /* DEFS_H_ */
