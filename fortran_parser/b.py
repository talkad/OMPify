Program(Comment(''), 
Main_Program(Program_Stmt('PROGRAM', Name('Area')),

 Specification_Part(Implicit_Part(Implicit_Stmt('NONE'), Comment('')),
     Type_Declaration_Stmt(Intrinsic_Type_Spec('REAL', None), None, 
     Entity_Decl_List(',', (Entity_Decl(Name('radius'), None, None, None), 
     Entity_Decl(Name('Area_Circle'), None, None, None)))), 
     
     Type_Declaration_Stmt(Intrinsic_Type_Spec('INTEGER', None), None, 
     Entity_Decl_List(',', (Entity_Decl(Name('idx'), None, None, None),)))), 
     
     
     Execution_Part(Block_Nonlabel_Do_Construct(Nonlabel_Do_Stmt('DO', Loop_Control(None, (Name('idx'), [Int_Literal_Constant('1', None), Int_Literal_Constant('3', None)]), None)), 
     Print_Stmt(Format('*'), Output_Item_List(',', (Name('idx'),))), End_Do_Stmt('DO', None)),
     
      Block_Label_Do_Construct(Comment(''), Comment(''), Label_Do_Stmt(None, Label('100'), Loop_Control(None, (Name('idx'), [Int_Literal_Constant('1', None), Int_Literal_Constant('3', None)]), None)),
       Print_Stmt(Format('*'), Output_Item_List(',', (Name('idx'),))), Continue_Stmt('CONTINUE')), 
       
    Block_Nonlabel_Do_Construct(Comment(''), Nonlabel_Do_Stmt('DO', Loop_Control(None, (Name('idx'), [Int_Literal_Constant('1', None), Int_Literal_Constant('3', None)]), None)),
     Print_Stmt(Format('*'), Output_Item_List(',', (Name('idx'),))), End_Do_Stmt('DO', None))), 
     
     
     End_Program_Stmt('PROGRAM', Name('Area'))))