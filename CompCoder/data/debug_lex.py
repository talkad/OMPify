from asts.lexicalization import lexicalize
from asts import parse_tools, convert_representation as cr


source = """function factorial(n) result(result)
    implicit none
    integer, intent(in) :: n
    integer :: result

    if (n == 0) then
        result = 1
    else
        result = n * factorial(n - 1)  ! Recursive call
    end if
end function factorial
"""
ast = parse_tools.parse(source, 'fortran')
code = cr.generate_replaced(ast)
# print(code)
lex = lexicalize(code, lang='fortran', replaced=True)
print(lex)
