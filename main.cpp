#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "parser.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "misc.hpp"
#include "astnode.hpp"
#include "printer.hpp"
#include "codegen.hpp"

using namespace std;
//Use tutorials in: https://llvm.org/docs/tutorial/
//=================================================================================================
int main (int argc, char *argv[]) 
{
  if ( argc != 2 )
  {
    cout << "Expected file" << endl;
    cout << "  ./mila <filename>" << endl;
    return 1;
  }
  
  ifstream fin ( argv[1] );

  if ( ! fin.is_open () )
  {
    cout << "Couldn't open the file" << endl;
    return 1;
  }

  vector<AST::ANode> nodes = CParser :: Analyse ( CLexer :: Analyse ( fin ) );
  if ( ! g_HadError )
  {
    //CPrinter::Print ( nodes );
    CCodeGen::Generate ( nodes );
  }
  else
  {
    cout << "There was a mistake in parsing the code" << endl;
    return 1;
  }

  fin . close ();
  return 0;
}
