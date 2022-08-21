PROGRAM Area
!---------------------------------------------------------------------
!
!  This program computes the area of a circle given the input radius
!
!  Uses:  SUBROUTINE Compute_Area(r, Area)
!
!---------------------------------------------------------------------
IMPLICIT NONE

INTERFACE 
   SUBROUTINE Compute_Area(r, Area)
     REAL, INTENT(IN) :: r
     REAL, INTENT(OUT) :: Area
   END SUBROUTINE Compute_Area
END INTERFACE

! Declare local variables
REAL :: radius, Area_Circle
INTEGER :: idx
INTEGER          :: x
CHARACTER(LEN=1) :: Grade

DO idx=1,3
  print*, idx
END DO

! Prompt user for radius of circle
write(*, '(A)', ADVANCE = "NO") "Enter the radius of the circle:  "
read(*,*) radius

! Call subroutine to compute area of circle
CALL Compute_Area(radius, Area_Circle)

! Write out area of circle 
write(*,100) "Area of circle with radius", radius, " is", &
            Area_Circle
100 format (A, 2x, F6.2, A, 2x, F11.2)

IF (x < 50) THEN
   Grade = 'F'
ELSE IF (x < 60) THEN
   Grade = 'D'
ELSE IF (x < 70) THEN
   Grade = 'C'
ELSE IF (x < 80) THEN
   Grade = 'B'
ELSE
   Grade = 'A'
END IF

END PROGRAM Area