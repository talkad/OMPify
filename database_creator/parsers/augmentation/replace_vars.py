import json
import re
import random
from pycparser import parse_file, c_ast, c_generator
from pycparser.plyparser import Coord
from collections import OrderedDict


VAR_PREFIX = "var"
ARR_PREFIX = "arr"
FUNC_PREFIX = "func"
STRUCT_PREFIX = "struct"


# Puts in array all the ids found, function name(calls), array and structs
class ReplaceIdsVisitor(c_ast.NodeVisitor):
    def __init__(self, var, array, struct, func):
        # remove duplicates..
        self.var = list(OrderedDict.fromkeys(var))
        self.array = list(OrderedDict.fromkeys(array))
        self.struct = list(OrderedDict.fromkeys(struct))
        self.func = list(OrderedDict.fromkeys(func))

        # now remove from self.var all the names from array struct and func
        self.var = [v for v in self.var if v not in self.array]
        self.var = [v for v in self.var if v not in self.struct]
        self.var = [v for v in self.var if v not in self.func]

        self.name_mapping = {}
        for type, names in zip([VAR_PREFIX, ARR_PREFIX, FUNC_PREFIX, STRUCT_PREFIX],
                                [self.var, self.array, self.func, self.struct]):
            idxs = list(range(len(names)))
            random.shuffle(idxs)
            # print(idxs)
            # print(names)

            for idx, name in zip(idxs, names):
                self.name_mapping[name] = f'{type}_{idx}'

    def visit_ID(self, node):
        if node.name in self.name_mapping:
            node.name = self.name_mapping[node.name]

    def visit_Decl(self, node):
        if node.name in self.name_mapping:
            node.name = self.name_mapping[node.name]

    def visit_TypeDecl(self, node):
        if node.declname in self.name_mapping:
            node.declname = self.name_mapping[node.declname]

    def visit_ArrayDecl(self, node):
        if isinstance(node.type, c_ast.PtrDecl):
            if node.type.type.declname in self.name_mapping:
                node.type.type.declname = self.name_mapping[node.type.type.declname]
        if isinstance(node.type, c_ast.TypeDecl):
            if node.type.declname in self.name_mapping:
                node.type.declname= self.name_mapping[node.type.declname]

    def reset(self):
        self.var = []
        self.array = []
        self.struct = []
        self.func = []



# Puts in array all the ids found, function name(calls), array and structs
class CounterIdVisitor(c_ast.NodeVisitor):
    def __init__(self):
        self.reset()

    def visit_ID(self, node):
        if node.name:
            self.ids.append(node.name)

    def visit_FuncCall(self, node):
        try:
            if isinstance(node.name, c_ast.UnaryOp):
                if node.name.op == '*':
                    self.generic_visit(node)
            else:
                self.func.append(node.name.name)
                self.generic_visit(node)
        except:
            pass

    def visit_ArrayRef(self, node):
        if isinstance(node.name, c_ast.BinaryOp):
            self.generic_visit(node)
            return
        # CASTING
        if isinstance(node.name, c_ast.Cast):
            if isinstance(node.name.expr, c_ast.ID) or isinstance(node.name.expr, c_ast.ArrayRef):
                name = node.name.expr.name
            if isinstance(node.name.expr, c_ast.StructRef):
                name = node.name.expr.field
            if isinstance(node.name.expr, c_ast.UnaryOp):
                if isinstance(node.name.expr.expr, c_ast.ArrayRef):
                    self.generic_visit(node)
                    return
                name = node.name.expr.expr.name
        # ARRAY OF STRUCT
        if isinstance(node.name, c_ast.StructRef):
            name = node.name.field
        # NORMAL
        if isinstance(node.name, c_ast.ID):
            name = node.name.name
        # UNARY OP WHICH IS BASICALLY CAST TO STRUCT..
        if isinstance(node.name, c_ast.UnaryOp):
            if isinstance(node.name.expr, c_ast.StructRef):
                name = node.name.expr.field
            if isinstance(node.name.expr, c_ast.ID):
                name = node.name.expr.name
        if isinstance(node.name, c_ast.ArrayRef):
            # if it is an array of arrays (2d array etc, we will just continue to the next expr..)
            self.generic_visit(node)
            return
        try:
            if isinstance(name, c_ast.ID):
                name = name.name
            self.array.append(name)
            self.generic_visit(node)
        except:
            pass
            # print(node.name)
            # exit(1)

    def visit_ArrayDecl(self, node):
        if isinstance(node.type, c_ast.PtrDecl):
            name = node.type.type.declname
        if isinstance(node.type, c_ast.TypeDecl):
            name = node.type.declname
        if isinstance(node.type, c_ast.ArrayDecl):
            self.generic_visit(node)
            return
        try:
            self.array.append(name)
            self.generic_visit(node)
        except:
            print(node)
            exit(1)

    def visit_StructRef(self, node):
        if isinstance(node.name, c_ast.ID):
            name = node.name.name
            self.struct.append(name)
        self.generic_visit(node)

    def reset(self):
        self.ids = []
        self.func = []
        self.array = []
        self.struct = []
