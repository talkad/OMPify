#define AA 5

#if AA==1

int main(){
    int sum = 0;
    for(int i=0; i<10;++i)
        sum += i;

    printf(sum);
}
#elif (AA==2)

int main(){
    int sum = 0;
    #pragma omp for
    for(int i=0; i<10;++i)
        sum += i;

    printf(sum);
}
#else 
int main(){
    int sum = 0;
    for(int i=0; i<10;++i)
    {

    }

    printf(sum);
}


#endif

