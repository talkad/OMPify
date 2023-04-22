from pycparser import parse_file, c_ast, c_generator

class LoopSample:
    def __init__(self):
        self.func_name = ''           # current function name
        self.for_node = None          # for loop node
        self.for_variables = []       # variables used within the loop
        self.pragma = ''              # pragma related to the loop (if exists)
        self.functions = {}           # function nodes called within the loop
        self.var_decls = {}           # var decleration nodes of variables used within the loop
        self.prev_assignments = []    # assignments nodes occured before the loop, include variables used within the loop
        self.post_assignments = []    # assignments nodes occured after the loop, include variables used within the loop

        self.code_generator = c_generator.CGenerator()

    def stmnt2str(self, nodes):
        code = ''
        for node in nodes:
            code += self.code_generator.visit(node) + ';\n'

        return code

    def __str__(self):
        # print('for_node', self.for_node)
        # print('for_variables', self.for_variables)
        # print('pragma', self.pragma)
        # print('var_decls', self.var_decls)
        # print('prev_assignments', self.prev_assignments)
        # print('post_assignments', self.post_assignments)

        code = '//functions\n'
        code += '\n'.join([self.code_generator.visit(func) for func in self.functions.values()]) + '\n'
        code += '//declerations\n'
        code += self.stmnt2str(self.var_decls.values()) + '\n'
        code += '//assignments\n'
        code += self.stmnt2str(self.prev_assignments) + '\n'
        code += '//LOOP\n'
        code +=  self.code_generator.visit(self.for_node) + '\n'
        code += '//assignments\n'
        code += self.stmnt2str(self.post_assignments)

        return code


class LoopExtractor(c_ast.NodeVisitor):
    def __init__(self, functions):
        self.loop_samples = []
        self.assignments = []
        self.var_decls = {}
        self.functions = functions
        self.current_func_name = ''

        self.var_extractor = VariableExtractor()
        self.funcCall_extractor = FuncCallExtractor()
        self.decl_extractor = DeclExtractor()

        self.inner_visitor = InnerLoopExtractor()

    def reset(self):
        self.loop_samples = []
        self.assignments = []
        self.var_decls = {}

    def extract(self, func_name, node):
        self.current_func_name = func_name
        self.var_decls = self.decl_extractor.extract(node)

        try:
            self.visit(node)
        except:
            pass
        return self.loop_samples

    def create_sample(self):
        if len(self.loop_samples) > 0 and len(self.loop_samples[-1].pragma)>0  and self.loop_samples[-1].for_node is None:
            sample = self.loop_samples[-1]
            self.loop_samples = self.loop_samples[:-1]
            return sample

        return LoopSample()

    def fill_fields(self, sample):
        # current function name
        sample.func_name = self.current_func_name
        
        # variables used within the for-loop
        sample.for_variables = self.var_extractor.extract(sample.for_node)

        # functions used within the for-loop
        function_names = self.funcCall_extractor.extract(sample.for_node)

        for func_name in function_names:
            sample.functions[func_name] = self.functions[func_name] if func_name in self.functions else None

        # variable decleration
        for var in sample.for_variables:
            if var in self.var_decls:
                sample.var_decls[var] = self.var_decls[var]

        # assignments occured before the for-loop
        for assignment in self.assignments:
            assignment_vars = self.var_extractor.extract(assignment)

            if any ([var in assignment_vars for var in sample.for_variables]):
                sample.prev_assignments.append(assignment)


    def visit_For(self, node):
        sample = self.create_sample()
        sample.for_node = node
        self.fill_fields(sample)
        self.loop_samples.append(sample)

        samples = self.inner_visitor.extract(node)
        for sample in samples:
            self.fill_fields(sample)
            self.loop_samples.append(sample)
        self.inner_visitor.reset()

    def visit_Pragma(self, node):
        line = node.string.lower()

        if "omp" in line and " for" in line:
            sample = LoopSample()
            sample.pragma = line

            self.loop_samples.append(sample)

    def visit_Assignment(self, node):
        self.assignments.append(node)

        # add the assignemnt to for-loops that already found
        assignment_vars = self.var_extractor.extract(node)
        for sample in self.loop_samples:
            if any ([var in assignment_vars for var in sample.for_variables]):
                sample.post_assignments.append(node)


class InnerLoopExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.loop_samples = []

    def extract(self, node):
        try:
            self.visit(node)
        except:
            pass
        
        return self.loop_samples

    def reset(self):
        self.loop_samples = []

    def visit_For(self, node):
        if len(self.loop_samples) > 0 and len(self.loop_samples[-1].pragma)>0:
            sample = self.loop_samples[-1]
            self.loop_samples = self.loop_samples[:-1]
            sample.for_node = node
            self.loop_samples.append(sample)

        # capture inner for loops as well
        try:
            self.generic_visit(node)
        except:
            pass

    def visit_Pragma(self, node):
        line = node.string.lower()

        if "omp" in line and " for" in line:
            sample = LoopSample()
            sample.pragma = line

            self.loop_samples.append(sample)


class VariableExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.variables = []

    def extract(self, node):
        self.variables = []
        try:
            self.visit(node)
        except:
            pass
        return self.variables

    def visit_ID(self, node):
        var_name = node.name

        if var_name not in self.variables:
            self.variables.append(var_name)


class FuncCallExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.func_calls = []

    def extract(self, node):
        self.func_calls = []
        try:
            self.visit(node)
        except:
            pass
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
        try:
            self.visit(node)
        except:
            pass
        return self.functions

    def visit_FuncDef(self, node):
        self.functions[node.decl.name] = node


class DeclExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.decls = {}

    def extract(self, node):
        self.decls = {}
        try:
            self.visit(node)
        except:
            pass
        return self.decls

    def visit_ArrayDecl(self, node):
        self.decls.append(node)

    def visit_Decl(self, node):
        if any([type(node.type) is ast_type for ast_type in [c_ast.PtrDecl, c_ast.TypeDecl]]):
            self.decls[node.name] = node
