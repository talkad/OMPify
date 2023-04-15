from pycparser import parse_file, c_ast, c_generator


class LoopSample:
    def __init__(self):
        self.for_node = None          # for loop node
        self.for_variables = []       # variables used within the loop
        self.pragma = ''              # pragma related to the loop (if exists)
        self.functions = {}           # function nodes called within the loop
        self.var_decls = {}           # var decleration nodes of variables used within the loop
        self.prev_assignments = []    # assignments nodes occured before the loop, include variables used within the loop
        self.post_assignments = []    # assignments nodes occured after the loop, include variables used within the loop


class LoopExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.loop_samples = []
        self.assignments = []
        self.var_decls = {}

        self.var_extractor = VariableExtractor()
        self.funcCall_extractor = FuncCallExtractor()
        self.funcDef_extractor = FunctionExtractor()
        self.decl_extractor = DeclExtractor()

    def extract(self, node):
        self.loop_samples = []
        self.visit(node)
        return self.loop_samples

    def create_sample(self):
        if len(self.samples) > 0 and len(self.loop_samples[-1].pragma)>0  and self.loop_samples[-1].for_node is None:
            sample = self.loop_samples[-1]
            self.loop_samples = self.loop_samples[:-1]
            return sample

        return LoopSample()

    def fill_fields(self, sample)
        sample.for_variables = var_extractor.extractor(sample.for_node)

    def visit_For(self, node):
        sample = self.create_sample()
        sample.for_node = node
        self.fill_fields(sample)
        self.loop_samples.append(sample)

        # self.inner_visitor.visit(node)
        # self.pos_nodes += self.inner_visitor.pos_nodes
        # self.pragmas += self.inner_visitor.pragmas
        # self.inner_visitor.reset()

    def visit_Pragma(self, node):
        line = node.string.lower()

        if "omp" in line and " for" in line:
            sample = LoopSample()
            sample.pragma = line

            self.loop_samples.append(sample)

    def visit_Assignment(self, node):
        self.assignments.append(node)


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



class VariableExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.variables = []

    def find_variables(self, node):
        self.variables = []
        self.visit(node)
        return self.variables

    def visit_ID(self, node):
        var_name = node.name

        if var_name not in self.variables:
            self.variables.append(var_name)


class FuncCallExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.func_calls = []

    def find_funcCalll(self, node):
        self.func_calls = []
        self.visit(node)
        return self.func_calls

    def visit_FuncCall(self, node):
        func_name = node.name.name

        if func_name not in self.func_calls:
            self.func_calls.append(func_name)


class FunctionExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.functions = {}  # function name : func AST

    def extract(self, node):
        self.functions = {}
        self.visit(node)
        return self.functions

    def visit_FuncDef(self, node):
        self.functions[node.decl.name] = node


class DeclExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.decls = {}

    def extract(self, node):
        self.decls = {}
        self.visit(node)
        return self.decls

    def visit_ArrayDecl(self, node):
        self.decls.append(node)

    def visit_Decl(self, node):
        if any([type(node.type) is ast_type for ast_type in [c_ast.PtrDecl, c_ast.TypeDecl]]):
            self.decls[node.name] = node
