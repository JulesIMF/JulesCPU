#include <stdio.h>
#include "Common.h"
#include "SetColor.h"

static void errorMessage(char const* error, int lineN)
{
    setColor(LightRed);
    if (lineN > 0)
        printf("Error: %s. Line %d. Stop.\n", error, lineN);
    else
        printf("Error: %s. Stop.\n", error);
    setColor(LightGray);
}