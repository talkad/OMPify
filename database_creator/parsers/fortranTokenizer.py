from HPCorpus_parser.parse_tools import parse

def iterate_tree(node):
    
    if node.children:
        return list(map(iterate_tree, node.children))
    else:
        return [node.text.decode()]
    

def flatten_list(nested_list):
    flattened_list = []

    for item in nested_list:
        if isinstance(item, list):
            flattened_list.extend(flatten_list(item))
        else:
            flattened_list.append(item)

    return flattened_list


def tokenize(code):
    tree = parse(code, lang='fortran')

    return flatten_list(iterate_tree(tree.root_node))



# code ='''
# program sum_array
#   implicit none

#   integer, parameter :: n = 5
#   integer :: i
#   real :: array(n)
#   real :: sum

#   array = [1.0, 2.0, 3.0, 4.0, 5.0]

#   sum = 0.0
#   do i = 1, n
#     sum = sum + array(i)
#   end do

#   print *, "Sum:", sum

# end program sum_array
# '''

# print(tokenize(code))
# ['program', 'sum_array', 'implicit', 'none', 'integer', ',', 'parameter', '::', 'n', '=', '5', 'integer', '::', 'i', 'real', '::', 'array', '(', 'n', ')', 'real', '::', 'sum', 'array', '=', '[', '1.0', ',', '2.0', ',', '3.0', ',', '4.0', ',', '5.0', ']', 'sum', '=', '0.0', 'do', 'i', '=', '1', ',', 'n', 'sum', '=', 'sum', '+', 'array', '(', 'i', ')', 'end', 'do', 'print', '*', ',', '"Sum:"', ',', 'sum', 'end', 'program', 'sum_array']
