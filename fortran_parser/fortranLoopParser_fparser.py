
from fparser.two.parser import ParserFactory
from fparser.common.readfortran import FortranStringReader

MYFILE = """
PROGRAM Area
IMPLICIT NONE


! Declare local variables
REAL :: radius, Area_Circle
INTEGER :: idx

#!Somp parallel do
DO idx=1,3
  print*, idx
END DO



END PROGRAM Area
"""

READER = FortranStringReader(MYFILE)
F2008_PARSER = ParserFactory().create(std="f2008")
PROGRAM = F2008_PARSER(READER)
PROGRAM