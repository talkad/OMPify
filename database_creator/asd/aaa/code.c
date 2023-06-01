static long num_steps = 100000; 
double step;

int main ()
{
    double pi, sum = 0.0;
    step = 1.0/(double) num_steps;

        for (int i=0;i< num_steps; i++){
            double x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }

    pi = step * sum;

    return pi;
}