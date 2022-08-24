
    SUBROUTINE tea_leaf_calc_2norm_kernel(x_min, &
            x_max,             &
            y_min,             &
            y_max,             &
            arr,               &
            norm, &
            dev_id)

        IMPLICIT NONE

        INTEGER(KIND=4):: x_min,x_max,y_min,y_max
        REAL(KIND=8), DIMENSION(x_min-2:x_max+2,y_min-2:y_max+2) :: arr
        REAL(KIND=8) :: norm
        integer :: j,k,l,dev_id


        norm = 0.0_8
        !$OMP TARGET DEVICE(dev_id)
        !$OMP PARALLEL DO REDUCTION(+:norm)
        DO k=y_min,y_max
            DO j=x_min,x_max
                norm = norm + arr(j, k)*arr(j, k)
            ENDDO
        ENDDO
        !$OMP END PARALLEL DO
        !$OMP END TARGET
    end SUBROUTINE tea_leaf_calc_2norm_kernel
