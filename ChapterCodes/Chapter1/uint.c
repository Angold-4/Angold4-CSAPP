typedef unsigned __int128 uint128_t;

void store_uprod(uint128_t *dest, unsigned long x, unsigned long y){
    *dest = x * (uint128_t) y;
}
