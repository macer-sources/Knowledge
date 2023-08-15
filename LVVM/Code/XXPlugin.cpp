#include <iostream>
#include "clang/AST/AST.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

// 声明使用命名空间
using namespace clang;
using namespace std;
using namespace llvm;
using namespace clang::ast_matchers;

// 插件命名空间
namespace XJPlugin {

    // 第三步：扫描完毕回调
    // 4、自定义回调类，继承自MatchCallback
    class XJMatchCallback : public MatchFinder::MatchCallback {

    private:
        // CI传递路径：XJASTAction类中的CreateASTConsumer方法参数 -> XJASTConsumer的构造函数 -> XJMatchCallback的私有属性，通过构造函数从XJASTConsumer构造函数中获取
        CompilerInstance &CI;

        // 判断是否是自己的文件
        bool isUserSourceCode(const string fileName) {
            // 文件名不为空
            if (fileName.empty()) return false;
            // 非Xcode中的代码都认为是用户的
            if (0 == fileName.find("/Applications/Xcode.app/")) return false;
            return true;
        }

        // 判断是否应该用copy修饰
        bool isShouldUseCopy(const string typeStr) {
            // 判断类型是否是 NSString / NSArray / NSDictionary
            if (typeStr.find("NSString") != string::npos ||
                typeStr.find("NSArray") != string::npos ||
                typeStr.find("NSDictionary") != string::npos) {
                return true;
            }
            return false;
        }

    public:
        // 构造方法
        XJMatchCallback(CompilerInstance &CI):CI(CI) {}

        // 重载run方法
        void run(const MatchFinder::MatchResult &Result) {
            // 通过Result获取节点对象，根据节点id("objcPropertyDecl")获取(此id需要与XJASTConsumer构造方法中bind的id一致)
            const ObjCPropertyDecl *propertyDecl = Result.Nodes.getNodeAs<ObjCPropertyDecl>("objcPropertyDecl");
            // 获取文件名称（包含路径）
            string fileName = CI.getSourceManager().getFilename(propertyDecl->getSourceRange().getBegin()).str();
            // 如果节点有值 && 是用户文件
            if (propertyDecl && isUserSourceCode(fileName)) {
                // 获取节点的类型，并转成字符串
                string typeStr = propertyDecl->getType().getAsString();
                // 节点的描述信息
                ObjCPropertyAttribute::Kind attrKind = propertyDecl->getPropertyAttributes();
                // 应该使用copy，但是没有使用copy
                if (isShouldUseCopy(typeStr) && !(attrKind & ObjCPropertyAttribute::kind_copy)) {
                    // 通过CI获取诊断引擎
                    DiagnosticsEngine &diag = CI.getDiagnostics();
                    // Report 报告
                    /**
                     错误位置：getLocation 节点位置
                     错误：getCustomDiagID（等级，提示）
                     */
                    diag.Report(propertyDecl->getLocation(), diag.getCustomDiagID(DiagnosticsEngine::Warning, "%0 - 这个属性推荐使用copy修饰!!"))<< typeStr;
                }
            }
        }
    };

    // 第二步：扫描配置完毕
    // 3、自定义XJASTConsumer，继承自抽象类 ASTConsumer，用于监听AST节点的信息 -- 过滤器
    class XJASTConsumer : public ASTConsumer {
    private:
        // AST 节点查找器（过滤器）
        MatchFinder matcher;
        // 回调对象
        XJMatchCallback callback;

    public:
        // 构造方法中创建MatchFinder对象
        XJASTConsumer(CompilerInstance &CI):callback(CI) { // 构造即将CI传递给callback
            // 添加一个MatchFinder，每个objcPropertyDecl节点绑定一个objcPropertyDecl标识（去匹配objcPropertyDecl节点）
            // 回调callback，其实是在CJLMatchCallback里面重写run方法（真正回调的是回调run方法）
matcher.addMatcher(objcPropertyDecl().bind("objcPropertyDecl"), &callback);
        }

        // 重载两个方法 HandleTopLevelDecl 和 HandleTranslationUnit

        // 解析完毕一个顶级的声明就回调一次（顶级节点，即全局变量，属性，函数等）
        bool HandleTopLevelDecl(DeclGroupRef D) {
//            cout<<"正在解析..."<<endl;
            return true;
        }

        // 当整个文件都解析完毕后回调
        void HandleTranslationUnit(ASTContext &Ctx) {
//            cout<<"文件解析完毕！！！"<<endl;
            // 将文件解析完毕后的上下文context（即AST语法树） 给 matcher
            matcher.matchAST(Ctx);
        }
    };

    //2、继承PluginASTAction，实现我们自定义的XJASTAction，即自定义AST语法树行为
    class XJASTAction : public PluginASTAction {
    public:

        // 重载ParseArgs 和 CreateASTConsumer方法

        /*
         解析给定的插件命令行参数
         - param CI 编译器实例，用于报告诊断。
         - return 如果解析成功，则为true；否则，插件将被销毁，并且不执行任何操作。该插件负责使用CompilerInstance的Diagnostic对象报告错误。
         */
        bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &arg) {
            return true;
        }

        // 返回自定义的XJASTConsumer对象，抽象类ASTConsumer的子类
        unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) {
            /**
             传递CI
             CI用于：
             - 判断文件是否是用户的
             - 抛出警告
             */
            return unique_ptr<XJASTConsumer>(new XJASTConsumer(CI));
        }
    };
}

// 第一步：注册插件，并自定义XJASTAction类
// 1、注册插件
static FrontendPluginRegistry::Add<XJPlugin::XJASTAction> X("XJPlugin", "this is XJPlugin");
