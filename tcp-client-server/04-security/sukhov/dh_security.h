#ifndef DF_SECURITY
#define DF_SECURITY 0

unsigned int power(unsigned int a, unsigned long long n, unsigned int m);

typedef struct
{
    unsigned int p,q;
    unsigned long long alpha;
    unsigned int k_local;
    unsigned int k_external;
} dh_security_t;

void dh_init_by_default(dh_security_t *dh);
void dh_update_alpha_k(dh_security_t *dh);
void dh_update_from_packet(char *packet, dh_security_t *dh);
void dh_init_from_packet(char *packet, dh_security_t *dh);
int dh_print_to_packet(char *packet, dh_security_t *dh);
void dh_get_symmetric_key(char *key, dh_security_t *dh);


#endif
