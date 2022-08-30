
int main(){
    int i,j, sum = 0;

#ifdef AA

    #pragma omp parallel for
    for(i = 0; i < 10; ++i){
        #pragma omp parallel for
        for(j = 0; j < 10; ++j)
            sum = sum + i;
    }
#else
    for(j = 0; j < 10; ++j)
            sum = sum + i;
#endif
}