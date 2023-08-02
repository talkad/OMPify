from asts import parse_tools, convert_representation as cr
# from ast.tokenizer import Tokompiler
from asts.lexicalization import lexicalize

code = """FUNCTION calculate_pi(max, seed) RESULT(pi)
  IMPLICIT NONE
  INTEGER, INTENT(IN) :: max, seed
  REAL(8) :: pi
  REAL(8) :: area, x, y
  INTEGER :: i
  EXTERNAL :: DRAND48

  INTEGER :: pi_count

  pi_count = 0
  CALL seed48(seed)

  DO i = 1, max
     x = DRAND48() * 2 - 1
     y = DRAND48() * 2 - 1
     IF (x * x + y * y < 1) THEN
        pi_count = pi_count + 1
     END IF
     area = 4.0 * REAL(pi_count) / REAL(i)
  END DO

  pi = 4.0 * REAL(pi_count) / REAL(max)
END FUNCTION
""".lower()

# print(code)

# lex = lexicalize(code, lang='fortran')
# print(lex.split(), len(lex.split()))


ast = parse_tools.parse(code, lang='fortran')
replaced_code = cr.generate_replaced(ast)

print(replaced_code)
lex = lexicalize(replaced_code, lang='fortran', replaced=True)
print(lex.split(), len(lex.split()))


# xsbt = cr.code2xsbt(code)
# print(xsbt.split(), len(xsbt.split()))


ast = cr.code2ast(replaced_code).split()
ast_new = []

for token in ast:
   if token=='ERROR':
      continue
    
   if any([token.startswith(a) for a in cr.replaced_prefixes.values()]):
      ast_new += token.split('_')
   else:
      ast_new.append(token)
   
   
print(ast_new, len(ast_new))