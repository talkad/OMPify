; ModuleID = 'target_teams_reduction_messages.cpp'
source_filename = "target_teams_reduction_messages.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.S2 = type { i32 }
%class.S3 = type { i32, i32 }
%class.S6 = type { i32 }
%class.S4 = type { i32 }
%class.S5 = type { i32 }

$_ZN2S2C2Ev = comdat any

$_ZN2S3C2Ev = comdat any

$_ZN2S6C2Ev = comdat any

$_ZN2S4C2Ei = comdat any

$_ZN2S5C2Ei = comdat any

$_Z5tmainIiET_S0_ = comdat any

$_Z5tmainIfET_S0_ = comdat any

@_ZN2S24S2scE = dso_local constant float 0.000000e+00, align 4, !dbg !0
@b = dso_local global %class.S2 zeroinitializer, align 4, !dbg !8
@_ZL2ba = internal global [5 x %class.S2] zeroinitializer, align 16, !dbg !30
@c = dso_local global %class.S3 zeroinitializer, align 4, !dbg !35
@_ZL2ca = internal global [5 x %class.S3] zeroinitializer, align 16, !dbg !53
@o = dso_local global %class.S6 zeroinitializer, align 4, !dbg !56
@h = dso_local global %class.S3 zeroinitializer, align 4, !dbg !68
@k = dso_local global %class.S3 zeroinitializer, align 4, !dbg !70
@_ZN1A1xE = dso_local global double 0.000000e+00, align 8, !dbg !72
@.str = private unnamed_addr constant [37 x i8] c"#pragma omp parallel for private(fl)\00", align 1
@.str.7 = private unnamed_addr constant [43 x i8] c"#pragma omp parallel for reduction(- : fl)\00", align 1
@_ZZ4mainE1m = internal global i32 0, align 4, !dbg !76
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_target_teams_reduction_messages.cpp, i8* null }]

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_Z3foov() #0 !dbg !91 {
  ret void, !dbg !94
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @_Z7foobooli(i32 %0) #0 !dbg !95 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !99, metadata !DIExpression()), !dbg !100
  %3 = load i32, i32* %2, align 4, !dbg !101
  %4 = icmp ne i32 %3, 0, !dbg !101
  ret i1 %4, !dbg !102
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_Z6foobarRi(i32* dereferenceable(4) %0) #0 !dbg !103 {
  %2 = alloca i32*, align 8
  store i32* %0, i32** %2, align 8
  call void @llvm.dbg.declare(metadata i32** %2, metadata !107, metadata !DIExpression()), !dbg !108
  call void @_Z3foov(), !dbg !109
  ret void, !dbg !110
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #2 section ".text.startup" !dbg !111 {
  call void @_ZN2S2C2Ev(%class.S2* @b), !dbg !112
  ret void, !dbg !112
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN2S2C2Ev(%class.S2* %0) unnamed_addr #0 comdat align 2 !dbg !113 {
  %2 = alloca %class.S2*, align 8
  store %class.S2* %0, %class.S2** %2, align 8
  call void @llvm.dbg.declare(metadata %class.S2** %2, metadata !114, metadata !DIExpression()), !dbg !116
  %3 = load %class.S2*, %class.S2** %2, align 8
  %4 = getelementptr inbounds %class.S2, %class.S2* %3, i32 0, i32 0, !dbg !117
  store i32 0, i32* %4, align 4, !dbg !117
  ret void, !dbg !118
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.1() #2 section ".text.startup" !dbg !119 {
  br label %1, !dbg !120

1:                                                ; preds = %1, %0
  %2 = phi %class.S2* [ getelementptr inbounds ([5 x %class.S2], [5 x %class.S2]* @_ZL2ba, i32 0, i32 0), %0 ], [ %3, %1 ], !dbg !120
  call void @_ZN2S2C2Ev(%class.S2* %2), !dbg !120
  %3 = getelementptr inbounds %class.S2, %class.S2* %2, i64 1, !dbg !120
  %4 = icmp eq %class.S2* %3, getelementptr inbounds (%class.S2, %class.S2* getelementptr inbounds ([5 x %class.S2], [5 x %class.S2]* @_ZL2ba, i32 0, i32 0), i64 5), !dbg !120
  br i1 %4, label %5, label %1, !dbg !120

5:                                                ; preds = %1
  ret void, !dbg !120
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @_ZplRK2S3S1_(%class.S3* dereferenceable(8) %0, %class.S3* dereferenceable(8) %1) #0 !dbg !121 {
  %3 = alloca %class.S3*, align 8
  %4 = alloca %class.S3*, align 8
  store %class.S3* %0, %class.S3** %3, align 8
  call void @llvm.dbg.declare(metadata %class.S3** %3, metadata !124, metadata !DIExpression()), !dbg !125
  store %class.S3* %1, %class.S3** %4, align 8
  call void @llvm.dbg.declare(metadata %class.S3** %4, metadata !126, metadata !DIExpression()), !dbg !127
  ret i32 5, !dbg !128
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.2() #2 section ".text.startup" !dbg !129 {
  call void @_ZN2S3C2Ev(%class.S3* @c), !dbg !130
  ret void, !dbg !130
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN2S3C2Ev(%class.S3* %0) unnamed_addr #0 comdat align 2 !dbg !131 {
  %2 = alloca %class.S3*, align 8
  store %class.S3* %0, %class.S3** %2, align 8
  call void @llvm.dbg.declare(metadata %class.S3** %2, metadata !132, metadata !DIExpression()), !dbg !134
  %3 = load %class.S3*, %class.S3** %2, align 8
  %4 = getelementptr inbounds %class.S3, %class.S3* %3, i32 0, i32 0, !dbg !135
  store i32 0, i32* %4, align 4, !dbg !135
  ret void, !dbg !136
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.3() #2 section ".text.startup" !dbg !137 {
  br label %1, !dbg !138

1:                                                ; preds = %1, %0
  %2 = phi %class.S3* [ getelementptr inbounds ([5 x %class.S3], [5 x %class.S3]* @_ZL2ca, i32 0, i32 0), %0 ], [ %3, %1 ], !dbg !138
  call void @_ZN2S3C2Ev(%class.S3* %2), !dbg !138
  %3 = getelementptr inbounds %class.S3, %class.S3* %2, i64 1, !dbg !138
  %4 = icmp eq %class.S3* %3, getelementptr inbounds (%class.S3, %class.S3* getelementptr inbounds ([5 x %class.S3], [5 x %class.S3]* @_ZL2ca, i32 0, i32 0), i64 5), !dbg !138
  br i1 %4, label %5, label %1, !dbg !138

5:                                                ; preds = %1
  ret void, !dbg !138
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local dereferenceable(4) %class.S4* @_ZaNR2S4S0_(%class.S4* dereferenceable(4) %0, %class.S4* dereferenceable(4) %1) #0 !dbg !139 {
  %3 = alloca %class.S4*, align 8
  %4 = alloca %class.S4*, align 8
  store %class.S4* %0, %class.S4** %3, align 8
  call void @llvm.dbg.declare(metadata %class.S4** %3, metadata !161, metadata !DIExpression()), !dbg !162
  store %class.S4* %1, %class.S4** %4, align 8
  call void @llvm.dbg.declare(metadata %class.S4** %4, metadata !163, metadata !DIExpression()), !dbg !164
  %5 = load %class.S4*, %class.S4** %3, align 8, !dbg !165
  ret %class.S4* %5, !dbg !166
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.4() #2 section ".text.startup" !dbg !167 {
  call void @_ZN2S6C2Ev(%class.S6* @o), !dbg !168
  ret void, !dbg !168
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN2S6C2Ev(%class.S6* %0) unnamed_addr #0 comdat align 2 !dbg !169 {
  %2 = alloca %class.S6*, align 8
  store %class.S6* %0, %class.S6** %2, align 8
  call void @llvm.dbg.declare(metadata %class.S6** %2, metadata !170, metadata !DIExpression()), !dbg !172
  %3 = load %class.S6*, %class.S6** %2, align 8
  %4 = getelementptr inbounds %class.S6, %class.S6* %3, i32 0, i32 0, !dbg !173
  store i32 6, i32* %4, align 4, !dbg !173
  ret void, !dbg !174
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.5() #2 section ".text.startup" !dbg !175 {
  call void @_ZN2S3C2Ev(%class.S3* @h), !dbg !176
  ret void, !dbg !176
}

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init.6() #2 section ".text.startup" !dbg !177 {
  call void @_ZN2S3C2Ev(%class.S3* @k), !dbg !178
  ret void, !dbg !178
}

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main(i32 %0, i8** %1) #3 !dbg !78 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca [5 x i32], align 16
  %8 = alloca [5 x i32], align 16
  %9 = alloca %class.S4, align 4
  %10 = alloca %class.S5, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32*, align 8
  %13 = alloca %class.S3*, align 8
  %14 = alloca i32*, align 8
  %15 = alloca i32*, align 8
  %16 = alloca float, align 4
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !179, metadata !DIExpression()), !dbg !180
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !181, metadata !DIExpression()), !dbg !182
  call void @llvm.dbg.declare(metadata i32* %6, metadata !183, metadata !DIExpression()), !dbg !185
  store i32 5, i32* %6, align 4, !dbg !185
  call void @llvm.dbg.declare(metadata [5 x i32]* %7, metadata !186, metadata !DIExpression()), !dbg !188
  %19 = bitcast [5 x i32]* %7 to i8*, !dbg !188
  call void @llvm.memset.p0i8.i64(i8* align 16 %19, i8 0, i64 20, i1 false), !dbg !188
  call void @llvm.dbg.declare(metadata [5 x i32]* %8, metadata !189, metadata !DIExpression()), !dbg !191
  %20 = bitcast [5 x i32]* %8 to i8*, !dbg !191
  call void @llvm.memset.p0i8.i64(i8* align 16 %20, i8 0, i64 20, i1 false), !dbg !191
  call void @llvm.dbg.declare(metadata %class.S4* %9, metadata !192, metadata !DIExpression()), !dbg !193
  call void @_ZN2S4C2Ei(%class.S4* %9, i32 4), !dbg !193
  call void @llvm.dbg.declare(metadata %class.S5* %10, metadata !194, metadata !DIExpression()), !dbg !214
  call void @_ZN2S5C2Ei(%class.S5* %10, i32 5), !dbg !214
  call void @llvm.dbg.declare(metadata i32* %11, metadata !215, metadata !DIExpression()), !dbg !216
  call void @llvm.dbg.declare(metadata i32** %12, metadata !217, metadata !DIExpression()), !dbg !218
  store i32* %11, i32** %12, align 8, !dbg !218
  call void @llvm.dbg.declare(metadata %class.S3** %13, metadata !219, metadata !DIExpression()), !dbg !221
  store %class.S3* @k, %class.S3** %13, align 8, !dbg !221
  call void @llvm.dbg.declare(metadata i32** %14, metadata !222, metadata !DIExpression()), !dbg !224
  %21 = load i32, i32* %11, align 4, !dbg !225
  %22 = sext i32 %21 to i64, !dbg !226
  %23 = getelementptr inbounds [5 x i32], [5 x i32]* %7, i64 0, i64 %22, !dbg !226
  store i32* %23, i32** %14, align 8, !dbg !224
  call void @llvm.dbg.declare(metadata i32** %15, metadata !227, metadata !DIExpression()), !dbg !228
  %24 = load i32, i32* %11, align 4, !dbg !229
  %25 = sext i32 %24 to i64, !dbg !230
  %26 = getelementptr inbounds [5 x i32], [5 x i32]* %8, i64 0, i64 %25, !dbg !230
  store i32* %26, i32** %15, align 8, !dbg !228
  call void @llvm.dbg.declare(metadata float* %16, metadata !231, metadata !DIExpression()), !dbg !232
  call void @_Z3foov(), !dbg !233
  call void @_Z3foov(), !dbg !234
  call void @_Z3foov(), !dbg !235
  call void @_Z3foov(), !dbg !236
  call void @_Z3foov(), !dbg !237
  call void @_Z3foov(), !dbg !238
  call void @_Z3foov(), !dbg !239
  call void @_Z3foov(), !dbg !240
  call void @_Z3foov(), !dbg !241
  call void @_Z3foov(), !dbg !242
  call void @_Z3foov(), !dbg !243
  call void @_Z3foov(), !dbg !244
  call void @_Z3foov(), !dbg !245
  call void @_Z3foov(), !dbg !246
  call void @_Z3foov(), !dbg !247
  call void @_Z3foov(), !dbg !248
  call void @_Z3foov(), !dbg !249
  call void @_Z3foov(), !dbg !250
  call void @_Z3foov(), !dbg !251
  call void @_Z3foov(), !dbg !252
  call void @_Z3foov(), !dbg !253
  call void @_Z3foov(), !dbg !254
  call void @_Z3foov(), !dbg !255
  call void @_Z3foov(), !dbg !256
  call void @_Z3foov(), !dbg !257
  call void @_Z3foov(), !dbg !258
  call void @_Z3foov(), !dbg !259
  call void @_Z3foov(), !dbg !260
  call void @_Z3foov(), !dbg !261
  call void @_Z3foov(), !dbg !262
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str, i64 0, i64 0)), !dbg !263
  call void @_Z19for_loop_talkad7420v(), !dbg !264
  call void @llvm.dbg.declare(metadata i32* %17, metadata !265, metadata !DIExpression()), !dbg !267
  store i32 0, i32* %17, align 4, !dbg !267
  br label %27, !dbg !268

27:                                               ; preds = %31, %2
  %28 = load i32, i32* %17, align 4, !dbg !269
  %29 = icmp slt i32 %28, 10, !dbg !271
  br i1 %29, label %30, label %34, !dbg !272

30:                                               ; preds = %27
  br label %31, !dbg !273

31:                                               ; preds = %30
  %32 = load i32, i32* %17, align 4, !dbg !275
  %33 = add nsw i32 %32, 1, !dbg !275
  store i32 %33, i32* %17, align 4, !dbg !275
  br label %27, !dbg !276, !llvm.loop !277

34:                                               ; preds = %27
  call void @_Z3foov(), !dbg !279
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @.str.7, i64 0, i64 0)), !dbg !280
  call void @_Z19for_loop_talkad7420v(), !dbg !281
  call void @llvm.dbg.declare(metadata i32* %18, metadata !282, metadata !DIExpression()), !dbg !284
  store i32 0, i32* %18, align 4, !dbg !284
  br label %35, !dbg !285

35:                                               ; preds = %39, %34
  %36 = load i32, i32* %18, align 4, !dbg !286
  %37 = icmp slt i32 %36, 10, !dbg !288
  br i1 %37, label %38, label %42, !dbg !289

38:                                               ; preds = %35
  br label %39, !dbg !290

39:                                               ; preds = %38
  %40 = load i32, i32* %18, align 4, !dbg !292
  %41 = add nsw i32 %40, 1, !dbg !292
  store i32 %41, i32* %18, align 4, !dbg !292
  br label %35, !dbg !293, !llvm.loop !294

42:                                               ; preds = %35
  call void @_Z3foov(), !dbg !296
  call void @_Z3foov(), !dbg !297
  %43 = load i32, i32* %4, align 4, !dbg !298
  %44 = call i32 @_Z5tmainIiET_S0_(i32 %43), !dbg !299
  %45 = sitofp i32 %44 to float, !dbg !299
  %46 = load float, float* %16, align 4, !dbg !300
  %47 = call float @_Z5tmainIfET_S0_(float %46), !dbg !301
  %48 = fadd float %45, %47, !dbg !302
  %49 = fptosi float %48 to i32, !dbg !299
  ret i32 %49, !dbg !303
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #4

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN2S4C2Ei(%class.S4* %0, i32 %1) unnamed_addr #0 comdat align 2 !dbg !304 {
  %3 = alloca %class.S4*, align 8
  %4 = alloca i32, align 4
  store %class.S4* %0, %class.S4** %3, align 8
  call void @llvm.dbg.declare(metadata %class.S4** %3, metadata !305, metadata !DIExpression()), !dbg !307
  store i32 %1, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !308, metadata !DIExpression()), !dbg !309
  %5 = load %class.S4*, %class.S4** %3, align 8
  %6 = getelementptr inbounds %class.S4, %class.S4* %5, i32 0, i32 0, !dbg !310
  %7 = load i32, i32* %4, align 4, !dbg !311
  store i32 %7, i32* %6, align 4, !dbg !310
  ret void, !dbg !312
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN2S5C2Ei(%class.S5* %0, i32 %1) unnamed_addr #0 comdat align 2 !dbg !313 {
  %3 = alloca %class.S5*, align 8
  %4 = alloca i32, align 4
  store %class.S5* %0, %class.S5** %3, align 8
  call void @llvm.dbg.declare(metadata %class.S5** %3, metadata !314, metadata !DIExpression()), !dbg !316
  store i32 %1, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !317, metadata !DIExpression()), !dbg !318
  %5 = load %class.S5*, %class.S5** %3, align 8
  %6 = getelementptr inbounds %class.S5, %class.S5* %5, i32 0, i32 0, !dbg !319
  %7 = load i32, i32* %4, align 4, !dbg !320
  store i32 %7, i32* %6, align 4, !dbg !319
  ret void, !dbg !321
}

declare dso_local void @_Z25omp_for_pragma_talkad7420Pc(i8*) #5

declare dso_local void @_Z19for_loop_talkad7420v() #5

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local i32 @_Z5tmainIiET_S0_(i32 %0) #6 comdat !dbg !322 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca [5 x i32], align 16
  %5 = alloca [5 x i32], align 16
  %6 = alloca i32, align 4
  %7 = alloca i32*, align 8
  %8 = alloca %class.S3*, align 8
  %9 = alloca i32*, align 8
  %10 = alloca i32*, align 8
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !327, metadata !DIExpression()), !dbg !328
  call void @llvm.dbg.declare(metadata i32* %3, metadata !329, metadata !DIExpression()), !dbg !330
  store i32 0, i32* %3, align 4, !dbg !330
  call void @llvm.dbg.declare(metadata [5 x i32]* %4, metadata !331, metadata !DIExpression()), !dbg !332
  %14 = bitcast [5 x i32]* %4 to i8*, !dbg !332
  call void @llvm.memset.p0i8.i64(i8* align 16 %14, i8 0, i64 20, i1 false), !dbg !332
  call void @llvm.dbg.declare(metadata [5 x i32]* %5, metadata !333, metadata !DIExpression()), !dbg !334
  %15 = bitcast [5 x i32]* %5 to i8*, !dbg !334
  call void @llvm.memset.p0i8.i64(i8* align 16 %15, i8 0, i64 20, i1 false), !dbg !334
  call void @llvm.dbg.declare(metadata i32* %6, metadata !335, metadata !DIExpression()), !dbg !336
  call void @llvm.dbg.declare(metadata i32** %7, metadata !337, metadata !DIExpression()), !dbg !338
  store i32* %6, i32** %7, align 8, !dbg !338
  call void @llvm.dbg.declare(metadata %class.S3** %8, metadata !339, metadata !DIExpression()), !dbg !340
  store %class.S3* @k, %class.S3** %8, align 8, !dbg !340
  call void @llvm.dbg.declare(metadata i32** %9, metadata !341, metadata !DIExpression()), !dbg !342
  %16 = load i32, i32* %6, align 4, !dbg !343
  %17 = sext i32 %16 to i64, !dbg !344
  %18 = getelementptr inbounds [5 x i32], [5 x i32]* %4, i64 0, i64 %17, !dbg !344
  store i32* %18, i32** %9, align 8, !dbg !342
  call void @llvm.dbg.declare(metadata i32** %10, metadata !345, metadata !DIExpression()), !dbg !346
  %19 = load i32, i32* %6, align 4, !dbg !347
  %20 = sext i32 %19 to i64, !dbg !348
  %21 = getelementptr inbounds [5 x i32], [5 x i32]* %5, i64 0, i64 %20, !dbg !348
  store i32* %21, i32** %10, align 8, !dbg !346
  call void @llvm.dbg.declare(metadata i32* %11, metadata !349, metadata !DIExpression()), !dbg !350
  call void @_Z3foov(), !dbg !351
  call void @_Z3foov(), !dbg !352
  call void @_Z3foov(), !dbg !353
  call void @_Z3foov(), !dbg !354
  call void @_Z3foov(), !dbg !355
  call void @_Z3foov(), !dbg !356
  call void @_Z3foov(), !dbg !357
  call void @_Z3foov(), !dbg !358
  call void @_Z3foov(), !dbg !359
  call void @_Z3foov(), !dbg !360
  call void @_Z3foov(), !dbg !361
  call void @_Z3foov(), !dbg !362
  call void @_Z3foov(), !dbg !363
  call void @_Z3foov(), !dbg !364
  call void @_Z3foov(), !dbg !365
  call void @_Z3foov(), !dbg !366
  call void @_Z3foov(), !dbg !367
  call void @_Z3foov(), !dbg !368
  call void @_Z3foov(), !dbg !369
  call void @_Z3foov(), !dbg !370
  call void @_Z3foov(), !dbg !371
  call void @_Z3foov(), !dbg !372
  call void @_Z3foov(), !dbg !373
  call void @_Z3foov(), !dbg !374
  call void @_Z3foov(), !dbg !375
  call void @_Z3foov(), !dbg !376
  call void @_Z3foov(), !dbg !377
  call void @_Z3foov(), !dbg !378
  call void @_Z3foov(), !dbg !379
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str, i64 0, i64 0)), !dbg !380
  call void @_Z19for_loop_talkad7420v(), !dbg !381
  call void @llvm.dbg.declare(metadata i32* %12, metadata !382, metadata !DIExpression()), !dbg !384
  store i32 0, i32* %12, align 4, !dbg !384
  br label %22, !dbg !385

22:                                               ; preds = %26, %1
  %23 = load i32, i32* %12, align 4, !dbg !386
  %24 = icmp slt i32 %23, 10, !dbg !388
  br i1 %24, label %25, label %29, !dbg !389

25:                                               ; preds = %22
  br label %26, !dbg !390

26:                                               ; preds = %25
  %27 = load i32, i32* %12, align 4, !dbg !392
  %28 = add nsw i32 %27, 1, !dbg !392
  store i32 %28, i32* %12, align 4, !dbg !392
  br label %22, !dbg !393, !llvm.loop !394

29:                                               ; preds = %22
  call void @_Z3foov(), !dbg !396
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @.str.7, i64 0, i64 0)), !dbg !397
  call void @_Z19for_loop_talkad7420v(), !dbg !398
  call void @llvm.dbg.declare(metadata i32* %13, metadata !399, metadata !DIExpression()), !dbg !401
  store i32 0, i32* %13, align 4, !dbg !401
  br label %30, !dbg !402

30:                                               ; preds = %34, %29
  %31 = load i32, i32* %13, align 4, !dbg !403
  %32 = icmp slt i32 %31, 10, !dbg !405
  br i1 %32, label %33, label %37, !dbg !406

33:                                               ; preds = %30
  br label %34, !dbg !407

34:                                               ; preds = %33
  %35 = load i32, i32* %13, align 4, !dbg !409
  %36 = add nsw i32 %35, 1, !dbg !409
  store i32 %36, i32* %13, align 4, !dbg !409
  br label %30, !dbg !410, !llvm.loop !411

37:                                               ; preds = %30
  call void @_Z3foov(), !dbg !413
  ret i32 0, !dbg !414
}

; Function Attrs: noinline optnone uwtable
define linkonce_odr dso_local float @_Z5tmainIfET_S0_(float %0) #6 comdat !dbg !415 {
  %2 = alloca float, align 4
  %3 = alloca float, align 4
  %4 = alloca [5 x float], align 16
  %5 = alloca [5 x float], align 16
  %6 = alloca float, align 4
  %7 = alloca float*, align 8
  %8 = alloca %class.S3*, align 8
  %9 = alloca float*, align 8
  %10 = alloca float*, align 8
  %11 = alloca float, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  store float %0, float* %2, align 4
  call void @llvm.dbg.declare(metadata float* %2, metadata !420, metadata !DIExpression()), !dbg !421
  call void @llvm.dbg.declare(metadata float* %3, metadata !422, metadata !DIExpression()), !dbg !423
  store float 0.000000e+00, float* %3, align 4, !dbg !423
  call void @llvm.dbg.declare(metadata [5 x float]* %4, metadata !424, metadata !DIExpression()), !dbg !426
  %14 = bitcast [5 x float]* %4 to i8*, !dbg !426
  call void @llvm.memset.p0i8.i64(i8* align 16 %14, i8 0, i64 20, i1 false), !dbg !426
  call void @llvm.dbg.declare(metadata [5 x float]* %5, metadata !427, metadata !DIExpression()), !dbg !429
  %15 = bitcast [5 x float]* %5 to i8*, !dbg !429
  call void @llvm.memset.p0i8.i64(i8* align 16 %15, i8 0, i64 20, i1 false), !dbg !429
  call void @llvm.dbg.declare(metadata float* %6, metadata !430, metadata !DIExpression()), !dbg !431
  call void @llvm.dbg.declare(metadata float** %7, metadata !432, metadata !DIExpression()), !dbg !434
  store float* %6, float** %7, align 8, !dbg !434
  call void @llvm.dbg.declare(metadata %class.S3** %8, metadata !435, metadata !DIExpression()), !dbg !436
  store %class.S3* @k, %class.S3** %8, align 8, !dbg !436
  call void @llvm.dbg.declare(metadata float** %9, metadata !437, metadata !DIExpression()), !dbg !439
  %16 = load float, float* %6, align 4, !dbg !440
  %17 = fptosi float %16 to i32, !dbg !440
  %18 = sext i32 %17 to i64, !dbg !441
  %19 = getelementptr inbounds [5 x float], [5 x float]* %4, i64 0, i64 %18, !dbg !441
  store float* %19, float** %9, align 8, !dbg !439
  call void @llvm.dbg.declare(metadata float** %10, metadata !442, metadata !DIExpression()), !dbg !443
  %20 = load float, float* %6, align 4, !dbg !444
  %21 = fptosi float %20 to i32, !dbg !444
  %22 = sext i32 %21 to i64, !dbg !445
  %23 = getelementptr inbounds [5 x float], [5 x float]* %5, i64 0, i64 %22, !dbg !445
  store float* %23, float** %10, align 8, !dbg !443
  call void @llvm.dbg.declare(metadata float* %11, metadata !446, metadata !DIExpression()), !dbg !447
  call void @_Z3foov(), !dbg !448
  call void @_Z3foov(), !dbg !449
  call void @_Z3foov(), !dbg !450
  call void @_Z3foov(), !dbg !451
  call void @_Z3foov(), !dbg !452
  call void @_Z3foov(), !dbg !453
  call void @_Z3foov(), !dbg !454
  call void @_Z3foov(), !dbg !455
  call void @_Z3foov(), !dbg !456
  call void @_Z3foov(), !dbg !457
  call void @_Z3foov(), !dbg !458
  call void @_Z3foov(), !dbg !459
  call void @_Z3foov(), !dbg !460
  call void @_Z3foov(), !dbg !461
  call void @_Z3foov(), !dbg !462
  call void @_Z3foov(), !dbg !463
  call void @_Z3foov(), !dbg !464
  call void @_Z3foov(), !dbg !465
  call void @_Z3foov(), !dbg !466
  call void @_Z3foov(), !dbg !467
  call void @_Z3foov(), !dbg !468
  call void @_Z3foov(), !dbg !469
  call void @_Z3foov(), !dbg !470
  call void @_Z3foov(), !dbg !471
  call void @_Z3foov(), !dbg !472
  call void @_Z3foov(), !dbg !473
  call void @_Z3foov(), !dbg !474
  call void @_Z3foov(), !dbg !475
  call void @_Z3foov(), !dbg !476
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str, i64 0, i64 0)), !dbg !477
  call void @_Z19for_loop_talkad7420v(), !dbg !478
  call void @llvm.dbg.declare(metadata i32* %12, metadata !479, metadata !DIExpression()), !dbg !481
  store i32 0, i32* %12, align 4, !dbg !481
  br label %24, !dbg !482

24:                                               ; preds = %28, %1
  %25 = load i32, i32* %12, align 4, !dbg !483
  %26 = icmp slt i32 %25, 10, !dbg !485
  br i1 %26, label %27, label %31, !dbg !486

27:                                               ; preds = %24
  br label %28, !dbg !487

28:                                               ; preds = %27
  %29 = load i32, i32* %12, align 4, !dbg !489
  %30 = add nsw i32 %29, 1, !dbg !489
  store i32 %30, i32* %12, align 4, !dbg !489
  br label %24, !dbg !490, !llvm.loop !491

31:                                               ; preds = %24
  call void @_Z3foov(), !dbg !493
  call void @_Z25omp_for_pragma_talkad7420Pc(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @.str.7, i64 0, i64 0)), !dbg !494
  call void @_Z19for_loop_talkad7420v(), !dbg !495
  call void @llvm.dbg.declare(metadata i32* %13, metadata !496, metadata !DIExpression()), !dbg !498
  store i32 0, i32* %13, align 4, !dbg !498
  br label %32, !dbg !499

32:                                               ; preds = %36, %31
  %33 = load i32, i32* %13, align 4, !dbg !500
  %34 = icmp slt i32 %33, 10, !dbg !502
  br i1 %34, label %35, label %39, !dbg !503

35:                                               ; preds = %32
  br label %36, !dbg !504

36:                                               ; preds = %35
  %37 = load i32, i32* %13, align 4, !dbg !506
  %38 = add nsw i32 %37, 1, !dbg !506
  store i32 %38, i32* %13, align 4, !dbg !506
  br label %32, !dbg !507, !llvm.loop !508

39:                                               ; preds = %32
  call void @_Z3foov(), !dbg !510
  ret float 0.000000e+00, !dbg !511
}

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_target_teams_reduction_messages.cpp() #2 section ".text.startup" !dbg !512 {
  call void @__cxx_global_var_init(), !dbg !514
  call void @__cxx_global_var_init.1(), !dbg !514
  call void @__cxx_global_var_init.2(), !dbg !514
  call void @__cxx_global_var_init.3(), !dbg !514
  call void @__cxx_global_var_init.4(), !dbg !514
  call void @__cxx_global_var_init.5(), !dbg !514
  call void @__cxx_global_var_init.6(), !dbg !514
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { noinline uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind willreturn }
attributes #5 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!87, !88, !89}
!llvm.ident = !{!90}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "S2sc", linkageName: "_ZN2S24S2scE", scope: !2, file: !3, line: 27, type: !16, isLocal: false, isDefinition: true, declaration: !15)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !7, imports: !84, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "target_teams_reduction_messages.cpp", directory: "/home/talkad/OpenMPdb/temp/DebasmitaBasu/custom-OpenMP-directives/clang/test/OpenMP")
!4 = !{}
!5 = !{!6}
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !{!0, !8, !30, !35, !53, !56, !68, !70, !72, !76}
!8 = !DIGlobalVariableExpression(var: !9, expr: !DIExpression())
!9 = distinct !DIGlobalVariable(name: "b", scope: !2, file: !3, line: 28, type: !10, isLocal: false, isDefinition: true)
!10 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "S2", file: !3, line: 15, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !11, identifier: "_ZTS2S2")
!11 = !{!12, !13, !15, !17, !24, !27}
!12 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !10, file: !3, line: 16, baseType: !6, size: 32)
!13 = !DIDerivedType(tag: DW_TAG_member, name: "S2s", scope: !10, file: !3, line: 24, baseType: !14, flags: DIFlagPublic | DIFlagStaticMember)
!14 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!15 = !DIDerivedType(tag: DW_TAG_member, name: "S2sc", scope: !10, file: !3, line: 25, baseType: !16, flags: DIFlagPublic | DIFlagStaticMember)
!16 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!17 = !DISubprogram(name: "operator+", linkageName: "_ZN2S2plERKS_", scope: !10, file: !3, line: 17, type: !18, scopeLine: 17, flags: DIFlagPrototyped, spFlags: 0)
!18 = !DISubroutineType(types: !19)
!19 = !{!20, !21, !22}
!20 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !10, size: 64)
!21 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!22 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !23, size: 64)
!23 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !10)
!24 = !DISubprogram(name: "S2", scope: !10, file: !3, line: 20, type: !25, scopeLine: 20, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!25 = !DISubroutineType(types: !26)
!26 = !{null, !21}
!27 = !DISubprogram(name: "S2", scope: !10, file: !3, line: 22, type: !28, scopeLine: 22, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!28 = !DISubroutineType(types: !29)
!29 = !{null, !21, !20}
!30 = !DIGlobalVariableExpression(var: !31, expr: !DIExpression())
!31 = distinct !DIGlobalVariable(name: "ba", linkageName: "_ZL2ba", scope: !2, file: !3, line: 29, type: !32, isLocal: true, isDefinition: true)
!32 = !DICompositeType(tag: DW_TAG_array_type, baseType: !23, size: 160, elements: !33)
!33 = !{!34}
!34 = !DISubrange(count: 5)
!35 = !DIGlobalVariableExpression(var: !36, expr: !DIExpression())
!36 = distinct !DIGlobalVariable(name: "c", scope: !2, file: !3, line: 43, type: !37, isLocal: false, isDefinition: true)
!37 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "S3", file: !3, line: 30, size: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !38, identifier: "_ZTS2S3")
!38 = !{!39, !40, !41, !45, !50}
!39 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !37, file: !3, line: 31, baseType: !6, size: 32)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "b", scope: !37, file: !3, line: 33, baseType: !6, size: 32, offset: 32, flags: DIFlagPublic)
!41 = !DISubprogram(name: "S3", scope: !37, file: !3, line: 34, type: !42, scopeLine: 34, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!42 = !DISubroutineType(types: !43)
!43 = !{null, !44}
!44 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !37, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!45 = !DISubprogram(name: "S3", scope: !37, file: !3, line: 36, type: !46, scopeLine: 36, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!46 = !DISubroutineType(types: !47)
!47 = !{null, !44, !48}
!48 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !49, size: 64)
!49 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !37)
!50 = !DISubprogram(name: "operator+", linkageName: "_ZN2S3plERKS_", scope: !37, file: !3, line: 38, type: !51, scopeLine: 38, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!51 = !DISubroutineType(types: !52)
!52 = !{!37, !44, !48}
!53 = !DIGlobalVariableExpression(var: !54, expr: !DIExpression())
!54 = distinct !DIGlobalVariable(name: "ca", linkageName: "_ZL2ca", scope: !2, file: !3, line: 44, type: !55, isLocal: true, isDefinition: true)
!55 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, size: 320, elements: !33)
!56 = !DIGlobalVariableExpression(var: !57, expr: !DIExpression())
!57 = distinct !DIGlobalVariable(name: "o", scope: !2, file: !3, line: 78, type: !58, isLocal: false, isDefinition: true)
!58 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "S6", file: !3, line: 69, size: 32, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !59, identifier: "_ZTS2S6")
!59 = !{!60, !61, !65}
!60 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !58, file: !3, line: 72, baseType: !6, size: 32)
!61 = !DISubprogram(name: "S6", scope: !58, file: !3, line: 74, type: !62, scopeLine: 74, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!62 = !DISubroutineType(types: !63)
!63 = !{null, !64}
!64 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!65 = !DISubprogram(name: "operator int", linkageName: "_ZN2S6cviEv", scope: !58, file: !3, line: 76, type: !66, scopeLine: 76, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!66 = !DISubroutineType(types: !67)
!67 = !{!6, !64}
!68 = !DIGlobalVariableExpression(var: !69, expr: !DIExpression())
!69 = distinct !DIGlobalVariable(name: "h", scope: !2, file: !3, line: 79, type: !37, isLocal: false, isDefinition: true)
!70 = !DIGlobalVariableExpression(var: !71, expr: !DIExpression())
!71 = distinct !DIGlobalVariable(name: "k", scope: !2, file: !3, line: 79, type: !37, isLocal: false, isDefinition: true)
!72 = !DIGlobalVariableExpression(var: !73, expr: !DIExpression())
!73 = distinct !DIGlobalVariable(name: "x", linkageName: "_ZN1A1xE", scope: !74, file: !3, line: 174, type: !75, isLocal: false, isDefinition: true)
!74 = !DINamespace(name: "A", scope: null)
!75 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!76 = !DIGlobalVariableExpression(var: !77, expr: !DIExpression())
!77 = distinct !DIGlobalVariable(name: "m", scope: !78, file: !3, line: 273, type: !6, isLocal: true, isDefinition: true)
!78 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 180, type: !79, scopeLine: 180, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!79 = !DISubroutineType(types: !80)
!80 = !{!6, !6, !81}
!81 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !82, size: 64)
!82 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !83, size: 64)
!83 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!84 = !{!85}
!85 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !86, entity: !73, file: !3, line: 178)
!86 = !DINamespace(name: "B", scope: null)
!87 = !{i32 7, !"Dwarf Version", i32 4}
!88 = !{i32 2, !"Debug Info Version", i32 3}
!89 = !{i32 1, !"wchar_size", i32 4}
!90 = !{!"clang version 10.0.0-4ubuntu1 "}
!91 = distinct !DISubprogram(name: "foo", linkageName: "_Z3foov", scope: !3, file: !3, line: 4, type: !92, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!92 = !DISubroutineType(types: !93)
!93 = !{null}
!94 = !DILocation(line: 5, column: 1, scope: !91)
!95 = distinct !DISubprogram(name: "foobool", linkageName: "_Z7foobooli", scope: !3, file: !3, line: 6, type: !96, scopeLine: 6, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!96 = !DISubroutineType(types: !97)
!97 = !{!98, !6}
!98 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!99 = !DILocalVariable(name: "argc", arg: 1, scope: !95, file: !3, line: 6, type: !6)
!100 = !DILocation(line: 6, column: 18, scope: !95)
!101 = !DILocation(line: 7, column: 10, scope: !95)
!102 = !DILocation(line: 7, column: 3, scope: !95)
!103 = distinct !DISubprogram(name: "foobar", linkageName: "_Z6foobarRi", scope: !3, file: !3, line: 9, type: !104, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!104 = !DISubroutineType(types: !105)
!105 = !{null, !106}
!106 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !6, size: 64)
!107 = !DILocalVariable(name: "ref", arg: 1, scope: !103, file: !3, line: 9, type: !106)
!108 = !DILocation(line: 9, column: 18, scope: !103)
!109 = !DILocation(line: 11, column: 3, scope: !103)
!110 = !DILocation(line: 12, column: 1, scope: !103)
!111 = distinct !DISubprogram(name: "__cxx_global_var_init", scope: !3, file: !3, line: 28, type: !92, scopeLine: 28, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!112 = !DILocation(line: 28, column: 4, scope: !111)
!113 = distinct !DISubprogram(name: "S2", linkageName: "_ZN2S2C2Ev", scope: !10, file: !3, line: 20, type: !25, scopeLine: 20, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !24, retainedNodes: !4)
!114 = !DILocalVariable(name: "this", arg: 1, scope: !113, type: !115, flags: DIFlagArtificial | DIFlagObjectPointer)
!115 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!116 = !DILocation(line: 0, scope: !113)
!117 = !DILocation(line: 20, column: 10, scope: !113)
!118 = !DILocation(line: 21, column: 1, scope: !113)
!119 = distinct !DISubprogram(name: "__cxx_global_var_init.1", scope: !3, file: !3, line: 29, type: !92, scopeLine: 29, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!120 = !DILocation(line: 29, column: 10, scope: !119)
!121 = distinct !DISubprogram(name: "operator+", linkageName: "_ZplRK2S3S1_", scope: !3, file: !3, line: 41, type: !122, scopeLine: 41, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!122 = !DISubroutineType(types: !123)
!123 = !{!6, !48, !48}
!124 = !DILocalVariable(name: "arg1", arg: 1, scope: !121, file: !3, line: 41, type: !48)
!125 = !DILocation(line: 41, column: 25, scope: !121)
!126 = !DILocalVariable(name: "arg2", arg: 2, scope: !121, file: !3, line: 41, type: !48)
!127 = !DILocation(line: 41, column: 41, scope: !121)
!128 = !DILocation(line: 41, column: 49, scope: !121)
!129 = distinct !DISubprogram(name: "__cxx_global_var_init.2", scope: !3, file: !3, line: 43, type: !92, scopeLine: 43, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!130 = !DILocation(line: 43, column: 4, scope: !129)
!131 = distinct !DISubprogram(name: "S3", linkageName: "_ZN2S3C2Ev", scope: !37, file: !3, line: 34, type: !42, scopeLine: 34, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !41, retainedNodes: !4)
!132 = !DILocalVariable(name: "this", arg: 1, scope: !131, type: !133, flags: DIFlagArtificial | DIFlagObjectPointer)
!133 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !37, size: 64)
!134 = !DILocation(line: 0, scope: !131)
!135 = !DILocation(line: 34, column: 10, scope: !131)
!136 = !DILocation(line: 35, column: 1, scope: !131)
!137 = distinct !DISubprogram(name: "__cxx_global_var_init.3", scope: !3, file: !3, line: 44, type: !92, scopeLine: 44, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!138 = !DILocation(line: 44, column: 10, scope: !137)
!139 = distinct !DISubprogram(name: "operator&=", linkageName: "_ZaNR2S4S0_", scope: !3, file: !3, line: 56, type: !140, scopeLine: 56, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!140 = !DISubroutineType(types: !141)
!141 = !{!142, !142, !142}
!142 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !143, size: 64)
!143 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "S4", file: !3, line: 46, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !144, identifier: "_ZTS2S4")
!144 = !{!145, !146, !150, !155, !158}
!145 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !143, file: !3, line: 47, baseType: !6, size: 32)
!146 = !DISubprogram(name: "S4", scope: !143, file: !3, line: 48, type: !147, scopeLine: 48, flags: DIFlagPrototyped, spFlags: 0)
!147 = !DISubroutineType(types: !148)
!148 = !{null, !149}
!149 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !143, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!150 = !DISubprogram(name: "S4", scope: !143, file: !3, line: 49, type: !151, scopeLine: 49, flags: DIFlagPrototyped, spFlags: 0)
!151 = !DISubroutineType(types: !152)
!152 = !{null, !149, !153}
!153 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !154, size: 64)
!154 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !143)
!155 = !DISubprogram(name: "operator+", linkageName: "_ZN2S4plERKS_", scope: !143, file: !3, line: 50, type: !156, scopeLine: 50, flags: DIFlagPrototyped, spFlags: 0)
!156 = !DISubroutineType(types: !157)
!157 = !{!142, !149, !153}
!158 = !DISubprogram(name: "S4", scope: !143, file: !3, line: 53, type: !159, scopeLine: 53, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!159 = !DISubroutineType(types: !160)
!160 = !{null, !149, !6}
!161 = !DILocalVariable(name: "arg1", arg: 1, scope: !139, file: !3, line: 56, type: !142)
!162 = !DILocation(line: 56, column: 20, scope: !139)
!163 = !DILocalVariable(name: "arg2", arg: 2, scope: !139, file: !3, line: 56, type: !142)
!164 = !DILocation(line: 56, column: 30, scope: !139)
!165 = !DILocation(line: 56, column: 45, scope: !139)
!166 = !DILocation(line: 56, column: 38, scope: !139)
!167 = distinct !DISubprogram(name: "__cxx_global_var_init.4", scope: !3, file: !3, line: 78, type: !92, scopeLine: 78, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!168 = !DILocation(line: 78, column: 3, scope: !167)
!169 = distinct !DISubprogram(name: "S6", linkageName: "_ZN2S6C2Ev", scope: !58, file: !3, line: 74, type: !62, scopeLine: 74, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !61, retainedNodes: !4)
!170 = !DILocalVariable(name: "this", arg: 1, scope: !169, type: !171, flags: DIFlagArtificial | DIFlagObjectPointer)
!171 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 64)
!172 = !DILocation(line: 0, scope: !169)
!173 = !DILocation(line: 74, column: 10, scope: !169)
!174 = !DILocation(line: 75, column: 1, scope: !169)
!175 = distinct !DISubprogram(name: "__cxx_global_var_init.5", scope: !3, file: !3, line: 79, type: !92, scopeLine: 79, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!176 = !DILocation(line: 79, column: 4, scope: !175)
!177 = distinct !DISubprogram(name: "__cxx_global_var_init.6", scope: !3, file: !3, line: 79, type: !92, scopeLine: 79, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!178 = !DILocation(line: 79, column: 7, scope: !177)
!179 = !DILocalVariable(name: "argc", arg: 1, scope: !78, file: !3, line: 180, type: !6)
!180 = !DILocation(line: 180, column: 14, scope: !78)
!181 = !DILocalVariable(name: "argv", arg: 2, scope: !78, file: !3, line: 180, type: !81)
!182 = !DILocation(line: 180, column: 27, scope: !78)
!183 = !DILocalVariable(name: "d", scope: !78, file: !3, line: 181, type: !184)
!184 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !6)
!185 = !DILocation(line: 181, column: 13, scope: !78)
!186 = !DILocalVariable(name: "da", scope: !78, file: !3, line: 182, type: !187)
!187 = !DICompositeType(tag: DW_TAG_array_type, baseType: !184, size: 160, elements: !33)
!188 = !DILocation(line: 182, column: 13, scope: !78)
!189 = !DILocalVariable(name: "qa", scope: !78, file: !3, line: 184, type: !190)
!190 = !DICompositeType(tag: DW_TAG_array_type, baseType: !6, size: 160, elements: !33)
!191 = !DILocation(line: 184, column: 7, scope: !78)
!192 = !DILocalVariable(name: "e", scope: !78, file: !3, line: 186, type: !143)
!193 = !DILocation(line: 186, column: 6, scope: !78)
!194 = !DILocalVariable(name: "g", scope: !78, file: !3, line: 187, type: !195)
!195 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "S5", file: !3, line: 58, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !196, identifier: "_ZTS2S5")
!196 = !{!197, !198, !202, !207, !211}
!197 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !195, file: !3, line: 59, baseType: !6, size: 32)
!198 = !DISubprogram(name: "S5", scope: !195, file: !3, line: 60, type: !199, scopeLine: 60, flags: DIFlagPrototyped, spFlags: 0)
!199 = !DISubroutineType(types: !200)
!200 = !{null, !201}
!201 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !195, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!202 = !DISubprogram(name: "S5", scope: !195, file: !3, line: 62, type: !203, scopeLine: 62, flags: DIFlagPrototyped, spFlags: 0)
!203 = !DISubroutineType(types: !204)
!204 = !{null, !201, !205}
!205 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !206, size: 64)
!206 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !195)
!207 = !DISubprogram(name: "operator+", linkageName: "_ZN2S5plERKS_", scope: !195, file: !3, line: 64, type: !208, scopeLine: 64, flags: DIFlagPrototyped, spFlags: 0)
!208 = !DISubroutineType(types: !209)
!209 = !{!210, !201, !205}
!210 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !195, size: 64)
!211 = !DISubprogram(name: "S5", scope: !195, file: !3, line: 66, type: !212, scopeLine: 66, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!212 = !DISubroutineType(types: !213)
!213 = !{null, !201, !6}
!214 = !DILocation(line: 187, column: 6, scope: !78)
!215 = !DILocalVariable(name: "i", scope: !78, file: !3, line: 188, type: !6)
!216 = !DILocation(line: 188, column: 7, scope: !78)
!217 = !DILocalVariable(name: "j", scope: !78, file: !3, line: 189, type: !106)
!218 = !DILocation(line: 189, column: 8, scope: !78)
!219 = !DILocalVariable(name: "p", scope: !78, file: !3, line: 190, type: !220)
!220 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !37, size: 64)
!221 = !DILocation(line: 190, column: 7, scope: !78)
!222 = !DILocalVariable(name: "r", scope: !78, file: !3, line: 191, type: !223)
!223 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !184, size: 64)
!224 = !DILocation(line: 191, column: 14, scope: !78)
!225 = !DILocation(line: 191, column: 21, scope: !78)
!226 = !DILocation(line: 191, column: 18, scope: !78)
!227 = !DILocalVariable(name: "q", scope: !78, file: !3, line: 192, type: !106)
!228 = !DILocation(line: 192, column: 8, scope: !78)
!229 = !DILocation(line: 192, column: 15, scope: !78)
!230 = !DILocation(line: 192, column: 12, scope: !78)
!231 = !DILocalVariable(name: "fl", scope: !78, file: !3, line: 193, type: !14)
!232 = !DILocation(line: 193, column: 9, scope: !78)
!233 = !DILocation(line: 195, column: 3, scope: !78)
!234 = !DILocation(line: 197, column: 3, scope: !78)
!235 = !DILocation(line: 199, column: 3, scope: !78)
!236 = !DILocation(line: 201, column: 3, scope: !78)
!237 = !DILocation(line: 203, column: 3, scope: !78)
!238 = !DILocation(line: 205, column: 3, scope: !78)
!239 = !DILocation(line: 207, column: 3, scope: !78)
!240 = !DILocation(line: 209, column: 3, scope: !78)
!241 = !DILocation(line: 211, column: 3, scope: !78)
!242 = !DILocation(line: 213, column: 3, scope: !78)
!243 = !DILocation(line: 215, column: 3, scope: !78)
!244 = !DILocation(line: 217, column: 3, scope: !78)
!245 = !DILocation(line: 219, column: 3, scope: !78)
!246 = !DILocation(line: 221, column: 3, scope: !78)
!247 = !DILocation(line: 223, column: 3, scope: !78)
!248 = !DILocation(line: 225, column: 3, scope: !78)
!249 = !DILocation(line: 227, column: 3, scope: !78)
!250 = !DILocation(line: 229, column: 3, scope: !78)
!251 = !DILocation(line: 231, column: 3, scope: !78)
!252 = !DILocation(line: 233, column: 3, scope: !78)
!253 = !DILocation(line: 235, column: 3, scope: !78)
!254 = !DILocation(line: 237, column: 3, scope: !78)
!255 = !DILocation(line: 239, column: 3, scope: !78)
!256 = !DILocation(line: 241, column: 3, scope: !78)
!257 = !DILocation(line: 243, column: 3, scope: !78)
!258 = !DILocation(line: 245, column: 3, scope: !78)
!259 = !DILocation(line: 248, column: 3, scope: !78)
!260 = !DILocation(line: 250, column: 3, scope: !78)
!261 = !DILocation(line: 252, column: 3, scope: !78)
!262 = !DILocation(line: 256, column: 3, scope: !78)
!263 = !DILocation(line: 258, column: 1, scope: !78)
!264 = !DILocation(line: 259, column: 1, scope: !78)
!265 = !DILocalVariable(name: "i", scope: !266, file: !3, line: 260, type: !6)
!266 = distinct !DILexicalBlock(scope: !78, file: !3, line: 260, column: 3)
!267 = !DILocation(line: 260, column: 12, scope: !266)
!268 = !DILocation(line: 260, column: 8, scope: !266)
!269 = !DILocation(line: 260, column: 19, scope: !270)
!270 = distinct !DILexicalBlock(scope: !266, file: !3, line: 260, column: 3)
!271 = !DILocation(line: 260, column: 21, scope: !270)
!272 = !DILocation(line: 260, column: 3, scope: !266)
!273 = !DILocation(line: 262, column: 1, scope: !274)
!274 = distinct !DILexicalBlock(scope: !270, file: !3, line: 261, column: 3)
!275 = !DILocation(line: 260, column: 27, scope: !270)
!276 = !DILocation(line: 260, column: 3, scope: !270)
!277 = distinct !{!277, !272, !278}
!278 = !DILocation(line: 262, column: 1, scope: !266)
!279 = !DILocation(line: 264, column: 5, scope: !78)
!280 = !DILocation(line: 266, column: 1, scope: !78)
!281 = !DILocation(line: 267, column: 1, scope: !78)
!282 = !DILocalVariable(name: "i", scope: !283, file: !3, line: 268, type: !6)
!283 = distinct !DILexicalBlock(scope: !78, file: !3, line: 268, column: 3)
!284 = !DILocation(line: 268, column: 12, scope: !283)
!285 = !DILocation(line: 268, column: 8, scope: !283)
!286 = !DILocation(line: 268, column: 19, scope: !287)
!287 = distinct !DILexicalBlock(scope: !283, file: !3, line: 268, column: 3)
!288 = !DILocation(line: 268, column: 21, scope: !287)
!289 = !DILocation(line: 268, column: 3, scope: !283)
!290 = !DILocation(line: 270, column: 1, scope: !291)
!291 = distinct !DILexicalBlock(scope: !287, file: !3, line: 269, column: 3)
!292 = !DILocation(line: 268, column: 27, scope: !287)
!293 = !DILocation(line: 268, column: 3, scope: !287)
!294 = distinct !{!294, !289, !295}
!295 = !DILocation(line: 270, column: 1, scope: !283)
!296 = !DILocation(line: 272, column: 5, scope: !78)
!297 = !DILocation(line: 275, column: 3, scope: !78)
!298 = !DILocation(line: 276, column: 16, scope: !78)
!299 = !DILocation(line: 276, column: 10, scope: !78)
!300 = !DILocation(line: 276, column: 30, scope: !78)
!301 = !DILocation(line: 276, column: 24, scope: !78)
!302 = !DILocation(line: 276, column: 22, scope: !78)
!303 = !DILocation(line: 276, column: 3, scope: !78)
!304 = distinct !DISubprogram(name: "S4", linkageName: "_ZN2S4C2Ei", scope: !143, file: !3, line: 53, type: !159, scopeLine: 53, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !158, retainedNodes: !4)
!305 = !DILocalVariable(name: "this", arg: 1, scope: !304, type: !306, flags: DIFlagArtificial | DIFlagObjectPointer)
!306 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !143, size: 64)
!307 = !DILocation(line: 0, scope: !304)
!308 = !DILocalVariable(name: "v", arg: 2, scope: !304, file: !3, line: 53, type: !6)
!309 = !DILocation(line: 53, column: 10, scope: !304)
!310 = !DILocation(line: 53, column: 15, scope: !304)
!311 = !DILocation(line: 53, column: 17, scope: !304)
!312 = !DILocation(line: 54, column: 1, scope: !304)
!313 = distinct !DISubprogram(name: "S5", linkageName: "_ZN2S5C2Ei", scope: !195, file: !3, line: 66, type: !212, scopeLine: 66, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, declaration: !211, retainedNodes: !4)
!314 = !DILocalVariable(name: "this", arg: 1, scope: !313, type: !315, flags: DIFlagArtificial | DIFlagObjectPointer)
!315 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !195, size: 64)
!316 = !DILocation(line: 0, scope: !313)
!317 = !DILocalVariable(name: "v", arg: 2, scope: !313, file: !3, line: 66, type: !6)
!318 = !DILocation(line: 66, column: 10, scope: !313)
!319 = !DILocation(line: 66, column: 15, scope: !313)
!320 = !DILocation(line: 66, column: 17, scope: !313)
!321 = !DILocation(line: 67, column: 1, scope: !313)
!322 = distinct !DISubprogram(name: "tmain<int>", linkageName: "_Z5tmainIiET_S0_", scope: !3, file: !3, line: 82, type: !323, scopeLine: 82, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, templateParams: !325, retainedNodes: !4)
!323 = !DISubroutineType(types: !324)
!324 = !{!6, !6}
!325 = !{!326}
!326 = !DITemplateTypeParameter(name: "T", type: !6)
!327 = !DILocalVariable(name: "argc", arg: 1, scope: !322, file: !3, line: 82, type: !6)
!328 = !DILocation(line: 82, column: 11, scope: !322)
!329 = !DILocalVariable(name: "d", scope: !322, file: !3, line: 83, type: !184)
!330 = !DILocation(line: 83, column: 11, scope: !322)
!331 = !DILocalVariable(name: "da", scope: !322, file: !3, line: 84, type: !187)
!332 = !DILocation(line: 84, column: 11, scope: !322)
!333 = !DILocalVariable(name: "qa", scope: !322, file: !3, line: 86, type: !190)
!334 = !DILocation(line: 86, column: 5, scope: !322)
!335 = !DILocalVariable(name: "i", scope: !322, file: !3, line: 88, type: !6)
!336 = !DILocation(line: 88, column: 5, scope: !322)
!337 = !DILocalVariable(name: "j", scope: !322, file: !3, line: 89, type: !106)
!338 = !DILocation(line: 89, column: 6, scope: !322)
!339 = !DILocalVariable(name: "p", scope: !322, file: !3, line: 90, type: !220)
!340 = !DILocation(line: 90, column: 7, scope: !322)
!341 = !DILocalVariable(name: "r", scope: !322, file: !3, line: 91, type: !223)
!342 = !DILocation(line: 91, column: 12, scope: !322)
!343 = !DILocation(line: 91, column: 24, scope: !322)
!344 = !DILocation(line: 91, column: 16, scope: !322)
!345 = !DILocalVariable(name: "q", scope: !322, file: !3, line: 92, type: !106)
!346 = !DILocation(line: 92, column: 6, scope: !322)
!347 = !DILocation(line: 92, column: 18, scope: !322)
!348 = !DILocation(line: 92, column: 10, scope: !322)
!349 = !DILocalVariable(name: "fl", scope: !322, file: !3, line: 93, type: !6)
!350 = !DILocation(line: 93, column: 5, scope: !322)
!351 = !DILocation(line: 95, column: 3, scope: !322)
!352 = !DILocation(line: 97, column: 3, scope: !322)
!353 = !DILocation(line: 99, column: 3, scope: !322)
!354 = !DILocation(line: 101, column: 3, scope: !322)
!355 = !DILocation(line: 103, column: 3, scope: !322)
!356 = !DILocation(line: 105, column: 3, scope: !322)
!357 = !DILocation(line: 107, column: 3, scope: !322)
!358 = !DILocation(line: 109, column: 3, scope: !322)
!359 = !DILocation(line: 111, column: 3, scope: !322)
!360 = !DILocation(line: 113, column: 3, scope: !322)
!361 = !DILocation(line: 115, column: 3, scope: !322)
!362 = !DILocation(line: 117, column: 3, scope: !322)
!363 = !DILocation(line: 119, column: 3, scope: !322)
!364 = !DILocation(line: 121, column: 3, scope: !322)
!365 = !DILocation(line: 123, column: 3, scope: !322)
!366 = !DILocation(line: 125, column: 3, scope: !322)
!367 = !DILocation(line: 127, column: 3, scope: !322)
!368 = !DILocation(line: 129, column: 3, scope: !322)
!369 = !DILocation(line: 131, column: 3, scope: !322)
!370 = !DILocation(line: 133, column: 3, scope: !322)
!371 = !DILocation(line: 135, column: 3, scope: !322)
!372 = !DILocation(line: 137, column: 3, scope: !322)
!373 = !DILocation(line: 139, column: 3, scope: !322)
!374 = !DILocation(line: 141, column: 3, scope: !322)
!375 = !DILocation(line: 143, column: 3, scope: !322)
!376 = !DILocation(line: 146, column: 3, scope: !322)
!377 = !DILocation(line: 148, column: 3, scope: !322)
!378 = !DILocation(line: 150, column: 3, scope: !322)
!379 = !DILocation(line: 154, column: 3, scope: !322)
!380 = !DILocation(line: 156, column: 1, scope: !322)
!381 = !DILocation(line: 157, column: 1, scope: !322)
!382 = !DILocalVariable(name: "i", scope: !383, file: !3, line: 158, type: !6)
!383 = distinct !DILexicalBlock(scope: !322, file: !3, line: 158, column: 3)
!384 = !DILocation(line: 158, column: 12, scope: !383)
!385 = !DILocation(line: 158, column: 8, scope: !383)
!386 = !DILocation(line: 158, column: 19, scope: !387)
!387 = distinct !DILexicalBlock(scope: !383, file: !3, line: 158, column: 3)
!388 = !DILocation(line: 158, column: 21, scope: !387)
!389 = !DILocation(line: 158, column: 3, scope: !383)
!390 = !DILocation(line: 160, column: 1, scope: !391)
!391 = distinct !DILexicalBlock(scope: !387, file: !3, line: 159, column: 3)
!392 = !DILocation(line: 158, column: 27, scope: !387)
!393 = !DILocation(line: 158, column: 3, scope: !387)
!394 = distinct !{!394, !389, !395}
!395 = !DILocation(line: 160, column: 1, scope: !383)
!396 = !DILocation(line: 162, column: 5, scope: !322)
!397 = !DILocation(line: 164, column: 1, scope: !322)
!398 = !DILocation(line: 165, column: 1, scope: !322)
!399 = !DILocalVariable(name: "i", scope: !400, file: !3, line: 166, type: !6)
!400 = distinct !DILexicalBlock(scope: !322, file: !3, line: 166, column: 3)
!401 = !DILocation(line: 166, column: 12, scope: !400)
!402 = !DILocation(line: 166, column: 8, scope: !400)
!403 = !DILocation(line: 166, column: 19, scope: !404)
!404 = distinct !DILexicalBlock(scope: !400, file: !3, line: 166, column: 3)
!405 = !DILocation(line: 166, column: 21, scope: !404)
!406 = !DILocation(line: 166, column: 3, scope: !400)
!407 = !DILocation(line: 168, column: 1, scope: !408)
!408 = distinct !DILexicalBlock(scope: !404, file: !3, line: 167, column: 3)
!409 = !DILocation(line: 166, column: 27, scope: !404)
!410 = !DILocation(line: 166, column: 3, scope: !404)
!411 = distinct !{!411, !406, !412}
!412 = !DILocation(line: 168, column: 1, scope: !400)
!413 = !DILocation(line: 170, column: 5, scope: !322)
!414 = !DILocation(line: 171, column: 3, scope: !322)
!415 = distinct !DISubprogram(name: "tmain<float>", linkageName: "_Z5tmainIfET_S0_", scope: !3, file: !3, line: 82, type: !416, scopeLine: 82, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, templateParams: !418, retainedNodes: !4)
!416 = !DISubroutineType(types: !417)
!417 = !{!14, !14}
!418 = !{!419}
!419 = !DITemplateTypeParameter(name: "T", type: !14)
!420 = !DILocalVariable(name: "argc", arg: 1, scope: !415, file: !3, line: 82, type: !14)
!421 = !DILocation(line: 82, column: 11, scope: !415)
!422 = !DILocalVariable(name: "d", scope: !415, file: !3, line: 83, type: !16)
!423 = !DILocation(line: 83, column: 11, scope: !415)
!424 = !DILocalVariable(name: "da", scope: !415, file: !3, line: 84, type: !425)
!425 = !DICompositeType(tag: DW_TAG_array_type, baseType: !16, size: 160, elements: !33)
!426 = !DILocation(line: 84, column: 11, scope: !415)
!427 = !DILocalVariable(name: "qa", scope: !415, file: !3, line: 86, type: !428)
!428 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 160, elements: !33)
!429 = !DILocation(line: 86, column: 5, scope: !415)
!430 = !DILocalVariable(name: "i", scope: !415, file: !3, line: 88, type: !14)
!431 = !DILocation(line: 88, column: 5, scope: !415)
!432 = !DILocalVariable(name: "j", scope: !415, file: !3, line: 89, type: !433)
!433 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !14, size: 64)
!434 = !DILocation(line: 89, column: 6, scope: !415)
!435 = !DILocalVariable(name: "p", scope: !415, file: !3, line: 90, type: !220)
!436 = !DILocation(line: 90, column: 7, scope: !415)
!437 = !DILocalVariable(name: "r", scope: !415, file: !3, line: 91, type: !438)
!438 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !16, size: 64)
!439 = !DILocation(line: 91, column: 12, scope: !415)
!440 = !DILocation(line: 91, column: 24, scope: !415)
!441 = !DILocation(line: 91, column: 16, scope: !415)
!442 = !DILocalVariable(name: "q", scope: !415, file: !3, line: 92, type: !433)
!443 = !DILocation(line: 92, column: 6, scope: !415)
!444 = !DILocation(line: 92, column: 18, scope: !415)
!445 = !DILocation(line: 92, column: 10, scope: !415)
!446 = !DILocalVariable(name: "fl", scope: !415, file: !3, line: 93, type: !14)
!447 = !DILocation(line: 93, column: 5, scope: !415)
!448 = !DILocation(line: 95, column: 3, scope: !415)
!449 = !DILocation(line: 97, column: 3, scope: !415)
!450 = !DILocation(line: 99, column: 3, scope: !415)
!451 = !DILocation(line: 101, column: 3, scope: !415)
!452 = !DILocation(line: 103, column: 3, scope: !415)
!453 = !DILocation(line: 105, column: 3, scope: !415)
!454 = !DILocation(line: 107, column: 3, scope: !415)
!455 = !DILocation(line: 109, column: 3, scope: !415)
!456 = !DILocation(line: 111, column: 3, scope: !415)
!457 = !DILocation(line: 113, column: 3, scope: !415)
!458 = !DILocation(line: 115, column: 3, scope: !415)
!459 = !DILocation(line: 117, column: 3, scope: !415)
!460 = !DILocation(line: 119, column: 3, scope: !415)
!461 = !DILocation(line: 121, column: 3, scope: !415)
!462 = !DILocation(line: 123, column: 3, scope: !415)
!463 = !DILocation(line: 125, column: 3, scope: !415)
!464 = !DILocation(line: 127, column: 3, scope: !415)
!465 = !DILocation(line: 129, column: 3, scope: !415)
!466 = !DILocation(line: 131, column: 3, scope: !415)
!467 = !DILocation(line: 133, column: 3, scope: !415)
!468 = !DILocation(line: 135, column: 3, scope: !415)
!469 = !DILocation(line: 137, column: 3, scope: !415)
!470 = !DILocation(line: 139, column: 3, scope: !415)
!471 = !DILocation(line: 141, column: 3, scope: !415)
!472 = !DILocation(line: 143, column: 3, scope: !415)
!473 = !DILocation(line: 146, column: 3, scope: !415)
!474 = !DILocation(line: 148, column: 3, scope: !415)
!475 = !DILocation(line: 150, column: 3, scope: !415)
!476 = !DILocation(line: 154, column: 3, scope: !415)
!477 = !DILocation(line: 156, column: 1, scope: !415)
!478 = !DILocation(line: 157, column: 1, scope: !415)
!479 = !DILocalVariable(name: "i", scope: !480, file: !3, line: 158, type: !6)
!480 = distinct !DILexicalBlock(scope: !415, file: !3, line: 158, column: 3)
!481 = !DILocation(line: 158, column: 12, scope: !480)
!482 = !DILocation(line: 158, column: 8, scope: !480)
!483 = !DILocation(line: 158, column: 19, scope: !484)
!484 = distinct !DILexicalBlock(scope: !480, file: !3, line: 158, column: 3)
!485 = !DILocation(line: 158, column: 21, scope: !484)
!486 = !DILocation(line: 158, column: 3, scope: !480)
!487 = !DILocation(line: 160, column: 1, scope: !488)
!488 = distinct !DILexicalBlock(scope: !484, file: !3, line: 159, column: 3)
!489 = !DILocation(line: 158, column: 27, scope: !484)
!490 = !DILocation(line: 158, column: 3, scope: !484)
!491 = distinct !{!491, !486, !492}
!492 = !DILocation(line: 160, column: 1, scope: !480)
!493 = !DILocation(line: 162, column: 5, scope: !415)
!494 = !DILocation(line: 164, column: 1, scope: !415)
!495 = !DILocation(line: 165, column: 1, scope: !415)
!496 = !DILocalVariable(name: "i", scope: !497, file: !3, line: 166, type: !6)
!497 = distinct !DILexicalBlock(scope: !415, file: !3, line: 166, column: 3)
!498 = !DILocation(line: 166, column: 12, scope: !497)
!499 = !DILocation(line: 166, column: 8, scope: !497)
!500 = !DILocation(line: 166, column: 19, scope: !501)
!501 = distinct !DILexicalBlock(scope: !497, file: !3, line: 166, column: 3)
!502 = !DILocation(line: 166, column: 21, scope: !501)
!503 = !DILocation(line: 166, column: 3, scope: !497)
!504 = !DILocation(line: 168, column: 1, scope: !505)
!505 = distinct !DILexicalBlock(scope: !501, file: !3, line: 167, column: 3)
!506 = !DILocation(line: 166, column: 27, scope: !501)
!507 = !DILocation(line: 166, column: 3, scope: !501)
!508 = distinct !{!508, !503, !509}
!509 = !DILocation(line: 168, column: 1, scope: !497)
!510 = !DILocation(line: 170, column: 5, scope: !415)
!511 = !DILocation(line: 171, column: 3, scope: !415)
!512 = distinct !DISubprogram(linkageName: "_GLOBAL__sub_I_target_teams_reduction_messages.cpp", scope: !3, file: !3, type: !513, flags: DIFlagArtificial, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!513 = !DISubroutineType(types: !4)
!514 = !DILocation(line: 0, scope: !512)
