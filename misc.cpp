#include "misc.hpp"


bool g_HadError = false;
int g_NumErrors = 0;
//-------------------------------------------------------------------------------------------------         
void                         error                         ( std::string       message,
                                                             int               line,
                                                             int               offset )
{
  std::cerr << "[offset: "<< offset << ",line: "<< line << "] Error" << ": " << message << std::endl;
  g_HadError = true;
  ++ g_NumErrors;
}
//-------------------------------------------------------------------------------------------------
int                          decaDigit                     ( char              asses )
{
  if ( asses >= '0' && asses <= '9' )
    return asses - '0';
  else
    return -1;
}
//-------------------------------------------------------------------------------------------------
int                          octaDigit                     ( char              asses )
{
  if ( asses >= '0' && asses <= '7' )
    return asses - '0';
  else
    return -1;
}
//-------------------------------------------------------------------------------------------------
int                          hexaDigit                     ( char              asses )
{
  if ( ( asses >= '0' && asses <= '9' ) || ( asses >= 'A' && asses <= 'F' ) )
  {
    if ( asses >= '0' && asses <= '9'  )
      return asses - '0';
    else
      return asses - 'A' + 10;
  }
  else
    return -1;
}
//-------------------------------------------------------------------------------------------------