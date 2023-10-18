#ifndef TYPES_H
#define TYPES_H

#define NULL (void*)0
#define IFNULL(x) if(x == NULL)
#define IFREAL(x) if(x != NULL)

#define TRUE 1
#define FALSE 0

typedef unsigned char byte;
typedef byte bool;
typedef unsigned int uint;

#endif