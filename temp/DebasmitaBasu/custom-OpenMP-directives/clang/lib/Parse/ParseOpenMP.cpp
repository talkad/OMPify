#include "_fake_typedefs.h"
#include "_fake_defines.h"
#include "patch.h"
#include "RAIIObjectsForParser.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/StmtOpenMP.h"
#include "clang/Parse/ParseDiagnostic.h"
#include "clang/Parse/Parser.h"
#include "clang/Sema/Scope.h"
#include "llvm/ADT/PointerIntPair.h"
using namespace clang;
namespace {
enum OpenMPDirectiveKindEx {
  OMPD_cancellation = OMPD_unknown + 1,
  OMPD_data,
  OMPD_declare,
  OMPD_end,
  OMPD_end_declare,
  OMPD_enter,
  OMPD_exit,
  OMPD_point,
  OMPD_reduction,
  OMPD_target_enter,
  OMPD_target_exit,
  OMPD_update,
  OMPD_distribute_parallel,
  OMPD_teams_distribute_parallel,
  OMPD_target_teams_distribute_parallel
};
class ThreadprivateListParserHelper final {
  SmallVector<Expr *, 4> Identifiers;
  Parser *P;
public:
  ThreadprivateListParserHelper(Parser *P) : P(P) {
}
  void operator()(CXXScopeSpec &SS, DeclarationNameInfo NameInfo) {
    ExprResult Res =
        P->getActions().ActOnOpenMPIdExpression(P->getCurScope(), SS, NameInfo);
    if (Res.isUsable())
      Identifiers.push_back(Res.get());
}
  llvm::ArrayRef<Expr *> getIdentifiers() const { return Identifiers; 
}
};
} 
static unsigned getOpenMPDirectiveKindEx(StringRef S) {
  auto DKind = getOpenMPDirectiveKind(S);
  if (DKind != OMPD_unknown)
    return DKind;
  return llvm::StringSwitch<unsigned>(S)
      .Case("cancellation", OMPD_cancellation)
      .Case("data", OMPD_data)
      .Case("declare", OMPD_declare)
      .Case("end", OMPD_end)
      .Case("enter", OMPD_enter)
      .Case("exit", OMPD_exit)
      .Case("point", OMPD_point)
      .Case("reduction", OMPD_reduction)
      .Case("update", OMPD_update)
      .Default(OMPD_unknown);
}
static OpenMPDirectiveKind ParseOpenMPDirectiveKind(Parser &P) {
  static const unsigned F[][3] = {
    { OMPD_cancellation, OMPD_point, OMPD_cancellation_point 
},
    { OMPD_declare, OMPD_reduction, OMPD_declare_reduction 
},
    { OMPD_declare, OMPD_simd, OMPD_declare_simd 
},
    { OMPD_declare, OMPD_target, OMPD_declare_target 
},
    { OMPD_distribute, OMPD_parallel, OMPD_distribute_parallel 
},
    { OMPD_distribute_parallel, OMPD_for, OMPD_distribute_parallel_for 
},
    { OMPD_distribute_parallel_for, OMPD_simd, 
      OMPD_distribute_parallel_for_simd 
},
    { OMPD_distribute, OMPD_simd, OMPD_distribute_simd 
},
    { OMPD_end, OMPD_declare, OMPD_end_declare 
},
    { OMPD_end_declare, OMPD_target, OMPD_end_declare_target 
},
    { OMPD_target, OMPD_data, OMPD_target_data 
},
    { OMPD_target, OMPD_enter, OMPD_target_enter 
},
    { OMPD_target, OMPD_exit, OMPD_target_exit 
},
    { OMPD_target, OMPD_update, OMPD_target_update 
},
    { OMPD_target_enter, OMPD_data, OMPD_target_enter_data 
},
    { OMPD_target_exit, OMPD_data, OMPD_target_exit_data 
},
    { OMPD_for, OMPD_simd, OMPD_for_simd 
},
    { OMPD_parallel, OMPD_for, OMPD_parallel_for 
},
    { OMPD_parallel_for, OMPD_simd, OMPD_parallel_for_simd 
},
    { OMPD_parallel, OMPD_sections, OMPD_parallel_sections 
},
    { OMPD_taskloop, OMPD_simd, OMPD_taskloop_simd 
},
    { OMPD_target, OMPD_parallel, OMPD_target_parallel 
},
    { OMPD_target, OMPD_simd, OMPD_target_simd 
},
    { OMPD_target_parallel, OMPD_for, OMPD_target_parallel_for 
},
    { OMPD_target_parallel_for, OMPD_simd, OMPD_target_parallel_for_simd 
},
    { OMPD_teams, OMPD_distribute, OMPD_teams_distribute 
},
    { OMPD_teams_distribute, OMPD_simd, OMPD_teams_distribute_simd 
},
    { OMPD_teams_distribute, OMPD_parallel, OMPD_teams_distribute_parallel 
},
    { OMPD_teams_distribute_parallel, OMPD_for, OMPD_teams_distribute_parallel_for 
},
    { OMPD_teams_distribute_parallel_for, OMPD_simd, OMPD_teams_distribute_parallel_for_simd 
},
    { OMPD_target, OMPD_teams, OMPD_target_teams 
},
    { OMPD_target_teams, OMPD_distribute, OMPD_target_teams_distribute 
},
    { OMPD_target_teams_distribute, OMPD_parallel, OMPD_target_teams_distribute_parallel 
},
    { OMPD_target_teams_distribute, OMPD_simd, OMPD_target_teams_distribute_simd 
},
    { OMPD_target_teams_distribute_parallel, OMPD_for, OMPD_target_teams_distribute_parallel_for 
},
    { OMPD_target_teams_distribute_parallel_for, OMPD_simd, OMPD_target_teams_distribute_parallel_for_simd 
}
};
  enum { CancellationPoint = 0, DeclareReduction = 1, TargetData = 2 
};
  auto Tok = P.getCurToken();
  unsigned DKind =
      Tok.isAnnotation()
          ? static_cast<unsigned>(OMPD_unknown)
          : getOpenMPDirectiveKindEx(P.getPreprocessor().getSpelling(Tok));
  if (DKind == OMPD_unknown)
    return OMPD_unknown;
for_loop_talkad7420();
  for (unsigned i = 0; i < llvm::array_lengthof(F); ++i) {
    if (DKind != F[i][0])
      continue;
    Tok = P.getPreprocessor().LookAhead(0);
    unsigned SDKind =
        Tok.isAnnotation()
            ? static_cast<unsigned>(OMPD_unknown)
            : getOpenMPDirectiveKindEx(P.getPreprocessor().getSpelling(Tok));
    if (SDKind == OMPD_unknown)
      continue;
    if (SDKind == F[i][1]) {
      P.ConsumeToken();
      DKind = F[i][2];
}
}
  return DKind < OMPD_unknown ? static_cast<OpenMPDirectiveKind>(DKind)
                              : OMPD_unknown;
}
static DeclarationName parseOpenMPReductionId(Parser &P) {
  Token Tok = P.getCurToken();
  Sema &Actions = P.getActions();
  OverloadedOperatorKind OOK = OO_None;
  bool WithOperator = false;
  if (Tok.is(tok::kw_operator)) {
    P.ConsumeToken();
    Tok = P.getCurToken();
    WithOperator = true;
}
  switch (Tok.getKind()) {
  case tok::plus: 
    OOK = OO_Plus;
    break;
  case tok::minus: 
    OOK = OO_Minus;
    break;
  case tok::star: 
    OOK = OO_Star;
    break;
  case tok::amp: 
    OOK = OO_Amp;
    break;
  case tok::pipe: 
    OOK = OO_Pipe;
    break;
  case tok::caret: 
    OOK = OO_Caret;
    break;
  case tok::ampamp: 
    OOK = OO_AmpAmp;
    break;
  case tok::pipepipe: 
    OOK = OO_PipePipe;
    break;
  case tok::identifier: 
    if (!WithOperator)
      break;
  default:
    P.Diag(Tok.getLocation(), diag::err_omp_expected_reduction_identifier);
    P.SkipUntil(tok::colon, tok::r_paren, tok::annot_pragma_openmp_end,
                Parser::StopBeforeMatch);
    return DeclarationName();
}
  P.ConsumeToken();
  auto &DeclNames = Actions.getASTContext().DeclarationNames;
  return OOK == OO_None ? DeclNames.getIdentifier(Tok.getIdentifierInfo())
                        : DeclNames.getCXXOperatorName(OOK);
}
Parser::DeclGroupPtrTy
Parser::ParseOpenMPDeclareReductionDirective(AccessSpecifier AS) {
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after,
                         getOpenMPDirectiveName(OMPD_declare_reduction))) {
    SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
    return DeclGroupPtrTy();
}
  DeclarationName Name = parseOpenMPReductionId(*this);
  if (Name.isEmpty() && Tok.is(tok::annot_pragma_openmp_end))
    return DeclGroupPtrTy();
  bool IsCorrect = !ExpectAndConsume(tok::colon);
  if (!IsCorrect && Tok.is(tok::annot_pragma_openmp_end))
    return DeclGroupPtrTy();
  IsCorrect = IsCorrect && !Name.isEmpty();
  if (Tok.is(tok::colon) || Tok.is(tok::annot_pragma_openmp_end)) {
    Diag(Tok.getLocation(), diag::err_expected_type);
    IsCorrect = false;
}
  if (!IsCorrect && Tok.is(tok::annot_pragma_openmp_end))
    return DeclGroupPtrTy();
  SmallVector<std::pair<QualType, SourceLocation>, 8> ReductionTypes;
  do {
    ColonProtectionRAIIObject ColonRAII(*this);
    SourceRange Range;
    TypeResult TR = ParseTypeName(&Range, Declarator::PrototypeContext, AS);
    if (TR.isUsable()) {
      auto ReductionType =
          Actions.ActOnOpenMPDeclareReductionType(Range.getBegin(), TR);
      if (!ReductionType.isNull()) {
        ReductionTypes.push_back(
            std::make_pair(ReductionType, Range.getBegin()));
}
} else {
      SkipUntil(tok::comma, tok::colon, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
}
    if (Tok.is(tok::colon) || Tok.is(tok::annot_pragma_openmp_end))
      break;
    if (ExpectAndConsume(tok::comma)) {
      IsCorrect = false;
      if (Tok.is(tok::annot_pragma_openmp_end)) {
        Diag(Tok.getLocation(), diag::err_expected_type);
        return DeclGroupPtrTy();
}
}
} while (Tok.isNot(tok::annot_pragma_openmp_end));
  if (ReductionTypes.empty()) {
    SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
    return DeclGroupPtrTy();
}
  if (!IsCorrect && Tok.is(tok::annot_pragma_openmp_end))
    return DeclGroupPtrTy();
  if (ExpectAndConsume(tok::colon))
    IsCorrect = false;
  if (Tok.is(tok::annot_pragma_openmp_end)) {
    Diag(Tok.getLocation(), diag::err_expected_expression);
    return DeclGroupPtrTy();
}
  DeclGroupPtrTy DRD = Actions.ActOnOpenMPDeclareReductionDirectiveStart(
      getCurScope(), Actions.getCurLexicalContext(), Name, ReductionTypes, AS);
  unsigned I = 0, E = ReductionTypes.size();
for_loop_talkad7420();
  for (auto *D : DRD.get()) {
    TentativeParsingAction TPA(*this);
    ParseScope OMPDRScope(this, Scope::FnScope | Scope::DeclScope |
                                    Scope::OpenMPDirectiveScope);
    Actions.ActOnOpenMPDeclareReductionCombinerStart(getCurScope(), D);
    ExprResult CombinerResult =
        Actions.ActOnFinishFullExpr(ParseAssignmentExpression().get(),
                                    D->getLocation(), 
true);
    Actions.ActOnOpenMPDeclareReductionCombinerEnd(D, CombinerResult.get());
    if (CombinerResult.isInvalid() && Tok.isNot(tok::r_paren) &&
        Tok.isNot(tok::annot_pragma_openmp_end)) {
      TPA.Commit();
      IsCorrect = false;
      break;
}
    IsCorrect = !T.consumeClose() && IsCorrect && CombinerResult.isUsable();
    ExprResult InitializerResult;
    if (Tok.isNot(tok::annot_pragma_openmp_end)) {
      if (Tok.is(tok::identifier) &&
          Tok.getIdentifierInfo()->isStr("initializer"))
        ConsumeToken();
      else {
        Diag(Tok.getLocation(), diag::err_expected) << "'initializer'";
        TPA.Commit();
        IsCorrect = false;
        break;
}
      BalancedDelimiterTracker T(*this, tok::l_paren,
                                 tok::annot_pragma_openmp_end);
      IsCorrect =
          !T.expectAndConsume(diag::err_expected_lparen_after, "initializer") &&
          IsCorrect;
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        ParseScope OMPDRScope(this, Scope::FnScope | Scope::DeclScope |
                                        Scope::OpenMPDirectiveScope);
        Actions.ActOnOpenMPDeclareReductionInitializerStart(getCurScope(), D);
        InitializerResult = Actions.ActOnFinishFullExpr(
            ParseAssignmentExpression().get(), D->getLocation(),
true);
        Actions.ActOnOpenMPDeclareReductionInitializerEnd(
            D, InitializerResult.get());
        if (InitializerResult.isInvalid() && Tok.isNot(tok::r_paren) &&
            Tok.isNot(tok::annot_pragma_openmp_end)) {
          TPA.Commit();
          IsCorrect = false;
          break;
}
        IsCorrect =
            !T.consumeClose() && IsCorrect && !InitializerResult.isInvalid();
}
}
    ++I;
    if (I != E)
      TPA.Revert();
    else
      TPA.Commit();
}
  return Actions.ActOnOpenMPDeclareReductionDirectiveEnd(getCurScope(), DRD,
                                                         IsCorrect);
}
namespace {
class FNContextRAII final {
  Parser &P;
  Sema::CXXThisScopeRAII *ThisScope;
  Parser::ParseScope *TempScope;
  Parser::ParseScope *FnScope;
  bool HasTemplateScope = false;
  bool HasFunScope = false;
  FNContextRAII() = delete;
  FNContextRAII(const FNContextRAII &) = delete;
  FNContextRAII &operator=(const FNContextRAII &) = delete;
public:
  FNContextRAII(Parser &P, Parser::DeclGroupPtrTy Ptr) : P(P) {
    Decl *D = *Ptr.get().begin();
    NamedDecl *ND = dyn_cast<NamedDecl>(D);
    RecordDecl *RD = dyn_cast_or_null<RecordDecl>(D->getDeclContext());
    Sema &Actions = P.getActions();
    ThisScope = new Sema::CXXThisScopeRAII(Actions, RD, 
0,
                                           ND && ND->isCXXInstanceMember());
    HasTemplateScope = D->isTemplateDecl();
    TempScope =
        new Parser::ParseScope(&P, Scope::TemplateParamScope, HasTemplateScope);
    if (HasTemplateScope)
      Actions.ActOnReenterTemplateScope(Actions.getCurScope(), D);
    HasFunScope = D->isFunctionOrFunctionTemplate();
    FnScope = new Parser::ParseScope(&P, Scope::FnScope | Scope::DeclScope,
                                     HasFunScope);
    if (HasFunScope)
      Actions.ActOnReenterFunctionContext(Actions.getCurScope(), D);
}
  ~FNContextRAII() {
    if (HasFunScope) {
      P.getActions().ActOnExitFunctionContext();
      FnScope->Exit(); 
}
    if (HasTemplateScope)
      TempScope->Exit();
    delete FnScope;
    delete TempScope;
    delete ThisScope;
}
};
} 
static bool parseDeclareSimdClauses(
    Parser &P, OMPDeclareSimdDeclAttr::BranchStateTy &BS, ExprResult &SimdLen,
    SmallVectorImpl<Expr *> &Uniforms, SmallVectorImpl<Expr *> &Aligneds,
    SmallVectorImpl<Expr *> &Alignments, SmallVectorImpl<Expr *> &Linears,
    SmallVectorImpl<unsigned> &LinModifiers, SmallVectorImpl<Expr *> &Steps) {
  SourceRange BSRange;
  const Token &Tok = P.getCurToken();
  bool IsError = false;
  while (Tok.isNot(tok::annot_pragma_openmp_end)) {
    if (Tok.isNot(tok::identifier))
      break;
    OMPDeclareSimdDeclAttr::BranchStateTy Out;
    IdentifierInfo *II = Tok.getIdentifierInfo();
    StringRef ClauseName = II->getName();
    if (OMPDeclareSimdDeclAttr::ConvertStrToBranchStateTy(ClauseName, Out)) {
      if (BS != OMPDeclareSimdDeclAttr::BS_Undefined && BS != Out) {
        P.Diag(Tok, diag::err_omp_declare_simd_inbranch_notinbranch)
            << ClauseName
            << OMPDeclareSimdDeclAttr::ConvertBranchStateTyToStr(BS) << BSRange;
        IsError = true;
}
      BS = Out;
      BSRange = SourceRange(Tok.getLocation(), Tok.getEndLoc());
      P.ConsumeToken();
} else if (ClauseName.equals("simdlen")) {
      if (SimdLen.isUsable()) {
        P.Diag(Tok, diag::err_omp_more_one_clause)
            << getOpenMPDirectiveName(OMPD_declare_simd) << ClauseName << 0;
        IsError = true;
}
      P.ConsumeToken();
      SourceLocation RLoc;
      SimdLen = P.ParseOpenMPParensExpr(ClauseName, RLoc);
      if (SimdLen.isInvalid())
        IsError = true;
} else {
      OpenMPClauseKind CKind = getOpenMPClauseKind(ClauseName);
      if (CKind == OMPC_uniform || CKind == OMPC_aligned ||
          CKind == OMPC_linear) {
        Parser::OpenMPVarListDataTy Data;
        auto *Vars = &Uniforms;
        if (CKind == OMPC_aligned)
          Vars = &Aligneds;
        else if (CKind == OMPC_linear)
          Vars = &Linears;
        P.ConsumeToken();
        if (P.ParseOpenMPVarList(OMPD_declare_simd,
                                 getOpenMPClauseKind(ClauseName), *Vars, Data))
          IsError = true;
        if (CKind == OMPC_aligned)
          Alignments.append(Aligneds.size() - Alignments.size(), Data.TailExpr);
        else if (CKind == OMPC_linear) {
          if (P.getActions().CheckOpenMPLinearModifier(Data.LinKind,
                                                       Data.DepLinMapLoc))
            Data.LinKind = OMPC_LINEAR_val;
          LinModifiers.append(Linears.size() - LinModifiers.size(),
                              Data.LinKind);
          Steps.append(Linears.size() - Steps.size(), Data.TailExpr);
}
} else
        break;
}
    if (Tok.is(tok::comma))
      P.ConsumeToken();
}
  return IsError;
}
Parser::DeclGroupPtrTy
Parser::ParseOMPDeclareSimdClauses(Parser::DeclGroupPtrTy Ptr,
                                   CachedTokens &Toks, SourceLocation Loc) {
  PP.EnterToken(Tok);
  PP.EnterTokenStream(Toks, 
true);
  ConsumeAnyToken(
true);
  FNContextRAII FnContext(*this, Ptr);
  OMPDeclareSimdDeclAttr::BranchStateTy BS =
      OMPDeclareSimdDeclAttr::BS_Undefined;
  ExprResult Simdlen;
  SmallVector<Expr *, 4> Uniforms;
  SmallVector<Expr *, 4> Aligneds;
  SmallVector<Expr *, 4> Alignments;
  SmallVector<Expr *, 4> Linears;
  SmallVector<unsigned, 4> LinModifiers;
  SmallVector<Expr *, 4> Steps;
  bool IsError =
      parseDeclareSimdClauses(*this, BS, Simdlen, Uniforms, Aligneds,
                              Alignments, Linears, LinModifiers, Steps);
  if (Tok.isNot(tok::annot_pragma_openmp_end)) {
    Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
        << getOpenMPDirectiveName(OMPD_declare_simd);
    while (Tok.isNot(tok::annot_pragma_openmp_end))
      ConsumeAnyToken();
}
  SourceLocation EndLoc = ConsumeToken();
  if (!IsError) {
    return Actions.ActOnOpenMPDeclareSimdDirective(
        Ptr, BS, Simdlen.get(), Uniforms, Aligneds, Alignments, Linears,
        LinModifiers, Steps, SourceRange(Loc, EndLoc));
}
  return Ptr;
}
Parser::DeclGroupPtrTy Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(
    AccessSpecifier &AS, ParsedAttributesWithRange &Attrs,
    DeclSpec::TST TagType, Decl *Tag) {
  assert(Tok.is(tok::annot_pragma_openmp) && "Not an OpenMP directive!");
  ParenBraceBracketBalancer BalancerRAIIObj(*this);
  SourceLocation Loc = ConsumeToken();
  auto DKind = ParseOpenMPDirectiveKind(*this);
  switch (DKind) {
  case OMPD_threadprivate: {
    ConsumeToken();
    ThreadprivateListParserHelper Helper(this);
    if (!ParseOpenMPSimpleVarList(OMPD_threadprivate, Helper, true)) {
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
            << getOpenMPDirectiveName(OMPD_threadprivate);
        SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
}
      ConsumeToken();
      return Actions.ActOnOpenMPThreadprivateDirective(Loc,
                                                       Helper.getIdentifiers());
}
    break;
}
  case OMPD_declare_reduction:
    ConsumeToken();
    if (auto Res = ParseOpenMPDeclareReductionDirective(AS)) {
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
            << getOpenMPDirectiveName(OMPD_declare_reduction);
        while (Tok.isNot(tok::annot_pragma_openmp_end))
          ConsumeAnyToken();
}
      ConsumeToken();
      return Res;
}
    break;
  case OMPD_declare_simd: {
    ConsumeToken();
    CachedTokens Toks;
    while(Tok.isNot(tok::annot_pragma_openmp_end)) {
      Toks.push_back(Tok);
      ConsumeAnyToken();
}
    Toks.push_back(Tok);
    ConsumeAnyToken();
    DeclGroupPtrTy Ptr;
    if (Tok.is(tok::annot_pragma_openmp))
      Ptr = ParseOpenMPDeclarativeDirectiveWithExtDecl(AS, Attrs, TagType, Tag);
    else if (Tok.isNot(tok::r_brace) && !isEofOrEom()) {
      if (AS == AS_none) {
        assert(TagType == DeclSpec::TST_unspecified);
        MaybeParseCXX11Attributes(Attrs);
        ParsingDeclSpec PDS(*this);
        Ptr = ParseExternalDeclaration(Attrs, &PDS);
} else {
        Ptr =
            ParseCXXClassMemberDeclarationWithPragmas(AS, Attrs, TagType, Tag);
}
}
    if (!Ptr) {
      Diag(Loc, diag::err_omp_decl_in_declare_simd);
      return DeclGroupPtrTy();
}
    return ParseOMPDeclareSimdClauses(Ptr, Toks, Loc);
}
  case OMPD_declare_target: {
    SourceLocation DTLoc = ConsumeAnyToken();
    if (Tok.isNot(tok::annot_pragma_openmp_end)) {
      llvm::SmallSetVector<const NamedDecl*, 16> SameDirectiveDecls;
      while (Tok.isNot(tok::annot_pragma_openmp_end)) {
        OMPDeclareTargetDeclAttr::MapTypeTy MT =
            OMPDeclareTargetDeclAttr::MT_To;
        if (Tok.is(tok::identifier)) {
          IdentifierInfo *II = Tok.getIdentifierInfo();
          StringRef ClauseName = II->getName();
          if (!OMPDeclareTargetDeclAttr::ConvertStrToMapTypeTy(ClauseName,
                                                               MT)) {
            Diag(Tok, diag::err_omp_declare_target_unexpected_clause)
                << ClauseName;
            break;
}
          ConsumeToken();
}
        auto Callback = [this, MT, &SameDirectiveDecls](
            CXXScopeSpec &SS, DeclarationNameInfo NameInfo) {
          Actions.ActOnOpenMPDeclareTargetName(getCurScope(), SS, NameInfo, MT,
                                               SameDirectiveDecls);
};
        if (ParseOpenMPSimpleVarList(OMPD_declare_target, Callback, true))
          break;
        if (Tok.is(tok::comma))
          ConsumeToken();
}
      SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
      ConsumeAnyToken();
      return DeclGroupPtrTy();
}
    ConsumeAnyToken();
    if (!Actions.ActOnStartOpenMPDeclareTargetDirective(DTLoc))
      return DeclGroupPtrTy();
    DKind = ParseOpenMPDirectiveKind(*this);
    while (DKind != OMPD_end_declare_target && DKind != OMPD_declare_target &&
           Tok.isNot(tok::eof) && Tok.isNot(tok::r_brace)) {
      ParsedAttributesWithRange attrs(AttrFactory);
      MaybeParseCXX11Attributes(attrs);
      ParseExternalDeclaration(attrs);
      if (Tok.isAnnotation() && Tok.is(tok::annot_pragma_openmp)) {
        TentativeParsingAction TPA(*this);
        ConsumeToken();
        DKind = ParseOpenMPDirectiveKind(*this);
        if (DKind != OMPD_end_declare_target)
          TPA.Revert();
        else
          TPA.Commit();
}
}
    if (DKind == OMPD_end_declare_target) {
      ConsumeAnyToken();
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
            << getOpenMPDirectiveName(OMPD_end_declare_target);
        SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
}
      ConsumeAnyToken();
} else {
      Diag(Tok, diag::err_expected_end_declare_target);
      Diag(DTLoc, diag::note_matching) << "'#pragma omp declare target'";
}
    Actions.ActOnFinishOpenMPDeclareTargetDirective();
    return DeclGroupPtrTy();
}
  case OMPD_unknown:
    Diag(Tok, diag::err_omp_unknown_directive);
    break;
  case OMPD_parallel:
  case OMPD_simd:
  case OMPD_task:
  case OMPD_taskyield:
  case OMPD_barrier:
  case OMPD_taskwait:
  case OMPD_taskgroup:
  case OMPD_flush:
  case OMPD_inc:
  case OMPD_for:
  case OMPD_for_simd:
  case OMPD_sections:
  case OMPD_section:
  case OMPD_single:
  case OMPD_master:
  case OMPD_ordered:
  case OMPD_critical:
  case OMPD_parallel_for:
  case OMPD_parallel_for_simd:
  case OMPD_parallel_sections:
  case OMPD_atomic:
  case OMPD_target:
  case OMPD_teams:
  case OMPD_cancellation_point:
  case OMPD_cancel:
  case OMPD_target_data:
  case OMPD_target_enter_data:
  case OMPD_target_exit_data:
  case OMPD_target_parallel:
  case OMPD_target_parallel_for:
  case OMPD_taskloop:
  case OMPD_taskloop_simd:
  case OMPD_distribute:
  case OMPD_end_declare_target:
  case OMPD_target_update:
  case OMPD_distribute_parallel_for:
  case OMPD_distribute_parallel_for_simd:
  case OMPD_distribute_simd:
  case OMPD_target_parallel_for_simd:
  case OMPD_target_simd:
  case OMPD_teams_distribute:
  case OMPD_teams_distribute_simd:
  case OMPD_teams_distribute_parallel_for_simd:
  case OMPD_teams_distribute_parallel_for:
  case OMPD_target_teams:
  case OMPD_target_teams_distribute:
  case OMPD_target_teams_distribute_parallel_for:
  case OMPD_target_teams_distribute_parallel_for_simd:
  case OMPD_target_teams_distribute_simd:
    Diag(Tok, diag::err_omp_unexpected_directive)
        << getOpenMPDirectiveName(DKind);
    break;
}
  while (Tok.isNot(tok::annot_pragma_openmp_end))
    ConsumeAnyToken();
  ConsumeAnyToken();
  return nullptr;
}
StmtResult Parser::ParseOpenMPDeclarativeOrExecutableDirective(
    AllowedContsructsKind Allowed) {
  assert(Tok.is(tok::annot_pragma_openmp) && "Not an OpenMP directive!");
  ParenBraceBracketBalancer BalancerRAIIObj(*this);
  SmallVector<OMPClause *, 5> Clauses;
  SmallVector<llvm::PointerIntPair<OMPClause *, 1, bool>, OMPC_unknown + 1>
  FirstClauses(OMPC_unknown + 1);
  unsigned ScopeFlags =
      Scope::FnScope | Scope::DeclScope | Scope::OpenMPDirectiveScope;
  SourceLocation Loc = ConsumeToken(), EndLoc;
  auto DKind = ParseOpenMPDirectiveKind(*this);
  OpenMPDirectiveKind CancelRegion = OMPD_unknown;
  DeclarationNameInfo DirName;
  StmtResult Directive = StmtError();
  bool HasAssociatedStatement = true;
  bool FlushHasClause = false;
  bool IncHasClause = false;
  switch (DKind) {
  case OMPD_threadprivate: {
    if (Allowed != ACK_Any) {
      Diag(Tok, diag::err_omp_immediate_directive)
          << getOpenMPDirectiveName(DKind) << 0;
}
    ConsumeToken();
    ThreadprivateListParserHelper Helper(this);
    if (!ParseOpenMPSimpleVarList(OMPD_threadprivate, Helper, false)) {
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
            << getOpenMPDirectiveName(OMPD_threadprivate);
        SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
}
      DeclGroupPtrTy Res = Actions.ActOnOpenMPThreadprivateDirective(
          Loc, Helper.getIdentifiers());
      Directive = Actions.ActOnDeclStmt(Res, Loc, Tok.getLocation());
}
    SkipUntil(tok::annot_pragma_openmp_end);
    break;
}
  case OMPD_declare_reduction:
    ConsumeToken();
    if (auto Res = ParseOpenMPDeclareReductionDirective(
AS_none)) {
      if (Tok.isNot(tok::annot_pragma_openmp_end)) {
        Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
            << getOpenMPDirectiveName(OMPD_declare_reduction);
        while (Tok.isNot(tok::annot_pragma_openmp_end))
          ConsumeAnyToken();
}
      ConsumeAnyToken();
      Directive = Actions.ActOnDeclStmt(Res, Loc, Tok.getLocation());
} else
      SkipUntil(tok::annot_pragma_openmp_end);
    break;
  case OMPD_inc:
    if (PP.LookAhead(0).is(tok::l_paren)) {
      IncHasClause = true;
      PP.EnterToken(Tok);
}
    else {
      Diag(Tok, diag::err_expected_lparen_after)
          << getOpenMPDirectiveName(DKind) << 0;
}
   case OMPD_flush:
    if (PP.LookAhead(0).is(tok::l_paren && !IncHasClause)) {
      FlushHasClause = true;
      PP.EnterToken(Tok);
}
  case OMPD_taskyield:
  case OMPD_barrier:
  case OMPD_taskwait:
  case OMPD_cancellation_point:
  case OMPD_cancel:
  case OMPD_target_enter_data:
  case OMPD_target_exit_data:
  case OMPD_target_update:
    if (Allowed == ACK_StatementsOpenMPNonStandalone) {
      Diag(Tok, diag::err_omp_immediate_directive)
          << getOpenMPDirectiveName(DKind) << 0;
}
    HasAssociatedStatement = false;
  case OMPD_parallel:
  case OMPD_simd:
  case OMPD_for:
  case OMPD_for_simd:
  case OMPD_sections:
  case OMPD_single:
  case OMPD_section:
  case OMPD_master:
  case OMPD_critical:
  case OMPD_parallel_for:
  case OMPD_parallel_for_simd:
  case OMPD_parallel_sections:
  case OMPD_task:
  case OMPD_ordered:
  case OMPD_atomic:
  case OMPD_target:
  case OMPD_teams:
  case OMPD_taskgroup:
  case OMPD_target_data:
  case OMPD_target_parallel:
  case OMPD_target_parallel_for:
  case OMPD_taskloop:
  case OMPD_taskloop_simd:
  case OMPD_distribute:
  case OMPD_distribute_parallel_for:
  case OMPD_distribute_parallel_for_simd:
  case OMPD_distribute_simd:
  case OMPD_target_parallel_for_simd:
  case OMPD_target_simd:
  case OMPD_teams_distribute:
  case OMPD_teams_distribute_simd:
  case OMPD_teams_distribute_parallel_for_simd:
  case OMPD_teams_distribute_parallel_for:
  case OMPD_target_teams:
  case OMPD_target_teams_distribute:
  case OMPD_target_teams_distribute_parallel_for:
  case OMPD_target_teams_distribute_parallel_for_simd:
  case OMPD_target_teams_distribute_simd: {
    ConsumeToken();
    if (DKind == OMPD_critical) {
      BalancedDelimiterTracker T(*this, tok::l_paren,
                                 tok::annot_pragma_openmp_end);
      if (!T.consumeOpen()) {
        if (Tok.isAnyIdentifier()) {
          DirName =
              DeclarationNameInfo(Tok.getIdentifierInfo(), Tok.getLocation());
          ConsumeAnyToken();
} else {
          Diag(Tok, diag::err_omp_expected_identifier_for_critical);
}
        T.consumeClose();
}
} else if (DKind == OMPD_cancellation_point || DKind == OMPD_cancel) {
      CancelRegion = ParseOpenMPDirectiveKind(*this);
      if (Tok.isNot(tok::annot_pragma_openmp_end))
        ConsumeToken();
}
    if (isOpenMPLoopDirective(DKind))
      ScopeFlags |= Scope::OpenMPLoopDirectiveScope;
    if (isOpenMPSimdDirective(DKind))
      ScopeFlags |= Scope::OpenMPSimdDirectiveScope;
    ParseScope OMPDirectiveScope(this, ScopeFlags);
    Actions.StartOpenMPDSABlock(DKind, DirName, Actions.getCurScope(), Loc);
    while (Tok.isNot(tok::annot_pragma_openmp_end)) {
      OpenMPClauseKind CKind =
          Tok.isAnnotation()
              ? OMPC_unknown
              : FlushHasClause ? OMPC_flush
                               : IncHasClause ? OMPC_inc
                               		      : getOpenMPClauseKind(PP.getSpelling(Tok));
      Actions.StartOpenMPClause(CKind);
      if(FlushHasClause) FlushHasClause = false;
      else if(IncHasClause) IncHasClause = false;
      OMPClause *Clause =
          ParseOpenMPClause(DKind, CKind, !FirstClauses[CKind].getInt());
      FirstClauses[CKind].setInt(true);
      if (Clause) {
        FirstClauses[CKind].setPointer(Clause);
        Clauses.push_back(Clause);
}
      if (Tok.is(tok::comma))
        ConsumeToken();
      Actions.EndOpenMPClause();
}
    EndLoc = Tok.getLocation();
    ConsumeToken();
    if (DKind == OMPD_ordered && FirstClauses[OMPC_depend].getInt()) {
      if (Allowed == ACK_StatementsOpenMPNonStandalone) {
        Diag(Loc, diag::err_omp_immediate_directive)
            << getOpenMPDirectiveName(DKind) << 1
            << getOpenMPClauseName(OMPC_depend);
}
      HasAssociatedStatement = false;
}
    StmtResult AssociatedStmt;
    if (HasAssociatedStatement) {
      Sema::CompoundScopeRAII CompoundScope(Actions);
      Actions.ActOnOpenMPRegionStart(DKind, getCurScope());
      Actions.ActOnStartOfCompoundStmt();
      AssociatedStmt = ParseStatement();
      Actions.ActOnFinishOfCompoundStmt();
      AssociatedStmt = Actions.ActOnOpenMPRegionEnd(AssociatedStmt, Clauses);
}
    Directive = Actions.ActOnOpenMPExecutableDirective(
        DKind, DirName, CancelRegion, Clauses, AssociatedStmt.get(), Loc,
        EndLoc);
    Actions.EndOpenMPDSABlock(Directive.get());
    OMPDirectiveScope.Exit();
    break;
}
  case OMPD_declare_simd:
  case OMPD_declare_target:
  case OMPD_end_declare_target:
    Diag(Tok, diag::err_omp_unexpected_directive)
        << getOpenMPDirectiveName(DKind);
    SkipUntil(tok::annot_pragma_openmp_end);
    break;
  case OMPD_unknown:
    Diag(Tok, diag::err_omp_unknown_directive);
    SkipUntil(tok::annot_pragma_openmp_end);
    break;
}
  return Directive;
}
bool Parser::ParseOpenMPSimpleVarList(
    OpenMPDirectiveKind Kind,
    const llvm::function_ref<void(CXXScopeSpec &, DeclarationNameInfo)> &
        Callback,
    bool AllowScopeSpecifier) {
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after,
                         getOpenMPDirectiveName(Kind)))
    return true;
  bool IsCorrect = true;
  bool NoIdentIsFound = true;
  while (Tok.isNot(tok::r_paren) && Tok.isNot(tok::annot_pragma_openmp_end)) {
    CXXScopeSpec SS;
    SourceLocation TemplateKWLoc;
    UnqualifiedId Name;
    Token PrevTok = Tok;
    NoIdentIsFound = false;
    if (AllowScopeSpecifier && getLangOpts().CPlusPlus &&
        ParseOptionalCXXScopeSpecifier(SS, nullptr, false)) {
      IsCorrect = false;
      SkipUntil(tok::comma, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
} else if (ParseUnqualifiedId(SS, false, false, false, nullptr,
                                  TemplateKWLoc, Name)) {
      IsCorrect = false;
      SkipUntil(tok::comma, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
} else if (Tok.isNot(tok::comma) && Tok.isNot(tok::r_paren) &&
               Tok.isNot(tok::annot_pragma_openmp_end)) {
      IsCorrect = false;
      SkipUntil(tok::comma, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
      Diag(PrevTok.getLocation(), diag::err_expected)
          << tok::identifier
          << SourceRange(PrevTok.getLocation(), PrevTokLocation);
} else {
      Callback(SS, Actions.GetNameFromUnqualifiedId(Name));
}
    if (Tok.is(tok::comma)) {
      ConsumeToken();
}
}
  if (NoIdentIsFound) {
    Diag(Tok, diag::err_expected) << tok::identifier;
    IsCorrect = false;
}
  IsCorrect = !T.consumeClose() && IsCorrect;
  return !IsCorrect;
}
OMPClause *Parser::ParseOpenMPClause(OpenMPDirectiveKind DKind,
                                     OpenMPClauseKind CKind, bool FirstClause) {
  OMPClause *Clause = nullptr;
  bool ErrorFound = false;
  if (CKind != OMPC_unknown && !isAllowedClauseForDirective(DKind, CKind)) {
    Diag(Tok, diag::err_omp_unexpected_clause) << getOpenMPClauseName(CKind)
                                               << getOpenMPDirectiveName(DKind);
    ErrorFound = true;
}
  switch (CKind) {
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_collapse:
  case OMPC_ordered:
  case OMPC_device:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_num_tasks:
  case OMPC_hint:
    if (!FirstClause) {
      Diag(Tok, diag::err_omp_more_one_clause)
          << getOpenMPDirectiveName(DKind) << getOpenMPClauseName(CKind) << 0;
      ErrorFound = true;
}
    if (CKind == OMPC_ordered && PP.LookAhead(
0).isNot(tok::l_paren))
      Clause = ParseOpenMPClause(CKind);
    else
      Clause = ParseOpenMPSingleExprClause(CKind);
    break;
  case OMPC_default:
  case OMPC_proc_bind:
    if (!FirstClause) {
      Diag(Tok, diag::err_omp_more_one_clause)
          << getOpenMPDirectiveName(DKind) << getOpenMPClauseName(CKind) << 0;
      ErrorFound = true;
}
    Clause = ParseOpenMPSimpleClause(CKind);
    break;
  case OMPC_schedule:
  case OMPC_dist_schedule:
  case OMPC_defaultmap:
    if (!FirstClause) {
      Diag(Tok, diag::err_omp_more_one_clause)
          << getOpenMPDirectiveName(DKind) << getOpenMPClauseName(CKind) << 0;
      ErrorFound = true;
}
  case OMPC_if:
    Clause = ParseOpenMPSingleExprWithArgClause(CKind);
    break;
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_nogroup:
    if (!FirstClause) {
      Diag(Tok, diag::err_omp_more_one_clause)
          << getOpenMPDirectiveName(DKind) << getOpenMPClauseName(CKind) << 0;
      ErrorFound = true;
}
    Clause = ParseOpenMPClause(CKind);
    break;
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_lastprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_linear:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_flush:
  case OMPC_inc:
  case OMPC_depend:
  case OMPC_map:
  case OMPC_to:
  case OMPC_from:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
    Clause = ParseOpenMPVarListClause(DKind, CKind);
    break;
  case OMPC_unknown:
    Diag(Tok, diag::warn_omp_extra_tokens_at_eol)
        << getOpenMPDirectiveName(DKind);
    SkipUntil(tok::annot_pragma_openmp_end, StopBeforeMatch);
    break;
  case OMPC_threadprivate:
  case OMPC_uniform:
    Diag(Tok, diag::err_omp_unexpected_clause) << getOpenMPClauseName(CKind)
                                               << getOpenMPDirectiveName(DKind);
    SkipUntil(tok::comma, tok::annot_pragma_openmp_end, StopBeforeMatch);
    break;
}
  return ErrorFound ? nullptr : Clause;
}
ExprResult Parser::ParseOpenMPParensExpr(StringRef ClauseName,
                                         SourceLocation &RLoc) {
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after, ClauseName.data()))
    return ExprError();
  SourceLocation ELoc = Tok.getLocation();
  ExprResult LHS(ParseCastExpression(
false, 
false, NotTypeCast));
  ExprResult Val(ParseRHSOfBinaryExpression(LHS, prec::Conditional));
  Val = Actions.ActOnFinishFullExpr(Val.get(), ELoc);
  T.consumeClose();
  RLoc = T.getCloseLocation();
  return Val;
}
OMPClause *Parser::ParseOpenMPSingleExprClause(OpenMPClauseKind Kind) {
  SourceLocation Loc = ConsumeToken();
  SourceLocation LLoc = Tok.getLocation();
  SourceLocation RLoc;
  ExprResult Val = ParseOpenMPParensExpr(getOpenMPClauseName(Kind), RLoc);
  if (Val.isInvalid())
    return nullptr;
  return Actions.ActOnOpenMPSingleExprClause(Kind, Val.get(), Loc, LLoc, RLoc);
}
OMPClause *Parser::ParseOpenMPSimpleClause(OpenMPClauseKind Kind) {
  SourceLocation Loc = Tok.getLocation();
  SourceLocation LOpen = ConsumeToken();
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after,
                         getOpenMPClauseName(Kind)))
    return nullptr;
  unsigned Type = getOpenMPSimpleClauseType(
      Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok));
  SourceLocation TypeLoc = Tok.getLocation();
  if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
      Tok.isNot(tok::annot_pragma_openmp_end))
    ConsumeAnyToken();
  T.consumeClose();
  return Actions.ActOnOpenMPSimpleClause(Kind, Type, TypeLoc, LOpen, Loc,
                                         Tok.getLocation());
}
OMPClause *Parser::ParseOpenMPClause(OpenMPClauseKind Kind) {
  SourceLocation Loc = Tok.getLocation();
  ConsumeAnyToken();
  return Actions.ActOnOpenMPClause(Kind, Loc, Tok.getLocation());
}
OMPClause *Parser::ParseOpenMPSingleExprWithArgClause(OpenMPClauseKind Kind) {
  SourceLocation Loc = ConsumeToken();
  SourceLocation DelimLoc;
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after,
                         getOpenMPClauseName(Kind)))
    return nullptr;
  ExprResult Val;
  SmallVector<unsigned, 4> Arg;
  SmallVector<SourceLocation, 4> KLoc;
  if (Kind == OMPC_schedule) {
    enum { Modifier1, Modifier2, ScheduleKind, NumberOfElements 
};
    Arg.resize(NumberOfElements);
    KLoc.resize(NumberOfElements);
    Arg[Modifier1] = OMPC_SCHEDULE_MODIFIER_unknown;
    Arg[Modifier2] = OMPC_SCHEDULE_MODIFIER_unknown;
    Arg[ScheduleKind] = OMPC_SCHEDULE_unknown;
    auto KindModifier = getOpenMPSimpleClauseType(
        Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok));
    if (KindModifier > OMPC_SCHEDULE_unknown) {
      Arg[Modifier1] = KindModifier;
      KLoc[Modifier1] = Tok.getLocation();
      if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
          Tok.isNot(tok::annot_pragma_openmp_end))
        ConsumeAnyToken();
      if (Tok.is(tok::comma)) {
        ConsumeAnyToken();
        KindModifier = getOpenMPSimpleClauseType(
            Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok));
        Arg[Modifier2] = KindModifier > OMPC_SCHEDULE_unknown
                             ? KindModifier
                             : (unsigned)OMPC_SCHEDULE_unknown;
        KLoc[Modifier2] = Tok.getLocation();
        if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
            Tok.isNot(tok::annot_pragma_openmp_end))
          ConsumeAnyToken();
}
      if (Tok.is(tok::colon))
        ConsumeAnyToken();
      else
        Diag(Tok, diag::warn_pragma_expected_colon) << "schedule modifier";
      KindModifier = getOpenMPSimpleClauseType(
          Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok));
}
    Arg[ScheduleKind] = KindModifier;
    KLoc[ScheduleKind] = Tok.getLocation();
    if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
        Tok.isNot(tok::annot_pragma_openmp_end))
      ConsumeAnyToken();
    if ((Arg[ScheduleKind] == OMPC_SCHEDULE_static ||
         Arg[ScheduleKind] == OMPC_SCHEDULE_dynamic ||
         Arg[ScheduleKind] == OMPC_SCHEDULE_guided) &&
        Tok.is(tok::comma))
      DelimLoc = ConsumeAnyToken();
} else if (Kind == OMPC_dist_schedule) {
    Arg.push_back(getOpenMPSimpleClauseType(
        Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok)));
    KLoc.push_back(Tok.getLocation());
    if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
        Tok.isNot(tok::annot_pragma_openmp_end))
      ConsumeAnyToken();
    if (Arg.back() == OMPC_DIST_SCHEDULE_static && Tok.is(tok::comma))
      DelimLoc = ConsumeAnyToken();
} else if (Kind == OMPC_defaultmap) {
    Arg.push_back(getOpenMPSimpleClauseType(
        Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok)));
    KLoc.push_back(Tok.getLocation());
    if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
        Tok.isNot(tok::annot_pragma_openmp_end))
      ConsumeAnyToken();
    if (Tok.is(tok::colon))
      ConsumeAnyToken();
    else if (Arg.back() != OMPC_DEFAULTMAP_MODIFIER_unknown)
      Diag(Tok, diag::warn_pragma_expected_colon) << "defaultmap modifier";
    Arg.push_back(getOpenMPSimpleClauseType(
        Kind, Tok.isAnnotation() ? "" : PP.getSpelling(Tok)));
    KLoc.push_back(Tok.getLocation());
    if (Tok.isNot(tok::r_paren) && Tok.isNot(tok::comma) &&
        Tok.isNot(tok::annot_pragma_openmp_end))
      ConsumeAnyToken();
} else {
    assert(Kind == OMPC_if);
    KLoc.push_back(Tok.getLocation());
    TentativeParsingAction TPA(*this);
    Arg.push_back(ParseOpenMPDirectiveKind(*this));
    if (Arg.back() != OMPD_unknown) {
      ConsumeToken();
      if (Tok.is(tok::colon) && getLangOpts().OpenMP > 40) {
        TPA.Commit();
        DelimLoc = ConsumeToken();
} else {
        TPA.Revert();
        Arg.back() = OMPD_unknown;
}
} else
      TPA.Revert();
}
  bool NeedAnExpression = (Kind == OMPC_schedule && DelimLoc.isValid()) ||
                          (Kind == OMPC_dist_schedule && DelimLoc.isValid()) ||
                          Kind == OMPC_if;
  if (NeedAnExpression) {
    SourceLocation ELoc = Tok.getLocation();
    ExprResult LHS(ParseCastExpression(false, false, NotTypeCast));
    Val = ParseRHSOfBinaryExpression(LHS, prec::Conditional);
    Val = Actions.ActOnFinishFullExpr(Val.get(), ELoc);
}
  T.consumeClose();
  if (NeedAnExpression && Val.isInvalid())
    return nullptr;
  return Actions.ActOnOpenMPSingleExprWithArgClause(
      Kind, Arg, Val.get(), Loc, T.getOpenLocation(), KLoc, DelimLoc,
      T.getCloseLocation());
}
static bool ParseReductionId(Parser &P, CXXScopeSpec &ReductionIdScopeSpec,
                             UnqualifiedId &ReductionId) {
  SourceLocation TemplateKWLoc;
  if (ReductionIdScopeSpec.isEmpty()) {
    auto OOK = OO_None;
    switch (P.getCurToken().getKind()) {
    case tok::plus:
      OOK = OO_Plus;
      break;
    case tok::minus:
      OOK = OO_Minus;
      break;
    case tok::star:
      OOK = OO_Star;
      break;
    case tok::amp:
      OOK = OO_Amp;
      break;
    case tok::pipe:
      OOK = OO_Pipe;
      break;
    case tok::caret:
      OOK = OO_Caret;
      break;
    case tok::ampamp:
      OOK = OO_AmpAmp;
      break;
    case tok::pipepipe:
      break;
      OOK = OO_PipePipe;
    default:
      break;
}
    if (OOK != OO_None) {
      SourceLocation OpLoc = P.ConsumeToken();
      SourceLocation SymbolLocations[] = {OpLoc, OpLoc, SourceLocation()
};
      ReductionId.setOperatorFunctionId(OpLoc, OOK, SymbolLocations);
      return false;
}
}
  return P.ParseUnqualifiedId(ReductionIdScopeSpec, 
 false,
 false,
 false, nullptr,
                              TemplateKWLoc, ReductionId);
}
bool Parser::ParseOpenMPVarList(OpenMPDirectiveKind DKind,
                                OpenMPClauseKind Kind,
                                SmallVectorImpl<Expr *> &Vars,
                                OpenMPVarListDataTy &Data) {
  UnqualifiedId UnqualifiedReductionId;
  bool InvalidReductionId = false;
  bool MapTypeModifierSpecified = false;
  BalancedDelimiterTracker T(*this, tok::l_paren, tok::annot_pragma_openmp_end);
  if (T.expectAndConsume(diag::err_expected_lparen_after,
                         getOpenMPClauseName(Kind)))
    return true;
  bool NeedRParenForLinear = false;
  BalancedDelimiterTracker LinearT(*this, tok::l_paren,
                                  tok::annot_pragma_openmp_end);
  if (Kind == OMPC_reduction) {
    ColonProtectionRAIIObject ColonRAII(*this);
    if (getLangOpts().CPlusPlus)
      ParseOptionalCXXScopeSpecifier(Data.ReductionIdScopeSpec,
nullptr,
false);
    InvalidReductionId = ParseReductionId(*this, Data.ReductionIdScopeSpec,
                                          UnqualifiedReductionId);
    if (InvalidReductionId) {
      SkipUntil(tok::colon, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
}
    if (Tok.is(tok::colon))
      Data.ColonLoc = ConsumeToken();
    else
      Diag(Tok, diag::warn_pragma_expected_colon) << "reduction identifier";
    if (!InvalidReductionId)
      Data.ReductionId =
          Actions.GetNameFromUnqualifiedId(UnqualifiedReductionId);
} else if (Kind == OMPC_depend) {
    ColonProtectionRAIIObject ColonRAII(*this);
    Data.DepKind =
        static_cast<OpenMPDependClauseKind>(getOpenMPSimpleClauseType(
            Kind, Tok.is(tok::identifier) ? PP.getSpelling(Tok) : ""));
    Data.DepLinMapLoc = Tok.getLocation();
    if (Data.DepKind == OMPC_DEPEND_unknown) {
      SkipUntil(tok::colon, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
} else {
      ConsumeToken();
      if (DKind == OMPD_ordered && Data.DepKind == OMPC_DEPEND_source) {
        T.consumeClose();
        return false;
}
}
    if (Tok.is(tok::colon))
      Data.ColonLoc = ConsumeToken();
    else {
      Diag(Tok, DKind == OMPD_ordered ? diag::warn_pragma_expected_colon_r_paren
                                      : diag::warn_pragma_expected_colon)
          << "dependency type";
}
} else if (Kind == OMPC_linear) {
    if (Tok.is(tok::identifier) && PP.LookAhead(0).is(tok::l_paren)) {
      Data.LinKind = static_cast<OpenMPLinearClauseKind>(
          getOpenMPSimpleClauseType(Kind, PP.getSpelling(Tok)));
      Data.DepLinMapLoc = ConsumeToken();
      LinearT.consumeOpen();
      NeedRParenForLinear = true;
}
} else if (Kind == OMPC_map) {
    ColonProtectionRAIIObject ColonRAII(*this);
    auto &&IsMapClauseModifierToken = [](const Token &Tok) -> bool {
      return Tok.isOneOf(tok::identifier, tok::kw_delete);
};
    Data.MapType =
        IsMapClauseModifierToken(Tok)
            ? static_cast<OpenMPMapClauseKind>(
                  getOpenMPSimpleClauseType(Kind, PP.getSpelling(Tok)))
            : OMPC_MAP_unknown;
    Data.DepLinMapLoc = Tok.getLocation();
    bool ColonExpected = false;
    if (IsMapClauseModifierToken(Tok)) {
      if (PP.LookAhead(0).is(tok::colon)) {
        if (Data.MapType == OMPC_MAP_unknown)
          Diag(Tok, diag::err_omp_unknown_map_type);
        else if (Data.MapType == OMPC_MAP_always)
          Diag(Tok, diag::err_omp_map_type_missing);
        ConsumeToken();
} else if (PP.LookAhead(0).is(tok::comma)) {
        if (IsMapClauseModifierToken(PP.LookAhead(1)) &&
            PP.LookAhead(2).is(tok::colon)) {
          Data.MapTypeModifier = Data.MapType;
          if (Data.MapTypeModifier != OMPC_MAP_always) {
            Diag(Tok, diag::err_omp_unknown_map_type_modifier);
            Data.MapTypeModifier = OMPC_MAP_unknown;
} else
            MapTypeModifierSpecified = true;
          ConsumeToken();
          ConsumeToken();
          Data.MapType =
              IsMapClauseModifierToken(Tok)
                  ? static_cast<OpenMPMapClauseKind>(
                        getOpenMPSimpleClauseType(Kind, PP.getSpelling(Tok)))
                  : OMPC_MAP_unknown;
          if (Data.MapType == OMPC_MAP_unknown ||
              Data.MapType == OMPC_MAP_always)
            Diag(Tok, diag::err_omp_unknown_map_type);
          ConsumeToken();
} else {
          Data.MapType = OMPC_MAP_tofrom;
          Data.IsMapTypeImplicit = true;
}
} else {
        Data.MapType = OMPC_MAP_tofrom;
        Data.IsMapTypeImplicit = true;
}
} else {
      Data.MapType = OMPC_MAP_tofrom;
      Data.IsMapTypeImplicit = true;
}
    if (Tok.is(tok::colon))
      Data.ColonLoc = ConsumeToken();
    else if (ColonExpected)
      Diag(Tok, diag::warn_pragma_expected_colon) << "map type";
}
  bool IsComma =
      (Kind != OMPC_reduction && Kind != OMPC_depend && Kind != OMPC_map) ||
      (Kind == OMPC_reduction && !InvalidReductionId) ||
      (Kind == OMPC_map && Data.MapType != OMPC_MAP_unknown &&
       (!MapTypeModifierSpecified ||
        Data.MapTypeModifier == OMPC_MAP_always)) ||
      (Kind == OMPC_depend && Data.DepKind != OMPC_DEPEND_unknown);
  const bool MayHaveTail = (Kind == OMPC_linear || Kind == OMPC_aligned);
  while (IsComma || (Tok.isNot(tok::r_paren) && Tok.isNot(tok::colon) &&
                     Tok.isNot(tok::annot_pragma_openmp_end))) {
    ColonProtectionRAIIObject ColonRAII(*this, MayHaveTail);
    ExprResult VarExpr =
        Actions.CorrectDelayedTyposInExpr(ParseAssignmentExpression());
    if (VarExpr.isUsable())
      Vars.push_back(VarExpr.get());
    else {
      SkipUntil(tok::comma, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
}
    IsComma = Tok.is(tok::comma);
    if (IsComma)
      ConsumeToken();
    else if (Tok.isNot(tok::r_paren) &&
             Tok.isNot(tok::annot_pragma_openmp_end) &&
             (!MayHaveTail || Tok.isNot(tok::colon)))
      if(Kind == OMPC_flush) {
        Diag(Tok, diag::err_omp_expected_punc)
          << getOpenMPDirectiveName(OMPD_flush)
          << (Kind == OMPC_flush);
}
      else if(Kind == OMPC_inc) {
          Diag(Tok, diag::err_omp_expected_punc)
          << getOpenMPDirectiveName(OMPD_inc)
          << (Kind == OMPC_inc);
}
      else {
        Diag(Tok, diag::err_omp_expected_punc)
          << getOpenMPClauseName(Kind)
          << false;
}
}
  if (NeedRParenForLinear)
    LinearT.consumeClose();
  const bool MustHaveTail = MayHaveTail && Tok.is(tok::colon);
  if (MustHaveTail) {
    Data.ColonLoc = Tok.getLocation();
    SourceLocation ELoc = ConsumeToken();
    ExprResult Tail = ParseAssignmentExpression();
    Tail = Actions.ActOnFinishFullExpr(Tail.get(), ELoc);
    if (Tail.isUsable())
      Data.TailExpr = Tail.get();
    else
      SkipUntil(tok::comma, tok::r_paren, tok::annot_pragma_openmp_end,
                StopBeforeMatch);
}
  T.consumeClose();
  if ((Kind == OMPC_depend && Data.DepKind != OMPC_DEPEND_unknown &&
       Vars.empty()) ||
      (Kind != OMPC_depend && Kind != OMPC_map && Vars.empty()) ||
      (MustHaveTail && !Data.TailExpr) || InvalidReductionId)
    return true;
  return false;
}
OMPClause *Parser::ParseOpenMPVarListClause(OpenMPDirectiveKind DKind,
                                            OpenMPClauseKind Kind) {
  SourceLocation Loc = Tok.getLocation();
  SourceLocation LOpen = ConsumeToken();
  SmallVector<Expr *, 4> Vars;
  OpenMPVarListDataTy Data;
  if (ParseOpenMPVarList(DKind, Kind, Vars, Data))
    return nullptr;
  return Actions.ActOnOpenMPVarListClause(
      Kind, Vars, Data.TailExpr, Loc, LOpen, Data.ColonLoc, Tok.getLocation(),
      Data.ReductionIdScopeSpec, Data.ReductionId, Data.DepKind, Data.LinKind,
      Data.MapTypeModifier, Data.MapType, Data.IsMapTypeImplicit,
      Data.DepLinMapLoc);
}