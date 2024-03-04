#include "astnode.hpp"

using namespace std;
namespace AST
{
//-------------------------------------------------------------------------------------------------
                             CGroupNode::CGroupNode        ( AExprNode         op )
  : m_Child ( op )
{}
AExprNode                    CGroupNode::Child             ( void )
{
  return m_Child;
}
void                         CGroupNode::Accept            ( CVisitor &        v )
{
  v . VisitGroup ( *this );
}
//=================================================================================================
                             CIntNode::CIntNode            ( int               value )
  : m_Value ( value )
{}
int                          CIntNode::Value               ( void )
{
  return m_Value;
}
void                         CIntNode::Accept              ( CVisitor &        v )
{
  v . VisitInt ( *this );
}
//=================================================================================================
                             CStringNode::CStringNode      ( string             value )
  : m_Value ( value )
{}
string                       CStringNode::Value            ( void )
{
  return m_Value;
}
void                         CStringNode::Accept           ( CVisitor &        v )
{
  v . VisitString ( *this );
}
//=================================================================================================
                             CIdentifierNode::CIdentifierNode ( string         name )
  : m_Name ( name )
{}
string                     & CIdentifierNode::Name         ( void )
{
  return m_Name;
}
void                         CIdentifierNode::Accept       ( CVisitor &        v )
{
  v . VisitIdentifier ( *this );
}
//=================================================================================================
                             CCallNode::CCallNode          ( std::string       callee,
                                                             std::vector<AExprNode> args )
  : m_Callee ( callee ),
    m_Args ( args )
{}
string                     & CCallNode::Callee             ( void )
{
  return m_Callee;
}
vector<AExprNode>          & CCallNode::Arguments          ( void )
{
  return m_Args;
}
void                         CCallNode::Accept             ( CVisitor &        v )
{
  v . VisitCall ( *this );
}
//=================================================================================================
                             CAssignNode::CAssignNode      ( string            name,
                                                             AExprNode         value )
  : m_Name ( name ),
    m_Value ( value )
{}
string                     & CAssignNode::Name             ( void )
{
  return m_Name;
}
AExprNode                    CAssignNode::Value            ( void )
{
  return m_Value;
}
void                         CAssignNode::Accept           ( CVisitor &        v )
{
  v . VisitAssign ( *this );
}
//=================================================================================================
                             CUnaryNode::CUnaryNode        ( AExprNode         op )
  : m_Child ( op )
{}
AExprNode                    CUnaryNode::Child             ( void )
{
  return m_Child;
}
void                         CUnaryNode::Accept            ( CVisitor &        v )
{
  v . VisitUnary ( *this );
}
//=================================================================================================
                             CNotNode::CNotNode            ( AExprNode         op )
  : CUnaryNode ( op )
{}
void                         CNotNode::Accept              ( CVisitor &        v )
{
  v . VisitNot ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CPosNode::CPosNode            ( AExprNode         op )
  : CUnaryNode ( op )
{}
void                         CPosNode::Accept              ( CVisitor &        v )
{
  v . VisitPos ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CNegNode::CNegNode            ( AExprNode         op )
  : CUnaryNode ( op )
{}
void                         CNegNode::Accept              ( CVisitor &        v )
{
  v . VisitNeg ( *this );
}
//=================================================================================================
                             CBinaryNode::CBinaryNode      ( AExprNode         left,
                                                             AExprNode         right )
  : m_Left  ( left ),
    m_Right ( right )
{}
AExprNode                    CBinaryNode::LeftChild             ( void )
{
  return m_Left;
}
AExprNode                    CBinaryNode::RightChild             ( void )
{
  return m_Right;
}
void                         CBinaryNode::Accept           ( CVisitor &        v )
{
  v . VisitBinary ( *this );
}
//=================================================================================================
                             CEqualNode::CEqualNode        ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CEqualNode::Accept            ( CVisitor &        v )
{
  v . VisitEqual ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CNotEqNode::CNotEqNode        ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CNotEqNode::Accept            ( CVisitor &        v )
{
  v . VisitNotEq ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CGreatNode::CGreatNode        ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CGreatNode::Accept            ( CVisitor &        v )
{
  v . VisitGreat ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CGreatEqNode::CGreatEqNode    ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CGreatEqNode::Accept          ( CVisitor &        v )
{
  v . VisitGreatEq ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CLessNode::CLessNode          ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CLessNode::Accept             ( CVisitor &        v )
{
  v . VisitLess ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CLessEqNode::CLessEqNode      ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CLessEqNode::Accept           ( CVisitor &        v )
{
  v . VisitLessEq ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CPlusNode::CPlusNode          ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CPlusNode::Accept             ( CVisitor &        v )
{
  v . VisitPlus ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CMinusNode::CMinusNode        ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CMinusNode::Accept            ( CVisitor &        v )
{
  v . VisitMinus ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CMulNode::CMulNode            ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CMulNode::Accept              ( CVisitor &        v )
{
  v . VisitMul ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CDivNode::CDivNode            ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CDivNode::Accept              ( CVisitor &        v )
{
  v . VisitDiv ( *this );
}
//-------------------------------------------------------------------------------------------------
                             COrNode::COrNode              ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         COrNode::Accept               ( CVisitor &        v )
{
  v . VisitOr ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CAndNode::CAndNode            ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CAndNode::Accept              ( CVisitor &        v )
{
  v . VisitAnd ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CModNode::CModNode            ( AExprNode         left,
                                                             AExprNode         right )
  : CBinaryNode ( left, right )
{}

void                         CModNode::Accept              ( CVisitor &        v )
{
  v . VisitMod ( *this );
}
//=================================================================================================
                             CExprStmtNode::CExprStmtNode  ( AExprNode         expr )
  : m_Child ( expr )
{}
AExprNode                    CExprStmtNode::Child          ( void )
{
  return m_Child;
}

void                         CExprStmtNode::Accept         ( CVisitor &        v )
{
  v . VisitExprStmt ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CWrtIntNode::CWrtIntNode      ( AExprNode         expr )
  : m_Child ( expr )
{}
AExprNode                    CWrtIntNode::Child            ( void )
{
  return m_Child;
}
void                         CWrtIntNode::Accept           ( CVisitor &        v )
{
  v . VisitWrtInt ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CWrtStrNode::CWrtStrNode      ( AExprNode         expr )
  : m_Child ( expr )
{}
AExprNode                    CWrtStrNode::Child            ( void )
{
  return m_Child;
}
void                         CWrtStrNode::Accept           ( CVisitor &        v )
{
  v . VisitWrtStr ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CRdlStmtNode::CRdlStmtNode    ( AExprNode         expr )
  : m_Child ( expr )
{}
AExprNode                    CRdlStmtNode::Child           ( void )
{
  return m_Child;
}
void                         CRdlStmtNode::Accept          ( CVisitor &        v )
{
  v . VisitRdlStmt ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CVarNode::CVarNode            ( std::string       name )
  : m_Name ( name )
{}
std::string                & CVarNode::Name                ( void )
{
  return m_Name;
}
void                         CVarNode::Accept              ( CVisitor &        v )
{
  v . VisitVar ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CVarsNode::CVarsNode          ( std::vector<AVarNode> vars )
: m_Variables ( move ( vars ) )
{}
std::vector<AVarNode>      & CVarsNode::Vars               ( void )
{
  return m_Variables;
}
void                         CVarsNode::Accept             ( CVisitor &        v )
{
    v . VisitVars ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CConstNode::CConstNode        ( std::string       name,
                                                             int               val )
  : m_Value ( val ),
    m_Name ( move ( name ) )
{}
int                          CConstNode::Value             ( void )
{
  return m_Value;
}
std::string                & CConstNode::Name              ( void )
{
  return m_Name;
}
void                         CConstNode::Accept            ( CVisitor &        v )
{
  v . VisitConst ( *this );
}
//-------------------------------------------------------------------------------------------------
                             CConstsNode::CConstsNode      ( std::vector<AConstNode> consts )
  : m_Constants ( move ( consts ) )
{}
std::vector<AConstNode>&     CConstsNode::Consts           ( void )
{
  return m_Constants;
}
void                         CConstsNode::Accept           ( CVisitor &        v )
{
  v . VisitConsts ( *this ); 
}
//-------------------------------------------------------------------------------------------------
                             CBlockNode::CBlockNode        ( vector<AStmtNode> stmts )// move!
  : m_Statements ( move ( stmts ) )
{}
std::vector<AStmtNode>     & CBlockNode::Stmts               ( void )
{
  return m_Statements;
}
void                         CBlockNode::Accept            ( CVisitor &        v )
{
  v . VisitBlock ( *this ); 
}
//-------------------------------------------------------------------------------------------------
                             CIfNode::CIfNode              ( AExprNode         condition,
                                                             AStmtNode         thens,
                                                             AStmtNode         elses )
  : m_Cond ( condition ),
    m_Then ( thens ),
    m_Else ( elses )
{}
AExprNode                    CIfNode::Condition            ( void )
{
  return m_Cond;
}
AStmtNode                    CIfNode::Then                 ( void )
{
  return m_Then;
}
AStmtNode                    CIfNode::Else                 ( void )
{
  return m_Else;
}
void                         CIfNode::Accept               ( CVisitor &        v )
{
  v . VisitIf ( *this ); 
}
//-------------------------------------------------------------------------------------------------
                             CWhileNode::CWhileNode        ( AExprNode         condition,
                                                             AStmtNode         body )
  : m_Cond ( condition ),
    m_Body ( body )
{}
AExprNode                    CWhileNode::Condition         ( void )
{
  return m_Cond;
}
AStmtNode                    CWhileNode::Body              ( void )
{
  return m_Body;
}
void                         CWhileNode::Accept            ( CVisitor &        v )
{
  v . VisitWhile ( *this ); 
}
//-------------------------------------------------------------------------------------------------
                             CFunDeclNode::CFunDeclNode    ( EType             type,
                                                             std::string       name,
                                                             AStmtNode         body,
                                                             std::vector<AIdentifierNode> parameters,
                                                             AVarsNode         vars )
  : m_ReturnType ( type ),
    m_Name ( name ),
    m_Body ( body ),
    m_Parameters ( parameters ),
    m_Vars ( vars )
{}
CFunDeclNode::EType          CFunDeclNode::ReturnType      ( void )
{
  return m_ReturnType;
}
std::string                  CFunDeclNode::Name            ( void )
{
  return m_Name;
}
AStmtNode                    CFunDeclNode::Body            ( void )
{
  return m_Body;
}
std::vector<AIdentifierNode> & CFunDeclNode::Parameters    ( void )
{
  return m_Parameters;
}
AVarsNode                    CFunDeclNode::Vars            ( void )
{
  return m_Vars;
}
void                         CFunDeclNode::Accept          ( CVisitor &        v )
{
  v . VisitFunDecl ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CExitNode::Accept             ( CVisitor &        v )
{
  v . VisitExit ( *this ); 
}
//-------------------------------------------------------------------------------------------------
void                         CBreakNode::Accept            ( CVisitor &        v )
{
  v . VisitBreak ( *this ); 
}
//=================================================================================================
}