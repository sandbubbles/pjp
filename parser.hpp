#ifndef PJPPROJECT_PARSER_HPP
#define PJPPROJECT_PARSER_HPP

#include <iostream>
#include <vector>

#include "lexer.hpp"
#include "astnode.hpp"

/*
  Recursive descent parser for mila.
  This is similar to how it is done in Crafting Interpreters. Although there it is written in Java
  and it is a parser for a different language; it is quite similar.
*/
//=================================================================================================
class CParser {
  public:
                             CParser                       ( std::vector<CToken> & tokens );
    static std::vector<AST::ANode>Analyse                  ( std::vector<CToken> tokens );

  private:
    class CParserException : public std::exception
    {
      const char           * what                          ( void ) const noexcept override;
    };
    AST::AStmtNode           n_Declaration                 ( void );
    AST::AStmtNode           n_FunDeclaration              ( void );
    AST::AStmtNode           n_VarDeclaration              ( void );
    AST::AStmtNode           n_ConstDeclaration            ( void );
    AST::AStmtNode           n_Statement                   ( void );
    AST::AStmtNode           n_While                       ( void );
    AST::AStmtNode           n_For                         ( void );
    AST::AStmtNode           n_If                          ( void );
    AST::AStmtNode           n_Block                       ( void );
    AST::AStmtNode           n_Exit                        ( void );
    AST::AStmtNode           n_Break                       ( void );
    AST::AStmtNode           n_WrtInt                      ( void );
    AST::AStmtNode           n_WrtStr                      ( void );
    AST::AStmtNode           n_RdlStmt                     ( void );
    AST::AExprNode           n_Inc                         ( void );
    AST::AExprNode           n_Dec                         ( void );
    AST::AStmtNode           n_ExprStmt                    ( void );
    AST::AExprNode           n_Expression                  ( void );
    AST::AExprNode           n_Assignment                  ( void );
    AST::AExprNode           n_Logic                       ( void );
    AST::AExprNode           n_Equality                    ( void );
    AST::AExprNode           n_Comparison                  ( void );
    AST::AExprNode           n_Term                        ( void );
    AST::AExprNode           n_Factor                      ( void );
    AST::AExprNode           n_Unary                       ( void );
    AST::AExprNode           n_Call                        ( void );
    AST::AExprNode           n_Primary                     ( void );

    //needs to be called with curly brackets as well as normal eg., match({2,3})
    bool                     match                         ( std::initializer_list<CToken::EType> list);
    bool                     match                         ( std::initializer_list<CToken::EType> list,
                                                             std::string       message );
    bool                     match                         ( CToken::EType     type );
    bool                     match                         ( CToken::EType     type,
                                                             std::string       message );
    bool                     check                         ( CToken::EType     type );
    bool                     atEnd                         ( void );
    CToken                   peek                          ( void );
    CToken                   previous                      ( void );
    CToken                   advance                       ( void );
    void                     skipSemicolons                ( void );
    CParserException         error                         ( CToken            token,
                                                             std::string       message );
    void                     synchronize                   ( void );

    std::vector<CToken>    & m_Tokens;
    size_t                   m_TPos    = 0;
};
//=================================================================================================
#endif //PJPPROJECT_PARSER_HPP
