# TOY 语言词法分析器


```c
/** 
    1. 规定文件的结束
    2. 当前token是数值类型  
    3. 当前token 是标识符 
    4. 当前token是括号  
    5. 当前token是 def 声明，之后是函数定义
*/
enum Token_Type {
    EOF_TOEKN = 0,          
    NUMERIC_TOKEN,
    IDENTIFIER_TOKEN,
    PARAN_TOKEN,
    DEF_TOKEN
}




```