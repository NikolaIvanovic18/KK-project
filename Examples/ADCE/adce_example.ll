; ModuleID = 'adce_example.c'
source_filename = "adce_example.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @helper(i32 noundef %x) #0 {
entry:
  %x.addr = alloca i32, align 4
  store i32 %x, ptr %x.addr, align 4
  %0 = load i32, ptr %x.addr, align 4
  %mul = mul nsw i32 %0, 2
  ret i32 %mul
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @compute(i32 noundef %a, i32 noundef %b) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %dead1 = alloca i32, align 4
  %dead2 = alloca i32, align 4
  %dead3 = alloca i32, align 4
  %unused_call = alloca i32, align 4
  %live1 = alloca i32, align 4
  %live2 = alloca i32, align 4
  %never_used = alloca i32, align 4
  %after_return = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %b.addr, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, ptr %dead1, align 4
  %2 = load i32, ptr %dead1, align 4
  %mul = mul nsw i32 %2, 3
  store i32 %mul, ptr %dead2, align 4
  %3 = load i32, ptr %dead2, align 4
  %sub = sub nsw i32 %3, 7
  store i32 %sub, ptr %dead3, align 4
  %4 = load i32, ptr %a.addr, align 4
  %call = call i32 @helper(i32 noundef %4)
  store i32 %call, ptr %unused_call, align 4
  %5 = load i32, ptr %a.addr, align 4
  %6 = load i32, ptr %b.addr, align 4
  %mul1 = mul nsw i32 %5, %6
  store i32 %mul1, ptr %live1, align 4
  %7 = load i32, ptr %live1, align 4
  %add2 = add nsw i32 %7, 10
  store i32 %add2, ptr %live2, align 4
  %8 = load i32, ptr %a.addr, align 4
  %cmp = icmp sgt i32 %8, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %9 = load i32, ptr %live2, align 4
  store i32 %9, ptr %retval, align 4
  br label %return

if.else:                                          ; preds = %entry
  %10 = load i32, ptr %a.addr, align 4
  %11 = load i32, ptr %b.addr, align 4
  %sub3 = sub nsw i32 %10, %11
  store i32 %sub3, ptr %never_used, align 4
  store i32 0, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.else, %if.then
  %12 = load i32, ptr %retval, align 4
  ret i32 %12
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  %call = call i32 @compute(i32 noundef 5, i32 noundef 3)
  store i32 %call, ptr %result, align 4
  %0 = load i32, ptr %result, align 4
  ret i32 %0
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
