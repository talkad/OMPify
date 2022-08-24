from pycparser import parse_file, c_ast, c_generator


#  Traverse the whole code and attempts to find an omp for pragma omp that afterwards there is a for loop
class PragmaForVisitor(c_ast.NodeVisitor):
    def __init__(self):
        self.pos_nodes = []
        self.neg_nodes = []
        self.pragmas = []
        self.found = False

    def visit_For(self, node):
        if self.found:
            self.pos_nodes.append(node)
            self.found = False
        else:
            self.neg_nodes.append(node)
            # self.generic_visit(node)

    def visit_Pragma(self, node):
        line = node.string.lower()

        if "omp" in line and " for" in line:
            self.pragmas.append(node)
            self.found = True

    def generic_visit(self, node):
        """ Called if no explicit visitor function exists for a node. 
            Implements preorder visiting of the node.
        """
        self.found = False
        for c in node:
            self.visit(c)

# Travels the node of a outer for-loop that has an openmp directive, and it if finds an atomic or critical inside - returns true!
class ForLoopChecker(c_ast.NodeVisitor):
    """
    Class that travels the node of a for loop that has an openmp directive to find an atomic
    """
    def __init__(self):
        self.found = False

    def reset(self):
        self.found = False

    def visit_Pragma(self, node):
        if "omp" in node.string and ("atomic" in node.string or "barri" in node.string or "critical" in node.string):
            self.found = True
        else:
            self.generic_visit(node)


# Travels the node and define whether it contains func call in it
class FuncCallChecker(c_ast.NodeVisitor):
    """
    Class that travels the node of a for loop that has an openmp directive to find an atomic
    """
    def __init__(self):
        self.found = False

    def reset(self):
        self.found = False

    def visit_FuncCall(self, node):
        self.found = True

