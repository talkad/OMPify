PROGRAM Area
IMPLICIT NONE


! Declare local variables
REAL :: radius, Area_Circle
INTEGER :: idx

! what
!$omp parallel do
DO idx=1,3
  print*, idx
END DO

END PROGRAM Area