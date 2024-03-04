#include <iostream>

#include "printer.hpp"

using namespace std;
//=================================================================================================
void                         CPrinter::Print               ( vector<AST::ANode> & ns )
{
  CPrinter printer;
  for ( auto node : ns )
  {
    node -> Accept ( printer );
    printer . m_Result . append ("\n");
  }

  cout << printer . m_Result << endl;
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitGroup          ( AST::CGroupNode  & n )
{
  m_Result . append ( "( GROUP " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitInt            ( AST::CIntNode    & n )
{
  m_Result . append ( to_string ( n . Value() ) );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitString         ( AST::CStringNode & n )
{
  m_Result . append ( n . Value() );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitIdentifier     ( AST::CIdentifierNode & n )
{
  m_Result . append ( "id-" );
  m_Result . append ( n . Name () );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitCall           ( AST::CCallNode   & n )
{
  m_Result . append ( "( CALL " );

  m_Result . append ( n . Callee () );

  m_Result . append ( " ;ARGS " );
  for ( auto arg : n . Arguments () )
  {
    arg -> Accept ( *this );
    m_Result . append ( " " );
  }
  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitAssign         ( AST::CAssignNode & n )
{
  m_Result . append ( "( ASSIGN " );
  m_Result . append ( n . Name () );
  m_Result . append ( " = " );
  n . Value () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitUnary          ( AST::CUnaryNode  & n )
{}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitNot            ( AST::CNotNode    & n )
{
  m_Result . append ( "( NOT " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitPos            ( AST::CPosNode    & n )
{
  m_Result . append ( "( + " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitNeg            ( AST::CNegNode    & n )
{
  m_Result . append ( "( - " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitBinary         ( AST::CBinaryNode & n )
{}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitEqual          ( AST::CEqualNode  & n )
{
  m_Result . append ( "( = " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitNotEq          ( AST::CNotEqNode  & n )
{
  m_Result . append ( "( <> " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitGreat          ( AST::CGreatNode  & n )
{
  m_Result . append ( "( > " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitGreatEq        ( AST::CGreatEqNode& n )
{
  m_Result . append ( "( >= " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitLess           ( AST::CLessNode   & n )
{
  m_Result . append ( "( < " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitLessEq         ( AST::CLessEqNode & n )
{
  m_Result . append ( "( <= " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitPlus           ( AST::CPlusNode   & n )
{
  m_Result . append ( "( + " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitMinus          ( AST::CMinusNode  & n )
{
  m_Result . append ( "( - " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitMul            ( AST::CMulNode    & n )
{
  m_Result . append ( "( * " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitDiv            ( AST::CDivNode    & n )
{
  m_Result . append ( "( div " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitOr             ( AST::COrNode     & n )
{
  m_Result . append ( "( or " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitAnd            ( AST::CAndNode    & n )
{
  m_Result . append ( "( and " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitMod            ( AST::CModNode    & n )
{
  m_Result . append ( "( mod " );
  n . LeftChild () -> Accept ( *this );
  m_Result . append ( "  " );
  n . RightChild () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitExprStmt       ( AST::CExprStmtNode & n )
{
  m_Result . append ( "( EXPRSTMT " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitWrtInt         ( AST::CWrtIntNode & n )
{
  m_Result . append ( "( WRITEINT " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitWrtStr         ( AST::CWrtStrNode & n )
{
  m_Result . append ( "( WRITESTR " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}

//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitRdlStmt        ( AST::CRdlStmtNode& n )  
{
  m_Result . append ( "( READ " );
  n . Child () -> Accept ( *this );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitVar            ( AST::CVarNode    & n )
{
  m_Result . append ( "( VAR " );
  m_Result . append ( n . Name () );
  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitVars           ( AST::CVarsNode   & n )
{
  m_Result . append ( "( VARS " );
  for ( auto var : n . Vars () )
  {
    m_Result . append ( var -> Name () );
    m_Result . append ( " " );
  }
  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitConst          ( AST::CConstNode  & n )
{
  m_Result . append ( "( CONST " );
  m_Result . append ( n . Name () );
  m_Result . append ( " : " );
  m_Result . append ( to_string ( n . Value () ) );
  m_Result . append ( " )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitConsts         ( AST::CConstsNode & n )
{
  m_Result . append ( "( CONSTS " );
  for ( auto var : n . Consts () )
  {
    m_Result . append ( var -> Name () );
    m_Result . append ( "=" );
    m_Result . append ( to_string ( var -> Value ()) );
    m_Result . append ( " " );
  }
  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitBlock          ( AST::CBlockNode & n )
{
  m_Result . append ( "( BLOK " );

  for ( auto stmt : n . Stmts () )
  {
    stmt -> Accept ( *this );
    m_Result . append ( " " );
  }

  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitIf             ( AST::CIfNode     & n )
{
  m_Result . append ( "( IF " );
  n . Condition () -> Accept ( *this );
  m_Result . append ( "; THEN " );
  n . Then () -> Accept ( *this );
  
  if ( n . Else () )
  {
    m_Result . append ( "; ELSE " );
    n . Else () -> Accept ( *this );
  }

  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitWhile          ( AST::CWhileNode  & n )
{
  m_Result . append ( "( WHILE " );
  n . Condition () -> Accept ( *this );
  m_Result . append ( "; THEN " );
  n . Body () -> Accept ( *this );

  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitFunDecl        ( AST::CFunDeclNode& n )
{
  m_Result . append ( "( DECL " );
  m_Result . append ( n . Name () );
  m_Result . append ( " ; PARAMS " );
  for ( auto expr : n . Parameters () )
  {
    m_Result . append ( " " );
    expr -> Accept ( *this );
  }
  if (  n . Body () )
  {
    m_Result . append ( "; BODY " );
    n . Body () -> Accept ( *this );
  }

  m_Result . append ( " ) " );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitExit           ( AST::CExitNode   & n )
{
  m_Result . append ( "( EXIT )" );
}
//-------------------------------------------------------------------------------------------------
void                         CPrinter::VisitBreak          ( AST::CBreakNode   & n )
{
  m_Result . append ( "( BREAK )" );
}
//=================================================================================================