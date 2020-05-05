#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dh_security.h"



unsigned int power(unsigned int a, unsigned long long n, unsigned int m)
{
    unsigned long long b = 1;
    unsigned long long c = a;
    unsigned long long k = n;
    
    while(k > 0)
    {
        if(k % 2 == 0)
        {
            c = (c * c) % m;
            k /= 2;
        }
        else
        {
            b = (b * c) % m;
            k--;
        }
    }
    
    return (unsigned int) b;
}

/*
 * build package with parameters and return length of the package
 * packet should be at least 34 bytes in size
 */
int build_packet(char *packet, unsigned int q, unsigned int p, unsigned int k)
{
    sprintf(packet,"%u-%u-%u",q ,p, k);
    return strlen(packet);
}

/*
 * parse package with parameters
 */
void parse_packet(char *packet, unsigned int *q, unsigned int *p, unsigned int *k)
{
    sscanf(packet, "%u-%u-%u", q, p, k);
}

unsigned long long get_alpha()
{
    return (long long) rand() * rand();
}

/*
 * public
 */

void dh_update_alpha_k(dh_security_t *dh)
{
    dh->alpha = get_alpha();
    dh->k_local = power(dh->q, dh->alpha, dh->p);    
}

void dh_init_by_default(dh_security_t *dh)
{
    dh->q = 514229;
    dh->p = ((unsigned int)1 << 31) - 1;
    dh_update_alpha_k(dh);
    dh->k_external = 0;
}

void dh_update_from_packet(char *packet, dh_security_t *dh)
{
    parse_packet(packet, &(dh->q), &(dh->p), &(dh->k_external));
}

void dh_init_from_packet(char *packet, dh_security_t *dh)
{
    dh_update_from_packet(packet, dh);
    dh_update_alpha_k(dh);
}

int dh_print_to_packet(char *packet, dh_security_t *dh)
{
    return build_packet(packet, dh->q, dh->p, dh->k_local);
}

void dh_get_symmetric_key(char *key, dh_security_t *dh)
{
    int k = power(dh->k_external, dh->alpha, dh->p);
    sprintf(key, "%d", k);
}


/*
 * for tests
 */
/*
int main(int argc, char *argv[])
{
    unsigned int a;
    unsigned long long n;
    unsigned int m;
    
    //sscanf(argv[1], "%u", &a);
    //sscanf(argv[2], "%llu", &n);
    //sscanf(argv[3], "%u", &m);
    
    //printf("%u\n", power(a, n, m));
    
    dh_security_t dh;
    
    dh_init_by_default(&dh);
    char S[100];
    dh_print_to_packet(S, &dh);
    printf("%s\n", S);
    dh_update_from_packet(S, &dh);
    dh_get_symmetric_key(S, &dh);
    printf("%s\n", S);

    return 0;
    
}
*/
