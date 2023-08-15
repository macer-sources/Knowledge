# 常见 opt 转换 pass

| Name   |      Desc       |
|----------|:-------------:|
| adce    | 入侵式无用代码消除 |
| bb-vectorize | 基本块向量化 | 
| constprop | 简单常量传播 | 
| dce    | 无用代码消除 | 
| deadargelim | 无用参数消除 | 
| globaldce | 无用全局变量消除 | 
| globalopt | 全局变量优化 | 
| gvn | 全局变量编号 | 
| inline | 函数内联 | 
| instcombine | 冗余指令合并 |
| licm   | 循环常量代码外提 |
| loop-unswitch | 循环外提|
| loweratomic | 原子内建函数lowering | 
| lowerinvoke | invode指令lowering，以支持不稳定的代码生成器 | 
| lowerswitch |  switch指令lowering | 
| mem2reg | 内存访问优化 | 
| memcpyopt | MemCpy优化 | 
| simplifycfg | 简化CFG | 
| sink | 代码提升 | 
| tailcallelim | 尾调用消除 |