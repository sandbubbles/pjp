#ifndef PJPPROJECT_TOKEN_HPP
#define PJPPROJECT_TOKEN_HPP

#include <iostream>
#include <sstream>
#include <optional>
/* CToken does not have Types to support real numbers and arrays.
*/
//=================================================================================================
class CToken
{
  public:
    enum class EType : uint8_t
    {
      EOFF = 0,              // EOF is a C++ keyword
    //Punctuation
      LEFT_PAREN, RIGHT_PAREN,
      COMMA, DOT, SEMICOLON, DOUBLE_DOT,
    //Operators
      ASSIGN,
      EQUAL, NOT_EQUAL,
      GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
      PLUS, MINUS,
      MULTIPLY,
    //Keywords
      PROGRAM,
      FUNCTION, PROCEDURE,
      FORWARD,
      CONST, VAR,
      BEGIN, END,
      INTEGER_KW, STRING_KW, REAL_KW,
      IF, THEN, ELSE,
      FOR, TO, DONWTO,
      WHILE, DO,
      EXIT,
      BREAK,
      INC, DEC,
      WRITELN, WRITE, READLN,
      OR, AND, DIV, MOD, NOT,
    //Literals
      IDENTIFIER,
      NUMBER,
      STRING,
    //
      ERROR,
      TOTAL
    };
    //---------------------------------------------------------------------------------------------

  public:
                             CToken                        ( EType             type,
                                                             uint32_t          offset,
                                                             uint32_t          line );
                             CToken                        ( EType             type,
                                                             int               num,
                                                             uint32_t          offset,
                                                             uint32_t          line );
                             CToken                        ( EType             type,
                                                             std::string       str,
                                                             uint32_t          offset,
                                                             uint32_t          line );
                             CToken                        ( void );
    EType                    Type                          ( void ) const;
    int                      Int                           ( void ) const;
    std::string              String                        ( void ) const;
    friend std::ostream    & operator <<                   ( std::ostream    & oss,
                                                             const CToken    & x );
    uint32_t                 Offset                        ( void );
    uint32_t                 Line                          ( void );

  private:
    EType                    m_Type;
    std::optional<int>       m_Int                         = std::nullopt;
    std::optional<std::string> m_String                    = std::nullopt;
    uint32_t                 m_Offset;
    uint32_t                 m_Line;
};
//=================================================================================================
#endif //PJPPROJECT_TOKEN_HPP
