#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
static long long getHash(byte* buffer, size_t size)
{
    static long long const base = 1000000007ll;
    unsigned long long hash = 0;
    //FILE* of = fopen("hash.txt", "w");
    for (int i = 0; i != size; i++)
    {
        if (i == 8)
        {
            int g = 0;
        }
        hash *= base;
        hash += buffer[i];
        //fprintf(of, "%d:   %016llx\n", i,  hash);
    }
    //fclose(of);
    return hash;
}