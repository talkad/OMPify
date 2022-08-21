#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv){
    
    int sum = 0;
    int i, j;
    int mark[5] = {19, 10, 8, 17, 9};

    #pragma omp parallel for reduction(+: sum)
    for(i = 0; i < 5; ++i)
    {
        for(j = 0; j < 5; ++ j){
            sum += mark[i];
        }
    }

    for(i = 0; i < 5; ++i)
    {
        printf("hello world ");
    }

    printf("the sum is %i", sum);

    return 0;
}