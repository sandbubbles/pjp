#ifndef PJPPROJECT_NODE_HPP
#define PJPPROJECT_NODE_HPP
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "visitor.hpp"

namespace AST {
/* Here are all the AST nodes needed to represent mila code.
   The top level node is CNode. Two main classes CExprNode and CStmtNode inherit from that. All the
   other nodes are divided into these two main "groups".
   Code is a sequence of statements. Statements do not have "return value". Expressions do.
*/
//=================================================================================================
class CNode
{
  public:
    virtual                 ~CNode                         ( void )            = default;
    virtual void             Accept                        ( CVisitor &        v ) = 0;
};
typedef std::shared_ptr<CNode>         ANode;
//not unique because working with shared_ptr is less of a hustle (mm wisdom)
//=================================================================================================
class CExprNode : public CNode
{
  public:
    virtual                 ~CExprNode                     ( void )            = default;
};
typedef std::shared_ptr<CExprNode>     AExprNode;
//=================================================================================================
class CGroupNode : public CExprNode
{
  public:
                             CGroupNode                    ( AExprNode         op );       
    virtual                 ~CGroupNode                    ( void )            = default;
    AExprNode                Child                         ( void );
    virtual void             Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;
};
typedef std::shared_ptr<CGroupNode>    AGroupNode;
//=================================================================================================
class CStringNode : public CExprNode
{
  public:
                             CStringNode                   ( std::string       value );       
    virtual                 ~CStringNode                   ( void )            = default;
    std::string              Value                         ( void );
    virtual void             Accept                        ( CVisitor &        v ) override;
  private:
    std::string              m_Value;
};
typedef std::shared_ptr<CIntNode>      AStringNode;
//=================================================================================================
class CIntNode : public CExprNode
{
  public:
                             CIntNode                      ( int               value );       
    virtual                 ~CIntNode                      ( void )            = default;
    int                      Value                         ( void );
    virtual void             Accept                        ( CVisitor &        v ) override;
  private:
    int                      m_Value;
};
typedef std::shared_ptr<CIntNode>      AIntNode;
//-------------------------------------------------------------------------------------------------
/* This node is meant for accessing vars and consts. CVar and CConst are meant 
   for declaring the variables, this node doesn't do that.
*/
class CIdentifierNode : public CExprNode
{
  public:
                             CIdentifierNode               ( std::string       name );
                            ~CIdentifierNode               ( void )            = default;
    std::string            & Name                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::string              m_Name;

};
typedef std::shared_ptr<CIdentifierNode> AIdentifierNode;
//-------------------------------------------------------------------------------------------------
class CCallNode : public CExprNode
{
  public:
                             CCallNode                     ( std::string       callee,
                                                             std::vector<AExprNode> args );
                            ~CCallNode                     ( void )            = default;
    std::string            & Callee                        ( void );
    std::vector<AExprNode> & Arguments                     ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::string              m_Callee;
    std::vector<AExprNode>   m_Args;
};
typedef std::shared_ptr<CCallNode>     ACallNode;
//=================================================================================================
class CAssignNode : public CExprNode
{
  public:
                             CAssignNode                   ( std::string       name,
                                                             AExprNode         value );
                            ~CAssignNode                   ( void )            = default;
    std::string            & Name                          ( void );
    AExprNode                Value                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::string              m_Name;
    AExprNode                m_Value;

};
typedef std::shared_ptr<CAssignNode>   AAssignNode;
//=================================================================================================
class CUnaryNode : public CExprNode
{
  public:
                             CUnaryNode                    ( AExprNode         op );       
    virtual                 ~CUnaryNode                    ( void )            = default;
    AExprNode                Child                         ( void );
    virtual void             Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;
};
typedef std::shared_ptr<CUnaryNode>    AUnaryNode;
//-------------------------------------------------------------------------------------------------
class CNotNode : public CUnaryNode
{
  public:
                             CNotNode                      ( AExprNode         op );       
                            ~CNotNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CNotNode>      ANotNode;
//-------------------------------------------------------------------------------------------------
class CPosNode : public CUnaryNode
{
  public:
                             CPosNode                      ( AExprNode         op );       
                            ~CPosNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CPosNode>      APosNode;
//-------------------------------------------------------------------------------------------------
class CNegNode : public CUnaryNode
{
  public:
                             CNegNode                      ( AExprNode         op );       
                            ~CNegNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CNegNode>      ANegNode;
//=================================================================================================
class CBinaryNode : public CExprNode
{
  public:
                             CBinaryNode                   ( AExprNode         left,
                                                             AExprNode         right );       
    virtual                 ~CBinaryNode                   ( void )            = default;
    AExprNode                LeftChild                     ( void );
    AExprNode                RightChild                    ( void );
    virtual void             Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Left;
    AExprNode                m_Right;
};
typedef std::shared_ptr<CBinaryNode>   ABinaryNode;
//-------------------------------------------------------------------------------------------------
class CEqualNode : public CBinaryNode
{
  public:
                             CEqualNode                    ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CEqualNode                    ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CEqualNode>    AEqualNode;
//-------------------------------------------------------------------------------------------------
class CNotEqNode : public CBinaryNode
{
  public:
                             CNotEqNode                    ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CNotEqNode                    ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CNotEqNode>    ANotEqNode;
//-------------------------------------------------------------------------------------------------
class CGreatNode : public CBinaryNode
{
  public:
                             CGreatNode                    ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CGreatNode                    ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CGreatNode>    AGreatNode;
//-------------------------------------------------------------------------------------------------
class CGreatEqNode : public CBinaryNode
{
  public:
                             CGreatEqNode                  ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CGreatEqNode                  ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CGreatEqNode>  AGreatEqNode;
//-------------------------------------------------------------------------------------------------
class CLessNode : public CBinaryNode
{
  public:
                             CLessNode                     ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CLessNode                     ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CLessNode>     ALessNode;
//-------------------------------------------------------------------------------------------------
class CLessEqNode : public CBinaryNode
{
  public:
                             CLessEqNode                   ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CLessEqNode                   ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CLessEqNode>   ALessEqNode;
//-------------------------------------------------------------------------------------------------
class CPlusNode : public CBinaryNode
{
  public:
                             CPlusNode                     ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CPlusNode                     ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CPlusNode>     APlusNode;
//-------------------------------------------------------------------------------------------------
class CMinusNode : public CBinaryNode
{
  public:
                             CMinusNode                    ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CMinusNode                    ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CMinusNode>    AMinusNode;
//-------------------------------------------------------------------------------------------------
class CMulNode : public CBinaryNode
{
  public:
                             CMulNode                      ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CMulNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CMulNode>      AMulNode;
//-------------------------------------------------------------------------------------------------
class CDivNode : public CBinaryNode
{
  public:
                             CDivNode                      ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CDivNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CDivNode>      ADivNode;
//-------------------------------------------------------------------------------------------------
class COrNode : public CBinaryNode
{
  public:
                             COrNode                       ( AExprNode         left,
                                                             AExprNode         right );       
                            ~COrNode                       ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<COrNode>       AOrNode;
//-------------------------------------------------------------------------------------------------
class CAndNode : public CBinaryNode
{
  public:
                             CAndNode                      ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CAndNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CAndNode>      AAndNode;
//-------------------------------------------------------------------------------------------------
class CModNode : public CBinaryNode
{
  public:
                             CModNode                      ( AExprNode         left,
                                                             AExprNode         right );       
                            ~CModNode                      ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
};
typedef std::shared_ptr<CModNode>      AModNode;
//=================================================================================================
class CStmtNode : public CNode
{
  public:
    virtual                 ~CStmtNode                     ( void )            = default;
};
typedef std::shared_ptr<CStmtNode>     AStmtNode;
//-------------------------------------------------------------------------------------------------
/* Expression statement is a way to wrap expressions into statements.
*/
class CExprStmtNode : public CStmtNode
{
  public:
                             CExprStmtNode                 ( AExprNode         expr );
                            ~CExprStmtNode                 ( void )            = default;
    AExprNode                Child                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;

};
typedef std::shared_ptr<CExprStmtNode> AExprStmtNode;
//-------------------------------------------------------------------------------------------------
/* Write and read functionality is usually accessed through std library. Here it is treated like a
   statement. However, in codegen it has to be a propper function call.
*/
class CWrtIntNode : public CStmtNode
{
  public:
                             CWrtIntNode                   ( AExprNode         expr );
                            ~CWrtIntNode                   ( void )            = default;
    AExprNode                Child                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;

};
typedef std::shared_ptr<CWrtIntNode>   AWrtIntNode;
//-------------------------------------------------------------------------------------------------
class CWrtStrNode : public CStmtNode     // Print statement (not the function)
{
  public:
                             CWrtStrNode                   ( AExprNode         expr );
                            ~CWrtStrNode                   ( void )            = default;
    AExprNode                Child                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;

};
typedef std::shared_ptr<CWrtStrNode>   AWrtStrNode;
//-------------------------------------------------------------------------------------------------
class CRdlStmtNode : public CStmtNode     // Read statement (not the function)
{
  public:
                             CRdlStmtNode                  ( AExprNode         expr );
                            ~CRdlStmtNode                  ( void )            = default;
    AExprNode                Child                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Child;

};
typedef std::shared_ptr<CRdlStmtNode>  ARdlStmtNode;
//-------------------------------------------------------------------------------------------------
class CVarNode : public CStmtNode
{
  public:
                             CVarNode                      ( std::string       name );
                            ~CVarNode                      ( void )            = default;
    std::string            & Name                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::string              m_Name;

};
typedef std::shared_ptr<CVarNode>      AVarNode;
//-------------------------------------------------------------------------------------------------
class CVarsNode : public CStmtNode
{
  public:
                             CVarsNode                     ( std::vector<AVarNode> vars );
                            ~CVarsNode                     ( void )            = default;
    std::vector<AVarNode>  & Vars                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::vector<AVarNode>    m_Variables;

};
typedef std::shared_ptr<CVarsNode>     AVarsNode;
//-------------------------------------------------------------------------------------------------
class CConstNode : public CStmtNode
{
  public:
                             CConstNode                    ( std::string       name,
                                                             int               val );
                            ~CConstNode                    ( void )            = default;
    int                      Value                         ( void );
    std::string            & Name                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    int                      m_Value;
    std::string              m_Name;

};
typedef std::shared_ptr<CConstNode>    AConstNode;
//-------------------------------------------------------------------------------------------------
class CConstsNode : public CStmtNode
{
  public:
                             CConstsNode                   ( std::vector<AConstNode> consts );
                            ~CConstsNode                   ( void )            = default;
    std::vector<AConstNode>& Consts                        ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::vector<AConstNode>  m_Constants;

};
typedef std::shared_ptr<CConstsNode>   AConstsNode;
//-------------------------------------------------------------------------------------------------
class CBlockNode : public CStmtNode
{
  public:
                             CBlockNode                    ( std::vector<AStmtNode>  stmts );// move!
                            ~CBlockNode                    ( void )            = default;
    std::vector<AStmtNode> & Stmts                         ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    std::vector<AStmtNode>   m_Statements;

};
typedef std::shared_ptr<CBlockNode>    ABlockNode;
//-------------------------------------------------------------------------------------------------
class CIfNode : public CStmtNode
{
  public:
                             CIfNode                       ( AExprNode         condition,
                                                             AStmtNode         thens,
                                                             AStmtNode         elses );
                            ~CIfNode                       ( void )            = default;
    AExprNode                Condition                     ( void );
    AStmtNode                Then                          ( void );
    AStmtNode                Else                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Cond;
    AStmtNode                m_Then;
    AStmtNode                m_Else;
};
typedef std::shared_ptr<CIfNode>       AIfNode;
//-------------------------------------------------------------------------------------------------
class CWhileNode : public CStmtNode
{
  public:
                             CWhileNode                    ( AExprNode         condition,
                                                             AStmtNode         body );
                            ~CWhileNode                    ( void )            = default;
    AExprNode                Condition                     ( void );
    AStmtNode                Body                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    AExprNode                m_Cond;
    AStmtNode                m_Body;
};
typedef std::shared_ptr<CWhileNode>    AWhileNode;
//-------------------------------------------------------------------------------------------------
class CFunDeclNode : public CStmtNode
{
  public:
    enum class EType : uint8_t
    {
      VOID = 0,
      INTEGER,
      TOTAL
    };
                             CFunDeclNode                  ( EType             type,
                                                             std::string       name,
                                                             AStmtNode         body,
                                                             std::vector<AIdentifierNode> parameters,
                                                             AVarsNode         vars );
                            ~CFunDeclNode                  ( void )            = default;
    EType                    ReturnType                    ( void );
    std::string              Name                          ( void );
    AStmtNode                Body                          ( void );
    std::vector<AIdentifierNode> & Parameters              ( void );
    AVarsNode                Vars                          ( void );
    void                     Accept                        ( CVisitor &        v ) override;
  private:
    EType                    m_ReturnType;
    std::string              m_Name;
    AStmtNode                m_Body;
    std::vector<AIdentifierNode> m_Parameters;
    AVarsNode                m_Vars;

};
typedef std::shared_ptr<CFunDeclNode>  AFunDeclNode;
//-------------------------------------------------------------------------------------------------
class CExitNode : public CStmtNode
{
  public:
                             CExitNode                     ( void )            = default;
                            ~CExitNode                     ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
  private:
};
typedef std::shared_ptr<CExitNode>     AExitNode;
//-------------------------------------------------------------------------------------------------
class CBreakNode : public CStmtNode
{
  public:
                             CBreakNode                    ( void )            = default;
                            ~CBreakNode                    ( void )            = default;
    void                     Accept                        ( CVisitor &        v ) override;
  private:
};
typedef std::shared_ptr<CBreakNode>    ABreakNode;
}
//=================================================================================================
#endif //PJPPROJECT_NODE_HPP