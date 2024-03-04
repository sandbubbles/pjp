#ifndef PJPPROJECT_LEXER_HPP
#define PJPPROJECT_LEXER_HPP

#include <iostream>
#include <vector>

#include "token.hpp"
#include "misc.hpp"
//=================================================================================================
class CIstream {
  public: 
                             CIstream                      ( std::istream    & sin );
    uint                     lineNumber                    ( void ) const;
    uint                     offset                        ( void ) const;
    bool                     eof                           ( void );
    int                      get                           ( void );           // this is often called advance
                                                                               // but in c++ streams does
                                                                               // the job, and this way we wont
                                                                               // have to rewrite everything 
    int                      peek                          ( void );   
  private:  
    std::istream           & m_Sin;
    uint                     m_LineNumber = 1;  
    uint                     m_Offset     = 1;                                      
};  
//=================================================================================================
/* This lexer does not support arrays and real numbers.
   It's pretty anyway.
*/
class CLexer {
  public:
                             CLexer                        ( std::istream    & sin );
    /* These are the two main ways to use the lexer - either ask for a token one at a time
       or ask for the vector of all the tokens in the input.
    */
    CToken                   NextToken                     ( void ); 
    static std::vector<CToken> Analyse                     ( std::istream    & sin );

  private:
    CToken                   nextToken                     ( void );
    CToken                   scanPunctuation               ( void ); 
    CToken                   scanOperator                  ( void );
    CToken                   scanNumber                    ( void );
    CToken                   scanString                    ( void );
    CToken                   scanName                      ( void );
    CToken                   scanRelationOp                ( void );
    CToken                   scanArithmeticOp              ( void );
    CToken                   scanOperatorKeyword           ( std::string     & name );
    CToken                   scanContFlowKeyword           ( std::string     & name );
    CToken                   scanBuiltinKeyword            ( std::string     & name );
    CToken                   scanBasicKeyword              ( std::string     & name );

    int                      getDecaNumber                 ( void );
    int                      getOctaNumber                 ( void );
    int                      getHexaNumber                 ( void );
    std::string              getName                       ( void ); 
    void                     noWhiteSpaces                 ( void );
    void                     skipCommentsBlanks            ( void );
    bool                     match                         ( char              compare );
    CToken                   addToken                      ( CToken::EType     type );
    CToken                   addToken                      ( CToken::EType     type,
                                                             int               num );
    CToken                   addToken                      ( CToken::EType     type,
                                                             std::string       str );

    CIstream                 m_Sin;
};
//=================================================================================================
#endif //PJPPROJECT_LEXER_HPP