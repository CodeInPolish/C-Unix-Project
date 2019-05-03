#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <error.h>

#define SYS(a) ((a) == -1) ? error(1, EAGAIN, #a ":ERROR\n") : 0
#define SYSN(a) ((a) == NULL) ? error(1, EAGAIN, #a ":ERROR\n") : 0

#endif