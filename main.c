/*Gideon Neeman 329924567*/
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char uchar;

typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar *block;
} cache_line_t;

typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t **cache;
} cache_t;
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar *start, long int off);
void write_byte(cache_t cache, uchar *start, long int off, uchar new);
void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                   cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t myCache;
    myCache.s = s;
    myCache.t = t;
    myCache.b = b;
    myCache.E = E;
    myCache.cache = (cache_line_t **) malloc((1 << s) * sizeof(cache_line_t *));
    for (int i = 0; i < (1 << s); i++) {
        myCache.cache[i] = (cache_line_t *) malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            myCache.cache[i][j].valid = 0;
            myCache.cache[i][j].frequency = 0;
            myCache.cache[i][j].tag = 0;
            myCache.cache[i][j].block = malloc((1 << b) * sizeof(uchar));
        }
    }
    return myCache;
}

uchar read_byte(cache_t cache, uchar *start, long int off) {
    int block = off & ((1 << cache.b) - 1);
    int set = (off >> cache.b) & ((1 << cache.s) - 1);
    int tag = off >> (cache.b + cache.s);

    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[set][i].valid == 1 && cache.cache[set][i].tag == tag) {
            cache.cache[set][i].frequency++;
            return cache.cache[set][i].block[block];
        }
    }
    for (int i = 0; i < cache.E; i++) {
        if (cache.cache[set][i].valid != 1) {
            cache.cache[set][i].valid = 1;
            cache.cache[set][i].tag = tag;
            cache.cache[set][i].frequency = 1;
            cache.cache[set][i].block = start + off;
            return cache.cache[set][i].block[block];
        }
    }
    int min = 0;
    for (int i = 1; i < cache.E; i++) {
        if (cache.cache[set][i].frequency < cache.cache[set][min].frequency) {
            min = i;
        }
    }
    cache.cache[set][min].tag = tag;
    cache.cache[set][min].frequency = 1;
    cache.cache[set][min].block = start + off;
    return cache.cache[set][min].block[block];
}

int main() {
    int n;
    printf("Size of data: ");
    scanf("%d", &n);
    uchar *mem = malloc(n);
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);

    int s, t, b, E;
    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);
    cache_t cache = initialize_cache(s, t, b, E);

    while (1) {
        scanf("%d", &n);
        if (n < 0) break;
        read_byte(cache, mem, n);
    }
    puts("");
    print_cache(cache);
    free(mem);
}
