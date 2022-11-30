

int main() {

    int i, j, sum = 0;

    #pragma omp parallel for
    for(i = 0; i < 10; i++){

        for(j = 0; j < 10; j++){

            do_some(i);
            do_some2(j);
        }
    }

    for(i = 0; i < 10; i++){
        #pragma omp parallel for
        for(j = 0; j < 10; j++){

            do_some(i);
        }
    }


}