//
// Created by golobii on 8/29/22.
//

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

// Error handling doesn't really need to be that good, because the compiler will handle everything.
void throw(char *message) {
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}