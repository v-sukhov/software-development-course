#include "symmetric_security.h"

void encript(char *key, char *input, int len, char *output)
{
    int i = 0;
    int j = 0;
    while(i < len)
    {
        if(key[j] == 0)
        {
            j = 0;
        }
        output[i] = ((int)input[i] + key[j]) % 256;
        i++;
        j++;
    }
    output[len] = 0;
}

void decript(char *key, char *input, int len, char *output)
{
    int i = 0;
    int j = 0;
    while(i < len)
    {
        if(key[j] == 0)
        {
            j = 0;
        }
        output[i] = ((int)input[i] + 256 - key[j]) % 256;
        i++;
        j++;
    }
    output[len] = 0;
}
