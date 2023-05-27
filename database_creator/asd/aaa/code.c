static long num_steps = 100000; 
double step;

void for_loop_talkad7420();
void omp_pragma_talkad7420(char *pragma);

int main ()
{
    double pi, sum = 0.0;
    step = 1.0/(double) num_steps;

        omp_pragma_talkad7420("#pragma omp parallel for reduction(+:sum)");
        for_loop_talkad7420();
        for (int i=0;i< num_steps; i++){
            double x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }

        omp_pragma_talkad7420("hello world");
    pi = step * sum;

    return pi;
}