#include <ctype.h>

#include "lexer.hpp"

using namespace std;
//=================================================================================================
                             CIstream::CIstream            ( std::istream    & sin )
  : m_Sin ( sin )
{}
//-------------------------------------------------------------------------------------------------
uint                         CIstream::lineNumber          ( void ) const
{
  return m_LineNumber;
}
//-------------------------------------------------------------------------------------------------
uint                         CIstream::offset              ( void ) const
{
  return m_Offset;
}
//-------------------------------------------------------------------------------------------------
bool                         CIstream::eof                 ( void )
{
  return m_Sin . eof ();
}
//-------------------------------------------------------------------------------------------------
int                          CIstream::get                 ( void )            
{
  int i = m_Sin . get ();

  if ( i == '\n')
  {
    ++m_LineNumber;
    m_Offset = 1;
  }
  else
    ++m_Offset;

  return i;
}
//-------------------------------------------------------------------------------------------------
int                          CIstream::peek                ( void )
{
  return m_Sin . peek ();
}
//=================================================================================================
                             CLexer::CLexer                ( istream         & sin )
  : m_Sin ( CIstream ( sin ) )
{}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::NextToken             ( void )
{

  skipCommentsBlanks();

  if ( m_Sin . eof () )
    return addToken( CToken::EType::EOFF );

  return nextToken () ;
}
//-------------------------------------------------------------------------------------------------
vector<CToken>               CLexer::Analyse               ( istream         & sin )
{
  vector<CToken> tokens;
  CLexer lexan ( sin );

  do
  {
    tokens . push_back ( lexan . NextToken () );
  }
  while ( tokens . back () . Type () != CToken::EType::EOFF );

  return tokens;
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::nextToken             ( void )
{
  /* TODO: now every operator token will take very long time to scan.
           ifs before calling previous methods helps a bit with speed because, but it is not as
           readable and mainly, if the content of the methods changed, the ifs must too.
           Fikovnik said that readability goes before performance, but hm, i'm not sure here, it 
           seems excessive to go through every method.
  */
  CToken token;

  token = scanName ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanString ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanNumber ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanPunctuation ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanOperator ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;
  
  error ( "Undefined token", token . Line (), token . Offset () );
  return token;
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanPunctuation       ( void )
{
  /* Scans also assign := sign due to collision with double dot
     it also reads equal = becuase equal can be comparison or as an assignment to const. 
     go figure.
     This methods reads punctuation and bunch of stuff i dont know where else to fit i guess.
  */
  switch ( m_Sin . peek () )
  {
    case '(':
      m_Sin . get ();
      return addToken ( CToken::EType::LEFT_PAREN );
    case ')':
      m_Sin . get ();
      return addToken ( CToken::EType::RIGHT_PAREN );
    case ',':
      m_Sin . get ();
      return addToken ( CToken::EType::COMMA );
    case '.':
      m_Sin . get ();
      return addToken ( CToken::EType::DOT );
    case ';':
      m_Sin . get ();
      return addToken ( CToken::EType::SEMICOLON );
    case ':':
      m_Sin . get ();
      return match ('=') ? addToken ( CToken::EType::ASSIGN ) : addToken ( CToken::EType::DOUBLE_DOT );
    case '=':                                              
      m_Sin . get ();                                      
      return addToken ( CToken::EType::EQUAL );
    default:
      return addToken ( CToken::EType::ERROR );
  }
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanOperator          ( void )
{
  CToken token;

  token = scanRelationOp ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanArithmeticOp ();
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  return token;
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanNumber            ( void )
{
  int number = -1;

  number = getHexaNumber ();
  if ( number != -1 )
    return addToken ( CToken::EType::NUMBER, number );

  number = getOctaNumber ();
  if ( number != -1 )
    return addToken ( CToken::EType::NUMBER, number );

  number = getDecaNumber ();
  if ( number != -1 )
    return addToken ( CToken::EType::NUMBER, number );

  return addToken ( CToken::EType::ERROR );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanString            ( void )
{
  if ( m_Sin . peek () != '\'')
    return addToken ( CToken::EType::ERROR );
  m_Sin . get ();                      // deletes first " 

  string str = "";
  while ( m_Sin . peek () != '\'' )
  {
    if ( m_Sin . peek () == EOF )
      return addToken ( CToken::EType::ERROR );
    str += m_Sin . get ();
  }
  m_Sin . get ();                     // deletes second " 

  return addToken ( CToken::EType::STRING, str );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanName              ( void )
{
  string name = getName();
  if ( name == "" )
    return addToken ( CToken::EType::ERROR );

  CToken token;

  token = scanOperatorKeyword ( name );
  if ( token . Type () != CToken::EType::ERROR )
    return token;
  
  token = scanContFlowKeyword ( name );
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanBuiltinKeyword ( name );
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  token = scanBasicKeyword ( name );
  if ( token . Type () != CToken::EType::ERROR )
    return token;

  return addToken ( CToken::EType::IDENTIFIER, name );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanRelationOp        ( void )
{
  switch ( m_Sin . peek () )
  {
    case '<':
      m_Sin . get ();
      if ( match( '>' ) )
        return addToken ( CToken::EType::NOT_EQUAL );

      if ( match ('=') )
        return addToken ( CToken::EType::LESS_EQUAL );
        
      return addToken ( CToken::EType::LESS );

    case '>':
      m_Sin . get ();
      return match ('=') ? addToken ( CToken::EType::GREATER_EQUAL ) : addToken ( CToken::EType::GREATER );
    
    default:
     return addToken ( CToken::EType::ERROR );
  }
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanArithmeticOp      ( void )
{
  switch ( m_Sin . peek () )
  {
    case '+':
      m_Sin . get ();
      return addToken ( CToken::EType::PLUS );
    case '-':
      m_Sin . get ();
      return addToken ( CToken::EType::MINUS );
    case '*':
      m_Sin . get ();
      return addToken ( CToken::EType::MULTIPLY );
    default:
     return addToken ( CToken::EType::ERROR );
  }
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanOperatorKeyword   ( std::string     & name )
{
  if ( name == "mod" ) 
    return addToken ( CToken::EType::MOD );
  else if ( name == "div" )
    return addToken ( CToken::EType::DIV );
  else if ( name == "not" )
    return addToken ( CToken::EType::NOT );
  else if ( name == "or" )
    return addToken ( CToken::EType::OR );
  else if ( name == "and" )
    return addToken ( CToken::EType::AND );

  return addToken ( CToken::EType::ERROR );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanContFlowKeyword   ( std::string     & name )
{
  if ( name == "if" ) 
    return addToken ( CToken::EType::IF );
  else if ( name == "then" )
    return addToken ( CToken::EType::THEN );
  else if ( name == "else" )
    return addToken ( CToken::EType::ELSE );
  else if ( name == "for" )
    return addToken ( CToken::EType::FOR );
  else if ( name == "to" )
    return addToken ( CToken::EType::TO );
  else if ( name == "downto" )
    return addToken ( CToken::EType::DONWTO );
  else if ( name == "while" )
    return addToken ( CToken::EType::WHILE );
  else if ( name == "do" )
    return addToken ( CToken::EType::DO );
  else if ( name == "exit" )
    return addToken ( CToken::EType::EXIT );
  else if ( name == "break" )
    return addToken ( CToken::EType::BREAK );
    
  return addToken ( CToken::EType::ERROR );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanBuiltinKeyword    ( std::string     & name )
{
  if ( name == "inc" )
    return addToken ( CToken::EType::INC );
  else if ( name == "dec" )
    return addToken ( CToken::EType::DEC );
  else if ( name == "writeln" )
    return addToken ( CToken::EType::WRITELN );
  else if ( name == "write" )
    return addToken ( CToken::EType::WRITE );
  else if ( name == "readln" )
    return addToken ( CToken::EType::READLN );

  return addToken ( CToken::EType::ERROR );
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::scanBasicKeyword      ( std::string     & name )
{
  if ( name == "program" ) 
    return addToken ( CToken::EType::PROGRAM );
  else if ( name == "function" )
    return addToken ( CToken::EType::FUNCTION );
  else if ( name == "procedure" )
    return addToken ( CToken::EType::PROCEDURE );
  else if ( name == "forward" )
    return addToken ( CToken::EType::FORWARD );
  else if ( name == "const" )
    return addToken ( CToken::EType::CONST );
  else if ( name == "var" )
    return addToken ( CToken::EType::VAR );
  else if ( name == "begin" )
    return addToken ( CToken::EType::BEGIN );
  else if ( name == "end" )
    return addToken ( CToken::EType::END );
  else if ( name == "integer" )
    return addToken ( CToken::EType::INTEGER_KW );
  else if ( name == "string" )
    return addToken ( CToken::EType::STRING_KW );
  else if ( name == "real" )
    return addToken ( CToken::EType::REAL_KW );

  return addToken ( CToken::EType::ERROR );
}
//-------------------------------------------------------------------------------------------------
int                          CLexer::getDecaNumber         ( void )
{
  int number = 0;

  if ( decaDigit ( m_Sin . peek () ) == -1 )
    return -1;

  while ( decaDigit ( m_Sin . peek () ) != -1 )
    number = number * 10 + ( decaDigit ( m_Sin . get () ) ) ;

  return number;
}
//-------------------------------------------------------------------------------------------------
int                          CLexer::getOctaNumber         ( void )
{
  int number = 0;
  if ( m_Sin . peek () != '&')
    return -1;
  m_Sin . get ();

  if ( octaDigit ( m_Sin . peek () ) == -1 )
     return -1;
  
  while ( octaDigit ( m_Sin . peek () ) != -1 )
    number = number * 8 + ( octaDigit ( m_Sin . get ()) );

  return number;
}
//-------------------------------------------------------------------------------------------------
int                          CLexer::getHexaNumber         ( void )
{
  int number = 0;

  if ( m_Sin . peek () != '$' )
    return -1;
  m_Sin . get ();

  if ( hexaDigit ( m_Sin . peek () ) == -1 )
    return -1;

  while ( hexaDigit ( m_Sin . peek () ) != -1 )
    number = number * 16 + ( hexaDigit ( m_Sin . get ()) );
  
  return number;
}
//-------------------------------------------------------------------------------------------------
string                       CLexer::getName               ( void )
{
  if ( ! isalpha ( m_Sin . peek() ) 
      && m_Sin . peek () != '_' 
      && m_Sin . peek () != '~'  )
    return "";

  string str = ""; 
  while ( isalpha ( m_Sin . peek () ) 
         || isdigit ( m_Sin . peek () )
         || m_Sin . peek () == '_'
         || m_Sin . peek () == '~'  )
    str += m_Sin . get ();
  return str;
}
//-------------------------------------------------------------------------------------------------
void                         CLexer::noWhiteSpaces         ( void )
{
  while ( isspace ( m_Sin . peek () ) )
    m_Sin . get ();
}
//-------------------------------------------------------------------------------------------------
void                         CLexer::skipCommentsBlanks    ( void )
{
  noWhiteSpaces ();

  bool hadComment = false;
  if ( m_Sin . peek () == '|')
  {
    hadComment = true;
    m_Sin . get ();
    while ( m_Sin . peek () != '|')
      m_Sin . get ();
    m_Sin . get ();
  } 

  if ( hadComment )
    skipCommentsBlanks ();
}
//-------------------------------------------------------------------------------------------------
bool                         CLexer::match                 ( char              compare )
{
  if ( m_Sin . peek () == compare ) 
  {
    m_Sin . get ();
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------------------------
CToken                       CLexer::addToken              ( CToken::EType     type )
{
  return CToken ( type, m_Sin . offset (), m_Sin . lineNumber () );
}
CToken                       CLexer::addToken              ( CToken::EType     type,
                                                             int               num )
{
  return CToken ( type, num, m_Sin . offset (), m_Sin . lineNumber () );
}
CToken                       CLexer::addToken              ( CToken::EType     type,
                                                             std::string       str )
{
  return CToken ( type, str, m_Sin . offset (), m_Sin . lineNumber () );
}
//=================================================================================================
