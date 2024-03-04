#include <cassert>

#include "token.hpp"

using namespace std;
//=================================================================================================
                             CToken::CToken                ( EType             type,
                                                             uint32_t          offset,
                                                             uint32_t          line )
  : m_Type ( type ),
    m_Offset ( offset ),
    m_Line ( line )
{}
//-------------------------------------------------------------------------------------------------
                             CToken::CToken                ( EType             type,
                                                             int               num,
                                                             uint32_t          offset,
                                                             uint32_t          line )
  : m_Type ( type ),
    m_Int  ( num ),
    m_Offset ( offset ),
    m_Line ( line )
{
  assert ( ( type == EType::NUMBER ) ^ ! m_Int . has_value () );
}
//-------------------------------------------------------------------------------------------------
                             CToken::CToken                ( EType             type,
                                                             string            str,
                                                             uint32_t          offset,
                                                             uint32_t          line )
  : m_Type ( type ),
    m_String ( str ),
    m_Offset ( offset ),
    m_Line ( line )
{
  assert ( ( type == EType::IDENTIFIER ) ^ ! m_String . has_value () 
        || ( type == EType::STRING ) ^ ! m_String . has_value () );
}
//-------------------------------------------------------------------------------------------------
                             CToken::CToken                ( void )
  : CToken ( EType::ERROR, 0, 0 )   
{}
//-------------------------------------------------------------------------------------------------
CToken::EType                CToken::Type                  ( void ) const
{
  return m_Type;
}
//-------------------------------------------------------------------------------------------------
int                          CToken::Int                   ( void ) const
{
  if ( m_Type != EType::NUMBER )
    return -1;               // the stored integer in a token is always positive
  return m_Int . value ();
}
//------------------------------------------------------------------------------------------------- 
string                       CToken::String                ( void ) const
{
  return m_String . value ();
}
//---------------------------------------------------------------------------------------------
ostream                    & operator <<                   ( ostream         & oss,
                                                             const CToken    & x ) 
{
  switch ( x . m_Type )
  {
    case CToken::EType::EOFF:
      return oss << "EOFF";
    case CToken::EType::LEFT_PAREN:
      return oss << "LEFT_PAREN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::RIGHT_PAREN:
      return oss << "RIGHT_PAREN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::COMMA:
      return oss << "COMMA( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DOT:
      return oss << "DOT( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::SEMICOLON:
      return oss << "SEMICOLON( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DOUBLE_DOT:
      return oss << "DOUBLE_DOT( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::ASSIGN:
      return oss << "ASSIGN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::EQUAL:
      return oss << "EQUAL( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::NOT_EQUAL:
      return oss << "NOT_EQUAL( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::GREATER:
      return oss << "GREATER( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::LESS:
      return oss << "LESS( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::GREATER_EQUAL:
      return oss << "GREATER_EQUAL( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::LESS_EQUAL:
      return oss << "LESS_EQUAL( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::PLUS:
      return oss << "PLUS( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::MINUS:
      return oss << "MINUS( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::MULTIPLY:
      return oss << "MULTIPLY( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::PROGRAM:
      return oss << "PROGRAM( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::FUNCTION:
      return oss << "FUNCTION( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::PROCEDURE:
      return oss << "PROCEDURE( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::FORWARD:
      return oss << "FORWARD( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::CONST:
      return oss << "CONST( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::VAR:
      return oss << "VAR( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::BEGIN:
      return oss << "BEGIN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::END:
      return oss << "END( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::IF:
      return oss << "IF( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::THEN:
      return oss << "THEN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::ELSE:
      return oss << "ELSE( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::FOR:
      return oss << "FOR( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::TO:
      return oss << "TO( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DONWTO:
      return oss << "DONWTO( "   << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::WHILE:
      return oss << "WHILE( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DO:
      return oss << "DO( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::EXIT:
      return oss << "EXIT( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::BREAK:
      return oss << "BREAK( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::INC:
      return oss << "INC( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DEC:
      return oss << "DEC( "   << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::WRITELN:
      return oss << "WRITELN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::WRITE:
      return oss << "WRITE( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::READLN:
      return oss << "READLN( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::OR:
      return oss << "OR( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::AND:
      return oss << "AND( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::DIV:
      return oss << "DIV( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::MOD:
      return oss << "MOD( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::NOT:
      return oss << "NOT( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::INTEGER_KW:
      return oss << "INTEGER( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::STRING_KW:
      return oss << "STRING( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::REAL_KW:
      return oss << "REAL( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::IDENTIFIER:
      return oss << "IDENTIFIER" << "[" << x . String () << "]( " << x . m_Offset << "o, " << x . m_Line << "l )" ;
    case CToken::EType::NUMBER:
      return oss << "NUMBER" << "[" << x . Int () << "](" << x . m_Offset << "o, " << x . m_Line << "l )" ;
    case CToken::EType::STRING:
      return oss << "STRING" << "[" << x . String () << "]( " << x . m_Offset << "o, " << x . m_Line << "l )" ;
    case CToken::EType::ERROR:
      return oss << "ERROR( " << x . m_Offset << "o, " << x . m_Line << "l )";
    case CToken::EType::TOTAL:
      return oss << "# of tokens: " << (int) CToken::EType::TOTAL;
    default:
      return oss; 
  }
}
//-------------------------------------------------------------------------------------------------
uint32_t                     CToken::Offset                ( void )
{
  return m_Offset;
}
//-------------------------------------------------------------------------------------------------
uint32_t                     CToken::Line                  ( void )
{
  return m_Line;
}
//=================================================================================================
