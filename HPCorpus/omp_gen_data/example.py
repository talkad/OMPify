from parse_tools import parse
import preprocess


code = """
int main() {
    int num_threads = 4;

    omp_set_num_threads(num_threads);

    int n = 16; 

    #pragma omp target teams distribute parallel for simd
    for (int i = 0; i < n; i++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d: Iteration %d\n", thread_id, i);
    }

    return 0;
}
"""

code = """
program DoLoopExample
    implicit none

    integer :: i

    ! Loop from 1 to 10
    !$omp parallel do
    do i = 1, 10
        print *, "Iteration: ", i
    end do

end program DoLoopExample
"""
node = parse(code, lang='fortran').root_node
preprocess.print_tree(node)