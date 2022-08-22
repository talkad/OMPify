module m_iter_jacobi
use m_global_data, only: mk, nthreads
! openmp declaration
!$ use omp_lib 
contains
    subroutine iter_jacobi(N,T_old,T_new,f,k_max,d,X_start, X_end, Y_start, Y_end)
    implicit none
    integer(mk) :: i, j, k, k_max, count0, count1, c_rate, tid
    integer(mk) :: N
    real(mk) :: elap_time, norm, sum, d, delta_X, delta_Y, X_start, X_end, Y_start, Y_end,norm1,norm2, aii, t0, t1
    real(mk),dimension(:,:) :: T_new, T_old, f


    delta_X = (X_end - X_start) / real(N+1, mk)
    delta_Y = (Y_end - Y_start) / real(N+1, mk)
    !print*, delta_X
    !print*, delta_Y
    norm = 50
    k = 0
    aii = 1.0/4.0
    sum = 0.0
    ! call system clock
    !call system_clock(count=count0, count_rate=c_rate)
    
    !$ t0 = omp_get_wtime()
     
   
    !$omp parallel default(shared) private(i, j, tid)
      !$omp master
!$  nthreads = omp_get_num_threads()
      print*,'num threads iter jacobi = ', nthreads
      !$omp end master
    do while (norm > d .and. k < k_max)
        
    !   !$omp single !puts up a implicit barrier and every thread runs it.
    !    sum = 0.0
    !   !$omp end single
        
       !!$omp parallel do num_threads(nthreads) default(shared) private(i, j) 
        !$omp do schedule(runtime) reduction(+ : sum) ! Can we place it only for the inner do where all the code is?
        do j=2,N+1
            do i=2,N+1
                T_new(i,j) = aii * (T_old(i-1,j) +&
                                        T_old(i+1,j) +&
                                        T_old(i,j-1) +&
                                        T_old(i,j+1) +&
                                        delta_X*delta_Y * f(i,j))
               ! !$omp critical(cric_sum)
                sum = sum + (T_new(i,j) - T_old(i,j))*(T_new(i,j) - T_old(i,j))
               ! !$omp end critical(cric_sum)
                 !print*,'sum =', sum,'tid= ', tid   
            enddo
        !$  tid = omp_get_thread_num()
        enddo
      !$omp end do  
    !!$omp end parallel do
    !$omp single
    k = k + 1

    ! calculate the norm
    norm = sum ** 0.5

    sum = 0.0
    !print*,'k=',k, 'tid=',tid
    !print*,'norm=',norm, 'tid=',tid
    !$omp end single
    !print*,'d',d !debug
    !!$omp parallel default(shared) private(i,j)
    !$omp do schedule(runtime)
    do j=2,N+1
        do i=2,N+1
           T_old(i,j) = T_new(i,j)
        end do
     end do      
    !$omp end do nowait
    !!$omp end parallel
    !print*,'*',  'tid=',tid

    enddo
    !$omp end parallel


    print*,'Total iterations=',k
    print*,'converged norm=',norm
    ! time
    !call system_clock(count=count1)
    !elap_time = real(count1 - count0)/real(c_rate)
    !$ t1 = omp_get_wtime()
    elap_time = t1 -t0
    print*,'Wall time (fortran):', elap_time,'[s]' 
    print*,'Iteration rate=', k/elap_time,'[iteration/s]'  
    !print*,'delta_X*(N+1)=', delta_X*(N+1) !debug
    !print*,'delta_Y*(N+1)=', delta_Y*(N+1) !debug
    end subroutine iter_jacobi
end module