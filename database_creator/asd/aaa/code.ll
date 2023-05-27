; ModuleID = 'code.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@step = common global double 0.000000e+00, align 8
@.str = private unnamed_addr constant [42 x i8] c"#pragma omp parallel for reduction(+:sum)\00", align 1
@.str1 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1

; Function Attrs: minsize nounwind optsize uwtable
define i32 @main() #0 {
  store double 1.000000e-05, double* @step, align 8, !tbaa !1
  tail call void @omp_pragma_talkad7420(i8* getelementptr inbounds ([42 x i8]* @.str, i64 0, i64 0)) #2
  tail call void (...)* @for_loop_talkad7420() #2
  %1 = load double* @step, align 8, !tbaa !1
  br label %2

; <label>:2                                       ; preds = %2, %0
  %sum.02 = phi double [ 0.000000e+00, %0 ], [ %9, %2 ]
  %i.01 = phi i32 [ 0, %0 ], [ %10, %2 ]
  %3 = sitofp i32 %i.01 to double
  %4 = fadd double %3, 5.000000e-01
  %5 = fmul double %4, %1
  %6 = fmul double %5, %5
  %7 = fadd double %6, 1.000000e+00
  %8 = fdiv double 4.000000e+00, %7
  %9 = fadd double %sum.02, %8
  %10 = add nsw i32 %i.01, 1
  %exitcond = icmp eq i32 %10, 100000
  br i1 %exitcond, label %11, label %2

; <label>:11                                      ; preds = %2
  tail call void @omp_pragma_talkad7420(i8* getelementptr inbounds ([12 x i8]* @.str1, i64 0, i64 0)) #2
  %12 = load double* @step, align 8, !tbaa !1
  %13 = fmul double %9, %12
  %14 = fptosi double %13 to i32
  ret i32 %14
}

; Function Attrs: minsize optsize
declare void @omp_pragma_talkad7420(i8*) #1

; Function Attrs: minsize optsize
declare void @for_loop_talkad7420(...) #1

attributes #0 = { minsize nounwind optsize uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { minsize optsize "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { minsize nounwind optsize }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4.2 (tags/RELEASE_34/dot2-final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"double", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
