#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
#include "TreeTransform.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTMutationListener.h"
#include "clang/AST/CXXInheritance.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclOpenMP.h"
#include "clang/AST/StmtCXX.h"
#include "clang/AST/StmtOpenMP.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/AST/TypeOrdering.h"
#include "clang/Basic/OpenMPKinds.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Initialization.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Scope.h"
#include "clang/Sema/ScopeInfo.h"
#include "clang/Sema/SemaInternal.h"
using namespace clang;
namespace {
enum DefaultDataSharingAttributes {
  DSA_unspecified = 0, 
  DSA_none = 1 << 0,   
  DSA_shared = 1 << 1  
};
class DSAStackTy final {
public:
  struct DSAVarData final {
    OpenMPDirectiveKind DKind = OMPD_unknown;
    OpenMPClauseKind CKind = OMPC_unknown;
    Expr *RefExpr = nullptr;
    DeclRefExpr *PrivateCopy = nullptr;
    SourceLocation ImplicitDSALoc;
    DSAVarData() {
}
};
  typedef llvm::SmallVector<std::pair<Expr *, OverloadedOperatorKind>, 4>
      OperatorOffsetTy;
private:
  struct DSAInfo final {
    OpenMPClauseKind Attributes = OMPC_unknown;
    llvm::PointerIntPair<Expr *, 1, bool> RefExpr;
    DeclRefExpr *PrivateCopy = nullptr;
};
  typedef llvm::DenseMap<ValueDecl *, DSAInfo> DeclSAMapTy;
  typedef llvm::DenseMap<ValueDecl *, Expr *> AlignedMapTy;
  typedef std::pair<unsigned, VarDecl *> LCDeclInfo;
  typedef llvm::DenseMap<ValueDecl *, LCDeclInfo> LoopControlVariablesMapTy;
  struct MappedExprComponentTy {
    OMPClauseMappableExprCommon::MappableExprComponentLists Components;
    OpenMPClauseKind Kind = OMPC_unknown;
};
  typedef llvm::DenseMap<ValueDecl *, MappedExprComponentTy>
      MappedExprComponentsTy;
  typedef llvm::StringMap<std::pair<OMPCriticalDirective *, llvm::APSInt>>
      CriticalsWithHintsTy;
  typedef llvm::DenseMap<OMPDependClause *, OperatorOffsetTy>
      DoacrossDependMapTy;
  struct SharingMapTy final {
    DeclSAMapTy SharingMap;
    AlignedMapTy AlignedMap;
    MappedExprComponentsTy MappedExprComponents;
    LoopControlVariablesMapTy LCVMap;
    DefaultDataSharingAttributes DefaultAttr = DSA_unspecified;
    SourceLocation DefaultAttrLoc;
    OpenMPDirectiveKind Directive = OMPD_unknown;
    DeclarationNameInfo DirectiveName;
    Scope *CurScope = nullptr;
    SourceLocation ConstructLoc;
    DoacrossDependMapTy DoacrossDepends;
    llvm::PointerIntPair<Expr *, 1, bool> OrderedRegion;
    bool NowaitRegion = false;
    bool CancelRegion = false;
    unsigned AssociatedLoops = 1;
    SourceLocation InnerTeamsRegionLoc;
    SharingMapTy(OpenMPDirectiveKind DKind, DeclarationNameInfo Name,
                 Scope *CurScope, SourceLocation Loc)
        : Directive(DKind), DirectiveName(Name), CurScope(CurScope),
          ConstructLoc(Loc) {
}
    SharingMapTy() {
}
};
  typedef SmallVector<SharingMapTy, 4> StackTy;
  StackTy Stack;
  OpenMPClauseKind ClauseKindMode = OMPC_unknown;
  Sema &SemaRef;
  bool ForceCapturing = false;
  CriticalsWithHintsTy Criticals;
  typedef SmallVector<SharingMapTy, 8>::reverse_iterator reverse_iterator;
  DSAVarData getDSA(StackTy::reverse_iterator &Iter, ValueDecl *D);
  bool isOpenMPLocal(VarDecl *D, StackTy::reverse_iterator Iter);
public:
  explicit DSAStackTy(Sema &S) : Stack(1), SemaRef(S) {
}
  bool isClauseParsingMode() const { return ClauseKindMode != OMPC_unknown; 
}
  void setClauseParsingMode(OpenMPClauseKind K) { ClauseKindMode = K; 
}
  bool isForceVarCapturing() const { return ForceCapturing; 
}
  void setForceVarCapturing(bool V) { ForceCapturing = V; 
}
  void push(OpenMPDirectiveKind DKind, const DeclarationNameInfo &DirName,
            Scope *CurScope, SourceLocation Loc) {
    Stack.push_back(SharingMapTy(DKind, DirName, CurScope, Loc));
    Stack.back().DefaultAttrLoc = Loc;
}
  void pop() {
    assert(Stack.size() > 1 && "Data-sharing attributes stack is empty!");
    Stack.pop_back();
}
  void addCriticalWithHint(OMPCriticalDirective *D, llvm::APSInt Hint) {
    Criticals[D->getDirectiveName().getAsString()] = std::make_pair(D, Hint);
}
  const std::pair<OMPCriticalDirective *, llvm::APSInt>
  getCriticalWithHint(const DeclarationNameInfo &Name) const {
    auto I = Criticals.find(Name.getAsString());
    if (I != Criticals.end())
      return I->second;
    return std::make_pair(nullptr, llvm::APSInt());
}
  Expr *addUniqueAligned(ValueDecl *D, Expr *NewDE);
  void addLoopControlVariable(ValueDecl *D, VarDecl *Capture);
  LCDeclInfo isLoopControlVariable(ValueDecl *D);
  LCDeclInfo isParentLoopControlVariable(ValueDecl *D);
  ValueDecl *getParentLoopControlVariable(unsigned I);
  void addDSA(ValueDecl *D, Expr *E, OpenMPClauseKind A,
              DeclRefExpr *PrivateCopy = nullptr);
  DSAVarData getTopDSA(ValueDecl *D, bool FromParent);
  DSAVarData getImplicitDSA(ValueDecl *D, bool FromParent);
  DSAVarData hasDSA(ValueDecl *D,
                    const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
                    const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
                    bool FromParent);
  DSAVarData
  hasInnermostDSA(ValueDecl *D,
                  const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
                  const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
                  bool FromParent);
  bool hasExplicitDSA(ValueDecl *D,
                      const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
                      unsigned Level, bool NotLastprivate = false);
  bool hasExplicitDirective(
      const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
      unsigned Level);
  bool hasDirective(const llvm::function_ref<bool(OpenMPDirectiveKind,
                                                  const DeclarationNameInfo &,
                                                  SourceLocation)> &DPred,
                    bool FromParent);
  OpenMPDirectiveKind getCurrentDirective() const {
    return Stack.back().Directive;
}
  OpenMPDirectiveKind getParentDirective() const {
    if (Stack.size() > 2)
      return Stack[Stack.size() - 2].Directive;
    return OMPD_unknown;
}
  void setDefaultDSANone(SourceLocation Loc) {
    Stack.back().DefaultAttr = DSA_none;
    Stack.back().DefaultAttrLoc = Loc;
}
  void setDefaultDSAShared(SourceLocation Loc) {
    Stack.back().DefaultAttr = DSA_shared;
    Stack.back().DefaultAttrLoc = Loc;
}
  DefaultDataSharingAttributes getDefaultDSA() const {
    return Stack.back().DefaultAttr;
}
  SourceLocation getDefaultDSALocation() const {
    return Stack.back().DefaultAttrLoc;
}
  bool isThreadPrivate(VarDecl *D) {
    DSAVarData DVar = getTopDSA(D, false);
    return isOpenMPThreadPrivate(DVar.CKind);
}
  void setOrderedRegion(bool IsOrdered, Expr *Param) {
    Stack.back().OrderedRegion.setInt(IsOrdered);
    Stack.back().OrderedRegion.setPointer(Param);
}
  bool isParentOrderedRegion() const {
    if (Stack.size() > 2)
      return Stack[Stack.size() - 2].OrderedRegion.getInt();
    return false;
}
  Expr *getParentOrderedRegionParam() const {
    if (Stack.size() > 2)
      return Stack[Stack.size() - 2].OrderedRegion.getPointer();
    return nullptr;
}
  void setNowaitRegion(bool IsNowait = true) {
    Stack.back().NowaitRegion = IsNowait;
}
  bool isParentNowaitRegion() const {
    if (Stack.size() > 2)
      return Stack[Stack.size() - 2].NowaitRegion;
    return false;
}
  void setParentCancelRegion(bool Cancel = true) {
    if (Stack.size() > 2)
      Stack[Stack.size() - 2].CancelRegion =
          Stack[Stack.size() - 2].CancelRegion || Cancel;
}
  bool isCancelRegion() const { return Stack.back().CancelRegion; 
}
  void setAssociatedLoops(unsigned Val) { Stack.back().AssociatedLoops = Val; 
}
  unsigned getAssociatedLoops() const { return Stack.back().AssociatedLoops; 
}
  void setParentTeamsRegionLoc(SourceLocation TeamsRegionLoc) {
    if (Stack.size() > 2)
      Stack[Stack.size() - 2].InnerTeamsRegionLoc = TeamsRegionLoc;
}
  bool hasInnerTeamsRegion() const {
    return getInnerTeamsRegionLoc().isValid();
}
  SourceLocation getInnerTeamsRegionLoc() const {
    if (Stack.size() > 1)
      return Stack.back().InnerTeamsRegionLoc;
    return SourceLocation();
}
  Scope *getCurScope() const { return Stack.back().CurScope; 
}
  Scope *getCurScope() { return Stack.back().CurScope; 
}
  SourceLocation getConstructLoc() { return Stack.back().ConstructLoc; 
}
  bool checkMappableExprComponentListsForDecl(
      ValueDecl *VD, bool CurrentRegionOnly,
      const llvm::function_ref<
          bool(OMPClauseMappableExprCommon::MappableExprComponentListRef,
               OpenMPClauseKind)> &Check) {
    auto SI = Stack.rbegin();
    auto SE = Stack.rend();
    if (SI == SE)
      return false;
    if (CurrentRegionOnly) {
      SE = std::next(SI);
} else {
      ++SI;
}
for_loop_talkad7420();
    for (; SI != SE; ++SI) {
      auto MI = SI->MappedExprComponents.find(VD);
      if (MI != SI->MappedExprComponents.end())
for_loop_talkad7420();
        for (auto &L : MI->second.Components)
{
          if (Check(L, MI->second.Kind))
            return true;
}
}
    return false;
}
  void addMappableExpressionComponents(
      ValueDecl *VD,
      OMPClauseMappableExprCommon::MappableExprComponentListRef Components,
      OpenMPClauseKind WhereFoundClauseKind) {
    assert(Stack.size() > 1 &&
           "Not expecting to retrieve components from a empty stack!");
    auto &MEC = Stack.back().MappedExprComponents[VD];
    MEC.Components.resize(MEC.Components.size() + 1);
    MEC.Components.back().append(Components.begin(), Components.end());
    MEC.Kind = WhereFoundClauseKind;
}
  unsigned getNestingLevel() const {
    assert(Stack.size() > 1);
    return Stack.size() - 2;
}
  void addDoacrossDependClause(OMPDependClause *C, OperatorOffsetTy &OpsOffs) {
    assert(Stack.size() > 2);
    assert(isOpenMPWorksharingDirective(Stack[Stack.size() - 2].Directive));
    Stack[Stack.size() - 2].DoacrossDepends.insert({C, OpsOffs
});
}
  llvm::iterator_range<DoacrossDependMapTy::const_iterator>
  getDoacrossDependClauses() const {
    assert(Stack.size() > 1);
    if (isOpenMPWorksharingDirective(Stack[Stack.size() - 1].Directive)) {
      auto &Ref = Stack[Stack.size() - 1].DoacrossDepends;
      return llvm::make_range(Ref.begin(), Ref.end());
}
    return llvm::make_range(Stack[0].DoacrossDepends.end(),
                            Stack[0].DoacrossDepends.end());
}
};
bool isParallelOrTaskRegion(OpenMPDirectiveKind DKind) {
  return isOpenMPParallelDirective(DKind) || isOpenMPTaskingDirective(DKind) ||
         isOpenMPTeamsDirective(DKind) || DKind == OMPD_unknown;
}
} 
static ValueDecl *getCanonicalDecl(ValueDecl *D) {
  auto *VD = dyn_cast<VarDecl>(D);
  auto *FD = dyn_cast<FieldDecl>(D);
  if (VD != nullptr) {
    VD = VD->getCanonicalDecl();
    D = VD;
} else {
    assert(FD);
    FD = FD->getCanonicalDecl();
    D = FD;
}
  return D;
}
DSAStackTy::DSAVarData DSAStackTy::getDSA(StackTy::reverse_iterator &Iter,
                                          ValueDecl *D) {
  D = getCanonicalDecl(D);
  auto *VD = dyn_cast<VarDecl>(D);
  auto *FD = dyn_cast<FieldDecl>(D);
  DSAVarData DVar;
  if (Iter == std::prev(Stack.rend())) {
    if (VD && !VD->isFunctionOrMethodVarDecl() && !isa<ParmVarDecl>(D))
      DVar.CKind = OMPC_shared;
    if (VD && VD->hasGlobalStorage())
      DVar.CKind = OMPC_shared;
    if (FD)
      DVar.CKind = OMPC_shared;
    return DVar;
}
  DVar.DKind = Iter->Directive;
  if (VD && isOpenMPLocal(VD, Iter) && VD->isLocalVarDecl() &&
      (VD->getStorageClass() == SC_Auto || VD->getStorageClass() == SC_None)) {
    DVar.CKind = OMPC_private;
    return DVar;
}
  if (Iter->SharingMap.count(D)) {
    DVar.RefExpr = Iter->SharingMap[D].RefExpr.getPointer();
    DVar.PrivateCopy = Iter->SharingMap[D].PrivateCopy;
    DVar.CKind = Iter->SharingMap[D].Attributes;
    DVar.ImplicitDSALoc = Iter->DefaultAttrLoc;
    return DVar;
}
  switch (Iter->DefaultAttr) {
  case DSA_shared:
    DVar.CKind = OMPC_shared;
    DVar.ImplicitDSALoc = Iter->DefaultAttrLoc;
    return DVar;
  case DSA_none:
    return DVar;
  case DSA_unspecified:
    DVar.ImplicitDSALoc = Iter->DefaultAttrLoc;
    if (isOpenMPParallelDirective(DVar.DKind) ||
        isOpenMPTeamsDirective(DVar.DKind)) {
      DVar.CKind = OMPC_shared;
      return DVar;
}
    if (isOpenMPTaskingDirective(DVar.DKind)) {
      DSAVarData DVarTemp;
for_loop_talkad7420();
      for (StackTy::reverse_iterator I = std::next(Iter), EE = Stack.rend();
{
}
           I != EE; ++I) {
        DVarTemp = getDSA(I, D);
        if (DVarTemp.CKind != OMPC_shared) {
          DVar.RefExpr = nullptr;
          DVar.CKind = OMPC_firstprivate;
          return DVar;
}
        if (isParallelOrTaskRegion(I->Directive))
          break;
}
      DVar.CKind =
          (DVarTemp.CKind == OMPC_unknown) ? OMPC_firstprivate : OMPC_shared;
      return DVar;
}
}
  return getDSA(++Iter, D);
}
Expr *DSAStackTy::addUniqueAligned(ValueDecl *D, Expr *NewDE) {
  assert(Stack.size() > 1 && "Data sharing attributes stack is empty");
  D = getCanonicalDecl(D);
  auto It = Stack.back().AlignedMap.find(D);
  if (It == Stack.back().AlignedMap.end()) {
    assert(NewDE && "Unexpected nullptr expr to be added into aligned map");
    Stack.back().AlignedMap[D] = NewDE;
    return nullptr;
} else {
    assert(It->second && "Unexpected nullptr expr in the aligned map");
    return It->second;
}
  return nullptr;
}
void DSAStackTy::addLoopControlVariable(ValueDecl *D, VarDecl *Capture) {
  assert(Stack.size() > 1 && "Data-sharing attributes stack is empty");
  D = getCanonicalDecl(D);
  Stack.back().LCVMap.insert(
      std::make_pair(D, LCDeclInfo(Stack.back().LCVMap.size() + 1, Capture)));
}
DSAStackTy::LCDeclInfo DSAStackTy::isLoopControlVariable(ValueDecl *D) {
  assert(Stack.size() > 1 && "Data-sharing attributes stack is empty");
  D = getCanonicalDecl(D);
  return Stack.back().LCVMap.count(D) > 0 ? Stack.back().LCVMap[D]
                                          : LCDeclInfo(0, nullptr);
}
DSAStackTy::LCDeclInfo DSAStackTy::isParentLoopControlVariable(ValueDecl *D) {
  assert(Stack.size() > 2 && "Data-sharing attributes stack is empty");
  D = getCanonicalDecl(D);
  return Stack[Stack.size() - 2].LCVMap.count(D) > 0
             ? Stack[Stack.size() - 2].LCVMap[D]
             : LCDeclInfo(0, nullptr);
}
ValueDecl *DSAStackTy::getParentLoopControlVariable(unsigned I) {
  assert(Stack.size() > 2 && "Data-sharing attributes stack is empty");
  if (Stack[Stack.size() - 2].LCVMap.size() < I)
    return nullptr;
for_loop_talkad7420();
  for (auto &Pair : Stack[Stack.size() - 2].LCVMap) {
    if (Pair.second.first == I)
      return Pair.first;
}
  return nullptr;
}
void DSAStackTy::addDSA(ValueDecl *D, Expr *E, OpenMPClauseKind A,
                        DeclRefExpr *PrivateCopy) {
  D = getCanonicalDecl(D);
  if (A == OMPC_threadprivate) {
    auto &Data = Stack[0].SharingMap[D];
    Data.Attributes = A;
    Data.RefExpr.setPointer(E);
    Data.PrivateCopy = nullptr;
} else {
    assert(Stack.size() > 1 && "Data-sharing attributes stack is empty");
    auto &Data = Stack.back().SharingMap[D];
    assert(Data.Attributes == OMPC_unknown || (A == Data.Attributes) ||
           (A == OMPC_firstprivate && Data.Attributes == OMPC_lastprivate) ||
           (A == OMPC_lastprivate && Data.Attributes == OMPC_firstprivate) ||
           (isLoopControlVariable(D).first && A == OMPC_private));
    if (A == OMPC_lastprivate && Data.Attributes == OMPC_firstprivate) {
      Data.RefExpr.setInt(
true);
      return;
}
    const bool IsLastprivate =
        A == OMPC_lastprivate || Data.Attributes == OMPC_lastprivate;
    Data.Attributes = A;
    Data.RefExpr.setPointerAndInt(E, IsLastprivate);
    Data.PrivateCopy = PrivateCopy;
    if (PrivateCopy) {
      auto &Data = Stack.back().SharingMap[PrivateCopy->getDecl()];
      Data.Attributes = A;
      Data.RefExpr.setPointerAndInt(PrivateCopy, IsLastprivate);
      Data.PrivateCopy = nullptr;
}
}
}
bool DSAStackTy::isOpenMPLocal(VarDecl *D, StackTy::reverse_iterator Iter) {
  D = D->getCanonicalDecl();
  if (Stack.size() > 2) {
    reverse_iterator I = Iter, E = std::prev(Stack.rend());
    Scope *TopScope = nullptr;
    while (I != E && !isParallelOrTaskRegion(I->Directive)) {
      ++I;
}
    if (I == E)
      return false;
    TopScope = I->CurScope ? I->CurScope->getParent() : nullptr;
    Scope *CurScope = getCurScope();
    while (CurScope != TopScope && !CurScope->isDeclScope(D)) {
      CurScope = CurScope->getParent();
}
    return CurScope != TopScope;
}
  return false;
}
static VarDecl *buildVarDecl(Sema &SemaRef, SourceLocation Loc, QualType Type,
                             StringRef Name, const AttrVec *Attrs = nullptr) {
  DeclContext *DC = SemaRef.CurContext;
  IdentifierInfo *II = &SemaRef.PP.getIdentifierTable().get(Name);
  TypeSourceInfo *TInfo = SemaRef.Context.getTrivialTypeSourceInfo(Type, Loc);
  VarDecl *Decl =
      VarDecl::Create(SemaRef.Context, DC, Loc, Loc, II, Type, TInfo, SC_None);
  if (Attrs) {
for_loop_talkad7420();
    for (specific_attr_iterator<AlignedAttr> I(Attrs->begin()), E(Attrs->end());
{
}
         I != E; ++I)
      Decl->addAttr(*I);
}
  Decl->setImplicit();
  return Decl;
}
static DeclRefExpr *buildDeclRefExpr(Sema &S, VarDecl *D, QualType Ty,
                                     SourceLocation Loc,
                                     bool RefersToCapture = false) {
  D->setReferenced();
  D->markUsed(S.Context);
  return DeclRefExpr::Create(S.getASTContext(), NestedNameSpecifierLoc(),
                             SourceLocation(), D, RefersToCapture, Loc, Ty,
                             VK_LValue);
}
DSAStackTy::DSAVarData DSAStackTy::getTopDSA(ValueDecl *D, bool FromParent) {
  D = getCanonicalDecl(D);
  DSAVarData DVar;
  auto *VD = dyn_cast<VarDecl>(D);
  if ((VD && VD->getTLSKind() != VarDecl::TLS_None &&
       !(VD->hasAttr<OMPThreadPrivateDeclAttr>() &&
         SemaRef.getLangOpts().OpenMPUseTLS &&
         SemaRef.getASTContext().getTargetInfo().isTLSSupported())) ||
      (VD && VD->getStorageClass() == SC_Register &&
       VD->hasAttr<AsmLabelAttr>() && !VD->isLocalVarDecl())) {
    addDSA(D, buildDeclRefExpr(SemaRef, VD, D->getType().getNonReferenceType(),
                               D->getLocation()),
           OMPC_threadprivate);
}
  if (Stack[0].SharingMap.count(D)) {
    DVar.RefExpr = Stack[0].SharingMap[D].RefExpr.getPointer();
    DVar.CKind = OMPC_threadprivate;
    return DVar;
}
  if (Stack.size() == 1) {
    return DVar;
}
  auto &&MatchesAlways = [](OpenMPDirectiveKind) -> bool { return true; 
};
  if (VD && VD->isStaticDataMember()) {
    DSAVarData DVarTemp = hasDSA(D, isOpenMPPrivate, MatchesAlways, FromParent);
    if (DVarTemp.CKind != OMPC_unknown && DVarTemp.RefExpr)
      return DVar;
    DVar.CKind = OMPC_shared;
    return DVar;
}
  QualType Type = D->getType().getNonReferenceType().getCanonicalType();
  bool IsConstant = Type.isConstant(SemaRef.getASTContext());
  Type = SemaRef.getASTContext().getBaseElementType(Type);
  CXXRecordDecl *RD =
      SemaRef.getLangOpts().CPlusPlus ? Type->getAsCXXRecordDecl() : nullptr;
  if (auto *CTSD = dyn_cast_or_null<ClassTemplateSpecializationDecl>(RD))
    if (auto *CTD = CTSD->getSpecializedTemplate())
      RD = CTD->getTemplatedDecl();
  if (IsConstant &&
      !(SemaRef.getLangOpts().CPlusPlus && RD && RD->hasDefinition() &&
        RD->hasMutableFields())) {
    DSAVarData DVarTemp = hasDSA(
        D, [](OpenMPClauseKind C) -> bool { return C == OMPC_firstprivate; 
},
        MatchesAlways, FromParent);
    if (DVarTemp.CKind == OMPC_firstprivate && DVarTemp.RefExpr)
      return DVar;
    DVar.CKind = OMPC_shared;
    return DVar;
}
  auto StartI = std::next(Stack.rbegin());
  auto EndI = std::prev(Stack.rend());
  if (FromParent && StartI != EndI) {
    StartI = std::next(StartI);
}
  auto I = std::prev(StartI);
  if (I->SharingMap.count(D)) {
    DVar.RefExpr = I->SharingMap[D].RefExpr.getPointer();
    DVar.PrivateCopy = I->SharingMap[D].PrivateCopy;
    DVar.CKind = I->SharingMap[D].Attributes;
    DVar.ImplicitDSALoc = I->DefaultAttrLoc;
}
  return DVar;
}
DSAStackTy::DSAVarData DSAStackTy::getImplicitDSA(ValueDecl *D,
                                                  bool FromParent) {
  D = getCanonicalDecl(D);
  auto StartI = Stack.rbegin();
  auto EndI = std::prev(Stack.rend());
  if (FromParent && StartI != EndI) {
    StartI = std::next(StartI);
}
  return getDSA(StartI, D);
}
DSAStackTy::DSAVarData
DSAStackTy::hasDSA(ValueDecl *D,
                   const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
                   const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
                   bool FromParent) {
  D = getCanonicalDecl(D);
  auto StartI = std::next(Stack.rbegin());
  auto EndI = Stack.rend();
  if (FromParent && StartI != EndI) {
    StartI = std::next(StartI);
}
for_loop_talkad7420();
  for (auto I = StartI, EE = EndI; I != EE; ++I) {
    if (!DPred(I->Directive) && !isParallelOrTaskRegion(I->Directive))
      continue;
    DSAVarData DVar = getDSA(I, D);
    if (CPred(DVar.CKind))
      return DVar;
}
  return DSAVarData();
}
DSAStackTy::DSAVarData DSAStackTy::hasInnermostDSA(
    ValueDecl *D, const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
    const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
    bool FromParent) {
  D = getCanonicalDecl(D);
  auto StartI = std::next(Stack.rbegin());
  auto EndI = Stack.rend();
  if (FromParent && StartI != EndI)
    StartI = std::next(StartI);
  if (StartI == EndI || !DPred(StartI->Directive))
    return DSAVarData();
  DSAVarData DVar = getDSA(StartI, D);
  return CPred(DVar.CKind) ? DVar : DSAVarData();
}
bool DSAStackTy::hasExplicitDSA(
    ValueDecl *D, const llvm::function_ref<bool(OpenMPClauseKind)> &CPred,
    unsigned Level, bool NotLastprivate) {
  if (CPred(ClauseKindMode))
    return true;
  D = getCanonicalDecl(D);
  auto StartI = std::next(Stack.begin());
  auto EndI = Stack.end();
  if (std::distance(StartI, EndI) <= (int)Level)
    return false;
  std::advance(StartI, Level);
  return (StartI->SharingMap.count(D) > 0) &&
         StartI->SharingMap[D].RefExpr.getPointer() &&
         CPred(StartI->SharingMap[D].Attributes) &&
         (!NotLastprivate || !StartI->SharingMap[D].RefExpr.getInt());
}
bool DSAStackTy::hasExplicitDirective(
    const llvm::function_ref<bool(OpenMPDirectiveKind)> &DPred,
    unsigned Level) {
  auto StartI = std::next(Stack.begin());
  auto EndI = Stack.end();
  if (std::distance(StartI, EndI) <= (int)Level)
    return false;
  std::advance(StartI, Level);
  return DPred(StartI->Directive);
}
bool DSAStackTy::hasDirective(
    const llvm::function_ref<bool(OpenMPDirectiveKind,
                                  const DeclarationNameInfo &, SourceLocation)>
        &DPred,
    bool FromParent) {
  if (Stack.size() < 2)
    return false;
  auto StartI = std::next(Stack.rbegin());
  auto EndI = std::prev(Stack.rend());
  if (FromParent && StartI != EndI) {
    StartI = std::next(StartI);
}
for_loop_talkad7420();
  for (auto I = StartI, EE = EndI; I != EE; ++I) {
    if (DPred(I->Directive, I->DirectiveName, I->ConstructLoc))
      return true;
}
  return false;
}
void Sema::InitDataSharingAttributesStack() {
  VarDataSharingAttributesStack = new DSAStackTy(*this);
}
#define DSAStack static_cast<DSAStackTy *>(VarDataSharingAttributesStack)
bool Sema::IsOpenMPCapturedByRef(ValueDecl *D, unsigned Level) {
  assert(LangOpts.OpenMP && "OpenMP is not allowed");
  auto &Ctx = getASTContext();
  bool IsByRef = true;
  auto Ty = D->getType();
  if (DSAStack->hasExplicitDirective(isOpenMPTargetExecutionDirective, Level)) {
    if (Ty->isReferenceType())
      Ty = Ty->castAs<ReferenceType>()->getPointeeType();
    bool IsVariableUsedInMapClause = false;
    bool IsVariableAssociatedWithSection = false;
    DSAStack->checkMappableExprComponentListsForDecl(
        D, 
true,
        [&](OMPClauseMappableExprCommon::MappableExprComponentListRef
                MapExprComponents,
            OpenMPClauseKind WhereFoundClauseKind) {
          if (WhereFoundClauseKind != OMPC_map)
            return false;
          auto EI = MapExprComponents.rbegin();
          auto EE = MapExprComponents.rend();
          assert(EI != EE && "Invalid map expression!");
          if (isa<DeclRefExpr>(EI->getAssociatedExpression()))
            IsVariableUsedInMapClause |= EI->getAssociatedDeclaration() == D;
          ++EI;
          if (EI == EE)
            return false;
          if (isa<ArraySubscriptExpr>(EI->getAssociatedExpression()) ||
              isa<OMPArraySectionExpr>(EI->getAssociatedExpression()) ||
              isa<MemberExpr>(EI->getAssociatedExpression())) {
            IsVariableAssociatedWithSection = true;
            return true;
}
          return false;
});
    if (IsVariableUsedInMapClause) {
      IsByRef = !(Ty->isPointerType() && IsVariableAssociatedWithSection);
} else {
      IsByRef = !Ty->isScalarType();
}
}
  if (IsByRef && Ty.getNonReferenceType()->isScalarType()) {
    IsByRef = !DSAStack->hasExplicitDSA(
        D, [](OpenMPClauseKind K) -> bool { return K == OMPC_firstprivate; 
},
        Level, 
true);
}
  if (!IsByRef &&
      (Ctx.getTypeSizeInChars(Ty) >
           Ctx.getTypeSizeInChars(Ctx.getUIntPtrType()) ||
       Ctx.getDeclAlign(D) > Ctx.getTypeAlignInChars(Ctx.getUIntPtrType()))) {
    IsByRef = true;
}
  return IsByRef;
}
unsigned Sema::getOpenMPNestingLevel() const {
  assert(getLangOpts().OpenMP);
  return DSAStack->getNestingLevel();
}
VarDecl *Sema::IsOpenMPCapturedDecl(ValueDecl *D) {
  assert(LangOpts.OpenMP && "OpenMP is not allowed");
  D = getCanonicalDecl(D);
  auto *VD = dyn_cast<VarDecl>(D);
  if (VD && !VD->hasLocalStorage()) {
    if (DSAStack->getCurrentDirective() == OMPD_target &&
        !DSAStack->isClauseParsingMode())
      return VD;
    if (DSAStack->hasDirective(
            [](OpenMPDirectiveKind K, const DeclarationNameInfo &,
               SourceLocation) -> bool {
              return isOpenMPTargetExecutionDirective(K);
},
            false))
      return VD;
}
  if (DSAStack->getCurrentDirective() != OMPD_unknown &&
      (!DSAStack->isClauseParsingMode() ||
       DSAStack->getParentDirective() != OMPD_unknown)) {
    auto &&Info = DSAStack->isLoopControlVariable(D);
    if (Info.first ||
        (VD && VD->hasLocalStorage() &&
         isParallelOrTaskRegion(DSAStack->getCurrentDirective())) ||
        (VD && DSAStack->isForceVarCapturing()))
      return VD ? VD : Info.second;
    auto DVarPrivate = DSAStack->getTopDSA(D, DSAStack->isClauseParsingMode());
    if (DVarPrivate.CKind != OMPC_unknown && isOpenMPPrivate(DVarPrivate.CKind))
      return VD ? VD : cast<VarDecl>(DVarPrivate.PrivateCopy->getDecl());
    DVarPrivate = DSAStack->hasDSA(
        D, isOpenMPPrivate, [](OpenMPDirectiveKind) -> bool { return true; 
},
        DSAStack->isClauseParsingMode());
    if (DVarPrivate.CKind != OMPC_unknown)
      return VD ? VD : cast<VarDecl>(DVarPrivate.PrivateCopy->getDecl());
}
  return nullptr;
}
bool Sema::isOpenMPPrivateDecl(ValueDecl *D, unsigned Level) {
  assert(LangOpts.OpenMP && "OpenMP is not allowed");
  return DSAStack->hasExplicitDSA(
      D, [](OpenMPClauseKind K) -> bool { return K == OMPC_private; 
}, Level);
}
bool Sema::isOpenMPTargetCapturedDecl(ValueDecl *D, unsigned Level) {
  assert(LangOpts.OpenMP && "OpenMP is not allowed");
  auto *VD = dyn_cast<VarDecl>(D);
  return VD && !VD->hasLocalStorage() &&
         DSAStack->hasExplicitDirective(isOpenMPTargetExecutionDirective,
                                        Level);
}
void Sema::DestroyDataSharingAttributesStack() { delete DSAStack; 
}
void Sema::StartOpenMPDSABlock(OpenMPDirectiveKind DKind,
                               const DeclarationNameInfo &DirName,
                               Scope *CurScope, SourceLocation Loc) {
  DSAStack->push(DKind, DirName, CurScope, Loc);
  PushExpressionEvaluationContext(PotentiallyEvaluated);
}
void Sema::StartOpenMPClause(OpenMPClauseKind K) {
  DSAStack->setClauseParsingMode(K);
}
void Sema::EndOpenMPClause() {
  DSAStack->setClauseParsingMode(
OMPC_unknown);
}
void Sema::EndOpenMPDSABlock(Stmt *CurDirective) {
  if (auto *D = dyn_cast_or_null<OMPExecutableDirective>(CurDirective)) {
for_loop_talkad7420();
    for (auto *C : D->clauses()) {
      if (auto *Clause = dyn_cast<OMPLastprivateClause>(C)) {
        SmallVector<Expr *, 8> PrivateCopies;
for_loop_talkad7420();
        for (auto *DE : Clause->varlists()) {
          if (DE->isValueDependent() || DE->isTypeDependent()) {
            PrivateCopies.push_back(nullptr);
            continue;
}
          auto *DRE = cast<DeclRefExpr>(DE->IgnoreParens());
          VarDecl *VD = cast<VarDecl>(DRE->getDecl());
          QualType Type = VD->getType().getNonReferenceType();
          auto DVar = DSAStack->getTopDSA(VD, false);
          if (DVar.CKind == OMPC_lastprivate) {
            auto *VDPrivate = buildVarDecl(
                *this, DE->getExprLoc(), Type.getUnqualifiedType(),
                VD->getName(), VD->hasAttrs() ? &VD->getAttrs() : nullptr);
            ActOnUninitializedDecl(VDPrivate);
            if (VDPrivate->isInvalidDecl())
              continue;
            PrivateCopies.push_back(buildDeclRefExpr(
                *this, VDPrivate, DE->getType(), DE->getExprLoc()));
} else {
            PrivateCopies.push_back(nullptr);
}
}
        if (PrivateCopies.size() == Clause->varlist_size())
          Clause->setPrivateCopies(PrivateCopies);
}
}
}
  DSAStack->pop();
  DiscardCleanupsInEvaluationContext();
  PopExpressionEvaluationContext();
}
static bool FinishOpenMPLinearClause(OMPLinearClause &Clause, DeclRefExpr *IV,
                                     Expr *NumIterations, Sema &SemaRef,
                                     Scope *S, DSAStackTy *Stack);
namespace {
class VarDeclFilterCCC : public CorrectionCandidateCallback {
private:
  Sema &SemaRef;
public:
  explicit VarDeclFilterCCC(Sema &S) : SemaRef(S) {
}
  bool ValidateCandidate(const TypoCorrection &Candidate) override {
    NamedDecl *ND = Candidate.getCorrectionDecl();
    if (auto *VD = dyn_cast_or_null<VarDecl>(ND)) {
      return VD->hasGlobalStorage() &&
             SemaRef.isDeclInScope(ND, SemaRef.getCurLexicalContext(),
                                   SemaRef.getCurScope());
}
    return false;
}
};
class VarOrFuncDeclFilterCCC : public CorrectionCandidateCallback {
private:
  Sema &SemaRef;
public:
  explicit VarOrFuncDeclFilterCCC(Sema &S) : SemaRef(S) {
}
  bool ValidateCandidate(const TypoCorrection &Candidate) override {
    NamedDecl *ND = Candidate.getCorrectionDecl();
    if (isa<VarDecl>(ND) || isa<FunctionDecl>(ND)) {
      return SemaRef.isDeclInScope(ND, SemaRef.getCurLexicalContext(),
                                   SemaRef.getCurScope());
}
    return false;
}
};
} 
ExprResult Sema::ActOnOpenMPIdExpression(Scope *CurScope,
                                         CXXScopeSpec &ScopeSpec,
                                         const DeclarationNameInfo &Id) {
  LookupResult Lookup(*this, Id, LookupOrdinaryName);
  LookupParsedName(Lookup, CurScope, &ScopeSpec, true);
  if (Lookup.isAmbiguous())
    return ExprError();
  VarDecl *VD;
  if (!Lookup.isSingleResult()) {
    if (TypoCorrection Corrected = CorrectTypo(
            Id, LookupOrdinaryName, CurScope, nullptr,
            llvm::make_unique<VarDeclFilterCCC>(*this), CTK_ErrorRecovery)) {
      diagnoseTypo(Corrected,
                   PDiag(Lookup.empty()
                             ? diag::err_undeclared_var_use_suggest
                             : diag::err_omp_expected_var_arg_suggest)
                       << Id.getName());
      VD = Corrected.getCorrectionDeclAs<VarDecl>();
} else {
      Diag(Id.getLoc(), Lookup.empty() ? diag::err_undeclared_var_use
                                       : diag::err_omp_expected_var_arg)
          << Id.getName();
      return ExprError();
}
} else {
    if (!(VD = Lookup.getAsSingle<VarDecl>())) {
      Diag(Id.getLoc(), diag::err_omp_expected_var_arg) << Id.getName();
      Diag(Lookup.getFoundDecl()->getLocation(), diag::note_declared_at);
      return ExprError();
}
}
  Lookup.suppressDiagnostics();
  if (!VD->hasGlobalStorage()) {
    Diag(Id.getLoc(), diag::err_omp_global_var_arg)
        << getOpenMPDirectiveName(OMPD_threadprivate) << !VD->isStaticLocal();
    bool IsDecl =
        VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
    Diag(VD->getLocation(),
         IsDecl ? diag::note_previous_decl : diag::note_defined_here)
        << VD;
    return ExprError();
}
  VarDecl *CanonicalVD = VD->getCanonicalDecl();
  NamedDecl *ND = cast<NamedDecl>(CanonicalVD);
  if (CanonicalVD->getDeclContext()->isTranslationUnit() &&
      !getCurLexicalContext()->isTranslationUnit()) {
    Diag(Id.getLoc(), diag::err_omp_var_scope)
        << getOpenMPDirectiveName(OMPD_threadprivate) << VD;
    bool IsDecl =
        VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
    Diag(VD->getLocation(),
         IsDecl ? diag::note_previous_decl : diag::note_defined_here)
        << VD;
    return ExprError();
}
  if (CanonicalVD->isStaticDataMember() &&
      !CanonicalVD->getDeclContext()->Equals(getCurLexicalContext())) {
    Diag(Id.getLoc(), diag::err_omp_var_scope)
        << getOpenMPDirectiveName(OMPD_threadprivate) << VD;
    bool IsDecl =
        VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
    Diag(VD->getLocation(),
         IsDecl ? diag::note_previous_decl : diag::note_defined_here)
        << VD;
    return ExprError();
}
  if (CanonicalVD->getDeclContext()->isNamespace() &&
      (!getCurLexicalContext()->isFileContext() ||
       !getCurLexicalContext()->Encloses(CanonicalVD->getDeclContext()))) {
    Diag(Id.getLoc(), diag::err_omp_var_scope)
        << getOpenMPDirectiveName(OMPD_threadprivate) << VD;
    bool IsDecl =
        VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
    Diag(VD->getLocation(),
         IsDecl ? diag::note_previous_decl : diag::note_defined_here)
        << VD;
    return ExprError();
}
  if (CanonicalVD->isStaticLocal() && CurScope &&
      !isDeclInScope(ND, getCurLexicalContext(), CurScope)) {
    Diag(Id.getLoc(), diag::err_omp_var_scope)
        << getOpenMPDirectiveName(OMPD_threadprivate) << VD;
    bool IsDecl =
        VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
    Diag(VD->getLocation(),
         IsDecl ? diag::note_previous_decl : diag::note_defined_here)
        << VD;
    return ExprError();
}
  if (VD->isUsed() && !DSAStack->isThreadPrivate(VD)) {
    Diag(Id.getLoc(), diag::err_omp_var_used)
        << getOpenMPDirectiveName(OMPD_threadprivate) << VD;
    return ExprError();
}
  QualType ExprType = VD->getType().getNonReferenceType();
  return DeclRefExpr::Create(Context, NestedNameSpecifierLoc(),
                             SourceLocation(), VD,
false,
                             Id.getLoc(), ExprType, VK_LValue);
}
Sema::DeclGroupPtrTy
Sema::ActOnOpenMPThreadprivateDirective(SourceLocation Loc,
                                        ArrayRef<Expr *> VarList) {
  if (OMPThreadPrivateDecl *D = CheckOMPThreadPrivateDecl(Loc, VarList)) {
    CurContext->addDecl(D);
    return DeclGroupPtrTy::make(DeclGroupRef(D));
}
  return nullptr;
}
namespace {
class LocalVarRefChecker : public ConstStmtVisitor<LocalVarRefChecker, bool> {
  Sema &SemaRef;
public:
  bool VisitDeclRefExpr(const DeclRefExpr *E) {
    if (auto *VD = dyn_cast<VarDecl>(E->getDecl())) {
      if (VD->hasLocalStorage()) {
        SemaRef.Diag(E->getLocStart(),
                     diag::err_omp_local_var_in_threadprivate_init)
            << E->getSourceRange();
        SemaRef.Diag(VD->getLocation(), diag::note_defined_here)
            << VD << VD->getSourceRange();
        return true;
}
}
    return false;
}
  bool VisitStmt(const Stmt *S) {
for_loop_talkad7420();
    for (auto Child : S->children()) {
      if (Child && Visit(Child))
        return true;
}
    return false;
}
  explicit LocalVarRefChecker(Sema &SemaRef) : SemaRef(SemaRef) {
}
};
} 
OMPThreadPrivateDecl *
Sema::CheckOMPThreadPrivateDecl(SourceLocation Loc, ArrayRef<Expr *> VarList) {
  SmallVector<Expr *, 8> Vars;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    DeclRefExpr *DE = cast<DeclRefExpr>(RefExpr);
    VarDecl *VD = cast<VarDecl>(DE->getDecl());
    SourceLocation ILoc = DE->getExprLoc();
    VD->setReferenced();
    VD->markUsed(Context);
    QualType QType = VD->getType();
    if (QType->isDependentType() || QType->isInstantiationDependentType()) {
      Vars.push_back(DE);
      continue;
}
    if (RequireCompleteType(ILoc, VD->getType(),
                            diag::err_omp_threadprivate_incomplete_type)) {
      continue;
}
    if (VD->getType()->isReferenceType()) {
      Diag(ILoc, diag::err_omp_ref_type_arg)
          << getOpenMPDirectiveName(OMPD_threadprivate) << VD->getType();
      bool IsDecl =
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(VD->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << VD;
      continue;
}
    if ((VD->getTLSKind() != VarDecl::TLS_None &&
         !(VD->hasAttr<OMPThreadPrivateDeclAttr>() &&
           getLangOpts().OpenMPUseTLS &&
           getASTContext().getTargetInfo().isTLSSupported())) ||
        (VD->getStorageClass() == SC_Register && VD->hasAttr<AsmLabelAttr>() &&
         !VD->isLocalVarDecl())) {
      Diag(ILoc, diag::err_omp_var_thread_local)
          << VD << ((VD->getTLSKind() != VarDecl::TLS_None) ? 0 : 1);
      bool IsDecl =
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(VD->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << VD;
      continue;
}
    if (auto Init = VD->getAnyInitializer()) {
      LocalVarRefChecker Checker(*this);
      if (Checker.Visit(Init))
        continue;
}
    Vars.push_back(RefExpr);
    DSAStack->addDSA(VD, DE, OMPC_threadprivate);
    VD->addAttr(OMPThreadPrivateDeclAttr::CreateImplicit(
        Context, SourceRange(Loc, Loc)));
    if (auto *ML = Context.getASTMutationListener())
      ML->DeclarationMarkedOpenMPThreadPrivate(VD);
}
  OMPThreadPrivateDecl *D = nullptr;
  if (!Vars.empty()) {
    D = OMPThreadPrivateDecl::Create(Context, getCurLexicalContext(), Loc,
                                     Vars);
    D->setAccess(AS_public);
}
  return D;
}
static void ReportOriginalDSA(Sema &SemaRef, DSAStackTy *Stack,
                              const ValueDecl *D, DSAStackTy::DSAVarData DVar,
                              bool IsLoopIterVar = false) {
  if (DVar.RefExpr) {
    SemaRef.Diag(DVar.RefExpr->getExprLoc(), diag::note_omp_explicit_dsa)
        << getOpenMPClauseName(DVar.CKind);
    return;
}
  enum {
    PDSA_StaticMemberShared,
    PDSA_StaticLocalVarShared,
    PDSA_LoopIterVarPrivate,
    PDSA_LoopIterVarLinear,
    PDSA_LoopIterVarLastprivate,
    PDSA_ConstVarShared,
    PDSA_GlobalVarShared,
    PDSA_TaskVarFirstprivate,
    PDSA_LocalVarPrivate,
    PDSA_Implicit
} Reason = PDSA_Implicit;
  bool ReportHint = false;
  auto ReportLoc = D->getLocation();
  auto *VD = dyn_cast<VarDecl>(D);
  if (IsLoopIterVar) {
    if (DVar.CKind == OMPC_private)
      Reason = PDSA_LoopIterVarPrivate;
    else if (DVar.CKind == OMPC_lastprivate)
      Reason = PDSA_LoopIterVarLastprivate;
    else
      Reason = PDSA_LoopIterVarLinear;
} else if (isOpenMPTaskingDirective(DVar.DKind) &&
             DVar.CKind == OMPC_firstprivate) {
    Reason = PDSA_TaskVarFirstprivate;
    ReportLoc = DVar.ImplicitDSALoc;
} else if (VD && VD->isStaticLocal())
    Reason = PDSA_StaticLocalVarShared;
  else if (VD && VD->isStaticDataMember())
    Reason = PDSA_StaticMemberShared;
  else if (VD && VD->isFileVarDecl())
    Reason = PDSA_GlobalVarShared;
  else if (D->getType().isConstant(SemaRef.getASTContext()))
    Reason = PDSA_ConstVarShared;
  else if (VD && VD->isLocalVarDecl() && DVar.CKind == OMPC_private) {
    ReportHint = true;
    Reason = PDSA_LocalVarPrivate;
}
  if (Reason != PDSA_Implicit) {
    SemaRef.Diag(ReportLoc, diag::note_omp_predetermined_dsa)
        << Reason << ReportHint
        << getOpenMPDirectiveName(Stack->getCurrentDirective());
} else if (DVar.ImplicitDSALoc.isValid()) {
    SemaRef.Diag(DVar.ImplicitDSALoc, diag::note_omp_implicit_dsa)
        << getOpenMPClauseName(DVar.CKind);
}
}
namespace {
class DSAAttrChecker : public StmtVisitor<DSAAttrChecker, void> {
  DSAStackTy *Stack;
  Sema &SemaRef;
  bool ErrorFound;
  CapturedStmt *CS;
  llvm::SmallVector<Expr *, 8> ImplicitFirstprivate;
  llvm::DenseMap<ValueDecl *, Expr *> VarsWithInheritedDSA;
public:
  void VisitDeclRefExpr(DeclRefExpr *E) {
    if (E->isTypeDependent() || E->isValueDependent() ||
        E->containsUnexpandedParameterPack() || E->isInstantiationDependent())
      return;
    if (auto *VD = dyn_cast<VarDecl>(E->getDecl())) {
      if (VD->isLocalVarDecl() && !CS->capturesVariable(VD))
        return;
      auto DVar = Stack->getTopDSA(VD, false);
      if (DVar.RefExpr)
        return;
      auto ELoc = E->getExprLoc();
      auto DKind = Stack->getCurrentDirective();
      if (DVar.CKind == OMPC_unknown && Stack->getDefaultDSA() == DSA_none &&
          isParallelOrTaskRegion(DKind) &&
          VarsWithInheritedDSA.count(VD) == 0) {
        VarsWithInheritedDSA[VD] = E;
        return;
}
      DVar = Stack->hasInnermostDSA(
          VD, [](OpenMPClauseKind C) -> bool { return C == OMPC_reduction; 
},
          [](OpenMPDirectiveKind K) -> bool {
            return isOpenMPParallelDirective(K) ||
                   isOpenMPWorksharingDirective(K) || isOpenMPTeamsDirective(K);
},
          false);
      if (isOpenMPTaskingDirective(DKind) && DVar.CKind == OMPC_reduction) {
        ErrorFound = true;
        SemaRef.Diag(ELoc, diag::err_omp_reduction_in_task);
        ReportOriginalDSA(SemaRef, Stack, VD, DVar);
        return;
}
      DVar = Stack->getImplicitDSA(VD, false);
      if (isOpenMPTaskingDirective(DKind) && DVar.CKind != OMPC_shared &&
          !Stack->isLoopControlVariable(VD).first)
        ImplicitFirstprivate.push_back(E);
}
}
  void VisitMemberExpr(MemberExpr *E) {
    if (E->isTypeDependent() || E->isValueDependent() ||
        E->containsUnexpandedParameterPack() || E->isInstantiationDependent())
      return;
    if (isa<CXXThisExpr>(E->getBase()->IgnoreParens())) {
      if (auto *FD = dyn_cast<FieldDecl>(E->getMemberDecl())) {
        auto DVar = Stack->getTopDSA(FD, false);
        if (DVar.RefExpr)
          return;
        auto ELoc = E->getExprLoc();
        auto DKind = Stack->getCurrentDirective();
        DVar = Stack->hasInnermostDSA(
            FD, [](OpenMPClauseKind C) -> bool { return C == OMPC_reduction; 
},
            [](OpenMPDirectiveKind K) -> bool {
              return isOpenMPParallelDirective(K) ||
                     isOpenMPWorksharingDirective(K) ||
                     isOpenMPTeamsDirective(K);
},
            false);
        if (isOpenMPTaskingDirective(DKind) && DVar.CKind == OMPC_reduction) {
          ErrorFound = true;
          SemaRef.Diag(ELoc, diag::err_omp_reduction_in_task);
          ReportOriginalDSA(SemaRef, Stack, FD, DVar);
          return;
}
        DVar = Stack->getImplicitDSA(FD, false);
        if (isOpenMPTaskingDirective(DKind) && DVar.CKind != OMPC_shared &&
            !Stack->isLoopControlVariable(FD).first)
          ImplicitFirstprivate.push_back(E);
}
} else
      Visit(E->getBase());
}
  void VisitOMPExecutableDirective(OMPExecutableDirective *S) {
for_loop_talkad7420();
    for (auto *C : S->clauses()) {
      if (C && (!isa<OMPFirstprivateClause>(C) || C->getLocStart().isValid()))
for_loop_talkad7420();
        for (auto *CC : C->children()) {
          if (CC)
            Visit(CC);
}
}
}
  void VisitStmt(Stmt *S) {
for_loop_talkad7420();
    for (auto *C : S->children()) {
      if (C && !isa<OMPExecutableDirective>(C))
        Visit(C);
}
}
  bool isErrorFound() { return ErrorFound; 
}
  ArrayRef<Expr *> getImplicitFirstprivate() { return ImplicitFirstprivate; 
}
  llvm::DenseMap<ValueDecl *, Expr *> &getVarsWithInheritedDSA() {
    return VarsWithInheritedDSA;
}
  DSAAttrChecker(DSAStackTy *S, Sema &SemaRef, CapturedStmt *CS)
      : Stack(S), SemaRef(SemaRef), ErrorFound(false), CS(CS) {
}
};
} 
void Sema::ActOnOpenMPRegionStart(OpenMPDirectiveKind DKind, Scope *CurScope) {
  switch (DKind) {
  case OMPD_parallel:
  case OMPD_parallel_for:
  case OMPD_parallel_for_simd:
  case OMPD_parallel_sections:
  case OMPD_teams:
  case OMPD_target_teams: {
    QualType KmpInt32Ty = Context.getIntTypeForBitwidth(32, 1);
    QualType KmpInt32PtrTy =
        Context.getPointerType(KmpInt32Ty).withConst().withRestrict();
    Sema::CapturedParamNameType Params[] = {
        std::make_pair(".global_tid.", KmpInt32PtrTy),
        std::make_pair(".bound_tid.", KmpInt32PtrTy),
        std::make_pair(StringRef(), QualType()) 
};
    ActOnCapturedRegionStart(DSAStack->getConstructLoc(), CurScope, CR_OpenMP,
                             Params);
    break;
}
  case OMPD_simd:
  case OMPD_for:
  case OMPD_for_simd:
  case OMPD_sections:
  case OMPD_section:
  case OMPD_single:
  case OMPD_master:
  case OMPD_critical:
  case OMPD_taskgroup:
  case OMPD_distribute:
  case OMPD_ordered:
  case OMPD_atomic:
  case OMPD_target_data:
  case OMPD_target:
  case OMPD_target_parallel:
  case OMPD_target_parallel_for:
  case OMPD_target_parallel_for_simd:
  case OMPD_target_simd: {
    Sema::CapturedParamNameType Params[] = {
        std::make_pair(StringRef(), QualType()) 
};
    ActOnCapturedRegionStart(DSAStack->getConstructLoc(), CurScope, CR_OpenMP,
                             Params);
    break;
}
  case OMPD_task: {
    QualType KmpInt32Ty = Context.getIntTypeForBitwidth(32, 1);
    QualType Args[] = {Context.VoidPtrTy.withConst().withRestrict()
};
    FunctionProtoType::ExtProtoInfo EPI;
    EPI.Variadic = true;
    QualType CopyFnType = Context.getFunctionType(Context.VoidTy, Args, EPI);
    Sema::CapturedParamNameType Params[] = {
        std::make_pair(".global_tid.", KmpInt32Ty),
        std::make_pair(".part_id.", Context.getPointerType(KmpInt32Ty)),
        std::make_pair(".privates.", Context.VoidPtrTy.withConst()),
        std::make_pair(".copy_fn.",
                       Context.getPointerType(CopyFnType).withConst()),
        std::make_pair(".task_t.", Context.VoidPtrTy.withConst()),
        std::make_pair(StringRef(), QualType()) 
};
    ActOnCapturedRegionStart(DSAStack->getConstructLoc(), CurScope, CR_OpenMP,
                             Params);
    getCurCapturedRegion()->TheCapturedDecl->addAttr(
        AlwaysInlineAttr::CreateImplicit(
            Context, AlwaysInlineAttr::Keyword_forceinline, SourceRange()));
    break;
}
  case OMPD_taskloop:
  case OMPD_taskloop_simd: {
    QualType KmpInt32Ty =
        Context.getIntTypeForBitwidth(
32, 
1);
    QualType KmpUInt64Ty =
        Context.getIntTypeForBitwidth(
64, 
0);
    QualType KmpInt64Ty =
        Context.getIntTypeForBitwidth(
64, 
1);
    QualType Args[] = {Context.VoidPtrTy.withConst().withRestrict()
};
    FunctionProtoType::ExtProtoInfo EPI;
    EPI.Variadic = true;
    QualType CopyFnType = Context.getFunctionType(Context.VoidTy, Args, EPI);
    Sema::CapturedParamNameType Params[] = {
        std::make_pair(".global_tid.", KmpInt32Ty),
        std::make_pair(".part_id.", Context.getPointerType(KmpInt32Ty)),
        std::make_pair(".privates.",
                       Context.VoidPtrTy.withConst().withRestrict()),
        std::make_pair(
            ".copy_fn.",
            Context.getPointerType(CopyFnType).withConst().withRestrict()),
        std::make_pair(".task_t.", Context.VoidPtrTy.withConst()),
        std::make_pair(".lb.", KmpUInt64Ty),
        std::make_pair(".ub.", KmpUInt64Ty), std::make_pair(".st.", KmpInt64Ty),
        std::make_pair(".liter.", KmpInt32Ty),
        std::make_pair(StringRef(), QualType()) 
};
    ActOnCapturedRegionStart(DSAStack->getConstructLoc(), CurScope, CR_OpenMP,
                             Params);
    getCurCapturedRegion()->TheCapturedDecl->addAttr(
        AlwaysInlineAttr::CreateImplicit(
            Context, AlwaysInlineAttr::Keyword_forceinline, SourceRange()));
    break;
}
  case OMPD_distribute_parallel_for_simd:
  case OMPD_distribute_simd:
  case OMPD_distribute_parallel_for:
  case OMPD_teams_distribute:
  case OMPD_teams_distribute_simd:
  case OMPD_teams_distribute_parallel_for_simd:
  case OMPD_teams_distribute_parallel_for:
  case OMPD_target_teams_distribute:
  case OMPD_target_teams_distribute_parallel_for:
  case OMPD_target_teams_distribute_parallel_for_simd:
  case OMPD_target_teams_distribute_simd: {
    QualType KmpInt32Ty = Context.getIntTypeForBitwidth(32, 1);
    QualType KmpInt32PtrTy =
        Context.getPointerType(KmpInt32Ty).withConst().withRestrict();
    Sema::CapturedParamNameType Params[] = {
        std::make_pair(".global_tid.", KmpInt32PtrTy),
        std::make_pair(".bound_tid.", KmpInt32PtrTy),
        std::make_pair(".previous.lb.", Context.getSizeType()),
        std::make_pair(".previous.ub.", Context.getSizeType()),
        std::make_pair(StringRef(), QualType()) 
};
    ActOnCapturedRegionStart(DSAStack->getConstructLoc(), CurScope, CR_OpenMP,
                             Params);
    break;
}
  case OMPD_threadprivate:
  case OMPD_taskyield:
  case OMPD_barrier:
  case OMPD_taskwait:
  case OMPD_cancellation_point:
  case OMPD_cancel:
  case OMPD_flush:
  case OMPD_inc:
  case OMPD_target_enter_data:
  case OMPD_target_exit_data:
  case OMPD_declare_reduction:
  case OMPD_declare_simd:
  case OMPD_declare_target:
  case OMPD_end_declare_target:
  case OMPD_target_update:
    llvm_unreachable("OpenMP Directive is not allowed");
  case OMPD_unknown:
    llvm_unreachable("Unknown OpenMP directive");
}
}
static OMPCapturedExprDecl *buildCaptureDecl(Sema &S, IdentifierInfo *Id,
                                             Expr *CaptureExpr, bool WithInit,
                                             bool AsExpression) {
  assert(CaptureExpr);
  ASTContext &C = S.getASTContext();
  Expr *Init = AsExpression ? CaptureExpr : CaptureExpr->IgnoreImpCasts();
  QualType Ty = Init->getType();
  if (CaptureExpr->getObjectKind() == OK_Ordinary && CaptureExpr->isGLValue()) {
    if (S.getLangOpts().CPlusPlus)
      Ty = C.getLValueReferenceType(Ty);
    else {
      Ty = C.getPointerType(Ty);
      ExprResult Res =
          S.CreateBuiltinUnaryOp(CaptureExpr->getExprLoc(), UO_AddrOf, Init);
      if (!Res.isUsable())
        return nullptr;
      Init = Res.get();
}
    WithInit = true;
}
  auto *CED = OMPCapturedExprDecl::Create(C, S.CurContext, Id, Ty,
                                          CaptureExpr->getLocStart());
  if (!WithInit)
    CED->addAttr(OMPCaptureNoInitAttr::CreateImplicit(C, SourceRange()));
  S.CurContext->addHiddenDecl(CED);
  S.AddInitializerToDecl(CED, Init, 
false);
  return CED;
}
static DeclRefExpr *buildCapture(Sema &S, ValueDecl *D, Expr *CaptureExpr,
                                 bool WithInit) {
  OMPCapturedExprDecl *CD;
  if (auto *VD = S.IsOpenMPCapturedDecl(D))
    CD = cast<OMPCapturedExprDecl>(VD);
  else
    CD = buildCaptureDecl(S, D->getIdentifier(), CaptureExpr, WithInit,
false);
  return buildDeclRefExpr(S, CD, CD->getType().getNonReferenceType(),
                          CaptureExpr->getExprLoc());
}
static ExprResult buildCapture(Sema &S, Expr *CaptureExpr, DeclRefExpr *&Ref) {
  if (!Ref) {
    auto *CD =
        buildCaptureDecl(S, &S.getASTContext().Idents.get(".capture_expr."),
                         CaptureExpr, 
true, 
true);
    Ref = buildDeclRefExpr(S, CD, CD->getType().getNonReferenceType(),
                           CaptureExpr->getExprLoc());
}
  ExprResult Res = Ref;
  if (!S.getLangOpts().CPlusPlus &&
      CaptureExpr->getObjectKind() == OK_Ordinary && CaptureExpr->isGLValue() &&
      Ref->getType()->isPointerType())
    Res = S.CreateBuiltinUnaryOp(CaptureExpr->getExprLoc(), UO_Deref, Ref);
  if (!Res.isUsable())
    return ExprError();
  return CaptureExpr->isGLValue() ? Res : S.DefaultLvalueConversion(Res.get());
}
StmtResult Sema::ActOnOpenMPRegionEnd(StmtResult S,
                                      ArrayRef<OMPClause *> Clauses) {
  if (!S.isUsable()) {
    ActOnCapturedRegionError();
    return StmtError();
}
  OMPOrderedClause *OC = nullptr;
  OMPScheduleClause *SC = nullptr;
  SmallVector<OMPLinearClause *, 4> LCs;
for_loop_talkad7420();
  for (auto *Clause : Clauses) {
    if (isOpenMPPrivate(Clause->getClauseKind()) ||
        Clause->getClauseKind() == OMPC_copyprivate ||
        (getLangOpts().OpenMPUseTLS &&
         getASTContext().getTargetInfo().isTLSSupported() &&
         Clause->getClauseKind() == OMPC_copyin)) {
      DSAStack->setForceVarCapturing(Clause->getClauseKind() == OMPC_copyin);
for_loop_talkad7420();
      for (auto *VarRef : Clause->children()) {
        if (auto *E = cast_or_null<Expr>(VarRef)) {
          MarkDeclarationsReferencedInExpr(E);
}
}
      DSAStack->setForceVarCapturing(
false);
} else if (isParallelOrTaskRegion(DSAStack->getCurrentDirective())) {
      if (auto *C = OMPClauseWithPreInit::get(Clause)) {
        if (auto *DS = cast_or_null<DeclStmt>(C->getPreInitStmt())) {
for_loop_talkad7420();
          for (auto *D : DS->decls())
{
            MarkVariableReferenced(D->getLocation(), cast<VarDecl>(D));
}
}
}
      if (auto *C = OMPClauseWithPostUpdate::get(Clause)) {
        if (auto *E = C->getPostUpdateExpr())
          MarkDeclarationsReferencedInExpr(E);
}
}
    if (Clause->getClauseKind() == OMPC_schedule)
      SC = cast<OMPScheduleClause>(Clause);
    else if (Clause->getClauseKind() == OMPC_ordered)
      OC = cast<OMPOrderedClause>(Clause);
    else if (Clause->getClauseKind() == OMPC_linear)
      LCs.push_back(cast<OMPLinearClause>(Clause));
}
  bool ErrorFound = false;
  if (SC &&
      (SC->getFirstScheduleModifier() == OMPC_SCHEDULE_MODIFIER_nonmonotonic ||
       SC->getSecondScheduleModifier() ==
           OMPC_SCHEDULE_MODIFIER_nonmonotonic) &&
      OC) {
    Diag(SC->getFirstScheduleModifier() == OMPC_SCHEDULE_MODIFIER_nonmonotonic
             ? SC->getFirstScheduleModifierLoc()
             : SC->getSecondScheduleModifierLoc(),
         diag::err_omp_schedule_nonmonotonic_ordered)
        << SourceRange(OC->getLocStart(), OC->getLocEnd());
    ErrorFound = true;
}
  if (!LCs.empty() && OC && OC->getNumForLoops()) {
for_loop_talkad7420();
    for (auto *C : LCs) {
      Diag(C->getLocStart(), diag::err_omp_linear_ordered)
          << SourceRange(OC->getLocStart(), OC->getLocEnd());
}
    ErrorFound = true;
}
  if (isOpenMPWorksharingDirective(DSAStack->getCurrentDirective()) &&
      isOpenMPSimdDirective(DSAStack->getCurrentDirective()) && OC &&
      OC->getNumForLoops()) {
    Diag(OC->getLocStart(), diag::err_omp_ordered_simd)
        << getOpenMPDirectiveName(DSAStack->getCurrentDirective());
    ErrorFound = true;
}
  if (ErrorFound) {
    ActOnCapturedRegionError();
    return StmtError();
}
  return ActOnCapturedRegionEnd(S.get());
}
static bool CheckNestingOfRegions(Sema &SemaRef, DSAStackTy *Stack,
                                  OpenMPDirectiveKind CurrentRegion,
                                  const DeclarationNameInfo &CurrentName,
                                  OpenMPDirectiveKind CancelRegion,
                                  SourceLocation StartLoc) {
  if (Stack->getCurScope()) {
    auto ParentRegion = Stack->getParentDirective();
    auto OffendingRegion = ParentRegion;
    bool NestingProhibited = false;
    bool CloseNesting = true;
    bool OrphanSeen = false;
    enum {
      NoRecommend,
      ShouldBeInParallelRegion,
      ShouldBeInOrderedRegion,
      ShouldBeInTargetRegion,
      ShouldBeInTeamsRegion
} Recommend = NoRecommend;
    if (isOpenMPSimdDirective(ParentRegion) && CurrentRegion != OMPD_ordered) {
      SemaRef.Diag(StartLoc, (CurrentRegion != OMPD_simd)
                                 ? diag::err_omp_prohibited_region_simd
                                 : diag::warn_omp_nesting_simd);
      return CurrentRegion != OMPD_simd;
}
    if (ParentRegion == OMPD_atomic) {
      SemaRef.Diag(StartLoc, diag::err_omp_prohibited_region_atomic);
      return true;
}
    if (CurrentRegion == OMPD_section) {
      if (ParentRegion != OMPD_sections &&
          ParentRegion != OMPD_parallel_sections) {
        SemaRef.Diag(StartLoc, diag::err_omp_orphaned_section_directive)
            << (ParentRegion != OMPD_unknown)
            << getOpenMPDirectiveName(ParentRegion);
        return true;
}
      return false;
}
    if (ParentRegion == OMPD_unknown &&
        !isOpenMPNestingTeamsDirective(CurrentRegion))
      return false;
    if (CurrentRegion == OMPD_cancellation_point ||
        CurrentRegion == OMPD_cancel) {
      NestingProhibited =
          !((CancelRegion == OMPD_parallel &&
             (ParentRegion == OMPD_parallel ||
              ParentRegion == OMPD_target_parallel)) ||
            (CancelRegion == OMPD_for &&
             (ParentRegion == OMPD_for || ParentRegion == OMPD_parallel_for ||
              ParentRegion == OMPD_target_parallel_for)) ||
            (CancelRegion == OMPD_taskgroup && ParentRegion == OMPD_task) ||
            (CancelRegion == OMPD_sections &&
             (ParentRegion == OMPD_section || ParentRegion == OMPD_sections ||
              ParentRegion == OMPD_parallel_sections)));
} else if (CurrentRegion == OMPD_master) {
      NestingProhibited = isOpenMPWorksharingDirective(ParentRegion) ||
                          isOpenMPTaskingDirective(ParentRegion);
} else if (CurrentRegion == OMPD_critical && CurrentName.getName()) {
      SourceLocation PreviousCriticalLoc;
      bool DeadLock = Stack->hasDirective(
          [CurrentName, &PreviousCriticalLoc](OpenMPDirectiveKind K,
                                              const DeclarationNameInfo &DNI,
                                              SourceLocation Loc) -> bool {
            if (K == OMPD_critical && DNI.getName() == CurrentName.getName()) {
              PreviousCriticalLoc = Loc;
              return true;
} else
              return false;
},
          false 
);
      if (DeadLock) {
        SemaRef.Diag(StartLoc,
                     diag::err_omp_prohibited_region_critical_same_name)
            << CurrentName.getName();
        if (PreviousCriticalLoc.isValid())
          SemaRef.Diag(PreviousCriticalLoc,
                       diag::note_omp_previous_critical_region);
        return true;
}
} else if (CurrentRegion == OMPD_barrier) {
      NestingProhibited = isOpenMPWorksharingDirective(ParentRegion) ||
                          isOpenMPTaskingDirective(ParentRegion) ||
                          ParentRegion == OMPD_master ||
                          ParentRegion == OMPD_critical ||
                          ParentRegion == OMPD_ordered;
} else if (isOpenMPWorksharingDirective(CurrentRegion) &&
               !isOpenMPParallelDirective(CurrentRegion) &&
               !isOpenMPTeamsDirective(CurrentRegion)) {
      NestingProhibited = isOpenMPWorksharingDirective(ParentRegion) ||
                          isOpenMPTaskingDirective(ParentRegion) ||
                          ParentRegion == OMPD_master ||
                          ParentRegion == OMPD_critical ||
                          ParentRegion == OMPD_ordered;
      Recommend = ShouldBeInParallelRegion;
} else if (CurrentRegion == OMPD_ordered) {
      NestingProhibited = ParentRegion == OMPD_critical ||
                          isOpenMPTaskingDirective(ParentRegion) ||
                          !(isOpenMPSimdDirective(ParentRegion) ||
                            Stack->isParentOrderedRegion());
      Recommend = ShouldBeInOrderedRegion;
} else if (isOpenMPNestingTeamsDirective(CurrentRegion)) {
      NestingProhibited = ParentRegion != OMPD_target;
      OrphanSeen = ParentRegion == OMPD_unknown;
      Recommend = ShouldBeInTargetRegion;
      Stack->setParentTeamsRegionLoc(Stack->getConstructLoc());
}
    if (!NestingProhibited &&
        !isOpenMPTargetExecutionDirective(CurrentRegion) &&
        !isOpenMPTargetDataManagementDirective(CurrentRegion) &&
        (ParentRegion == OMPD_teams || ParentRegion == OMPD_target_teams)) {
      NestingProhibited = !isOpenMPParallelDirective(CurrentRegion) &&
                          !isOpenMPDistributeDirective(CurrentRegion);
      Recommend = ShouldBeInParallelRegion;
}
    if (!NestingProhibited &&
        isOpenMPNestingDistributeDirective(CurrentRegion)) {
      NestingProhibited =
          (ParentRegion != OMPD_teams && ParentRegion != OMPD_target_teams);
      Recommend = ShouldBeInTeamsRegion;
}
    if (!NestingProhibited &&
        (isOpenMPTargetExecutionDirective(CurrentRegion) ||
         isOpenMPTargetDataManagementDirective(CurrentRegion))) {
      NestingProhibited = Stack->hasDirective(
          [&OffendingRegion](OpenMPDirectiveKind K, const DeclarationNameInfo &,
                             SourceLocation) -> bool {
            if (isOpenMPTargetExecutionDirective(K)) {
              OffendingRegion = K;
              return true;
} else
              return false;
},
          false 
);
      CloseNesting = false;
}
    if (NestingProhibited) {
      if (OrphanSeen) {
        SemaRef.Diag(StartLoc, diag::err_omp_orphaned_device_directive)
            << getOpenMPDirectiveName(CurrentRegion) << Recommend;
} else {
        SemaRef.Diag(StartLoc, diag::err_omp_prohibited_region)
            << CloseNesting << getOpenMPDirectiveName(OffendingRegion)
            << Recommend << getOpenMPDirectiveName(CurrentRegion);
}
      return true;
}
}
  return false;
}
static bool checkIfClauses(Sema &S, OpenMPDirectiveKind Kind,
                           ArrayRef<OMPClause *> Clauses,
                           ArrayRef<OpenMPDirectiveKind> AllowedNameModifiers) {
  bool ErrorFound = false;
  unsigned NamedModifiersNumber = 0;
  SmallVector<const OMPIfClause *, OMPC_unknown + 1> FoundNameModifiers(
      OMPD_unknown + 1);
  SmallVector<SourceLocation, 4> NameModifierLoc;
for_loop_talkad7420();
  for (const auto *C : Clauses) {
    if (const auto *IC = dyn_cast_or_null<OMPIfClause>(C)) {
      OpenMPDirectiveKind CurNM = IC->getNameModifier();
      if (FoundNameModifiers[CurNM]) {
        S.Diag(C->getLocStart(), diag::err_omp_more_one_clause)
            << getOpenMPDirectiveName(Kind) << getOpenMPClauseName(OMPC_if)
            << (CurNM != OMPD_unknown) << getOpenMPDirectiveName(CurNM);
        ErrorFound = true;
} else if (CurNM != OMPD_unknown) {
        NameModifierLoc.push_back(IC->getNameModifierLoc());
        ++NamedModifiersNumber;
}
      FoundNameModifiers[CurNM] = IC;
      if (CurNM == OMPD_unknown)
        continue;
      bool MatchFound = false;
for_loop_talkad7420();
      for (auto NM : AllowedNameModifiers) {
        if (CurNM == NM) {
          MatchFound = true;
          break;
}
}
      if (!MatchFound) {
        S.Diag(IC->getNameModifierLoc(),
               diag::err_omp_wrong_if_directive_name_modifier)
            << getOpenMPDirectiveName(CurNM) << getOpenMPDirectiveName(Kind);
        ErrorFound = true;
}
}
}
  if (FoundNameModifiers[OMPD_unknown] && NamedModifiersNumber > 0) {
    if (NamedModifiersNumber == AllowedNameModifiers.size()) {
      S.Diag(FoundNameModifiers[OMPD_unknown]->getLocStart(),
             diag::err_omp_no_more_if_clause);
} else {
      std::string Values;
      std::string Sep(", ");
      unsigned AllowedCnt = 0;
      unsigned TotalAllowedNum =
          AllowedNameModifiers.size() - NamedModifiersNumber;
for_loop_talkad7420();
      for (unsigned Cnt = 0, End = AllowedNameModifiers.size(); Cnt < End;
{
}
           ++Cnt) {
        OpenMPDirectiveKind NM = AllowedNameModifiers[Cnt];
        if (!FoundNameModifiers[NM]) {
          Values += "'";
          Values += getOpenMPDirectiveName(NM);
          Values += "'";
          if (AllowedCnt + 2 == TotalAllowedNum)
            Values += " or ";
          else if (AllowedCnt + 1 != TotalAllowedNum)
            Values += Sep;
          ++AllowedCnt;
}
}
      S.Diag(FoundNameModifiers[OMPD_unknown]->getCondition()->getLocStart(),
             diag::err_omp_unnamed_if_clause)
          << (TotalAllowedNum > 1) << Values;
}
for_loop_talkad7420();
    for (auto Loc : NameModifierLoc) {
      S.Diag(Loc, diag::note_omp_previous_named_if_clause);
}
    ErrorFound = true;
}
  return ErrorFound;
}
StmtResult Sema::ActOnOpenMPExecutableDirective(
    OpenMPDirectiveKind Kind, const DeclarationNameInfo &DirName,
    OpenMPDirectiveKind CancelRegion, ArrayRef<OMPClause *> Clauses,
    Stmt *AStmt, SourceLocation StartLoc, SourceLocation EndLoc) {
  StmtResult Res = StmtError();
  if (CheckNestingOfRegions(*this, DSAStack, Kind, DirName, CancelRegion,
                            StartLoc))
    return StmtError();
  llvm::SmallVector<OMPClause *, 8> ClausesWithImplicit;
  llvm::DenseMap<ValueDecl *, Expr *> VarsWithInheritedDSA;
  bool ErrorFound = false;
  ClausesWithImplicit.append(Clauses.begin(), Clauses.end());
  if (AStmt) {
    assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
    DSAAttrChecker DSAChecker(DSAStack, *this, cast<CapturedStmt>(AStmt));
    DSAChecker.Visit(cast<CapturedStmt>(AStmt)->getCapturedStmt());
    if (DSAChecker.isErrorFound())
      return StmtError();
    VarsWithInheritedDSA = DSAChecker.getVarsWithInheritedDSA();
    if (!DSAChecker.getImplicitFirstprivate().empty()) {
      if (OMPClause *Implicit = ActOnOpenMPFirstprivateClause(
              DSAChecker.getImplicitFirstprivate(), SourceLocation(),
              SourceLocation(), SourceLocation())) {
        ClausesWithImplicit.push_back(Implicit);
        ErrorFound = cast<OMPFirstprivateClause>(Implicit)->varlist_size() !=
                     DSAChecker.getImplicitFirstprivate().size();
} else
        ErrorFound = true;
}
}
  llvm::SmallVector<OpenMPDirectiveKind, 4> AllowedNameModifiers;
  switch (Kind) {
  case OMPD_parallel:
    Res = ActOnOpenMPParallelDirective(ClausesWithImplicit, AStmt, StartLoc,
                                       EndLoc);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_simd:
    Res = ActOnOpenMPSimdDirective(ClausesWithImplicit, AStmt, StartLoc, EndLoc,
                                   VarsWithInheritedDSA);
    break;
  case OMPD_for:
    Res = ActOnOpenMPForDirective(ClausesWithImplicit, AStmt, StartLoc, EndLoc,
                                  VarsWithInheritedDSA);
    break;
  case OMPD_for_simd:
    Res = ActOnOpenMPForSimdDirective(ClausesWithImplicit, AStmt, StartLoc,
                                      EndLoc, VarsWithInheritedDSA);
    break;
  case OMPD_sections:
    Res = ActOnOpenMPSectionsDirective(ClausesWithImplicit, AStmt, StartLoc,
                                       EndLoc);
    break;
  case OMPD_section:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp section' directive");
    Res = ActOnOpenMPSectionDirective(AStmt, StartLoc, EndLoc);
    break;
  case OMPD_single:
    Res = ActOnOpenMPSingleDirective(ClausesWithImplicit, AStmt, StartLoc,
                                     EndLoc);
    break;
  case OMPD_master:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp master' directive");
    Res = ActOnOpenMPMasterDirective(AStmt, StartLoc, EndLoc);
    break;
  case OMPD_critical:
    Res = ActOnOpenMPCriticalDirective(DirName, ClausesWithImplicit, AStmt,
                                       StartLoc, EndLoc);
    break;
  case OMPD_parallel_for:
    Res = ActOnOpenMPParallelForDirective(ClausesWithImplicit, AStmt, StartLoc,
                                          EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_parallel_for_simd:
    Res = ActOnOpenMPParallelForSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_parallel_sections:
    Res = ActOnOpenMPParallelSectionsDirective(ClausesWithImplicit, AStmt,
                                               StartLoc, EndLoc);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_task:
    Res =
        ActOnOpenMPTaskDirective(ClausesWithImplicit, AStmt, StartLoc, EndLoc);
    AllowedNameModifiers.push_back(OMPD_task);
    break;
  case OMPD_taskyield:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp taskyield' directive");
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp taskyield' directive");
    Res = ActOnOpenMPTaskyieldDirective(StartLoc, EndLoc);
    break;
  case OMPD_barrier:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp barrier' directive");
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp barrier' directive");
    Res = ActOnOpenMPBarrierDirective(StartLoc, EndLoc);
    break;
  case OMPD_taskwait:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp taskwait' directive");
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp taskwait' directive");
    Res = ActOnOpenMPTaskwaitDirective(StartLoc, EndLoc);
    break;
  case OMPD_taskgroup:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp taskgroup' directive");
    Res = ActOnOpenMPTaskgroupDirective(AStmt, StartLoc, EndLoc);
    break;
  case OMPD_flush:
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp flush' directive");
    Res = ActOnOpenMPFlushDirective(ClausesWithImplicit, StartLoc, EndLoc);
    break;
  case OMPD_inc:
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp inc' directive");
    Res = ActOnOpenMPIncDirective(ClausesWithImplicit, StartLoc, EndLoc);
    break;
  case OMPD_ordered:
    Res = ActOnOpenMPOrderedDirective(ClausesWithImplicit, AStmt, StartLoc,
                                      EndLoc);
    break;
  case OMPD_atomic:
    Res = ActOnOpenMPAtomicDirective(ClausesWithImplicit, AStmt, StartLoc,
                                     EndLoc);
    break;
  case OMPD_teams:
    Res =
        ActOnOpenMPTeamsDirective(ClausesWithImplicit, AStmt, StartLoc, EndLoc);
    break;
  case OMPD_target:
    Res = ActOnOpenMPTargetDirective(ClausesWithImplicit, AStmt, StartLoc,
                                     EndLoc);
    AllowedNameModifiers.push_back(OMPD_target);
    break;
  case OMPD_target_parallel:
    Res = ActOnOpenMPTargetParallelDirective(ClausesWithImplicit, AStmt,
                                             StartLoc, EndLoc);
    AllowedNameModifiers.push_back(OMPD_target);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_target_parallel_for:
    Res = ActOnOpenMPTargetParallelForDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_cancellation_point:
    assert(ClausesWithImplicit.empty() &&
           "No clauses are allowed for 'omp cancellation point' directive");
    assert(AStmt == nullptr && "No associated statement allowed for 'omp "
                               "cancellation point' directive");
    Res = ActOnOpenMPCancellationPointDirective(StartLoc, EndLoc, CancelRegion);
    break;
  case OMPD_cancel:
    assert(AStmt == nullptr &&
           "No associated statement allowed for 'omp cancel' directive");
    Res = ActOnOpenMPCancelDirective(ClausesWithImplicit, StartLoc, EndLoc,
                                     CancelRegion);
    AllowedNameModifiers.push_back(OMPD_cancel);
    break;
  case OMPD_target_data:
    Res = ActOnOpenMPTargetDataDirective(ClausesWithImplicit, AStmt, StartLoc,
                                         EndLoc);
    AllowedNameModifiers.push_back(OMPD_target_data);
    break;
  case OMPD_target_enter_data:
    Res = ActOnOpenMPTargetEnterDataDirective(ClausesWithImplicit, StartLoc,
                                              EndLoc);
    AllowedNameModifiers.push_back(OMPD_target_enter_data);
    break;
  case OMPD_target_exit_data:
    Res = ActOnOpenMPTargetExitDataDirective(ClausesWithImplicit, StartLoc,
                                             EndLoc);
    AllowedNameModifiers.push_back(OMPD_target_exit_data);
    break;
  case OMPD_taskloop:
    Res = ActOnOpenMPTaskLoopDirective(ClausesWithImplicit, AStmt, StartLoc,
                                       EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_taskloop);
    break;
  case OMPD_taskloop_simd:
    Res = ActOnOpenMPTaskLoopSimdDirective(ClausesWithImplicit, AStmt, StartLoc,
                                           EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_taskloop);
    break;
  case OMPD_distribute:
    Res = ActOnOpenMPDistributeDirective(ClausesWithImplicit, AStmt, StartLoc,
                                         EndLoc, VarsWithInheritedDSA);
    break;
  case OMPD_target_update:
    assert(!AStmt && "Statement is not allowed for target update");
    Res =
        ActOnOpenMPTargetUpdateDirective(ClausesWithImplicit, StartLoc, EndLoc);
    AllowedNameModifiers.push_back(OMPD_target_update);
    break;
  case OMPD_distribute_parallel_for:
    Res = ActOnOpenMPDistributeParallelForDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_distribute_parallel_for_simd:
    Res = ActOnOpenMPDistributeParallelForSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_distribute_simd:
    Res = ActOnOpenMPDistributeSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    break;
  case OMPD_target_parallel_for_simd:
    Res = ActOnOpenMPTargetParallelForSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_target_simd:
    Res = ActOnOpenMPTargetSimdDirective(ClausesWithImplicit, AStmt, StartLoc,
                                         EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    break;
  case OMPD_teams_distribute:
    Res = ActOnOpenMPTeamsDistributeDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    break;
  case OMPD_teams_distribute_simd:
    Res = ActOnOpenMPTeamsDistributeSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    break;
  case OMPD_teams_distribute_parallel_for_simd:
    Res = ActOnOpenMPTeamsDistributeParallelForSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_teams_distribute_parallel_for:
    Res = ActOnOpenMPTeamsDistributeParallelForDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_target_teams:
    Res = ActOnOpenMPTargetTeamsDirective(ClausesWithImplicit, AStmt, StartLoc,
                                          EndLoc);
    AllowedNameModifiers.push_back(OMPD_target);
    break;
  case OMPD_target_teams_distribute:
    Res = ActOnOpenMPTargetTeamsDistributeDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    break;
  case OMPD_target_teams_distribute_parallel_for:
    Res = ActOnOpenMPTargetTeamsDistributeParallelForDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_target_teams_distribute_parallel_for_simd:
    Res = ActOnOpenMPTargetTeamsDistributeParallelForSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    AllowedNameModifiers.push_back(OMPD_parallel);
    break;
  case OMPD_target_teams_distribute_simd:
    Res = ActOnOpenMPTargetTeamsDistributeSimdDirective(
        ClausesWithImplicit, AStmt, StartLoc, EndLoc, VarsWithInheritedDSA);
    AllowedNameModifiers.push_back(OMPD_target);
    break;
  case OMPD_declare_target:
  case OMPD_end_declare_target:
  case OMPD_threadprivate:
  case OMPD_declare_reduction:
  case OMPD_declare_simd:
    llvm_unreachable("OpenMP Directive is not allowed");
  case OMPD_unknown:
    llvm_unreachable("Unknown OpenMP directive");
}
for_loop_talkad7420();
  for (auto P : VarsWithInheritedDSA) {
    Diag(P.second->getExprLoc(), diag::err_omp_no_dsa_for_variable)
        << P.first << P.second->getSourceRange();
}
  ErrorFound = !VarsWithInheritedDSA.empty() || ErrorFound;
  if (!AllowedNameModifiers.empty())
    ErrorFound = checkIfClauses(*this, Kind, Clauses, AllowedNameModifiers) ||
                 ErrorFound;
  if (ErrorFound)
    return StmtError();
  return Res;
}
Sema::DeclGroupPtrTy Sema::ActOnOpenMPDeclareSimdDirective(
    DeclGroupPtrTy DG, OMPDeclareSimdDeclAttr::BranchStateTy BS, Expr *Simdlen,
    ArrayRef<Expr *> Uniforms, ArrayRef<Expr *> Aligneds,
    ArrayRef<Expr *> Alignments, ArrayRef<Expr *> Linears,
    ArrayRef<unsigned> LinModifiers, ArrayRef<Expr *> Steps, SourceRange SR) {
  assert(Aligneds.size() == Alignments.size());
  assert(Linears.size() == LinModifiers.size());
  assert(Linears.size() == Steps.size());
  if (!DG || DG.get().isNull())
    return DeclGroupPtrTy();
  if (!DG.get().isSingleDecl()) {
    Diag(SR.getBegin(), diag::err_omp_single_decl_in_declare_simd);
    return DG;
}
  auto *ADecl = DG.get().getSingleDecl();
  if (auto *FTD = dyn_cast<FunctionTemplateDecl>(ADecl))
    ADecl = FTD->getTemplatedDecl();
  auto *FD = dyn_cast<FunctionDecl>(ADecl);
  if (!FD) {
    Diag(ADecl->getLocation(), diag::err_omp_function_expected);
    return DeclGroupPtrTy();
}
  ExprResult SL;
  if (Simdlen)
    SL = VerifyPositiveIntegerConstantInClause(Simdlen, OMPC_simdlen);
  llvm::DenseMap<Decl *, Expr *> UniformedArgs;
  Expr *UniformedLinearThis = nullptr;
for_loop_talkad7420();
  for (auto *E : Uniforms) {
    E = E->IgnoreParenImpCasts();
    if (auto *DRE = dyn_cast<DeclRefExpr>(E))
      if (auto *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl()))
        if (FD->getNumParams() > PVD->getFunctionScopeIndex() &&
            FD->getParamDecl(PVD->getFunctionScopeIndex())
                    ->getCanonicalDecl() == PVD->getCanonicalDecl()) {
          UniformedArgs.insert(std::make_pair(PVD->getCanonicalDecl(), E));
          continue;
}
    if (isa<CXXThisExpr>(E)) {
      UniformedLinearThis = E;
      continue;
}
    Diag(E->getExprLoc(), diag::err_omp_param_or_this_in_clause)
        << FD->getDeclName() << (isa<CXXMethodDecl>(ADecl) ? 1 : 0);
}
  llvm::DenseMap<Decl *, Expr *> AlignedArgs;
  Expr *AlignedThis = nullptr;
for_loop_talkad7420();
  for (auto *E : Aligneds) {
    E = E->IgnoreParenImpCasts();
    if (auto *DRE = dyn_cast<DeclRefExpr>(E))
      if (auto *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
        auto *CanonPVD = PVD->getCanonicalDecl();
        if (FD->getNumParams() > PVD->getFunctionScopeIndex() &&
            FD->getParamDecl(PVD->getFunctionScopeIndex())
                    ->getCanonicalDecl() == CanonPVD) {
          if (AlignedArgs.count(CanonPVD) > 0) {
            Diag(E->getExprLoc(), diag::err_omp_aligned_twice)
                << 1 << E->getSourceRange();
            Diag(AlignedArgs[CanonPVD]->getExprLoc(),
                 diag::note_omp_explicit_dsa)
                << getOpenMPClauseName(OMPC_aligned);
            continue;
}
          AlignedArgs[CanonPVD] = E;
          QualType QTy = PVD->getType()
                             .getNonReferenceType()
                             .getUnqualifiedType()
                             .getCanonicalType();
          const Type *Ty = QTy.getTypePtrOrNull();
          if (!Ty || (!Ty->isArrayType() && !Ty->isPointerType())) {
            Diag(E->getExprLoc(), diag::err_omp_aligned_expected_array_or_ptr)
                << QTy << getLangOpts().CPlusPlus << E->getSourceRange();
            Diag(PVD->getLocation(), diag::note_previous_decl) << PVD;
}
          continue;
}
}
    if (isa<CXXThisExpr>(E)) {
      if (AlignedThis) {
        Diag(E->getExprLoc(), diag::err_omp_aligned_twice)
            << 2 << E->getSourceRange();
        Diag(AlignedThis->getExprLoc(), diag::note_omp_explicit_dsa)
            << getOpenMPClauseName(OMPC_aligned);
}
      AlignedThis = E;
      continue;
}
    Diag(E->getExprLoc(), diag::err_omp_param_or_this_in_clause)
        << FD->getDeclName() << (isa<CXXMethodDecl>(ADecl) ? 1 : 0);
}
  SmallVector<Expr *, 4> NewAligns;
for_loop_talkad7420();
  for (auto *E : Alignments) {
    ExprResult Align;
    if (E)
      Align = VerifyPositiveIntegerConstantInClause(E, OMPC_aligned);
    NewAligns.push_back(Align.get());
}
  llvm::DenseMap<Decl *, Expr *> LinearArgs;
  const bool IsUniformedThis = UniformedLinearThis != nullptr;
  auto MI = LinModifiers.begin();
for_loop_talkad7420();
  for (auto *E : Linears) {
    auto LinKind = static_cast<OpenMPLinearClauseKind>(*MI);
    ++MI;
    E = E->IgnoreParenImpCasts();
    if (auto *DRE = dyn_cast<DeclRefExpr>(E))
      if (auto *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
        auto *CanonPVD = PVD->getCanonicalDecl();
        if (FD->getNumParams() > PVD->getFunctionScopeIndex() &&
            FD->getParamDecl(PVD->getFunctionScopeIndex())
                    ->getCanonicalDecl() == CanonPVD) {
          if (LinearArgs.count(CanonPVD) > 0) {
            Diag(E->getExprLoc(), diag::err_omp_wrong_dsa)
                << getOpenMPClauseName(OMPC_linear)
                << getOpenMPClauseName(OMPC_linear) << E->getSourceRange();
            Diag(LinearArgs[CanonPVD]->getExprLoc(),
                 diag::note_omp_explicit_dsa)
                << getOpenMPClauseName(OMPC_linear);
            continue;
}
          if (UniformedArgs.count(CanonPVD) > 0) {
            Diag(E->getExprLoc(), diag::err_omp_wrong_dsa)
                << getOpenMPClauseName(OMPC_linear)
                << getOpenMPClauseName(OMPC_uniform) << E->getSourceRange();
            Diag(UniformedArgs[CanonPVD]->getExprLoc(),
                 diag::note_omp_explicit_dsa)
                << getOpenMPClauseName(OMPC_uniform);
            continue;
}
          LinearArgs[CanonPVD] = E;
          if (E->isValueDependent() || E->isTypeDependent() ||
              E->isInstantiationDependent() ||
              E->containsUnexpandedParameterPack())
            continue;
          (void)CheckOpenMPLinearDecl(CanonPVD, E->getExprLoc(), LinKind,
                                      PVD->getOriginalType());
          continue;
}
}
    if (isa<CXXThisExpr>(E)) {
      if (UniformedLinearThis) {
        Diag(E->getExprLoc(), diag::err_omp_wrong_dsa)
            << getOpenMPClauseName(OMPC_linear)
            << getOpenMPClauseName(IsUniformedThis ? OMPC_uniform : OMPC_linear)
            << E->getSourceRange();
        Diag(UniformedLinearThis->getExprLoc(), diag::note_omp_explicit_dsa)
            << getOpenMPClauseName(IsUniformedThis ? OMPC_uniform
                                                   : OMPC_linear);
        continue;
}
      UniformedLinearThis = E;
      if (E->isValueDependent() || E->isTypeDependent() ||
          E->isInstantiationDependent() || E->containsUnexpandedParameterPack())
        continue;
      (void)CheckOpenMPLinearDecl(
nullptr, E->getExprLoc(), LinKind,
                                  E->getType());
      continue;
}
    Diag(E->getExprLoc(), diag::err_omp_param_or_this_in_clause)
        << FD->getDeclName() << (isa<CXXMethodDecl>(ADecl) ? 1 : 0);
}
  Expr *Step = nullptr;
  Expr *NewStep = nullptr;
  SmallVector<Expr *, 4> NewSteps;
for_loop_talkad7420();
  for (auto *E : Steps) {
    if (Step == E || !E) {
      NewSteps.push_back(E ? NewStep : nullptr);
      continue;
}
    Step = E;
    if (auto *DRE = dyn_cast<DeclRefExpr>(Step))
      if (auto *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
        auto *CanonPVD = PVD->getCanonicalDecl();
        if (UniformedArgs.count(CanonPVD) == 0) {
          Diag(Step->getExprLoc(), diag::err_omp_expected_uniform_param)
              << Step->getSourceRange();
} else if (E->isValueDependent() || E->isTypeDependent() ||
                   E->isInstantiationDependent() ||
                   E->containsUnexpandedParameterPack() ||
                   CanonPVD->getType()->hasIntegerRepresentation())
          NewSteps.push_back(Step);
        else {
          Diag(Step->getExprLoc(), diag::err_omp_expected_int_param)
              << Step->getSourceRange();
}
        continue;
}
    NewStep = Step;
    if (Step && !Step->isValueDependent() && !Step->isTypeDependent() &&
        !Step->isInstantiationDependent() &&
        !Step->containsUnexpandedParameterPack()) {
      NewStep = PerformOpenMPImplicitIntegerConversion(Step->getExprLoc(), Step)
                    .get();
      if (NewStep)
        NewStep = VerifyIntegerConstantExpression(NewStep).get();
}
    NewSteps.push_back(NewStep);
}
  auto *NewAttr = OMPDeclareSimdDeclAttr::CreateImplicit(
      Context, BS, SL.get(), const_cast<Expr **>(Uniforms.data()),
      Uniforms.size(), const_cast<Expr **>(Aligneds.data()), Aligneds.size(),
      const_cast<Expr **>(NewAligns.data()), NewAligns.size(),
      const_cast<Expr **>(Linears.data()), Linears.size(),
      const_cast<unsigned *>(LinModifiers.data()), LinModifiers.size(),
      NewSteps.data(), NewSteps.size(), SR);
  ADecl->addAttr(NewAttr);
  return ConvertDeclToDeclGroup(ADecl);
}
StmtResult Sema::ActOnOpenMPParallelDirective(ArrayRef<OMPClause *> Clauses,
                                              Stmt *AStmt,
                                              SourceLocation StartLoc,
                                              SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPParallelDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt,
                                      DSAStack->isCancelRegion());
}
namespace {
class OpenMPIterationSpaceChecker {
  Sema &SemaRef;
  SourceLocation DefaultLoc;
  SourceLocation ConditionLoc;
  SourceRange InitSrcRange;
  SourceRange ConditionSrcRange;
  SourceRange IncrementSrcRange;
  ValueDecl *LCDecl = nullptr;
  Expr *LCRef = nullptr;
  Expr *LB = nullptr;
  Expr *UB = nullptr;
  Expr *Step = nullptr;
  bool TestIsLessOp = false;
  bool TestIsStrictOp = false;
  bool SubtractStep = false;
public:
  OpenMPIterationSpaceChecker(Sema &SemaRef, SourceLocation DefaultLoc)
      : SemaRef(SemaRef), DefaultLoc(DefaultLoc), ConditionLoc(DefaultLoc) {
}
  bool CheckInit(Stmt *S, bool EmitDiags = true);
  bool CheckCond(Expr *S);
  bool CheckInc(Expr *S);
  ValueDecl *GetLoopDecl() const { return LCDecl; 
}
  Expr *GetLoopDeclRefExpr() const { return LCRef; 
}
  SourceRange GetInitSrcRange() const { return InitSrcRange; 
}
  SourceRange GetConditionSrcRange() const { return ConditionSrcRange; 
}
  SourceRange GetIncrementSrcRange() const { return IncrementSrcRange; 
}
  bool ShouldSubtractStep() const { return SubtractStep; 
}
  Expr *
  BuildNumIterations(Scope *S, const bool LimitedType,
                     llvm::MapVector<Expr *, DeclRefExpr *> &Captures) const;
  Expr *BuildPreCond(Scope *S, Expr *Cond,
                     llvm::MapVector<Expr *, DeclRefExpr *> &Captures) const;
  DeclRefExpr *BuildCounterVar(llvm::MapVector<Expr *, DeclRefExpr *> &Captures,
                               DSAStackTy &DSA) const;
  Expr *BuildPrivateCounterVar() const;
  Expr *BuildCounterInit() const;
  Expr *BuildCounterStep() const;
  bool Dependent() const;
private:
  bool CheckIncRHS(Expr *RHS);
  bool SetLCDeclAndLB(ValueDecl *NewLCDecl, Expr *NewDeclRefExpr, Expr *NewLB);
  bool SetUB(Expr *NewUB, bool LessOp, bool StrictOp, SourceRange SR,
             SourceLocation SL);
  bool SetStep(Expr *NewStep, bool Subtract);
};
bool OpenMPIterationSpaceChecker::Dependent() const {
  if (!LCDecl) {
    assert(!LB && !UB && !Step);
    return false;
}
  return LCDecl->getType()->isDependentType() ||
         (LB && LB->isValueDependent()) || (UB && UB->isValueDependent()) ||
         (Step && Step->isValueDependent());
}
static Expr *getExprAsWritten(Expr *E) {
  if (auto *ExprTemp = dyn_cast<ExprWithCleanups>(E))
    E = ExprTemp->getSubExpr();
  if (auto *MTE = dyn_cast<MaterializeTemporaryExpr>(E))
    E = MTE->GetTemporaryExpr();
  while (auto *Binder = dyn_cast<CXXBindTemporaryExpr>(E))
    E = Binder->getSubExpr();
  if (auto *ICE = dyn_cast<ImplicitCastExpr>(E))
    E = ICE->getSubExprAsWritten();
  return E->IgnoreParens();
}
bool OpenMPIterationSpaceChecker::SetLCDeclAndLB(ValueDecl *NewLCDecl,
                                                 Expr *NewLCRefExpr,
                                                 Expr *NewLB) {
  assert(LCDecl == nullptr && LB == nullptr && LCRef == nullptr &&
         UB == nullptr && Step == nullptr && !TestIsLessOp && !TestIsStrictOp);
  if (!NewLCDecl || !NewLB)
    return true;
  LCDecl = getCanonicalDecl(NewLCDecl);
  LCRef = NewLCRefExpr;
  if (auto *CE = dyn_cast_or_null<CXXConstructExpr>(NewLB))
    if (const CXXConstructorDecl *Ctor = CE->getConstructor())
      if ((Ctor->isCopyOrMoveConstructor() ||
           Ctor->isConvertingConstructor(
false)) &&
          CE->getNumArgs() > 0 && CE->getArg(0) != nullptr)
        NewLB = CE->getArg(0)->IgnoreParenImpCasts();
  LB = NewLB;
  return false;
}
bool OpenMPIterationSpaceChecker::SetUB(Expr *NewUB, bool LessOp, bool StrictOp,
                                        SourceRange SR, SourceLocation SL) {
  assert(LCDecl != nullptr && LB != nullptr && UB == nullptr &&
         Step == nullptr && !TestIsLessOp && !TestIsStrictOp);
  if (!NewUB)
    return true;
  UB = NewUB;
  TestIsLessOp = LessOp;
  TestIsStrictOp = StrictOp;
  ConditionSrcRange = SR;
  ConditionLoc = SL;
  return false;
}
bool OpenMPIterationSpaceChecker::SetStep(Expr *NewStep, bool Subtract) {
  assert(LCDecl != nullptr && LB != nullptr && Step == nullptr);
  if (!NewStep)
    return true;
  if (!NewStep->isValueDependent()) {
    SourceLocation StepLoc = NewStep->getLocStart();
    ExprResult Val =
        SemaRef.PerformOpenMPImplicitIntegerConversion(StepLoc, NewStep);
    if (Val.isInvalid())
      return true;
    NewStep = Val.get();
    llvm::APSInt Result;
    bool IsConstant = NewStep->isIntegerConstantExpr(Result, SemaRef.Context);
    bool IsUnsigned = !NewStep->getType()->hasSignedIntegerRepresentation();
    bool IsConstNeg =
        IsConstant && Result.isSigned() && (Subtract != Result.isNegative());
    bool IsConstPos =
        IsConstant && Result.isSigned() && (Subtract == Result.isNegative());
    bool IsConstZero = IsConstant && !Result.getBoolValue();
    if (UB && (IsConstZero ||
               (TestIsLessOp ? (IsConstNeg || (IsUnsigned && Subtract))
                             : (IsConstPos || (IsUnsigned && !Subtract))))) {
      SemaRef.Diag(NewStep->getExprLoc(),
                   diag::err_omp_loop_incr_not_compatible)
          << LCDecl << TestIsLessOp << NewStep->getSourceRange();
      SemaRef.Diag(ConditionLoc,
                   diag::note_omp_loop_cond_requres_compatible_incr)
          << TestIsLessOp << ConditionSrcRange;
      return true;
}
    if (TestIsLessOp == Subtract) {
      NewStep =
          SemaRef.CreateBuiltinUnaryOp(NewStep->getExprLoc(), UO_Minus, NewStep)
              .get();
      Subtract = !Subtract;
}
}
  Step = NewStep;
  SubtractStep = Subtract;
  return false;
}
bool OpenMPIterationSpaceChecker::CheckInit(Stmt *S, bool EmitDiags) {
  if (!S) {
    if (EmitDiags) {
      SemaRef.Diag(DefaultLoc, diag::err_omp_loop_not_canonical_init);
}
    return true;
}
  if (auto *ExprTemp = dyn_cast<ExprWithCleanups>(S))
    if (!ExprTemp->cleanupsHaveSideEffects())
      S = ExprTemp->getSubExpr();
  InitSrcRange = S->getSourceRange();
  if (Expr *E = dyn_cast<Expr>(S))
    S = E->IgnoreParens();
  if (auto *BO = dyn_cast<BinaryOperator>(S)) {
    if (BO->getOpcode() == BO_Assign) {
      auto *LHS = BO->getLHS()->IgnoreParens();
      if (auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
        if (auto *CED = dyn_cast<OMPCapturedExprDecl>(DRE->getDecl()))
          if (auto *ME = dyn_cast<MemberExpr>(getExprAsWritten(CED->getInit())))
            return SetLCDeclAndLB(ME->getMemberDecl(), ME, BO->getRHS());
        return SetLCDeclAndLB(DRE->getDecl(), DRE, BO->getRHS());
}
      if (auto *ME = dyn_cast<MemberExpr>(LHS)) {
        if (ME->isArrow() &&
            isa<CXXThisExpr>(ME->getBase()->IgnoreParenImpCasts()))
          return SetLCDeclAndLB(ME->getMemberDecl(), ME, BO->getRHS());
}
}
} else if (auto *DS = dyn_cast<DeclStmt>(S)) {
    if (DS->isSingleDecl()) {
      if (auto *Var = dyn_cast_or_null<VarDecl>(DS->getSingleDecl())) {
        if (Var->hasInit() && !Var->getType()->isReferenceType()) {
          if (Var->getInitStyle() != VarDecl::CInit && EmitDiags)
            SemaRef.Diag(S->getLocStart(),
                         diag::ext_omp_loop_not_canonical_init)
                << S->getSourceRange();
          return SetLCDeclAndLB(Var, nullptr, Var->getInit());
}
}
}
} else if (auto *CE = dyn_cast<CXXOperatorCallExpr>(S)) {
    if (CE->getOperator() == OO_Equal) {
      auto *LHS = CE->getArg(0);
      if (auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
        if (auto *CED = dyn_cast<OMPCapturedExprDecl>(DRE->getDecl()))
          if (auto *ME = dyn_cast<MemberExpr>(getExprAsWritten(CED->getInit())))
            return SetLCDeclAndLB(ME->getMemberDecl(), ME, BO->getRHS());
        return SetLCDeclAndLB(DRE->getDecl(), DRE, CE->getArg(1));
}
      if (auto *ME = dyn_cast<MemberExpr>(LHS)) {
        if (ME->isArrow() &&
            isa<CXXThisExpr>(ME->getBase()->IgnoreParenImpCasts()))
          return SetLCDeclAndLB(ME->getMemberDecl(), ME, BO->getRHS());
}
}
}
  if (Dependent() || SemaRef.CurContext->isDependentContext())
    return false;
  if (EmitDiags) {
    SemaRef.Diag(S->getLocStart(), diag::err_omp_loop_not_canonical_init)
        << S->getSourceRange();
}
  return true;
}
static const ValueDecl *GetInitLCDecl(Expr *E) {
  if (!E)
    return nullptr;
  E = getExprAsWritten(E);
  if (auto *CE = dyn_cast_or_null<CXXConstructExpr>(E))
    if (const CXXConstructorDecl *Ctor = CE->getConstructor())
      if ((Ctor->isCopyOrMoveConstructor() ||
           Ctor->isConvertingConstructor(
false)) &&
          CE->getNumArgs() > 0 && CE->getArg(0) != nullptr)
        E = CE->getArg(0)->IgnoreParenImpCasts();
  if (auto *DRE = dyn_cast_or_null<DeclRefExpr>(E)) {
    if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      if (auto *CED = dyn_cast<OMPCapturedExprDecl>(VD))
        if (auto *ME = dyn_cast<MemberExpr>(getExprAsWritten(CED->getInit())))
          return getCanonicalDecl(ME->getMemberDecl());
      return getCanonicalDecl(VD);
}
}
  if (auto *ME = dyn_cast_or_null<MemberExpr>(E))
    if (ME->isArrow() && isa<CXXThisExpr>(ME->getBase()->IgnoreParenImpCasts()))
      return getCanonicalDecl(ME->getMemberDecl());
  return nullptr;
}
bool OpenMPIterationSpaceChecker::CheckCond(Expr *S) {
  if (!S) {
    SemaRef.Diag(DefaultLoc, diag::err_omp_loop_not_canonical_cond) << LCDecl;
    return true;
}
  S = getExprAsWritten(S);
  SourceLocation CondLoc = S->getLocStart();
  if (auto *BO = dyn_cast<BinaryOperator>(S)) {
    if (BO->isRelationalOp()) {
      if (GetInitLCDecl(BO->getLHS()) == LCDecl)
        return SetUB(BO->getRHS(),
                     (BO->getOpcode() == BO_LT || BO->getOpcode() == BO_LE),
                     (BO->getOpcode() == BO_LT || BO->getOpcode() == BO_GT),
                     BO->getSourceRange(), BO->getOperatorLoc());
      if (GetInitLCDecl(BO->getRHS()) == LCDecl)
        return SetUB(BO->getLHS(),
                     (BO->getOpcode() == BO_GT || BO->getOpcode() == BO_GE),
                     (BO->getOpcode() == BO_LT || BO->getOpcode() == BO_GT),
                     BO->getSourceRange(), BO->getOperatorLoc());
}
} else if (auto *CE = dyn_cast<CXXOperatorCallExpr>(S)) {
    if (CE->getNumArgs() == 2) {
      auto Op = CE->getOperator();
      switch (Op) {
      case OO_Greater:
      case OO_GreaterEqual:
      case OO_Less:
      case OO_LessEqual:
        if (GetInitLCDecl(CE->getArg(0)) == LCDecl)
          return SetUB(CE->getArg(1), Op == OO_Less || Op == OO_LessEqual,
                       Op == OO_Less || Op == OO_Greater, CE->getSourceRange(),
                       CE->getOperatorLoc());
        if (GetInitLCDecl(CE->getArg(1)) == LCDecl)
          return SetUB(CE->getArg(0), Op == OO_Greater || Op == OO_GreaterEqual,
                       Op == OO_Less || Op == OO_Greater, CE->getSourceRange(),
                       CE->getOperatorLoc());
        break;
      default:
        break;
}
}
}
  if (Dependent() || SemaRef.CurContext->isDependentContext())
    return false;
  SemaRef.Diag(CondLoc, diag::err_omp_loop_not_canonical_cond)
      << S->getSourceRange() << LCDecl;
  return true;
}
bool OpenMPIterationSpaceChecker::CheckIncRHS(Expr *RHS) {
  RHS = RHS->IgnoreParenImpCasts();
  if (auto *BO = dyn_cast<BinaryOperator>(RHS)) {
    if (BO->isAdditiveOp()) {
      bool IsAdd = BO->getOpcode() == BO_Add;
      if (GetInitLCDecl(BO->getLHS()) == LCDecl)
        return SetStep(BO->getRHS(), !IsAdd);
      if (IsAdd && GetInitLCDecl(BO->getRHS()) == LCDecl)
        return SetStep(BO->getLHS(), false);
}
} else if (auto *CE = dyn_cast<CXXOperatorCallExpr>(RHS)) {
    bool IsAdd = CE->getOperator() == OO_Plus;
    if ((IsAdd || CE->getOperator() == OO_Minus) && CE->getNumArgs() == 2) {
      if (GetInitLCDecl(CE->getArg(0)) == LCDecl)
        return SetStep(CE->getArg(1), !IsAdd);
      if (IsAdd && GetInitLCDecl(CE->getArg(1)) == LCDecl)
        return SetStep(CE->getArg(0), false);
}
}
  if (Dependent() || SemaRef.CurContext->isDependentContext())
    return false;
  SemaRef.Diag(RHS->getLocStart(), diag::err_omp_loop_not_canonical_incr)
      << RHS->getSourceRange() << LCDecl;
  return true;
}
bool OpenMPIterationSpaceChecker::CheckInc(Expr *S) {
  if (!S) {
    SemaRef.Diag(DefaultLoc, diag::err_omp_loop_not_canonical_incr) << LCDecl;
    return true;
}
  if (auto *ExprTemp = dyn_cast<ExprWithCleanups>(S))
    if (!ExprTemp->cleanupsHaveSideEffects())
      S = ExprTemp->getSubExpr();
  IncrementSrcRange = S->getSourceRange();
  S = S->IgnoreParens();
  if (auto *UO = dyn_cast<UnaryOperator>(S)) {
    if (UO->isIncrementDecrementOp() &&
        GetInitLCDecl(UO->getSubExpr()) == LCDecl)
      return SetStep(SemaRef
                         .ActOnIntegerConstant(UO->getLocStart(),
                                               (UO->isDecrementOp() ? -1 : 1))
                         .get(),
                     false);
} else if (auto *BO = dyn_cast<BinaryOperator>(S)) {
    switch (BO->getOpcode()) {
    case BO_AddAssign:
    case BO_SubAssign:
      if (GetInitLCDecl(BO->getLHS()) == LCDecl)
        return SetStep(BO->getRHS(), BO->getOpcode() == BO_SubAssign);
      break;
    case BO_Assign:
      if (GetInitLCDecl(BO->getLHS()) == LCDecl)
        return CheckIncRHS(BO->getRHS());
      break;
    default:
      break;
}
} else if (auto *CE = dyn_cast<CXXOperatorCallExpr>(S)) {
    switch (CE->getOperator()) {
    case OO_PlusPlus:
    case OO_MinusMinus:
      if (GetInitLCDecl(CE->getArg(0)) == LCDecl)
        return SetStep(SemaRef
                           .ActOnIntegerConstant(
                               CE->getLocStart(),
                               ((CE->getOperator() == OO_MinusMinus) ? -1 : 1))
                           .get(),
                       false);
      break;
    case OO_PlusEqual:
    case OO_MinusEqual:
      if (GetInitLCDecl(CE->getArg(0)) == LCDecl)
        return SetStep(CE->getArg(1), CE->getOperator() == OO_MinusEqual);
      break;
    case OO_Equal:
      if (GetInitLCDecl(CE->getArg(0)) == LCDecl)
        return CheckIncRHS(CE->getArg(1));
      break;
    default:
      break;
}
}
  if (Dependent() || SemaRef.CurContext->isDependentContext())
    return false;
  SemaRef.Diag(S->getLocStart(), diag::err_omp_loop_not_canonical_incr)
      << S->getSourceRange() << LCDecl;
  return true;
}
static ExprResult
tryBuildCapture(Sema &SemaRef, Expr *Capture,
                llvm::MapVector<Expr *, DeclRefExpr *> &Captures) {
  if (SemaRef.CurContext->isDependentContext())
    return ExprResult(Capture);
  if (Capture->isEvaluatable(SemaRef.Context, Expr::SE_AllowSideEffects))
    return SemaRef.PerformImplicitConversion(
        Capture->IgnoreImpCasts(), Capture->getType(), Sema::AA_Converting,
true);
  auto I = Captures.find(Capture);
  if (I != Captures.end())
    return buildCapture(SemaRef, Capture, I->second);
  DeclRefExpr *Ref = nullptr;
  ExprResult Res = buildCapture(SemaRef, Capture, Ref);
  Captures[Capture] = Ref;
  return Res;
}
Expr *OpenMPIterationSpaceChecker::BuildNumIterations(
    Scope *S, const bool LimitedType,
    llvm::MapVector<Expr *, DeclRefExpr *> &Captures) const {
  ExprResult Diff;
  auto VarType = LCDecl->getType().getNonReferenceType();
  if (VarType->isIntegerType() || VarType->isPointerType() ||
      SemaRef.getLangOpts().CPlusPlus) {
    auto *UBExpr = TestIsLessOp ? UB : LB;
    auto *LBExpr = TestIsLessOp ? LB : UB;
    Expr *Upper = tryBuildCapture(SemaRef, UBExpr, Captures).get();
    Expr *Lower = tryBuildCapture(SemaRef, LBExpr, Captures).get();
    if (!Upper || !Lower)
      return nullptr;
    Diff = SemaRef.BuildBinOp(S, DefaultLoc, BO_Sub, Upper, Lower);
    if (!Diff.isUsable() && VarType->getAsCXXRecordDecl()) {
      SemaRef.Diag(Upper->getLocStart(), diag::err_omp_loop_diff_cxx)
          << Upper->getSourceRange() << Lower->getSourceRange();
      return nullptr;
}
}
  if (!Diff.isUsable())
    return nullptr;
  if (TestIsStrictOp)
    Diff = SemaRef.BuildBinOp(
        S, DefaultLoc, BO_Sub, Diff.get(),
        SemaRef.ActOnIntegerConstant(SourceLocation(), 1).get());
  if (!Diff.isUsable())
    return nullptr;
  auto NewStep = tryBuildCapture(SemaRef, Step, Captures);
  if (!NewStep.isUsable())
    return nullptr;
  Diff = SemaRef.BuildBinOp(S, DefaultLoc, BO_Add, Diff.get(), NewStep.get());
  if (!Diff.isUsable())
    return nullptr;
  Diff = SemaRef.ActOnParenExpr(DefaultLoc, DefaultLoc, Diff.get());
  if (!Diff.isUsable())
    return nullptr;
  Diff = SemaRef.BuildBinOp(S, DefaultLoc, BO_Div, Diff.get(), NewStep.get());
  if (!Diff.isUsable())
    return nullptr;
  QualType Type = Diff.get()->getType();
  auto &C = SemaRef.Context;
  bool UseVarType = VarType->hasIntegerRepresentation() &&
                    C.getTypeSize(Type) > C.getTypeSize(VarType);
  if (!Type->isIntegerType() || UseVarType) {
    unsigned NewSize =
        UseVarType ? C.getTypeSize(VarType) : C.getTypeSize(Type);
    bool IsSigned = UseVarType ? VarType->hasSignedIntegerRepresentation()
                               : Type->hasSignedIntegerRepresentation();
    Type = C.getIntTypeForBitwidth(NewSize, IsSigned);
    if (!SemaRef.Context.hasSameType(Diff.get()->getType(), Type)) {
      Diff = SemaRef.PerformImplicitConversion(
          Diff.get(), Type, Sema::AA_Converting, 
true);
      if (!Diff.isUsable())
        return nullptr;
}
}
  if (LimitedType) {
    unsigned NewSize = (C.getTypeSize(Type) > 32) ? 64 : 32;
    if (NewSize != C.getTypeSize(Type)) {
      if (NewSize < C.getTypeSize(Type)) {
        assert(NewSize == 64 && "incorrect loop var size");
        SemaRef.Diag(DefaultLoc, diag::warn_omp_loop_64_bit_var)
            << InitSrcRange << ConditionSrcRange;
}
      QualType NewType = C.getIntTypeForBitwidth(
          NewSize, Type->hasSignedIntegerRepresentation() ||
                       C.getTypeSize(Type) < NewSize);
      if (!SemaRef.Context.hasSameType(Diff.get()->getType(), NewType)) {
        Diff = SemaRef.PerformImplicitConversion(Diff.get(), NewType,
                                                 Sema::AA_Converting, true);
        if (!Diff.isUsable())
          return nullptr;
}
}
}
  return Diff.get();
}
Expr *OpenMPIterationSpaceChecker::BuildPreCond(
    Scope *S, Expr *Cond,
    llvm::MapVector<Expr *, DeclRefExpr *> &Captures) const {
  bool Suppress = SemaRef.getDiagnostics().getSuppressAllDiagnostics();
  SemaRef.getDiagnostics().setSuppressAllDiagnostics(
true);
  auto NewLB = tryBuildCapture(SemaRef, LB, Captures);
  auto NewUB = tryBuildCapture(SemaRef, UB, Captures);
  if (!NewLB.isUsable() || !NewUB.isUsable())
    return nullptr;
  auto CondExpr = SemaRef.BuildBinOp(
      S, DefaultLoc, TestIsLessOp ? (TestIsStrictOp ? BO_LT : BO_LE)
                                  : (TestIsStrictOp ? BO_GT : BO_GE),
      NewLB.get(), NewUB.get());
  if (CondExpr.isUsable()) {
    if (!SemaRef.Context.hasSameUnqualifiedType(CondExpr.get()->getType(),
                                                SemaRef.Context.BoolTy))
      CondExpr = SemaRef.PerformImplicitConversion(
          CondExpr.get(), SemaRef.Context.BoolTy, 
Sema::AA_Casting,
true);
}
  SemaRef.getDiagnostics().setSuppressAllDiagnostics(Suppress);
  return CondExpr.isUsable() ? CondExpr.get() : Cond;
}
DeclRefExpr *OpenMPIterationSpaceChecker::BuildCounterVar(
    llvm::MapVector<Expr *, DeclRefExpr *> &Captures, DSAStackTy &DSA) const {
  auto *VD = dyn_cast<VarDecl>(LCDecl);
  if (!VD) {
    VD = SemaRef.IsOpenMPCapturedDecl(LCDecl);
    auto *Ref = buildDeclRefExpr(
        SemaRef, VD, VD->getType().getNonReferenceType(), DefaultLoc);
    DSAStackTy::DSAVarData Data = DSA.getTopDSA(LCDecl, 
false);
    if (!isOpenMPPrivate(Data.CKind) || !Data.RefExpr)
      Captures.insert(std::make_pair(LCRef, Ref));
    return Ref;
}
  return buildDeclRefExpr(SemaRef, VD, VD->getType().getNonReferenceType(),
                          DefaultLoc);
}
Expr *OpenMPIterationSpaceChecker::BuildPrivateCounterVar() const {
  if (LCDecl && !LCDecl->isInvalidDecl()) {
    auto Type = LCDecl->getType().getNonReferenceType();
    auto *PrivateVar =
        buildVarDecl(SemaRef, DefaultLoc, Type, LCDecl->getName(),
                     LCDecl->hasAttrs() ? &LCDecl->getAttrs() : nullptr);
    if (PrivateVar->isInvalidDecl())
      return nullptr;
    return buildDeclRefExpr(SemaRef, PrivateVar, Type, DefaultLoc);
}
  return nullptr;
}
Expr *OpenMPIterationSpaceChecker::BuildCounterInit() const { return LB; 
}
Expr *OpenMPIterationSpaceChecker::BuildCounterStep() const { return Step; 
}
struct LoopIterationSpace final {
  Expr *PreCond = nullptr;
  Expr *NumIterations = nullptr;
  Expr *CounterVar = nullptr;
  Expr *PrivateCounterVar = nullptr;
  Expr *CounterInit = nullptr;
  Expr *CounterStep = nullptr;
  bool Subtract = false;
  SourceRange InitSrcRange;
  SourceRange CondSrcRange;
  SourceRange IncSrcRange;
};
} 
void Sema::ActOnOpenMPLoopInitialization(SourceLocation ForLoc, Stmt *Init) {
  assert(getLangOpts().OpenMP && "OpenMP is not active.");
  assert(Init && "Expected loop in canonical form.");
  unsigned AssociatedLoops = DSAStack->getAssociatedLoops();
  if (AssociatedLoops > 0 &&
      isOpenMPLoopDirective(DSAStack->getCurrentDirective())) {
    OpenMPIterationSpaceChecker ISC(*this, ForLoc);
    if (!ISC.CheckInit(Init, 
false)) {
      if (auto *D = ISC.GetLoopDecl()) {
        auto *VD = dyn_cast<VarDecl>(D);
        if (!VD) {
          if (auto *Private = IsOpenMPCapturedDecl(D))
            VD = Private;
          else {
            auto *Ref = buildCapture(*this, D, ISC.GetLoopDeclRefExpr(),
false);
            VD = cast<VarDecl>(Ref->getDecl());
}
}
        DSAStack->addLoopControlVariable(D, VD);
}
}
    DSAStack->setAssociatedLoops(AssociatedLoops - 1);
}
}
static bool CheckOpenMPIterationSpace(
    OpenMPDirectiveKind DKind, Stmt *S, Sema &SemaRef, DSAStackTy &DSA,
    unsigned CurrentNestedLoopCount, unsigned NestedLoopCount,
    Expr *CollapseLoopCountExpr, Expr *OrderedLoopCountExpr,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA,
    LoopIterationSpace &ResultIterSpace,
    llvm::MapVector<Expr *, DeclRefExpr *> &Captures) {
  auto *For = dyn_cast_or_null<ForStmt>(S);
  if (!For) {
    SemaRef.Diag(S->getLocStart(), diag::err_omp_not_for)
        << (CollapseLoopCountExpr != nullptr || OrderedLoopCountExpr != nullptr)
        << getOpenMPDirectiveName(DKind) << NestedLoopCount
        << (CurrentNestedLoopCount > 0) << CurrentNestedLoopCount;
    if (NestedLoopCount > 1) {
      if (CollapseLoopCountExpr && OrderedLoopCountExpr)
        SemaRef.Diag(DSA.getConstructLoc(),
                     diag::note_omp_collapse_ordered_expr)
            << 2 << CollapseLoopCountExpr->getSourceRange()
            << OrderedLoopCountExpr->getSourceRange();
      else if (CollapseLoopCountExpr)
        SemaRef.Diag(CollapseLoopCountExpr->getExprLoc(),
                     diag::note_omp_collapse_ordered_expr)
            << 0 << CollapseLoopCountExpr->getSourceRange();
      else
        SemaRef.Diag(OrderedLoopCountExpr->getExprLoc(),
                     diag::note_omp_collapse_ordered_expr)
            << 1 << OrderedLoopCountExpr->getSourceRange();
}
    return true;
}
  assert(For->getBody());
  OpenMPIterationSpaceChecker ISC(SemaRef, For->getForLoc());
  auto Init = For->getInit();
  if (ISC.CheckInit(Init))
    return true;
  bool HasErrors = false;
  if (auto *LCDecl = ISC.GetLoopDecl()) {
    auto *LoopDeclRefExpr = ISC.GetLoopDeclRefExpr();
    auto VarType = LCDecl->getType().getNonReferenceType();
    if (!VarType->isDependentType() && !VarType->isIntegerType() &&
        !VarType->isPointerType() &&
        !(SemaRef.getLangOpts().CPlusPlus && VarType->isOverloadableType())) {
      SemaRef.Diag(Init->getLocStart(), diag::err_omp_loop_variable_type)
          << SemaRef.getLangOpts().CPlusPlus;
      HasErrors = true;
}
    VarsWithImplicitDSA.erase(LCDecl);
    DSAStackTy::DSAVarData DVar = DSA.getTopDSA(LCDecl, false);
    auto PredeterminedCKind =
        isOpenMPSimdDirective(DKind)
            ? ((NestedLoopCount == 1) ? OMPC_linear : OMPC_lastprivate)
            : OMPC_private;
    if (((isOpenMPSimdDirective(DKind) && DVar.CKind != OMPC_unknown &&
          DVar.CKind != PredeterminedCKind) ||
         ((isOpenMPWorksharingDirective(DKind) || DKind == OMPD_taskloop ||
           isOpenMPDistributeDirective(DKind)) &&
          !isOpenMPSimdDirective(DKind) && DVar.CKind != OMPC_unknown &&
          DVar.CKind != OMPC_private && DVar.CKind != OMPC_lastprivate)) &&
        (DVar.CKind != OMPC_private || DVar.RefExpr != nullptr)) {
      SemaRef.Diag(Init->getLocStart(), diag::err_omp_loop_var_dsa)
          << getOpenMPClauseName(DVar.CKind) << getOpenMPDirectiveName(DKind)
          << getOpenMPClauseName(PredeterminedCKind);
      if (DVar.RefExpr == nullptr)
        DVar.CKind = PredeterminedCKind;
      ReportOriginalDSA(SemaRef, &DSA, LCDecl, DVar, 
true);
      HasErrors = true;
} else if (LoopDeclRefExpr != nullptr) {
      if (DVar.CKind == OMPC_unknown)
        DVar = DSA.hasDSA(LCDecl, isOpenMPPrivate,
                          [](OpenMPDirectiveKind) -> bool { return true; 
},
false);
      DSA.addDSA(LCDecl, LoopDeclRefExpr, PredeterminedCKind);
}
    assert(isOpenMPLoopDirective(DKind) && "DSA for non-loop vars");
    HasErrors |= ISC.CheckCond(For->getCond());
    HasErrors |= ISC.CheckInc(For->getInc());
}
  if (ISC.Dependent() || SemaRef.CurContext->isDependentContext() || HasErrors)
    return HasErrors;
  ResultIterSpace.PreCond =
      ISC.BuildPreCond(DSA.getCurScope(), For->getCond(), Captures);
  ResultIterSpace.NumIterations = ISC.BuildNumIterations(
      DSA.getCurScope(),
      (isOpenMPWorksharingDirective(DKind) ||
       isOpenMPTaskLoopDirective(DKind) || isOpenMPDistributeDirective(DKind)),
      Captures);
  ResultIterSpace.CounterVar = ISC.BuildCounterVar(Captures, DSA);
  ResultIterSpace.PrivateCounterVar = ISC.BuildPrivateCounterVar();
  ResultIterSpace.CounterInit = ISC.BuildCounterInit();
  ResultIterSpace.CounterStep = ISC.BuildCounterStep();
  ResultIterSpace.InitSrcRange = ISC.GetInitSrcRange();
  ResultIterSpace.CondSrcRange = ISC.GetConditionSrcRange();
  ResultIterSpace.IncSrcRange = ISC.GetIncrementSrcRange();
  ResultIterSpace.Subtract = ISC.ShouldSubtractStep();
  HasErrors |= (ResultIterSpace.PreCond == nullptr ||
                ResultIterSpace.NumIterations == nullptr ||
                ResultIterSpace.CounterVar == nullptr ||
                ResultIterSpace.PrivateCounterVar == nullptr ||
                ResultIterSpace.CounterInit == nullptr ||
                ResultIterSpace.CounterStep == nullptr);
  return HasErrors;
}
static ExprResult
BuildCounterInit(Sema &SemaRef, Scope *S, SourceLocation Loc, ExprResult VarRef,
                 ExprResult Start,
                 llvm::MapVector<Expr *, DeclRefExpr *> &Captures) {
  auto NewStart = tryBuildCapture(SemaRef, Start.get(), Captures);
  if (!NewStart.isUsable())
    return ExprError();
  if (!SemaRef.Context.hasSameType(NewStart.get()->getType(),
                                   VarRef.get()->getType())) {
    NewStart = SemaRef.PerformImplicitConversion(
        NewStart.get(), VarRef.get()->getType(), Sema::AA_Converting,
true);
    if (!NewStart.isUsable())
      return ExprError();
}
  auto Init =
      SemaRef.BuildBinOp(S, Loc, BO_Assign, VarRef.get(), NewStart.get());
  return Init;
}
static ExprResult
BuildCounterUpdate(Sema &SemaRef, Scope *S, SourceLocation Loc,
                   ExprResult VarRef, ExprResult Start, ExprResult Iter,
                   ExprResult Step, bool Subtract,
                   llvm::MapVector<Expr *, DeclRefExpr *> *Captures = nullptr) {
  Iter = SemaRef.ActOnParenExpr(Loc, Loc, Iter.get());
  if (!VarRef.isUsable() || !Start.isUsable() || !Iter.isUsable() ||
      !Step.isUsable())
    return ExprError();
  ExprResult NewStep = Step;
  if (Captures)
    NewStep = tryBuildCapture(SemaRef, Step.get(), *Captures);
  if (NewStep.isInvalid())
    return ExprError();
  ExprResult Update =
      SemaRef.BuildBinOp(S, Loc, BO_Mul, Iter.get(), NewStep.get());
  if (!Update.isUsable())
    return ExprError();
  ExprResult NewStart = Start;
  if (Captures)
    NewStart = tryBuildCapture(SemaRef, Start.get(), *Captures);
  if (NewStart.isInvalid())
    return ExprError();
  ExprResult SavedUpdate = Update;
  ExprResult UpdateVal;
  if (VarRef.get()->getType()->isOverloadableType() ||
      NewStart.get()->getType()->isOverloadableType() ||
      Update.get()->getType()->isOverloadableType()) {
    bool Suppress = SemaRef.getDiagnostics().getSuppressAllDiagnostics();
    SemaRef.getDiagnostics().setSuppressAllDiagnostics(
true);
    Update =
        SemaRef.BuildBinOp(S, Loc, BO_Assign, VarRef.get(), NewStart.get());
    if (Update.isUsable()) {
      UpdateVal =
          SemaRef.BuildBinOp(S, Loc, Subtract ? BO_SubAssign : BO_AddAssign,
                             VarRef.get(), SavedUpdate.get());
      if (UpdateVal.isUsable()) {
        Update = SemaRef.CreateBuiltinBinOp(Loc, BO_Comma, Update.get(),
                                            UpdateVal.get());
}
}
    SemaRef.getDiagnostics().setSuppressAllDiagnostics(Suppress);
}
  if (!Update.isUsable() || !UpdateVal.isUsable()) {
    Update = SemaRef.BuildBinOp(S, Loc, Subtract ? BO_Sub : BO_Add,
                                NewStart.get(), SavedUpdate.get());
    if (!Update.isUsable())
      return ExprError();
    if (!SemaRef.Context.hasSameType(Update.get()->getType(),
                                     VarRef.get()->getType())) {
      Update = SemaRef.PerformImplicitConversion(
          Update.get(), VarRef.get()->getType(), Sema::AA_Converting, true);
      if (!Update.isUsable())
        return ExprError();
}
    Update = SemaRef.BuildBinOp(S, Loc, BO_Assign, VarRef.get(), Update.get());
}
  return Update;
}
static ExprResult WidenIterationCount(unsigned Bits, Expr *E, Sema &SemaRef) {
  if (E == nullptr)
    return ExprError();
  auto &C = SemaRef.Context;
  QualType OldType = E->getType();
  unsigned HasBits = C.getTypeSize(OldType);
  if (HasBits >= Bits)
    return ExprResult(E);
  QualType NewType = C.getIntTypeForBitwidth(Bits, 
 true);
  return SemaRef.PerformImplicitConversion(E, NewType, Sema::AA_Converting,
                                           true);
}
static bool FitsInto(unsigned Bits, bool Signed, Expr *E, Sema &SemaRef) {
  if (E == nullptr)
    return false;
  llvm::APSInt Result;
  if (E->isIntegerConstantExpr(Result, SemaRef.Context))
    return Signed ? Result.isSignedIntN(Bits) : Result.isIntN(Bits);
  return false;
}
static Stmt *buildPreInits(ASTContext &Context,
                           SmallVectorImpl<Decl *> &PreInits) {
  if (!PreInits.empty()) {
    return new (Context) DeclStmt(
        DeclGroupRef::Create(Context, PreInits.begin(), PreInits.size()),
        SourceLocation(), SourceLocation());
}
  return nullptr;
}
static Stmt *buildPreInits(ASTContext &Context,
                           llvm::MapVector<Expr *, DeclRefExpr *> &Captures) {
  if (!Captures.empty()) {
    SmallVector<Decl *, 16> PreInits;
for_loop_talkad7420();
    for (auto &Pair : Captures)
{
      PreInits.push_back(Pair.second->getDecl());
}
    return buildPreInits(Context, PreInits);
}
  return nullptr;
}
static Expr *buildPostUpdate(Sema &S, ArrayRef<Expr *> PostUpdates) {
  Expr *PostUpdate = nullptr;
  if (!PostUpdates.empty()) {
for_loop_talkad7420();
    for (auto *E : PostUpdates) {
      Expr *ConvE = S.BuildCStyleCastExpr(
                         E->getExprLoc(),
                         S.Context.getTrivialTypeSourceInfo(S.Context.VoidTy),
                         E->getExprLoc(), E)
                        .get();
      PostUpdate = PostUpdate
                       ? S.CreateBuiltinBinOp(ConvE->getExprLoc(), BO_Comma,
                                              PostUpdate, ConvE)
                             .get()
                       : ConvE;
}
}
  return PostUpdate;
}
static unsigned
CheckOpenMPLoop(OpenMPDirectiveKind DKind, Expr *CollapseLoopCountExpr,
                Expr *OrderedLoopCountExpr, Stmt *AStmt, Sema &SemaRef,
                DSAStackTy &DSA,
                llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA,
                OMPLoopDirective::HelperExprs &Built) {
  unsigned NestedLoopCount = 1;
  if (CollapseLoopCountExpr) {
    llvm::APSInt Result;
    if (CollapseLoopCountExpr->EvaluateAsInt(Result, SemaRef.getASTContext()))
      NestedLoopCount = Result.getLimitedValue();
}
  if (OrderedLoopCountExpr) {
    llvm::APSInt Result;
    if (OrderedLoopCountExpr->EvaluateAsInt(Result, SemaRef.getASTContext())) {
      if (Result.getLimitedValue() < NestedLoopCount) {
        SemaRef.Diag(OrderedLoopCountExpr->getExprLoc(),
                     diag::err_omp_wrong_ordered_loop_count)
            << OrderedLoopCountExpr->getSourceRange();
        SemaRef.Diag(CollapseLoopCountExpr->getExprLoc(),
                     diag::note_collapse_loop_count)
            << CollapseLoopCountExpr->getSourceRange();
}
      NestedLoopCount = Result.getLimitedValue();
}
}
  llvm::MapVector<Expr *, DeclRefExpr *> Captures;
  SmallVector<LoopIterationSpace, 4> IterSpaces;
  IterSpaces.resize(NestedLoopCount);
  Stmt *CurStmt = AStmt->IgnoreContainers(
 true);
for_loop_talkad7420();
  for (unsigned Cnt = 0; Cnt < NestedLoopCount; ++Cnt) {
    if (CheckOpenMPIterationSpace(DKind, CurStmt, SemaRef, DSA, Cnt,
                                  NestedLoopCount, CollapseLoopCountExpr,
                                  OrderedLoopCountExpr, VarsWithImplicitDSA,
                                  IterSpaces[Cnt], Captures))
      return 0;
    CurStmt = cast<ForStmt>(CurStmt)->getBody()->IgnoreContainers();
}
  Built.clear(
 NestedLoopCount);
  if (SemaRef.CurContext->isDependentContext())
    return NestedLoopCount;
  auto PreCond = ExprResult(IterSpaces[0].PreCond);
  auto N0 = IterSpaces[0].NumIterations;
  ExprResult LastIteration32 = WidenIterationCount(
      32 
, SemaRef
                         .PerformImplicitConversion(
                             N0->IgnoreImpCasts(), N0->getType(),
                             Sema::AA_Converting, 
true)
                         .get(),
      SemaRef);
  ExprResult LastIteration64 = WidenIterationCount(
      64 
, SemaRef
                         .PerformImplicitConversion(
                             N0->IgnoreImpCasts(), N0->getType(),
                             Sema::AA_Converting, 
true)
                         .get(),
      SemaRef);
  if (!LastIteration32.isUsable() || !LastIteration64.isUsable())
    return NestedLoopCount;
  auto &C = SemaRef.Context;
  bool AllCountsNeedLessThan32Bits = C.getTypeSize(N0->getType()) < 32;
  Scope *CurScope = DSA.getCurScope();
for_loop_talkad7420();
  for (unsigned Cnt = 1; Cnt < NestedLoopCount; ++Cnt) {
    if (PreCond.isUsable()) {
      PreCond =
          SemaRef.BuildBinOp(CurScope, PreCond.get()->getExprLoc(), BO_LAnd,
                             PreCond.get(), IterSpaces[Cnt].PreCond);
}
    auto N = IterSpaces[Cnt].NumIterations;
    SourceLocation Loc = N->getExprLoc();
    AllCountsNeedLessThan32Bits &= C.getTypeSize(N->getType()) < 32;
    if (LastIteration32.isUsable())
      LastIteration32 = SemaRef.BuildBinOp(
          CurScope, Loc, BO_Mul, LastIteration32.get(),
          SemaRef
              .PerformImplicitConversion(N->IgnoreImpCasts(), N->getType(),
                                         Sema::AA_Converting,
true)
              .get());
    if (LastIteration64.isUsable())
      LastIteration64 = SemaRef.BuildBinOp(
          CurScope, Loc, BO_Mul, LastIteration64.get(),
          SemaRef
              .PerformImplicitConversion(N->IgnoreImpCasts(), N->getType(),
                                         Sema::AA_Converting,
true)
              .get());
}
  ExprResult LastIteration = LastIteration64;
  if (LastIteration32.isUsable() &&
      C.getTypeSize(LastIteration32.get()->getType()) == 32 &&
      (AllCountsNeedLessThan32Bits || NestedLoopCount == 1 ||
       FitsInto(
           32 
,
           LastIteration32.get()->getType()->hasSignedIntegerRepresentation(),
           LastIteration64.get(), SemaRef)))
    LastIteration = LastIteration32;
  QualType VType = LastIteration.get()->getType();
  QualType RealVType = VType;
  QualType StrideVType = VType;
  if (isOpenMPTaskLoopDirective(DKind)) {
    VType =
        SemaRef.Context.getIntTypeForBitwidth(
64, 
0);
    StrideVType =
        SemaRef.Context.getIntTypeForBitwidth(
64, 
1);
}
  if (!LastIteration.isUsable())
    return 0;
  ExprResult NumIterations = LastIteration;
  {
    LastIteration = SemaRef.BuildBinOp(
        CurScope, LastIteration.get()->getExprLoc(), BO_Sub,
        LastIteration.get(),
        SemaRef.ActOnIntegerConstant(SourceLocation(), 1).get());
    if (!LastIteration.isUsable())
      return 0;
}
  llvm::APSInt Result;
  bool IsConstant =
      LastIteration.get()->isIntegerConstantExpr(Result, SemaRef.Context);
  ExprResult CalcLastIteration;
  if (!IsConstant) {
    ExprResult SaveRef =
        tryBuildCapture(SemaRef, LastIteration.get(), Captures);
    LastIteration = SaveRef;
    NumIterations = SemaRef.BuildBinOp(
        CurScope, SaveRef.get()->getExprLoc(), BO_Add, SaveRef.get(),
        SemaRef.ActOnIntegerConstant(SourceLocation(), 1).get());
    if (!NumIterations.isUsable())
      return 0;
}
  SourceLocation InitLoc = IterSpaces[0].InitSrcRange.getBegin();
  ExprResult LB, UB, IL, ST, EUB, PrevLB, PrevUB;
  if (isOpenMPWorksharingDirective(DKind) || isOpenMPTaskLoopDirective(DKind) ||
      isOpenMPDistributeDirective(DKind)) {
    VarDecl *LBDecl = buildVarDecl(SemaRef, InitLoc, VType, ".omp.lb");
    LB = buildDeclRefExpr(SemaRef, LBDecl, VType, InitLoc);
    SemaRef.AddInitializerToDecl(LBDecl,
                                 SemaRef.ActOnIntegerConstant(InitLoc, 0).get(),
 false);
    VarDecl *UBDecl = buildVarDecl(SemaRef, InitLoc, VType, ".omp.ub");
    UB = buildDeclRefExpr(SemaRef, UBDecl, VType, InitLoc);
    SemaRef.AddInitializerToDecl(UBDecl, LastIteration.get(),
 false);
    QualType Int32Ty = SemaRef.Context.getIntTypeForBitwidth(32, true);
    VarDecl *ILDecl = buildVarDecl(SemaRef, InitLoc, Int32Ty, ".omp.is_last");
    IL = buildDeclRefExpr(SemaRef, ILDecl, Int32Ty, InitLoc);
    SemaRef.AddInitializerToDecl(ILDecl,
                                 SemaRef.ActOnIntegerConstant(InitLoc, 0).get(),
 false);
    VarDecl *STDecl =
        buildVarDecl(SemaRef, InitLoc, StrideVType, ".omp.stride");
    ST = buildDeclRefExpr(SemaRef, STDecl, StrideVType, InitLoc);
    SemaRef.AddInitializerToDecl(STDecl,
                                 SemaRef.ActOnIntegerConstant(InitLoc, 1).get(),
 false);
    ExprResult IsUBGreater = SemaRef.BuildBinOp(CurScope, InitLoc, BO_GT,
                                                UB.get(), LastIteration.get());
    ExprResult CondOp = SemaRef.ActOnConditionalOp(
        InitLoc, InitLoc, IsUBGreater.get(), LastIteration.get(), UB.get());
    EUB = SemaRef.BuildBinOp(CurScope, InitLoc, BO_Assign, UB.get(),
                             CondOp.get());
    EUB = SemaRef.ActOnFinishFullExpr(EUB.get());
    if (isOpenMPLoopBoundSharingDirective(DKind)) {
      auto *CD = cast<CapturedStmt>(AStmt)->getCapturedDecl();
      assert(CD->getNumParams() >= 4 &&
             "Unexpected number of parameters in loop combined directive");
      auto *PrevLBDecl = CD->getParam(
2);
      auto *PrevUBDecl = CD->getParam(
3);
      PrevLB =
          buildDeclRefExpr(SemaRef, PrevLBDecl, PrevLBDecl->getType(), InitLoc);
      PrevUB =
          buildDeclRefExpr(SemaRef, PrevUBDecl, PrevUBDecl->getType(), InitLoc);
}
}
  ExprResult IV;
  ExprResult Init;
  {
    VarDecl *IVDecl = buildVarDecl(SemaRef, InitLoc, RealVType, ".omp.iv");
    IV = buildDeclRefExpr(SemaRef, IVDecl, RealVType, InitLoc);
    Expr *RHS =
        (isOpenMPWorksharingDirective(DKind) ||
         isOpenMPTaskLoopDirective(DKind) || isOpenMPDistributeDirective(DKind))
            ? LB.get()
            : SemaRef.ActOnIntegerConstant(SourceLocation(), 0).get();
    Init = SemaRef.BuildBinOp(CurScope, InitLoc, BO_Assign, IV.get(), RHS);
    Init = SemaRef.ActOnFinishFullExpr(Init.get());
}
  SourceLocation CondLoc;
  ExprResult Cond =
      (isOpenMPWorksharingDirective(DKind) ||
       isOpenMPTaskLoopDirective(DKind) || isOpenMPDistributeDirective(DKind))
          ? SemaRef.BuildBinOp(CurScope, CondLoc, BO_LE, IV.get(), UB.get())
          : SemaRef.BuildBinOp(CurScope, CondLoc, BO_LT, IV.get(),
                               NumIterations.get());
  SourceLocation IncLoc;
  ExprResult Inc =
      SemaRef.BuildBinOp(CurScope, IncLoc, BO_Add, IV.get(),
                         SemaRef.ActOnIntegerConstant(IncLoc, 1).get());
  if (!Inc.isUsable())
    return 0;
  Inc = SemaRef.BuildBinOp(CurScope, IncLoc, BO_Assign, IV.get(), Inc.get());
  Inc = SemaRef.ActOnFinishFullExpr(Inc.get());
  if (!Inc.isUsable())
    return 0;
  ExprResult NextLB, NextUB;
  if (isOpenMPWorksharingDirective(DKind) || isOpenMPTaskLoopDirective(DKind) ||
      isOpenMPDistributeDirective(DKind)) {
    NextLB = SemaRef.BuildBinOp(CurScope, IncLoc, BO_Add, LB.get(), ST.get());
    if (!NextLB.isUsable())
      return 0;
    NextLB =
        SemaRef.BuildBinOp(CurScope, IncLoc, BO_Assign, LB.get(), NextLB.get());
    NextLB = SemaRef.ActOnFinishFullExpr(NextLB.get());
    if (!NextLB.isUsable())
      return 0;
    NextUB = SemaRef.BuildBinOp(CurScope, IncLoc, BO_Add, UB.get(), ST.get());
    if (!NextUB.isUsable())
      return 0;
    NextUB =
        SemaRef.BuildBinOp(CurScope, IncLoc, BO_Assign, UB.get(), NextUB.get());
    NextUB = SemaRef.ActOnFinishFullExpr(NextUB.get());
    if (!NextUB.isUsable())
      return 0;
}
  bool HasErrors = false;
  Built.Counters.resize(NestedLoopCount);
  Built.Inits.resize(NestedLoopCount);
  Built.Updates.resize(NestedLoopCount);
  Built.Finals.resize(NestedLoopCount);
  SmallVector<Expr *, 4> LoopMultipliers;
  {
    ExprResult Div;
for_loop_talkad7420();
    for (int Cnt = NestedLoopCount - 1; Cnt >= 0; --Cnt) {
      LoopIterationSpace &IS = IterSpaces[Cnt];
      SourceLocation UpdLoc = IS.IncSrcRange.getBegin();
      ExprResult Iter;
      if (Div.isUsable()) {
        Iter =
            SemaRef.BuildBinOp(CurScope, UpdLoc, BO_Div, IV.get(), Div.get());
} else {
        Iter = IV;
        assert((Cnt == (int)NestedLoopCount - 1) &&
               "unusable div expected on first iteration only");
}
      if (Cnt != 0 && Iter.isUsable())
        Iter = SemaRef.BuildBinOp(CurScope, UpdLoc, BO_Rem, Iter.get(),
                                  IS.NumIterations);
      if (!Iter.isUsable()) {
        HasErrors = true;
        break;
}
      auto *VD = cast<VarDecl>(cast<DeclRefExpr>(IS.CounterVar)->getDecl());
      auto *CounterVar = buildDeclRefExpr(SemaRef, VD, IS.CounterVar->getType(),
                                          IS.CounterVar->getExprLoc(),
true);
      ExprResult Init = BuildCounterInit(SemaRef, CurScope, UpdLoc, CounterVar,
                                         IS.CounterInit, Captures);
      if (!Init.isUsable()) {
        HasErrors = true;
        break;
}
      ExprResult Update = BuildCounterUpdate(
          SemaRef, CurScope, UpdLoc, CounterVar, IS.CounterInit, Iter,
          IS.CounterStep, IS.Subtract, &Captures);
      if (!Update.isUsable()) {
        HasErrors = true;
        break;
}
      ExprResult Final = BuildCounterUpdate(
          SemaRef, CurScope, UpdLoc, CounterVar, IS.CounterInit,
          IS.NumIterations, IS.CounterStep, IS.Subtract, &Captures);
      if (!Final.isUsable()) {
        HasErrors = true;
        break;
}
      if (Cnt != 0) {
        if (Div.isUnset())
          Div = IS.NumIterations;
        else
          Div = SemaRef.BuildBinOp(CurScope, UpdLoc, BO_Mul, Div.get(),
                                   IS.NumIterations);
        if (Div.isUsable())
          Div = tryBuildCapture(SemaRef, Div.get(), Captures);
        if (!Div.isUsable()) {
          HasErrors = true;
          break;
}
        LoopMultipliers.push_back(Div.get());
}
      if (!Update.isUsable() || !Final.isUsable()) {
        HasErrors = true;
        break;
}
      Built.Counters[Cnt] = IS.CounterVar;
      Built.PrivateCounters[Cnt] = IS.PrivateCounterVar;
      Built.Inits[Cnt] = Init.get();
      Built.Updates[Cnt] = Update.get();
      Built.Finals[Cnt] = Final.get();
}
}
  if (HasErrors)
    return 0;
  Built.IterationVarRef = IV.get();
  Built.LastIteration = LastIteration.get();
  Built.NumIterations = NumIterations.get();
  Built.CalcLastIteration =
      SemaRef.ActOnFinishFullExpr(CalcLastIteration.get()).get();
  Built.PreCond = PreCond.get();
  Built.PreInits = buildPreInits(C, Captures);
  Built.Cond = Cond.get();
  Built.Init = Init.get();
  Built.Inc = Inc.get();
  Built.LB = LB.get();
  Built.UB = UB.get();
  Built.IL = IL.get();
  Built.ST = ST.get();
  Built.EUB = EUB.get();
  Built.NLB = NextLB.get();
  Built.NUB = NextUB.get();
  Built.PrevLB = PrevLB.get();
  Built.PrevUB = PrevUB.get();
  Expr *CounterVal = SemaRef.DefaultLvalueConversion(IV.get()).get();
for_loop_talkad7420();
  for (auto Pair : DSA.getDoacrossDependClauses()) {
    if (Pair.first->getDependencyKind() == OMPC_DEPEND_source)
      Pair.first->setCounterValue(CounterVal);
    else {
      if (NestedLoopCount != Pair.second.size() ||
          NestedLoopCount != LoopMultipliers.size() + 1) {
        Pair.first->setCounterValue(CounterVal);
        continue;
}
      assert(Pair.first->getDependencyKind() == OMPC_DEPEND_sink);
      auto I = Pair.second.rbegin();
      auto IS = IterSpaces.rbegin();
      auto ILM = LoopMultipliers.rbegin();
      Expr *UpCounterVal = CounterVal;
      Expr *Multiplier = nullptr;
for_loop_talkad7420();
      for (int Cnt = NestedLoopCount - 1; Cnt >= 0; --Cnt) {
        if (I->first) {
          assert(IS->CounterStep);
          Expr *NormalizedOffset =
              SemaRef
                  .BuildBinOp(CurScope, I->first->getExprLoc(), BO_Div,
                              I->first, IS->CounterStep)
                  .get();
          if (Multiplier) {
            NormalizedOffset =
                SemaRef
                    .BuildBinOp(CurScope, I->first->getExprLoc(), BO_Mul,
                                NormalizedOffset, Multiplier)
                    .get();
}
          assert(I->second == OO_Plus || I->second == OO_Minus);
          BinaryOperatorKind BOK = (I->second == OO_Plus) ? BO_Add : BO_Sub;
          UpCounterVal = SemaRef
                             .BuildBinOp(CurScope, I->first->getExprLoc(), BOK,
                                         UpCounterVal, NormalizedOffset)
                             .get();
}
        Multiplier = *ILM;
        ++I;
        ++IS;
        ++ILM;
}
      Pair.first->setCounterValue(UpCounterVal);
}
}
  return NestedLoopCount;
}
static Expr *getCollapseNumberExpr(ArrayRef<OMPClause *> Clauses) {
  auto CollapseClauses =
      OMPExecutableDirective::getClausesOfKind<OMPCollapseClause>(Clauses);
  if (CollapseClauses.begin() != CollapseClauses.end())
    return (*CollapseClauses.begin())->getNumForLoops();
  return nullptr;
}
static Expr *getOrderedNumberExpr(ArrayRef<OMPClause *> Clauses) {
  auto OrderedClauses =
      OMPExecutableDirective::getClausesOfKind<OMPOrderedClause>(Clauses);
  if (OrderedClauses.begin() != OrderedClauses.end())
    return (*OrderedClauses.begin())->getNumForLoops();
  return nullptr;
}
static bool checkSimdlenSafelenSpecified(Sema &S,
                                         const ArrayRef<OMPClause *> Clauses) {
  OMPSafelenClause *Safelen = nullptr;
  OMPSimdlenClause *Simdlen = nullptr;
for_loop_talkad7420();
  for (auto *Clause : Clauses) {
    if (Clause->getClauseKind() == OMPC_safelen)
      Safelen = cast<OMPSafelenClause>(Clause);
    else if (Clause->getClauseKind() == OMPC_simdlen)
      Simdlen = cast<OMPSimdlenClause>(Clause);
    if (Safelen && Simdlen)
      break;
}
  if (Simdlen && Safelen) {
    llvm::APSInt SimdlenRes, SafelenRes;
    auto SimdlenLength = Simdlen->getSimdlen();
    auto SafelenLength = Safelen->getSafelen();
    if (SimdlenLength->isValueDependent() || SimdlenLength->isTypeDependent() ||
        SimdlenLength->isInstantiationDependent() ||
        SimdlenLength->containsUnexpandedParameterPack())
      return false;
    if (SafelenLength->isValueDependent() || SafelenLength->isTypeDependent() ||
        SafelenLength->isInstantiationDependent() ||
        SafelenLength->containsUnexpandedParameterPack())
      return false;
    SimdlenLength->EvaluateAsInt(SimdlenRes, S.Context);
    SafelenLength->EvaluateAsInt(SafelenRes, S.Context);
    if (SimdlenRes > SafelenRes) {
      S.Diag(SimdlenLength->getExprLoc(),
             diag::err_omp_wrong_simdlen_safelen_values)
          << SimdlenLength->getSourceRange() << SafelenLength->getSourceRange();
      return true;
}
}
  return false;
}
StmtResult Sema::ActOnOpenMPSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_simd, getCollapseNumberExpr(Clauses), getOrderedNumberExpr(Clauses),
      AStmt, *this, *DSAStack, VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp simd loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPSimdDirective::Create(Context, StartLoc, EndLoc, NestedLoopCount,
                                  Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_for, getCollapseNumberExpr(Clauses), getOrderedNumberExpr(Clauses),
      AStmt, *this, *DSAStack, VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPForDirective::Create(Context, StartLoc, EndLoc, NestedLoopCount,
                                 Clauses, AStmt, B, DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_for_simd, getCollapseNumberExpr(Clauses),
                      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for simd loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPForSimdDirective::Create(Context, StartLoc, EndLoc, NestedLoopCount,
                                     Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPSectionsDirective(ArrayRef<OMPClause *> Clauses,
                                              Stmt *AStmt,
                                              SourceLocation StartLoc,
                                              SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  auto BaseStmt = AStmt;
  while (auto *CS = dyn_cast_or_null<CapturedStmt>(BaseStmt))
    BaseStmt = CS->getCapturedStmt();
  if (auto *C = dyn_cast_or_null<CompoundStmt>(BaseStmt)) {
    auto S = C->children();
    if (S.begin() == S.end())
      return StmtError();
for_loop_talkad7420();
    for (Stmt *SectionStmt : llvm::make_range(std::next(S.begin()), S.end())) {
      if (!SectionStmt || !isa<OMPSectionDirective>(SectionStmt)) {
        if (SectionStmt)
          Diag(SectionStmt->getLocStart(),
               diag::err_omp_sections_substmt_not_section);
        return StmtError();
}
      cast<OMPSectionDirective>(SectionStmt)
          ->setHasCancel(DSAStack->isCancelRegion());
}
} else {
    Diag(AStmt->getLocStart(), diag::err_omp_sections_not_compound_stmt);
    return StmtError();
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPSectionsDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt,
                                      DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPSectionDirective(Stmt *AStmt,
                                             SourceLocation StartLoc,
                                             SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  getCurFunction()->setHasBranchProtectedScope();
  DSAStack->setParentCancelRegion(DSAStack->isCancelRegion());
  return OMPSectionDirective::Create(Context, StartLoc, EndLoc, AStmt,
                                     DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPSingleDirective(ArrayRef<OMPClause *> Clauses,
                                            Stmt *AStmt,
                                            SourceLocation StartLoc,
                                            SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  getCurFunction()->setHasBranchProtectedScope();
  OMPClause *Nowait = nullptr;
  OMPClause *Copyprivate = nullptr;
for_loop_talkad7420();
  for (auto *Clause : Clauses) {
    if (Clause->getClauseKind() == OMPC_nowait)
      Nowait = Clause;
    else if (Clause->getClauseKind() == OMPC_copyprivate)
      Copyprivate = Clause;
    if (Copyprivate && Nowait) {
      Diag(Copyprivate->getLocStart(),
           diag::err_omp_single_copyprivate_with_nowait);
      Diag(Nowait->getLocStart(), diag::note_omp_nowait_clause_here);
      return StmtError();
}
}
  return OMPSingleDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt);
}
StmtResult Sema::ActOnOpenMPMasterDirective(Stmt *AStmt,
                                            SourceLocation StartLoc,
                                            SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPMasterDirective::Create(Context, StartLoc, EndLoc, AStmt);
}
StmtResult Sema::ActOnOpenMPCriticalDirective(
    const DeclarationNameInfo &DirName, ArrayRef<OMPClause *> Clauses,
    Stmt *AStmt, SourceLocation StartLoc, SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  bool ErrorFound = false;
  llvm::APSInt Hint;
  SourceLocation HintLoc;
  bool DependentHint = false;
for_loop_talkad7420();
  for (auto *C : Clauses) {
    if (C->getClauseKind() == OMPC_hint) {
      if (!DirName.getName()) {
        Diag(C->getLocStart(), diag::err_omp_hint_clause_no_name);
        ErrorFound = true;
}
      Expr *E = cast<OMPHintClause>(C)->getHint();
      if (E->isTypeDependent() || E->isValueDependent() ||
          E->isInstantiationDependent())
        DependentHint = true;
      else {
        Hint = E->EvaluateKnownConstInt(Context);
        HintLoc = C->getLocStart();
}
}
}
  if (ErrorFound)
    return StmtError();
  auto Pair = DSAStack->getCriticalWithHint(DirName);
  if (Pair.first && DirName.getName() && !DependentHint) {
    if (llvm::APSInt::compareValues(Hint, Pair.second) != 0) {
      Diag(StartLoc, diag::err_omp_critical_with_hint);
      if (HintLoc.isValid()) {
        Diag(HintLoc, diag::note_omp_critical_hint_here)
            << 0 << Hint.toString(
10, 
false);
} else
        Diag(StartLoc, diag::note_omp_critical_no_hint) << 0;
      if (auto *C = Pair.first->getSingleClause<OMPHintClause>()) {
        Diag(C->getLocStart(), diag::note_omp_critical_hint_here)
            << 1
            << C->getHint()->EvaluateKnownConstInt(Context).toString(
10, 
false);
} else
        Diag(Pair.first->getLocStart(), diag::note_omp_critical_no_hint) << 1;
}
}
  getCurFunction()->setHasBranchProtectedScope();
  auto *Dir = OMPCriticalDirective::Create(Context, DirName, StartLoc, EndLoc,
                                           Clauses, AStmt);
  if (!Pair.first && DirName.getName() && !DependentHint)
    DSAStack->addCriticalWithHint(Dir, Hint);
  return Dir;
}
StmtResult Sema::ActOnOpenMPParallelForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_parallel_for, getCollapseNumberExpr(Clauses),
                      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp parallel for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPParallelForDirective::Create(Context, StartLoc, EndLoc,
                                         NestedLoopCount, Clauses, AStmt, B,
                                         DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPParallelForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_parallel_for_simd, getCollapseNumberExpr(Clauses),
                      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPParallelForSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult
Sema::ActOnOpenMPParallelSectionsDirective(ArrayRef<OMPClause *> Clauses,
                                           Stmt *AStmt, SourceLocation StartLoc,
                                           SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  auto BaseStmt = AStmt;
  while (auto *CS = dyn_cast_or_null<CapturedStmt>(BaseStmt))
    BaseStmt = CS->getCapturedStmt();
  if (auto *C = dyn_cast_or_null<CompoundStmt>(BaseStmt)) {
    auto S = C->children();
    if (S.begin() == S.end())
      return StmtError();
for_loop_talkad7420();
    for (Stmt *SectionStmt : llvm::make_range(std::next(S.begin()), S.end())) {
      if (!SectionStmt || !isa<OMPSectionDirective>(SectionStmt)) {
        if (SectionStmt)
          Diag(SectionStmt->getLocStart(),
               diag::err_omp_parallel_sections_substmt_not_section);
        return StmtError();
}
      cast<OMPSectionDirective>(SectionStmt)
          ->setHasCancel(DSAStack->isCancelRegion());
}
} else {
    Diag(AStmt->getLocStart(),
         diag::err_omp_parallel_sections_not_compound_stmt);
    return StmtError();
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPParallelSectionsDirective::Create(
      Context, StartLoc, EndLoc, Clauses, AStmt, DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPTaskDirective(ArrayRef<OMPClause *> Clauses,
                                          Stmt *AStmt, SourceLocation StartLoc,
                                          SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  auto *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTaskDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt,
                                  DSAStack->isCancelRegion());
}
StmtResult Sema::ActOnOpenMPTaskyieldDirective(SourceLocation StartLoc,
                                               SourceLocation EndLoc) {
  return OMPTaskyieldDirective::Create(Context, StartLoc, EndLoc);
}
StmtResult Sema::ActOnOpenMPBarrierDirective(SourceLocation StartLoc,
                                             SourceLocation EndLoc) {
  return OMPBarrierDirective::Create(Context, StartLoc, EndLoc);
}
StmtResult Sema::ActOnOpenMPTaskwaitDirective(SourceLocation StartLoc,
                                              SourceLocation EndLoc) {
  return OMPTaskwaitDirective::Create(Context, StartLoc, EndLoc);
}
StmtResult Sema::ActOnOpenMPTaskgroupDirective(Stmt *AStmt,
                                               SourceLocation StartLoc,
                                               SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTaskgroupDirective::Create(Context, StartLoc, EndLoc, AStmt);
}
StmtResult Sema::ActOnOpenMPFlushDirective(ArrayRef<OMPClause *> Clauses,
                                           SourceLocation StartLoc,
                                           SourceLocation EndLoc) {
  assert(Clauses.size() <= 1 && "Extra clauses in flush directive");
  return OMPFlushDirective::Create(Context, StartLoc, EndLoc, Clauses);
}
StmtResult Sema::ActOnOpenMPIncDirective(ArrayRef<OMPClause *> Clauses,
                                           SourceLocation StartLoc,
                                           SourceLocation EndLoc) {
  assert(Clauses.size() <= 1 && "Extra clauses in inc directive");
  return OMPIncDirective::Create(Context, StartLoc, EndLoc, Clauses);
}
StmtResult Sema::ActOnOpenMPOrderedDirective(ArrayRef<OMPClause *> Clauses,
                                             Stmt *AStmt,
                                             SourceLocation StartLoc,
                                             SourceLocation EndLoc) {
  OMPClause *DependFound = nullptr;
  OMPClause *DependSourceClause = nullptr;
  OMPClause *DependSinkClause = nullptr;
  bool ErrorFound = false;
  OMPThreadsClause *TC = nullptr;
  OMPSIMDClause *SC = nullptr;
for_loop_talkad7420();
  for (auto *C : Clauses) {
    if (auto *DC = dyn_cast<OMPDependClause>(C)) {
      DependFound = C;
      if (DC->getDependencyKind() == OMPC_DEPEND_source) {
        if (DependSourceClause) {
          Diag(C->getLocStart(), diag::err_omp_more_one_clause)
              << getOpenMPDirectiveName(OMPD_ordered)
              << getOpenMPClauseName(OMPC_depend) << 2;
          ErrorFound = true;
} else
          DependSourceClause = C;
        if (DependSinkClause) {
          Diag(C->getLocStart(), diag::err_omp_depend_sink_source_not_allowed)
              << 0;
          ErrorFound = true;
}
} else if (DC->getDependencyKind() == OMPC_DEPEND_sink) {
        if (DependSourceClause) {
          Diag(C->getLocStart(), diag::err_omp_depend_sink_source_not_allowed)
              << 1;
          ErrorFound = true;
}
        DependSinkClause = C;
}
} else if (C->getClauseKind() == OMPC_threads)
      TC = cast<OMPThreadsClause>(C);
    else if (C->getClauseKind() == OMPC_simd)
      SC = cast<OMPSIMDClause>(C);
}
  if (!ErrorFound && !SC &&
      isOpenMPSimdDirective(DSAStack->getParentDirective())) {
    Diag(StartLoc, diag::err_omp_prohibited_region_simd);
    ErrorFound = true;
} else if (DependFound && (TC || SC)) {
    Diag(DependFound->getLocStart(), diag::err_omp_depend_clause_thread_simd)
        << getOpenMPClauseName(TC ? TC->getClauseKind() : SC->getClauseKind());
    ErrorFound = true;
} else if (DependFound && !DSAStack->getParentOrderedRegionParam()) {
    Diag(DependFound->getLocStart(),
         diag::err_omp_ordered_directive_without_param);
    ErrorFound = true;
} else if (TC || Clauses.empty()) {
    if (auto *Param = DSAStack->getParentOrderedRegionParam()) {
      SourceLocation ErrLoc = TC ? TC->getLocStart() : StartLoc;
      Diag(ErrLoc, diag::err_omp_ordered_directive_with_param)
          << (TC != nullptr);
      Diag(Param->getLocStart(), diag::note_omp_ordered_param);
      ErrorFound = true;
}
}
  if ((!AStmt && !DependFound) || ErrorFound)
    return StmtError();
  if (AStmt) {
    assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
    getCurFunction()->setHasBranchProtectedScope();
}
  return OMPOrderedDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt);
}
namespace {
class OpenMPAtomicUpdateChecker {
  enum ExprAnalysisErrorCode {
    NotAnExpression,
    NotABinaryOrUnaryExpression,
    NotAnUnaryIncDecExpression,
    NotAScalarType,
    NotAnAssignmentOp,
    NotABinaryExpression,
    NotABinaryOperator,
    NotAnUpdateExpression,
    NoError
};
  Sema &SemaRef;
  SourceLocation NoteLoc;
  Expr *X;
  Expr *E;
  Expr *UpdateExpr;
  bool IsXLHSInRHSPart;
  BinaryOperatorKind Op;
  SourceLocation OpLoc;
  bool IsPostfixUpdate;
public:
  OpenMPAtomicUpdateChecker(Sema &SemaRef)
      : SemaRef(SemaRef), X(nullptr), E(nullptr), UpdateExpr(nullptr),
        IsXLHSInRHSPart(false), Op(BO_PtrMemD), IsPostfixUpdate(false) {
}
  bool checkStatement(Stmt *S, unsigned DiagId = 0, unsigned NoteId = 0);
  Expr *getX() const { return X; 
}
  Expr *getExpr() const { return E; 
}
  Expr *getUpdateExpr() const { return UpdateExpr; 
}
  bool isXLHSInRHSPart() const { return IsXLHSInRHSPart; 
}
  bool isPostfixUpdate() const { return IsPostfixUpdate; 
}
private:
  bool checkBinaryOperation(BinaryOperator *AtomicBinOp, unsigned DiagId = 0,
                            unsigned NoteId = 0);
};
} 
bool OpenMPAtomicUpdateChecker::checkBinaryOperation(
    BinaryOperator *AtomicBinOp, unsigned DiagId, unsigned NoteId) {
  ExprAnalysisErrorCode ErrorFound = NoError;
  SourceLocation ErrorLoc, NoteLoc;
  SourceRange ErrorRange, NoteRange;
  if (AtomicBinOp->getOpcode() == BO_Assign) {
    X = AtomicBinOp->getLHS();
    if (auto *AtomicInnerBinOp = dyn_cast<BinaryOperator>(
            AtomicBinOp->getRHS()->IgnoreParenImpCasts())) {
      if (AtomicInnerBinOp->isMultiplicativeOp() ||
          AtomicInnerBinOp->isAdditiveOp() || AtomicInnerBinOp->isShiftOp() ||
          AtomicInnerBinOp->isBitwiseOp()) {
        Op = AtomicInnerBinOp->getOpcode();
        OpLoc = AtomicInnerBinOp->getOperatorLoc();
        auto *LHS = AtomicInnerBinOp->getLHS();
        auto *RHS = AtomicInnerBinOp->getRHS();
        llvm::FoldingSetNodeID XId, LHSId, RHSId;
        X->IgnoreParenImpCasts()->Profile(XId, SemaRef.getASTContext(),
true);
        LHS->IgnoreParenImpCasts()->Profile(LHSId, SemaRef.getASTContext(),
true);
        RHS->IgnoreParenImpCasts()->Profile(RHSId, SemaRef.getASTContext(),
true);
        if (XId == LHSId) {
          E = RHS;
          IsXLHSInRHSPart = true;
} else if (XId == RHSId) {
          E = LHS;
          IsXLHSInRHSPart = false;
} else {
          ErrorLoc = AtomicInnerBinOp->getExprLoc();
          ErrorRange = AtomicInnerBinOp->getSourceRange();
          NoteLoc = X->getExprLoc();
          NoteRange = X->getSourceRange();
          ErrorFound = NotAnUpdateExpression;
}
} else {
        ErrorLoc = AtomicInnerBinOp->getExprLoc();
        ErrorRange = AtomicInnerBinOp->getSourceRange();
        NoteLoc = AtomicInnerBinOp->getOperatorLoc();
        NoteRange = SourceRange(NoteLoc, NoteLoc);
        ErrorFound = NotABinaryOperator;
}
} else {
      NoteLoc = ErrorLoc = AtomicBinOp->getRHS()->getExprLoc();
      NoteRange = ErrorRange = AtomicBinOp->getRHS()->getSourceRange();
      ErrorFound = NotABinaryExpression;
}
} else {
    ErrorLoc = AtomicBinOp->getExprLoc();
    ErrorRange = AtomicBinOp->getSourceRange();
    NoteLoc = AtomicBinOp->getOperatorLoc();
    NoteRange = SourceRange(NoteLoc, NoteLoc);
    ErrorFound = NotAnAssignmentOp;
}
  if (ErrorFound != NoError && DiagId != 0 && NoteId != 0) {
    SemaRef.Diag(ErrorLoc, DiagId) << ErrorRange;
    SemaRef.Diag(NoteLoc, NoteId) << ErrorFound << NoteRange;
    return true;
} else if (SemaRef.CurContext->isDependentContext())
    E = X = UpdateExpr = nullptr;
  return ErrorFound != NoError;
}
bool OpenMPAtomicUpdateChecker::checkStatement(Stmt *S, unsigned DiagId,
                                               unsigned NoteId) {
  ExprAnalysisErrorCode ErrorFound = NoError;
  SourceLocation ErrorLoc, NoteLoc;
  SourceRange ErrorRange, NoteRange;
  if (auto *AtomicBody = dyn_cast<Expr>(S)) {
    AtomicBody = AtomicBody->IgnoreParenImpCasts();
    if (AtomicBody->getType()->isScalarType() ||
        AtomicBody->isInstantiationDependent()) {
      if (auto *AtomicCompAssignOp = dyn_cast<CompoundAssignOperator>(
              AtomicBody->IgnoreParenImpCasts())) {
        Op = BinaryOperator::getOpForCompoundAssignment(
            AtomicCompAssignOp->getOpcode());
        OpLoc = AtomicCompAssignOp->getOperatorLoc();
        E = AtomicCompAssignOp->getRHS();
        X = AtomicCompAssignOp->getLHS()->IgnoreParens();
        IsXLHSInRHSPart = true;
} else if (auto *AtomicBinOp = dyn_cast<BinaryOperator>(
                     AtomicBody->IgnoreParenImpCasts())) {
        if (checkBinaryOperation(AtomicBinOp, DiagId, NoteId))
          return true;
} else if (auto *AtomicUnaryOp = dyn_cast<UnaryOperator>(
                     AtomicBody->IgnoreParenImpCasts())) {
        if (AtomicUnaryOp->isIncrementDecrementOp()) {
          IsPostfixUpdate = AtomicUnaryOp->isPostfix();
          Op = AtomicUnaryOp->isIncrementOp() ? BO_Add : BO_Sub;
          OpLoc = AtomicUnaryOp->getOperatorLoc();
          X = AtomicUnaryOp->getSubExpr()->IgnoreParens();
          E = SemaRef.ActOnIntegerConstant(OpLoc, 
1).get();
          IsXLHSInRHSPart = true;
} else {
          ErrorFound = NotAnUnaryIncDecExpression;
          ErrorLoc = AtomicUnaryOp->getExprLoc();
          ErrorRange = AtomicUnaryOp->getSourceRange();
          NoteLoc = AtomicUnaryOp->getOperatorLoc();
          NoteRange = SourceRange(NoteLoc, NoteLoc);
}
} else if (!AtomicBody->isInstantiationDependent()) {
        ErrorFound = NotABinaryOrUnaryExpression;
        NoteLoc = ErrorLoc = AtomicBody->getExprLoc();
        NoteRange = ErrorRange = AtomicBody->getSourceRange();
}
} else {
      ErrorFound = NotAScalarType;
      NoteLoc = ErrorLoc = AtomicBody->getLocStart();
      NoteRange = ErrorRange = SourceRange(NoteLoc, NoteLoc);
}
} else {
    ErrorFound = NotAnExpression;
    NoteLoc = ErrorLoc = S->getLocStart();
    NoteRange = ErrorRange = SourceRange(NoteLoc, NoteLoc);
}
  if (ErrorFound != NoError && DiagId != 0 && NoteId != 0) {
    SemaRef.Diag(ErrorLoc, DiagId) << ErrorRange;
    SemaRef.Diag(NoteLoc, NoteId) << ErrorFound << NoteRange;
    return true;
} else if (SemaRef.CurContext->isDependentContext())
    E = X = UpdateExpr = nullptr;
  if (ErrorFound == NoError && E && X) {
    auto *OVEX = new (SemaRef.getASTContext())
        OpaqueValueExpr(X->getExprLoc(), X->getType(), VK_RValue);
    auto *OVEExpr = new (SemaRef.getASTContext())
        OpaqueValueExpr(E->getExprLoc(), E->getType(), VK_RValue);
    auto Update =
        SemaRef.CreateBuiltinBinOp(OpLoc, Op, IsXLHSInRHSPart ? OVEX : OVEExpr,
                                   IsXLHSInRHSPart ? OVEExpr : OVEX);
    if (Update.isInvalid())
      return true;
    Update = SemaRef.PerformImplicitConversion(Update.get(), X->getType(),
                                               Sema::AA_Casting);
    if (Update.isInvalid())
      return true;
    UpdateExpr = Update.get();
}
  return ErrorFound != NoError;
}
StmtResult Sema::ActOnOpenMPAtomicDirective(ArrayRef<OMPClause *> Clauses,
                                            Stmt *AStmt,
                                            SourceLocation StartLoc,
                                            SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  auto *CS = cast<CapturedStmt>(AStmt);
  OpenMPClauseKind AtomicKind = OMPC_unknown;
  SourceLocation AtomicKindLoc;
for_loop_talkad7420();
  for (auto *C : Clauses) {
    if (C->getClauseKind() == OMPC_read || C->getClauseKind() == OMPC_write ||
        C->getClauseKind() == OMPC_update ||
        C->getClauseKind() == OMPC_capture) {
      if (AtomicKind != OMPC_unknown) {
        Diag(C->getLocStart(), diag::err_omp_atomic_several_clauses)
            << SourceRange(C->getLocStart(), C->getLocEnd());
        Diag(AtomicKindLoc, diag::note_omp_atomic_previous_clause)
            << getOpenMPClauseName(AtomicKind);
} else {
        AtomicKind = C->getClauseKind();
        AtomicKindLoc = C->getLocStart();
}
}
}
  auto Body = CS->getCapturedStmt();
  if (auto *EWC = dyn_cast<ExprWithCleanups>(Body))
    Body = EWC->getSubExpr();
  Expr *X = nullptr;
  Expr *V = nullptr;
  Expr *E = nullptr;
  Expr *UE = nullptr;
  bool IsXLHSInRHSPart = false;
  bool IsPostfixUpdate = false;
  if (AtomicKind == OMPC_read) {
    enum {
      NotAnExpression,
      NotAnAssignmentOp,
      NotAScalarType,
      NotAnLValue,
      NoError
} ErrorFound = NoError;
    SourceLocation ErrorLoc, NoteLoc;
    SourceRange ErrorRange, NoteRange;
    if (auto *AtomicBody = dyn_cast<Expr>(Body)) {
      auto *AtomicBinOp =
          dyn_cast<BinaryOperator>(AtomicBody->IgnoreParenImpCasts());
      if (AtomicBinOp && AtomicBinOp->getOpcode() == BO_Assign) {
        X = AtomicBinOp->getRHS()->IgnoreParenImpCasts();
        V = AtomicBinOp->getLHS()->IgnoreParenImpCasts();
        if ((X->isInstantiationDependent() || X->getType()->isScalarType()) &&
            (V->isInstantiationDependent() || V->getType()->isScalarType())) {
          if (!X->isLValue() || !V->isLValue()) {
            auto NotLValueExpr = X->isLValue() ? V : X;
            ErrorFound = NotAnLValue;
            ErrorLoc = AtomicBinOp->getExprLoc();
            ErrorRange = AtomicBinOp->getSourceRange();
            NoteLoc = NotLValueExpr->getExprLoc();
            NoteRange = NotLValueExpr->getSourceRange();
}
} else if (!X->isInstantiationDependent() ||
                   !V->isInstantiationDependent()) {
          auto NotScalarExpr =
              (X->isInstantiationDependent() || X->getType()->isScalarType())
                  ? V
                  : X;
          ErrorFound = NotAScalarType;
          ErrorLoc = AtomicBinOp->getExprLoc();
          ErrorRange = AtomicBinOp->getSourceRange();
          NoteLoc = NotScalarExpr->getExprLoc();
          NoteRange = NotScalarExpr->getSourceRange();
}
} else if (!AtomicBody->isInstantiationDependent()) {
        ErrorFound = NotAnAssignmentOp;
        ErrorLoc = AtomicBody->getExprLoc();
        ErrorRange = AtomicBody->getSourceRange();
        NoteLoc = AtomicBinOp ? AtomicBinOp->getOperatorLoc()
                              : AtomicBody->getExprLoc();
        NoteRange = AtomicBinOp ? AtomicBinOp->getSourceRange()
                                : AtomicBody->getSourceRange();
}
} else {
      ErrorFound = NotAnExpression;
      NoteLoc = ErrorLoc = Body->getLocStart();
      NoteRange = ErrorRange = SourceRange(NoteLoc, NoteLoc);
}
    if (ErrorFound != NoError) {
      Diag(ErrorLoc, diag::err_omp_atomic_read_not_expression_statement)
          << ErrorRange;
      Diag(NoteLoc, diag::note_omp_atomic_read_write) << ErrorFound
                                                      << NoteRange;
      return StmtError();
} else if (CurContext->isDependentContext())
      V = X = nullptr;
} else if (AtomicKind == OMPC_write) {
    enum {
      NotAnExpression,
      NotAnAssignmentOp,
      NotAScalarType,
      NotAnLValue,
      NoError
} ErrorFound = NoError;
    SourceLocation ErrorLoc, NoteLoc;
    SourceRange ErrorRange, NoteRange;
    if (auto *AtomicBody = dyn_cast<Expr>(Body)) {
      auto *AtomicBinOp =
          dyn_cast<BinaryOperator>(AtomicBody->IgnoreParenImpCasts());
      if (AtomicBinOp && AtomicBinOp->getOpcode() == BO_Assign) {
        X = AtomicBinOp->getLHS();
        E = AtomicBinOp->getRHS();
        if ((X->isInstantiationDependent() || X->getType()->isScalarType()) &&
            (E->isInstantiationDependent() || E->getType()->isScalarType())) {
          if (!X->isLValue()) {
            ErrorFound = NotAnLValue;
            ErrorLoc = AtomicBinOp->getExprLoc();
            ErrorRange = AtomicBinOp->getSourceRange();
            NoteLoc = X->getExprLoc();
            NoteRange = X->getSourceRange();
}
} else if (!X->isInstantiationDependent() ||
                   !E->isInstantiationDependent()) {
          auto NotScalarExpr =
              (X->isInstantiationDependent() || X->getType()->isScalarType())
                  ? E
                  : X;
          ErrorFound = NotAScalarType;
          ErrorLoc = AtomicBinOp->getExprLoc();
          ErrorRange = AtomicBinOp->getSourceRange();
          NoteLoc = NotScalarExpr->getExprLoc();
          NoteRange = NotScalarExpr->getSourceRange();
}
} else if (!AtomicBody->isInstantiationDependent()) {
        ErrorFound = NotAnAssignmentOp;
        ErrorLoc = AtomicBody->getExprLoc();
        ErrorRange = AtomicBody->getSourceRange();
        NoteLoc = AtomicBinOp ? AtomicBinOp->getOperatorLoc()
                              : AtomicBody->getExprLoc();
        NoteRange = AtomicBinOp ? AtomicBinOp->getSourceRange()
                                : AtomicBody->getSourceRange();
}
} else {
      ErrorFound = NotAnExpression;
      NoteLoc = ErrorLoc = Body->getLocStart();
      NoteRange = ErrorRange = SourceRange(NoteLoc, NoteLoc);
}
    if (ErrorFound != NoError) {
      Diag(ErrorLoc, diag::err_omp_atomic_write_not_expression_statement)
          << ErrorRange;
      Diag(NoteLoc, diag::note_omp_atomic_read_write) << ErrorFound
                                                      << NoteRange;
      return StmtError();
} else if (CurContext->isDependentContext())
      E = X = nullptr;
} else if (AtomicKind == OMPC_update || AtomicKind == OMPC_unknown) {
    OpenMPAtomicUpdateChecker Checker(*this);
    if (Checker.checkStatement(
            Body, (AtomicKind == OMPC_update)
                      ? diag::err_omp_atomic_update_not_expression_statement
                      : diag::err_omp_atomic_not_expression_statement,
            diag::note_omp_atomic_update))
      return StmtError();
    if (!CurContext->isDependentContext()) {
      E = Checker.getExpr();
      X = Checker.getX();
      UE = Checker.getUpdateExpr();
      IsXLHSInRHSPart = Checker.isXLHSInRHSPart();
}
} else if (AtomicKind == OMPC_capture) {
    enum {
      NotAnAssignmentOp,
      NotACompoundStatement,
      NotTwoSubstatements,
      NotASpecificExpression,
      NoError
} ErrorFound = NoError;
    SourceLocation ErrorLoc, NoteLoc;
    SourceRange ErrorRange, NoteRange;
    if (auto *AtomicBody = dyn_cast<Expr>(Body)) {
      auto *AtomicBinOp =
          dyn_cast<BinaryOperator>(AtomicBody->IgnoreParenImpCasts());
      if (AtomicBinOp && AtomicBinOp->getOpcode() == BO_Assign) {
        V = AtomicBinOp->getLHS();
        Body = AtomicBinOp->getRHS()->IgnoreParenImpCasts();
        OpenMPAtomicUpdateChecker Checker(*this);
        if (Checker.checkStatement(
                Body, diag::err_omp_atomic_capture_not_expression_statement,
                diag::note_omp_atomic_update))
          return StmtError();
        E = Checker.getExpr();
        X = Checker.getX();
        UE = Checker.getUpdateExpr();
        IsXLHSInRHSPart = Checker.isXLHSInRHSPart();
        IsPostfixUpdate = Checker.isPostfixUpdate();
} else if (!AtomicBody->isInstantiationDependent()) {
        ErrorLoc = AtomicBody->getExprLoc();
        ErrorRange = AtomicBody->getSourceRange();
        NoteLoc = AtomicBinOp ? AtomicBinOp->getOperatorLoc()
                              : AtomicBody->getExprLoc();
        NoteRange = AtomicBinOp ? AtomicBinOp->getSourceRange()
                                : AtomicBody->getSourceRange();
        ErrorFound = NotAnAssignmentOp;
}
      if (ErrorFound != NoError) {
        Diag(ErrorLoc, diag::err_omp_atomic_capture_not_expression_statement)
            << ErrorRange;
        Diag(NoteLoc, diag::note_omp_atomic_capture) << ErrorFound << NoteRange;
        return StmtError();
} else if (CurContext->isDependentContext()) {
        UE = V = E = X = nullptr;
}
} else {
      if (auto *CS = dyn_cast<CompoundStmt>(Body)) {
        if (CS->size() == 2) {
          auto *First = CS->body_front();
          auto *Second = CS->body_back();
          if (auto *EWC = dyn_cast<ExprWithCleanups>(First))
            First = EWC->getSubExpr()->IgnoreParenImpCasts();
          if (auto *EWC = dyn_cast<ExprWithCleanups>(Second))
            Second = EWC->getSubExpr()->IgnoreParenImpCasts();
          OpenMPAtomicUpdateChecker Checker(*this);
          bool IsUpdateExprFound = !Checker.checkStatement(Second);
          BinaryOperator *BinOp = nullptr;
          if (IsUpdateExprFound) {
            BinOp = dyn_cast<BinaryOperator>(First);
            IsUpdateExprFound = BinOp && BinOp->getOpcode() == BO_Assign;
}
          if (IsUpdateExprFound && !CurContext->isDependentContext()) {
            auto *PossibleX = BinOp->getRHS()->IgnoreParenImpCasts();
            llvm::FoldingSetNodeID XId, PossibleXId;
            Checker.getX()->Profile(XId, Context, 
true);
            PossibleX->Profile(PossibleXId, Context, 
true);
            IsUpdateExprFound = XId == PossibleXId;
            if (IsUpdateExprFound) {
              V = BinOp->getLHS();
              X = Checker.getX();
              E = Checker.getExpr();
              UE = Checker.getUpdateExpr();
              IsXLHSInRHSPart = Checker.isXLHSInRHSPart();
              IsPostfixUpdate = true;
}
}
          if (!IsUpdateExprFound) {
            IsUpdateExprFound = !Checker.checkStatement(First);
            BinOp = nullptr;
            if (IsUpdateExprFound) {
              BinOp = dyn_cast<BinaryOperator>(Second);
              IsUpdateExprFound = BinOp && BinOp->getOpcode() == BO_Assign;
}
            if (IsUpdateExprFound && !CurContext->isDependentContext()) {
              auto *PossibleX = BinOp->getRHS()->IgnoreParenImpCasts();
              llvm::FoldingSetNodeID XId, PossibleXId;
              Checker.getX()->Profile(XId, Context, 
true);
              PossibleX->Profile(PossibleXId, Context, 
true);
              IsUpdateExprFound = XId == PossibleXId;
              if (IsUpdateExprFound) {
                V = BinOp->getLHS();
                X = Checker.getX();
                E = Checker.getExpr();
                UE = Checker.getUpdateExpr();
                IsXLHSInRHSPart = Checker.isXLHSInRHSPart();
                IsPostfixUpdate = false;
}
}
}
          if (!IsUpdateExprFound) {
            auto *FirstExpr = dyn_cast<Expr>(First);
            auto *SecondExpr = dyn_cast<Expr>(Second);
            if (!FirstExpr || !SecondExpr ||
                !(FirstExpr->isInstantiationDependent() ||
                  SecondExpr->isInstantiationDependent())) {
              auto *FirstBinOp = dyn_cast<BinaryOperator>(First);
              if (!FirstBinOp || FirstBinOp->getOpcode() != BO_Assign) {
                ErrorFound = NotAnAssignmentOp;
                NoteLoc = ErrorLoc = FirstBinOp ? FirstBinOp->getOperatorLoc()
                                                : First->getLocStart();
                NoteRange = ErrorRange = FirstBinOp
                                             ? FirstBinOp->getSourceRange()
                                             : SourceRange(ErrorLoc, ErrorLoc);
} else {
                auto *SecondBinOp = dyn_cast<BinaryOperator>(Second);
                if (!SecondBinOp || SecondBinOp->getOpcode() != BO_Assign) {
                  ErrorFound = NotAnAssignmentOp;
                  NoteLoc = ErrorLoc = SecondBinOp
                                           ? SecondBinOp->getOperatorLoc()
                                           : Second->getLocStart();
                  NoteRange = ErrorRange =
                      SecondBinOp ? SecondBinOp->getSourceRange()
                                  : SourceRange(ErrorLoc, ErrorLoc);
} else {
                  auto *PossibleXRHSInFirst =
                      FirstBinOp->getRHS()->IgnoreParenImpCasts();
                  auto *PossibleXLHSInSecond =
                      SecondBinOp->getLHS()->IgnoreParenImpCasts();
                  llvm::FoldingSetNodeID X1Id, X2Id;
                  PossibleXRHSInFirst->Profile(X1Id, Context,
true);
                  PossibleXLHSInSecond->Profile(X2Id, Context,
true);
                  IsUpdateExprFound = X1Id == X2Id;
                  if (IsUpdateExprFound) {
                    V = FirstBinOp->getLHS();
                    X = SecondBinOp->getLHS();
                    E = SecondBinOp->getRHS();
                    UE = nullptr;
                    IsXLHSInRHSPart = false;
                    IsPostfixUpdate = true;
} else {
                    ErrorFound = NotASpecificExpression;
                    ErrorLoc = FirstBinOp->getExprLoc();
                    ErrorRange = FirstBinOp->getSourceRange();
                    NoteLoc = SecondBinOp->getLHS()->getExprLoc();
                    NoteRange = SecondBinOp->getRHS()->getSourceRange();
}
}
}
}
}
} else {
          NoteLoc = ErrorLoc = Body->getLocStart();
          NoteRange = ErrorRange =
              SourceRange(Body->getLocStart(), Body->getLocStart());
          ErrorFound = NotTwoSubstatements;
}
} else {
        NoteLoc = ErrorLoc = Body->getLocStart();
        NoteRange = ErrorRange =
            SourceRange(Body->getLocStart(), Body->getLocStart());
        ErrorFound = NotACompoundStatement;
}
      if (ErrorFound != NoError) {
        Diag(ErrorLoc, diag::err_omp_atomic_capture_not_compound_statement)
            << ErrorRange;
        Diag(NoteLoc, diag::note_omp_atomic_capture) << ErrorFound << NoteRange;
        return StmtError();
} else if (CurContext->isDependentContext()) {
        UE = V = E = X = nullptr;
}
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPAtomicDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt,
                                    X, V, E, UE, IsXLHSInRHSPart,
                                    IsPostfixUpdate);
}
StmtResult Sema::ActOnOpenMPTargetDirective(ArrayRef<OMPClause *> Clauses,
                                            Stmt *AStmt,
                                            SourceLocation StartLoc,
                                            SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  if (DSAStack->hasInnerTeamsRegion()) {
    auto S = AStmt->IgnoreContainers(
 true);
    bool OMPTeamsFound = true;
    if (auto *CS = dyn_cast<CompoundStmt>(S)) {
      auto I = CS->body_begin();
      while (I != CS->body_end()) {
        auto *OED = dyn_cast<OMPExecutableDirective>(*I);
        if (!OED || !isOpenMPTeamsDirective(OED->getDirectiveKind())) {
          OMPTeamsFound = false;
          break;
}
        ++I;
}
      assert(I != CS->body_end() && "Not found statement");
      S = *I;
} else {
      auto *OED = dyn_cast<OMPExecutableDirective>(S);
      OMPTeamsFound = OED && isOpenMPTeamsDirective(OED->getDirectiveKind());
}
    if (!OMPTeamsFound) {
      Diag(StartLoc, diag::err_omp_target_contains_not_only_teams);
      Diag(DSAStack->getInnerTeamsRegionLoc(),
           diag::note_omp_nested_teams_construct_here);
      Diag(S->getLocStart(), diag::note_omp_nested_statement_here)
          << isa<OMPExecutableDirective>(S);
      return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt);
}
StmtResult
Sema::ActOnOpenMPTargetParallelDirective(ArrayRef<OMPClause *> Clauses,
                                         Stmt *AStmt, SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetParallelDirective::Create(Context, StartLoc, EndLoc, Clauses,
                                            AStmt);
}
StmtResult Sema::ActOnOpenMPTargetParallelForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_target_parallel_for, getCollapseNumberExpr(Clauses),
                      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target parallel for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetParallelForDirective::Create(Context, StartLoc, EndLoc,
                                               NestedLoopCount, Clauses, AStmt,
                                               B, DSAStack->isCancelRegion());
}
static bool HasMapClause(ArrayRef<OMPClause *> Clauses) {
for_loop_talkad7420();
  for (ArrayRef<OMPClause *>::iterator I = Clauses.begin(), E = Clauses.end();
{
}
       I != E; ++I) {
    if (*I != nullptr && (*I)->getClauseKind() == OMPC_map) {
      return true;
}
}
  return false;
}
StmtResult Sema::ActOnOpenMPTargetDataDirective(ArrayRef<OMPClause *> Clauses,
                                                Stmt *AStmt,
                                                SourceLocation StartLoc,
                                                SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  if (!HasMapClause(Clauses)) {
    Diag(StartLoc, diag::err_omp_no_map_for_directive)
        << getOpenMPDirectiveName(OMPD_target_data);
    return StmtError();
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetDataDirective::Create(Context, StartLoc, EndLoc, Clauses,
                                        AStmt);
}
StmtResult
Sema::ActOnOpenMPTargetEnterDataDirective(ArrayRef<OMPClause *> Clauses,
                                          SourceLocation StartLoc,
                                          SourceLocation EndLoc) {
  if (!HasMapClause(Clauses)) {
    Diag(StartLoc, diag::err_omp_no_map_for_directive)
        << getOpenMPDirectiveName(OMPD_target_enter_data);
    return StmtError();
}
  return OMPTargetEnterDataDirective::Create(Context, StartLoc, EndLoc,
                                             Clauses);
}
StmtResult
Sema::ActOnOpenMPTargetExitDataDirective(ArrayRef<OMPClause *> Clauses,
                                         SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  if (!HasMapClause(Clauses)) {
    Diag(StartLoc, diag::err_omp_no_map_for_directive)
        << getOpenMPDirectiveName(OMPD_target_exit_data);
    return StmtError();
}
  return OMPTargetExitDataDirective::Create(Context, StartLoc, EndLoc, Clauses);
}
StmtResult Sema::ActOnOpenMPTargetUpdateDirective(ArrayRef<OMPClause *> Clauses,
                                                  SourceLocation StartLoc,
                                                  SourceLocation EndLoc) {
  bool seenMotionClause = false;
for_loop_talkad7420();
  for (auto *C : Clauses) {
    if (C->getClauseKind() == OMPC_to || C->getClauseKind() == OMPC_from)
      seenMotionClause = true;
}
  if (!seenMotionClause) {
    Diag(StartLoc, diag::err_omp_at_least_one_motion_clause_required);
    return StmtError();
}
  return OMPTargetUpdateDirective::Create(Context, StartLoc, EndLoc, Clauses);
}
StmtResult Sema::ActOnOpenMPTeamsDirective(ArrayRef<OMPClause *> Clauses,
                                           Stmt *AStmt, SourceLocation StartLoc,
                                           SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTeamsDirective::Create(Context, StartLoc, EndLoc, Clauses, AStmt);
}
StmtResult
Sema::ActOnOpenMPCancellationPointDirective(SourceLocation StartLoc,
                                            SourceLocation EndLoc,
                                            OpenMPDirectiveKind CancelRegion) {
  if (CancelRegion != OMPD_parallel && CancelRegion != OMPD_for &&
      CancelRegion != OMPD_sections && CancelRegion != OMPD_taskgroup) {
    Diag(StartLoc, diag::err_omp_wrong_cancel_region)
        << getOpenMPDirectiveName(CancelRegion);
    return StmtError();
}
  if (DSAStack->isParentNowaitRegion()) {
    Diag(StartLoc, diag::err_omp_parent_cancel_region_nowait) << 0;
    return StmtError();
}
  if (DSAStack->isParentOrderedRegion()) {
    Diag(StartLoc, diag::err_omp_parent_cancel_region_ordered) << 0;
    return StmtError();
}
  return OMPCancellationPointDirective::Create(Context, StartLoc, EndLoc,
                                               CancelRegion);
}
StmtResult Sema::ActOnOpenMPCancelDirective(ArrayRef<OMPClause *> Clauses,
                                            SourceLocation StartLoc,
                                            SourceLocation EndLoc,
                                            OpenMPDirectiveKind CancelRegion) {
  if (CancelRegion != OMPD_parallel && CancelRegion != OMPD_for &&
      CancelRegion != OMPD_sections && CancelRegion != OMPD_taskgroup) {
    Diag(StartLoc, diag::err_omp_wrong_cancel_region)
        << getOpenMPDirectiveName(CancelRegion);
    return StmtError();
}
  if (DSAStack->isParentNowaitRegion()) {
    Diag(StartLoc, diag::err_omp_parent_cancel_region_nowait) << 1;
    return StmtError();
}
  if (DSAStack->isParentOrderedRegion()) {
    Diag(StartLoc, diag::err_omp_parent_cancel_region_ordered) << 1;
    return StmtError();
}
  DSAStack->setParentCancelRegion(
true);
  return OMPCancelDirective::Create(Context, StartLoc, EndLoc, Clauses,
                                    CancelRegion);
}
static bool checkGrainsizeNumTasksClauses(Sema &S,
                                          ArrayRef<OMPClause *> Clauses) {
  OMPClause *PrevClause = nullptr;
  bool ErrorFound = false;
for_loop_talkad7420();
  for (auto *C : Clauses) {
    if (C->getClauseKind() == OMPC_grainsize ||
        C->getClauseKind() == OMPC_num_tasks) {
      if (!PrevClause)
        PrevClause = C;
      else if (PrevClause->getClauseKind() != C->getClauseKind()) {
        S.Diag(C->getLocStart(),
               diag::err_omp_grainsize_num_tasks_mutually_exclusive)
            << getOpenMPClauseName(C->getClauseKind())
            << getOpenMPClauseName(PrevClause->getClauseKind());
        S.Diag(PrevClause->getLocStart(),
               diag::note_omp_previous_grainsize_num_tasks)
            << getOpenMPClauseName(PrevClause->getClauseKind());
        ErrorFound = true;
}
}
}
  return ErrorFound;
}
StmtResult Sema::ActOnOpenMPTaskLoopDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_taskloop, getCollapseNumberExpr(Clauses),
nullptr, AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (checkGrainsizeNumTasksClauses(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTaskLoopDirective::Create(Context, StartLoc, EndLoc,
                                      NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTaskLoopSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_taskloop_simd, getCollapseNumberExpr(Clauses),
nullptr, AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkGrainsizeNumTasksClauses(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTaskLoopSimdDirective::Create(Context, StartLoc, EndLoc,
                                          NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPDistributeDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  assert(isa<CapturedStmt>(AStmt) && "Captured statement expected");
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_distribute, getCollapseNumberExpr(Clauses),
                      nullptr 
, AStmt,
                      *this, *DSAStack, VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPDistributeDirective::Create(Context, StartLoc, EndLoc,
                                        NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPDistributeParallelForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_distribute_parallel_for, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPDistributeParallelForDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPDistributeParallelForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_distribute_parallel_for_simd, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPDistributeParallelForSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPDistributeSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_distribute_simd, getCollapseNumberExpr(Clauses),
                      nullptr 
, AStmt,
                      *this, *DSAStack, VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPDistributeSimdDirective::Create(Context, StartLoc, EndLoc,
                                            NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetParallelForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_target_parallel_for_simd, getCollapseNumberExpr(Clauses),
      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target parallel for simd loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetParallelForSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_target_simd, getCollapseNumberExpr(Clauses),
                      getOrderedNumberExpr(Clauses), AStmt, *this, *DSAStack,
                      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target simd loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetSimdDirective::Create(Context, StartLoc, EndLoc,
                                        NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTeamsDistributeDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount =
      CheckOpenMPLoop(OMPD_teams_distribute, getCollapseNumberExpr(Clauses),
                      nullptr 
, AStmt,
                      *this, *DSAStack, VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp teams distribute loop exprs were not built");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTeamsDistributeDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTeamsDistributeSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_teams_distribute_simd, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp teams distribute simd loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTeamsDistributeSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTeamsDistributeParallelForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  auto NestedLoopCount = CheckOpenMPLoop(
      OMPD_teams_distribute_parallel_for_simd, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  if (checkSimdlenSafelenSpecified(*this, Clauses))
    return StmtError();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTeamsDistributeParallelForSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTeamsDistributeParallelForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  unsigned NestedLoopCount = CheckOpenMPLoop(
      OMPD_teams_distribute_parallel_for, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTeamsDistributeParallelForDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetTeamsDirective(ArrayRef<OMPClause *> Clauses,
                                                 Stmt *AStmt,
                                                 SourceLocation StartLoc,
                                                 SourceLocation EndLoc) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetTeamsDirective::Create(Context, StartLoc, EndLoc, Clauses,
                                         AStmt);
}
StmtResult Sema::ActOnOpenMPTargetTeamsDistributeDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  auto NestedLoopCount = CheckOpenMPLoop(
      OMPD_target_teams_distribute,
      getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target teams distribute loop exprs were not built");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetTeamsDistributeDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetTeamsDistributeParallelForDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  auto NestedLoopCount = CheckOpenMPLoop(
      OMPD_target_teams_distribute_parallel_for,
      getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target teams distribute parallel for loop exprs were not built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetTeamsDistributeParallelForDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetTeamsDistributeParallelForSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  CapturedStmt *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  auto NestedLoopCount = CheckOpenMPLoop(
      OMPD_target_teams_distribute_parallel_for_simd,
      getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target teams distribute parallel for simd loop exprs were not "
         "built");
  if (!CurContext->isDependentContext()) {
for_loop_talkad7420();
    for (auto C : Clauses) {
      if (auto *LC = dyn_cast<OMPLinearClause>(C))
        if (FinishOpenMPLinearClause(*LC, cast<DeclRefExpr>(B.IterationVarRef),
                                     B.NumIterations, *this, CurScope,
                                     DSAStack))
          return StmtError();
}
}
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetTeamsDistributeParallelForSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
StmtResult Sema::ActOnOpenMPTargetTeamsDistributeSimdDirective(
    ArrayRef<OMPClause *> Clauses, Stmt *AStmt, SourceLocation StartLoc,
    SourceLocation EndLoc,
    llvm::DenseMap<ValueDecl *, Expr *> &VarsWithImplicitDSA) {
  if (!AStmt)
    return StmtError();
  auto *CS = cast<CapturedStmt>(AStmt);
  CS->getCapturedDecl()->setNothrow();
  OMPLoopDirective::HelperExprs B;
  auto NestedLoopCount = CheckOpenMPLoop(
      OMPD_target_teams_distribute_simd, getCollapseNumberExpr(Clauses),
      nullptr 
, AStmt, *this, *DSAStack,
      VarsWithImplicitDSA, B);
  if (NestedLoopCount == 0)
    return StmtError();
  assert((CurContext->isDependentContext() || B.builtAll()) &&
         "omp target teams distribute simd loop exprs were not built");
  getCurFunction()->setHasBranchProtectedScope();
  return OMPTargetTeamsDistributeSimdDirective::Create(
      Context, StartLoc, EndLoc, NestedLoopCount, Clauses, AStmt, B);
}
OMPClause *Sema::ActOnOpenMPSingleExprClause(OpenMPClauseKind Kind, Expr *Expr,
                                             SourceLocation StartLoc,
                                             SourceLocation LParenLoc,
                                             SourceLocation EndLoc) {
  OMPClause *Res = nullptr;
  switch (Kind) {
  case OMPC_final:
    Res = ActOnOpenMPFinalClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_num_threads:
    Res = ActOnOpenMPNumThreadsClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_safelen:
    Res = ActOnOpenMPSafelenClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_simdlen:
    Res = ActOnOpenMPSimdlenClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_collapse:
    Res = ActOnOpenMPCollapseClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_ordered:
    Res = ActOnOpenMPOrderedClause(StartLoc, EndLoc, LParenLoc, Expr);
    break;
  case OMPC_device:
    Res = ActOnOpenMPDeviceClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_num_teams:
    Res = ActOnOpenMPNumTeamsClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_thread_limit:
    Res = ActOnOpenMPThreadLimitClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_priority:
    Res = ActOnOpenMPPriorityClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_grainsize:
    Res = ActOnOpenMPGrainsizeClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_num_tasks:
    Res = ActOnOpenMPNumTasksClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_hint:
    Res = ActOnOpenMPHintClause(Expr, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_if:
  case OMPC_default:
  case OMPC_proc_bind:
  case OMPC_schedule:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_threadprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_depend:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_map:
  case OMPC_nogroup:
  case OMPC_dist_schedule:
  case OMPC_defaultmap:
  case OMPC_unknown:
  case OMPC_uniform:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    llvm_unreachable("Clause is not allowed.");
}
  return Res;
}
OMPClause *Sema::ActOnOpenMPIfClause(OpenMPDirectiveKind NameModifier,
                                     Expr *Condition, SourceLocation StartLoc,
                                     SourceLocation LParenLoc,
                                     SourceLocation NameModifierLoc,
                                     SourceLocation ColonLoc,
                                     SourceLocation EndLoc) {
  Expr *ValExpr = Condition;
  if (!Condition->isValueDependent() && !Condition->isTypeDependent() &&
      !Condition->isInstantiationDependent() &&
      !Condition->containsUnexpandedParameterPack()) {
    ExprResult Val = CheckBooleanCondition(StartLoc, Condition);
    if (Val.isInvalid())
      return nullptr;
    ValExpr = MakeFullExpr(Val.get()).get();
}
  return new (Context) OMPIfClause(NameModifier, ValExpr, StartLoc, LParenLoc,
                                   NameModifierLoc, ColonLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPFinalClause(Expr *Condition,
                                        SourceLocation StartLoc,
                                        SourceLocation LParenLoc,
                                        SourceLocation EndLoc) {
  Expr *ValExpr = Condition;
  if (!Condition->isValueDependent() && !Condition->isTypeDependent() &&
      !Condition->isInstantiationDependent() &&
      !Condition->containsUnexpandedParameterPack()) {
    ExprResult Val = CheckBooleanCondition(StartLoc, Condition);
    if (Val.isInvalid())
      return nullptr;
    ValExpr = MakeFullExpr(Val.get()).get();
}
  return new (Context) OMPFinalClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
ExprResult Sema::PerformOpenMPImplicitIntegerConversion(SourceLocation Loc,
                                                        Expr *Op) {
  if (!Op)
    return ExprError();
  class IntConvertDiagnoser : public ICEConvertDiagnoser {
  public:
    IntConvertDiagnoser()
        : ICEConvertDiagnoser(
 false, false, true) {
}
    SemaDiagnosticBuilder diagnoseNotInt(Sema &S, SourceLocation Loc,
                                         QualType T) override {
      return S.Diag(Loc, diag::err_omp_not_integral) << T;
}
    SemaDiagnosticBuilder diagnoseIncomplete(Sema &S, SourceLocation Loc,
                                             QualType T) override {
      return S.Diag(Loc, diag::err_omp_incomplete_type) << T;
}
    SemaDiagnosticBuilder diagnoseExplicitConv(Sema &S, SourceLocation Loc,
                                               QualType T,
                                               QualType ConvTy) override {
      return S.Diag(Loc, diag::err_omp_explicit_conversion) << T << ConvTy;
}
    SemaDiagnosticBuilder noteExplicitConv(Sema &S, CXXConversionDecl *Conv,
                                           QualType ConvTy) override {
      return S.Diag(Conv->getLocation(), diag::note_omp_conversion_here)
             << ConvTy->isEnumeralType() << ConvTy;
}
    SemaDiagnosticBuilder diagnoseAmbiguous(Sema &S, SourceLocation Loc,
                                            QualType T) override {
      return S.Diag(Loc, diag::err_omp_ambiguous_conversion) << T;
}
    SemaDiagnosticBuilder noteAmbiguous(Sema &S, CXXConversionDecl *Conv,
                                        QualType ConvTy) override {
      return S.Diag(Conv->getLocation(), diag::note_omp_conversion_here)
             << ConvTy->isEnumeralType() << ConvTy;
}
    SemaDiagnosticBuilder diagnoseConversion(Sema &, SourceLocation, QualType,
                                             QualType) override {
      llvm_unreachable("conversion functions are permitted");
}
} ConvertDiagnoser;
  return PerformContextualImplicitConversion(Loc, Op, ConvertDiagnoser);
}
static bool IsNonNegativeIntegerValue(Expr *&ValExpr, Sema &SemaRef,
                                      OpenMPClauseKind CKind,
                                      bool StrictlyPositive) {
  if (!ValExpr->isTypeDependent() && !ValExpr->isValueDependent() &&
      !ValExpr->isInstantiationDependent()) {
    SourceLocation Loc = ValExpr->getExprLoc();
    ExprResult Value =
        SemaRef.PerformOpenMPImplicitIntegerConversion(Loc, ValExpr);
    if (Value.isInvalid())
      return false;
    ValExpr = Value.get();
    llvm::APSInt Result;
    if (ValExpr->isIntegerConstantExpr(Result, SemaRef.Context) &&
        Result.isSigned() &&
        !((!StrictlyPositive && Result.isNonNegative()) ||
          (StrictlyPositive && Result.isStrictlyPositive()))) {
      SemaRef.Diag(Loc, diag::err_omp_negative_expression_in_clause)
          << getOpenMPClauseName(CKind) << (StrictlyPositive ? 1 : 0)
          << ValExpr->getSourceRange();
      return false;
}
}
  return true;
}
OMPClause *Sema::ActOnOpenMPNumThreadsClause(Expr *NumThreads,
                                             SourceLocation StartLoc,
                                             SourceLocation LParenLoc,
                                             SourceLocation EndLoc) {
  Expr *ValExpr = NumThreads;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_num_threads,
true))
    return nullptr;
  return new (Context)
      OMPNumThreadsClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
ExprResult Sema::VerifyPositiveIntegerConstantInClause(Expr *E,
                                                       OpenMPClauseKind CKind,
                                                       bool StrictlyPositive) {
  if (!E)
    return ExprError();
  if (E->isValueDependent() || E->isTypeDependent() ||
      E->isInstantiationDependent() || E->containsUnexpandedParameterPack())
    return E;
  llvm::APSInt Result;
  ExprResult ICE = VerifyIntegerConstantExpression(E, &Result);
  if (ICE.isInvalid())
    return ExprError();
  if ((StrictlyPositive && !Result.isStrictlyPositive()) ||
      (!StrictlyPositive && !Result.isNonNegative())) {
    Diag(E->getExprLoc(), diag::err_omp_negative_expression_in_clause)
        << getOpenMPClauseName(CKind) << (StrictlyPositive ? 1 : 0)
        << E->getSourceRange();
    return ExprError();
}
  if (CKind == OMPC_aligned && !Result.isPowerOf2()) {
    Diag(E->getExprLoc(), diag::warn_omp_alignment_not_power_of_two)
        << E->getSourceRange();
    return ExprError();
}
  if (CKind == OMPC_collapse && DSAStack->getAssociatedLoops() == 1)
    DSAStack->setAssociatedLoops(Result.getExtValue());
  else if (CKind == OMPC_ordered)
    DSAStack->setAssociatedLoops(Result.getExtValue());
  return ICE;
}
OMPClause *Sema::ActOnOpenMPSafelenClause(Expr *Len, SourceLocation StartLoc,
                                          SourceLocation LParenLoc,
                                          SourceLocation EndLoc) {
  ExprResult Safelen = VerifyPositiveIntegerConstantInClause(Len, OMPC_safelen);
  if (Safelen.isInvalid())
    return nullptr;
  return new (Context)
      OMPSafelenClause(Safelen.get(), StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPSimdlenClause(Expr *Len, SourceLocation StartLoc,
                                          SourceLocation LParenLoc,
                                          SourceLocation EndLoc) {
  ExprResult Simdlen = VerifyPositiveIntegerConstantInClause(Len, OMPC_simdlen);
  if (Simdlen.isInvalid())
    return nullptr;
  return new (Context)
      OMPSimdlenClause(Simdlen.get(), StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPCollapseClause(Expr *NumForLoops,
                                           SourceLocation StartLoc,
                                           SourceLocation LParenLoc,
                                           SourceLocation EndLoc) {
  ExprResult NumForLoopsResult =
      VerifyPositiveIntegerConstantInClause(NumForLoops, OMPC_collapse);
  if (NumForLoopsResult.isInvalid())
    return nullptr;
  return new (Context)
      OMPCollapseClause(NumForLoopsResult.get(), StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPOrderedClause(SourceLocation StartLoc,
                                          SourceLocation EndLoc,
                                          SourceLocation LParenLoc,
                                          Expr *NumForLoops) {
  if (NumForLoops && LParenLoc.isValid()) {
    ExprResult NumForLoopsResult =
        VerifyPositiveIntegerConstantInClause(NumForLoops, OMPC_ordered);
    if (NumForLoopsResult.isInvalid())
      return nullptr;
    NumForLoops = NumForLoopsResult.get();
} else
    NumForLoops = nullptr;
  DSAStack->setOrderedRegion(
true, NumForLoops);
  return new (Context)
      OMPOrderedClause(NumForLoops, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPSimpleClause(
    OpenMPClauseKind Kind, unsigned Argument, SourceLocation ArgumentLoc,
    SourceLocation StartLoc, SourceLocation LParenLoc, SourceLocation EndLoc) {
  OMPClause *Res = nullptr;
  switch (Kind) {
  case OMPC_default:
    Res =
        ActOnOpenMPDefaultClause(static_cast<OpenMPDefaultClauseKind>(Argument),
                                 ArgumentLoc, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_proc_bind:
    Res = ActOnOpenMPProcBindClause(
        static_cast<OpenMPProcBindClauseKind>(Argument), ArgumentLoc, StartLoc,
        LParenLoc, EndLoc);
    break;
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_schedule:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_threadprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_depend:
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
  case OMPC_unknown:
  case OMPC_uniform:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    llvm_unreachable("Clause is not allowed.");
}
  return Res;
}
static std::string
getListOfPossibleValues(OpenMPClauseKind K, unsigned First, unsigned Last,
                        ArrayRef<unsigned> Exclude = llvm::None) {
  std::string Values;
  unsigned Bound = Last >= 2 ? Last - 2 : 0;
  unsigned Skipped = Exclude.size();
  auto S = Exclude.begin(), E = Exclude.end();
for_loop_talkad7420();
  for (unsigned i = First; i < Last; ++i) {
    if (std::find(S, E, i) != E) {
      --Skipped;
      continue;
}
    Values += "'";
    Values += getOpenMPSimpleClauseTypeName(K, i);
    Values += "'";
    if (i == Bound - Skipped)
      Values += " or ";
    else if (i != Bound + 1 - Skipped)
      Values += ", ";
}
  return Values;
}
OMPClause *Sema::ActOnOpenMPDefaultClause(OpenMPDefaultClauseKind Kind,
                                          SourceLocation KindKwLoc,
                                          SourceLocation StartLoc,
                                          SourceLocation LParenLoc,
                                          SourceLocation EndLoc) {
  if (Kind == OMPC_DEFAULT_unknown) {
    static_assert(OMPC_DEFAULT_unknown > 0,
                  "OMPC_DEFAULT_unknown not greater than 0");
    Diag(KindKwLoc, diag::err_omp_unexpected_clause_value)
        << getListOfPossibleValues(OMPC_default, 
0,
OMPC_DEFAULT_unknown)
        << getOpenMPClauseName(OMPC_default);
    return nullptr;
}
  switch (Kind) {
  case OMPC_DEFAULT_none:
    DSAStack->setDefaultDSANone(KindKwLoc);
    break;
  case OMPC_DEFAULT_shared:
    DSAStack->setDefaultDSAShared(KindKwLoc);
    break;
  case OMPC_DEFAULT_unknown:
    llvm_unreachable("Clause kind is not allowed.");
    break;
}
  return new (Context)
      OMPDefaultClause(Kind, KindKwLoc, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPProcBindClause(OpenMPProcBindClauseKind Kind,
                                           SourceLocation KindKwLoc,
                                           SourceLocation StartLoc,
                                           SourceLocation LParenLoc,
                                           SourceLocation EndLoc) {
  if (Kind == OMPC_PROC_BIND_unknown) {
    Diag(KindKwLoc, diag::err_omp_unexpected_clause_value)
        << getListOfPossibleValues(OMPC_proc_bind, 
0,
OMPC_PROC_BIND_unknown)
        << getOpenMPClauseName(OMPC_proc_bind);
    return nullptr;
}
  return new (Context)
      OMPProcBindClause(Kind, KindKwLoc, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPSingleExprWithArgClause(
    OpenMPClauseKind Kind, ArrayRef<unsigned> Argument, Expr *Expr,
    SourceLocation StartLoc, SourceLocation LParenLoc,
    ArrayRef<SourceLocation> ArgumentLoc, SourceLocation DelimLoc,
    SourceLocation EndLoc) {
  OMPClause *Res = nullptr;
  switch (Kind) {
  case OMPC_schedule:
    enum { Modifier1, Modifier2, ScheduleKind, NumberOfElements 
};
    assert(Argument.size() == NumberOfElements &&
           ArgumentLoc.size() == NumberOfElements);
    Res = ActOnOpenMPScheduleClause(
        static_cast<OpenMPScheduleClauseModifier>(Argument[Modifier1]),
        static_cast<OpenMPScheduleClauseModifier>(Argument[Modifier2]),
        static_cast<OpenMPScheduleClauseKind>(Argument[ScheduleKind]), Expr,
        StartLoc, LParenLoc, ArgumentLoc[Modifier1], ArgumentLoc[Modifier2],
        ArgumentLoc[ScheduleKind], DelimLoc, EndLoc);
    break;
  case OMPC_if:
    assert(Argument.size() == 1 && ArgumentLoc.size() == 1);
    Res = ActOnOpenMPIfClause(static_cast<OpenMPDirectiveKind>(Argument.back()),
                              Expr, StartLoc, LParenLoc, ArgumentLoc.back(),
                              DelimLoc, EndLoc);
    break;
  case OMPC_dist_schedule:
    Res = ActOnOpenMPDistScheduleClause(
        static_cast<OpenMPDistScheduleClauseKind>(Argument.back()), Expr,
        StartLoc, LParenLoc, ArgumentLoc.back(), DelimLoc, EndLoc);
    break;
  case OMPC_defaultmap:
    enum { Modifier, DefaultmapKind 
};
    Res = ActOnOpenMPDefaultmapClause(
        static_cast<OpenMPDefaultmapClauseModifier>(Argument[Modifier]),
        static_cast<OpenMPDefaultmapClauseKind>(Argument[DefaultmapKind]),
        StartLoc, LParenLoc, ArgumentLoc[Modifier], ArgumentLoc[DefaultmapKind],
        EndLoc);
    break;
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_default:
  case OMPC_proc_bind:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_threadprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_depend:
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
  case OMPC_unknown:
  case OMPC_uniform:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    llvm_unreachable("Clause is not allowed.");
}
  return Res;
}
static bool checkScheduleModifiers(Sema &S, OpenMPScheduleClauseModifier M1,
                                   OpenMPScheduleClauseModifier M2,
                                   SourceLocation M1Loc, SourceLocation M2Loc) {
  if (M1 == OMPC_SCHEDULE_MODIFIER_unknown && M1Loc.isValid()) {
    SmallVector<unsigned, 2> Excluded;
    if (M2 != OMPC_SCHEDULE_MODIFIER_unknown)
      Excluded.push_back(M2);
    if (M2 == OMPC_SCHEDULE_MODIFIER_nonmonotonic)
      Excluded.push_back(OMPC_SCHEDULE_MODIFIER_monotonic);
    if (M2 == OMPC_SCHEDULE_MODIFIER_monotonic)
      Excluded.push_back(OMPC_SCHEDULE_MODIFIER_nonmonotonic);
    S.Diag(M1Loc, diag::err_omp_unexpected_clause_value)
        << getListOfPossibleValues(OMPC_schedule,
OMPC_SCHEDULE_MODIFIER_unknown + 1,
OMPC_SCHEDULE_MODIFIER_last,
                                   Excluded)
        << getOpenMPClauseName(OMPC_schedule);
    return true;
}
  return false;
}
OMPClause *Sema::ActOnOpenMPScheduleClause(
    OpenMPScheduleClauseModifier M1, OpenMPScheduleClauseModifier M2,
    OpenMPScheduleClauseKind Kind, Expr *ChunkSize, SourceLocation StartLoc,
    SourceLocation LParenLoc, SourceLocation M1Loc, SourceLocation M2Loc,
    SourceLocation KindLoc, SourceLocation CommaLoc, SourceLocation EndLoc) {
  if (checkScheduleModifiers(*this, M1, M2, M1Loc, M2Loc) ||
      checkScheduleModifiers(*this, M2, M1, M2Loc, M1Loc))
    return nullptr;
  if ((M1 == M2 && M1 != OMPC_SCHEDULE_MODIFIER_unknown) ||
      (M1 == OMPC_SCHEDULE_MODIFIER_monotonic &&
       M2 == OMPC_SCHEDULE_MODIFIER_nonmonotonic) ||
      (M1 == OMPC_SCHEDULE_MODIFIER_nonmonotonic &&
       M2 == OMPC_SCHEDULE_MODIFIER_monotonic)) {
    Diag(M2Loc, diag::err_omp_unexpected_schedule_modifier)
        << getOpenMPSimpleClauseTypeName(OMPC_schedule, M2)
        << getOpenMPSimpleClauseTypeName(OMPC_schedule, M1);
    return nullptr;
}
  if (Kind == OMPC_SCHEDULE_unknown) {
    std::string Values;
    if (M1Loc.isInvalid() && M2Loc.isInvalid()) {
      unsigned Exclude[] = {OMPC_SCHEDULE_unknown
};
      Values = getListOfPossibleValues(OMPC_schedule, 
0,
OMPC_SCHEDULE_MODIFIER_last,
                                       Exclude);
} else {
      Values = getListOfPossibleValues(OMPC_schedule, 
0,
OMPC_SCHEDULE_unknown);
}
    Diag(KindLoc, diag::err_omp_unexpected_clause_value)
        << Values << getOpenMPClauseName(OMPC_schedule);
    return nullptr;
}
  if ((M1 == OMPC_SCHEDULE_MODIFIER_nonmonotonic ||
       M2 == OMPC_SCHEDULE_MODIFIER_nonmonotonic) &&
      Kind != OMPC_SCHEDULE_dynamic && Kind != OMPC_SCHEDULE_guided) {
    Diag(M1 == OMPC_SCHEDULE_MODIFIER_nonmonotonic ? M1Loc : M2Loc,
         diag::err_omp_schedule_nonmonotonic_static);
    return nullptr;
}
  Expr *ValExpr = ChunkSize;
  Stmt *HelperValStmt = nullptr;
  if (ChunkSize) {
    if (!ChunkSize->isValueDependent() && !ChunkSize->isTypeDependent() &&
        !ChunkSize->isInstantiationDependent() &&
        !ChunkSize->containsUnexpandedParameterPack()) {
      SourceLocation ChunkSizeLoc = ChunkSize->getLocStart();
      ExprResult Val =
          PerformOpenMPImplicitIntegerConversion(ChunkSizeLoc, ChunkSize);
      if (Val.isInvalid())
        return nullptr;
      ValExpr = Val.get();
      llvm::APSInt Result;
      if (ValExpr->isIntegerConstantExpr(Result, Context)) {
        if (Result.isSigned() && !Result.isStrictlyPositive()) {
          Diag(ChunkSizeLoc, diag::err_omp_negative_expression_in_clause)
              << "schedule" << 1 << ChunkSize->getSourceRange();
          return nullptr;
}
} else if (isParallelOrTaskRegion(DSAStack->getCurrentDirective()) &&
                 !CurContext->isDependentContext()) {
        llvm::MapVector<Expr *, DeclRefExpr *> Captures;
        ValExpr = tryBuildCapture(*this, ValExpr, Captures).get();
        HelperValStmt = buildPreInits(Context, Captures);
}
}
}
  return new (Context)
      OMPScheduleClause(StartLoc, LParenLoc, KindLoc, CommaLoc, EndLoc, Kind,
                        ValExpr, HelperValStmt, M1, M1Loc, M2, M2Loc);
}
OMPClause *Sema::ActOnOpenMPClause(OpenMPClauseKind Kind,
                                   SourceLocation StartLoc,
                                   SourceLocation EndLoc) {
  OMPClause *Res = nullptr;
  switch (Kind) {
  case OMPC_ordered:
    Res = ActOnOpenMPOrderedClause(StartLoc, EndLoc);
    break;
  case OMPC_nowait:
    Res = ActOnOpenMPNowaitClause(StartLoc, EndLoc);
    break;
  case OMPC_untied:
    Res = ActOnOpenMPUntiedClause(StartLoc, EndLoc);
    break;
  case OMPC_mergeable:
    Res = ActOnOpenMPMergeableClause(StartLoc, EndLoc);
    break;
  case OMPC_read:
    Res = ActOnOpenMPReadClause(StartLoc, EndLoc);
    break;
  case OMPC_write:
    Res = ActOnOpenMPWriteClause(StartLoc, EndLoc);
    break;
  case OMPC_update:
    Res = ActOnOpenMPUpdateClause(StartLoc, EndLoc);
    break;
  case OMPC_capture:
    Res = ActOnOpenMPCaptureClause(StartLoc, EndLoc);
    break;
  case OMPC_seq_cst:
    Res = ActOnOpenMPSeqCstClause(StartLoc, EndLoc);
    break;
  case OMPC_threads:
    Res = ActOnOpenMPThreadsClause(StartLoc, EndLoc);
    break;
  case OMPC_simd:
    Res = ActOnOpenMPSIMDClause(StartLoc, EndLoc);
    break;
  case OMPC_nogroup:
    Res = ActOnOpenMPNogroupClause(StartLoc, EndLoc);
    break;
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_schedule:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_default:
  case OMPC_proc_bind:
  case OMPC_threadprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_depend:
  case OMPC_device:
  case OMPC_map:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_num_tasks:
  case OMPC_hint:
  case OMPC_dist_schedule:
  case OMPC_defaultmap:
  case OMPC_unknown:
  case OMPC_uniform:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    llvm_unreachable("Clause is not allowed.");
}
  return Res;
}
OMPClause *Sema::ActOnOpenMPNowaitClause(SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  DSAStack->setNowaitRegion();
  return new (Context) OMPNowaitClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPUntiedClause(SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  return new (Context) OMPUntiedClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPMergeableClause(SourceLocation StartLoc,
                                            SourceLocation EndLoc) {
  return new (Context) OMPMergeableClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPReadClause(SourceLocation StartLoc,
                                       SourceLocation EndLoc) {
  return new (Context) OMPReadClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPWriteClause(SourceLocation StartLoc,
                                        SourceLocation EndLoc) {
  return new (Context) OMPWriteClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPUpdateClause(SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  return new (Context) OMPUpdateClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPCaptureClause(SourceLocation StartLoc,
                                          SourceLocation EndLoc) {
  return new (Context) OMPCaptureClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPSeqCstClause(SourceLocation StartLoc,
                                         SourceLocation EndLoc) {
  return new (Context) OMPSeqCstClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPThreadsClause(SourceLocation StartLoc,
                                          SourceLocation EndLoc) {
  return new (Context) OMPThreadsClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPSIMDClause(SourceLocation StartLoc,
                                       SourceLocation EndLoc) {
  return new (Context) OMPSIMDClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPNogroupClause(SourceLocation StartLoc,
                                          SourceLocation EndLoc) {
  return new (Context) OMPNogroupClause(StartLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPVarListClause(
    OpenMPClauseKind Kind, ArrayRef<Expr *> VarList, Expr *TailExpr,
    SourceLocation StartLoc, SourceLocation LParenLoc, SourceLocation ColonLoc,
    SourceLocation EndLoc, CXXScopeSpec &ReductionIdScopeSpec,
    const DeclarationNameInfo &ReductionId, OpenMPDependClauseKind DepKind,
    OpenMPLinearClauseKind LinKind, OpenMPMapClauseKind MapTypeModifier,
    OpenMPMapClauseKind MapType, bool IsMapTypeImplicit,
    SourceLocation DepLinMapLoc) {
  OMPClause *Res = nullptr;
  switch (Kind) {
  case OMPC_private:
    Res = ActOnOpenMPPrivateClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_firstprivate:
    Res = ActOnOpenMPFirstprivateClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_lastprivate:
    Res = ActOnOpenMPLastprivateClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_shared:
    Res = ActOnOpenMPSharedClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_reduction:
    Res = ActOnOpenMPReductionClause(VarList, StartLoc, LParenLoc, ColonLoc,
                                     EndLoc, ReductionIdScopeSpec, ReductionId);
    break;
  case OMPC_linear:
    Res = ActOnOpenMPLinearClause(VarList, TailExpr, StartLoc, LParenLoc,
                                  LinKind, DepLinMapLoc, ColonLoc, EndLoc);
    break;
  case OMPC_aligned:
    Res = ActOnOpenMPAlignedClause(VarList, TailExpr, StartLoc, LParenLoc,
                                   ColonLoc, EndLoc);
    break;
  case OMPC_copyin:
    Res = ActOnOpenMPCopyinClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_copyprivate:
    Res = ActOnOpenMPCopyprivateClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_flush:
    Res = ActOnOpenMPFlushClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_inc:
    Res = ActOnOpenMPIncClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_depend:
    Res = ActOnOpenMPDependClause(DepKind, DepLinMapLoc, ColonLoc, VarList,
                                  StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_map:
    Res = ActOnOpenMPMapClause(MapTypeModifier, MapType, IsMapTypeImplicit,
                               DepLinMapLoc, ColonLoc, VarList, StartLoc,
                               LParenLoc, EndLoc);
    break;
  case OMPC_to:
    Res = ActOnOpenMPToClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_from:
    Res = ActOnOpenMPFromClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_use_device_ptr:
    Res = ActOnOpenMPUseDevicePtrClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_is_device_ptr:
    Res = ActOnOpenMPIsDevicePtrClause(VarList, StartLoc, LParenLoc, EndLoc);
    break;
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_default:
  case OMPC_proc_bind:
  case OMPC_schedule:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_threadprivate:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_device:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_nogroup:
  case OMPC_num_tasks:
  case OMPC_hint:
  case OMPC_dist_schedule:
  case OMPC_defaultmap:
  case OMPC_unknown:
  case OMPC_uniform:
    llvm_unreachable("Clause is not allowed.");
}
  return Res;
}
ExprResult Sema::getOpenMPCapturedExpr(VarDecl *Capture, ExprValueKind VK,
                                       ExprObjectKind OK, SourceLocation Loc) {
  ExprResult Res = BuildDeclRefExpr(
      Capture, Capture->getType().getNonReferenceType(), VK_LValue, Loc);
  if (!Res.isUsable())
    return ExprError();
  if (OK == OK_Ordinary && !getLangOpts().CPlusPlus) {
    Res = CreateBuiltinUnaryOp(Loc, UO_Deref, Res.get());
    if (!Res.isUsable())
      return ExprError();
}
  if (VK != VK_LValue && Res.get()->isGLValue()) {
    Res = DefaultLvalueConversion(Res.get());
    if (!Res.isUsable())
      return ExprError();
}
  return Res;
}
static std::pair<ValueDecl *, bool>
getPrivateItem(Sema &S, Expr *&RefExpr, SourceLocation &ELoc,
               SourceRange &ERange, bool AllowArraySection = false) {
  if (RefExpr->isTypeDependent() || RefExpr->isValueDependent() ||
      RefExpr->containsUnexpandedParameterPack())
    return std::make_pair(nullptr, true);
  RefExpr = RefExpr->IgnoreParens();
  enum {
    NoArrayExpr = -1,
    ArraySubscript = 0,
    OMPArraySection = 1
} IsArrayExpr = NoArrayExpr;
  if (AllowArraySection) {
    if (auto *ASE = dyn_cast_or_null<ArraySubscriptExpr>(RefExpr)) {
      auto *Base = ASE->getBase()->IgnoreParenImpCasts();
      while (auto *TempASE = dyn_cast<ArraySubscriptExpr>(Base))
        Base = TempASE->getBase()->IgnoreParenImpCasts();
      RefExpr = Base;
      IsArrayExpr = ArraySubscript;
} else if (auto *OASE = dyn_cast_or_null<OMPArraySectionExpr>(RefExpr)) {
      auto *Base = OASE->getBase()->IgnoreParenImpCasts();
      while (auto *TempOASE = dyn_cast<OMPArraySectionExpr>(Base))
        Base = TempOASE->getBase()->IgnoreParenImpCasts();
      while (auto *TempASE = dyn_cast<ArraySubscriptExpr>(Base))
        Base = TempASE->getBase()->IgnoreParenImpCasts();
      RefExpr = Base;
      IsArrayExpr = OMPArraySection;
}
}
  ELoc = RefExpr->getExprLoc();
  ERange = RefExpr->getSourceRange();
  RefExpr = RefExpr->IgnoreParenImpCasts();
  auto *DE = dyn_cast_or_null<DeclRefExpr>(RefExpr);
  auto *ME = dyn_cast_or_null<MemberExpr>(RefExpr);
  if ((!DE || !isa<VarDecl>(DE->getDecl())) &&
      (S.getCurrentThisType().isNull() || !ME ||
       !isa<CXXThisExpr>(ME->getBase()->IgnoreParenImpCasts()) ||
       !isa<FieldDecl>(ME->getMemberDecl()))) {
    if (IsArrayExpr != NoArrayExpr)
      S.Diag(ELoc, diag::err_omp_expected_base_var_name) << IsArrayExpr
                                                         << ERange;
    else {
      S.Diag(ELoc,
             AllowArraySection
                 ? diag::err_omp_expected_var_name_member_expr_or_array_item
                 : diag::err_omp_expected_var_name_member_expr)
          << (S.getCurrentThisType().isNull() ? 0 : 1) << ERange;
}
    return std::make_pair(nullptr, false);
}
  return std::make_pair(DE ? DE->getDecl() : ME->getMemberDecl(), false);
}
OMPClause *Sema::ActOnOpenMPPrivateClause(ArrayRef<Expr *> VarList,
                                          SourceLocation StartLoc,
                                          SourceLocation LParenLoc,
                                          SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> PrivateCopies;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP private clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      Vars.push_back(RefExpr);
      PrivateCopies.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    if (RequireCompleteType(ELoc, Type, diag::err_omp_private_incomplete_type))
      continue;
    Type = Type.getNonReferenceType();
    DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
    if (DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_private) {
      Diag(ELoc, diag::err_omp_wrong_dsa) << getOpenMPClauseName(DVar.CKind)
                                          << getOpenMPClauseName(OMPC_private);
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    auto CurrDir = DSAStack->getCurrentDirective();
    if (!Type->isAnyPointerType() && Type->isVariablyModifiedType() &&
        isOpenMPTaskingDirective(CurrDir)) {
      Diag(ELoc, diag::err_omp_variably_modified_type_not_supported)
          << getOpenMPClauseName(OMPC_private) << Type
          << getOpenMPDirectiveName(CurrDir);
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
      continue;
}
    if (CurrDir == OMPD_target || CurrDir == OMPD_target_parallel ||
        CurrDir == OMPD_target_teams || 
        CurrDir == OMPD_target_teams_distribute ||
        CurrDir == OMPD_target_teams_distribute_parallel_for ||
        CurrDir == OMPD_target_teams_distribute_parallel_for_simd ||
        CurrDir == OMPD_target_teams_distribute_simd ||
        CurrDir == OMPD_target_parallel_for_simd ||
        CurrDir == OMPD_target_parallel_for) {
      OpenMPClauseKind ConflictKind;
      if (DSAStack->checkMappableExprComponentListsForDecl(
              VD, 
true,
              [&](OMPClauseMappableExprCommon::MappableExprComponentListRef,
                  OpenMPClauseKind WhereFoundClauseKind) -> bool {
                ConflictKind = WhereFoundClauseKind;
                return true;
})) {
        Diag(ELoc, diag::err_omp_variable_in_given_clause_and_dsa)
            << getOpenMPClauseName(OMPC_private)
            << getOpenMPClauseName(ConflictKind)
            << getOpenMPDirectiveName(CurrDir);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
}
    Type = Type.getUnqualifiedType();
    auto VDPrivate = buildVarDecl(*this, ELoc, Type, D->getName(),
                                  D->hasAttrs() ? &D->getAttrs() : nullptr);
    ActOnUninitializedDecl(VDPrivate);
    if (VDPrivate->isInvalidDecl())
      continue;
    auto VDPrivateRefExpr = buildDeclRefExpr(
        *this, VDPrivate, RefExpr->getType().getUnqualifiedType(), ELoc);
    DeclRefExpr *Ref = nullptr;
    if (!VD && !CurContext->isDependentContext())
      Ref = buildCapture(*this, D, SimpleRefExpr, 
false);
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_private, Ref);
    Vars.push_back((VD || CurContext->isDependentContext())
                       ? RefExpr->IgnoreParens()
                       : Ref);
    PrivateCopies.push_back(VDPrivateRefExpr);
}
  if (Vars.empty())
    return nullptr;
  return OMPPrivateClause::Create(Context, StartLoc, LParenLoc, EndLoc, Vars,
                                  PrivateCopies);
}
namespace {
class DiagsUninitializedSeveretyRAII {
private:
  DiagnosticsEngine &Diags;
  SourceLocation SavedLoc;
  bool IsIgnored;
public:
  DiagsUninitializedSeveretyRAII(DiagnosticsEngine &Diags, SourceLocation Loc,
                                 bool IsIgnored)
      : Diags(Diags), SavedLoc(Loc), IsIgnored(IsIgnored) {
    if (!IsIgnored) {
      Diags.setSeverity(
 diag::warn_uninit_self_reference_in_init,
 diag::Severity::Ignored, Loc);
}
}
  ~DiagsUninitializedSeveretyRAII() {
    if (!IsIgnored)
      Diags.popMappings(SavedLoc);
}
};
}
OMPClause *Sema::ActOnOpenMPFirstprivateClause(ArrayRef<Expr *> VarList,
                                               SourceLocation StartLoc,
                                               SourceLocation LParenLoc,
                                               SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> PrivateCopies;
  SmallVector<Expr *, 8> Inits;
  SmallVector<Decl *, 4> ExprCaptures;
  bool IsImplicitClause =
      StartLoc.isInvalid() && LParenLoc.isInvalid() && EndLoc.isInvalid();
  auto ImplicitClauseLoc = DSAStack->getConstructLoc();
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP firstprivate clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      Vars.push_back(RefExpr);
      PrivateCopies.push_back(nullptr);
      Inits.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    ELoc = IsImplicitClause ? ImplicitClauseLoc : ELoc;
    QualType Type = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    if (RequireCompleteType(ELoc, Type,
                            diag::err_omp_firstprivate_incomplete_type))
      continue;
    Type = Type.getNonReferenceType();
    auto ElemType = Context.getBaseElementType(Type).getNonReferenceType();
    DSAStackTy::DSAVarData TopDVar;
    if (!IsImplicitClause) {
      DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
      TopDVar = DVar;
      bool IsConstant = ElemType.isConstant(Context);
      if (DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_firstprivate &&
          DVar.CKind != OMPC_lastprivate && DVar.RefExpr) {
        Diag(ELoc, diag::err_omp_wrong_dsa)
            << getOpenMPClauseName(DVar.CKind)
            << getOpenMPClauseName(OMPC_firstprivate);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
      if (!(IsConstant || (VD && VD->isStaticDataMember())) && !DVar.RefExpr &&
          DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_shared) {
        Diag(ELoc, diag::err_omp_wrong_dsa)
            << getOpenMPClauseName(DVar.CKind)
            << getOpenMPClauseName(OMPC_firstprivate);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
      OpenMPDirectiveKind CurrDir = DSAStack->getCurrentDirective();
      if (isOpenMPWorksharingDirective(CurrDir) &&
          !isOpenMPParallelDirective(CurrDir) &&
          !isOpenMPTeamsDirective(CurrDir)) {
        DVar = DSAStack->getImplicitDSA(D, true);
        if (DVar.CKind != OMPC_shared &&
            (isOpenMPParallelDirective(DVar.DKind) ||
             DVar.DKind == OMPD_unknown)) {
          Diag(ELoc, diag::err_omp_required_access)
              << getOpenMPClauseName(OMPC_firstprivate)
              << getOpenMPClauseName(OMPC_shared);
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
}
      if (isOpenMPTaskingDirective(CurrDir)) {
        DVar = DSAStack->hasInnermostDSA(
            D, [](OpenMPClauseKind C) -> bool { return C == OMPC_reduction; 
},
            [](OpenMPDirectiveKind K) -> bool {
              return isOpenMPParallelDirective(K) ||
                     isOpenMPWorksharingDirective(K);
},
            false);
        if (DVar.CKind == OMPC_reduction &&
            (isOpenMPParallelDirective(DVar.DKind) ||
             isOpenMPWorksharingDirective(DVar.DKind))) {
          Diag(ELoc, diag::err_omp_parallel_reduction_in_task_firstprivate)
              << getOpenMPDirectiveName(DVar.DKind);
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
}
      if (CurrDir == OMPD_distribute) {
        DVar = DSAStack->hasInnermostDSA(
            D, [](OpenMPClauseKind C) -> bool { return C == OMPC_private; 
},
            [](OpenMPDirectiveKind K) -> bool {
              return isOpenMPTeamsDirective(K);
},
            false);
        if (DVar.CKind == OMPC_private && isOpenMPTeamsDirective(DVar.DKind)) {
          Diag(ELoc, diag::err_omp_firstprivate_distribute_private_teams);
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
        DVar = DSAStack->hasInnermostDSA(
            D, [](OpenMPClauseKind C) -> bool { return C == OMPC_reduction; 
},
            [](OpenMPDirectiveKind K) -> bool {
              return isOpenMPTeamsDirective(K);
},
            false);
        if (DVar.CKind == OMPC_reduction &&
            isOpenMPTeamsDirective(DVar.DKind)) {
          Diag(ELoc, diag::err_omp_firstprivate_distribute_in_teams_reduction);
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
        DVar = DSAStack->getTopDSA(D, false);
        if (DVar.CKind == OMPC_lastprivate) {
          Diag(ELoc, diag::err_omp_firstprivate_and_lastprivate_in_distribute);
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
}
      if (CurrDir == OMPD_target || CurrDir == OMPD_target_parallel ||
          CurrDir == OMPD_target_teams || 
          CurrDir == OMPD_target_teams_distribute ||
          CurrDir == OMPD_target_teams_distribute_parallel_for ||
          CurrDir == OMPD_target_teams_distribute_parallel_for_simd ||
          CurrDir == OMPD_target_teams_distribute_simd ||
          CurrDir == OMPD_target_parallel_for_simd ||
          CurrDir == OMPD_target_parallel_for) {
        OpenMPClauseKind ConflictKind;
        if (DSAStack->checkMappableExprComponentListsForDecl(
                VD, 
true,
                [&](OMPClauseMappableExprCommon::MappableExprComponentListRef,
                    OpenMPClauseKind WhereFoundClauseKind) -> bool {
                  ConflictKind = WhereFoundClauseKind;
                  return true;
})) {
          Diag(ELoc, diag::err_omp_variable_in_given_clause_and_dsa)
              << getOpenMPClauseName(OMPC_firstprivate)
              << getOpenMPClauseName(ConflictKind)
              << getOpenMPDirectiveName(DSAStack->getCurrentDirective());
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
}
}
    if (!Type->isAnyPointerType() && Type->isVariablyModifiedType() &&
        isOpenMPTaskingDirective(DSAStack->getCurrentDirective())) {
      Diag(ELoc, diag::err_omp_variably_modified_type_not_supported)
          << getOpenMPClauseName(OMPC_firstprivate) << Type
          << getOpenMPDirectiveName(DSAStack->getCurrentDirective());
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
      continue;
}
    Type = Type.getUnqualifiedType();
    auto VDPrivate = buildVarDecl(*this, ELoc, Type, D->getName(),
                                  D->hasAttrs() ? &D->getAttrs() : nullptr);
    Expr *VDInitRefExpr = nullptr;
    if (Type->isArrayType()) {
      auto VDInit =
          buildVarDecl(*this, RefExpr->getExprLoc(), ElemType, D->getName());
      VDInitRefExpr = buildDeclRefExpr(*this, VDInit, ElemType, ELoc);
      auto Init = DefaultLvalueConversion(VDInitRefExpr).get();
      ElemType = ElemType.getUnqualifiedType();
      auto *VDInitTemp = buildVarDecl(*this, RefExpr->getExprLoc(), ElemType,
                                      ".firstprivate.temp");
      InitializedEntity Entity =
          InitializedEntity::InitializeVariable(VDInitTemp);
      InitializationKind Kind = InitializationKind::CreateCopy(ELoc, ELoc);
      InitializationSequence InitSeq(*this, Entity, Kind, Init);
      ExprResult Result = InitSeq.Perform(*this, Entity, Kind, Init);
      if (Result.isInvalid())
        VDPrivate->setInvalidDecl();
      else
        VDPrivate->setInit(Result.getAs<Expr>());
      Context.Deallocate(VDInitTemp);
} else {
      auto *VDInit = buildVarDecl(*this, RefExpr->getExprLoc(), Type,
                                  ".firstprivate.temp");
      VDInitRefExpr = buildDeclRefExpr(*this, VDInit, RefExpr->getType(),
                                       RefExpr->getExprLoc());
      AddInitializerToDecl(VDPrivate,
                           DefaultLvalueConversion(VDInitRefExpr).get(),
false);
}
    if (VDPrivate->isInvalidDecl()) {
      if (IsImplicitClause) {
        Diag(RefExpr->getExprLoc(),
             diag::note_omp_task_predetermined_firstprivate_here);
}
      continue;
}
    CurContext->addDecl(VDPrivate);
    auto VDPrivateRefExpr = buildDeclRefExpr(
        *this, VDPrivate, RefExpr->getType().getUnqualifiedType(),
        RefExpr->getExprLoc());
    DeclRefExpr *Ref = nullptr;
    if (!VD && !CurContext->isDependentContext()) {
      if (TopDVar.CKind == OMPC_lastprivate)
        Ref = TopDVar.PrivateCopy;
      else {
        Ref = buildCapture(*this, D, SimpleRefExpr, 
true);
        if (!IsOpenMPCapturedDecl(D))
          ExprCaptures.push_back(Ref->getDecl());
}
}
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_firstprivate, Ref);
    Vars.push_back((VD || CurContext->isDependentContext())
                       ? RefExpr->IgnoreParens()
                       : Ref);
    PrivateCopies.push_back(VDPrivateRefExpr);
    Inits.push_back(VDInitRefExpr);
}
  if (Vars.empty())
    return nullptr;
  return OMPFirstprivateClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                                       Vars, PrivateCopies, Inits,
                                       buildPreInits(Context, ExprCaptures));
}
OMPClause *Sema::ActOnOpenMPLastprivateClause(ArrayRef<Expr *> VarList,
                                              SourceLocation StartLoc,
                                              SourceLocation LParenLoc,
                                              SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> SrcExprs;
  SmallVector<Expr *, 8> DstExprs;
  SmallVector<Expr *, 8> AssignmentOps;
  SmallVector<Decl *, 4> ExprCaptures;
  SmallVector<Expr *, 4> ExprPostUpdates;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP lastprivate clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      Vars.push_back(RefExpr);
      SrcExprs.push_back(nullptr);
      DstExprs.push_back(nullptr);
      AssignmentOps.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    if (RequireCompleteType(ELoc, Type,
                            diag::err_omp_lastprivate_incomplete_type))
      continue;
    Type = Type.getNonReferenceType();
    DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
    if (DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_lastprivate &&
        DVar.CKind != OMPC_firstprivate &&
        (DVar.CKind != OMPC_private || DVar.RefExpr != nullptr)) {
      Diag(ELoc, diag::err_omp_wrong_dsa)
          << getOpenMPClauseName(DVar.CKind)
          << getOpenMPClauseName(OMPC_lastprivate);
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    OpenMPDirectiveKind CurrDir = DSAStack->getCurrentDirective();
    DSAStackTy::DSAVarData TopDVar = DVar;
    if (isOpenMPWorksharingDirective(CurrDir) &&
        !isOpenMPParallelDirective(CurrDir) &&
        !isOpenMPTeamsDirective(CurrDir)) {
      DVar = DSAStack->getImplicitDSA(D, true);
      if (DVar.CKind != OMPC_shared) {
        Diag(ELoc, diag::err_omp_required_access)
            << getOpenMPClauseName(OMPC_lastprivate)
            << getOpenMPClauseName(OMPC_shared);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
}
    if (CurrDir == OMPD_distribute) {
      DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
      if (DVar.CKind == OMPC_firstprivate) {
        Diag(ELoc, diag::err_omp_firstprivate_and_lastprivate_in_distribute);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
}
    Type = Context.getBaseElementType(Type).getNonReferenceType();
    auto *SrcVD = buildVarDecl(*this, ERange.getBegin(),
                               Type.getUnqualifiedType(), ".lastprivate.src",
                               D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *PseudoSrcExpr =
        buildDeclRefExpr(*this, SrcVD, Type.getUnqualifiedType(), ELoc);
    auto *DstVD =
        buildVarDecl(*this, ERange.getBegin(), Type, ".lastprivate.dst",
                     D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *PseudoDstExpr = buildDeclRefExpr(*this, DstVD, Type, ELoc);
    auto AssignmentOp = BuildBinOp(
nullptr, ELoc, BO_Assign,
                                   PseudoDstExpr, PseudoSrcExpr);
    if (AssignmentOp.isInvalid())
      continue;
    AssignmentOp = ActOnFinishFullExpr(AssignmentOp.get(), ELoc,
true);
    if (AssignmentOp.isInvalid())
      continue;
    DeclRefExpr *Ref = nullptr;
    if (!VD && !CurContext->isDependentContext()) {
      if (TopDVar.CKind == OMPC_firstprivate)
        Ref = TopDVar.PrivateCopy;
      else {
        Ref = buildCapture(*this, D, SimpleRefExpr, 
false);
        if (!IsOpenMPCapturedDecl(D))
          ExprCaptures.push_back(Ref->getDecl());
}
      if (TopDVar.CKind == OMPC_firstprivate ||
          (!IsOpenMPCapturedDecl(D) &&
           Ref->getDecl()->hasAttr<OMPCaptureNoInitAttr>())) {
        ExprResult RefRes = DefaultLvalueConversion(Ref);
        if (!RefRes.isUsable())
          continue;
        ExprResult PostUpdateRes =
            BuildBinOp(DSAStack->getCurScope(), ELoc, BO_Assign, SimpleRefExpr,
                       RefRes.get());
        if (!PostUpdateRes.isUsable())
          continue;
        ExprPostUpdates.push_back(
            IgnoredValueConversions(PostUpdateRes.get()).get());
}
}
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_lastprivate, Ref);
    Vars.push_back((VD || CurContext->isDependentContext())
                       ? RefExpr->IgnoreParens()
                       : Ref);
    SrcExprs.push_back(PseudoSrcExpr);
    DstExprs.push_back(PseudoDstExpr);
    AssignmentOps.push_back(AssignmentOp.get());
}
  if (Vars.empty())
    return nullptr;
  return OMPLastprivateClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                                      Vars, SrcExprs, DstExprs, AssignmentOps,
                                      buildPreInits(Context, ExprCaptures),
                                      buildPostUpdate(*this, ExprPostUpdates));
}
OMPClause *Sema::ActOnOpenMPSharedClause(ArrayRef<Expr *> VarList,
                                         SourceLocation StartLoc,
                                         SourceLocation LParenLoc,
                                         SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP lastprivate clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      Vars.push_back(RefExpr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    auto *VD = dyn_cast<VarDecl>(D);
    DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
    if (DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_shared &&
        DVar.RefExpr) {
      Diag(ELoc, diag::err_omp_wrong_dsa) << getOpenMPClauseName(DVar.CKind)
                                          << getOpenMPClauseName(OMPC_shared);
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    DeclRefExpr *Ref = nullptr;
    if (!VD && IsOpenMPCapturedDecl(D) && !CurContext->isDependentContext())
      Ref = buildCapture(*this, D, SimpleRefExpr, 
true);
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_shared, Ref);
    Vars.push_back((VD || !Ref || CurContext->isDependentContext())
                       ? RefExpr->IgnoreParens()
                       : Ref);
}
  if (Vars.empty())
    return nullptr;
  return OMPSharedClause::Create(Context, StartLoc, LParenLoc, EndLoc, Vars);
}
namespace {
class DSARefChecker : public StmtVisitor<DSARefChecker, bool> {
  DSAStackTy *Stack;
public:
  bool VisitDeclRefExpr(DeclRefExpr *E) {
    if (VarDecl *VD = dyn_cast<VarDecl>(E->getDecl())) {
      DSAStackTy::DSAVarData DVar = Stack->getTopDSA(VD, false);
      if (DVar.CKind == OMPC_shared && !DVar.RefExpr)
        return false;
      if (DVar.CKind != OMPC_unknown)
        return true;
      DSAStackTy::DSAVarData DVarPrivate = Stack->hasDSA(
          VD, isOpenMPPrivate, [](OpenMPDirectiveKind) -> bool { return true; 
},
          false);
      if (DVarPrivate.CKind != OMPC_unknown)
        return true;
      return false;
}
    return false;
}
  bool VisitStmt(Stmt *S) {
for_loop_talkad7420();
    for (auto Child : S->children()) {
      if (Child && Visit(Child))
        return true;
}
    return false;
}
  explicit DSARefChecker(DSAStackTy *S) : Stack(S) {
}
};
} 
namespace {
class TransformExprToCaptures : public TreeTransform<TransformExprToCaptures> {
  typedef TreeTransform<TransformExprToCaptures> BaseTransform;
  ValueDecl *Field;
  DeclRefExpr *CapturedExpr;
public:
  TransformExprToCaptures(Sema &SemaRef, ValueDecl *FieldDecl)
      : BaseTransform(SemaRef), Field(FieldDecl), CapturedExpr(nullptr) {
}
  ExprResult TransformMemberExpr(MemberExpr *E) {
    if (isa<CXXThisExpr>(E->getBase()->IgnoreParenImpCasts()) &&
        E->getMemberDecl() == Field) {
      CapturedExpr = buildCapture(SemaRef, Field, E, 
false);
      return CapturedExpr;
}
    return BaseTransform::TransformMemberExpr(E);
}
  DeclRefExpr *getCapturedExpr() { return CapturedExpr; 
}
};
} 
template <typename T>
static T filterLookupForUDR(SmallVectorImpl<UnresolvedSet<8>> &Lookups,
                            const llvm::function_ref<T(ValueDecl *)> &Gen) {
for_loop_talkad7420();
  for (auto &Set : Lookups) {
for_loop_talkad7420();
    for (auto *D : Set) {
      if (auto Res = Gen(cast<ValueDecl>(D)))
        return Res;
}
}
  return T();
}
static ExprResult
buildDeclareReductionRef(Sema &SemaRef, SourceLocation Loc, SourceRange Range,
                         Scope *S, CXXScopeSpec &ReductionIdScopeSpec,
                         const DeclarationNameInfo &ReductionId, QualType Ty,
                         CXXCastPath &BasePath, Expr *UnresolvedReduction) {
  if (ReductionIdScopeSpec.isInvalid())
    return ExprError();
  SmallVector<UnresolvedSet<8>, 4> Lookups;
  if (S) {
    LookupResult Lookup(SemaRef, ReductionId, Sema::LookupOMPReductionName);
    Lookup.suppressDiagnostics();
    while (S && SemaRef.LookupParsedName(Lookup, S, &ReductionIdScopeSpec)) {
      auto *D = Lookup.getRepresentativeDecl();
      do {
        S = S->getParent();
} while (S && !S->isDeclScope(D));
      if (S)
        S = S->getParent();
      Lookups.push_back(UnresolvedSet<8>());
      Lookups.back().append(Lookup.begin(), Lookup.end());
      Lookup.clear();
}
} else if (auto *ULE =
                 cast_or_null<UnresolvedLookupExpr>(UnresolvedReduction)) {
    Lookups.push_back(UnresolvedSet<8>());
    Decl *PrevD = nullptr;
for_loop_talkad7420();
    for (auto *D : ULE->decls()) {
      if (D == PrevD)
        Lookups.push_back(UnresolvedSet<8>());
      else if (auto *DRD = cast<OMPDeclareReductionDecl>(D))
        Lookups.back().addDecl(DRD);
      PrevD = D;
}
}
  if (Ty->isDependentType() || Ty->isInstantiationDependentType() ||
      Ty->containsUnexpandedParameterPack() ||
      filterLookupForUDR<bool>(Lookups, [](ValueDecl *D) -> bool {
        return !D->isInvalidDecl() &&
               (D->getType()->isDependentType() ||
                D->getType()->isInstantiationDependentType() ||
                D->getType()->containsUnexpandedParameterPack());
})) {
    UnresolvedSet<8> ResSet;
for_loop_talkad7420();
    for (auto &Set : Lookups) {
      ResSet.append(Set.begin(), Set.end());
      ResSet.addDecl(Set[Set.size() - 1]);
}
    return UnresolvedLookupExpr::Create(
        SemaRef.Context, 
nullptr,
        ReductionIdScopeSpec.getWithLocInContext(SemaRef.Context), ReductionId,
true, 
true, ResSet.begin(), ResSet.end());
}
  if (auto *VD = filterLookupForUDR<ValueDecl *>(
          Lookups, [&SemaRef, Ty](ValueDecl *D) -> ValueDecl * {
            if (!D->isInvalidDecl() &&
                SemaRef.Context.hasSameType(D->getType(), Ty))
              return D;
            return nullptr;
}))
    return SemaRef.BuildDeclRefExpr(VD, Ty, VK_LValue, Loc);
  if (auto *VD = filterLookupForUDR<ValueDecl *>(
          Lookups, [&SemaRef, Ty, Loc](ValueDecl *D) -> ValueDecl * {
            if (!D->isInvalidDecl() &&
                SemaRef.IsDerivedFrom(Loc, Ty, D->getType()) &&
                !Ty.isMoreQualifiedThan(D->getType()))
              return D;
            return nullptr;
})) {
    CXXBasePaths Paths(
true, 
true,
false);
    if (SemaRef.IsDerivedFrom(Loc, Ty, VD->getType(), Paths)) {
      if (!Paths.isAmbiguous(SemaRef.Context.getCanonicalType(
              VD->getType().getUnqualifiedType()))) {
        if (SemaRef.CheckBaseClassAccess(Loc, VD->getType(), Ty, Paths.front(),
0) !=
            Sema::AR_inaccessible) {
          SemaRef.BuildBasePathArray(Paths, BasePath);
          return SemaRef.BuildDeclRefExpr(VD, Ty, VK_LValue, Loc);
}
}
}
}
  if (ReductionIdScopeSpec.isSet()) {
    SemaRef.Diag(Loc, diag::err_omp_not_resolved_reduction_identifier) << Range;
    return ExprError();
}
  return ExprEmpty();
}
OMPClause *Sema::ActOnOpenMPReductionClause(
    ArrayRef<Expr *> VarList, SourceLocation StartLoc, SourceLocation LParenLoc,
    SourceLocation ColonLoc, SourceLocation EndLoc,
    CXXScopeSpec &ReductionIdScopeSpec, const DeclarationNameInfo &ReductionId,
    ArrayRef<Expr *> UnresolvedReductions) {
  auto DN = ReductionId.getName();
  auto OOK = DN.getCXXOverloadedOperator();
  BinaryOperatorKind BOK = BO_Comma;
  switch (OOK) {
  case OO_Plus:
  case OO_Minus:
    BOK = BO_Add;
    break;
  case OO_Star:
    BOK = BO_Mul;
    break;
  case OO_Amp:
    BOK = BO_And;
    break;
  case OO_Pipe:
    BOK = BO_Or;
    break;
  case OO_Caret:
    BOK = BO_Xor;
    break;
  case OO_AmpAmp:
    BOK = BO_LAnd;
    break;
  case OO_PipePipe:
    BOK = BO_LOr;
    break;
  case OO_New:
  case OO_Delete:
  case OO_Array_New:
  case OO_Array_Delete:
  case OO_Slash:
  case OO_Percent:
  case OO_Tilde:
  case OO_Exclaim:
  case OO_Equal:
  case OO_Less:
  case OO_Greater:
  case OO_LessEqual:
  case OO_GreaterEqual:
  case OO_PlusEqual:
  case OO_MinusEqual:
  case OO_StarEqual:
  case OO_SlashEqual:
  case OO_PercentEqual:
  case OO_CaretEqual:
  case OO_AmpEqual:
  case OO_PipeEqual:
  case OO_LessLess:
  case OO_GreaterGreater:
  case OO_LessLessEqual:
  case OO_GreaterGreaterEqual:
  case OO_EqualEqual:
  case OO_ExclaimEqual:
  case OO_PlusPlus:
  case OO_MinusMinus:
  case OO_Comma:
  case OO_ArrowStar:
  case OO_Arrow:
  case OO_Call:
  case OO_Subscript:
  case OO_Conditional:
  case OO_Coawait:
  case NUM_OVERLOADED_OPERATORS:
    llvm_unreachable("Unexpected reduction identifier");
  case OO_None:
    if (auto II = DN.getAsIdentifierInfo()) {
      if (II->isStr("max"))
        BOK = BO_GT;
      else if (II->isStr("min"))
        BOK = BO_LT;
}
    break;
}
  SourceRange ReductionIdRange;
  if (ReductionIdScopeSpec.isValid())
    ReductionIdRange.setBegin(ReductionIdScopeSpec.getBeginLoc());
  ReductionIdRange.setEnd(ReductionId.getEndLoc());
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> Privates;
  SmallVector<Expr *, 8> LHSs;
  SmallVector<Expr *, 8> RHSs;
  SmallVector<Expr *, 8> ReductionOps;
  SmallVector<Decl *, 4> ExprCaptures;
  SmallVector<Expr *, 4> ExprPostUpdates;
  auto IR = UnresolvedReductions.begin(), ER = UnresolvedReductions.end();
  bool FirstIter = true;
for_loop_talkad7420();
  for (auto RefExpr : VarList) {
    assert(RefExpr && "nullptr expr in OpenMP reduction clause.");
    if (!FirstIter && IR != ER)
      ++IR;
    FirstIter = false;
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange,
true);
    if (Res.second) {
      Vars.push_back(RefExpr);
      Privates.push_back(nullptr);
      LHSs.push_back(nullptr);
      RHSs.push_back(nullptr);
      QualType Type = Context.DependentTy;
      CXXCastPath BasePath;
      ExprResult DeclareReductionRef = buildDeclareReductionRef(
          *this, ELoc, ERange, DSAStack->getCurScope(), ReductionIdScopeSpec,
          ReductionId, Type, BasePath, IR == ER ? nullptr : *IR);
      if (CurContext->isDependentContext() &&
          (DeclareReductionRef.isUnset() ||
           isa<UnresolvedLookupExpr>(DeclareReductionRef.get())))
        ReductionOps.push_back(DeclareReductionRef.get());
      else
        ReductionOps.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type;
    auto *ASE = dyn_cast<ArraySubscriptExpr>(RefExpr->IgnoreParens());
    auto *OASE = dyn_cast<OMPArraySectionExpr>(RefExpr->IgnoreParens());
    if (ASE)
      Type = ASE->getType().getNonReferenceType();
    else if (OASE) {
      auto BaseType = OMPArraySectionExpr::getBaseOriginalType(OASE->getBase());
      if (auto *ATy = BaseType->getAsArrayTypeUnsafe())
        Type = ATy->getElementType();
      else
        Type = BaseType->getPointeeType();
      Type = Type.getNonReferenceType();
} else
      Type = Context.getBaseElementType(D->getType().getNonReferenceType());
    auto *VD = dyn_cast<VarDecl>(D);
    if (RequireCompleteType(ELoc, Type,
                            diag::err_omp_reduction_incomplete_type))
      continue;
    if (Type.getNonReferenceType().isConstant(Context)) {
      Diag(ELoc, diag::err_omp_const_reduction_list_item)
          << getOpenMPClauseName(OMPC_reduction) << Type << ERange;
      if (!ASE && !OASE) {
        bool IsDecl = !VD ||
                      VD->isThisDeclarationADefinition(Context) ==
                          VarDecl::DeclarationOnly;
        Diag(D->getLocation(),
             IsDecl ? diag::note_previous_decl : diag::note_defined_here)
            << D;
}
      continue;
}
    if (!ASE && !OASE && VD) {
      VarDecl *VDDef = VD->getDefinition();
      if (VD->getType()->isReferenceType() && VDDef && VDDef->hasInit()) {
        DSARefChecker Check(DSAStack);
        if (Check.Visit(VDDef->getInit())) {
          Diag(ELoc, diag::err_omp_reduction_ref_type_arg) << ERange;
          Diag(VDDef->getLocation(), diag::note_defined_here) << VDDef;
          continue;
}
}
}
    DSAStackTy::DSAVarData DVar;
    DVar = DSAStack->getTopDSA(D, false);
    if (DVar.CKind == OMPC_reduction) {
      Diag(ELoc, diag::err_omp_once_referenced)
          << getOpenMPClauseName(OMPC_reduction);
      if (DVar.RefExpr)
        Diag(DVar.RefExpr->getExprLoc(), diag::note_omp_referenced);
} else if (DVar.CKind != OMPC_unknown) {
      Diag(ELoc, diag::err_omp_wrong_dsa)
          << getOpenMPClauseName(DVar.CKind)
          << getOpenMPClauseName(OMPC_reduction);
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    OpenMPDirectiveKind CurrDir = DSAStack->getCurrentDirective();
    if (isOpenMPWorksharingDirective(CurrDir) &&
        !isOpenMPParallelDirective(CurrDir) &&
        !isOpenMPTeamsDirective(CurrDir)) {
      DVar = DSAStack->getImplicitDSA(D, true);
      if (DVar.CKind != OMPC_shared) {
        Diag(ELoc, diag::err_omp_required_access)
            << getOpenMPClauseName(OMPC_reduction)
            << getOpenMPClauseName(OMPC_shared);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
}
    CXXCastPath BasePath;
    ExprResult DeclareReductionRef = buildDeclareReductionRef(
        *this, ELoc, ERange, DSAStack->getCurScope(), ReductionIdScopeSpec,
        ReductionId, Type, BasePath, IR == ER ? nullptr : *IR);
    if (DeclareReductionRef.isInvalid())
      continue;
    if (CurContext->isDependentContext() &&
        (DeclareReductionRef.isUnset() ||
         isa<UnresolvedLookupExpr>(DeclareReductionRef.get()))) {
      Vars.push_back(RefExpr);
      Privates.push_back(nullptr);
      LHSs.push_back(nullptr);
      RHSs.push_back(nullptr);
      ReductionOps.push_back(DeclareReductionRef.get());
      continue;
}
    if (BOK == BO_Comma && DeclareReductionRef.isUnset()) {
      Diag(ReductionId.getLocStart(),
           diag::err_omp_unknown_reduction_identifier)
          << Type << ReductionIdRange;
      continue;
}
    if (DeclareReductionRef.isUnset()) {
      if ((BOK == BO_GT || BOK == BO_LT) &&
          !(Type->isScalarType() ||
            (getLangOpts().CPlusPlus && Type->isArithmeticType()))) {
        Diag(ELoc, diag::err_omp_clause_not_arithmetic_type_arg)
            << getLangOpts().CPlusPlus;
        if (!ASE && !OASE) {
          bool IsDecl = !VD ||
                        VD->isThisDeclarationADefinition(Context) ==
                            VarDecl::DeclarationOnly;
          Diag(D->getLocation(),
               IsDecl ? diag::note_previous_decl : diag::note_defined_here)
              << D;
}
        continue;
}
      if ((BOK == BO_OrAssign || BOK == BO_AndAssign || BOK == BO_XorAssign) &&
          !getLangOpts().CPlusPlus && Type->isFloatingType()) {
        Diag(ELoc, diag::err_omp_clause_floating_type_arg);
        if (!ASE && !OASE) {
          bool IsDecl = !VD ||
                        VD->isThisDeclarationADefinition(Context) ==
                            VarDecl::DeclarationOnly;
          Diag(D->getLocation(),
               IsDecl ? diag::note_previous_decl : diag::note_defined_here)
              << D;
}
        continue;
}
}
    Type = Type.getNonLValueExprType(Context).getUnqualifiedType();
    auto *LHSVD = buildVarDecl(*this, ELoc, Type, ".reduction.lhs",
                               D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *RHSVD = buildVarDecl(*this, ELoc, Type, D->getName(),
                               D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto PrivateTy = Type;
    if (OASE ||
        (!ASE &&
         D->getType().getNonReferenceType()->isVariablyModifiedType())) {
      PrivateTy = Context.getVariableArrayType(
          Type, new (Context) OpaqueValueExpr(SourceLocation(),
                                              Context.getSizeType(), VK_RValue),
          ArrayType::Normal, 
0, SourceRange());
} else if (!ASE && !OASE &&
               Context.getAsArrayType(D->getType().getNonReferenceType()))
      PrivateTy = D->getType().getNonReferenceType();
    auto *PrivateVD = buildVarDecl(*this, ELoc, PrivateTy, D->getName(),
                                   D->hasAttrs() ? &D->getAttrs() : nullptr);
    Expr *Init = nullptr;
    auto *LHSDRE = buildDeclRefExpr(*this, LHSVD, Type, ELoc);
    auto *RHSDRE = buildDeclRefExpr(*this, RHSVD, Type, ELoc);
    if (DeclareReductionRef.isUsable()) {
      auto *DRDRef = DeclareReductionRef.getAs<DeclRefExpr>();
      auto *DRD = cast<OMPDeclareReductionDecl>(DRDRef->getDecl());
      if (DRD->getInitializer()) {
        Init = DRDRef;
        RHSVD->setInit(DRDRef);
        RHSVD->setInitStyle(VarDecl::CallInit);
}
} else {
      switch (BOK) {
      case BO_Add:
      case BO_Xor:
      case BO_Or:
      case BO_LOr:
        if (Type->isScalarType() || Type->isAnyComplexType())
          Init = ActOnIntegerConstant(ELoc, 
0).get();
        break;
      case BO_Mul:
      case BO_LAnd:
        if (Type->isScalarType() || Type->isAnyComplexType()) {
          Init = ActOnIntegerConstant(ELoc, 
1).get();
}
        break;
      case BO_And: {
        QualType OrigType = Type;
        if (auto *ComplexTy = OrigType->getAs<ComplexType>())
          Type = ComplexTy->getElementType();
        if (Type->isRealFloatingType()) {
          llvm::APFloat InitValue =
              llvm::APFloat::getAllOnesValue(Context.getTypeSize(Type),
true);
          Init = FloatingLiteral::Create(Context, InitValue, 
true,
                                         Type, ELoc);
} else if (Type->isScalarType()) {
          auto Size = Context.getTypeSize(Type);
          QualType IntTy = Context.getIntTypeForBitwidth(Size, 
0);
          llvm::APInt InitValue = llvm::APInt::getAllOnesValue(Size);
          Init = IntegerLiteral::Create(Context, InitValue, IntTy, ELoc);
}
        if (Init && OrigType->isAnyComplexType()) {
          auto *Im = new (Context) ImaginaryLiteral(Init, OrigType);
          Init = CreateBuiltinBinOp(ELoc, BO_Add, Init, Im).get();
}
        Type = OrigType;
        break;
}
      case BO_LT:
      case BO_GT: {
        if (Type->isIntegerType() || Type->isPointerType()) {
          bool IsSigned = Type->hasSignedIntegerRepresentation();
          auto Size = Context.getTypeSize(Type);
          QualType IntTy =
              Context.getIntTypeForBitwidth(Size, 
IsSigned);
          llvm::APInt InitValue =
              (BOK != BO_LT)
                  ? IsSigned ? llvm::APInt::getSignedMinValue(Size)
                             : llvm::APInt::getMinValue(Size)
                  : IsSigned ? llvm::APInt::getSignedMaxValue(Size)
                             : llvm::APInt::getMaxValue(Size);
          Init = IntegerLiteral::Create(Context, InitValue, IntTy, ELoc);
          if (Type->isPointerType()) {
            auto CastExpr = BuildCStyleCastExpr(
                SourceLocation(), Context.getTrivialTypeSourceInfo(Type, ELoc),
                SourceLocation(), Init);
            if (CastExpr.isInvalid())
              continue;
            Init = CastExpr.get();
}
} else if (Type->isRealFloatingType()) {
          llvm::APFloat InitValue = llvm::APFloat::getLargest(
              Context.getFloatTypeSemantics(Type), BOK != BO_LT);
          Init = FloatingLiteral::Create(Context, InitValue, 
true,
                                         Type, ELoc);
}
        break;
}
      case BO_PtrMemD:
      case BO_PtrMemI:
      case BO_MulAssign:
      case BO_Div:
      case BO_Rem:
      case BO_Sub:
      case BO_Shl:
      case BO_Shr:
      case BO_LE:
      case BO_GE:
      case BO_EQ:
      case BO_NE:
      case BO_AndAssign:
      case BO_XorAssign:
      case BO_OrAssign:
      case BO_Assign:
      case BO_AddAssign:
      case BO_SubAssign:
      case BO_DivAssign:
      case BO_RemAssign:
      case BO_ShlAssign:
      case BO_ShrAssign:
      case BO_Comma:
        llvm_unreachable("Unexpected reduction operation");
}
}
    if (Init && DeclareReductionRef.isUnset()) {
      AddInitializerToDecl(RHSVD, Init, 
false);
} else if (!Init)
      ActOnUninitializedDecl(RHSVD);
    if (RHSVD->isInvalidDecl())
      continue;
    if (!RHSVD->hasInit() && DeclareReductionRef.isUnset()) {
      Diag(ELoc, diag::err_omp_reduction_id_not_compatible) << Type
                                                            << ReductionIdRange;
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
      continue;
}
    PrivateVD->setInit(RHSVD->getInit());
    PrivateVD->setInitStyle(RHSVD->getInitStyle());
    auto *PrivateDRE = buildDeclRefExpr(*this, PrivateVD, PrivateTy, ELoc);
    ExprResult ReductionOp;
    if (DeclareReductionRef.isUsable()) {
      QualType RedTy = DeclareReductionRef.get()->getType();
      QualType PtrRedTy = Context.getPointerType(RedTy);
      ExprResult LHS = CreateBuiltinUnaryOp(ELoc, UO_AddrOf, LHSDRE);
      ExprResult RHS = CreateBuiltinUnaryOp(ELoc, UO_AddrOf, RHSDRE);
      if (!BasePath.empty()) {
        LHS = DefaultLvalueConversion(LHS.get());
        RHS = DefaultLvalueConversion(RHS.get());
        LHS = ImplicitCastExpr::Create(Context, PtrRedTy,
                                       CK_UncheckedDerivedToBase, LHS.get(),
                                       &BasePath, LHS.get()->getValueKind());
        RHS = ImplicitCastExpr::Create(Context, PtrRedTy,
                                       CK_UncheckedDerivedToBase, RHS.get(),
                                       &BasePath, RHS.get()->getValueKind());
}
      FunctionProtoType::ExtProtoInfo EPI;
      QualType Params[] = {PtrRedTy, PtrRedTy
};
      QualType FnTy = Context.getFunctionType(Context.VoidTy, Params, EPI);
      auto *OVE = new (Context) OpaqueValueExpr(
          ELoc, Context.getPointerType(FnTy), VK_RValue, OK_Ordinary,
          DefaultLvalueConversion(DeclareReductionRef.get()).get());
      Expr *Args[] = {LHS.get(), RHS.get()
};
      ReductionOp = new (Context)
          CallExpr(Context, OVE, Args, Context.VoidTy, VK_RValue, ELoc);
} else {
      ReductionOp = BuildBinOp(DSAStack->getCurScope(),
                               ReductionId.getLocStart(), BOK, LHSDRE, RHSDRE);
      if (ReductionOp.isUsable()) {
        if (BOK != BO_LT && BOK != BO_GT) {
          ReductionOp =
              BuildBinOp(DSAStack->getCurScope(), ReductionId.getLocStart(),
                         BO_Assign, LHSDRE, ReductionOp.get());
} else {
          auto *ConditionalOp = new (Context) ConditionalOperator(
              ReductionOp.get(), SourceLocation(), LHSDRE, SourceLocation(),
              RHSDRE, Type, VK_LValue, OK_Ordinary);
          ReductionOp =
              BuildBinOp(DSAStack->getCurScope(), ReductionId.getLocStart(),
                         BO_Assign, LHSDRE, ConditionalOp);
}
        ReductionOp = ActOnFinishFullExpr(ReductionOp.get());
}
      if (ReductionOp.isInvalid())
        continue;
}
    DeclRefExpr *Ref = nullptr;
    Expr *VarsExpr = RefExpr->IgnoreParens();
    if (!VD && !CurContext->isDependentContext()) {
      if (ASE || OASE) {
        TransformExprToCaptures RebuildToCapture(*this, D);
        VarsExpr =
            RebuildToCapture.TransformExpr(RefExpr->IgnoreParens()).get();
        Ref = RebuildToCapture.getCapturedExpr();
} else {
        VarsExpr = Ref =
            buildCapture(*this, D, SimpleRefExpr, 
false);
}
      if (!IsOpenMPCapturedDecl(D)) {
        ExprCaptures.push_back(Ref->getDecl());
        if (Ref->getDecl()->hasAttr<OMPCaptureNoInitAttr>()) {
          ExprResult RefRes = DefaultLvalueConversion(Ref);
          if (!RefRes.isUsable())
            continue;
          ExprResult PostUpdateRes =
              BuildBinOp(DSAStack->getCurScope(), ELoc, BO_Assign,
                         SimpleRefExpr, RefRes.get());
          if (!PostUpdateRes.isUsable())
            continue;
          ExprPostUpdates.push_back(
              IgnoredValueConversions(PostUpdateRes.get()).get());
}
}
}
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_reduction, Ref);
    Vars.push_back(VarsExpr);
    Privates.push_back(PrivateDRE);
    LHSs.push_back(LHSDRE);
    RHSs.push_back(RHSDRE);
    ReductionOps.push_back(ReductionOp.get());
}
  if (Vars.empty())
    return nullptr;
  return OMPReductionClause::Create(
      Context, StartLoc, LParenLoc, ColonLoc, EndLoc, Vars,
      ReductionIdScopeSpec.getWithLocInContext(Context), ReductionId, Privates,
      LHSs, RHSs, ReductionOps, buildPreInits(Context, ExprCaptures),
      buildPostUpdate(*this, ExprPostUpdates));
}
bool Sema::CheckOpenMPLinearModifier(OpenMPLinearClauseKind LinKind,
                                     SourceLocation LinLoc) {
  if ((!LangOpts.CPlusPlus && LinKind != OMPC_LINEAR_val) ||
      LinKind == OMPC_LINEAR_unknown) {
    Diag(LinLoc, diag::err_omp_wrong_linear_modifier) << LangOpts.CPlusPlus;
    return true;
}
  return false;
}
bool Sema::CheckOpenMPLinearDecl(ValueDecl *D, SourceLocation ELoc,
                                 OpenMPLinearClauseKind LinKind,
                                 QualType Type) {
  auto *VD = dyn_cast_or_null<VarDecl>(D);
  if (RequireCompleteType(ELoc, Type, diag::err_omp_linear_incomplete_type))
    return true;
  if ((LinKind == OMPC_LINEAR_uval || LinKind == OMPC_LINEAR_ref) &&
      !Type->isReferenceType()) {
    Diag(ELoc, diag::err_omp_wrong_linear_modifier_non_reference)
        << Type << getOpenMPSimpleClauseTypeName(OMPC_linear, LinKind);
    return true;
}
  Type = Type.getNonReferenceType();
  if (Type.isConstant(Context)) {
    Diag(ELoc, diag::err_omp_const_variable)
        << getOpenMPClauseName(OMPC_linear);
    if (D) {
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
}
    return true;
}
  Type = Type.getUnqualifiedType().getCanonicalType();
  const auto *Ty = Type.getTypePtrOrNull();
  if (!Ty || (!Ty->isDependentType() && !Ty->isIntegralType(Context) &&
              !Ty->isPointerType())) {
    Diag(ELoc, diag::err_omp_linear_expected_int_or_ptr) << Type;
    if (D) {
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
}
    return true;
}
  return false;
}
OMPClause *Sema::ActOnOpenMPLinearClause(
    ArrayRef<Expr *> VarList, Expr *Step, SourceLocation StartLoc,
    SourceLocation LParenLoc, OpenMPLinearClauseKind LinKind,
    SourceLocation LinLoc, SourceLocation ColonLoc, SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> Privates;
  SmallVector<Expr *, 8> Inits;
  SmallVector<Decl *, 4> ExprCaptures;
  SmallVector<Expr *, 4> ExprPostUpdates;
  if (CheckOpenMPLinearModifier(LinKind, LinLoc))
    LinKind = OMPC_LINEAR_val;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP linear clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange,
false);
    if (Res.second) {
      Vars.push_back(RefExpr);
      Privates.push_back(nullptr);
      Inits.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    DSAStackTy::DSAVarData DVar = DSAStack->getTopDSA(D, false);
    if (DVar.RefExpr) {
      Diag(ELoc, diag::err_omp_wrong_dsa) << getOpenMPClauseName(DVar.CKind)
                                          << getOpenMPClauseName(OMPC_linear);
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    if (CheckOpenMPLinearDecl(D, ELoc, LinKind, Type))
      continue;
    Type = Type.getNonReferenceType().getUnqualifiedType().getCanonicalType();
    auto *Private = buildVarDecl(*this, ELoc, Type, D->getName(),
                                 D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *PrivateRef = buildDeclRefExpr(*this, Private, Type, ELoc);
    VarDecl *Init = buildVarDecl(*this, ELoc, Type, ".linear.start");
    Expr *InitExpr;
    DeclRefExpr *Ref = nullptr;
    if (!VD && !CurContext->isDependentContext()) {
      Ref = buildCapture(*this, D, SimpleRefExpr, 
false);
      if (!IsOpenMPCapturedDecl(D)) {
        ExprCaptures.push_back(Ref->getDecl());
        if (Ref->getDecl()->hasAttr<OMPCaptureNoInitAttr>()) {
          ExprResult RefRes = DefaultLvalueConversion(Ref);
          if (!RefRes.isUsable())
            continue;
          ExprResult PostUpdateRes =
              BuildBinOp(DSAStack->getCurScope(), ELoc, BO_Assign,
                         SimpleRefExpr, RefRes.get());
          if (!PostUpdateRes.isUsable())
            continue;
          ExprPostUpdates.push_back(
              IgnoredValueConversions(PostUpdateRes.get()).get());
}
}
}
    if (LinKind == OMPC_LINEAR_uval)
      InitExpr = VD ? VD->getInit() : SimpleRefExpr;
    else
      InitExpr = VD ? SimpleRefExpr : Ref;
    AddInitializerToDecl(Init, DefaultLvalueConversion(InitExpr).get(),
false);
    auto InitRef = buildDeclRefExpr(*this, Init, Type, ELoc);
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_linear, Ref);
    Vars.push_back((VD || CurContext->isDependentContext())
                       ? RefExpr->IgnoreParens()
                       : Ref);
    Privates.push_back(PrivateRef);
    Inits.push_back(InitRef);
}
  if (Vars.empty())
    return nullptr;
  Expr *StepExpr = Step;
  Expr *CalcStepExpr = nullptr;
  if (Step && !Step->isValueDependent() && !Step->isTypeDependent() &&
      !Step->isInstantiationDependent() &&
      !Step->containsUnexpandedParameterPack()) {
    SourceLocation StepLoc = Step->getLocStart();
    ExprResult Val = PerformOpenMPImplicitIntegerConversion(StepLoc, Step);
    if (Val.isInvalid())
      return nullptr;
    StepExpr = Val.get();
    VarDecl *SaveVar =
        buildVarDecl(*this, StepLoc, StepExpr->getType(), ".linear.step");
    ExprResult SaveRef =
        buildDeclRefExpr(*this, SaveVar, StepExpr->getType(), StepLoc);
    ExprResult CalcStep =
        BuildBinOp(CurScope, StepLoc, BO_Assign, SaveRef.get(), StepExpr);
    CalcStep = ActOnFinishFullExpr(CalcStep.get());
    llvm::APSInt Result;
    bool IsConstant = StepExpr->isIntegerConstantExpr(Result, Context);
    if (IsConstant && !Result.isNegative() && !Result.isStrictlyPositive())
      Diag(StepLoc, diag::warn_omp_linear_step_zero) << Vars[0]
                                                     << (Vars.size() > 1);
    if (!IsConstant && CalcStep.isUsable()) {
      CalcStepExpr = CalcStep.get();
}
}
  return OMPLinearClause::Create(Context, StartLoc, LParenLoc, LinKind, LinLoc,
                                 ColonLoc, EndLoc, Vars, Privates, Inits,
                                 StepExpr, CalcStepExpr,
                                 buildPreInits(Context, ExprCaptures),
                                 buildPostUpdate(*this, ExprPostUpdates));
}
static bool FinishOpenMPLinearClause(OMPLinearClause &Clause, DeclRefExpr *IV,
                                     Expr *NumIterations, Sema &SemaRef,
                                     Scope *S, DSAStackTy *Stack) {
  SmallVector<Expr *, 8> Updates;
  SmallVector<Expr *, 8> Finals;
  Expr *Step = Clause.getStep();
  Expr *CalcStep = Clause.getCalcStep();
  if (Step == nullptr)
    Step = SemaRef.ActOnIntegerConstant(SourceLocation(), 1).get();
  else if (CalcStep) {
    Step = cast<BinaryOperator>(CalcStep)->getLHS();
}
  bool HasErrors = false;
  auto CurInit = Clause.inits().begin();
  auto CurPrivate = Clause.privates().begin();
  auto LinKind = Clause.getModifier();
for_loop_talkad7420();
  for (auto &RefExpr : Clause.varlists()) {
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(SemaRef, SimpleRefExpr, ELoc, ERange,
false);
    ValueDecl *D = Res.first;
    if (Res.second || !D) {
      Updates.push_back(nullptr);
      Finals.push_back(nullptr);
      HasErrors = true;
      continue;
}
    if (auto *CED = dyn_cast<OMPCapturedExprDecl>(D)) {
      D = cast<MemberExpr>(CED->getInit()->IgnoreParenImpCasts())
              ->getMemberDecl();
}
    auto &&Info = Stack->isLoopControlVariable(D);
    Expr *InitExpr = *CurInit;
    auto *DE = cast<DeclRefExpr>(SimpleRefExpr);
    Expr *CapturedRef;
    if (LinKind == OMPC_LINEAR_uval)
      CapturedRef = cast<VarDecl>(DE->getDecl())->getInit();
    else
      CapturedRef =
          buildDeclRefExpr(SemaRef, cast<VarDecl>(DE->getDecl()),
                           DE->getType().getUnqualifiedType(), DE->getExprLoc(),
true);
    ExprResult Update;
    if (!Info.first) {
      Update =
          BuildCounterUpdate(SemaRef, S, RefExpr->getExprLoc(), *CurPrivate,
                             InitExpr, IV, Step, 
 false);
} else
      Update = *CurPrivate;
    Update = SemaRef.ActOnFinishFullExpr(Update.get(), DE->getLocStart(),
true);
    ExprResult Final;
    if (!Info.first) {
      Final = BuildCounterUpdate(SemaRef, S, RefExpr->getExprLoc(), CapturedRef,
                                 InitExpr, NumIterations, Step,
 false);
} else
      Final = *CurPrivate;
    Final = SemaRef.ActOnFinishFullExpr(Final.get(), DE->getLocStart(),
true);
    if (!Update.isUsable() || !Final.isUsable()) {
      Updates.push_back(nullptr);
      Finals.push_back(nullptr);
      HasErrors = true;
} else {
      Updates.push_back(Update.get());
      Finals.push_back(Final.get());
}
    ++CurInit;
    ++CurPrivate;
}
  Clause.setUpdates(Updates);
  Clause.setFinals(Finals);
  return HasErrors;
}
OMPClause *Sema::ActOnOpenMPAlignedClause(
    ArrayRef<Expr *> VarList, Expr *Alignment, SourceLocation StartLoc,
    SourceLocation LParenLoc, SourceLocation ColonLoc, SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP linear clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange,
false);
    if (Res.second) {
      Vars.push_back(RefExpr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType QType = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    QType = QType.getNonReferenceType().getUnqualifiedType().getCanonicalType();
    const Type *Ty = QType.getTypePtrOrNull();
    if (!Ty || (!Ty->isArrayType() && !Ty->isPointerType())) {
      Diag(ELoc, diag::err_omp_aligned_expected_array_or_ptr)
          << QType << getLangOpts().CPlusPlus << ERange;
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
      continue;
}
    if (Expr *PrevRef = DSAStack->addUniqueAligned(D, SimpleRefExpr)) {
      Diag(ELoc, diag::err_omp_aligned_twice) << 0 << ERange;
      Diag(PrevRef->getExprLoc(), diag::note_omp_explicit_dsa)
          << getOpenMPClauseName(OMPC_aligned);
      continue;
}
    DeclRefExpr *Ref = nullptr;
    if (!VD && IsOpenMPCapturedDecl(D))
      Ref = buildCapture(*this, D, SimpleRefExpr, 
true);
    Vars.push_back(DefaultFunctionArrayConversion(
                       (VD || !Ref) ? RefExpr->IgnoreParens() : Ref)
                       .get());
}
  if (Alignment != nullptr) {
    ExprResult AlignResult =
        VerifyPositiveIntegerConstantInClause(Alignment, OMPC_aligned);
    if (AlignResult.isInvalid())
      return nullptr;
    Alignment = AlignResult.get();
}
  if (Vars.empty())
    return nullptr;
  return OMPAlignedClause::Create(Context, StartLoc, LParenLoc, ColonLoc,
                                  EndLoc, Vars, Alignment);
}
OMPClause *Sema::ActOnOpenMPCopyinClause(ArrayRef<Expr *> VarList,
                                         SourceLocation StartLoc,
                                         SourceLocation LParenLoc,
                                         SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> SrcExprs;
  SmallVector<Expr *, 8> DstExprs;
  SmallVector<Expr *, 8> AssignmentOps;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP copyin clause.");
    if (isa<DependentScopeDeclRefExpr>(RefExpr)) {
      Vars.push_back(RefExpr);
      SrcExprs.push_back(nullptr);
      DstExprs.push_back(nullptr);
      AssignmentOps.push_back(nullptr);
      continue;
}
    SourceLocation ELoc = RefExpr->getExprLoc();
    DeclRefExpr *DE = dyn_cast<DeclRefExpr>(RefExpr);
    if (!DE || !isa<VarDecl>(DE->getDecl())) {
      Diag(ELoc, diag::err_omp_expected_var_name_member_expr)
          << 0 << RefExpr->getSourceRange();
      continue;
}
    Decl *D = DE->getDecl();
    VarDecl *VD = cast<VarDecl>(D);
    QualType Type = VD->getType();
    if (Type->isDependentType() || Type->isInstantiationDependentType()) {
      Vars.push_back(DE);
      SrcExprs.push_back(nullptr);
      DstExprs.push_back(nullptr);
      AssignmentOps.push_back(nullptr);
      continue;
}
    if (!DSAStack->isThreadPrivate(VD)) {
      Diag(ELoc, diag::err_omp_required_access)
          << getOpenMPClauseName(OMPC_copyin)
          << getOpenMPDirectiveName(OMPD_threadprivate);
      continue;
}
    auto ElemType = Context.getBaseElementType(Type).getNonReferenceType();
    auto *SrcVD =
        buildVarDecl(*this, DE->getLocStart(), ElemType.getUnqualifiedType(),
                     ".copyin.src", VD->hasAttrs() ? &VD->getAttrs() : nullptr);
    auto *PseudoSrcExpr = buildDeclRefExpr(
        *this, SrcVD, ElemType.getUnqualifiedType(), DE->getExprLoc());
    auto *DstVD =
        buildVarDecl(*this, DE->getLocStart(), ElemType, ".copyin.dst",
                     VD->hasAttrs() ? &VD->getAttrs() : nullptr);
    auto *PseudoDstExpr =
        buildDeclRefExpr(*this, DstVD, ElemType, DE->getExprLoc());
    auto AssignmentOp = BuildBinOp(
nullptr, DE->getExprLoc(), BO_Assign,
                                   PseudoDstExpr, PseudoSrcExpr);
    if (AssignmentOp.isInvalid())
      continue;
    AssignmentOp = ActOnFinishFullExpr(AssignmentOp.get(), DE->getExprLoc(),
true);
    if (AssignmentOp.isInvalid())
      continue;
    DSAStack->addDSA(VD, DE, OMPC_copyin);
    Vars.push_back(DE);
    SrcExprs.push_back(PseudoSrcExpr);
    DstExprs.push_back(PseudoDstExpr);
    AssignmentOps.push_back(AssignmentOp.get());
}
  if (Vars.empty())
    return nullptr;
  return OMPCopyinClause::Create(Context, StartLoc, LParenLoc, EndLoc, Vars,
                                 SrcExprs, DstExprs, AssignmentOps);
}
OMPClause *Sema::ActOnOpenMPCopyprivateClause(ArrayRef<Expr *> VarList,
                                              SourceLocation StartLoc,
                                              SourceLocation LParenLoc,
                                              SourceLocation EndLoc) {
  SmallVector<Expr *, 8> Vars;
  SmallVector<Expr *, 8> SrcExprs;
  SmallVector<Expr *, 8> DstExprs;
  SmallVector<Expr *, 8> AssignmentOps;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP linear clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange,
false);
    if (Res.second) {
      Vars.push_back(RefExpr);
      SrcExprs.push_back(nullptr);
      DstExprs.push_back(nullptr);
      AssignmentOps.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    auto *VD = dyn_cast<VarDecl>(D);
    if (!VD || !DSAStack->isThreadPrivate(VD)) {
      auto DVar = DSAStack->getTopDSA(D, false);
      if (DVar.CKind != OMPC_unknown && DVar.CKind != OMPC_copyprivate &&
          DVar.RefExpr) {
        Diag(ELoc, diag::err_omp_wrong_dsa)
            << getOpenMPClauseName(DVar.CKind)
            << getOpenMPClauseName(OMPC_copyprivate);
        ReportOriginalDSA(*this, DSAStack, D, DVar);
        continue;
}
      if (DVar.CKind == OMPC_unknown) {
        DVar = DSAStack->getImplicitDSA(D, false);
        if (DVar.CKind == OMPC_shared) {
          Diag(ELoc, diag::err_omp_required_access)
              << getOpenMPClauseName(OMPC_copyprivate)
              << "threadprivate or private in the enclosing context";
          ReportOriginalDSA(*this, DSAStack, D, DVar);
          continue;
}
}
}
    if (!Type->isAnyPointerType() && Type->isVariablyModifiedType()) {
      Diag(ELoc, diag::err_omp_variably_modified_type_not_supported)
          << getOpenMPClauseName(OMPC_copyprivate) << Type
          << getOpenMPDirectiveName(DSAStack->getCurrentDirective());
      bool IsDecl =
          !VD ||
          VD->isThisDeclarationADefinition(Context) == VarDecl::DeclarationOnly;
      Diag(D->getLocation(),
           IsDecl ? diag::note_previous_decl : diag::note_defined_here)
          << D;
      continue;
}
    Type = Context.getBaseElementType(Type.getNonReferenceType())
               .getUnqualifiedType();
    auto *SrcVD =
        buildVarDecl(*this, RefExpr->getLocStart(), Type, ".copyprivate.src",
                     D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *PseudoSrcExpr = buildDeclRefExpr(*this, SrcVD, Type, ELoc);
    auto *DstVD =
        buildVarDecl(*this, RefExpr->getLocStart(), Type, ".copyprivate.dst",
                     D->hasAttrs() ? &D->getAttrs() : nullptr);
    auto *PseudoDstExpr = buildDeclRefExpr(*this, DstVD, Type, ELoc);
    auto AssignmentOp = BuildBinOp(DSAStack->getCurScope(), ELoc, BO_Assign,
                                   PseudoDstExpr, PseudoSrcExpr);
    if (AssignmentOp.isInvalid())
      continue;
    AssignmentOp = ActOnFinishFullExpr(AssignmentOp.get(), ELoc,
true);
    if (AssignmentOp.isInvalid())
      continue;
    assert(VD || IsOpenMPCapturedDecl(D));
    Vars.push_back(
        VD ? RefExpr->IgnoreParens()
           : buildCapture(*this, D, SimpleRefExpr, 
false));
    SrcExprs.push_back(PseudoSrcExpr);
    DstExprs.push_back(PseudoDstExpr);
    AssignmentOps.push_back(AssignmentOp.get());
}
  if (Vars.empty())
    return nullptr;
  return OMPCopyprivateClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                                      Vars, SrcExprs, DstExprs, AssignmentOps);
}
OMPClause *Sema::ActOnOpenMPFlushClause(ArrayRef<Expr *> VarList,
                                        SourceLocation StartLoc,
                                        SourceLocation LParenLoc,
                                        SourceLocation EndLoc) {
  if (VarList.empty())
    return nullptr;
  return OMPFlushClause::Create(Context, StartLoc, LParenLoc, EndLoc, VarList);
}
OMPClause *Sema::ActOnOpenMPIncClause(ArrayRef<Expr *> VarList,
                                        SourceLocation StartLoc,
                                        SourceLocation LParenLoc,
                                        SourceLocation EndLoc) {
  if (VarList.empty())
    return nullptr;
  return OMPIncClause::Create(Context, StartLoc, LParenLoc, EndLoc, VarList);
}
OMPClause *
Sema::ActOnOpenMPDependClause(OpenMPDependClauseKind DepKind,
                              SourceLocation DepLoc, SourceLocation ColonLoc,
                              ArrayRef<Expr *> VarList, SourceLocation StartLoc,
                              SourceLocation LParenLoc, SourceLocation EndLoc) {
  if (DSAStack->getCurrentDirective() == OMPD_ordered &&
      DepKind != OMPC_DEPEND_source && DepKind != OMPC_DEPEND_sink) {
    Diag(DepLoc, diag::err_omp_unexpected_clause_value)
        << "'source' or 'sink'" << getOpenMPClauseName(OMPC_depend);
    return nullptr;
}
  if (DSAStack->getCurrentDirective() != OMPD_ordered &&
      (DepKind == OMPC_DEPEND_unknown || DepKind == OMPC_DEPEND_source ||
       DepKind == OMPC_DEPEND_sink)) {
    unsigned Except[] = {OMPC_DEPEND_source, OMPC_DEPEND_sink
};
    Diag(DepLoc, diag::err_omp_unexpected_clause_value)
        << getListOfPossibleValues(OMPC_depend, 
0,
OMPC_DEPEND_unknown, Except)
        << getOpenMPClauseName(OMPC_depend);
    return nullptr;
}
  SmallVector<Expr *, 8> Vars;
  DSAStackTy::OperatorOffsetTy OpsOffs;
  llvm::APSInt DepCounter(
32);
  llvm::APSInt TotalDepCount(
32);
  if (DepKind == OMPC_DEPEND_sink) {
    if (auto *OrderedCountExpr = DSAStack->getParentOrderedRegionParam()) {
      TotalDepCount = OrderedCountExpr->EvaluateKnownConstInt(Context);
      TotalDepCount.setIsUnsigned(
true);
}
}
  if ((DepKind != OMPC_DEPEND_sink && DepKind != OMPC_DEPEND_source) ||
      DSAStack->getParentOrderedRegionParam()) {
for_loop_talkad7420();
    for (auto &RefExpr : VarList) {
      assert(RefExpr && "NULL expr in OpenMP shared clause.");
      if (isa<DependentScopeDeclRefExpr>(RefExpr)) {
        Vars.push_back(RefExpr);
        continue;
}
      SourceLocation ELoc = RefExpr->getExprLoc();
      auto *SimpleExpr = RefExpr->IgnoreParenCasts();
      if (DepKind == OMPC_DEPEND_sink) {
        if (DepCounter >= TotalDepCount) {
          Diag(ELoc, diag::err_omp_depend_sink_unexpected_expr);
          continue;
}
        ++DepCounter;
        if (CurContext->isDependentContext()) {
          Vars.push_back(RefExpr);
          continue;
}
        SimpleExpr = SimpleExpr->IgnoreImplicit();
        OverloadedOperatorKind OOK = OO_None;
        SourceLocation OOLoc;
        Expr *LHS = SimpleExpr;
        Expr *RHS = nullptr;
        if (auto *BO = dyn_cast<BinaryOperator>(SimpleExpr)) {
          OOK = BinaryOperator::getOverloadedOperator(BO->getOpcode());
          OOLoc = BO->getOperatorLoc();
          LHS = BO->getLHS()->IgnoreParenImpCasts();
          RHS = BO->getRHS()->IgnoreParenImpCasts();
} else if (auto *OCE = dyn_cast<CXXOperatorCallExpr>(SimpleExpr)) {
          OOK = OCE->getOperator();
          OOLoc = OCE->getOperatorLoc();
          LHS = OCE->getArg(
0)->IgnoreParenImpCasts();
          RHS = OCE->getArg(
1)->IgnoreParenImpCasts();
} else if (auto *MCE = dyn_cast<CXXMemberCallExpr>(SimpleExpr)) {
          OOK = MCE->getMethodDecl()
                    ->getNameInfo()
                    .getName()
                    .getCXXOverloadedOperator();
          OOLoc = MCE->getCallee()->getExprLoc();
          LHS = MCE->getImplicitObjectArgument()->IgnoreParenImpCasts();
          RHS = MCE->getArg(
0)->IgnoreParenImpCasts();
}
        SourceLocation ELoc;
        SourceRange ERange;
        auto Res = getPrivateItem(*this, LHS, ELoc, ERange,
false);
        if (Res.second) {
          Vars.push_back(RefExpr);
}
        ValueDecl *D = Res.first;
        if (!D)
          continue;
        if (OOK != OO_Plus && OOK != OO_Minus && (RHS || OOK != OO_None)) {
          Diag(OOLoc, diag::err_omp_depend_sink_expected_plus_minus);
          continue;
}
        if (RHS) {
          ExprResult RHSRes = VerifyPositiveIntegerConstantInClause(
              RHS, OMPC_depend, 
false);
          if (RHSRes.isInvalid())
            continue;
}
        if (!CurContext->isDependentContext() &&
            DSAStack->getParentOrderedRegionParam() &&
            DepCounter != DSAStack->isParentLoopControlVariable(D).first) {
          Diag(ELoc, diag::err_omp_depend_sink_expected_loop_iteration)
              << DSAStack->getParentLoopControlVariable(
                     DepCounter.getZExtValue());
          continue;
}
        OpsOffs.push_back({RHS, OOK
});
} else {
        auto *DE = dyn_cast<DeclRefExpr>(SimpleExpr);
        auto *ASE = dyn_cast<ArraySubscriptExpr>(SimpleExpr);
        auto *OASE = dyn_cast<OMPArraySectionExpr>(SimpleExpr);
        if (!RefExpr->IgnoreParenImpCasts()->isLValue() ||
            (!ASE && !DE && !OASE) || (DE && !isa<VarDecl>(DE->getDecl())) ||
            (ASE &&
             !ASE->getBase()
                  ->getType()
                  .getNonReferenceType()
                  ->isPointerType() &&
             !ASE->getBase()->getType().getNonReferenceType()->isArrayType())) {
          Diag(ELoc, diag::err_omp_expected_var_name_member_expr_or_array_item)
              << 0 << RefExpr->getSourceRange();
          continue;
}
}
      Vars.push_back(RefExpr->IgnoreParenImpCasts());
}
    if (!CurContext->isDependentContext() && DepKind == OMPC_DEPEND_sink &&
        TotalDepCount > VarList.size() &&
        DSAStack->getParentOrderedRegionParam()) {
      Diag(EndLoc, diag::err_omp_depend_sink_expected_loop_iteration)
          << DSAStack->getParentLoopControlVariable(VarList.size() + 1);
}
    if (DepKind != OMPC_DEPEND_source && DepKind != OMPC_DEPEND_sink &&
        Vars.empty())
      return nullptr;
}
  auto *C = OMPDependClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                                    DepKind, DepLoc, ColonLoc, Vars);
  if (DepKind == OMPC_DEPEND_sink || DepKind == OMPC_DEPEND_source)
    DSAStack->addDoacrossDependClause(C, OpsOffs);
  return C;
}
OMPClause *Sema::ActOnOpenMPDeviceClause(Expr *Device, SourceLocation StartLoc,
                                         SourceLocation LParenLoc,
                                         SourceLocation EndLoc) {
  Expr *ValExpr = Device;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_device,
false))
    return nullptr;
  return new (Context) OMPDeviceClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
static bool IsCXXRecordForMappable(Sema &SemaRef, SourceLocation Loc,
                                   DSAStackTy *Stack, CXXRecordDecl *RD) {
  if (!RD || RD->isInvalidDecl())
    return true;
  auto QTy = SemaRef.Context.getRecordType(RD);
  if (RD->isDynamicClass()) {
    SemaRef.Diag(Loc, diag::err_omp_not_mappable_type) << QTy;
    SemaRef.Diag(RD->getLocation(), diag::note_omp_polymorphic_in_target);
    return false;
}
  auto *DC = RD;
  bool IsCorrect = true;
for_loop_talkad7420();
  for (auto *I : DC->decls()) {
    if (I) {
      if (auto *MD = dyn_cast<CXXMethodDecl>(I)) {
        if (MD->isStatic()) {
          SemaRef.Diag(Loc, diag::err_omp_not_mappable_type) << QTy;
          SemaRef.Diag(MD->getLocation(),
                       diag::note_omp_static_member_in_target);
          IsCorrect = false;
}
} else if (auto *VD = dyn_cast<VarDecl>(I)) {
        if (VD->isStaticDataMember()) {
          SemaRef.Diag(Loc, diag::err_omp_not_mappable_type) << QTy;
          SemaRef.Diag(VD->getLocation(),
                       diag::note_omp_static_member_in_target);
          IsCorrect = false;
}
}
}
}
for_loop_talkad7420();
  for (auto &I : RD->bases()) {
    if (!IsCXXRecordForMappable(SemaRef, I.getLocStart(), Stack,
                                I.getType()->getAsCXXRecordDecl()))
      IsCorrect = false;
}
  return IsCorrect;
}
static bool CheckTypeMappable(SourceLocation SL, SourceRange SR, Sema &SemaRef,
                              DSAStackTy *Stack, QualType QTy) {
  NamedDecl *ND;
  if (QTy->isIncompleteType(&ND)) {
    SemaRef.Diag(SL, diag::err_incomplete_type) << QTy << SR;
    return false;
} else if (CXXRecordDecl *RD = dyn_cast_or_null<CXXRecordDecl>(ND)) {
    if (!RD->isInvalidDecl() && !IsCXXRecordForMappable(SemaRef, SL, Stack, RD))
      return false;
}
  return true;
}
static bool CheckArrayExpressionDoesNotReferToWholeSize(Sema &SemaRef,
                                                        const Expr *E,
                                                        QualType BaseQTy) {
  auto *OASE = dyn_cast<OMPArraySectionExpr>(E);
  if (isa<ArraySubscriptExpr>(E) || (OASE && OASE->getColonLoc().isInvalid())) {
    if (auto *ATy = dyn_cast<ConstantArrayType>(BaseQTy.getTypePtr()))
      return ATy->getSize().getSExtValue() != 1;
    return false;
}
  assert(OASE && "Expecting array section if not an array subscript.");
  auto *LowerBound = OASE->getLowerBound();
  auto *Length = OASE->getLength();
  if (LowerBound) {
    llvm::APSInt ConstLowerBound;
    if (!LowerBound->EvaluateAsInt(ConstLowerBound, SemaRef.getASTContext()))
      return false; 
    if (ConstLowerBound.getSExtValue())
      return true;
}
  if (!Length)
    return false;
  if (BaseQTy->isPointerType())
    return false;
  auto *CATy = dyn_cast<ConstantArrayType>(BaseQTy.getTypePtr());
  if (!CATy)
    return false;
  llvm::APSInt ConstLength;
  if (!Length->EvaluateAsInt(ConstLength, SemaRef.getASTContext()))
    return false; 
  return CATy->getSize().getSExtValue() != ConstLength.getSExtValue();
}
static bool CheckArrayExpressionDoesNotReferToUnitySize(Sema &SemaRef,
                                                        const Expr *E,
                                                        QualType BaseQTy) {
  auto *OASE = dyn_cast<OMPArraySectionExpr>(E);
  if (isa<ArraySubscriptExpr>(E) || (OASE && OASE->getColonLoc().isInvalid()))
    return false;
  assert(OASE && "Expecting array section if not an array subscript.");
  auto *Length = OASE->getLength();
  if (!Length) {
    if (auto *ATy = dyn_cast<ConstantArrayType>(BaseQTy.getTypePtr()))
      return ATy->getSize().getSExtValue() != 1;
    return false;
}
  llvm::APSInt ConstLength;
  if (!Length->EvaluateAsInt(ConstLength, SemaRef.getASTContext()))
    return false; 
  return ConstLength.getSExtValue() != 1;
}
static Expr *CheckMapClauseExpressionBase(
    Sema &SemaRef, Expr *E,
    OMPClauseMappableExprCommon::MappableExprComponentList &CurComponents,
    OpenMPClauseKind CKind) {
  SourceLocation ELoc = E->getExprLoc();
  SourceRange ERange = E->getSourceRange();
  Expr *RelevantExpr = nullptr;
  bool AllowUnitySizeArraySection = true;
  bool AllowWholeSizeArraySection = true;
  while (!RelevantExpr) {
    E = E->IgnoreParenImpCasts();
    if (auto *CurE = dyn_cast<DeclRefExpr>(E)) {
      if (!isa<VarDecl>(CurE->getDecl()))
        break;
      RelevantExpr = CurE;
      AllowUnitySizeArraySection = false;
      AllowWholeSizeArraySection = false;
      CurComponents.push_back(OMPClauseMappableExprCommon::MappableComponent(
          CurE, CurE->getDecl()));
      continue;
}
    if (auto *CurE = dyn_cast<MemberExpr>(E)) {
      auto *BaseE = CurE->getBase()->IgnoreParenImpCasts();
      if (isa<CXXThisExpr>(BaseE))
        RelevantExpr = CurE;
      else
        E = BaseE;
      if (!isa<FieldDecl>(CurE->getMemberDecl())) {
        SemaRef.Diag(ELoc, diag::err_omp_expected_access_to_data_field)
            << CurE->getSourceRange();
        break;
}
      auto *FD = cast<FieldDecl>(CurE->getMemberDecl());
      if (FD->isBitField()) {
        SemaRef.Diag(ELoc, diag::err_omp_bit_fields_forbidden_in_clause)
            << CurE->getSourceRange() << getOpenMPClauseName(CKind);
        break;
}
      QualType CurType = BaseE->getType().getNonReferenceType();
      if (auto *RT = CurType->getAs<RecordType>())
        if (RT->isUnionType()) {
          SemaRef.Diag(ELoc, diag::err_omp_union_type_not_allowed)
              << CurE->getSourceRange();
          break;
}
      AllowUnitySizeArraySection = false;
      AllowWholeSizeArraySection = false;
      CurComponents.push_back(
          OMPClauseMappableExprCommon::MappableComponent(CurE, FD));
      continue;
}
    if (auto *CurE = dyn_cast<ArraySubscriptExpr>(E)) {
      E = CurE->getBase()->IgnoreParenImpCasts();
      if (!E->getType()->isAnyPointerType() && !E->getType()->isArrayType()) {
        SemaRef.Diag(ELoc, diag::err_omp_expected_base_var_name)
            << 0 << CurE->getSourceRange();
        break;
}
      if (CheckArrayExpressionDoesNotReferToWholeSize(SemaRef, CurE,
                                                      E->getType()))
        AllowWholeSizeArraySection = false;
      CurComponents.push_back(
          OMPClauseMappableExprCommon::MappableComponent(CurE, nullptr));
      continue;
}
    if (auto *CurE = dyn_cast<OMPArraySectionExpr>(E)) {
      E = CurE->getBase()->IgnoreParenImpCasts();
      auto CurType =
          OMPArraySectionExpr::getBaseOriginalType(E).getCanonicalType();
      if (CurType->isReferenceType())
        CurType = CurType->getPointeeType();
      bool IsPointer = CurType->isAnyPointerType();
      if (!IsPointer && !CurType->isArrayType()) {
        SemaRef.Diag(ELoc, diag::err_omp_expected_base_var_name)
            << 0 << CurE->getSourceRange();
        break;
}
      bool NotWhole =
          CheckArrayExpressionDoesNotReferToWholeSize(SemaRef, CurE, CurType);
      bool NotUnity =
          CheckArrayExpressionDoesNotReferToUnitySize(SemaRef, CurE, CurType);
      if (AllowWholeSizeArraySection) {
        if (NotWhole || IsPointer)
          AllowWholeSizeArraySection = false;
} else if (AllowUnitySizeArraySection && NotUnity) {
        SemaRef.Diag(
            ELoc, diag::err_array_section_does_not_specify_contiguous_storage)
            << CurE->getSourceRange();
        break;
}
      CurComponents.push_back(
          OMPClauseMappableExprCommon::MappableComponent(CurE, nullptr));
      continue;
}
    SemaRef.Diag(ELoc,
                 diag::err_omp_expected_named_var_member_or_array_expression)
        << ERange;
    break;
}
  return RelevantExpr;
}
static bool CheckMapConflicts(
    Sema &SemaRef, DSAStackTy *DSAS, ValueDecl *VD, Expr *E,
    bool CurrentRegionOnly,
    OMPClauseMappableExprCommon::MappableExprComponentListRef CurComponents,
    OpenMPClauseKind CKind) {
  assert(VD && E);
  SourceLocation ELoc = E->getExprLoc();
  SourceRange ERange = E->getSourceRange();
  assert(!CurComponents.empty() && "Map clause expression with no components!");
  assert(CurComponents.back().getAssociatedDeclaration() == VD &&
         "Map clause expression with unexpected base!");
  bool IsEnclosedByDataEnvironmentExpr = false;
  const Expr *EnclosingExpr = nullptr;
  bool FoundError = DSAS->checkMappableExprComponentListsForDecl(
      VD, CurrentRegionOnly,
      [&](OMPClauseMappableExprCommon::MappableExprComponentListRef
              StackComponents,
          OpenMPClauseKind) -> bool {
        assert(!StackComponents.empty() &&
               "Map clause expression with no components!");
        assert(StackComponents.back().getAssociatedDeclaration() == VD &&
               "Map clause expression with unexpected base!");
        auto *RE = StackComponents.front().getAssociatedExpression();
        auto CI = CurComponents.rbegin();
        auto CE = CurComponents.rend();
        auto SI = StackComponents.rbegin();
        auto SE = StackComponents.rend();
for_loop_talkad7420();
        for (; CI != CE && SI != SE; ++CI, ++SI) {
          if (CurrentRegionOnly &&
              (isa<ArraySubscriptExpr>(CI->getAssociatedExpression()) ||
               isa<OMPArraySectionExpr>(CI->getAssociatedExpression())) &&
              (isa<ArraySubscriptExpr>(SI->getAssociatedExpression()) ||
               isa<OMPArraySectionExpr>(SI->getAssociatedExpression()))) {
            SemaRef.Diag(CI->getAssociatedExpression()->getExprLoc(),
                         diag::err_omp_multiple_array_items_in_map_clause)
                << CI->getAssociatedExpression()->getSourceRange();
            SemaRef.Diag(SI->getAssociatedExpression()->getExprLoc(),
                         diag::note_used_here)
                << SI->getAssociatedExpression()->getSourceRange();
            return true;
}
          if (CI->getAssociatedExpression()->getStmtClass() !=
              SI->getAssociatedExpression()->getStmtClass())
            break;
          if (CI->getAssociatedDeclaration() != SI->getAssociatedDeclaration())
            break;
}
for_loop_talkad7420();
        for (; SI != SE; ++SI) {
          QualType Type;
          if (auto *ASE =
                  dyn_cast<ArraySubscriptExpr>(SI->getAssociatedExpression())) {
            Type = ASE->getBase()->IgnoreParenImpCasts()->getType();
} else if (auto *OASE = dyn_cast<OMPArraySectionExpr>(
                         SI->getAssociatedExpression())) {
            auto *E = OASE->getBase()->IgnoreParenImpCasts();
            Type =
                OMPArraySectionExpr::getBaseOriginalType(E).getCanonicalType();
}
          if (Type.isNull() || Type->isAnyPointerType() ||
              CheckArrayExpressionDoesNotReferToWholeSize(
                  SemaRef, SI->getAssociatedExpression(), Type))
            break;
}
        if (CI == CE && SI == SE) {
          if (CurrentRegionOnly) {
            if (CKind == OMPC_map)
              SemaRef.Diag(ELoc, diag::err_omp_map_shared_storage) << ERange;
            else {
              assert(CKind == OMPC_to || CKind == OMPC_from);
              SemaRef.Diag(ELoc, diag::err_omp_once_referenced_in_target_update)
                  << ERange;
}
            SemaRef.Diag(RE->getExprLoc(), diag::note_used_here)
                << RE->getSourceRange();
            return true;
} else {
            IsEnclosedByDataEnvironmentExpr = true;
            return false;
}
}
        QualType DerivedType =
            std::prev(CI)->getAssociatedDeclaration()->getType();
        SourceLocation DerivedLoc =
            std::prev(CI)->getAssociatedExpression()->getExprLoc();
        DerivedType = DerivedType.getNonReferenceType();
        if (DerivedType->isAnyPointerType()) {
          if (CI == CE || SI == SE) {
            SemaRef.Diag(
                DerivedLoc,
                diag::err_omp_pointer_mapped_along_with_derived_section)
                << DerivedLoc;
} else {
            assert(CI != CE && SI != SE);
            SemaRef.Diag(DerivedLoc, diag::err_omp_same_pointer_derreferenced)
                << DerivedLoc;
}
          SemaRef.Diag(RE->getExprLoc(), diag::note_used_here)
              << RE->getSourceRange();
          return true;
}
        if (CurrentRegionOnly && (CI == CE || SI == SE)) {
          if (CKind == OMPC_map)
            SemaRef.Diag(ELoc, diag::err_omp_map_shared_storage) << ERange;
          else {
            assert(CKind == OMPC_to || CKind == OMPC_from);
            SemaRef.Diag(ELoc, diag::err_omp_once_referenced_in_target_update)
                << ERange;
}
          SemaRef.Diag(RE->getExprLoc(), diag::note_used_here)
              << RE->getSourceRange();
          return true;
}
        if (!CurrentRegionOnly && SI != SE)
          EnclosingExpr = RE;
        IsEnclosedByDataEnvironmentExpr |=
            (!CurrentRegionOnly && CI != CE && SI == SE);
        return false;
});
  if (CurrentRegionOnly)
    return FoundError;
  if (EnclosingExpr && !IsEnclosedByDataEnvironmentExpr) {
    SemaRef.Diag(ELoc,
                 diag::err_omp_original_storage_is_shared_and_does_not_contain)
        << ERange;
    SemaRef.Diag(EnclosingExpr->getExprLoc(), diag::note_used_here)
        << EnclosingExpr->getSourceRange();
    return true;
}
  return FoundError;
}
namespace {
struct MappableVarListInfo final {
  ArrayRef<Expr *> VarList;
  SmallVector<Expr *, 16> ProcessedVarList;
  OMPClauseMappableExprCommon::MappableExprComponentLists VarComponents;
  SmallVector<ValueDecl *, 16> VarBaseDeclarations;
  MappableVarListInfo(ArrayRef<Expr *> VarList) : VarList(VarList) {
    VarComponents.reserve(VarList.size());
    VarBaseDeclarations.reserve(VarList.size());
}
};
}
static void
checkMappableExpressionList(Sema &SemaRef, DSAStackTy *DSAS,
                            OpenMPClauseKind CKind, MappableVarListInfo &MVLI,
                            SourceLocation StartLoc,
                            OpenMPMapClauseKind MapType = OMPC_MAP_unknown,
                            bool IsMapTypeImplicit = false) {
  assert((CKind == OMPC_map || CKind == OMPC_to || CKind == OMPC_from) &&
         "Unexpected clause kind with mappable expressions!");
for_loop_talkad7420();
  for (auto &RE : MVLI.VarList) {
    assert(RE && "Null expr in omp to/from/map clause");
    SourceLocation ELoc = RE->getExprLoc();
    auto *VE = RE->IgnoreParenLValueCasts();
    if (VE->isValueDependent() || VE->isTypeDependent() ||
        VE->isInstantiationDependent() ||
        VE->containsUnexpandedParameterPack()) {
      MVLI.ProcessedVarList.push_back(RE);
      continue;
}
    auto *SimpleExpr = RE->IgnoreParenCasts();
    if (!RE->IgnoreParenImpCasts()->isLValue()) {
      SemaRef.Diag(ELoc,
                   diag::err_omp_expected_named_var_member_or_array_expression)
          << RE->getSourceRange();
      continue;
}
    OMPClauseMappableExprCommon::MappableExprComponentList CurComponents;
    ValueDecl *CurDeclaration = nullptr;
    auto *BE =
        CheckMapClauseExpressionBase(SemaRef, SimpleExpr, CurComponents, CKind);
    if (!BE)
      continue;
    assert(!CurComponents.empty() &&
           "Invalid mappable expression information.");
    CurDeclaration = CurComponents.back().getAssociatedDeclaration();
    assert(CurDeclaration && "Null decl on map clause.");
    assert(
        CurDeclaration->isCanonicalDecl() &&
        "Expecting components to have associated only canonical declarations.");
    auto *VD = dyn_cast<VarDecl>(CurDeclaration);
    auto *FD = dyn_cast<FieldDecl>(CurDeclaration);
    assert((VD || FD) && "Only variables or fields are expected here!");
    (void)FD;
    if (VD && DSAS->isThreadPrivate(VD)) {
      auto DVar = DSAS->getTopDSA(VD, false);
      SemaRef.Diag(ELoc, diag::err_omp_threadprivate_in_clause)
          << getOpenMPClauseName(CKind);
      ReportOriginalDSA(SemaRef, DSAS, VD, DVar);
      continue;
}
    if (CheckMapConflicts(SemaRef, DSAS, CurDeclaration, SimpleExpr,
true, CurComponents, CKind))
      break;
    if (CKind == OMPC_map &&
        CheckMapConflicts(SemaRef, DSAS, CurDeclaration, SimpleExpr,
false, CurComponents, CKind))
      break;
    QualType Type = CurDeclaration->getType().getNonReferenceType();
    if (!CheckTypeMappable(VE->getExprLoc(), VE->getSourceRange(), SemaRef,
                           DSAS, Type))
      continue;
    if (CKind == OMPC_map) {
      OpenMPDirectiveKind DKind = DSAS->getCurrentDirective();
      if (DKind == OMPD_target_enter_data &&
          !(MapType == OMPC_MAP_to || MapType == OMPC_MAP_alloc)) {
        SemaRef.Diag(StartLoc, diag::err_omp_invalid_map_type_for_directive)
            << (IsMapTypeImplicit ? 1 : 0)
            << getOpenMPSimpleClauseTypeName(OMPC_map, MapType)
            << getOpenMPDirectiveName(DKind);
        continue;
}
      if (DKind == OMPD_target_exit_data &&
          !(MapType == OMPC_MAP_from || MapType == OMPC_MAP_release ||
            MapType == OMPC_MAP_delete)) {
        SemaRef.Diag(StartLoc, diag::err_omp_invalid_map_type_for_directive)
            << (IsMapTypeImplicit ? 1 : 0)
            << getOpenMPSimpleClauseTypeName(OMPC_map, MapType)
            << getOpenMPDirectiveName(DKind);
        continue;
}
      if ((DKind == OMPD_target || DKind == OMPD_target_teams ||
           DKind == OMPD_target_teams_distribute ||
           DKind == OMPD_target_teams_distribute_parallel_for ||
           DKind == OMPD_target_teams_distribute_parallel_for_simd ||
           DKind == OMPD_target_teams_distribute_simd) && VD) {
        auto DVar = DSAS->getTopDSA(VD, false);
        if (isOpenMPPrivate(DVar.CKind)) {
          SemaRef.Diag(ELoc, diag::err_omp_variable_in_given_clause_and_dsa)
              << getOpenMPClauseName(DVar.CKind)
              << getOpenMPClauseName(OMPC_map)
              << getOpenMPDirectiveName(DSAS->getCurrentDirective());
          ReportOriginalDSA(SemaRef, DSAS, CurDeclaration, DVar);
          continue;
}
}
}
    MVLI.ProcessedVarList.push_back(RE);
    DSAS->addMappableExpressionComponents(CurDeclaration, CurComponents,
OMPC_map);
    MVLI.VarComponents.resize(MVLI.VarComponents.size() + 1);
    MVLI.VarComponents.back().append(CurComponents.begin(),
                                     CurComponents.end());
    MVLI.VarBaseDeclarations.push_back(isa<MemberExpr>(BE) ? nullptr
                                                           : CurDeclaration);
}
}
OMPClause *
Sema::ActOnOpenMPMapClause(OpenMPMapClauseKind MapTypeModifier,
                           OpenMPMapClauseKind MapType, bool IsMapTypeImplicit,
                           SourceLocation MapLoc, SourceLocation ColonLoc,
                           ArrayRef<Expr *> VarList, SourceLocation StartLoc,
                           SourceLocation LParenLoc, SourceLocation EndLoc) {
  MappableVarListInfo MVLI(VarList);
  checkMappableExpressionList(*this, DSAStack, OMPC_map, MVLI, StartLoc,
                              MapType, IsMapTypeImplicit);
  return OMPMapClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                              MVLI.ProcessedVarList, MVLI.VarBaseDeclarations,
                              MVLI.VarComponents, MapTypeModifier, MapType,
                              IsMapTypeImplicit, MapLoc);
}
QualType Sema::ActOnOpenMPDeclareReductionType(SourceLocation TyLoc,
                                               TypeResult ParsedType) {
  assert(ParsedType.isUsable());
  QualType ReductionType = GetTypeFromParser(ParsedType.get());
  if (ReductionType.isNull())
    return QualType();
  if (ReductionType.hasQualifiers()) {
    Diag(TyLoc, diag::err_omp_reduction_wrong_type) << 0;
    return QualType();
}
  if (ReductionType->isFunctionType()) {
    Diag(TyLoc, diag::err_omp_reduction_wrong_type) << 1;
    return QualType();
}
  if (ReductionType->isReferenceType()) {
    Diag(TyLoc, diag::err_omp_reduction_wrong_type) << 2;
    return QualType();
}
  if (ReductionType->isArrayType()) {
    Diag(TyLoc, diag::err_omp_reduction_wrong_type) << 3;
    return QualType();
}
  return ReductionType;
}
Sema::DeclGroupPtrTy Sema::ActOnOpenMPDeclareReductionDirectiveStart(
    Scope *S, DeclContext *DC, DeclarationName Name,
    ArrayRef<std::pair<QualType, SourceLocation>> ReductionTypes,
    AccessSpecifier AS, Decl *PrevDeclInScope) {
  SmallVector<Decl *, 8> Decls;
  Decls.reserve(ReductionTypes.size());
  LookupResult Lookup(*this, Name, SourceLocation(), LookupOMPReductionName,
                      ForRedeclaration);
  llvm::DenseMap<QualType, SourceLocation> PreviousRedeclTypes;
  OMPDeclareReductionDecl *PrevDRD = nullptr;
  bool InCompoundScope = true;
  if (S != nullptr) {
    FunctionScopeInfo *ParentFn = getEnclosingFunction();
    InCompoundScope =
        (ParentFn != nullptr) && !ParentFn->CompoundScopes.empty();
    LookupName(Lookup, S);
    FilterLookupForScope(Lookup, DC, S, 
false,
false);
    llvm::DenseMap<OMPDeclareReductionDecl *, bool> UsedAsPrevious;
    auto Filter = Lookup.makeFilter();
    while (Filter.hasNext()) {
      auto *PrevDecl = cast<OMPDeclareReductionDecl>(Filter.next());
      if (InCompoundScope) {
        auto I = UsedAsPrevious.find(PrevDecl);
        if (I == UsedAsPrevious.end())
          UsedAsPrevious[PrevDecl] = false;
        if (auto *D = PrevDecl->getPrevDeclInScope())
          UsedAsPrevious[D] = true;
}
      PreviousRedeclTypes[PrevDecl->getType().getCanonicalType()] =
          PrevDecl->getLocation();
}
    Filter.done();
    if (InCompoundScope) {
for_loop_talkad7420();
      for (auto &PrevData : UsedAsPrevious) {
        if (!PrevData.second) {
          PrevDRD = PrevData.first;
          break;
}
}
}
} else if (PrevDeclInScope != nullptr) {
    auto *PrevDRDInScope = PrevDRD =
        cast<OMPDeclareReductionDecl>(PrevDeclInScope);
    do {
      PreviousRedeclTypes[PrevDRDInScope->getType().getCanonicalType()] =
          PrevDRDInScope->getLocation();
      PrevDRDInScope = PrevDRDInScope->getPrevDeclInScope();
} while (PrevDRDInScope != nullptr);
}
for_loop_talkad7420();
  for (auto &TyData : ReductionTypes) {
    auto I = PreviousRedeclTypes.find(TyData.first.getCanonicalType());
    bool Invalid = false;
    if (I != PreviousRedeclTypes.end()) {
      Diag(TyData.second, diag::err_omp_declare_reduction_redefinition)
          << TyData.first;
      Diag(I->second, diag::note_previous_definition);
      Invalid = true;
}
    PreviousRedeclTypes[TyData.first.getCanonicalType()] = TyData.second;
    auto *DRD = OMPDeclareReductionDecl::Create(Context, DC, TyData.second,
                                                Name, TyData.first, PrevDRD);
    DC->addDecl(DRD);
    DRD->setAccess(AS);
    Decls.push_back(DRD);
    if (Invalid)
      DRD->setInvalidDecl();
    else
      PrevDRD = DRD;
}
  return DeclGroupPtrTy::make(
      DeclGroupRef::Create(Context, Decls.begin(), Decls.size()));
}
void Sema::ActOnOpenMPDeclareReductionCombinerStart(Scope *S, Decl *D) {
  auto *DRD = cast<OMPDeclareReductionDecl>(D);
  PushFunctionScope();
  getCurFunction()->setHasBranchProtectedScope();
  getCurFunction()->setHasOMPDeclareReductionCombiner();
  if (S != nullptr)
    PushDeclContext(S, DRD);
  else
    CurContext = DRD;
  PushExpressionEvaluationContext(PotentiallyEvaluated);
  QualType ReductionType = DRD->getType();
  auto *OmpInParm =
      buildVarDecl(*this, D->getLocation(), ReductionType, "omp_in");
  auto *OmpOutParm =
      buildVarDecl(*this, D->getLocation(), ReductionType, "omp_out");
  if (S != nullptr) {
    PushOnScopeChains(OmpInParm, S);
    PushOnScopeChains(OmpOutParm, S);
} else {
    DRD->addDecl(OmpInParm);
    DRD->addDecl(OmpOutParm);
}
}
void Sema::ActOnOpenMPDeclareReductionCombinerEnd(Decl *D, Expr *Combiner) {
  auto *DRD = cast<OMPDeclareReductionDecl>(D);
  DiscardCleanupsInEvaluationContext();
  PopExpressionEvaluationContext();
  PopDeclContext();
  PopFunctionScopeInfo();
  if (Combiner != nullptr)
    DRD->setCombiner(Combiner);
  else
    DRD->setInvalidDecl();
}
void Sema::ActOnOpenMPDeclareReductionInitializerStart(Scope *S, Decl *D) {
  auto *DRD = cast<OMPDeclareReductionDecl>(D);
  PushFunctionScope();
  getCurFunction()->setHasBranchProtectedScope();
  if (S != nullptr)
    PushDeclContext(S, DRD);
  else
    CurContext = DRD;
  PushExpressionEvaluationContext(PotentiallyEvaluated);
  QualType ReductionType = DRD->getType();
  auto *OmpPrivParm =
      buildVarDecl(*this, D->getLocation(), ReductionType, "omp_priv");
  auto *OmpOrigParm =
      buildVarDecl(*this, D->getLocation(), ReductionType, "omp_orig");
  if (S != nullptr) {
    PushOnScopeChains(OmpPrivParm, S);
    PushOnScopeChains(OmpOrigParm, S);
} else {
    DRD->addDecl(OmpPrivParm);
    DRD->addDecl(OmpOrigParm);
}
}
void Sema::ActOnOpenMPDeclareReductionInitializerEnd(Decl *D,
                                                     Expr *Initializer) {
  auto *DRD = cast<OMPDeclareReductionDecl>(D);
  DiscardCleanupsInEvaluationContext();
  PopExpressionEvaluationContext();
  PopDeclContext();
  PopFunctionScopeInfo();
  if (Initializer != nullptr)
    DRD->setInitializer(Initializer);
  else
    DRD->setInvalidDecl();
}
Sema::DeclGroupPtrTy Sema::ActOnOpenMPDeclareReductionDirectiveEnd(
    Scope *S, DeclGroupPtrTy DeclReductions, bool IsValid) {
for_loop_talkad7420();
  for (auto *D : DeclReductions.get()) {
    if (IsValid) {
      auto *DRD = cast<OMPDeclareReductionDecl>(D);
      if (S != nullptr)
        PushOnScopeChains(DRD, S, 
false);
} else
      D->setInvalidDecl();
}
  return DeclReductions;
}
OMPClause *Sema::ActOnOpenMPNumTeamsClause(Expr *NumTeams,
                                           SourceLocation StartLoc,
                                           SourceLocation LParenLoc,
                                           SourceLocation EndLoc) {
  Expr *ValExpr = NumTeams;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_num_teams,
true))
    return nullptr;
  return new (Context) OMPNumTeamsClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPThreadLimitClause(Expr *ThreadLimit,
                                              SourceLocation StartLoc,
                                              SourceLocation LParenLoc,
                                              SourceLocation EndLoc) {
  Expr *ValExpr = ThreadLimit;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_thread_limit,
true))
    return nullptr;
  return new (Context)
      OMPThreadLimitClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPPriorityClause(Expr *Priority,
                                           SourceLocation StartLoc,
                                           SourceLocation LParenLoc,
                                           SourceLocation EndLoc) {
  Expr *ValExpr = Priority;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_priority,
false))
    return nullptr;
  return new (Context) OMPPriorityClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPGrainsizeClause(Expr *Grainsize,
                                            SourceLocation StartLoc,
                                            SourceLocation LParenLoc,
                                            SourceLocation EndLoc) {
  Expr *ValExpr = Grainsize;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_grainsize,
true))
    return nullptr;
  return new (Context) OMPGrainsizeClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPNumTasksClause(Expr *NumTasks,
                                           SourceLocation StartLoc,
                                           SourceLocation LParenLoc,
                                           SourceLocation EndLoc) {
  Expr *ValExpr = NumTasks;
  if (!IsNonNegativeIntegerValue(ValExpr, *this, OMPC_num_tasks,
true))
    return nullptr;
  return new (Context) OMPNumTasksClause(ValExpr, StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPHintClause(Expr *Hint, SourceLocation StartLoc,
                                       SourceLocation LParenLoc,
                                       SourceLocation EndLoc) {
  ExprResult HintExpr = VerifyPositiveIntegerConstantInClause(Hint, OMPC_hint);
  if (HintExpr.isInvalid())
    return nullptr;
  return new (Context)
      OMPHintClause(HintExpr.get(), StartLoc, LParenLoc, EndLoc);
}
OMPClause *Sema::ActOnOpenMPDistScheduleClause(
    OpenMPDistScheduleClauseKind Kind, Expr *ChunkSize, SourceLocation StartLoc,
    SourceLocation LParenLoc, SourceLocation KindLoc, SourceLocation CommaLoc,
    SourceLocation EndLoc) {
  if (Kind == OMPC_DIST_SCHEDULE_unknown) {
    std::string Values;
    Values += "'";
    Values += getOpenMPSimpleClauseTypeName(OMPC_dist_schedule, 0);
    Values += "'";
    Diag(KindLoc, diag::err_omp_unexpected_clause_value)
        << Values << getOpenMPClauseName(OMPC_dist_schedule);
    return nullptr;
}
  Expr *ValExpr = ChunkSize;
  Stmt *HelperValStmt = nullptr;
  if (ChunkSize) {
    if (!ChunkSize->isValueDependent() && !ChunkSize->isTypeDependent() &&
        !ChunkSize->isInstantiationDependent() &&
        !ChunkSize->containsUnexpandedParameterPack()) {
      SourceLocation ChunkSizeLoc = ChunkSize->getLocStart();
      ExprResult Val =
          PerformOpenMPImplicitIntegerConversion(ChunkSizeLoc, ChunkSize);
      if (Val.isInvalid())
        return nullptr;
      ValExpr = Val.get();
      llvm::APSInt Result;
      if (ValExpr->isIntegerConstantExpr(Result, Context)) {
        if (Result.isSigned() && !Result.isStrictlyPositive()) {
          Diag(ChunkSizeLoc, diag::err_omp_negative_expression_in_clause)
              << "dist_schedule" << ChunkSize->getSourceRange();
          return nullptr;
}
} else if (isParallelOrTaskRegion(DSAStack->getCurrentDirective()) &&
                 !CurContext->isDependentContext()) {
        llvm::MapVector<Expr *, DeclRefExpr *> Captures;
        ValExpr = tryBuildCapture(*this, ValExpr, Captures).get();
        HelperValStmt = buildPreInits(Context, Captures);
}
}
}
  return new (Context)
      OMPDistScheduleClause(StartLoc, LParenLoc, KindLoc, CommaLoc, EndLoc,
                            Kind, ValExpr, HelperValStmt);
}
OMPClause *Sema::ActOnOpenMPDefaultmapClause(
    OpenMPDefaultmapClauseModifier M, OpenMPDefaultmapClauseKind Kind,
    SourceLocation StartLoc, SourceLocation LParenLoc, SourceLocation MLoc,
    SourceLocation KindLoc, SourceLocation EndLoc) {
  if (M != OMPC_DEFAULTMAP_MODIFIER_tofrom || Kind != OMPC_DEFAULTMAP_scalar) {
    std::string Value;
    SourceLocation Loc;
    Value += "'";
    if (M != OMPC_DEFAULTMAP_MODIFIER_tofrom) {
      Value += getOpenMPSimpleClauseTypeName(OMPC_defaultmap,
                                             OMPC_DEFAULTMAP_MODIFIER_tofrom);
      Loc = MLoc;
} else {
      Value += getOpenMPSimpleClauseTypeName(OMPC_defaultmap,
                                             OMPC_DEFAULTMAP_scalar);
      Loc = KindLoc;
}
    Value += "'";
    Diag(Loc, diag::err_omp_unexpected_clause_value)
        << Value << getOpenMPClauseName(OMPC_defaultmap);
    return nullptr;
}
  return new (Context)
      OMPDefaultmapClause(StartLoc, LParenLoc, MLoc, KindLoc, EndLoc, Kind, M);
}
bool Sema::ActOnStartOpenMPDeclareTargetDirective(SourceLocation Loc) {
  DeclContext *CurLexicalContext = getCurLexicalContext();
  if (!CurLexicalContext->isFileContext() &&
      !CurLexicalContext->isExternCContext() &&
      !CurLexicalContext->isExternCXXContext()) {
    Diag(Loc, diag::err_omp_region_not_file_context);
    return false;
}
  if (IsInOpenMPDeclareTargetContext) {
    Diag(Loc, diag::err_omp_enclosed_declare_target);
    return false;
}
  IsInOpenMPDeclareTargetContext = true;
  return true;
}
void Sema::ActOnFinishOpenMPDeclareTargetDirective() {
  assert(IsInOpenMPDeclareTargetContext &&
         "Unexpected ActOnFinishOpenMPDeclareTargetDirective");
  IsInOpenMPDeclareTargetContext = false;
}
void Sema::ActOnOpenMPDeclareTargetName(Scope *CurScope,
                                        CXXScopeSpec &ScopeSpec,
                                        const DeclarationNameInfo &Id,
                                        OMPDeclareTargetDeclAttr::MapTypeTy MT,
                                        NamedDeclSetType &SameDirectiveDecls) {
  LookupResult Lookup(*this, Id, LookupOrdinaryName);
  LookupParsedName(Lookup, CurScope, &ScopeSpec, true);
  if (Lookup.isAmbiguous())
    return;
  Lookup.suppressDiagnostics();
  if (!Lookup.isSingleResult()) {
    if (TypoCorrection Corrected =
            CorrectTypo(Id, LookupOrdinaryName, CurScope, nullptr,
                        llvm::make_unique<VarOrFuncDeclFilterCCC>(*this),
                        CTK_ErrorRecovery)) {
      diagnoseTypo(Corrected, PDiag(diag::err_undeclared_var_use_suggest)
                                  << Id.getName());
      checkDeclIsAllowedInOpenMPTarget(nullptr, Corrected.getCorrectionDecl());
      return;
}
    Diag(Id.getLoc(), diag::err_undeclared_var_use) << Id.getName();
    return;
}
  NamedDecl *ND = Lookup.getAsSingle<NamedDecl>();
  if (isa<VarDecl>(ND) || isa<FunctionDecl>(ND)) {
    if (!SameDirectiveDecls.insert(cast<NamedDecl>(ND->getCanonicalDecl())))
      Diag(Id.getLoc(), diag::err_omp_declare_target_multiple) << Id.getName();
    if (!ND->hasAttr<OMPDeclareTargetDeclAttr>()) {
      Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(Context, MT);
      ND->addAttr(A);
      if (ASTMutationListener *ML = Context.getASTMutationListener())
        ML->DeclarationMarkedOpenMPDeclareTarget(ND, A);
      checkDeclIsAllowedInOpenMPTarget(nullptr, ND);
} else if (ND->getAttr<OMPDeclareTargetDeclAttr>()->getMapType() != MT) {
      Diag(Id.getLoc(), diag::err_omp_declare_target_to_and_link)
          << Id.getName();
}
} else
    Diag(Id.getLoc(), diag::err_omp_invalid_target_decl) << Id.getName();
}
static void checkDeclInTargetContext(SourceLocation SL, SourceRange SR,
                                     Sema &SemaRef, Decl *D) {
  if (!D)
    return;
  Decl *LD = nullptr;
  if (isa<TagDecl>(D)) {
    LD = cast<TagDecl>(D)->getDefinition();
} else if (isa<VarDecl>(D)) {
    LD = cast<VarDecl>(D)->getDefinition();
    if (cast<VarDecl>(D)->isImplicit()) {
      Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(
          SemaRef.Context, OMPDeclareTargetDeclAttr::MT_To);
      D->addAttr(A);
      if (ASTMutationListener *ML = SemaRef.Context.getASTMutationListener())
        ML->DeclarationMarkedOpenMPDeclareTarget(D, A);
      return;
}
} else if (isa<FunctionDecl>(D)) {
    const FunctionDecl *FD = nullptr;
    if (cast<FunctionDecl>(D)->hasBody(FD))
      LD = const_cast<FunctionDecl *>(FD);
    if (LD == D) {
      Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(
          SemaRef.Context, OMPDeclareTargetDeclAttr::MT_To);
      D->addAttr(A);
      if (ASTMutationListener *ML = SemaRef.Context.getASTMutationListener())
        ML->DeclarationMarkedOpenMPDeclareTarget(D, A);
      return;
}
}
  if (!LD)
    LD = D;
  if (LD && !LD->hasAttr<OMPDeclareTargetDeclAttr>() &&
      (isa<VarDecl>(LD) || isa<FunctionDecl>(LD))) {
    if (LD->isOutOfLine()) {
      SemaRef.Diag(LD->getLocation(), diag::warn_omp_not_in_target_context);
      SemaRef.Diag(SL, diag::note_used_here) << SR;
} else {
      DeclContext *DC = LD->getDeclContext();
      while (DC) {
        if (isa<FunctionDecl>(DC) &&
            cast<FunctionDecl>(DC)->hasAttr<OMPDeclareTargetDeclAttr>())
          break;
        DC = DC->getParent();
}
      if (DC)
        return;
      SemaRef.Diag(LD->getLocation(), diag::warn_omp_not_in_target_context);
      SemaRef.Diag(SL, diag::note_used_here) << SR;
}
    Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(
        SemaRef.Context, OMPDeclareTargetDeclAttr::MT_To);
    D->addAttr(A);
    if (ASTMutationListener *ML = SemaRef.Context.getASTMutationListener())
      ML->DeclarationMarkedOpenMPDeclareTarget(D, A);
}
}
static bool checkValueDeclInTarget(SourceLocation SL, SourceRange SR,
                                   Sema &SemaRef, DSAStackTy *Stack,
                                   ValueDecl *VD) {
  if (VD->hasAttr<OMPDeclareTargetDeclAttr>())
    return true;
  if (!CheckTypeMappable(SL, SR, SemaRef, Stack, VD->getType()))
    return false;
  return true;
}
void Sema::checkDeclIsAllowedInOpenMPTarget(Expr *E, Decl *D) {
  if (!D || D->isInvalidDecl())
    return;
  SourceRange SR = E ? E->getSourceRange() : D->getSourceRange();
  SourceLocation SL = E ? E->getLocStart() : D->getLocation();
  if (VarDecl *VD = dyn_cast<VarDecl>(D)) {
    if (DSAStack->isThreadPrivate(VD)) {
      Diag(SL, diag::err_omp_threadprivate_in_target);
      ReportOriginalDSA(*this, DSAStack, VD, DSAStack->getTopDSA(VD, false));
      return;
}
}
  if (ValueDecl *VD = dyn_cast<ValueDecl>(D)) {
    if ((E || !VD->getType()->isIncompleteType()) &&
        !checkValueDeclInTarget(SL, SR, *this, DSAStack, VD)) {
      if (isa<VarDecl>(VD) || isa<FunctionDecl>(VD)) {
        Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(
            Context, OMPDeclareTargetDeclAttr::MT_To);
        VD->addAttr(A);
        if (ASTMutationListener *ML = Context.getASTMutationListener())
          ML->DeclarationMarkedOpenMPDeclareTarget(VD, A);
}
      return;
}
}
  if (!E) {
    if (!D->hasAttr<OMPDeclareTargetDeclAttr>() &&
        (isa<VarDecl>(D) || isa<FunctionDecl>(D))) {
      Attr *A = OMPDeclareTargetDeclAttr::CreateImplicit(
          Context, OMPDeclareTargetDeclAttr::MT_To);
      D->addAttr(A);
      if (ASTMutationListener *ML = Context.getASTMutationListener())
        ML->DeclarationMarkedOpenMPDeclareTarget(D, A);
}
    return;
}
  checkDeclInTargetContext(E->getExprLoc(), E->getSourceRange(), *this, D);
}
OMPClause *Sema::ActOnOpenMPToClause(ArrayRef<Expr *> VarList,
                                     SourceLocation StartLoc,
                                     SourceLocation LParenLoc,
                                     SourceLocation EndLoc) {
  MappableVarListInfo MVLI(VarList);
  checkMappableExpressionList(*this, DSAStack, OMPC_to, MVLI, StartLoc);
  if (MVLI.ProcessedVarList.empty())
    return nullptr;
  return OMPToClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                             MVLI.ProcessedVarList, MVLI.VarBaseDeclarations,
                             MVLI.VarComponents);
}
OMPClause *Sema::ActOnOpenMPFromClause(ArrayRef<Expr *> VarList,
                                       SourceLocation StartLoc,
                                       SourceLocation LParenLoc,
                                       SourceLocation EndLoc) {
  MappableVarListInfo MVLI(VarList);
  checkMappableExpressionList(*this, DSAStack, OMPC_from, MVLI, StartLoc);
  if (MVLI.ProcessedVarList.empty())
    return nullptr;
  return OMPFromClause::Create(Context, StartLoc, LParenLoc, EndLoc,
                               MVLI.ProcessedVarList, MVLI.VarBaseDeclarations,
                               MVLI.VarComponents);
}
OMPClause *Sema::ActOnOpenMPUseDevicePtrClause(ArrayRef<Expr *> VarList,
                                               SourceLocation StartLoc,
                                               SourceLocation LParenLoc,
                                               SourceLocation EndLoc) {
  MappableVarListInfo MVLI(VarList);
  SmallVector<Expr *, 8> PrivateCopies;
  SmallVector<Expr *, 8> Inits;
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP use_device_ptr clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      MVLI.ProcessedVarList.push_back(RefExpr);
      PrivateCopies.push_back(nullptr);
      Inits.push_back(nullptr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    Type = Type.getNonReferenceType().getUnqualifiedType();
    auto *VD = dyn_cast<VarDecl>(D);
    if (!Type->isPointerType()) {
      Diag(ELoc, diag::err_omp_usedeviceptr_not_a_pointer)
          << 0 << RefExpr->getSourceRange();
      continue;
}
    auto VDPrivate = buildVarDecl(*this, ELoc, Type, D->getName(),
                                  D->hasAttrs() ? &D->getAttrs() : nullptr);
    if (VDPrivate->isInvalidDecl())
      continue;
    CurContext->addDecl(VDPrivate);
    auto VDPrivateRefExpr = buildDeclRefExpr(
        *this, VDPrivate, RefExpr->getType().getUnqualifiedType(), ELoc);
    auto *VDInit =
        buildVarDecl(*this, RefExpr->getExprLoc(), Type, ".devptr.temp");
    auto *VDInitRefExpr = buildDeclRefExpr(*this, VDInit, RefExpr->getType(),
                                           RefExpr->getExprLoc());
    AddInitializerToDecl(VDPrivate,
                         DefaultLvalueConversion(VDInitRefExpr).get(),
false);
    DeclRefExpr *Ref = nullptr;
    if (!VD)
      Ref = buildCapture(*this, D, SimpleRefExpr, 
true);
    MVLI.ProcessedVarList.push_back(VD ? RefExpr->IgnoreParens() : Ref);
    PrivateCopies.push_back(VDPrivateRefExpr);
    Inits.push_back(VDInitRefExpr);
    DSAStack->addDSA(D, RefExpr->IgnoreParens(), OMPC_firstprivate, Ref);
    MVLI.VarBaseDeclarations.push_back(D);
    MVLI.VarComponents.resize(MVLI.VarComponents.size() + 1);
    MVLI.VarComponents.back().push_back(
        OMPClauseMappableExprCommon::MappableComponent(SimpleRefExpr, D));
}
  if (MVLI.ProcessedVarList.empty())
    return nullptr;
  return OMPUseDevicePtrClause::Create(
      Context, StartLoc, LParenLoc, EndLoc, MVLI.ProcessedVarList,
      PrivateCopies, Inits, MVLI.VarBaseDeclarations, MVLI.VarComponents);
}
OMPClause *Sema::ActOnOpenMPIsDevicePtrClause(ArrayRef<Expr *> VarList,
                                              SourceLocation StartLoc,
                                              SourceLocation LParenLoc,
                                              SourceLocation EndLoc) {
  MappableVarListInfo MVLI(VarList);
for_loop_talkad7420();
  for (auto &RefExpr : VarList) {
    assert(RefExpr && "NULL expr in OpenMP is_device_ptr clause.");
    SourceLocation ELoc;
    SourceRange ERange;
    Expr *SimpleRefExpr = RefExpr;
    auto Res = getPrivateItem(*this, SimpleRefExpr, ELoc, ERange);
    if (Res.second) {
      MVLI.ProcessedVarList.push_back(RefExpr);
}
    ValueDecl *D = Res.first;
    if (!D)
      continue;
    QualType Type = D->getType();
    if (!Type.getNonReferenceType()->isPointerType() &&
        !Type.getNonReferenceType()->isArrayType()) {
      Diag(ELoc, diag::err_omp_argument_type_isdeviceptr)
          << 0 << RefExpr->getSourceRange();
      continue;
}
    auto DVar = DSAStack->getTopDSA(D, false);
    if (isOpenMPPrivate(DVar.CKind)) {
      Diag(ELoc, diag::err_omp_variable_in_given_clause_and_dsa)
          << getOpenMPClauseName(DVar.CKind)
          << getOpenMPClauseName(OMPC_is_device_ptr)
          << getOpenMPDirectiveName(DSAStack->getCurrentDirective());
      ReportOriginalDSA(*this, DSAStack, D, DVar);
      continue;
}
    Expr *ConflictExpr;
    if (DSAStack->checkMappableExprComponentListsForDecl(
            D, 
true,
            [&ConflictExpr](
                OMPClauseMappableExprCommon::MappableExprComponentListRef R,
                OpenMPClauseKind) -> bool {
              ConflictExpr = R.front().getAssociatedExpression();
              return true;
})) {
      Diag(ELoc, diag::err_omp_map_shared_storage) << RefExpr->getSourceRange();
      Diag(ConflictExpr->getExprLoc(), diag::note_used_here)
          << ConflictExpr->getSourceRange();
      continue;
}
    OMPClauseMappableExprCommon::MappableComponent MC(SimpleRefExpr, D);
    DSAStack->addMappableExpressionComponents(
        D, MC, 
OMPC_is_device_ptr);
    MVLI.ProcessedVarList.push_back(SimpleRefExpr);
    assert((isa<DeclRefExpr>(SimpleRefExpr) ||
            isa<CXXThisExpr>(cast<MemberExpr>(SimpleRefExpr)->getBase())) &&
           "Unexpected device pointer expression!");
    MVLI.VarBaseDeclarations.push_back(
        isa<DeclRefExpr>(SimpleRefExpr) ? D : nullptr);
    MVLI.VarComponents.resize(MVLI.VarComponents.size() + 1);
    MVLI.VarComponents.back().push_back(MC);
}
  if (MVLI.ProcessedVarList.empty())
    return nullptr;
  return OMPIsDevicePtrClause::Create(
      Context, StartLoc, LParenLoc, EndLoc, MVLI.ProcessedVarList,
      MVLI.VarBaseDeclarations, MVLI.VarComponents);
}