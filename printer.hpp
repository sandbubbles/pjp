#ifndef PJPPROJECT_PRINTER_HPP
#define PJPPROJECT_PRINTER_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "visitor.hpp"
#include "astnode.hpp"

//=================================================================================================
class CPrinter : public CVisitor
{
  public:
    static void              Print                         ( std::vector<AST::ANode> & ns );
    void                     VisitGroup                    ( AST::CGroupNode  & n ) override;
    void                     VisitInt                      ( AST::CIntNode    & n ) override;
    void                     VisitString                   ( AST::CStringNode & n ) override;
    void                     VisitIdentifier               ( AST::CIdentifierNode & n ) override;
    void                     VisitCall                     ( AST::CCallNode   & n ) override;
    void                     VisitAssign                   ( AST::CAssignNode & n ) override;
    void                     VisitUnary                    ( AST::CUnaryNode  & n ) override;
    void                     VisitNot                      ( AST::CNotNode    & n ) override;
    void                     VisitPos                      ( AST::CPosNode    & n ) override;
    void                     VisitNeg                      ( AST::CNegNode    & n ) override;
    void                     VisitBinary                   ( AST::CBinaryNode & n ) override;
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
    void                     VisitVars                     ( AST::CVarsNode   & n ) override;   
    void                     VisitConst                    ( AST::CConstNode  & n ) override;
    void                     VisitConsts                   ( AST::CConstsNode & n ) override;
    void                     VisitBlock                    ( AST::CBlockNode  & n ) override;
    void                     VisitIf                       ( AST::CIfNode     & n ) override;
    void                     VisitWhile                    ( AST::CWhileNode  & n ) override;
    void                     VisitFunDecl                  ( AST::CFunDeclNode & n ) override;
    void                     VisitExit                     ( AST::CExitNode   & n ) override;
    void                     VisitBreak                    ( AST::CBreakNode  & n ) override;

  private:
    std::string              m_Result                      = "";
};
//=================================================================================================
#endif //PJPPROJECT_PRINTER_HPP