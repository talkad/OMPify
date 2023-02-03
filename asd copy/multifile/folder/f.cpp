#include <string>
#include <omp.h>
typedef int AA;

void start_for_loop() {}
void end_for_loop() {}
void omp_pragma(std::string pragma) {}

int main()
{
        AA a;
        int N = 10000;
        int arr[N];

        int sum = 0;

        omp_pragma("#pragma omp parallel for");
        start_for_loop();
        for(int i = 0; i < N; i++)
                sum += arr[i];
        end_for_loop();
}
