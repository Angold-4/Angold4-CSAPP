float float_mov(float v1, float *src, float *dest){
    float v2 = *src;
    *dest = v1;
    return v2;
}