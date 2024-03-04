#ifndef PJPPROJECT_VISITOR_HPP
#define PJPPROJECT_VISITOR_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace AST
{
  class CNode;
  class CExprNode;
  class CGroupNode;
  class CIntNode;
  class CStringNode;
  class CUnaryNode;
  class CNotNode;
  class CPosNode;
  class CNegNode;
  class CBinaryNode;
  class CEqualNode;
  class CNotEqNode;
  class CGreatNode;
  class CGreatEqNode;
  class CLessNode;
  class CLessEqNode;
  class CPlusNode;
  class CMinusNode;
  class CMulNode;
  class CDivNode;
  class COrNode;
  class CAndNode;
  class CModNode;
  class CExprStmtNode;
  class CWrtIntNode;
  class CWrtStrNode;
  class CRdlStmtNode;
  class CIncNode;
  class CDecNode;
  class CVarNode;
  class CVarsNode;
  class CConstNode;
  class CConstsNode;
  class CIdentifierNode;
  class CCallNode;
  class CAssignNode;
  class CBlockNode;
  class CIfNode;
  class CWhileNode;
  class CFunDeclNode;
  class CExitNode;
  class CBreakNode;
}
//=================================================================================================
class CVisitor
{
  public:
    virtual                 ~CVisitor                      ( void ) = default;
    virtual void             VisitGroup                    ( AST::CGroupNode  & n ) = 0;
    virtual void             VisitInt                      ( AST::CIntNode    & n ) = 0;
    virtual void             VisitString                   ( AST::CStringNode & n ) = 0;
    virtual void             VisitIdentifier               ( AST::CIdentifierNode & n ) = 0;
    virtual void             VisitCall                     ( AST::CCallNode   & n ) = 0;
    virtual void             VisitAssign                   ( AST::CAssignNode & n ) = 0;
    virtual void             VisitUnary                    ( AST::CUnaryNode  & n ) {}
    virtual void             VisitNot                      ( AST::CNotNode    & n ) = 0;
    virtual void             VisitPos                      ( AST::CPosNode    & n ) = 0;
    virtual void             VisitNeg                      ( AST::CNegNode    & n ) = 0;
    virtual void             VisitBinary                   ( AST::CBinaryNode & n ) {}
    virtual void             VisitEqual                    ( AST::CEqualNode  & n ) = 0;
    virtual void             VisitNotEq                    ( AST::CNotEqNode  & n ) = 0;
    virtual void             VisitGreat                    ( AST::CGreatNode  & n ) = 0;
    virtual void             VisitGreatEq                  ( AST::CGreatEqNode & n ) = 0;
    virtual void             VisitLess                     ( AST::CLessNode   & n ) = 0;
    virtual void             VisitLessEq                   ( AST::CLessEqNode & n ) = 0;
    virtual void             VisitPlus                     ( AST::CPlusNode   & n ) = 0;
    virtual void             VisitMinus                    ( AST::CMinusNode  & n ) = 0;
    virtual void             VisitMul                      ( AST::CMulNode    & n ) = 0;
    virtual void             VisitDiv                      ( AST::CDivNode    & n ) = 0;
    virtual void             VisitOr                       ( AST::COrNode     & n ) = 0;
    virtual void             VisitAnd                      ( AST::CAndNode    & n ) = 0;
    virtual void             VisitMod                      ( AST::CModNode    & n ) = 0;
    virtual void             VisitExprStmt                 ( AST::CExprStmtNode & n )= 0;
    virtual void             VisitWrtInt                   ( AST::CWrtIntNode & n ) = 0;
    virtual void             VisitWrtStr                   ( AST::CWrtStrNode & n ) = 0;
    virtual void             VisitRdlStmt                  ( AST::CRdlStmtNode & n ) = 0;
    virtual void             VisitVar                      ( AST::CVarNode    & n ) = 0;
    virtual void             VisitVars                     ( AST::CVarsNode   & n ) = 0;
    virtual void             VisitConst                    ( AST::CConstNode  & n ) = 0;
    virtual void             VisitConsts                   ( AST::CConstsNode & n ) = 0;
    virtual void             VisitBlock                    ( AST::CBlockNode  & n ) = 0;
    virtual void             VisitIf                       ( AST::CIfNode     & n ) = 0;
    virtual void             VisitWhile                    ( AST::CWhileNode  & n ) = 0;
    virtual void             VisitFunDecl                  ( AST::CFunDeclNode & n ) = 0;
    virtual void             VisitExit                     ( AST::CExitNode   & n ) = 0;
    virtual void             VisitBreak                    ( AST::CBreakNode  & n ) = 0;
};
//=================================================================================================
#endif //PJPPROJECT_VISITOR_HPP