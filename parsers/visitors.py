from pycparser import parse_file, c_ast, c_generator


class PragmaForVisitor(c_ast.NodeVisitor):
    '''
    Traverse the whole code and attempts to find an omp for pragma omp that afterwards there is a for loop
    '''
    def __init__(self):
        self.inner_visitor = PragmaForInnerVisitor()
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
        
        # capture inner for loops as well
        # self.generic_visit(node)

        self.inner_visitor.visit(node)
        self.pos_nodes += self.inner_visitor.pos_nodes
        self.pragmas += self.inner_visitor.pragmas
        self.inner_visitor.reset()

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


class PragmaForInnerVisitor(c_ast.NodeVisitor):
    '''
    Traverse the whole code and attempts to find an omp for pragma omp that afterwards there is a for loop
    '''
    def __init__(self):
        self.pos_nodes = []
        self.pragmas = []
        self.found = False

    def reset(self):
        self.pos_nodes = []
        self.pragmas = []
        self.found = False

    def visit_For(self, node):
        if self.found:
            self.pos_nodes.append(node)
            self.found = False
        
        # capture inner for loops as well
        self.generic_visit(node)

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


class ForLoopChecker(c_ast.NodeVisitor):
    """
    Travels the node of a outer for-loop that has an openmp directive, and it if finds an atomic or critical inside - return true!
    """
    def __init__(self):
        self.found = False

    def reset(self):
        self.found = False

    def visit_Pragma(self, node):
        if "omp" in node.string and ("atomic" in node.string or "barrier" in node.string or "critical" in node.string):
            self.found = True
        else:
            self.generic_visit(node)


class OmpChecker(c_ast.NodeVisitor):
    """
    Travels the node of a outer for-loop that has an openmp directive, and it if finds any omp pragma - return true!
    """
    def __init__(self):
        self.found = False

    def reset(self):
        self.found = False

    def visit_Pragma(self, node):
        if "omp" in node.string:
            self.found = True
        else:
            self.generic_visit(node)


class FuncCallChecker(c_ast.NodeVisitor):
    """
    Travels the node and define whether it contains func call in it
    """
    def __init__(self):
        self.found = False
        self.func_calls = []

    def reset(self):
        self.found = False

    def visit_FuncCall(self, node):
        self.found = True
        self.func_calls.append(node)


class DepthVisitor(c_ast.NodeVisitor):
    '''
    Find the depth of AST
    '''
    def __init__(self):
        self.curr_len = 0

    def generic_visit(self, node):
        self.curr_len += 1

        for c in node:
            self.visit(c)


class FuncDefVisitor(c_ast.NodeVisitor):
    '''
    find all func. decl in node
    '''
    def __init__(self):
        self.func_def = []

    def visit_FuncDef(self, node):
        self.func_def.append(node)

