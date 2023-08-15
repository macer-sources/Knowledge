int mult() {
int a =5;
int b = 3;
int c = a * b;
return c;
}


// clang -emit-llvm -S multiply.c -o multiply.ll    将C语言代码转换成LLVM IR
// clang -cc1 -emit-llvm testfile.c -o testfile.ll