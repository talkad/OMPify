from asts import parse_tools, convert_representation as cr


code = """subroutine meshrefin(n1,x,y,z,n)

    integer(i4), intent(in) :: n  

    integer(i4), intent(inout) :: n1 

    real(r8), intent(inout) ::  x(1:n)
    real(r8), intent(inout) :: y(1:n)
    real(r8), intent(inout) :: z(1:n)

    integer(i4), allocatable :: list(:)
    integer(i4), allocatable :: lptr(:)
    integer(i4), allocatable :: lend(:)
    integer(i4), allocatable :: iwk(:)
    real(r8), allocatable :: dist(:)
    integer (i4):: lnew
    integer(i4), allocatable :: neigh(:,:)
    integer(i4), allocatable :: neisz(:)

    

    integer(i4):: node
    integer(i4):: lpl
    integer(i4):: k
    integer(i4):: lp
    integer(i4):: ier
    integer(i4):: i
    integer(i4):: j

    

    real (r8):: r

    allocate(list(1:3*(2*n1-4)), lptr(1:3*(2*n1-4)), lend(1:n1))
    allocate(iwk(1:3*(2*n1-4)))
    allocate(dist(1:n1))
    allocate(neigh(1:n1,1:30), neisz(1:n1))

    

    call trmesh (n1,x,y,z,list,lptr,lend,lnew,iwk,iwk(n1+1),dist,ier)

    

    

    do  node = 1,n1
       lpl = lend(node)
       lp = lpl
       do k=1,20
          lp = lptr(lp)
          neigh(node,k) = list(lp)
          if(lp == lpl)then
             exit
          end if
       end do
       neisz(node) = k
       

    enddo

    

    k=n1+1
    do i = 1,n1
       do j = 1,neisz(i)
          if (neigh(i,j)>i) then
             x(k) = 0.5*(x(i)+x(neigh(i,j)))
             y(k) = 0.5*(y(i)+y(neigh(i,j)))
             z(k) = 0.5*(z(i)+z(neigh(i,j)))
             r = dsqrt(x(k)**2+y(k)**2+z(k)**2)
             x(k) = x(k)/r
             y(k) = y(k)/r
             z(k) = z(k)/r
             k=k+1
          endif
       end do
    enddo
    

    n1=k-1
    


    return
  end subroutine meshrefin
"""


code = """int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 5;
    int y = 10;
    int result = add(x, y);
    printf("Result: %d\n", result);
    return 0;
}
"""

ast = parse_tools.parse(code, lang='c')
replaced_code = cr.generate_replaced(ast)

print(replaced_code)
