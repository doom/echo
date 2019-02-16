/*
** Created by doom on 11/02/19.
*/

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "annotated_match_callback.hpp"

int main(int ac, const char **av)
{
    using namespace clang::tooling;
    using namespace clang::ast_matchers;

    llvm::cl::OptionCategory category("echogen options");
    CommonOptionsParser opt_parser(ac, av, category);
    ClangTool tool(opt_parser.getCompilations(), opt_parser.getSourcePathList());

    MatchFinder mf;

    DeclarationMatcher class_matcher = cxxRecordDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));
    DeclarationMatcher field_matcher = fieldDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));
    DeclarationMatcher function_matcher = functionDecl(decl().bind("id"), hasAttr(clang::attr::Annotate));

    echogen::annotated_match_callback match_cb;

    mf.addMatcher(class_matcher, &match_cb);
    mf.addMatcher(field_matcher, &match_cb);
    mf.addMatcher(function_matcher, &match_cb);

    return tool.run(newFrontendActionFactory(&mf).get());
}
