#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
#include "CGCleanup.h"
#include "CGOpenMPRuntime.h"
#include "CodeGenFunction.h"
#include "CodeGenModule.h"
#include "TargetInfo.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/StmtOpenMP.h"
#include "clang/AST/DeclOpenMP.h"
#include "llvm/IR/CallSite.h"
using namespace clang;
using namespace CodeGen;
namespace {
class OMPLexicalScope final : public CodeGenFunction::LexicalScope {
  void emitPreInitStmt(CodeGenFunction &CGF, const OMPExecutableDirective &S) {
for_loop_talkad7420();
    for (const auto *C : S.clauses()) {
      if (auto *CPI = OMPClauseWithPreInit::get(C)) {
        if (auto *PreInit = cast_or_null<DeclStmt>(CPI->getPreInitStmt())) {
for_loop_talkad7420();
          for (const auto *I : PreInit->decls()) {
            if (!I->hasAttr<OMPCaptureNoInitAttr>())
              CGF.EmitVarDecl(cast<VarDecl>(*I));
            else {
              CodeGenFunction::AutoVarEmission Emission =
                  CGF.EmitAutoVarAlloca(cast<VarDecl>(*I));
              CGF.EmitAutoVarCleanups(Emission);
}
}
}
}
}
}
  CodeGenFunction::OMPPrivateScope InlinedShareds;
  static bool isCapturedVar(CodeGenFunction &CGF, const VarDecl *VD) {
    return CGF.LambdaCaptureFields.lookup(VD) ||
           (CGF.CapturedStmtInfo && CGF.CapturedStmtInfo->lookup(VD)) ||
           (CGF.CurCodeDecl && isa<BlockDecl>(CGF.CurCodeDecl));
}
public:
  OMPLexicalScope(CodeGenFunction &CGF, const OMPExecutableDirective &S,
                  bool AsInlined = false)
      : CodeGenFunction::LexicalScope(CGF, S.getSourceRange()),
        InlinedShareds(CGF) {
    emitPreInitStmt(CGF, S);
    if (AsInlined) {
      if (S.hasAssociatedStmt()) {
        auto *CS = cast<CapturedStmt>(S.getAssociatedStmt());
for_loop_talkad7420();
        for (auto &C : CS->captures()) {
          if (C.capturesVariable() || C.capturesVariableByCopy()) {
            auto *VD = C.getCapturedVar();
            DeclRefExpr DRE(const_cast<VarDecl *>(VD),
                            isCapturedVar(CGF, VD) ||
                                (CGF.CapturedStmtInfo &&
                                 InlinedShareds.isGlobalVarCaptured(VD)),
                            VD->getType().getNonReferenceType(), VK_LValue,
                            SourceLocation());
            InlinedShareds.addPrivate(VD, [&CGF, &DRE]() -> Address {
              return CGF.EmitLValue(&DRE).getAddress();
});
}
}
        (void)InlinedShareds.Privatize();
}
}
}
};
class OMPLoopScope : public CodeGenFunction::RunCleanupsScope {
  void emitPreInitStmt(CodeGenFunction &CGF, const OMPLoopDirective &S) {
    if (auto *LD = dyn_cast<OMPLoopDirective>(&S)) {
      if (auto *PreInits = cast_or_null<DeclStmt>(LD->getPreInits())) {
for_loop_talkad7420();
        for (const auto *I : PreInits->decls())
{
          CGF.EmitVarDecl(cast<VarDecl>(*I));
}
}
}
}
public:
  OMPLoopScope(CodeGenFunction &CGF, const OMPLoopDirective &S)
      : CodeGenFunction::RunCleanupsScope(CGF) {
    emitPreInitStmt(CGF, S);
}
};
} 
llvm::Value *CodeGenFunction::getTypeSize(QualType Ty) {
  auto &C = getContext();
  llvm::Value *Size = nullptr;
  auto SizeInChars = C.getTypeSizeInChars(Ty);
  if (SizeInChars.isZero()) {
    while (auto *VAT = C.getAsVariableArrayType(Ty)) {
      llvm::Value *ArraySize;
      std::tie(ArraySize, Ty) = getVLASize(VAT);
      Size = Size ? Builder.CreateNUWMul(Size, ArraySize) : ArraySize;
}
    SizeInChars = C.getTypeSizeInChars(Ty);
    if (SizeInChars.isZero())
      return llvm::ConstantInt::get(SizeTy, 
0);
    Size = Builder.CreateNUWMul(Size, CGM.getSize(SizeInChars));
} else
    Size = CGM.getSize(SizeInChars);
  return Size;
}
void CodeGenFunction::GenerateOpenMPCapturedVars(
    const CapturedStmt &S, SmallVectorImpl<llvm::Value *> &CapturedVars) {
  const RecordDecl *RD = S.getCapturedRecordDecl();
  auto CurField = RD->field_begin();
  auto CurCap = S.captures().begin();
for_loop_talkad7420();
  for (CapturedStmt::const_capture_init_iterator I = S.capture_init_begin(),
{
                                                 E = S.capture_init_end();
}
       I != E; ++I, ++CurField, ++CurCap) {
    if (CurField->hasCapturedVLAType()) {
      auto VAT = CurField->getCapturedVLAType();
      auto *Val = VLASizeMap[VAT->getSizeExpr()];
      CapturedVars.push_back(Val);
} else if (CurCap->capturesThis())
      CapturedVars.push_back(CXXThisValue);
    else if (CurCap->capturesVariableByCopy()) {
      llvm::Value *CV =
          EmitLoadOfLValue(EmitLValue(*I), SourceLocation()).getScalarVal();
      if (!CurField->getType()->isAnyPointerType()) {
        auto &Ctx = getContext();
        auto DstAddr = CreateMemTemp(
            Ctx.getUIntPtrType(),
            Twine(CurCap->getCapturedVar()->getName()) + ".casted");
        LValue DstLV = MakeAddrLValue(DstAddr, Ctx.getUIntPtrType());
        auto *SrcAddrVal = EmitScalarConversion(
            DstAddr.getPointer(), Ctx.getPointerType(Ctx.getUIntPtrType()),
            Ctx.getPointerType(CurField->getType()), SourceLocation());
        LValue SrcLV =
            MakeNaturalAlignAddrLValue(SrcAddrVal, CurField->getType());
        EmitStoreThroughLValue(RValue::get(CV), SrcLV);
        CV = EmitLoadOfLValue(DstLV, SourceLocation()).getScalarVal();
}
      CapturedVars.push_back(CV);
} else {
      assert(CurCap->capturesVariable() && "Expected capture by reference.");
      CapturedVars.push_back(EmitLValue(*I).getAddress().getPointer());
}
}
}
static Address castValueFromUintptr(CodeGenFunction &CGF, QualType DstType,
                                    StringRef Name, LValue AddrLV,
                                    bool isReferenceType = false) {
  ASTContext &Ctx = CGF.getContext();
  auto *CastedPtr = CGF.EmitScalarConversion(
      AddrLV.getAddress().getPointer(), Ctx.getUIntPtrType(),
      Ctx.getPointerType(DstType), SourceLocation());
  auto TmpAddr =
      CGF.MakeNaturalAlignAddrLValue(CastedPtr, Ctx.getPointerType(DstType))
          .getAddress();
  if (isReferenceType) {
    QualType RefType = Ctx.getLValueReferenceType(DstType);
    auto *RefVal = TmpAddr.getPointer();
    TmpAddr = CGF.CreateMemTemp(RefType, Twine(Name) + ".ref");
    auto TmpLVal = CGF.MakeAddrLValue(TmpAddr, RefType);
    CGF.EmitStoreThroughLValue(RValue::get(RefVal), TmpLVal, 
 true);
}
  return TmpAddr;
}
llvm::Function *
CodeGenFunction::GenerateOpenMPCapturedStmtFunction(const CapturedStmt &S) {
  assert(
      CapturedStmtInfo &&
      "CapturedStmtInfo should be set when generating the captured function");
  const CapturedDecl *CD = S.getCapturedDecl();
  const RecordDecl *RD = S.getCapturedRecordDecl();
  assert(CD->hasBody() && "missing CapturedDecl body");
  ASTContext &Ctx = CGM.getContext();
  FunctionArgList Args;
  Args.append(CD->param_begin(),
              std::next(CD->param_begin(), CD->getContextParamPosition()));
  auto I = S.captures().begin();
for_loop_talkad7420();
  for (auto *FD : RD->fields()) {
    QualType ArgType = FD->getType();
    IdentifierInfo *II = nullptr;
    VarDecl *CapVar = nullptr;
    if ((I->capturesVariableByCopy() && !ArgType->isAnyPointerType()) ||
        I->capturesVariableArrayType())
      ArgType = Ctx.getUIntPtrType();
    if (I->capturesVariable() || I->capturesVariableByCopy()) {
      CapVar = I->getCapturedVar();
      II = CapVar->getIdentifier();
} else if (I->capturesThis())
      II = &getContext().Idents.get("this");
    else {
      assert(I->capturesVariableArrayType());
      II = &getContext().Idents.get("vla");
}
    if (ArgType->isVariablyModifiedType()) {
      bool IsReference = ArgType->isLValueReferenceType();
      ArgType =
          getContext().getCanonicalParamType(ArgType.getNonReferenceType());
      if (IsReference && !ArgType->isPointerType()) {
        ArgType = getContext().getLValueReferenceType(
            ArgType, 
false);
}
}
    Args.push_back(ImplicitParamDecl::Create(getContext(), nullptr,
                                             FD->getLocation(), II, ArgType));
    ++I;
}
  Args.append(
      std::next(CD->param_begin(), CD->getContextParamPosition() + 1),
      CD->param_end());
  FunctionType::ExtInfo ExtInfo;
  const CGFunctionInfo &FuncInfo =
      CGM.getTypes().arrangeBuiltinFunctionDeclaration(Ctx.VoidTy, Args);
  llvm::FunctionType *FuncLLVMTy = CGM.getTypes().GetFunctionType(FuncInfo);
  llvm::Function *F = llvm::Function::Create(
      FuncLLVMTy, llvm::GlobalValue::InternalLinkage,
      CapturedStmtInfo->getHelperName(), &CGM.getModule());
  CGM.SetInternalFunctionAttributes(CD, F, FuncInfo);
  if (CD->isNothrow())
    F->addFnAttr(llvm::Attribute::NoUnwind);
  StartFunction(CD, Ctx.VoidTy, F, FuncInfo, Args, CD->getLocation(),
                CD->getBody()->getLocStart());
  unsigned Cnt = CD->getContextParamPosition();
  I = S.captures().begin();
for_loop_talkad7420();
  for (auto *FD : RD->fields()) {
    if (I->capturesVariableByCopy() && FD->getType()->isAnyPointerType()) {
      const VarDecl *CurVD = I->getCapturedVar();
      Address LocalAddr = GetAddrOfLocalVar(Args[Cnt]);
      if (CurVD->getType()->isReferenceType()) {
        Address RefAddr = CreateMemTemp(CurVD->getType(), getPointerAlign(),
                                        ".materialized_ref");
        EmitStoreOfScalar(LocalAddr.getPointer(), RefAddr, 
false,
                          CurVD->getType());
        LocalAddr = RefAddr;
}
      setAddrOfLocalVar(CurVD, LocalAddr);
      ++Cnt;
      ++I;
      continue;
}
    LValue ArgLVal =
        MakeAddrLValue(GetAddrOfLocalVar(Args[Cnt]), Args[Cnt]->getType(),
                       AlignmentSource::Decl);
    if (FD->hasCapturedVLAType()) {
      LValue CastedArgLVal =
          MakeAddrLValue(castValueFromUintptr(*this, FD->getType(),
                                              Args[Cnt]->getName(), ArgLVal),
                         FD->getType(), AlignmentSource::Decl);
      auto *ExprArg =
          EmitLoadOfLValue(CastedArgLVal, SourceLocation()).getScalarVal();
      auto VAT = FD->getCapturedVLAType();
      VLASizeMap[VAT->getSizeExpr()] = ExprArg;
} else if (I->capturesVariable()) {
      auto *Var = I->getCapturedVar();
      QualType VarTy = Var->getType();
      Address ArgAddr = ArgLVal.getAddress();
      if (!VarTy->isReferenceType()) {
        if (ArgLVal.getType()->isLValueReferenceType()) {
          ArgAddr = EmitLoadOfReference(
              ArgAddr, ArgLVal.getType()->castAs<ReferenceType>());
} else if (!VarTy->isVariablyModifiedType() || !VarTy->isPointerType()) {
          assert(ArgLVal.getType()->isPointerType());
          ArgAddr = EmitLoadOfPointer(
              ArgAddr, ArgLVal.getType()->castAs<PointerType>());
}
}
      setAddrOfLocalVar(
          Var, Address(ArgAddr.getPointer(), getContext().getDeclAlign(Var)));
} else if (I->capturesVariableByCopy()) {
      assert(!FD->getType()->isAnyPointerType() &&
             "Not expecting a captured pointer.");
      auto *Var = I->getCapturedVar();
      QualType VarTy = Var->getType();
      setAddrOfLocalVar(Var, castValueFromUintptr(*this, FD->getType(),
                                                  Args[Cnt]->getName(), ArgLVal,
                                                  VarTy->isReferenceType()));
} else {
      assert(I->capturesThis());
      CXXThisValue =
          EmitLoadOfLValue(ArgLVal, Args[Cnt]->getLocation()).getScalarVal();
}
    ++Cnt;
    ++I;
}
  PGO.assignRegionCounters(GlobalDecl(CD), F);
  CapturedStmtInfo->EmitBody(*this, CD->getBody());
  FinishFunction(CD->getBodyRBrace());
  return F;
}
void CodeGenFunction::EmitOMPAggregateAssign(
    Address DestAddr, Address SrcAddr, QualType OriginalType,
    const llvm::function_ref<void(Address, Address)> &CopyGen) {
  QualType ElementTy;
  auto ArrayTy = OriginalType->getAsArrayTypeUnsafe();
  auto NumElements = emitArrayLength(ArrayTy, ElementTy, DestAddr);
  SrcAddr = Builder.CreateElementBitCast(SrcAddr, DestAddr.getElementType());
  auto SrcBegin = SrcAddr.getPointer();
  auto DestBegin = DestAddr.getPointer();
  auto DestEnd = Builder.CreateGEP(DestBegin, NumElements);
  auto BodyBB = createBasicBlock("omp.arraycpy.body");
  auto DoneBB = createBasicBlock("omp.arraycpy.done");
  auto IsEmpty =
      Builder.CreateICmpEQ(DestBegin, DestEnd, "omp.arraycpy.isempty");
  Builder.CreateCondBr(IsEmpty, DoneBB, BodyBB);
  auto EntryBB = Builder.GetInsertBlock();
  EmitBlock(BodyBB);
  CharUnits ElementSize = getContext().getTypeSizeInChars(ElementTy);
  llvm::PHINode *SrcElementPHI =
    Builder.CreatePHI(SrcBegin->getType(), 2, "omp.arraycpy.srcElementPast");
  SrcElementPHI->addIncoming(SrcBegin, EntryBB);
  Address SrcElementCurrent =
      Address(SrcElementPHI,
              SrcAddr.getAlignment().alignmentOfArrayElement(ElementSize));
  llvm::PHINode *DestElementPHI =
    Builder.CreatePHI(DestBegin->getType(), 2, "omp.arraycpy.destElementPast");
  DestElementPHI->addIncoming(DestBegin, EntryBB);
  Address DestElementCurrent =
    Address(DestElementPHI,
            DestAddr.getAlignment().alignmentOfArrayElement(ElementSize));
  CopyGen(DestElementCurrent, SrcElementCurrent);
  auto DestElementNext = Builder.CreateConstGEP1_32(
      DestElementPHI, 
1, "omp.arraycpy.dest.element");
  auto SrcElementNext = Builder.CreateConstGEP1_32(
      SrcElementPHI, 
1, "omp.arraycpy.src.element");
  auto Done =
      Builder.CreateICmpEQ(DestElementNext, DestEnd, "omp.arraycpy.done");
  Builder.CreateCondBr(Done, DoneBB, BodyBB);
  DestElementPHI->addIncoming(DestElementNext, Builder.GetInsertBlock());
  SrcElementPHI->addIncoming(SrcElementNext, Builder.GetInsertBlock());
  EmitBlock(DoneBB, 
true);
}
static const OMPDeclareReductionDecl *
getReductionInit(const Expr *ReductionOp) {
  if (auto *CE = dyn_cast<CallExpr>(ReductionOp))
    if (auto *OVE = dyn_cast<OpaqueValueExpr>(CE->getCallee()))
      if (auto *DRE =
              dyn_cast<DeclRefExpr>(OVE->getSourceExpr()->IgnoreImpCasts()))
        if (auto *DRD = dyn_cast<OMPDeclareReductionDecl>(DRE->getDecl()))
          return DRD;
  return nullptr;
}
static void emitInitWithReductionInitializer(CodeGenFunction &CGF,
                                             const OMPDeclareReductionDecl *DRD,
                                             const Expr *InitOp,
                                             Address Private, Address Original,
                                             QualType Ty) {
  if (DRD->getInitializer()) {
    std::pair<llvm::Function *, llvm::Function *> Reduction =
        CGF.CGM.getOpenMPRuntime().getUserDefinedReduction(DRD);
    auto *CE = cast<CallExpr>(InitOp);
    auto *OVE = cast<OpaqueValueExpr>(CE->getCallee());
    const Expr *LHS = CE->getArg(
0)->IgnoreParenImpCasts();
    const Expr *RHS = CE->getArg(
1)->IgnoreParenImpCasts();
    auto *LHSDRE = cast<DeclRefExpr>(cast<UnaryOperator>(LHS)->getSubExpr());
    auto *RHSDRE = cast<DeclRefExpr>(cast<UnaryOperator>(RHS)->getSubExpr());
    CodeGenFunction::OMPPrivateScope PrivateScope(CGF);
    PrivateScope.addPrivate(cast<VarDecl>(LHSDRE->getDecl()),
                            [=]() -> Address { return Private; 
});
    PrivateScope.addPrivate(cast<VarDecl>(RHSDRE->getDecl()),
                            [=]() -> Address { return Original; 
});
    (void)PrivateScope.Privatize();
    RValue Func = RValue::get(Reduction.second);
    CodeGenFunction::OpaqueValueMapping Map(CGF, OVE, Func);
    CGF.EmitIgnoredExpr(InitOp);
} else {
    llvm::Constant *Init = CGF.CGM.EmitNullConstant(Ty);
    auto *GV = new llvm::GlobalVariable(
        CGF.CGM.getModule(), Init->getType(), 
true,
        llvm::GlobalValue::PrivateLinkage, Init, ".init");
    LValue LV = CGF.MakeNaturalAlignAddrLValue(GV, Ty);
    RValue InitRVal;
    switch (CGF.getEvaluationKind(Ty)) {
    case TEK_Scalar:
      InitRVal = CGF.EmitLoadOfLValue(LV, SourceLocation());
      break;
    case TEK_Complex:
      InitRVal =
          RValue::getComplex(CGF.EmitLoadOfComplex(LV, SourceLocation()));
      break;
    case TEK_Aggregate:
      InitRVal = RValue::getAggregate(LV.getAddress());
      break;
}
    OpaqueValueExpr OVE(SourceLocation(), Ty, VK_RValue);
    CodeGenFunction::OpaqueValueMapping OpaqueMap(CGF, &OVE, InitRVal);
    CGF.EmitAnyExprToMem(&OVE, Private, Ty.getQualifiers(),
false);
}
}
static void EmitOMPAggregateInit(CodeGenFunction &CGF, Address DestAddr,
                                 QualType Type, const Expr *Init,
                                 Address SrcAddr = Address::invalid()) {
  auto *DRD = getReductionInit(Init);
  QualType ElementTy;
  auto ArrayTy = Type->getAsArrayTypeUnsafe();
  auto NumElements = CGF.emitArrayLength(ArrayTy, ElementTy, DestAddr);
  DestAddr =
      CGF.Builder.CreateElementBitCast(DestAddr, DestAddr.getElementType());
  if (DRD)
    SrcAddr =
        CGF.Builder.CreateElementBitCast(SrcAddr, DestAddr.getElementType());
  llvm::Value *SrcBegin = nullptr;
  if (DRD)
    SrcBegin = SrcAddr.getPointer();
  auto DestBegin = DestAddr.getPointer();
  auto DestEnd = CGF.Builder.CreateGEP(DestBegin, NumElements);
  auto BodyBB = CGF.createBasicBlock("omp.arrayinit.body");
  auto DoneBB = CGF.createBasicBlock("omp.arrayinit.done");
  auto IsEmpty =
      CGF.Builder.CreateICmpEQ(DestBegin, DestEnd, "omp.arrayinit.isempty");
  CGF.Builder.CreateCondBr(IsEmpty, DoneBB, BodyBB);
  auto EntryBB = CGF.Builder.GetInsertBlock();
  CGF.EmitBlock(BodyBB);
  CharUnits ElementSize = CGF.getContext().getTypeSizeInChars(ElementTy);
  llvm::PHINode *SrcElementPHI = nullptr;
  Address SrcElementCurrent = Address::invalid();
  if (DRD) {
    SrcElementPHI = CGF.Builder.CreatePHI(SrcBegin->getType(), 2,
                                          "omp.arraycpy.srcElementPast");
    SrcElementPHI->addIncoming(SrcBegin, EntryBB);
    SrcElementCurrent =
        Address(SrcElementPHI,
                SrcAddr.getAlignment().alignmentOfArrayElement(ElementSize));
}
  llvm::PHINode *DestElementPHI = CGF.Builder.CreatePHI(
      DestBegin->getType(), 2, "omp.arraycpy.destElementPast");
  DestElementPHI->addIncoming(DestBegin, EntryBB);
  Address DestElementCurrent =
      Address(DestElementPHI,
              DestAddr.getAlignment().alignmentOfArrayElement(ElementSize));
  {
    CodeGenFunction::RunCleanupsScope InitScope(CGF);
    if (DRD && (DRD->getInitializer() || !Init)) {
      emitInitWithReductionInitializer(CGF, DRD, Init, DestElementCurrent,
                                       SrcElementCurrent, ElementTy);
} else
      CGF.EmitAnyExprToMem(Init, DestElementCurrent, ElementTy.getQualifiers(),
false);
}
  if (DRD) {
    auto SrcElementNext = CGF.Builder.CreateConstGEP1_32(
        SrcElementPHI, 
1, "omp.arraycpy.dest.element");
    SrcElementPHI->addIncoming(SrcElementNext, CGF.Builder.GetInsertBlock());
}
  auto DestElementNext = CGF.Builder.CreateConstGEP1_32(
      DestElementPHI, 
1, "omp.arraycpy.dest.element");
  auto Done =
      CGF.Builder.CreateICmpEQ(DestElementNext, DestEnd, "omp.arraycpy.done");
  CGF.Builder.CreateCondBr(Done, DoneBB, BodyBB);
  DestElementPHI->addIncoming(DestElementNext, CGF.Builder.GetInsertBlock());
  CGF.EmitBlock(DoneBB, 
true);
}
void CodeGenFunction::EmitOMPCopy(QualType OriginalType, Address DestAddr,
                                  Address SrcAddr, const VarDecl *DestVD,
                                  const VarDecl *SrcVD, const Expr *Copy) {
  if (OriginalType->isArrayType()) {
    auto *BO = dyn_cast<BinaryOperator>(Copy);
    if (BO && BO->getOpcode() == BO_Assign) {
      EmitAggregateAssign(DestAddr, SrcAddr, OriginalType);
} else {
      EmitOMPAggregateAssign(
          DestAddr, SrcAddr, OriginalType,
          [this, Copy, SrcVD, DestVD](Address DestElement, Address SrcElement) {
            CodeGenFunction::OMPPrivateScope Remap(*this);
            Remap.addPrivate(DestVD, [DestElement]() -> Address {
              return DestElement;
});
            Remap.addPrivate(
                SrcVD, [SrcElement]() -> Address { return SrcElement; 
});
            (void)Remap.Privatize();
            EmitIgnoredExpr(Copy);
});
}
} else {
    CodeGenFunction::OMPPrivateScope Remap(*this);
    Remap.addPrivate(SrcVD, [SrcAddr]() -> Address { return SrcAddr; 
});
    Remap.addPrivate(DestVD, [DestAddr]() -> Address { return DestAddr; 
});
    (void)Remap.Privatize();
    EmitIgnoredExpr(Copy);
}
}
bool CodeGenFunction::EmitOMPFirstprivateClause(const OMPExecutableDirective &D,
                                                OMPPrivateScope &PrivateScope) {
  if (!HaveInsertPoint())
    return false;
  bool FirstprivateIsLastprivate = false;
  llvm::DenseSet<const VarDecl *> Lastprivates;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLastprivateClause>()) {
for_loop_talkad7420();
    for (const auto *D : C->varlists())
{
      Lastprivates.insert(
          cast<VarDecl>(cast<DeclRefExpr>(D)->getDecl())->getCanonicalDecl());
}
}
  llvm::DenseSet<const VarDecl *> EmittedAsFirstprivate;
  CGCapturedStmtInfo CapturesInfo(cast<CapturedStmt>(*D.getAssociatedStmt()));
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPFirstprivateClause>()) {
    auto IRef = C->varlist_begin();
    auto InitsRef = C->inits().begin();
for_loop_talkad7420();
    for (auto IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      bool ThisFirstprivateIsLastprivate =
          Lastprivates.count(OrigVD->getCanonicalDecl()) > 0;
      auto *CapFD = CapturesInfo.lookup(OrigVD);
      auto *FD = CapturedStmtInfo->lookup(OrigVD);
      if (!ThisFirstprivateIsLastprivate && FD && (FD == CapFD) &&
          !FD->getType()->isReferenceType()) {
        EmittedAsFirstprivate.insert(OrigVD->getCanonicalDecl());
        ++IRef;
        ++InitsRef;
        continue;
}
      FirstprivateIsLastprivate =
          FirstprivateIsLastprivate || ThisFirstprivateIsLastprivate;
      if (EmittedAsFirstprivate.insert(OrigVD->getCanonicalDecl()).second) {
        auto *VD = cast<VarDecl>(cast<DeclRefExpr>(IInit)->getDecl());
        auto *VDInit = cast<VarDecl>(cast<DeclRefExpr>(*InitsRef)->getDecl());
        bool IsRegistered;
        DeclRefExpr DRE(const_cast<VarDecl *>(OrigVD),
FD != nullptr,
                        (*IRef)->getType(), VK_LValue, (*IRef)->getExprLoc());
        Address OriginalAddr = EmitLValue(&DRE).getAddress();
        QualType Type = VD->getType();
        if (Type->isArrayType()) {
          IsRegistered = PrivateScope.addPrivate(OrigVD, [&]() -> Address {
            auto Emission = EmitAutoVarAlloca(*VD);
            auto *Init = VD->getInit();
            if (!isa<CXXConstructExpr>(Init) || isTrivialInitializer(Init)) {
              EmitAggregateAssign(Emission.getAllocatedAddress(), OriginalAddr,
                                  Type);
} else {
              EmitOMPAggregateAssign(
                  Emission.getAllocatedAddress(), OriginalAddr, Type,
                  [this, VDInit, Init](Address DestElement,
                                       Address SrcElement) {
                    RunCleanupsScope InitScope(*this);
                    setAddrOfLocalVar(VDInit, SrcElement);
                    EmitAnyExprToMem(Init, DestElement,
                                     Init->getType().getQualifiers(),
 false);
                    LocalDeclMap.erase(VDInit);
});
}
            EmitAutoVarCleanups(Emission);
            return Emission.getAllocatedAddress();
});
} else {
          IsRegistered = PrivateScope.addPrivate(OrigVD, [&]() -> Address {
            setAddrOfLocalVar(VDInit, OriginalAddr);
            EmitDecl(*VD);
            LocalDeclMap.erase(VDInit);
            return GetAddrOfLocalVar(VD);
});
}
        assert(IsRegistered &&
               "firstprivate var already registered as private");
        (void)IsRegistered;
}
      ++IRef;
      ++InitsRef;
}
}
  return FirstprivateIsLastprivate && !EmittedAsFirstprivate.empty();
}
void CodeGenFunction::EmitOMPPrivateClause(
    const OMPExecutableDirective &D,
    CodeGenFunction::OMPPrivateScope &PrivateScope) {
  if (!HaveInsertPoint())
    return;
  llvm::DenseSet<const VarDecl *> EmittedAsPrivate;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPPrivateClause>()) {
    auto IRef = C->varlist_begin();
for_loop_talkad7420();
    for (auto IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      if (EmittedAsPrivate.insert(OrigVD->getCanonicalDecl()).second) {
        auto VD = cast<VarDecl>(cast<DeclRefExpr>(IInit)->getDecl());
        bool IsRegistered =
            PrivateScope.addPrivate(OrigVD, [&]() -> Address {
              EmitDecl(*VD);
              return GetAddrOfLocalVar(VD);
});
        assert(IsRegistered && "private var already registered as private");
        (void)IsRegistered;
}
      ++IRef;
}
}
}
bool CodeGenFunction::EmitOMPCopyinClause(const OMPExecutableDirective &D) {
  if (!HaveInsertPoint())
    return false;
  llvm::DenseSet<const VarDecl *> CopiedVars;
  llvm::BasicBlock *CopyBegin = nullptr, *CopyEnd = nullptr;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPCopyinClause>()) {
    auto IRef = C->varlist_begin();
    auto ISrcRef = C->source_exprs().begin();
    auto IDestRef = C->destination_exprs().begin();
for_loop_talkad7420();
    for (auto *AssignOp : C->assignment_ops()) {
      auto *VD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      QualType Type = VD->getType();
      if (CopiedVars.insert(VD->getCanonicalDecl()).second) {
        Address MasterAddr = Address::invalid();
        if (getLangOpts().OpenMPUseTLS &&
            getContext().getTargetInfo().isTLSSupported()) {
          assert(CapturedStmtInfo->lookup(VD) &&
                 "Copyin threadprivates should have been captured!");
          DeclRefExpr DRE(const_cast<VarDecl *>(VD), true, (*IRef)->getType(),
                          VK_LValue, (*IRef)->getExprLoc());
          MasterAddr = EmitLValue(&DRE).getAddress();
          LocalDeclMap.erase(VD);
} else {
          MasterAddr =
            Address(VD->isStaticLocal() ? CGM.getStaticLocalDeclAddress(VD)
                                        : CGM.GetAddrOfGlobal(VD),
                    getContext().getDeclAlign(VD));
}
        Address PrivateAddr = EmitLValue(*IRef).getAddress();
        if (CopiedVars.size() == 1) {
          CopyBegin = createBasicBlock("copyin.not.master");
          CopyEnd = createBasicBlock("copyin.not.master.end");
          Builder.CreateCondBr(
              Builder.CreateICmpNE(
                  Builder.CreatePtrToInt(MasterAddr.getPointer(), CGM.IntPtrTy),
                  Builder.CreatePtrToInt(PrivateAddr.getPointer(), CGM.IntPtrTy)),
              CopyBegin, CopyEnd);
          EmitBlock(CopyBegin);
}
        auto *SrcVD = cast<VarDecl>(cast<DeclRefExpr>(*ISrcRef)->getDecl());
        auto *DestVD = cast<VarDecl>(cast<DeclRefExpr>(*IDestRef)->getDecl());
        EmitOMPCopy(Type, PrivateAddr, MasterAddr, DestVD, SrcVD, AssignOp);
}
      ++IRef;
      ++ISrcRef;
      ++IDestRef;
}
}
  if (CopyEnd) {
    EmitBlock(CopyEnd, 
true);
    return true;
}
  return false;
}
bool CodeGenFunction::EmitOMPLastprivateClauseInit(
    const OMPExecutableDirective &D, OMPPrivateScope &PrivateScope) {
  if (!HaveInsertPoint())
    return false;
  bool HasAtLeastOneLastprivate = false;
  llvm::DenseSet<const VarDecl *> SIMDLCVs;
  if (isOpenMPSimdDirective(D.getDirectiveKind())) {
    auto *LoopDirective = cast<OMPLoopDirective>(&D);
for_loop_talkad7420();
    for (auto *C : LoopDirective->counters()) {
      SIMDLCVs.insert(
          cast<VarDecl>(cast<DeclRefExpr>(C)->getDecl())->getCanonicalDecl());
}
}
  llvm::DenseSet<const VarDecl *> AlreadyEmittedVars;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLastprivateClause>()) {
    HasAtLeastOneLastprivate = true;
    if (isOpenMPTaskLoopDirective(D.getDirectiveKind()))
      break;
    auto IRef = C->varlist_begin();
    auto IDestRef = C->destination_exprs().begin();
for_loop_talkad7420();
    for (auto *IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      if (AlreadyEmittedVars.insert(OrigVD->getCanonicalDecl()).second) {
        auto *DestVD = cast<VarDecl>(cast<DeclRefExpr>(*IDestRef)->getDecl());
        PrivateScope.addPrivate(DestVD, [this, OrigVD, IRef]() -> Address {
          DeclRefExpr DRE(
              const_cast<VarDecl *>(OrigVD),
CapturedStmtInfo->lookup(
                  OrigVD) != nullptr,
              (*IRef)->getType(), VK_LValue, (*IRef)->getExprLoc());
          return EmitLValue(&DRE).getAddress();
});
        if (IInit && !SIMDLCVs.count(OrigVD->getCanonicalDecl())) {
          auto *VD = cast<VarDecl>(cast<DeclRefExpr>(IInit)->getDecl());
          bool IsRegistered = PrivateScope.addPrivate(OrigVD, [&]() -> Address {
            EmitDecl(*VD);
            return GetAddrOfLocalVar(VD);
});
          assert(IsRegistered &&
                 "lastprivate var already registered as private");
          (void)IsRegistered;
}
}
      ++IRef;
      ++IDestRef;
}
}
  return HasAtLeastOneLastprivate;
}
void CodeGenFunction::EmitOMPLastprivateClauseFinal(
    const OMPExecutableDirective &D, bool NoFinals,
    llvm::Value *IsLastIterCond) {
  if (!HaveInsertPoint())
    return;
  llvm::BasicBlock *ThenBB = nullptr;
  llvm::BasicBlock *DoneBB = nullptr;
  if (IsLastIterCond) {
    ThenBB = createBasicBlock(".omp.lastprivate.then");
    DoneBB = createBasicBlock(".omp.lastprivate.done");
    Builder.CreateCondBr(IsLastIterCond, ThenBB, DoneBB);
    EmitBlock(ThenBB);
}
  llvm::DenseSet<const VarDecl *> AlreadyEmittedVars;
  llvm::DenseMap<const VarDecl *, const Expr *> LoopCountersAndUpdates;
  if (auto *LoopDirective = dyn_cast<OMPLoopDirective>(&D)) {
    auto IC = LoopDirective->counters().begin();
for_loop_talkad7420();
    for (auto F : LoopDirective->finals()) {
      auto *D =
          cast<VarDecl>(cast<DeclRefExpr>(*IC)->getDecl())->getCanonicalDecl();
      if (NoFinals)
        AlreadyEmittedVars.insert(D);
      else
        LoopCountersAndUpdates[D] = F;
      ++IC;
}
}
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLastprivateClause>()) {
    auto IRef = C->varlist_begin();
    auto ISrcRef = C->source_exprs().begin();
    auto IDestRef = C->destination_exprs().begin();
for_loop_talkad7420();
    for (auto *AssignOp : C->assignment_ops()) {
      auto *PrivateVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      QualType Type = PrivateVD->getType();
      auto *CanonicalVD = PrivateVD->getCanonicalDecl();
      if (AlreadyEmittedVars.insert(CanonicalVD).second) {
        if (auto *FinalExpr = LoopCountersAndUpdates.lookup(CanonicalVD))
          EmitIgnoredExpr(FinalExpr);
        auto *SrcVD = cast<VarDecl>(cast<DeclRefExpr>(*ISrcRef)->getDecl());
        auto *DestVD = cast<VarDecl>(cast<DeclRefExpr>(*IDestRef)->getDecl());
        Address OriginalAddr = GetAddrOfLocalVar(DestVD);
        Address PrivateAddr = GetAddrOfLocalVar(PrivateVD);
        if (auto RefTy = PrivateVD->getType()->getAs<ReferenceType>())
          PrivateAddr =
              Address(Builder.CreateLoad(PrivateAddr),
                      getNaturalTypeAlignment(RefTy->getPointeeType()));
        EmitOMPCopy(Type, OriginalAddr, PrivateAddr, DestVD, SrcVD, AssignOp);
}
      ++IRef;
      ++ISrcRef;
      ++IDestRef;
}
    if (auto *PostUpdate = C->getPostUpdateExpr())
      EmitIgnoredExpr(PostUpdate);
}
  if (IsLastIterCond)
    EmitBlock(DoneBB, 
true);
}
static Address castToBase(CodeGenFunction &CGF, QualType BaseTy, QualType ElTy,
                          LValue BaseLV, llvm::Value *Addr) {
  Address Tmp = Address::invalid();
  Address TopTmp = Address::invalid();
  Address MostTopTmp = Address::invalid();
  BaseTy = BaseTy.getNonReferenceType();
  while ((BaseTy->isPointerType() || BaseTy->isReferenceType()) &&
         !CGF.getContext().hasSameType(BaseTy, ElTy)) {
    Tmp = CGF.CreateMemTemp(BaseTy);
    if (TopTmp.isValid())
      CGF.Builder.CreateStore(Tmp.getPointer(), TopTmp);
    else
      MostTopTmp = Tmp;
    TopTmp = Tmp;
    BaseTy = BaseTy->getPointeeType();
}
  llvm::Type *Ty = BaseLV.getPointer()->getType();
  if (Tmp.isValid())
    Ty = Tmp.getElementType();
  Addr = CGF.Builder.CreatePointerBitCastOrAddrSpaceCast(Addr, Ty);
  if (Tmp.isValid()) {
    CGF.Builder.CreateStore(Addr, Tmp);
    return MostTopTmp;
}
  return Address(Addr, BaseLV.getAlignment());
}
static LValue loadToBegin(CodeGenFunction &CGF, QualType BaseTy, QualType ElTy,
                          LValue BaseLV) {
  BaseTy = BaseTy.getNonReferenceType();
  while ((BaseTy->isPointerType() || BaseTy->isReferenceType()) &&
         !CGF.getContext().hasSameType(BaseTy, ElTy)) {
    if (auto *PtrTy = BaseTy->getAs<PointerType>())
      BaseLV = CGF.EmitLoadOfPointerLValue(BaseLV.getAddress(), PtrTy);
    else {
      BaseLV = CGF.EmitLoadOfReferenceLValue(BaseLV.getAddress(),
                                             BaseTy->castAs<ReferenceType>());
}
    BaseTy = BaseTy->getPointeeType();
}
  return CGF.MakeAddrLValue(
      Address(
          CGF.Builder.CreatePointerBitCastOrAddrSpaceCast(
              BaseLV.getPointer(), CGF.ConvertTypeForMem(ElTy)->getPointerTo()),
          BaseLV.getAlignment()),
      BaseLV.getType(), BaseLV.getAlignmentSource());
}
void CodeGenFunction::EmitOMPReductionClauseInit(
    const OMPExecutableDirective &D,
    CodeGenFunction::OMPPrivateScope &PrivateScope) {
  if (!HaveInsertPoint())
    return;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPReductionClause>()) {
    auto ILHS = C->lhs_exprs().begin();
    auto IRHS = C->rhs_exprs().begin();
    auto IPriv = C->privates().begin();
    auto IRed = C->reduction_ops().begin();
for_loop_talkad7420();
    for (auto IRef : C->varlists()) {
      auto *LHSVD = cast<VarDecl>(cast<DeclRefExpr>(*ILHS)->getDecl());
      auto *RHSVD = cast<VarDecl>(cast<DeclRefExpr>(*IRHS)->getDecl());
      auto *PrivateVD = cast<VarDecl>(cast<DeclRefExpr>(*IPriv)->getDecl());
      auto *DRD = getReductionInit(*IRed);
      if (auto *OASE = dyn_cast<OMPArraySectionExpr>(IRef)) {
        auto *Base = OASE->getBase()->IgnoreParenImpCasts();
        while (auto *TempOASE = dyn_cast<OMPArraySectionExpr>(Base))
          Base = TempOASE->getBase()->IgnoreParenImpCasts();
        while (auto *TempASE = dyn_cast<ArraySubscriptExpr>(Base))
          Base = TempASE->getBase()->IgnoreParenImpCasts();
        auto *DE = cast<DeclRefExpr>(Base);
        auto *OrigVD = cast<VarDecl>(DE->getDecl());
        auto OASELValueLB = EmitOMPArraySectionExpr(OASE);
        auto OASELValueUB =
            EmitOMPArraySectionExpr(OASE, 
false);
        auto OriginalBaseLValue = EmitLValue(DE);
        LValue BaseLValue =
            loadToBegin(*this, OrigVD->getType(), OASELValueLB.getType(),
                        OriginalBaseLValue);
        PrivateScope.addPrivate(LHSVD, [this, OASELValueLB]() -> Address {
          return OASELValueLB.getAddress();
});
        bool IsRegistered = PrivateScope.addPrivate(
            OrigVD, [this, OrigVD, PrivateVD, BaseLValue, OASELValueLB,
                     OASELValueUB, OriginalBaseLValue, DRD, IRed]() -> Address {
              auto *Size = Builder.CreatePtrDiff(OASELValueUB.getPointer(),
                                                 OASELValueLB.getPointer());
              Size = Builder.CreateNUWAdd(
                  Size, llvm::ConstantInt::get(Size->getType(), 
1));
              CodeGenFunction::OpaqueValueMapping OpaqueMap(
                  *this, cast<OpaqueValueExpr>(
                             getContext()
                                 .getAsVariableArrayType(PrivateVD->getType())
                                 ->getSizeExpr()),
                  RValue::get(Size));
              EmitVariablyModifiedType(PrivateVD->getType());
              auto Emission = EmitAutoVarAlloca(*PrivateVD);
              auto Addr = Emission.getAllocatedAddress();
              auto *Init = PrivateVD->getInit();
              EmitOMPAggregateInit(*this, Addr, PrivateVD->getType(),
                                   DRD ? *IRed : Init,
                                   OASELValueLB.getAddress());
              EmitAutoVarCleanups(Emission);
              auto *Offset = Builder.CreatePtrDiff(BaseLValue.getPointer(),
                                                   OASELValueLB.getPointer());
              auto *Ptr = Builder.CreateGEP(Addr.getPointer(), Offset);
              return castToBase(*this, OrigVD->getType(),
                                OASELValueLB.getType(), OriginalBaseLValue,
                                Ptr);
});
        assert(IsRegistered && "private var already registered as private");
        (void)IsRegistered;
        PrivateScope.addPrivate(RHSVD, [this, PrivateVD]() -> Address {
          return GetAddrOfLocalVar(PrivateVD);
});
} else if (auto *ASE = dyn_cast<ArraySubscriptExpr>(IRef)) {
        auto *Base = ASE->getBase()->IgnoreParenImpCasts();
        while (auto *TempASE = dyn_cast<ArraySubscriptExpr>(Base))
          Base = TempASE->getBase()->IgnoreParenImpCasts();
        auto *DE = cast<DeclRefExpr>(Base);
        auto *OrigVD = cast<VarDecl>(DE->getDecl());
        auto ASELValue = EmitLValue(ASE);
        auto OriginalBaseLValue = EmitLValue(DE);
        LValue BaseLValue = loadToBegin(
            *this, OrigVD->getType(), ASELValue.getType(), OriginalBaseLValue);
        PrivateScope.addPrivate(LHSVD, [this, ASELValue]() -> Address {
          return ASELValue.getAddress();
});
        bool IsRegistered = PrivateScope.addPrivate(
            OrigVD, [this, OrigVD, PrivateVD, BaseLValue, ASELValue,
                     OriginalBaseLValue, DRD, IRed]() -> Address {
              AutoVarEmission Emission = EmitAutoVarAlloca(*PrivateVD);
              auto Addr = Emission.getAllocatedAddress();
              if (DRD && (DRD->getInitializer() || !PrivateVD->hasInit())) {
                emitInitWithReductionInitializer(*this, DRD, *IRed, Addr,
                                                 ASELValue.getAddress(),
                                                 ASELValue.getType());
} else
                EmitAutoVarInit(Emission);
              EmitAutoVarCleanups(Emission);
              auto *Offset = Builder.CreatePtrDiff(BaseLValue.getPointer(),
                                                   ASELValue.getPointer());
              auto *Ptr = Builder.CreateGEP(Addr.getPointer(), Offset);
              return castToBase(*this, OrigVD->getType(), ASELValue.getType(),
                                OriginalBaseLValue, Ptr);
});
        assert(IsRegistered && "private var already registered as private");
        (void)IsRegistered;
        PrivateScope.addPrivate(RHSVD, [this, PrivateVD, RHSVD]() -> Address {
          return Builder.CreateElementBitCast(
              GetAddrOfLocalVar(PrivateVD), ConvertTypeForMem(RHSVD->getType()),
              "rhs.begin");
});
} else {
        auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(IRef)->getDecl());
        QualType Type = PrivateVD->getType();
        if (getContext().getAsArrayType(Type)) {
          DeclRefExpr DRE(const_cast<VarDecl *>(OrigVD),
                          CapturedStmtInfo->lookup(OrigVD) != nullptr,
                          IRef->getType(), VK_LValue, IRef->getExprLoc());
          Address OriginalAddr = EmitLValue(&DRE).getAddress();
          PrivateScope.addPrivate(LHSVD, [this, &OriginalAddr,
                                          LHSVD]() -> Address {
            OriginalAddr = Builder.CreateElementBitCast(
                OriginalAddr, ConvertTypeForMem(LHSVD->getType()), "lhs.begin");
            return OriginalAddr;
});
          bool IsRegistered = PrivateScope.addPrivate(OrigVD, [&]() -> Address {
            if (Type->isVariablyModifiedType()) {
              CodeGenFunction::OpaqueValueMapping OpaqueMap(
                  *this, cast<OpaqueValueExpr>(
                             getContext()
                                 .getAsVariableArrayType(PrivateVD->getType())
                                 ->getSizeExpr()),
                  RValue::get(
                      getTypeSize(OrigVD->getType().getNonReferenceType())));
              EmitVariablyModifiedType(Type);
}
            auto Emission = EmitAutoVarAlloca(*PrivateVD);
            auto Addr = Emission.getAllocatedAddress();
            auto *Init = PrivateVD->getInit();
            EmitOMPAggregateInit(*this, Addr, PrivateVD->getType(),
                                 DRD ? *IRed : Init, OriginalAddr);
            EmitAutoVarCleanups(Emission);
            return Emission.getAllocatedAddress();
});
          assert(IsRegistered && "private var already registered as private");
          (void)IsRegistered;
          PrivateScope.addPrivate(RHSVD, [this, PrivateVD, RHSVD]() -> Address {
            return Builder.CreateElementBitCast(
                GetAddrOfLocalVar(PrivateVD),
                ConvertTypeForMem(RHSVD->getType()), "rhs.begin");
});
} else {
          Address OriginalAddr = Address::invalid();
          PrivateScope.addPrivate(LHSVD, [this, OrigVD, IRef,
                                          &OriginalAddr]() -> Address {
            DeclRefExpr DRE(const_cast<VarDecl *>(OrigVD),
                            CapturedStmtInfo->lookup(OrigVD) != nullptr,
                            IRef->getType(), VK_LValue, IRef->getExprLoc());
            OriginalAddr = EmitLValue(&DRE).getAddress();
            return OriginalAddr;
});
          bool IsRegistered = PrivateScope.addPrivate(
              OrigVD, [this, PrivateVD, OriginalAddr, DRD, IRed]() -> Address {
                AutoVarEmission Emission = EmitAutoVarAlloca(*PrivateVD);
                auto Addr = Emission.getAllocatedAddress();
                if (DRD && (DRD->getInitializer() || !PrivateVD->hasInit())) {
                  emitInitWithReductionInitializer(*this, DRD, *IRed, Addr,
                                                   OriginalAddr,
                                                   PrivateVD->getType());
} else
                  EmitAutoVarInit(Emission);
                EmitAutoVarCleanups(Emission);
                return Addr;
});
          assert(IsRegistered && "private var already registered as private");
          (void)IsRegistered;
          PrivateScope.addPrivate(RHSVD, [this, PrivateVD]() -> Address {
            return GetAddrOfLocalVar(PrivateVD);
});
}
}
      ++ILHS;
      ++IRHS;
      ++IPriv;
      ++IRed;
}
}
}
void CodeGenFunction::EmitOMPReductionClauseFinal(
    const OMPExecutableDirective &D) {
  if (!HaveInsertPoint())
    return;
  llvm::SmallVector<const Expr *, 8> Privates;
  llvm::SmallVector<const Expr *, 8> LHSExprs;
  llvm::SmallVector<const Expr *, 8> RHSExprs;
  llvm::SmallVector<const Expr *, 8> ReductionOps;
  bool HasAtLeastOneReduction = false;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPReductionClause>()) {
    HasAtLeastOneReduction = true;
    Privates.append(C->privates().begin(), C->privates().end());
    LHSExprs.append(C->lhs_exprs().begin(), C->lhs_exprs().end());
    RHSExprs.append(C->rhs_exprs().begin(), C->rhs_exprs().end());
    ReductionOps.append(C->reduction_ops().begin(), C->reduction_ops().end());
}
  if (HasAtLeastOneReduction) {
    CGM.getOpenMPRuntime().emitReduction(
        *this, D.getLocEnd(), Privates, LHSExprs, RHSExprs, ReductionOps,
        D.getSingleClause<OMPNowaitClause>() ||
            isOpenMPParallelDirective(D.getDirectiveKind()) ||
            D.getDirectiveKind() == OMPD_simd,
        D.getDirectiveKind() == OMPD_simd);
}
}
static void emitPostUpdateForReductionClause(
    CodeGenFunction &CGF, const OMPExecutableDirective &D,
    const llvm::function_ref<llvm::Value *(CodeGenFunction &)> &CondGen) {
  if (!CGF.HaveInsertPoint())
    return;
  llvm::BasicBlock *DoneBB = nullptr;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPReductionClause>()) {
    if (auto *PostUpdate = C->getPostUpdateExpr()) {
      if (!DoneBB) {
        if (auto *Cond = CondGen(CGF)) {
          auto *ThenBB = CGF.createBasicBlock(".omp.reduction.pu");
          DoneBB = CGF.createBasicBlock(".omp.reduction.pu.done");
          CGF.Builder.CreateCondBr(Cond, ThenBB, DoneBB);
          CGF.EmitBlock(ThenBB);
}
}
      CGF.EmitIgnoredExpr(PostUpdate);
}
}
  if (DoneBB)
    CGF.EmitBlock(DoneBB, 
true);
}
static void emitCommonOMPParallelDirective(CodeGenFunction &CGF,
                                           const OMPExecutableDirective &S,
                                           OpenMPDirectiveKind InnermostKind,
                                           const RegionCodeGenTy &CodeGen) {
  auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
  auto OutlinedFn = CGF.CGM.getOpenMPRuntime().
      emitParallelOrTeamsOutlinedFunction(S,
          *CS->getCapturedDecl()->param_begin(), InnermostKind, CodeGen);
  if (const auto *NumThreadsClause = S.getSingleClause<OMPNumThreadsClause>()) {
    CodeGenFunction::RunCleanupsScope NumThreadsScope(CGF);
    auto NumThreads = CGF.EmitScalarExpr(NumThreadsClause->getNumThreads(),
 true);
    CGF.CGM.getOpenMPRuntime().emitNumThreadsClause(
        CGF, NumThreads, NumThreadsClause->getLocStart());
}
  if (const auto *ProcBindClause = S.getSingleClause<OMPProcBindClause>()) {
    CodeGenFunction::RunCleanupsScope ProcBindScope(CGF);
    CGF.CGM.getOpenMPRuntime().emitProcBindClause(
        CGF, ProcBindClause->getProcBindKind(), ProcBindClause->getLocStart());
}
  const Expr *IfCond = nullptr;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPIfClause>()) {
    if (C->getNameModifier() == OMPD_unknown ||
        C->getNameModifier() == OMPD_parallel) {
      IfCond = C->getCondition();
      break;
}
}
  OMPLexicalScope Scope(CGF, S);
  llvm::SmallVector<llvm::Value *, 16> CapturedVars;
  CGF.GenerateOpenMPCapturedVars(*CS, CapturedVars);
  CGF.CGM.getOpenMPRuntime().emitParallelCall(CGF, S.getLocStart(), OutlinedFn,
                                              CapturedVars, IfCond);
}
void CodeGenFunction::EmitOMPParallelDirective(const OMPParallelDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    OMPPrivateScope PrivateScope(CGF);
    bool Copyins = CGF.EmitOMPCopyinClause(S);
    (void)CGF.EmitOMPFirstprivateClause(S, PrivateScope);
    if (Copyins) {
      CGF.CGM.getOpenMPRuntime().emitBarrierCall(
          CGF, S.getLocStart(), OMPD_unknown, 
false,
true);
}
    CGF.EmitOMPPrivateClause(S, PrivateScope);
    CGF.EmitOMPReductionClauseInit(S, PrivateScope);
    (void)PrivateScope.Privatize();
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
    CGF.EmitOMPReductionClauseFinal(S);
};
  emitCommonOMPParallelDirective(*this, S, OMPD_parallel, CodeGen);
  emitPostUpdateForReductionClause(
      *this, S, [](CodeGenFunction &) -> llvm::Value * { return nullptr; 
});
}
void CodeGenFunction::EmitOMPLoopBody(const OMPLoopDirective &D,
                                      JumpDest LoopExit) {
  RunCleanupsScope BodyScope(*this);
for_loop_talkad7420();
  for (auto I : D.updates()) {
    EmitIgnoredExpr(I);
}
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLinearClause>()) {
for_loop_talkad7420();
    for (auto *U : C->updates())
{
      EmitIgnoredExpr(U);
}
}
  auto Continue = getJumpDestInCurrentScope("omp.body.continue");
  BreakContinueStack.push_back(BreakContinue(LoopExit, Continue));
  EmitStmt(D.getBody());
  EmitBlock(Continue.getBlock());
  BreakContinueStack.pop_back();
}
void CodeGenFunction::EmitOMPInnerLoop(
    const Stmt &S, bool RequiresCleanup, const Expr *LoopCond,
    const Expr *IncExpr,
    const llvm::function_ref<void(CodeGenFunction &)> &BodyGen,
    const llvm::function_ref<void(CodeGenFunction &)> &PostIncGen) {
  auto LoopExit = getJumpDestInCurrentScope("omp.inner.for.end");
  auto CondBlock = createBasicBlock("omp.inner.for.cond");
  EmitBlock(CondBlock);
  const SourceRange &R = S.getSourceRange();
  LoopStack.push(CondBlock, SourceLocToDebugLoc(R.getBegin()),
                 SourceLocToDebugLoc(R.getEnd()));
  auto ExitBlock = LoopExit.getBlock();
  if (RequiresCleanup)
    ExitBlock = createBasicBlock("omp.inner.for.cond.cleanup");
  auto LoopBody = createBasicBlock("omp.inner.for.body");
  EmitBranchOnBoolExpr(LoopCond, LoopBody, ExitBlock, getProfileCount(&S));
  if (ExitBlock != LoopExit.getBlock()) {
    EmitBlock(ExitBlock);
    EmitBranchThroughCleanup(LoopExit);
}
  EmitBlock(LoopBody);
  incrementProfileCounter(&S);
  auto Continue = getJumpDestInCurrentScope("omp.inner.for.inc");
  BreakContinueStack.push_back(BreakContinue(LoopExit, Continue));
  BodyGen(*this);
  EmitBlock(Continue.getBlock());
  EmitIgnoredExpr(IncExpr);
  PostIncGen(*this);
  BreakContinueStack.pop_back();
  EmitBranch(CondBlock);
  LoopStack.pop();
  EmitBlock(LoopExit.getBlock());
}
void CodeGenFunction::EmitOMPLinearClauseInit(const OMPLoopDirective &D) {
  if (!HaveInsertPoint())
    return;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLinearClause>()) {
for_loop_talkad7420();
    for (auto *Init : C->inits()) {
      auto *VD = cast<VarDecl>(cast<DeclRefExpr>(Init)->getDecl());
      if (auto *Ref = dyn_cast<DeclRefExpr>(VD->getInit()->IgnoreImpCasts())) {
        AutoVarEmission Emission = EmitAutoVarAlloca(*VD);
        auto *OrigVD = cast<VarDecl>(Ref->getDecl());
        DeclRefExpr DRE(const_cast<VarDecl *>(OrigVD),
                        CapturedStmtInfo->lookup(OrigVD) != nullptr,
                        VD->getInit()->getType(), VK_LValue,
                        VD->getInit()->getExprLoc());
        EmitExprAsInit(&DRE, VD, MakeAddrLValue(Emission.getAllocatedAddress(),
                                                VD->getType()),
false);
        EmitAutoVarCleanups(Emission);
} else
        EmitVarDecl(*VD);
}
    if (auto CS = cast_or_null<BinaryOperator>(C->getCalcStep()))
      if (auto SaveRef = cast<DeclRefExpr>(CS->getLHS())) {
        EmitVarDecl(*cast<VarDecl>(SaveRef->getDecl()));
        EmitIgnoredExpr(CS);
}
}
}
void CodeGenFunction::EmitOMPLinearClauseFinal(
    const OMPLoopDirective &D,
    const llvm::function_ref<llvm::Value *(CodeGenFunction &)> &CondGen) {
  if (!HaveInsertPoint())
    return;
  llvm::BasicBlock *DoneBB = nullptr;
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLinearClause>()) {
    auto IC = C->varlist_begin();
for_loop_talkad7420();
    for (auto *F : C->finals()) {
      if (!DoneBB) {
        if (auto *Cond = CondGen(*this)) {
          auto *ThenBB = createBasicBlock(".omp.linear.pu");
          DoneBB = createBasicBlock(".omp.linear.pu.done");
          Builder.CreateCondBr(Cond, ThenBB, DoneBB);
          EmitBlock(ThenBB);
}
}
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IC)->getDecl());
      DeclRefExpr DRE(const_cast<VarDecl *>(OrigVD),
                      CapturedStmtInfo->lookup(OrigVD) != nullptr,
                      (*IC)->getType(), VK_LValue, (*IC)->getExprLoc());
      Address OrigAddr = EmitLValue(&DRE).getAddress();
      CodeGenFunction::OMPPrivateScope VarScope(*this);
      VarScope.addPrivate(OrigVD, [OrigAddr]() -> Address { return OrigAddr; 
});
      (void)VarScope.Privatize();
      EmitIgnoredExpr(F);
      ++IC;
}
    if (auto *PostUpdate = C->getPostUpdateExpr())
      EmitIgnoredExpr(PostUpdate);
}
  if (DoneBB)
    EmitBlock(DoneBB, 
true);
}
static void emitAlignedClause(CodeGenFunction &CGF,
                              const OMPExecutableDirective &D) {
  if (!CGF.HaveInsertPoint())
    return;
for_loop_talkad7420();
  for (const auto *Clause : D.getClausesOfKind<OMPAlignedClause>()) {
    unsigned ClauseAlignment = 0;
    if (auto AlignmentExpr = Clause->getAlignment()) {
      auto AlignmentCI =
          cast<llvm::ConstantInt>(CGF.EmitScalarExpr(AlignmentExpr));
      ClauseAlignment = static_cast<unsigned>(AlignmentCI->getZExtValue());
}
for_loop_talkad7420();
    for (auto E : Clause->varlists()) {
      unsigned Alignment = ClauseAlignment;
      if (Alignment == 0) {
        Alignment =
            CGF.getContext()
                .toCharUnitsFromBits(CGF.getContext().getOpenMPDefaultSimdAlign(
                    E->getType()->getPointeeType()))
                .getQuantity();
}
      assert((Alignment == 0 || llvm::isPowerOf2_32(Alignment)) &&
             "alignment is not power of 2");
      if (Alignment != 0) {
        llvm::Value *PtrValue = CGF.EmitScalarExpr(E);
        CGF.EmitAlignmentAssumption(PtrValue, Alignment);
}
}
}
}
void CodeGenFunction::EmitOMPPrivateLoopCounters(
    const OMPLoopDirective &S, CodeGenFunction::OMPPrivateScope &LoopScope) {
  if (!HaveInsertPoint())
    return;
  auto I = S.private_counters().begin();
for_loop_talkad7420();
  for (auto *E : S.counters()) {
    auto *VD = cast<VarDecl>(cast<DeclRefExpr>(E)->getDecl());
    auto *PrivateVD = cast<VarDecl>(cast<DeclRefExpr>(*I)->getDecl());
    (void)LoopScope.addPrivate(VD, [&]() -> Address {
      if (!LocalDeclMap.count(PrivateVD)) {
        auto VarEmission = EmitAutoVarAlloca(*PrivateVD);
        EmitAutoVarCleanups(VarEmission);
}
      DeclRefExpr DRE(const_cast<VarDecl *>(PrivateVD),
false,
                      (*I)->getType(), VK_LValue, (*I)->getExprLoc());
      return EmitLValue(&DRE).getAddress();
});
    if (LocalDeclMap.count(VD) || CapturedStmtInfo->lookup(VD) ||
        VD->hasGlobalStorage()) {
      (void)LoopScope.addPrivate(PrivateVD, [&]() -> Address {
        DeclRefExpr DRE(const_cast<VarDecl *>(VD),
                        LocalDeclMap.count(VD) || CapturedStmtInfo->lookup(VD),
                        E->getType(), VK_LValue, E->getExprLoc());
        return EmitLValue(&DRE).getAddress();
});
}
    ++I;
}
}
static void emitPreCond(CodeGenFunction &CGF, const OMPLoopDirective &S,
                        const Expr *Cond, llvm::BasicBlock *TrueBlock,
                        llvm::BasicBlock *FalseBlock, uint64_t TrueCount) {
  if (!CGF.HaveInsertPoint())
    return;
  {
    CodeGenFunction::OMPPrivateScope PreCondScope(CGF);
    CGF.EmitOMPPrivateLoopCounters(S, PreCondScope);
    (void)PreCondScope.Privatize();
for_loop_talkad7420();
    for (auto I : S.inits()) {
      CGF.EmitIgnoredExpr(I);
}
}
  CGF.EmitBranchOnBoolExpr(Cond, TrueBlock, FalseBlock, TrueCount);
}
void CodeGenFunction::EmitOMPLinearClause(
    const OMPLoopDirective &D, CodeGenFunction::OMPPrivateScope &PrivateScope) {
  if (!HaveInsertPoint())
    return;
  llvm::DenseSet<const VarDecl *> SIMDLCVs;
  if (isOpenMPSimdDirective(D.getDirectiveKind())) {
    auto *LoopDirective = cast<OMPLoopDirective>(&D);
for_loop_talkad7420();
    for (auto *C : LoopDirective->counters()) {
      SIMDLCVs.insert(
          cast<VarDecl>(cast<DeclRefExpr>(C)->getDecl())->getCanonicalDecl());
}
}
for_loop_talkad7420();
  for (const auto *C : D.getClausesOfKind<OMPLinearClause>()) {
    auto CurPrivate = C->privates().begin();
for_loop_talkad7420();
    for (auto *E : C->varlists()) {
      auto *VD = cast<VarDecl>(cast<DeclRefExpr>(E)->getDecl());
      auto *PrivateVD =
          cast<VarDecl>(cast<DeclRefExpr>(*CurPrivate)->getDecl());
      if (!SIMDLCVs.count(VD->getCanonicalDecl())) {
        bool IsRegistered = PrivateScope.addPrivate(VD, [&]() -> Address {
          EmitVarDecl(*PrivateVD);
          return GetAddrOfLocalVar(PrivateVD);
});
        assert(IsRegistered && "linear var already registered as private");
        (void)IsRegistered;
} else
        EmitVarDecl(*PrivateVD);
      ++CurPrivate;
}
}
}
static void emitSimdlenSafelenClause(CodeGenFunction &CGF,
                                     const OMPExecutableDirective &D,
                                     bool IsMonotonic) {
  if (!CGF.HaveInsertPoint())
    return;
  if (const auto *C = D.getSingleClause<OMPSimdlenClause>()) {
    RValue Len = CGF.EmitAnyExpr(C->getSimdlen(), AggValueSlot::ignored(),
true);
    llvm::ConstantInt *Val = cast<llvm::ConstantInt>(Len.getScalarVal());
    CGF.LoopStack.setVectorizeWidth(Val->getZExtValue());
    if (!IsMonotonic)
      CGF.LoopStack.setParallel(!D.getSingleClause<OMPSafelenClause>());
} else if (const auto *C = D.getSingleClause<OMPSafelenClause>()) {
    RValue Len = CGF.EmitAnyExpr(C->getSafelen(), AggValueSlot::ignored(),
true);
    llvm::ConstantInt *Val = cast<llvm::ConstantInt>(Len.getScalarVal());
    CGF.LoopStack.setVectorizeWidth(Val->getZExtValue());
    CGF.LoopStack.setParallel(false);
}
}
void CodeGenFunction::EmitOMPSimdInit(const OMPLoopDirective &D,
                                      bool IsMonotonic) {
  LoopStack.setParallel(!IsMonotonic);
  LoopStack.setVectorizeEnable(true);
  emitSimdlenSafelenClause(*this, D, IsMonotonic);
}
void CodeGenFunction::EmitOMPSimdFinal(
    const OMPLoopDirective &D,
    const llvm::function_ref<llvm::Value *(CodeGenFunction &)> &CondGen) {
  if (!HaveInsertPoint())
    return;
  llvm::BasicBlock *DoneBB = nullptr;
  auto IC = D.counters().begin();
  auto IPC = D.private_counters().begin();
for_loop_talkad7420();
  for (auto F : D.finals()) {
    auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>((*IC))->getDecl());
    auto *PrivateVD = cast<VarDecl>(cast<DeclRefExpr>((*IPC))->getDecl());
    auto *CED = dyn_cast<OMPCapturedExprDecl>(OrigVD);
    if (LocalDeclMap.count(OrigVD) || CapturedStmtInfo->lookup(OrigVD) ||
        OrigVD->hasGlobalStorage() || CED) {
      if (!DoneBB) {
        if (auto *Cond = CondGen(*this)) {
          auto *ThenBB = createBasicBlock(".omp.final.then");
          DoneBB = createBasicBlock(".omp.final.done");
          Builder.CreateCondBr(Cond, ThenBB, DoneBB);
          EmitBlock(ThenBB);
}
}
      Address OrigAddr = Address::invalid();
      if (CED)
        OrigAddr = EmitLValue(CED->getInit()->IgnoreImpCasts()).getAddress();
      else {
        DeclRefExpr DRE(const_cast<VarDecl *>(PrivateVD),
false,
                        (*IPC)->getType(), VK_LValue, (*IPC)->getExprLoc());
        OrigAddr = EmitLValue(&DRE).getAddress();
}
      OMPPrivateScope VarScope(*this);
      VarScope.addPrivate(OrigVD,
                          [OrigAddr]() -> Address { return OrigAddr; 
});
      (void)VarScope.Privatize();
      EmitIgnoredExpr(F);
}
    ++IC;
    ++IPC;
}
  if (DoneBB)
    EmitBlock(DoneBB, 
true);
}
void CodeGenFunction::EmitOMPSimdDirective(const OMPSimdDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    OMPLoopScope PreInitScope(CGF, S);
    bool CondConstant;
    llvm::BasicBlock *ContBlock = nullptr;
    if (CGF.ConstantFoldsToSimpleInteger(S.getPreCond(), CondConstant)) {
      if (!CondConstant)
        return;
} else {
      auto *ThenBlock = CGF.createBasicBlock("simd.if.then");
      ContBlock = CGF.createBasicBlock("simd.if.end");
      emitPreCond(CGF, S, S.getPreCond(), ThenBlock, ContBlock,
                  CGF.getProfileCount(&S));
      CGF.EmitBlock(ThenBlock);
      CGF.incrementProfileCounter(&S);
}
    const Expr *IVExpr = S.getIterationVariable();
    const VarDecl *IVDecl = cast<VarDecl>(cast<DeclRefExpr>(IVExpr)->getDecl());
    CGF.EmitVarDecl(*IVDecl);
    CGF.EmitIgnoredExpr(S.getInit());
    if (auto LIExpr = dyn_cast<DeclRefExpr>(S.getLastIteration())) {
      CGF.EmitVarDecl(*cast<VarDecl>(LIExpr->getDecl()));
      CGF.EmitIgnoredExpr(S.getCalcLastIteration());
}
    CGF.EmitOMPSimdInit(S);
    emitAlignedClause(CGF, S);
    CGF.EmitOMPLinearClauseInit(S);
    {
      OMPPrivateScope LoopScope(CGF);
      CGF.EmitOMPPrivateLoopCounters(S, LoopScope);
      CGF.EmitOMPLinearClause(S, LoopScope);
      CGF.EmitOMPPrivateClause(S, LoopScope);
      CGF.EmitOMPReductionClauseInit(S, LoopScope);
      bool HasLastprivateClause =
          CGF.EmitOMPLastprivateClauseInit(S, LoopScope);
      (void)LoopScope.Privatize();
      CGF.EmitOMPInnerLoop(S, LoopScope.requiresCleanups(), S.getCond(),
                           S.getInc(),
                           [&S](CodeGenFunction &CGF) {
                             CGF.EmitOMPLoopBody(S, JumpDest());
                             CGF.EmitStopPoint(&S);
},
                           [](CodeGenFunction &) {
});
      CGF.EmitOMPSimdFinal(
          S, [](CodeGenFunction &) -> llvm::Value * { return nullptr; 
});
      if (HasLastprivateClause)
        CGF.EmitOMPLastprivateClauseFinal(S, 
true);
      CGF.EmitOMPReductionClauseFinal(S);
      emitPostUpdateForReductionClause(
          CGF, S, [](CodeGenFunction &) -> llvm::Value * { return nullptr; 
});
}
    CGF.EmitOMPLinearClauseFinal(
        S, [](CodeGenFunction &) -> llvm::Value * { return nullptr; 
});
    if (ContBlock) {
      CGF.EmitBranch(ContBlock);
      CGF.EmitBlock(ContBlock, true);
}
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_simd, CodeGen);
}
void CodeGenFunction::EmitOMPOuterLoop(bool DynamicOrOrdered, bool IsMonotonic,
    const OMPLoopDirective &S, OMPPrivateScope &LoopScope, bool Ordered,
    Address LB, Address UB, Address ST, Address IL, llvm::Value *Chunk) {
  auto &RT = CGM.getOpenMPRuntime();
  const Expr *IVExpr = S.getIterationVariable();
  const unsigned IVSize = getContext().getTypeSize(IVExpr->getType());
  const bool IVSigned = IVExpr->getType()->hasSignedIntegerRepresentation();
  auto LoopExit = getJumpDestInCurrentScope("omp.dispatch.end");
  auto CondBlock = createBasicBlock("omp.dispatch.cond");
  EmitBlock(CondBlock);
  const SourceRange &R = S.getSourceRange();
  LoopStack.push(CondBlock, SourceLocToDebugLoc(R.getBegin()),
                 SourceLocToDebugLoc(R.getEnd()));
  llvm::Value *BoolCondVal = nullptr;
  if (!DynamicOrOrdered) {
    EmitIgnoredExpr(S.getEnsureUpperBound());
    EmitIgnoredExpr(S.getInit());
    BoolCondVal = EvaluateExprAsBool(S.getCond());
} else {
    BoolCondVal = RT.emitForNext(*this, S.getLocStart(), IVSize, IVSigned, IL,
                                 LB, UB, ST);
}
  auto ExitBlock = LoopExit.getBlock();
  if (LoopScope.requiresCleanups())
    ExitBlock = createBasicBlock("omp.dispatch.cleanup");
  auto LoopBody = createBasicBlock("omp.dispatch.body");
  Builder.CreateCondBr(BoolCondVal, LoopBody, ExitBlock);
  if (ExitBlock != LoopExit.getBlock()) {
    EmitBlock(ExitBlock);
    EmitBranchThroughCleanup(LoopExit);
}
  EmitBlock(LoopBody);
  if (DynamicOrOrdered)
    EmitIgnoredExpr(S.getInit());
  auto Continue = getJumpDestInCurrentScope("omp.dispatch.inc");
  BreakContinueStack.push_back(BreakContinue(LoopExit, Continue));
  if (!isOpenMPSimdDirective(S.getDirectiveKind()))
    LoopStack.setParallel(!IsMonotonic);
  else
    EmitOMPSimdInit(S, IsMonotonic);
  SourceLocation Loc = S.getLocStart();
  EmitOMPInnerLoop(S, LoopScope.requiresCleanups(), S.getCond(), S.getInc(),
                   [&S, LoopExit](CodeGenFunction &CGF) {
                     CGF.EmitOMPLoopBody(S, LoopExit);
                     CGF.EmitStopPoint(&S);
},
                   [Ordered, IVSize, IVSigned, Loc](CodeGenFunction &CGF) {
                     if (Ordered) {
                       CGF.CGM.getOpenMPRuntime().emitForOrderedIterationEnd(
                           CGF, Loc, IVSize, IVSigned);
}
});
  EmitBlock(Continue.getBlock());
  BreakContinueStack.pop_back();
  if (!DynamicOrOrdered) {
    EmitIgnoredExpr(S.getNextLowerBound());
    EmitIgnoredExpr(S.getNextUpperBound());
}
  EmitBranch(CondBlock);
  LoopStack.pop();
  EmitBlock(LoopExit.getBlock());
  auto &&CodeGen = [DynamicOrOrdered, &S](CodeGenFunction &CGF) {
    if (!DynamicOrOrdered)
      CGF.CGM.getOpenMPRuntime().emitForStaticFinish(CGF, S.getLocEnd());
};
  OMPCancelStack.emitExit(*this, S.getDirectiveKind(), CodeGen);
}
void CodeGenFunction::EmitOMPForOuterLoop(
    const OpenMPScheduleTy &ScheduleKind, bool IsMonotonic,
    const OMPLoopDirective &S, OMPPrivateScope &LoopScope, bool Ordered,
    Address LB, Address UB, Address ST, Address IL, llvm::Value *Chunk) {
  auto &RT = CGM.getOpenMPRuntime();
  const bool DynamicOrOrdered =
      Ordered || RT.isDynamic(ScheduleKind.Schedule);
  assert((Ordered ||
          !RT.isStaticNonchunked(ScheduleKind.Schedule,
Chunk != nullptr)) &&
         "static non-chunked schedule does not need outer loop");
  const Expr *IVExpr = S.getIterationVariable();
  const unsigned IVSize = getContext().getTypeSize(IVExpr->getType());
  const bool IVSigned = IVExpr->getType()->hasSignedIntegerRepresentation();
  if (DynamicOrOrdered) {
    llvm::Value *UBVal = EmitScalarExpr(S.getLastIteration());
    RT.emitForDispatchInit(*this, S.getLocStart(), ScheduleKind, IVSize,
                           IVSigned, Ordered, UBVal, Chunk);
} else {
    RT.emitForStaticInit(*this, S.getLocStart(), ScheduleKind, IVSize, IVSigned,
                         Ordered, IL, LB, UB, ST, Chunk);
}
  EmitOMPOuterLoop(DynamicOrOrdered, IsMonotonic, S, LoopScope, Ordered, LB, UB,
                   ST, IL, Chunk);
}
void CodeGenFunction::EmitOMPDistributeOuterLoop(
    OpenMPDistScheduleClauseKind ScheduleKind,
    const OMPDistributeDirective &S, OMPPrivateScope &LoopScope,
    Address LB, Address UB, Address ST, Address IL, llvm::Value *Chunk) {
  auto &RT = CGM.getOpenMPRuntime();
  const Expr *IVExpr = S.getIterationVariable();
  const unsigned IVSize = getContext().getTypeSize(IVExpr->getType());
  const bool IVSigned = IVExpr->getType()->hasSignedIntegerRepresentation();
  RT.emitDistributeStaticInit(*this, S.getLocStart(), ScheduleKind,
                              IVSize, IVSigned, 
 false,
                              IL, LB, UB, ST, Chunk);
  EmitOMPOuterLoop(
 false, 
 false,
                   S, LoopScope, 
 false, LB, UB, ST, IL, Chunk);
}
void CodeGenFunction::EmitOMPDistributeParallelForDirective(
    const OMPDistributeParallelForDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_distribute_parallel_for,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        OMPCancelStackRAII CancelRegion(CGF, OMPD_distribute_parallel_for,
false);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPDistributeParallelForSimdDirective(
    const OMPDistributeParallelForSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_distribute_parallel_for_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPDistributeSimdDirective(
    const OMPDistributeSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_distribute_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetParallelForSimdDirective(
    const OMPTargetParallelForSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_parallel_for_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetSimdDirective(
    const OMPTargetSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_simd, [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTeamsDistributeDirective(
    const OMPTeamsDistributeDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_teams_distribute,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTeamsDistributeSimdDirective(
    const OMPTeamsDistributeSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_teams_distribute_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTeamsDistributeParallelForSimdDirective(
    const OMPTeamsDistributeParallelForSimdDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_teams_distribute_parallel_for_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTeamsDistributeParallelForDirective(
    const OMPTeamsDistributeParallelForDirective &S) {
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_teams_distribute_parallel_for,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        OMPLoopScope PreInitScope(CGF, S);
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetTeamsDirective(
    const OMPTargetTeamsDirective &S) {
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_teams, [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetTeamsDistributeDirective(
    const OMPTargetTeamsDistributeDirective &S) {
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_teams_distribute,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetTeamsDistributeParallelForDirective(
    const OMPTargetTeamsDistributeParallelForDirective &S) {
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_teams_distribute_parallel_for,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetTeamsDistributeParallelForSimdDirective(
    const OMPTargetTeamsDistributeParallelForSimdDirective &S) {
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_teams_distribute_parallel_for_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
void CodeGenFunction::EmitOMPTargetTeamsDistributeSimdDirective(
    const OMPTargetTeamsDistributeSimdDirective &S) {
  CGM.getOpenMPRuntime().emitInlinedDirective(
      *this, OMPD_target_teams_distribute_simd,
      [&S](CodeGenFunction &CGF, PrePostActionTy &) {
        CGF.EmitStmt(
            cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
});
}
static LValue EmitOMPHelperVar(CodeGenFunction &CGF,
                               const DeclRefExpr *Helper) {
  auto VDecl = cast<VarDecl>(Helper->getDecl());
  CGF.EmitVarDecl(*VDecl);
  return CGF.EmitLValue(Helper);
}
namespace {
  struct ScheduleKindModifiersTy {
    OpenMPScheduleClauseKind Kind;
    OpenMPScheduleClauseModifier M1;
    OpenMPScheduleClauseModifier M2;
    ScheduleKindModifiersTy(OpenMPScheduleClauseKind Kind,
                            OpenMPScheduleClauseModifier M1,
                            OpenMPScheduleClauseModifier M2)
        : Kind(Kind), M1(M1), M2(M2) {
}
};
} 
bool CodeGenFunction::EmitOMPWorksharingLoop(const OMPLoopDirective &S) {
  auto IVExpr = cast<DeclRefExpr>(S.getIterationVariable());
  auto IVDecl = cast<VarDecl>(IVExpr->getDecl());
  EmitVarDecl(*IVDecl);
  if (auto LIExpr = dyn_cast<DeclRefExpr>(S.getLastIteration())) {
    EmitVarDecl(*cast<VarDecl>(LIExpr->getDecl()));
    EmitIgnoredExpr(S.getCalcLastIteration());
}
  auto &RT = CGM.getOpenMPRuntime();
  bool HasLastprivateClause;
  {
    OMPLoopScope PreInitScope(*this, S);
    bool CondConstant;
    llvm::BasicBlock *ContBlock = nullptr;
    if (ConstantFoldsToSimpleInteger(S.getPreCond(), CondConstant)) {
      if (!CondConstant)
        return false;
} else {
      auto *ThenBlock = createBasicBlock("omp.precond.then");
      ContBlock = createBasicBlock("omp.precond.end");
      emitPreCond(*this, S, S.getPreCond(), ThenBlock, ContBlock,
                  getProfileCount(&S));
      EmitBlock(ThenBlock);
      incrementProfileCounter(&S);
}
    bool Ordered = false;
    if (auto *OrderedClause = S.getSingleClause<OMPOrderedClause>()) {
      if (OrderedClause->getNumForLoops())
        RT.emitDoacrossInit(*this, S);
      else
        Ordered = true;
}
    llvm::DenseSet<const Expr *> EmittedFinals;
    emitAlignedClause(*this, S);
    EmitOMPLinearClauseInit(S);
    LValue LB =
        EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getLowerBoundVariable()));
    LValue UB =
        EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getUpperBoundVariable()));
    LValue ST =
        EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getStrideVariable()));
    LValue IL =
        EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getIsLastIterVariable()));
    {
      OMPPrivateScope LoopScope(*this);
      if (EmitOMPFirstprivateClause(S, LoopScope)) {
        CGM.getOpenMPRuntime().emitBarrierCall(
            *this, S.getLocStart(), OMPD_unknown, 
false,
true);
}
      EmitOMPPrivateClause(S, LoopScope);
      HasLastprivateClause = EmitOMPLastprivateClauseInit(S, LoopScope);
      EmitOMPReductionClauseInit(S, LoopScope);
      EmitOMPPrivateLoopCounters(S, LoopScope);
      EmitOMPLinearClause(S, LoopScope);
      (void)LoopScope.Privatize();
      llvm::Value *Chunk = nullptr;
      OpenMPScheduleTy ScheduleKind;
      if (auto *C = S.getSingleClause<OMPScheduleClause>()) {
        ScheduleKind.Schedule = C->getScheduleKind();
        ScheduleKind.M1 = C->getFirstScheduleModifier();
        ScheduleKind.M2 = C->getSecondScheduleModifier();
        if (const auto *Ch = C->getChunkSize()) {
          Chunk = EmitScalarExpr(Ch);
          Chunk = EmitScalarConversion(Chunk, Ch->getType(),
                                       S.getIterationVariable()->getType(),
                                       S.getLocStart());
}
}
      const unsigned IVSize = getContext().getTypeSize(IVExpr->getType());
      const bool IVSigned = IVExpr->getType()->hasSignedIntegerRepresentation();
      if (RT.isStaticNonchunked(ScheduleKind.Schedule,
 Chunk != nullptr) &&
          !Ordered) {
        if (isOpenMPSimdDirective(S.getDirectiveKind()))
          EmitOMPSimdInit(S, 
true);
        RT.emitForStaticInit(*this, S.getLocStart(), ScheduleKind,
                             IVSize, IVSigned, Ordered,
                             IL.getAddress(), LB.getAddress(),
                             UB.getAddress(), ST.getAddress());
        auto LoopExit =
            getJumpDestInCurrentScope(createBasicBlock("omp.loop.exit"));
        EmitIgnoredExpr(S.getEnsureUpperBound());
        EmitIgnoredExpr(S.getInit());
        EmitOMPInnerLoop(S, LoopScope.requiresCleanups(), S.getCond(),
                         S.getInc(),
                         [&S, LoopExit](CodeGenFunction &CGF) {
                           CGF.EmitOMPLoopBody(S, LoopExit);
                           CGF.EmitStopPoint(&S);
},
                         [](CodeGenFunction &) {
});
        EmitBlock(LoopExit.getBlock());
        auto &&CodeGen = [&S](CodeGenFunction &CGF) {
          CGF.CGM.getOpenMPRuntime().emitForStaticFinish(CGF, S.getLocEnd());
};
        OMPCancelStack.emitExit(*this, S.getDirectiveKind(), CodeGen);
} else {
        const bool IsMonotonic =
            Ordered || ScheduleKind.Schedule == OMPC_SCHEDULE_static ||
            ScheduleKind.Schedule == OMPC_SCHEDULE_unknown ||
            ScheduleKind.M1 == OMPC_SCHEDULE_MODIFIER_monotonic ||
            ScheduleKind.M2 == OMPC_SCHEDULE_MODIFIER_monotonic;
        EmitOMPForOuterLoop(ScheduleKind, IsMonotonic, S, LoopScope, Ordered,
                            LB.getAddress(), UB.getAddress(), ST.getAddress(),
                            IL.getAddress(), Chunk);
}
      if (isOpenMPSimdDirective(S.getDirectiveKind())) {
        EmitOMPSimdFinal(S,
                         [&](CodeGenFunction &CGF) -> llvm::Value * {
                           return CGF.Builder.CreateIsNotNull(
                               CGF.EmitLoadOfScalar(IL, S.getLocStart()));
});
}
      EmitOMPReductionClauseFinal(S);
      emitPostUpdateForReductionClause(
          *this, S, [&](CodeGenFunction &CGF) -> llvm::Value * {
            return CGF.Builder.CreateIsNotNull(
                CGF.EmitLoadOfScalar(IL, S.getLocStart()));
});
      if (HasLastprivateClause)
        EmitOMPLastprivateClauseFinal(
            S, isOpenMPSimdDirective(S.getDirectiveKind()),
            Builder.CreateIsNotNull(EmitLoadOfScalar(IL, S.getLocStart())));
}
    EmitOMPLinearClauseFinal(S, [&](CodeGenFunction &CGF) -> llvm::Value * {
      return CGF.Builder.CreateIsNotNull(
          CGF.EmitLoadOfScalar(IL, S.getLocStart()));
});
    if (ContBlock) {
      EmitBranch(ContBlock);
      EmitBlock(ContBlock, true);
}
}
  return HasLastprivateClause;
}
void CodeGenFunction::EmitOMPForDirective(const OMPForDirective &S) {
  bool HasLastprivates = false;
  auto &&CodeGen = [&S, &HasLastprivates](CodeGenFunction &CGF,
                                          PrePostActionTy &) {
    OMPCancelStackRAII CancelRegion(CGF, OMPD_for, S.hasCancel());
    HasLastprivates = CGF.EmitOMPWorksharingLoop(S);
};
  {
    OMPLexicalScope Scope(*this, S, 
true);
    CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_for, CodeGen,
                                                S.hasCancel());
}
  if (!S.getSingleClause<OMPNowaitClause>() || HasLastprivates) {
    CGM.getOpenMPRuntime().emitBarrierCall(*this, S.getLocStart(), OMPD_for);
}
}
void CodeGenFunction::EmitOMPForSimdDirective(const OMPForSimdDirective &S) {
  bool HasLastprivates = false;
  auto &&CodeGen = [&S, &HasLastprivates](CodeGenFunction &CGF,
                                          PrePostActionTy &) {
    HasLastprivates = CGF.EmitOMPWorksharingLoop(S);
};
  {
    OMPLexicalScope Scope(*this, S, 
true);
    CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_simd, CodeGen);
}
  if (!S.getSingleClause<OMPNowaitClause>() || HasLastprivates) {
    CGM.getOpenMPRuntime().emitBarrierCall(*this, S.getLocStart(), OMPD_for);
}
}
static LValue createSectionLVal(CodeGenFunction &CGF, QualType Ty,
                                const Twine &Name,
                                llvm::Value *Init = nullptr) {
  auto LVal = CGF.MakeAddrLValue(CGF.CreateMemTemp(Ty, Name), Ty);
  if (Init)
    CGF.EmitStoreThroughLValue(RValue::get(Init), LVal, 
 true);
  return LVal;
}
void CodeGenFunction::EmitSections(const OMPExecutableDirective &S) {
  auto *Stmt = cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt();
  auto *CS = dyn_cast<CompoundStmt>(Stmt);
  bool HasLastprivates = false;
  auto &&CodeGen = [&S, Stmt, CS, &HasLastprivates](CodeGenFunction &CGF,
                                                    PrePostActionTy &) {
    auto &C = CGF.CGM.getContext();
    auto KmpInt32Ty = C.getIntTypeForBitwidth(
32, 
1);
    LValue LB = createSectionLVal(CGF, KmpInt32Ty, ".omp.sections.lb.",
                                  CGF.Builder.getInt32(0));
    auto *GlobalUBVal = CS != nullptr ? CGF.Builder.getInt32(CS->size() - 1)
                                      : CGF.Builder.getInt32(0);
    LValue UB =
        createSectionLVal(CGF, KmpInt32Ty, ".omp.sections.ub.", GlobalUBVal);
    LValue ST = createSectionLVal(CGF, KmpInt32Ty, ".omp.sections.st.",
                                  CGF.Builder.getInt32(1));
    LValue IL = createSectionLVal(CGF, KmpInt32Ty, ".omp.sections.il.",
                                  CGF.Builder.getInt32(0));
    LValue IV = createSectionLVal(CGF, KmpInt32Ty, ".omp.sections.iv.");
    OpaqueValueExpr IVRefExpr(S.getLocStart(), KmpInt32Ty, VK_LValue);
    CodeGenFunction::OpaqueValueMapping OpaqueIV(CGF, &IVRefExpr, IV);
    OpaqueValueExpr UBRefExpr(S.getLocStart(), KmpInt32Ty, VK_LValue);
    CodeGenFunction::OpaqueValueMapping OpaqueUB(CGF, &UBRefExpr, UB);
    BinaryOperator Cond(&IVRefExpr, &UBRefExpr, BO_LE, C.BoolTy, VK_RValue,
                        OK_Ordinary, S.getLocStart(),
false);
    UnaryOperator Inc(&IVRefExpr, UO_PreInc, KmpInt32Ty, VK_RValue, OK_Ordinary,
                      S.getLocStart());
    auto BodyGen = [Stmt, CS, &S, &IV](CodeGenFunction &CGF) {
      auto *ExitBB = CGF.createBasicBlock(".omp.sections.exit");
      auto *SwitchStmt = CGF.Builder.CreateSwitch(
          CGF.EmitLoadOfLValue(IV, S.getLocStart()).getScalarVal(), ExitBB,
          CS == nullptr ? 1 : CS->size());
      if (CS) {
        unsigned CaseNumber = 0;
for_loop_talkad7420();
        for (auto *SubStmt : CS->children()) {
          auto CaseBB = CGF.createBasicBlock(".omp.sections.case");
          CGF.EmitBlock(CaseBB);
          SwitchStmt->addCase(CGF.Builder.getInt32(CaseNumber), CaseBB);
          CGF.EmitStmt(SubStmt);
          CGF.EmitBranch(ExitBB);
          ++CaseNumber;
}
} else {
        auto CaseBB = CGF.createBasicBlock(".omp.sections.case");
        CGF.EmitBlock(CaseBB);
        SwitchStmt->addCase(CGF.Builder.getInt32(0), CaseBB);
        CGF.EmitStmt(Stmt);
        CGF.EmitBranch(ExitBB);
}
      CGF.EmitBlock(ExitBB, 
true);
};
    CodeGenFunction::OMPPrivateScope LoopScope(CGF);
    if (CGF.EmitOMPFirstprivateClause(S, LoopScope)) {
      CGF.CGM.getOpenMPRuntime().emitBarrierCall(
          CGF, S.getLocStart(), OMPD_unknown, 
false,
true);
}
    CGF.EmitOMPPrivateClause(S, LoopScope);
    HasLastprivates = CGF.EmitOMPLastprivateClauseInit(S, LoopScope);
    CGF.EmitOMPReductionClauseInit(S, LoopScope);
    (void)LoopScope.Privatize();
    OpenMPScheduleTy ScheduleKind;
    ScheduleKind.Schedule = OMPC_SCHEDULE_static;
    CGF.CGM.getOpenMPRuntime().emitForStaticInit(
        CGF, S.getLocStart(), ScheduleKind, 
32,
true, 
false, IL.getAddress(), LB.getAddress(),
        UB.getAddress(), ST.getAddress());
    auto *UBVal = CGF.EmitLoadOfScalar(UB, S.getLocStart());
    auto *MinUBGlobalUB = CGF.Builder.CreateSelect(
        CGF.Builder.CreateICmpSLT(UBVal, GlobalUBVal), UBVal, GlobalUBVal);
    CGF.EmitStoreOfScalar(MinUBGlobalUB, UB);
    CGF.EmitStoreOfScalar(CGF.EmitLoadOfScalar(LB, S.getLocStart()), IV);
    CGF.EmitOMPInnerLoop(S, 
false, &Cond, &Inc, BodyGen,
                         [](CodeGenFunction &) {
});
    auto &&CodeGen = [&S](CodeGenFunction &CGF) {
      CGF.CGM.getOpenMPRuntime().emitForStaticFinish(CGF, S.getLocEnd());
};
    CGF.OMPCancelStack.emitExit(CGF, S.getDirectiveKind(), CodeGen);
    CGF.EmitOMPReductionClauseFinal(S);
    emitPostUpdateForReductionClause(
        CGF, S, [&](CodeGenFunction &CGF) -> llvm::Value * {
          return CGF.Builder.CreateIsNotNull(
              CGF.EmitLoadOfScalar(IL, S.getLocStart()));
});
    if (HasLastprivates)
      CGF.EmitOMPLastprivateClauseFinal(
          S, 
false,
          CGF.Builder.CreateIsNotNull(
              CGF.EmitLoadOfScalar(IL, S.getLocStart())));
};
  bool HasCancel = false;
  if (auto *OSD = dyn_cast<OMPSectionsDirective>(&S))
    HasCancel = OSD->hasCancel();
  else if (auto *OPSD = dyn_cast<OMPParallelSectionsDirective>(&S))
    HasCancel = OPSD->hasCancel();
  OMPCancelStackRAII CancelRegion(*this, S.getDirectiveKind(), HasCancel);
  CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_sections, CodeGen,
                                              HasCancel);
  if (HasLastprivates && S.getSingleClause<OMPNowaitClause>()) {
    CGM.getOpenMPRuntime().emitBarrierCall(*this, S.getLocStart(),
                                           OMPD_unknown);
}
}
void CodeGenFunction::EmitOMPSectionsDirective(const OMPSectionsDirective &S) {
  {
    OMPLexicalScope Scope(*this, S, 
true);
    EmitSections(S);
}
  if (!S.getSingleClause<OMPNowaitClause>()) {
    CGM.getOpenMPRuntime().emitBarrierCall(*this, S.getLocStart(),
                                           OMPD_sections);
}
}
void CodeGenFunction::EmitOMPSectionDirective(const OMPSectionDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_section, CodeGen,
                                              S.hasCancel());
}
void CodeGenFunction::EmitOMPSingleDirective(const OMPSingleDirective &S) {
  llvm::SmallVector<const Expr *, 8> CopyprivateVars;
  llvm::SmallVector<const Expr *, 8> DestExprs;
  llvm::SmallVector<const Expr *, 8> SrcExprs;
  llvm::SmallVector<const Expr *, 8> AssignmentOps;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPCopyprivateClause>()) {
    CopyprivateVars.append(C->varlists().begin(), C->varlists().end());
    DestExprs.append(C->destination_exprs().begin(),
                     C->destination_exprs().end());
    SrcExprs.append(C->source_exprs().begin(), C->source_exprs().end());
    AssignmentOps.append(C->assignment_ops().begin(),
                         C->assignment_ops().end());
}
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &Action) {
    Action.Enter(CGF);
    OMPPrivateScope SingleScope(CGF);
    (void)CGF.EmitOMPFirstprivateClause(S, SingleScope);
    CGF.EmitOMPPrivateClause(S, SingleScope);
    (void)SingleScope.Privatize();
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  {
    OMPLexicalScope Scope(*this, S, 
true);
    CGM.getOpenMPRuntime().emitSingleRegion(*this, CodeGen, S.getLocStart(),
                                            CopyprivateVars, DestExprs,
                                            SrcExprs, AssignmentOps);
}
  if (!S.getSingleClause<OMPNowaitClause>() && CopyprivateVars.empty()) {
    CGM.getOpenMPRuntime().emitBarrierCall(
        *this, S.getLocStart(),
        S.getSingleClause<OMPNowaitClause>() ? OMPD_unknown : OMPD_single);
}
}
void CodeGenFunction::EmitOMPMasterDirective(const OMPMasterDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &Action) {
    Action.Enter(CGF);
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitMasterRegion(*this, CodeGen, S.getLocStart());
}
void CodeGenFunction::EmitOMPCriticalDirective(const OMPCriticalDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &Action) {
    Action.Enter(CGF);
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  Expr *Hint = nullptr;
  if (auto *HintClause = S.getSingleClause<OMPHintClause>())
    Hint = HintClause->getHint();
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitCriticalRegion(*this,
                                            S.getDirectiveName().getAsString(),
                                            CodeGen, S.getLocStart(), Hint);
}
void CodeGenFunction::EmitOMPParallelForDirective(
    const OMPParallelForDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    OMPCancelStackRAII CancelRegion(CGF, OMPD_parallel_for, S.hasCancel());
    CGF.EmitOMPWorksharingLoop(S);
};
  emitCommonOMPParallelDirective(*this, S, OMPD_for, CodeGen);
}
void CodeGenFunction::EmitOMPParallelForSimdDirective(
    const OMPParallelForSimdDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    CGF.EmitOMPWorksharingLoop(S);
};
  emitCommonOMPParallelDirective(*this, S, OMPD_simd, CodeGen);
}
void CodeGenFunction::EmitOMPParallelSectionsDirective(
    const OMPParallelSectionsDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    CGF.EmitSections(S);
};
  emitCommonOMPParallelDirective(*this, S, OMPD_sections, CodeGen);
}
void CodeGenFunction::EmitOMPTaskBasedDirective(const OMPExecutableDirective &S,
                                                const RegionCodeGenTy &BodyGen,
                                                const TaskGenTy &TaskGen,
                                                OMPTaskDataTy &Data) {
  auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
  auto *I = CS->getCapturedDecl()->param_begin();
  auto *PartId = std::next(I);
  auto *TaskT = std::next(I, 4);
  if (const auto *Clause = S.getSingleClause<OMPFinalClause>()) {
    auto *Cond = Clause->getCondition();
    bool CondConstant;
    if (ConstantFoldsToSimpleInteger(Cond, CondConstant))
      Data.Final.setInt(CondConstant);
    else
      Data.Final.setPointer(EvaluateExprAsBool(Cond));
} else {
    Data.Final.setInt(
false);
}
  if (const auto *Clause = S.getSingleClause<OMPPriorityClause>()) {
    auto *Prio = Clause->getPriority();
    Data.Priority.setInt(
true);
    Data.Priority.setPointer(EmitScalarConversion(
        EmitScalarExpr(Prio), Prio->getType(),
        getContext().getIntTypeForBitwidth(
32, 
1),
        Prio->getExprLoc()));
}
  llvm::DenseSet<const VarDecl *> EmittedAsPrivate;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPPrivateClause>()) {
    auto IRef = C->varlist_begin();
for_loop_talkad7420();
    for (auto *IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      if (EmittedAsPrivate.insert(OrigVD->getCanonicalDecl()).second) {
        Data.PrivateVars.push_back(*IRef);
        Data.PrivateCopies.push_back(IInit);
}
      ++IRef;
}
}
  EmittedAsPrivate.clear();
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPFirstprivateClause>()) {
    auto IRef = C->varlist_begin();
    auto IElemInitRef = C->inits().begin();
for_loop_talkad7420();
    for (auto *IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      if (EmittedAsPrivate.insert(OrigVD->getCanonicalDecl()).second) {
        Data.FirstprivateVars.push_back(*IRef);
        Data.FirstprivateCopies.push_back(IInit);
        Data.FirstprivateInits.push_back(*IElemInitRef);
}
      ++IRef;
      ++IElemInitRef;
}
}
  llvm::DenseMap<const VarDecl *, const DeclRefExpr *> LastprivateDstsOrigs;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPLastprivateClause>()) {
    auto IRef = C->varlist_begin();
    auto ID = C->destination_exprs().begin();
for_loop_talkad7420();
    for (auto *IInit : C->private_copies()) {
      auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*IRef)->getDecl());
      if (EmittedAsPrivate.insert(OrigVD->getCanonicalDecl()).second) {
        Data.LastprivateVars.push_back(*IRef);
        Data.LastprivateCopies.push_back(IInit);
}
      LastprivateDstsOrigs.insert(
          {cast<VarDecl>(cast<DeclRefExpr>(*ID)->getDecl()),
           cast<DeclRefExpr>(*IRef)
});
      ++IRef;
      ++ID;
}
}
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPDependClause>())
{
for_loop_talkad7420();
    for (auto *IRef : C->varlists())
{
      Data.Dependences.push_back(std::make_pair(C->getDependencyKind(), IRef));
}
}
  auto &&CodeGen = [PartId, &S, &Data, CS, &BodyGen, &LastprivateDstsOrigs](
      CodeGenFunction &CGF, PrePostActionTy &Action) {
    OMPPrivateScope Scope(CGF);
    if (!Data.PrivateVars.empty() || !Data.FirstprivateVars.empty() ||
        !Data.LastprivateVars.empty()) {
      auto *CopyFn = CGF.Builder.CreateLoad(
          CGF.GetAddrOfLocalVar(CS->getCapturedDecl()->getParam(3)));
      auto *PrivatesPtr = CGF.Builder.CreateLoad(
          CGF.GetAddrOfLocalVar(CS->getCapturedDecl()->getParam(2)));
      llvm::SmallVector<std::pair<const VarDecl *, Address>, 16> PrivatePtrs;
      llvm::SmallVector<llvm::Value *, 16> CallArgs;
      CallArgs.push_back(PrivatesPtr);
for_loop_talkad7420();
      for (auto *E : Data.PrivateVars) {
        auto *VD = cast<VarDecl>(cast<DeclRefExpr>(E)->getDecl());
        Address PrivatePtr = CGF.CreateMemTemp(
            CGF.getContext().getPointerType(E->getType()), ".priv.ptr.addr");
        PrivatePtrs.push_back(std::make_pair(VD, PrivatePtr));
        CallArgs.push_back(PrivatePtr.getPointer());
}
for_loop_talkad7420();
      for (auto *E : Data.FirstprivateVars) {
        auto *VD = cast<VarDecl>(cast<DeclRefExpr>(E)->getDecl());
        Address PrivatePtr =
            CGF.CreateMemTemp(CGF.getContext().getPointerType(E->getType()),
                              ".firstpriv.ptr.addr");
        PrivatePtrs.push_back(std::make_pair(VD, PrivatePtr));
        CallArgs.push_back(PrivatePtr.getPointer());
}
for_loop_talkad7420();
      for (auto *E : Data.LastprivateVars) {
        auto *VD = cast<VarDecl>(cast<DeclRefExpr>(E)->getDecl());
        Address PrivatePtr =
            CGF.CreateMemTemp(CGF.getContext().getPointerType(E->getType()),
                              ".lastpriv.ptr.addr");
        PrivatePtrs.push_back(std::make_pair(VD, PrivatePtr));
        CallArgs.push_back(PrivatePtr.getPointer());
}
      CGF.EmitRuntimeCall(CopyFn, CallArgs);
for_loop_talkad7420();
      for (auto &&Pair : LastprivateDstsOrigs) {
        auto *OrigVD = cast<VarDecl>(Pair.second->getDecl());
        DeclRefExpr DRE(
            const_cast<VarDecl *>(OrigVD),
CGF.CapturedStmtInfo->lookup(
                OrigVD) != nullptr,
            Pair.second->getType(), VK_LValue, Pair.second->getExprLoc());
        Scope.addPrivate(Pair.first, [&CGF, &DRE]() {
          return CGF.EmitLValue(&DRE).getAddress();
});
}
for_loop_talkad7420();
      for (auto &&Pair : PrivatePtrs) {
        Address Replacement(CGF.Builder.CreateLoad(Pair.second),
                            CGF.getContext().getDeclAlign(Pair.first));
        Scope.addPrivate(Pair.first, [Replacement]() { return Replacement; 
});
}
}
    (void)Scope.Privatize();
    Action.Enter(CGF);
    BodyGen(CGF);
};
  auto *OutlinedFn = CGM.getOpenMPRuntime().emitTaskOutlinedFunction(
      S, *I, *PartId, *TaskT, S.getDirectiveKind(), CodeGen, Data.Tied,
      Data.NumberOfParts);
  OMPLexicalScope Scope(*this, S);
  TaskGen(*this, OutlinedFn, Data);
}
void CodeGenFunction::EmitOMPTaskDirective(const OMPTaskDirective &S) {
  auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
  auto CapturedStruct = GenerateCapturedStmtArgument(*CS);
  auto SharedsTy = getContext().getRecordType(CS->getCapturedRecordDecl());
  const Expr *IfCond = nullptr;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPIfClause>()) {
    if (C->getNameModifier() == OMPD_unknown ||
        C->getNameModifier() == OMPD_task) {
      IfCond = C->getCondition();
      break;
}
}
  OMPTaskDataTy Data;
  Data.Tied = !S.getSingleClause<OMPUntiedClause>();
  auto &&BodyGen = [CS](CodeGenFunction &CGF, PrePostActionTy &) {
    CGF.EmitStmt(CS->getCapturedStmt());
};
  auto &&TaskGen = [&S, SharedsTy, CapturedStruct,
                    IfCond](CodeGenFunction &CGF, llvm::Value *OutlinedFn,
                            const OMPTaskDataTy &Data) {
    CGF.CGM.getOpenMPRuntime().emitTaskCall(CGF, S.getLocStart(), S, OutlinedFn,
                                            SharedsTy, CapturedStruct, IfCond,
                                            Data);
};
  EmitOMPTaskBasedDirective(S, BodyGen, TaskGen, Data);
}
void CodeGenFunction::EmitOMPTaskyieldDirective(
    const OMPTaskyieldDirective &S) {
  CGM.getOpenMPRuntime().emitTaskyieldCall(*this, S.getLocStart());
}
void CodeGenFunction::EmitOMPBarrierDirective(const OMPBarrierDirective &S) {
  CGM.getOpenMPRuntime().emitBarrierCall(*this, S.getLocStart(), OMPD_barrier);
}
void CodeGenFunction::EmitOMPTaskwaitDirective(const OMPTaskwaitDirective &S) {
  CGM.getOpenMPRuntime().emitTaskwaitCall(*this, S.getLocStart());
}
void CodeGenFunction::EmitOMPTaskgroupDirective(
    const OMPTaskgroupDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &Action) {
    Action.Enter(CGF);
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitTaskgroupRegion(*this, CodeGen, S.getLocStart());
}
void CodeGenFunction::EmitOMPFlushDirective(const OMPFlushDirective &S) {
  CGM.getOpenMPRuntime().emitFlush(*this, [&]() -> ArrayRef<const Expr *> {
    if (const auto *FlushClause = S.getSingleClause<OMPFlushClause>()) {
      return llvm::makeArrayRef(FlushClause->varlist_begin(),
                                FlushClause->varlist_end());
}
    return llvm::None;
}(), S.getLocStart());
}
void CodeGenFunction::EmitOMPIncDirective(const OMPIncDirective &S) {
  CGM.getOpenMPRuntime().emitInc(*this, [&]() -> ArrayRef<const Expr *> {
    if (const auto *IncClause = S.getSingleClause<OMPIncClause>()) {
      return llvm::makeArrayRef(IncClause->varlist_begin(),
                                IncClause->varlist_end());
}
    return llvm::None;
}(), S.getLocStart());
}
void CodeGenFunction::EmitOMPDistributeLoop(const OMPDistributeDirective &S) {
  auto IVExpr = cast<DeclRefExpr>(S.getIterationVariable());
  auto IVDecl = cast<VarDecl>(IVExpr->getDecl());
  EmitVarDecl(*IVDecl);
  if (auto LIExpr = dyn_cast<DeclRefExpr>(S.getLastIteration())) {
    EmitVarDecl(*cast<VarDecl>(LIExpr->getDecl()));
    EmitIgnoredExpr(S.getCalcLastIteration());
}
  auto &RT = CGM.getOpenMPRuntime();
  bool HasLastprivateClause = false;
  {
    OMPLoopScope PreInitScope(*this, S);
    bool CondConstant;
    llvm::BasicBlock *ContBlock = nullptr;
    if (ConstantFoldsToSimpleInteger(S.getPreCond(), CondConstant)) {
      if (!CondConstant)
        return;
} else {
      auto *ThenBlock = createBasicBlock("omp.precond.then");
      ContBlock = createBasicBlock("omp.precond.end");
      emitPreCond(*this, S, S.getPreCond(), ThenBlock, ContBlock,
                  getProfileCount(&S));
      EmitBlock(ThenBlock);
      incrementProfileCounter(&S);
}
    {
      LValue LB =
          EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getLowerBoundVariable()));
      LValue UB =
          EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getUpperBoundVariable()));
      LValue ST =
          EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getStrideVariable()));
      LValue IL =
          EmitOMPHelperVar(*this, cast<DeclRefExpr>(S.getIsLastIterVariable()));
      OMPPrivateScope LoopScope(*this);
      if (EmitOMPFirstprivateClause(S, LoopScope)) {
        CGM.getOpenMPRuntime().emitBarrierCall(
          *this, S.getLocStart(), OMPD_unknown, 
false,
true);
}
      EmitOMPPrivateClause(S, LoopScope);
      HasLastprivateClause = EmitOMPLastprivateClauseInit(S, LoopScope);
      EmitOMPPrivateLoopCounters(S, LoopScope);
      (void)LoopScope.Privatize();
      llvm::Value *Chunk = nullptr;
      OpenMPDistScheduleClauseKind ScheduleKind = OMPC_DIST_SCHEDULE_unknown;
      if (auto *C = S.getSingleClause<OMPDistScheduleClause>()) {
        ScheduleKind = C->getDistScheduleKind();
        if (const auto *Ch = C->getChunkSize()) {
          Chunk = EmitScalarExpr(Ch);
          Chunk = EmitScalarConversion(Chunk, Ch->getType(),
          S.getIterationVariable()->getType(),
          S.getLocStart());
}
}
      const unsigned IVSize = getContext().getTypeSize(IVExpr->getType());
      const bool IVSigned = IVExpr->getType()->hasSignedIntegerRepresentation();
      if (RT.isStaticNonchunked(ScheduleKind,
 Chunk != nullptr)) {
        RT.emitDistributeStaticInit(*this, S.getLocStart(), ScheduleKind,
                             IVSize, IVSigned, 
 false,
                             IL.getAddress(), LB.getAddress(),
                             UB.getAddress(), ST.getAddress());
        auto LoopExit =
            getJumpDestInCurrentScope(createBasicBlock("omp.loop.exit"));
        EmitIgnoredExpr(S.getEnsureUpperBound());
        EmitIgnoredExpr(S.getInit());
        EmitOMPInnerLoop(S, LoopScope.requiresCleanups(), S.getCond(),
                         S.getInc(),
                         [&S, LoopExit](CodeGenFunction &CGF) {
                           CGF.EmitOMPLoopBody(S, LoopExit);
                           CGF.EmitStopPoint(&S);
},
                         [](CodeGenFunction &) {
});
        EmitBlock(LoopExit.getBlock());
        RT.emitForStaticFinish(*this, S.getLocStart());
} else {
        EmitOMPDistributeOuterLoop(ScheduleKind, S, LoopScope,
                            LB.getAddress(), UB.getAddress(), ST.getAddress(),
                            IL.getAddress(), Chunk);
}
      if (HasLastprivateClause)
        EmitOMPLastprivateClauseFinal(
            S, 
false,
            Builder.CreateIsNotNull(
                EmitLoadOfScalar(IL, S.getLocStart())));
}
    if (ContBlock) {
      EmitBranch(ContBlock);
      EmitBlock(ContBlock, true);
}
}
}
void CodeGenFunction::EmitOMPDistributeDirective(
    const OMPDistributeDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    CGF.EmitOMPDistributeLoop(S);
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_distribute, CodeGen,
                                              false);
}
static llvm::Function *emitOutlinedOrderedFunction(CodeGenModule &CGM,
                                                   const CapturedStmt *S) {
  CodeGenFunction CGF(CGM, 
true);
  CodeGenFunction::CGCapturedStmtInfo CapStmtInfo;
  CGF.CapturedStmtInfo = &CapStmtInfo;
  auto *Fn = CGF.GenerateOpenMPCapturedStmtFunction(*S);
  Fn->addFnAttr(llvm::Attribute::NoInline);
  return Fn;
}
void CodeGenFunction::EmitOMPOrderedDirective(const OMPOrderedDirective &S) {
  if (!S.getAssociatedStmt()) {
for_loop_talkad7420();
    for (const auto *DC : S.getClausesOfKind<OMPDependClause>())
{
      CGM.getOpenMPRuntime().emitDoacrossOrdered(*this, DC);
}
    return;
}
  auto *C = S.getSingleClause<OMPSIMDClause>();
  auto &&CodeGen = [&S, C, this](CodeGenFunction &CGF,
                                 PrePostActionTy &Action) {
    if (C) {
      auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
      llvm::SmallVector<llvm::Value *, 16> CapturedVars;
      CGF.GenerateOpenMPCapturedVars(*CS, CapturedVars);
      auto *OutlinedFn = emitOutlinedOrderedFunction(CGM, CS);
      CGF.EmitNounwindRuntimeCall(OutlinedFn, CapturedVars);
} else {
      Action.Enter(CGF);
      CGF.EmitStmt(
          cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
}
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitOrderedRegion(*this, CodeGen, S.getLocStart(), !C);
}
static llvm::Value *convertToScalarValue(CodeGenFunction &CGF, RValue Val,
                                         QualType SrcType, QualType DestType,
                                         SourceLocation Loc) {
  assert(CGF.hasScalarEvaluationKind(DestType) &&
         "DestType must have scalar evaluation kind.");
  assert(!Val.isAggregate() && "Must be a scalar or complex.");
  return Val.isScalar()
             ? CGF.EmitScalarConversion(Val.getScalarVal(), SrcType, DestType,
                                        Loc)
             : CGF.EmitComplexToScalarConversion(Val.getComplexVal(), SrcType,
                                                 DestType, Loc);
}
static CodeGenFunction::ComplexPairTy
convertToComplexValue(CodeGenFunction &CGF, RValue Val, QualType SrcType,
                      QualType DestType, SourceLocation Loc) {
  assert(CGF.getEvaluationKind(DestType) == TEK_Complex &&
         "DestType must have complex evaluation kind.");
  CodeGenFunction::ComplexPairTy ComplexVal;
  if (Val.isScalar()) {
    auto DestElementType = DestType->castAs<ComplexType>()->getElementType();
    auto ScalarVal = CGF.EmitScalarConversion(Val.getScalarVal(), SrcType,
                                              DestElementType, Loc);
    ComplexVal = CodeGenFunction::ComplexPairTy(
        ScalarVal, llvm::Constant::getNullValue(ScalarVal->getType()));
} else {
    assert(Val.isComplex() && "Must be a scalar or complex.");
    auto SrcElementType = SrcType->castAs<ComplexType>()->getElementType();
    auto DestElementType = DestType->castAs<ComplexType>()->getElementType();
    ComplexVal.first = CGF.EmitScalarConversion(
        Val.getComplexVal().first, SrcElementType, DestElementType, Loc);
    ComplexVal.second = CGF.EmitScalarConversion(
        Val.getComplexVal().second, SrcElementType, DestElementType, Loc);
}
  return ComplexVal;
}
static void emitSimpleAtomicStore(CodeGenFunction &CGF, bool IsSeqCst,
                                  LValue LVal, RValue RVal) {
  if (LVal.isGlobalReg()) {
    CGF.EmitStoreThroughGlobalRegLValue(RVal, LVal);
} else {
    CGF.EmitAtomicStore(RVal, LVal,
                        IsSeqCst ? llvm::AtomicOrdering::SequentiallyConsistent
                                 : llvm::AtomicOrdering::Monotonic,
                        LVal.isVolatile(), 
false);
}
}
void CodeGenFunction::emitOMPSimpleStore(LValue LVal, RValue RVal,
                                         QualType RValTy, SourceLocation Loc) {
  switch (getEvaluationKind(LVal.getType())) {
  case TEK_Scalar:
    EmitStoreThroughLValue(RValue::get(convertToScalarValue(
                               *this, RVal, RValTy, LVal.getType(), Loc)),
                           LVal);
    break;
  case TEK_Complex:
    EmitStoreOfComplex(
        convertToComplexValue(*this, RVal, RValTy, LVal.getType(), Loc), LVal,
false);
    break;
  case TEK_Aggregate:
    llvm_unreachable("Must be a scalar or complex.");
}
}
const auto *IncClause = S.getSingleClause<OMPIncClause>();
  auto Vars = llvm::makeArrayRef(IncClause->varlist_begin(),
                                IncClause->varlist_end());
for_loop_talkad7420();
for(auto v:Vars) {
    auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*v).getDecl());
    CGM.getOpenMPRuntime().emitInc(*this, OrigVD, S.getLocStart());
}
static void EmitOMPAtomicReadExpr(CodeGenFunction &CGF, bool IsSeqCst,
                                  const Expr *X, const Expr *V,
                                  SourceLocation Loc) {
  assert(V->isLValue() && "V of 'omp atomic read' is not lvalue");
  assert(X->isLValue() && "X of 'omp atomic read' is not lvalue");
  LValue XLValue = CGF.EmitLValue(X);
  LValue VLValue = CGF.EmitLValue(V);
  RValue Res = XLValue.isGlobalReg()
                   ? CGF.EmitLoadOfLValue(XLValue, Loc)
                   : CGF.EmitAtomicLoad(
                         XLValue, Loc,
                         IsSeqCst ? llvm::AtomicOrdering::SequentiallyConsistent
                                  : llvm::AtomicOrdering::Monotonic,
                         XLValue.isVolatile());
  if (IsSeqCst)
    CGF.CGM.getOpenMPRuntime().emitFlush(CGF, llvm::None, Loc);
  CGF.emitOMPSimpleStore(VLValue, Res, X->getType().getNonReferenceType(), Loc);
}
static void EmitOMPAtomicWriteExpr(CodeGenFunction &CGF, bool IsSeqCst,
                                   const Expr *X, const Expr *E,
                                   SourceLocation Loc) {
  assert(X->isLValue() && "X of 'omp atomic write' is not lvalue");
  emitSimpleAtomicStore(CGF, IsSeqCst, CGF.EmitLValue(X), CGF.EmitAnyExpr(E));
  if (IsSeqCst)
    CGF.CGM.getOpenMPRuntime().emitFlush(CGF, llvm::None, Loc);
}
static std::pair<bool, RValue> emitOMPAtomicRMW(CodeGenFunction &CGF, LValue X,
                                                RValue Update,
                                                BinaryOperatorKind BO,
                                                llvm::AtomicOrdering AO,
                                                bool IsXLHSInRHSPart) {
  auto &Context = CGF.CGM.getContext();
  if (BO == BO_Comma || !Update.isScalar() ||
      !Update.getScalarVal()->getType()->isIntegerTy() ||
      !X.isSimple() || (!isa<llvm::ConstantInt>(Update.getScalarVal()) &&
                        (Update.getScalarVal()->getType() !=
                         X.getAddress().getElementType())) ||
      !X.getAddress().getElementType()->isIntegerTy() ||
      !Context.getTargetInfo().hasBuiltinAtomic(
          Context.getTypeSize(X.getType()), Context.toBits(X.getAlignment())))
    return std::make_pair(false, RValue::get(nullptr));
  llvm::AtomicRMWInst::BinOp RMWOp;
  switch (BO) {
  case BO_Add:
    RMWOp = llvm::AtomicRMWInst::Add;
    break;
  case BO_Sub:
    if (!IsXLHSInRHSPart)
      return std::make_pair(false, RValue::get(nullptr));
    RMWOp = llvm::AtomicRMWInst::Sub;
    break;
  case BO_And:
    RMWOp = llvm::AtomicRMWInst::And;
    break;
  case BO_Or:
    RMWOp = llvm::AtomicRMWInst::Or;
    break;
  case BO_Xor:
    RMWOp = llvm::AtomicRMWInst::Xor;
    break;
  case BO_LT:
    RMWOp = X.getType()->hasSignedIntegerRepresentation()
                ? (IsXLHSInRHSPart ? llvm::AtomicRMWInst::Min
                                   : llvm::AtomicRMWInst::Max)
                : (IsXLHSInRHSPart ? llvm::AtomicRMWInst::UMin
                                   : llvm::AtomicRMWInst::UMax);
    break;
  case BO_GT:
    RMWOp = X.getType()->hasSignedIntegerRepresentation()
                ? (IsXLHSInRHSPart ? llvm::AtomicRMWInst::Max
                                   : llvm::AtomicRMWInst::Min)
                : (IsXLHSInRHSPart ? llvm::AtomicRMWInst::UMax
                                   : llvm::AtomicRMWInst::UMin);
    break;
  case BO_Assign:
    RMWOp = llvm::AtomicRMWInst::Xchg;
    break;
  case BO_Mul:
  case BO_Div:
  case BO_Rem:
  case BO_Shl:
  case BO_Shr:
  case BO_LAnd:
  case BO_LOr:
    return std::make_pair(false, RValue::get(nullptr));
  case BO_PtrMemD:
  case BO_PtrMemI:
  case BO_LE:
  case BO_GE:
  case BO_EQ:
  case BO_NE:
  case BO_AddAssign:
  case BO_SubAssign:
  case BO_AndAssign:
  case BO_OrAssign:
  case BO_XorAssign:
  case BO_MulAssign:
  case BO_DivAssign:
  case BO_RemAssign:
  case BO_ShlAssign:
  case BO_ShrAssign:
  case BO_Comma:
    llvm_unreachable("Unsupported atomic update operation");
}
  auto *UpdateVal = Update.getScalarVal();
  if (auto *IC = dyn_cast<llvm::ConstantInt>(UpdateVal)) {
    UpdateVal = CGF.Builder.CreateIntCast(
        IC, X.getAddress().getElementType(),
        X.getType()->hasSignedIntegerRepresentation());
}
  auto *Res = CGF.Builder.CreateAtomicRMW(RMWOp, X.getPointer(), UpdateVal, AO);
  return std::make_pair(true, RValue::get(Res));
}
std::pair<bool, RValue> CodeGenFunction::EmitOMPAtomicSimpleUpdateExpr(
    LValue X, RValue E, BinaryOperatorKind BO, bool IsXLHSInRHSPart,
    llvm::AtomicOrdering AO, SourceLocation Loc,
    const llvm::function_ref<RValue(RValue)> &CommonGen) {
  auto Res = emitOMPAtomicRMW(*this, X, E, BO, AO, IsXLHSInRHSPart);
  if (!Res.first) {
    if (X.isGlobalReg()) {
      EmitStoreThroughLValue(CommonGen(EmitLoadOfLValue(X, Loc)), X);
} else {
      EmitAtomicUpdate(X, AO, CommonGen, X.getType().isVolatileQualified());
}
}
  return Res;
}
static void EmitOMPAtomicUpdateExpr(CodeGenFunction &CGF, bool IsSeqCst,
                                    const Expr *X, const Expr *E,
                                    const Expr *UE, bool IsXLHSInRHSPart,
                                    SourceLocation Loc) {
  assert(isa<BinaryOperator>(UE->IgnoreImpCasts()) &&
         "Update expr in 'atomic update' must be a binary operator.");
  auto *BOUE = cast<BinaryOperator>(UE->IgnoreImpCasts());
  assert(X->isLValue() && "X of 'omp atomic update' is not lvalue");
  LValue XLValue = CGF.EmitLValue(X);
  RValue ExprRValue = CGF.EmitAnyExpr(E);
  auto AO = IsSeqCst ? llvm::AtomicOrdering::SequentiallyConsistent
                     : llvm::AtomicOrdering::Monotonic;
  auto *LHS = cast<OpaqueValueExpr>(BOUE->getLHS()->IgnoreImpCasts());
  auto *RHS = cast<OpaqueValueExpr>(BOUE->getRHS()->IgnoreImpCasts());
  auto *XRValExpr = IsXLHSInRHSPart ? LHS : RHS;
  auto *ERValExpr = IsXLHSInRHSPart ? RHS : LHS;
  auto Gen =
      [&CGF, UE, ExprRValue, XRValExpr, ERValExpr](RValue XRValue) -> RValue {
        CodeGenFunction::OpaqueValueMapping MapExpr(CGF, ERValExpr, ExprRValue);
        CodeGenFunction::OpaqueValueMapping MapX(CGF, XRValExpr, XRValue);
        return CGF.EmitAnyExpr(UE);
};
  (void)CGF.EmitOMPAtomicSimpleUpdateExpr(
      XLValue, ExprRValue, BOUE->getOpcode(), IsXLHSInRHSPart, AO, Loc, Gen);
  if (IsSeqCst)
    CGF.CGM.getOpenMPRuntime().emitFlush(CGF, llvm::None, Loc);
}
static RValue convertToType(CodeGenFunction &CGF, RValue Value,
                            QualType SourceType, QualType ResType,
                            SourceLocation Loc) {
  switch (CGF.getEvaluationKind(ResType)) {
  case TEK_Scalar:
    return RValue::get(
        convertToScalarValue(CGF, Value, SourceType, ResType, Loc));
  case TEK_Complex: {
    auto Res = convertToComplexValue(CGF, Value, SourceType, ResType, Loc);
    return RValue::getComplex(Res.first, Res.second);
}
  case TEK_Aggregate:
    break;
}
  llvm_unreachable("Must be a scalar or complex.");
}
static void EmitOMPAtomicCaptureExpr(CodeGenFunction &CGF, bool IsSeqCst,
                                     bool IsPostfixUpdate, const Expr *V,
                                     const Expr *X, const Expr *E,
                                     const Expr *UE, bool IsXLHSInRHSPart,
                                     SourceLocation Loc) {
  assert(X->isLValue() && "X of 'omp atomic capture' is not lvalue");
  assert(V->isLValue() && "V of 'omp atomic capture' is not lvalue");
  RValue NewVVal;
  LValue VLValue = CGF.EmitLValue(V);
  LValue XLValue = CGF.EmitLValue(X);
  RValue ExprRValue = CGF.EmitAnyExpr(E);
  auto AO = IsSeqCst ? llvm::AtomicOrdering::SequentiallyConsistent
                     : llvm::AtomicOrdering::Monotonic;
  QualType NewVValType;
  if (UE) {
    assert(isa<BinaryOperator>(UE->IgnoreImpCasts()) &&
           "Update expr in 'atomic capture' must be a binary operator.");
    auto *BOUE = cast<BinaryOperator>(UE->IgnoreImpCasts());
    auto *LHS = cast<OpaqueValueExpr>(BOUE->getLHS()->IgnoreImpCasts());
    auto *RHS = cast<OpaqueValueExpr>(BOUE->getRHS()->IgnoreImpCasts());
    auto *XRValExpr = IsXLHSInRHSPart ? LHS : RHS;
    NewVValType = XRValExpr->getType();
    auto *ERValExpr = IsXLHSInRHSPart ? RHS : LHS;
    auto &&Gen = [&CGF, &NewVVal, UE, ExprRValue, XRValExpr, ERValExpr,
                  IsSeqCst, IsPostfixUpdate](RValue XRValue) -> RValue {
      CodeGenFunction::OpaqueValueMapping MapExpr(CGF, ERValExpr, ExprRValue);
      CodeGenFunction::OpaqueValueMapping MapX(CGF, XRValExpr, XRValue);
      RValue Res = CGF.EmitAnyExpr(UE);
      NewVVal = IsPostfixUpdate ? XRValue : Res;
      return Res;
};
    auto Res = CGF.EmitOMPAtomicSimpleUpdateExpr(
        XLValue, ExprRValue, BOUE->getOpcode(), IsXLHSInRHSPart, AO, Loc, Gen);
    if (Res.first) {
      if (IsPostfixUpdate) {
        NewVVal = Res.second;
} else {
        CodeGenFunction::OpaqueValueMapping MapExpr(CGF, ERValExpr, ExprRValue);
        CodeGenFunction::OpaqueValueMapping MapX(CGF, XRValExpr, Res.second);
        NewVVal = CGF.EmitAnyExpr(UE);
}
}
} else {
    NewVValType = X->getType().getNonReferenceType();
    ExprRValue = convertToType(CGF, ExprRValue, E->getType(),
                               X->getType().getNonReferenceType(), Loc);
    auto &&Gen = [&CGF, &NewVVal, ExprRValue](RValue XRValue) -> RValue {
      NewVVal = XRValue;
      return ExprRValue;
};
    auto Res = CGF.EmitOMPAtomicSimpleUpdateExpr(
        XLValue, ExprRValue, 
BO_Assign, 
false, AO,
        Loc, Gen);
    if (Res.first) {
      NewVVal = IsPostfixUpdate ? Res.second : ExprRValue;
}
}
  CGF.emitOMPSimpleStore(VLValue, NewVVal, NewVValType, Loc);
  if (IsSeqCst)
    CGF.CGM.getOpenMPRuntime().emitFlush(CGF, llvm::None, Loc);
}
static void EmitOMPAtomicExpr(CodeGenFunction &CGF, OpenMPClauseKind Kind,
                              bool IsSeqCst, bool IsPostfixUpdate,
                              const Expr *X, const Expr *V, const Expr *E,
                              const Expr *UE, bool IsXLHSInRHSPart,
                              SourceLocation Loc) {
  switch (Kind) {
  case OMPC_read:
    EmitOMPAtomicReadExpr(CGF, IsSeqCst, X, V, Loc);
    break;
  case OMPC_write:
    EmitOMPAtomicWriteExpr(CGF, IsSeqCst, X, E, Loc);
    break;
  case OMPC_unknown:
  case OMPC_update:
    EmitOMPAtomicUpdateExpr(CGF, IsSeqCst, X, E, UE, IsXLHSInRHSPart, Loc);
    break;
  case OMPC_capture:
    EmitOMPAtomicCaptureExpr(CGF, IsSeqCst, IsPostfixUpdate, V, X, E, UE,
                             IsXLHSInRHSPart, Loc);
    break;
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_reduction:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_default:
  case OMPC_seq_cst:
  case OMPC_shared:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_proc_bind:
  case OMPC_schedule:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_threadprivate:
  case OMPC_depend:
  case OMPC_mergeable:
  case OMPC_device:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_map:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_nogroup:
  case OMPC_num_tasks:
  case OMPC_hint:
  case OMPC_dist_schedule:
  case OMPC_defaultmap:
  case OMPC_uniform:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    llvm_unreachable("Clause is not allowed in 'omp atomic'.");
}
}
void CodeGenFunction::EmitOMPAtomicDirective(const OMPAtomicDirective &S) {
  bool IsSeqCst = S.getSingleClause<OMPSeqCstClause>();
  OpenMPClauseKind Kind = OMPC_unknown;
for_loop_talkad7420();
  for (auto *C : S.clauses()) {
    if (C->getClauseKind() != OMPC_seq_cst) {
      Kind = C->getClauseKind();
      break;
}
}
  const auto *CS =
      S.getAssociatedStmt()->IgnoreContainers(
true);
  if (const auto *EWC = dyn_cast<ExprWithCleanups>(CS)) {
    enterFullExpression(EWC);
}
  if (const auto *Compound = dyn_cast<CompoundStmt>(CS)) {
for_loop_talkad7420();
    for (const auto *C : Compound->body()) {
      if (const auto *EWC = dyn_cast<ExprWithCleanups>(C)) {
        enterFullExpression(EWC);
}
}
}
  auto &&CodeGen = [&S, Kind, IsSeqCst, CS](CodeGenFunction &CGF,
                                            PrePostActionTy &) {
    CGF.EmitStopPoint(CS);
    EmitOMPAtomicExpr(CGF, Kind, IsSeqCst, S.isPostfixUpdate(), S.getX(),
                      S.getV(), S.getExpr(), S.getUpdateExpr(),
                      S.isXLHSInRHSPart(), S.getLocStart());
};
  OMPLexicalScope Scope(*this, S, 
true);
  CGM.getOpenMPRuntime().emitInlinedDirective(*this, OMPD_atomic, CodeGen);
}
std::pair<llvm::Function * 
, llvm::Constant * 
>
CodeGenFunction::EmitOMPTargetDirectiveOutlinedFunction(
    CodeGenModule &CGM, const OMPTargetDirective &S, StringRef ParentName,
    bool IsOffloadEntry) {
  llvm::Function *OutlinedFn = nullptr;
  llvm::Constant *OutlinedFnID = nullptr;
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &Action) {
    OMPPrivateScope PrivateScope(CGF);
    (void)CGF.EmitOMPFirstprivateClause(S, PrivateScope);
    CGF.EmitOMPPrivateClause(S, PrivateScope);
    (void)PrivateScope.Privatize();
    Action.Enter(CGF);
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  CGM.getOpenMPRuntime().emitTargetOutlinedFunction(
      S, ParentName, OutlinedFn, OutlinedFnID, IsOffloadEntry, CodeGen);
  return std::make_pair(OutlinedFn, OutlinedFnID);
}
void CodeGenFunction::EmitOMPTargetDirective(const OMPTargetDirective &S) {
  const CapturedStmt &CS = *cast<CapturedStmt>(S.getAssociatedStmt());
  llvm::SmallVector<llvm::Value *, 16> CapturedVars;
  GenerateOpenMPCapturedVars(CS, CapturedVars);
  llvm::Function *Fn = nullptr;
  llvm::Constant *FnID = nullptr;
  const Expr *IfCond = nullptr;
  if (auto *C = S.getSingleClause<OMPIfClause>()) {
    IfCond = C->getCondition();
}
  const Expr *Device = nullptr;
  if (auto *C = S.getSingleClause<OMPDeviceClause>()) {
    Device = C->getDevice();
}
  bool IsOffloadEntry = true;
  if (IfCond) {
    bool Val;
    if (ConstantFoldsToSimpleInteger(IfCond, Val) && !Val)
      IsOffloadEntry = false;
}
  if (CGM.getLangOpts().OMPTargetTriples.empty())
    IsOffloadEntry = false;
  assert(CurFuncDecl && "No parent declaration for target region!");
  StringRef ParentName;
  if (auto *D = dyn_cast<CXXConstructorDecl>(CurFuncDecl))
    ParentName = CGM.getMangledName(GlobalDecl(D, Ctor_Complete));
  else if (auto *D = dyn_cast<CXXDestructorDecl>(CurFuncDecl))
    ParentName = CGM.getMangledName(GlobalDecl(D, Dtor_Complete));
  else
    ParentName =
        CGM.getMangledName(GlobalDecl(cast<FunctionDecl>(CurFuncDecl)));
  std::tie(Fn, FnID) = EmitOMPTargetDirectiveOutlinedFunction(
      CGM, S, ParentName, IsOffloadEntry);
  OMPLexicalScope Scope(*this, S);
  CGM.getOpenMPRuntime().emitTargetCall(*this, S, Fn, FnID, IfCond, Device,
                                        CapturedVars);
}
static void emitCommonOMPTeamsDirective(CodeGenFunction &CGF,
                                        const OMPExecutableDirective &S,
                                        OpenMPDirectiveKind InnermostKind,
                                        const RegionCodeGenTy &CodeGen) {
  auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
  auto OutlinedFn = CGF.CGM.getOpenMPRuntime().
      emitParallelOrTeamsOutlinedFunction(S,
          *CS->getCapturedDecl()->param_begin(), InnermostKind, CodeGen);
  const OMPTeamsDirective &TD = *dyn_cast<OMPTeamsDirective>(&S);
  const OMPNumTeamsClause *NT = TD.getSingleClause<OMPNumTeamsClause>();
  const OMPThreadLimitClause *TL = TD.getSingleClause<OMPThreadLimitClause>();
  if (NT || TL) {
    Expr *NumTeams = (NT) ? NT->getNumTeams() : nullptr;
    Expr *ThreadLimit = (TL) ? TL->getThreadLimit() : nullptr;
    CGF.CGM.getOpenMPRuntime().emitNumTeamsClause(CGF, NumTeams, ThreadLimit,
                                                  S.getLocStart());
}
  OMPLexicalScope Scope(CGF, S);
  llvm::SmallVector<llvm::Value *, 16> CapturedVars;
  CGF.GenerateOpenMPCapturedVars(*CS, CapturedVars);
  CGF.CGM.getOpenMPRuntime().emitTeamsCall(CGF, S, S.getLocStart(), OutlinedFn,
                                           CapturedVars);
}
void CodeGenFunction::EmitOMPTeamsDirective(const OMPTeamsDirective &S) {
  auto &&CodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
    OMPPrivateScope PrivateScope(CGF);
    (void)CGF.EmitOMPFirstprivateClause(S, PrivateScope);
    CGF.EmitOMPPrivateClause(S, PrivateScope);
    (void)PrivateScope.Privatize();
    CGF.EmitStmt(cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
  emitCommonOMPTeamsDirective(*this, S, OMPD_teams, CodeGen);
}
void CodeGenFunction::EmitOMPCancellationPointDirective(
    const OMPCancellationPointDirective &S) {
  CGM.getOpenMPRuntime().emitCancellationPointCall(*this, S.getLocStart(),
                                                   S.getCancelRegion());
}
void CodeGenFunction::EmitOMPCancelDirective(const OMPCancelDirective &S) {
  const Expr *IfCond = nullptr;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPIfClause>()) {
    if (C->getNameModifier() == OMPD_unknown ||
        C->getNameModifier() == OMPD_cancel) {
      IfCond = C->getCondition();
      break;
}
}
  CGM.getOpenMPRuntime().emitCancelCall(*this, S.getLocStart(), IfCond,
                                        S.getCancelRegion());
}
CodeGenFunction::JumpDest
CodeGenFunction::getOMPCancelDestination(OpenMPDirectiveKind Kind) {
  if (Kind == OMPD_parallel || Kind == OMPD_task ||
      Kind == OMPD_target_parallel)
    return ReturnBlock;
  assert(Kind == OMPD_for || Kind == OMPD_section || Kind == OMPD_sections ||
         Kind == OMPD_parallel_sections || Kind == OMPD_parallel_for ||
         Kind == OMPD_distribute_parallel_for ||
         Kind == OMPD_target_parallel_for);
  return OMPCancelStack.getExitBlock();
}
void CodeGenFunction::EmitOMPUseDevicePtrClause(
    const OMPClause &NC, OMPPrivateScope &PrivateScope,
    const llvm::DenseMap<const ValueDecl *, Address> &CaptureDeviceAddrMap) {
  const auto &C = cast<OMPUseDevicePtrClause>(NC);
  auto OrigVarIt = C.varlist_begin();
  auto InitIt = C.inits().begin();
for_loop_talkad7420();
  for (auto PvtVarIt : C.private_copies()) {
    auto *OrigVD = cast<VarDecl>(cast<DeclRefExpr>(*OrigVarIt)->getDecl());
    auto *InitVD = cast<VarDecl>(cast<DeclRefExpr>(*InitIt)->getDecl());
    auto *PvtVD = cast<VarDecl>(cast<DeclRefExpr>(PvtVarIt)->getDecl());
    const ValueDecl *MatchingVD = OrigVD;
    if (auto *OED = dyn_cast<OMPCapturedExprDecl>(MatchingVD)) {
      auto *ME = cast<MemberExpr>(OED->getInit());
      assert(isa<CXXThisExpr>(ME->getBase()) &&
             "Base should be the current struct!");
      MatchingVD = ME->getMemberDecl();
}
    auto InitAddrIt = CaptureDeviceAddrMap.find(MatchingVD);
    if (InitAddrIt == CaptureDeviceAddrMap.end())
      continue;
    bool IsRegistered = PrivateScope.addPrivate(OrigVD, [&]() -> Address {
      QualType AddrQTy =
          getContext().getPointerType(OrigVD->getType().getNonReferenceType());
      llvm::Type *AddrTy = ConvertTypeForMem(AddrQTy);
      Address InitAddr = Builder.CreateBitCast(InitAddrIt->second, AddrTy);
      setAddrOfLocalVar(InitVD, InitAddr);
      EmitDecl(*PvtVD);
      LocalDeclMap.erase(InitVD);
      return GetAddrOfLocalVar(PvtVD);
});
    assert(IsRegistered && "firstprivate var already registered as private");
    (void)IsRegistered;
    ++OrigVarIt;
    ++InitIt;
}
}
void CodeGenFunction::EmitOMPTargetDataDirective(
    const OMPTargetDataDirective &S) {
  CGOpenMPRuntime::TargetDataInfo Info(
true);
  bool PrivatizeDevicePointers = false;
  class DevicePointerPrivActionTy : public PrePostActionTy {
    bool &PrivatizeDevicePointers;
  public:
    explicit DevicePointerPrivActionTy(bool &PrivatizeDevicePointers)
        : PrePostActionTy(), PrivatizeDevicePointers(PrivatizeDevicePointers) {
}
    void Enter(CodeGenFunction &CGF) override {
      PrivatizeDevicePointers = true;
}
};
  DevicePointerPrivActionTy PrivAction(PrivatizeDevicePointers);
  auto &&CodeGen = [&S, &Info, &PrivatizeDevicePointers](
      CodeGenFunction &CGF, PrePostActionTy &Action) {
    auto &&InnermostCodeGen = [&S](CodeGenFunction &CGF, PrePostActionTy &) {
      CGF.EmitStmt(
          cast<CapturedStmt>(S.getAssociatedStmt())->getCapturedStmt());
};
    auto &&PrivCodeGen = [&S, &Info, &PrivatizeDevicePointers,
                          &InnermostCodeGen](CodeGenFunction &CGF,
                                             PrePostActionTy &Action) {
      RegionCodeGenTy RCG(InnermostCodeGen);
      PrivatizeDevicePointers = false;
      Action.Enter(CGF);
      if (PrivatizeDevicePointers) {
        OMPPrivateScope PrivateScope(CGF);
for_loop_talkad7420();
        for (const auto *C : S.getClausesOfKind<OMPUseDevicePtrClause>())
{
          CGF.EmitOMPUseDevicePtrClause(*C, PrivateScope,
                                        Info.CaptureDeviceAddrMap);
}
        (void)PrivateScope.Privatize();
        RCG(CGF);
} else
        RCG(CGF);
};
    RegionCodeGenTy PrivRCG(PrivCodeGen);
    PrivRCG.setAction(Action);
    OMPLexicalScope Scope(CGF, S);
    CGF.CGM.getOpenMPRuntime().emitInlinedDirective(CGF, OMPD_target_data,
                                                    PrivRCG);
};
  RegionCodeGenTy RCG(CodeGen);
  if (CGM.getLangOpts().OMPTargetTriples.empty()) {
    RCG(*this);
    return;
}
  const Expr *IfCond = nullptr;
  if (auto *C = S.getSingleClause<OMPIfClause>())
    IfCond = C->getCondition();
  const Expr *Device = nullptr;
  if (auto *C = S.getSingleClause<OMPDeviceClause>())
    Device = C->getDevice();
  RCG.setAction(PrivAction);
  CGM.getOpenMPRuntime().emitTargetDataCalls(*this, S, IfCond, Device, RCG,
                                             Info);
}
void CodeGenFunction::EmitOMPTargetEnterDataDirective(
    const OMPTargetEnterDataDirective &S) {
  if (CGM.getLangOpts().OMPTargetTriples.empty())
    return;
  const Expr *IfCond = nullptr;
  if (auto *C = S.getSingleClause<OMPIfClause>())
    IfCond = C->getCondition();
  const Expr *Device = nullptr;
  if (auto *C = S.getSingleClause<OMPDeviceClause>())
    Device = C->getDevice();
  CGM.getOpenMPRuntime().emitTargetDataStandAloneCall(*this, S, IfCond, Device);
}
void CodeGenFunction::EmitOMPTargetExitDataDirective(
    const OMPTargetExitDataDirective &S) {
  if (CGM.getLangOpts().OMPTargetTriples.empty())
    return;
  const Expr *IfCond = nullptr;
  if (auto *C = S.getSingleClause<OMPIfClause>())
    IfCond = C->getCondition();
  const Expr *Device = nullptr;
  if (auto *C = S.getSingleClause<OMPDeviceClause>())
    Device = C->getDevice();
  CGM.getOpenMPRuntime().emitTargetDataStandAloneCall(*this, S, IfCond, Device);
}
void CodeGenFunction::EmitOMPTargetParallelDirective(
    const OMPTargetParallelDirective &S) {
}
void CodeGenFunction::EmitOMPTargetParallelForDirective(
    const OMPTargetParallelForDirective &S) {
}
static void mapParam(CodeGenFunction &CGF, const DeclRefExpr *Helper,
                     const ImplicitParamDecl *PVD,
                     CodeGenFunction::OMPPrivateScope &Privates) {
  auto *VDecl = cast<VarDecl>(Helper->getDecl());
  Privates.addPrivate(
      VDecl, [&CGF, PVD]() -> Address { return CGF.GetAddrOfLocalVar(PVD); 
});
}
void CodeGenFunction::EmitOMPTaskLoopBasedDirective(const OMPLoopDirective &S) {
  assert(isOpenMPTaskLoopDirective(S.getDirectiveKind()));
  auto CS = cast<CapturedStmt>(S.getAssociatedStmt());
  auto CapturedStruct = GenerateCapturedStmtArgument(*CS);
  auto SharedsTy = getContext().getRecordType(CS->getCapturedRecordDecl());
  const Expr *IfCond = nullptr;
for_loop_talkad7420();
  for (const auto *C : S.getClausesOfKind<OMPIfClause>()) {
    if (C->getNameModifier() == OMPD_unknown ||
        C->getNameModifier() == OMPD_taskloop) {
      IfCond = C->getCondition();
      break;
}
}
  OMPTaskDataTy Data;
  Data.Nogroup = S.getSingleClause<OMPNogroupClause>();
  Data.Tied = true;
  if (const auto* Clause = S.getSingleClause<OMPGrainsizeClause>()) {
    Data.Schedule.setInt(
false);
    Data.Schedule.setPointer(EmitScalarExpr(Clause->getGrainsize()));
} else if (const auto* Clause = S.getSingleClause<OMPNumTasksClause>()) {
    Data.Schedule.setInt(
true);
    Data.Schedule.setPointer(EmitScalarExpr(Clause->getNumTasks()));
}
  auto &&BodyGen = [CS, &S](CodeGenFunction &CGF, PrePostActionTy &) {
    bool CondConstant;
    llvm::BasicBlock *ContBlock = nullptr;
    OMPLoopScope PreInitScope(CGF, S);
    if (CGF.ConstantFoldsToSimpleInteger(S.getPreCond(), CondConstant)) {
      if (!CondConstant)
        return;
} else {
      auto *ThenBlock = CGF.createBasicBlock("taskloop.if.then");
      ContBlock = CGF.createBasicBlock("taskloop.if.end");
      emitPreCond(CGF, S, S.getPreCond(), ThenBlock, ContBlock,
                  CGF.getProfileCount(&S));
      CGF.EmitBlock(ThenBlock);
      CGF.incrementProfileCounter(&S);
}
    if (isOpenMPSimdDirective(S.getDirectiveKind()))
      CGF.EmitOMPSimdInit(S);
    OMPPrivateScope LoopScope(CGF);
    enum { LowerBound = 5, UpperBound, Stride, LastIter 
};
    auto *I = CS->getCapturedDecl()->param_begin();
    auto *LBP = std::next(I, LowerBound);
    auto *UBP = std::next(I, UpperBound);
    auto *STP = std::next(I, Stride);
    auto *LIP = std::next(I, LastIter);
    mapParam(CGF, cast<DeclRefExpr>(S.getLowerBoundVariable()), *LBP,
             LoopScope);
    mapParam(CGF, cast<DeclRefExpr>(S.getUpperBoundVariable()), *UBP,
             LoopScope);
    mapParam(CGF, cast<DeclRefExpr>(S.getStrideVariable()), *STP, LoopScope);
    mapParam(CGF, cast<DeclRefExpr>(S.getIsLastIterVariable()), *LIP,
             LoopScope);
    CGF.EmitOMPPrivateLoopCounters(S, LoopScope);
    bool HasLastprivateClause = CGF.EmitOMPLastprivateClauseInit(S, LoopScope);
    (void)LoopScope.Privatize();
    const Expr *IVExpr = S.getIterationVariable();
    const VarDecl *IVDecl = cast<VarDecl>(cast<DeclRefExpr>(IVExpr)->getDecl());
    CGF.EmitVarDecl(*IVDecl);
    CGF.EmitIgnoredExpr(S.getInit());
    if (auto LIExpr = dyn_cast<DeclRefExpr>(S.getLastIteration())) {
      CGF.EmitVarDecl(*cast<VarDecl>(LIExpr->getDecl()));
      CGF.EmitIgnoredExpr(S.getCalcLastIteration());
}
    CGF.EmitOMPInnerLoop(S, LoopScope.requiresCleanups(), S.getCond(),
                         S.getInc(),
                         [&S](CodeGenFunction &CGF) {
                           CGF.EmitOMPLoopBody(S, JumpDest());
                           CGF.EmitStopPoint(&S);
},
                         [](CodeGenFunction &) {
});
    if (ContBlock) {
      CGF.EmitBranch(ContBlock);
      CGF.EmitBlock(ContBlock, true);
}
    if (HasLastprivateClause) {
      CGF.EmitOMPLastprivateClauseFinal(
          S, isOpenMPSimdDirective(S.getDirectiveKind()),
          CGF.Builder.CreateIsNotNull(CGF.EmitLoadOfScalar(
              CGF.GetAddrOfLocalVar(*LIP), 
false,
              (*LIP)->getType(), S.getLocStart())));
}
};
  auto &&TaskGen = [&S, SharedsTy, CapturedStruct,
                    IfCond](CodeGenFunction &CGF, llvm::Value *OutlinedFn,
                            const OMPTaskDataTy &Data) {
    auto &&CodeGen = [&](CodeGenFunction &CGF, PrePostActionTy &) {
      OMPLoopScope PreInitScope(CGF, S);
      CGF.CGM.getOpenMPRuntime().emitTaskLoopCall(CGF, S.getLocStart(), S,
                                                  OutlinedFn, SharedsTy,
                                                  CapturedStruct, IfCond, Data);
};
    CGF.CGM.getOpenMPRuntime().emitInlinedDirective(CGF, OMPD_taskloop,
                                                    CodeGen);
};
  EmitOMPTaskBasedDirective(S, BodyGen, TaskGen, Data);
}
void CodeGenFunction::EmitOMPTaskLoopDirective(const OMPTaskLoopDirective &S) {
  EmitOMPTaskLoopBasedDirective(S);
}
void CodeGenFunction::EmitOMPTaskLoopSimdDirective(
    const OMPTaskLoopSimdDirective &S) {
  EmitOMPTaskLoopBasedDirective(S);
}
void CodeGenFunction::EmitOMPTargetUpdateDirective(
    const OMPTargetUpdateDirective &S) {
  if (CGM.getLangOpts().OMPTargetTriples.empty())
    return;
  const Expr *IfCond = nullptr;
  if (auto *C = S.getSingleClause<OMPIfClause>())
    IfCond = C->getCondition();
  const Expr *Device = nullptr;
  if (auto *C = S.getSingleClause<OMPDeviceClause>())
    Device = C->getDevice();
  CGM.getOpenMPRuntime().emitTargetDataStandAloneCall(*this, S, IfCond, Device);
}