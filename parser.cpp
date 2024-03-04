#include <iostream>

#include "parser.hpp"
#include "misc.hpp"

using namespace std;

typedef CToken::EType        Token;
//=================================================================================================
                             CParser::CParser              ( vector<CToken>  & tokens )
  : m_Tokens ( tokens )
{}
//-------------------------------------------------------------------------------------------------
vector<AST::ANode>           CParser::Analyse              ( vector<CToken>    tokens )
{
  CParser parser ( tokens );

  vector <AST::ANode> nodes;

  parser . match ( Token::PROGRAM, "Expected keyword 'program'" );
  parser . match ( Token::IDENTIFIER, "Expected program identifier" );
  parser . match ( Token::SEMICOLON, "Expected ';'" );
  do
  {
    nodes . push_back ( parser . n_Declaration () );
  } while ( nodes . back () );

  parser . match ( Token::BEGIN, "There must be a main block to execute" );

  AST::AStmtNode block = parser . n_Block ();

  if ( ! block )
    throw parser . error ( parser. peek (), "Must have a block" );

  nodes . back () = make_shared<AST::CFunDeclNode>( AST::CFunDeclNode::EType::INTEGER,
                                                    "main", 
                                                    block,
                                                    vector<AST::AIdentifierNode> (),
                                                    make_shared<AST::CVarsNode> ( vector<AST::AVarNode>( 0 ) ) );

  parser . match ( Token::DOT, "Expected '.'" );
  return nodes;
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_Declaration        ( void )
{
  try 
  {
    AST::AStmtNode node;
    if ( match ( Token::VAR ) )
      node = n_VarDeclaration ();
    else if ( match ( Token::CONST ) )
      node = n_ConstDeclaration ();
    else if ( match ( Token::FUNCTION ) )
      node = n_FunDeclaration ();
    else if ( match ( Token::PROCEDURE ) )
      node = n_FunDeclaration ();
    else 
      node = NULL;
    
    skipSemicolons ();
    return node;
  }
  catch ( CParserException & error )
  {
    synchronize();
    return NULL;
  }
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_FunDeclaration     ( void )
{
  AST::CFunDeclNode::EType type;
  if ( previous () . Type () == Token::PROCEDURE )
    type = AST::CFunDeclNode::EType::VOID;
  if ( previous () . Type () == Token::FUNCTION )
    type = AST::CFunDeclNode::EType::INTEGER;

  match ( Token::IDENTIFIER, "Expected the name of the function declaration" );
  string name = previous () . String ();

  match ( Token::LEFT_PAREN , "Expected left parenthesis" );

  vector<AST::AIdentifierNode> parameters;
  if ( ! match ( Token::RIGHT_PAREN ) )
  {
    do 
    {
      match ( Token::IDENTIFIER, "Expected parameters identifier" );
      parameters . push_back ( 
                       make_shared<AST::CIdentifierNode>( previous () . String ()  ) );
      match ( Token::DOUBLE_DOT, "Expected ':' in parameters" );
      match ( Token::INTEGER_KW, "Supports integer parameters only" );
    }
    while ( match ( Token::SEMICOLON ) );
    match ( Token::RIGHT_PAREN , "Expected right parenthesis" );
  }
  if ( type == AST::CFunDeclNode::EType::INTEGER )
  {
    match ( Token::DOUBLE_DOT, "Expected ':'" );
    match ( Token::INTEGER_KW, "Only supports integers as of now" );
  }
  match ( Token::SEMICOLON, "Expected ';'" );

  AST::AStmtNode body = NULL;
  AST::AVarsNode vars = make_shared<AST::CVarsNode> ( vector<AST::AVarNode>( ) );

  if ( match ( Token::FORWARD ) )
    match ( Token::SEMICOLON, "Expected ';' after forward keyword " );
  else
  {
    if ( match ( Token::VAR ) )
      vars = dynamic_pointer_cast<AST::CVarsNode>( n_VarDeclaration () );

    match ( Token::BEGIN, "Expected a block" );
    body = n_Block ();
  }

  return make_shared<AST::CFunDeclNode>( type, name, body, parameters, vars );
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_VarDeclaration     ( void )
{
  //varDecl → "var"  ( identifiers ":" data_type ";" )* ;
  //node with vector with vars inside
  vector<AST::AVarNode> nodes;
  while ( match ( Token::IDENTIFIER ) )
  {
    string name = previous () . String ();
    nodes . push_back ( make_shared<AST::CVarNode> ( name ) );
    while ( match ( Token::COMMA ) )
    {
      match ( Token::IDENTIFIER, "Expected identifier" );
      name = previous () . String ();
      nodes . push_back ( make_shared<AST::CVarNode> ( name ) );
    }
    match ( Token::DOUBLE_DOT, "Expected ':' before type definition" );
    match ( Token::INTEGER_KW, "Variable type must be an integer" );
    match ( Token::SEMICOLON, "Expected ';' at the end of a var definition" );
  }
  return make_shared<AST::CVarsNode> (nodes);
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_ConstDeclaration   ( void )
{
  //constDef → "const" ( IDENTIFIER "=" NUMBER ";" )* ;
  //node with vector with consts inside
  vector<AST::AConstNode> nodes;
  while ( match ( Token::IDENTIFIER ) )
  {
    string name = previous () . String ();
    match ( Token::EQUAL, "Expected '=' in a const definition" );
    match ( Token::NUMBER, "Expected a number in a const definition" ); // with time either int or string
    int val = previous () . Int ();
    match ( Token::SEMICOLON, "Expected ';' at the end of a const definition" );

    nodes . push_back ( make_shared<AST::CConstNode> ( name, val ) );
  }
  return make_shared<AST::CConstsNode> (nodes);
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_Statement          ( void )
{
  skipSemicolons();
  if ( match ( Token::WHILE ) )
    return n_While ();
  if ( match ( Token::FOR ) )
    return n_For ();
  if ( match ( Token::IF ) )
    return n_If ();
  if ( match ( Token::WRITELN ) )
    return n_WrtInt ();
  if ( match ( Token::WRITE ) )
    return n_WrtStr ();
  if ( match ( Token::READLN ) )
    return n_RdlStmt ();
  if ( match ( Token::BEGIN ) )
    return n_Block ();
  if ( match ( Token::EXIT ) )
    return n_Exit ();
  if ( match ( Token::BREAK ) )
    return n_Break ();
  return n_ExprStmt ();
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_While              ( void )
{
  AST::AExprNode cond = n_Expression ();
  match ( Token::DO, "Expected 'do' keyword after condition" );
  AST::AStmtNode body = n_Statement ();
  return make_shared<AST::CWhileNode> ( cond, body );
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_For                ( void )
{
  match ( Token::IDENTIFIER, "Expected variable to modify" );
  AST::AIdentifierNode var = make_shared<AST::CIdentifierNode>( previous () . String () );  // the variable which will be modified

  match ( Token::ASSIGN, "Variable must be assigned" );
  AST::AExprNode set = n_Expression (); // the value the variable is initially set to

  match ( {Token::TO, Token::DONWTO}, "Expected 'to' or 'downto'" );
  CToken direction = previous ();  // token which will determined whether inc or dec on var

  AST::AExprNode to = n_Expression (); // the right side to which the varibale will be compared to

  match ( Token::DO, "Expected 'do' at the end of for header" );
  AST::AStmtNode body = n_Statement (); // body of the for

  AST::ANotEqNode cond = make_shared<AST::CNotEqNode> ( var, to ); // the condition evalueted every single round
  
  vector<AST::AStmtNode> stmts;
  stmts . push_back ( body );

  AST::AExprNode end;
  if ( direction . Type () == Token::TO )
    end = make_shared<AST::CAssignNode> ( var -> Name (), 
                                         make_shared<AST::CPlusNode> ( var,
                                                                        make_shared<AST::CIntNode> ( 1 ) ) );
  else
    end = make_shared<AST::CAssignNode> ( var -> Name (), 
                                         make_shared<AST::CMinusNode> ( var,
                                                                        make_shared<AST::CIntNode> ( 1 ) ) );
  stmts . push_back ( make_shared<AST::CExprStmtNode>( end ) );

  body = make_shared<AST::CBlockNode> ( stmts );
  // Creating while node with the inc or dec as the last instruction
  AST::AWhileNode whileNode = make_shared<AST::CWhileNode> ( cond, body );
  
  stmts . clear ();
  AST::AExprStmtNode assign = make_shared<AST::CExprStmtNode> (
                                       make_shared<AST::CAssignNode> ( var -> Name (), set ));
  stmts . push_back ( static_pointer_cast<AST::CStmtNode> ( assign ));
  stmts . push_back ( whileNode );

  // Creating block with asignment at first and then the while segment
  return make_shared<AST::CBlockNode> ( stmts ); 
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_If                 ( void )
{
  AST::AExprNode cond = n_Expression ();
  match ( Token::THEN, "Expected 'then' keyword after condition" );
  AST::AStmtNode thens = n_Statement ();
  AST::AStmtNode elses = NULL;
  if ( match ( Token::ELSE ) )
    elses = n_Statement ();
  
  return make_shared<AST::CIfNode> ( cond, thens, elses );
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_Block              ( void )
{
  vector<AST::AStmtNode> stmts;
  while ( ! match ( Token::END ) )
  {
    stmts . push_back ( n_Statement () );
    skipSemicolons();
  }
  return make_shared<AST::CBlockNode> (stmts);
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_Exit               ( void )
{
  return make_shared<AST::CExitNode> ();
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_Break               ( void )
{
  return make_shared<AST::CBreakNode> ();
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_WrtInt             ( void )
{
  AST::AStmtNode stmt;
  match ( Token::LEFT_PAREN, "Expected left parenthesis" );
  AST::AExprNode expr = n_Expression ();
  stmt = make_shared<AST::CWrtIntNode> ( expr );
  match ( Token::RIGHT_PAREN, "Expected right parenthesis" );
  
  return stmt;
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_WrtStr             ( void )
{
  AST::AStmtNode stmt;
  match ( Token::LEFT_PAREN, "Expected left parenthesis" );
  AST::AExprNode expr = n_Expression ();
  stmt = make_shared<AST::CWrtStrNode> ( expr );
  match ( Token::RIGHT_PAREN, "Expected right parenthesis" );
  
  return stmt;
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_RdlStmt            ( void )
{
  match ( Token::LEFT_PAREN, "Expected left parenthesis" );
  AST::AExprNode expr = n_Expression ();
  match ( Token::RIGHT_PAREN, "Expected right parenthesis" );
  return make_shared<AST::CRdlStmtNode> ( expr );
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Inc                ( void )
{
  match ( Token::LEFT_PAREN, "Expected left parenthesis" );
  match ( Token::IDENTIFIER, "Expected identifier" );
  AST::AIdentifierNode idt = make_shared<AST::CIdentifierNode> ( previous () . String () );
  match ( Token::RIGHT_PAREN, "Expected right parenthesis" );
  return make_shared<AST::CAssignNode> ( idt -> Name (), 
                                         make_shared<AST::CPlusNode> ( idt,
                                                                        make_shared<AST::CIntNode> ( 1 ) ) );
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Dec                ( void )
{
  match ( Token::LEFT_PAREN, "Expected left parenthesis" );
  match ( Token::IDENTIFIER, "Expected identifier" );
  AST::AIdentifierNode idt = make_shared<AST::CIdentifierNode> ( previous () . String () );
  match ( Token::RIGHT_PAREN, "Expected right parenthesis" );
  return make_shared<AST::CAssignNode> ( idt -> Name (), 
                                         make_shared<AST::CMinusNode> ( idt,
                                                                        make_shared<AST::CIntNode> ( 1 ) ) );
}
//-------------------------------------------------------------------------------------------------
AST::AStmtNode               CParser::n_ExprStmt           ( void )
{
  AST::AExprNode expr = n_Expression ();
  return make_shared<AST::CExprStmtNode> ( expr );
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Expression         ( void )
{
  return n_Assignment ();
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Assignment         ( void )
{
  AST::AExprNode expr = n_Logic ();
  
  if ( match ( Token::ASSIGN ) )
    expr = make_shared<AST::CAssignNode>
                      ((dynamic_pointer_cast<AST::CIdentifierNode> (expr) ) -> Name(),
                      n_Assignment () );
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Logic              ( void )
{
  AST::AExprNode expr = n_Equality ();
  while ( match ( {Token::AND, Token::OR} ) )
  {
    if ( previous () . Type () == Token::AND )
      expr = make_shared<AST::CAndNode>( expr, n_Equality () );
    else if ( previous () . Type () == Token::OR )
      expr = make_shared<AST::COrNode>( expr, n_Equality () );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Equality           ( void )
{
  AST::AExprNode expr = n_Comparison ();

  while ( match ( {Token::EQUAL, Token::NOT_EQUAL} ) )
  {
    if ( previous () . Type () == Token::EQUAL )
      expr = make_shared<AST::CEqualNode>( expr, n_Comparison () );
    else if ( previous () . Type () == Token::NOT_EQUAL )
      expr = make_shared<AST::CNotEqNode>( expr, n_Comparison () );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Comparison         ( void )
{
  AST::AExprNode expr = n_Term ();

  while ( match ( {Token::GREATER, Token::GREATER_EQUAL,
                   Token::LESS, Token::LESS_EQUAL} ) )
  {
    if ( previous () . Type () == Token::GREATER )
      expr = make_shared<AST::CGreatNode>( expr, n_Term () );
    else if ( previous () . Type () == Token::GREATER_EQUAL )
      expr = make_shared<AST::CGreatEqNode>( expr, n_Term () );
    else if ( previous () . Type () == Token::LESS )
      expr = make_shared<AST::CLessNode>( expr, n_Term () );
    else if ( previous () . Type () == Token::LESS_EQUAL )
      expr = make_shared<AST::CLessEqNode>( expr, n_Term () );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Term               ( void )
{
  AST::AExprNode expr = n_Factor ();

  while ( match ( {Token::PLUS, Token::MINUS} ) )
  {
    if ( previous () . Type () == Token::PLUS )
      expr = make_shared<AST::CPlusNode>( expr, n_Factor () );
    else if ( previous () . Type () == Token::MINUS )
      expr = make_shared<AST::CMinusNode>( expr, n_Factor () );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Factor             ( void )
{
  AST::AExprNode expr = n_Unary ();

  while ( match ( {Token::MULTIPLY, Token::DIV,
                   Token::MOD} ) )
  {
    if ( previous () . Type () == Token::MULTIPLY )
      expr = make_shared<AST::CMulNode>( expr, n_Unary () );
    else if ( previous () . Type () == Token::DIV )
      expr = make_shared<AST::CDivNode>( expr, n_Unary () );
    else if ( previous () . Type () == Token::MOD )
      expr = make_shared<AST::CModNode>( expr, n_Unary () );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Unary              ( void )
{
  if ( match ({Token::NOT, Token::PLUS, Token::MINUS}) )
  {
    if ( previous () . Type () == Token::NOT )
      return make_shared<AST::CNotNode>( n_Primary () );
    else if ( previous () . Type () == Token::PLUS )
      return make_shared<AST::CPosNode>( n_Primary () );
    else if ( previous () . Type () == Token::MINUS )
      return make_shared<AST::CNegNode>( n_Primary () );
  }

  return n_Call ();
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Call               ( void )
{
  AST::AExprNode expr = n_Primary ();

  CToken token = previous ();
  if ( match ( Token::LEFT_PAREN ) )
  {
    if ( token . Type () != Token::IDENTIFIER )
      throw error ( previous (), "Expected Identifier in function call" );
    std::vector<AST::AExprNode> args;
    if ( ! match ( Token::RIGHT_PAREN ) )
    {
      do 
        args . push_back ( n_Expression () );
      while ( match ( Token::COMMA ) );
      match ( Token::RIGHT_PAREN, "Expected ')' in function call" );
    }
    expr = make_shared<AST::CCallNode>( token . String (), args );
  }
  return expr;
}
//-------------------------------------------------------------------------------------------------
AST::AExprNode               CParser::n_Primary            ( void )
{
  if ( match ( Token::NUMBER ) )        // with time possible also string, variables etc.
      return make_shared<AST::CIntNode>( previous () . Int () );
  AST::AExprNode node;
  if ( match ( Token:: LEFT_PAREN ) )
  {
    node = make_shared <AST::CGroupNode>( n_Expression () );
    match ( Token::RIGHT_PAREN, "Expects right parenthesis" );
    return node;
  }
  if ( match ( Token:: IDENTIFIER ) )
    return make_shared <AST::CIdentifierNode>( previous () . String () );

  if ( match ( Token::STRING ) )
    return make_shared <AST::CStringNode> ( previous () . String () );
  
  if ( match ( Token::INC ) )
    return n_Inc ();
  if ( match ( Token::DEC ) )
    return n_Dec ();

  throw error ( peek (), "Not a primary" );
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::match                ( std::initializer_list<Token> list)
{
  for ( CToken::EType type : list )
    if ( match ( type ) )
      return true;

  return false;
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::match                ( std::initializer_list<Token> list,
                                                             std::string       message )
{
  if ( match ( list ) )
    return true;
  throw error ( peek(), message );
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::match                ( Token             type )
{
  if ( check ( type ) )
  {
    advance ();
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::match                ( Token             type,
                                                             std::string       message )
{
  if ( match ( type ) )
    return true;
  throw error ( peek(), message );
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::check                ( Token             type )
{
  if ( atEnd () )
    return false;
  return peek () . Type () == type; 
}
//-------------------------------------------------------------------------------------------------
bool                         CParser::atEnd                ( void )
{
  return ( peek() . Type () == Token::EOFF )
          || ( m_Tokens . size () == m_TPos + 1 );
}
//-------------------------------------------------------------------------------------------------
CToken                       CParser::peek                 ( void )
{
  return m_Tokens[m_TPos];
}
//-------------------------------------------------------------------------------------------------
CToken                       CParser::previous             ( void )
{
  return m_Tokens[m_TPos - 1];
}
//-------------------------------------------------------------------------------------------------
CToken                       CParser::advance              ( void )
{
  if ( !atEnd() 
       && m_Tokens . size () > m_TPos )
       ++ m_TPos;
  return previous ();
}
//-------------------------------------------------------------------------------------------------
void                         CParser::skipSemicolons       ( void )
{
  while ( peek () . Type () == Token::SEMICOLON )
    advance ();
}
//-------------------------------------------------------------------------------------------------
CParser::CParserException    CParser::error                ( CToken            token,
                                                             std::string       message )
{
  cout << token << endl;
  ::error ( message, token . Line(), token . Offset () );
  return CParserException ();
}
//-------------------------------------------------------------------------------------------------
void                         CParser::synchronize          ( void )
{
  while ( ! atEnd () )
  {
    switch ( peek () . Type () )
    {
      case Token::SEMICOLON:
        advance ();
      case Token::IF:
      case Token::FOR:
      case Token::WHILE:
      case Token::BEGIN:
      case Token::WRITELN:
      case Token::READLN:
      case Token::VAR:
      case Token::CONST:
      case Token::INC:
      case Token::DEC:
      case Token::EXIT:
        return;
      default:
        advance ();
    } 
  }
}
//-------------------------------------------------------------------------------------------------
const char                 * CParser::CParserException::what ( void ) const noexcept
{
  return "parser exception";
}
//=================================================================================================