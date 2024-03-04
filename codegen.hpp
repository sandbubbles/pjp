#ifndef PJPPROJECT_CODEGEN_HPP
#define PJPPROJECT_CODEGEN_HPP
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <stack>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "visitor.hpp"
#include "astnode.hpp"
/* Simle codegen for mila. Some things are not done properly. For example constant are set
*/
//=================================================================================================
class CCodeGen : public CVisitor
{
  public:
                             CCodeGen                      ( void );
    virtual                 ~CCodeGen                      ( void ) override = default;       
    static void              Generate                      ( std::vector<AST::ANode> & ns );
    void                     VisitGroup                    ( AST::CGroupNode  & n ) override;
    void                     VisitInt                      ( AST::CIntNode    & n ) override;
    void                     VisitString                   ( AST::CStringNode & n ) override;
    void                     VisitIdentifier               ( AST::CIdentifierNode & n ) override;
    void                     VisitCall                     ( AST::CCallNode   & n ) override;
    void                     VisitAssign                   ( AST::CAssignNode & n ) override;
    void                     VisitNot                      ( AST::CNotNode    & n ) override;
    void                     VisitPos                      ( AST::CPosNode    & n ) override;
    void                     VisitNeg                      ( AST::CNegNode    & n ) override;
    void                     VisitEqual                    ( AST::CEqualNode  & n ) override;
    void                     VisitNotEq                    ( AST::CNotEqNode  & n ) override;
    void                     VisitGreat                    ( AST::CGreatNode  & n ) override;
    void                     VisitGreatEq                  ( AST::CGreatEqNode & n ) override;
    void                     VisitLess                     ( AST::CLessNode   & n ) override;
    void                     VisitLessEq                   ( AST::CLessEqNode & n ) override;
    void                     VisitPlus                     ( AST::CPlusNode   & n ) override;
    void                     VisitMinus                    ( AST::CMinusNode  & n ) override;
    void                     VisitMul                      ( AST::CMulNode    & n ) override;
    void                     VisitDiv                      ( AST::CDivNode    & n ) override;
    void                     VisitOr                       ( AST::COrNode     & n ) override;
    void                     VisitAnd                      ( AST::CAndNode    & n ) override;
    void                     VisitMod                      ( AST::CModNode    & n ) override;
    void                     VisitExprStmt                 ( AST::CExprStmtNode & n ) override;
    void                     VisitWrtInt                   ( AST::CWrtIntNode & n ) override;
    void                     VisitWrtStr                   ( AST::CWrtStrNode & n ) override;
    void                     VisitRdlStmt                  ( AST::CRdlStmtNode & n ) override;
    void                     VisitVar                      ( AST::CVarNode    & n ) override;
    void                     VisitConst                    ( AST::CConstNode  & n ) override;
    void                     VisitVars                     ( AST::CVarsNode   & n ) override;   
    void                     VisitConsts                   ( AST::CConstsNode & n ) override;
    void                     VisitBlock                    ( AST::CBlockNode  & n ) override;
    void                     VisitIf                       ( AST::CIfNode     & n ) override;
    void                     VisitWhile                    ( AST::CWhileNode  & n ) override;
    void                     VisitFunDecl                  ( AST::CFunDeclNode & n ) override;
    void                     VisitExit                     ( AST::CExitNode   & n ) override;
    void                     VisitBreak                    ( AST::CBreakNode  & n ) override;
  
  private:
    class CCodegenException : public std::exception
    {
      const char           * what                          ( void ) const noexcept override;
    };
    CCodegenException        error                         ( std::string       message );
    void                     createIO                      ( void );
    llvm::AllocaInst       * createEntryBAlloca            ( llvm::Function  * function,
                                                             const std::string & name ); 
    llvm::Value            * findByName                    ( AST::CIdentifierNode & n );
    llvm::Function         * prototype                     ( AST::CFunDeclNode & n );
  
  private:
    llvm::LLVMContext                          m_Context;
    llvm::IRBuilder<>                          m_Builder;
    std::unique_ptr<llvm::Module>              m_Module;
    std::map<std::string, llvm::GlobalValue *> m_GlobalVars;
    std::map<std::string, llvm::AllocaInst*>   m_LocalVars;
    std::map<std::string, llvm::GlobalValue *> m_Constants;
    std::stack<llvm::BasicBlock*>              m_Scopes;
    llvm::Value                              * m_Value;
};
//=================================================================================================
#endif //PJPPROJECT_CODEGEN_HPP