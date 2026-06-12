; ModuleID = 'example_tce.ll'
source_filename = "example_tce.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @factorial(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  br label %loop.header

loop.header:                                      ; preds = %10, %2
  %6 = load i32, ptr %4, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %10

8:                                                ; preds = %loop.header
  %9 = load i32, ptr %5, align 4
  store i32 %9, ptr %3, align 4
  br label %16

10:                                               ; preds = %loop.header
  %11 = load i32, ptr %4, align 4
  %12 = sub nsw i32 %11, 1
  %13 = load i32, ptr %4, align 4
  %14 = load i32, ptr %5, align 4
  %15 = mul nsw i32 %13, %14
  store i32 %12, ptr %4, align 4
  store i32 %15, ptr %5, align 4
  br label %loop.header

16:                                               ; preds = %8
  %17 = load i32, ptr %3, align 4
  ret i32 %17
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @fib(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, ptr %5, align 4
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  br label %loop.header

loop.header:                                      ; preds = %12, %3
  %8 = load i32, ptr %5, align 4
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %12

10:                                               ; preds = %loop.header
  %11 = load i32, ptr %6, align 4
  store i32 %11, ptr %4, align 4
  br label %19

12:                                               ; preds = %loop.header
  %13 = load i32, ptr %5, align 4
  %14 = sub nsw i32 %13, 1
  %15 = load i32, ptr %7, align 4
  %16 = load i32, ptr %6, align 4
  %17 = load i32, ptr %7, align 4
  %18 = add nsw i32 %16, %17
  store i32 %14, ptr %5, align 4
  store i32 %15, ptr %6, align 4
  store i32 %18, ptr %7, align 4
  br label %loop.header

19:                                               ; preds = %10
  %20 = load i32, ptr %4, align 4
  ret i32 %20
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @sum(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  br label %loop.header

loop.header:                                      ; preds = %10, %2
  %6 = load i32, ptr %4, align 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %8, label %10

8:                                                ; preds = %loop.header
  %9 = load i32, ptr %5, align 4
  store i32 %9, ptr %3, align 4
  br label %16

10:                                               ; preds = %loop.header
  %11 = load i32, ptr %4, align 4
  %12 = sub nsw i32 %11, 1
  %13 = load i32, ptr %5, align 4
  %14 = load i32, ptr %4, align 4
  %15 = add nsw i32 %13, %14
  store i32 %12, ptr %4, align 4
  store i32 %15, ptr %5, align 4
  br label %loop.header

16:                                               ; preds = %8
  %17 = load i32, ptr %3, align 4
  ret i32 %17
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @bad_factorial(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  store i32 1, ptr %2, align 4
  br label %13

7:                                                ; preds = %1
  %8 = load i32, ptr %3, align 4
  %9 = load i32, ptr %3, align 4
  %10 = sub nsw i32 %9, 1
  %11 = call i32 @bad_factorial(i32 noundef %10)
  %12 = mul nsw i32 %8, %11
  store i32 %12, ptr %2, align 4
  br label %13

13:                                               ; preds = %7, %6
  %14 = load i32, ptr %2, align 4
  ret i32 %14
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @countdown(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  br label %loop.header

loop.header:                                      ; preds = %6, %1
  %3 = load i32, ptr %2, align 4
  %4 = icmp eq i32 %3, 0
  br i1 %4, label %5, label %6

5:                                                ; preds = %loop.header
  br label %9

6:                                                ; preds = %loop.header
  %7 = load i32, ptr %2, align 4
  %8 = sub nsw i32 %7, 1
  store i32 %8, ptr %2, align 4
  br label %loop.header

9:                                                ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @collatz(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %4, align 4
  store i32 %1, ptr %5, align 4
  br label %loop.header

loop.header:                                      ; preds = %19, %14, %2
  %6 = load i32, ptr %4, align 4
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %10

8:                                                ; preds = %loop.header
  %9 = load i32, ptr %5, align 4
  store i32 %9, ptr %3, align 4
  br label %25

10:                                               ; preds = %loop.header
  %11 = load i32, ptr %4, align 4
  %12 = srem i32 %11, 2
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %14, label %19

14:                                               ; preds = %10
  %15 = load i32, ptr %4, align 4
  %16 = sdiv i32 %15, 2
  %17 = load i32, ptr %5, align 4
  %18 = add nsw i32 %17, 1
  store i32 %16, ptr %4, align 4
  store i32 %18, ptr %5, align 4
  br label %loop.header

19:                                               ; preds = %10
  %20 = load i32, ptr %4, align 4
  %21 = mul nsw i32 3, %20
  %22 = add nsw i32 %21, 1
  %23 = load i32, ptr %5, align 4
  %24 = add nsw i32 %23, 1
  store i32 %22, ptr %4, align 4
  store i32 %24, ptr %5, align 4
  br label %loop.header

25:                                               ; preds = %8
  %26 = load i32, ptr %3, align 4
  ret i32 %26
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.0"}
