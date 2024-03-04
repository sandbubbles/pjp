#include <iostream>

#include "codegen.hpp"
#include "misc.hpp"

#define INT_SIZE 32

using namespace std;
//=================================================================================================
                             CCodeGen::CCodeGen            ( void )
  : m_Builder ( m_Context ),
    m_Module ( std::make_unique<llvm::Module> ( "milabb", m_Context ) )
{}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::Generate            ( vector<AST::ANode> & ns )
{
  try
  {
    CCodeGen codegen;
    codegen . createIO ();

    for ( auto node : ns )
      node -> Accept ( codegen );

    codegen . m_Module -> print ( llvm::outs(), nullptr ); // mila script takes the output from std::out
  }
  catch ( CCodegenException & e )
  {
    cout << "Codegen exception: ";
    throw e;
  }
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitGroup          ( AST::CGroupNode  & n )
{
  n . Child () -> Accept ( *this ); 
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitInt            ( AST::CIntNode    & n )
{
  m_Value = llvm::ConstantInt::get ( m_Context, llvm::APInt ( INT_SIZE, n . Value (), false ) );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitString         ( AST::CStringNode & n )
{
  m_Value = m_Builder . CreateGlobalStringPtr ( llvm::StringRef ( n . Value () ) );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitAssign         ( AST::CAssignNode & n )
{
  llvm::Value * r;
  n . Value  () -> Accept ( *this );
  r = m_Value;
  
  if ( m_LocalVars[n . Name ()] )
    m_Value = m_Builder . CreateStore ( r , m_LocalVars[n . Name ()] );
  else if ( m_GlobalVars[n . Name ()] )
    m_Value = m_Builder . CreateStore ( r, m_GlobalVars[n . Name ()] );
  else
    throw error ( "Couldn't store value into " + n . Name () );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitCall           ( AST::CCallNode   & n )
{
  llvm::Function * calleeF = m_Module -> getFunction ( n . Callee () );

  if ( ! calleeF )
    throw error ( "No function of name" + n . Callee () );
  
  uint32_t argSize = n . Arguments () . size ();
  if ( calleeF -> arg_size () != argSize )
    throw error ( "Arguments size and Parameters size aren't the same" );

  vector<llvm::Value *> argsV;
  for ( uint32_t i = 0, e = argSize; i != e; ++i )
  {
    ( n . Arguments ())[i] -> Accept ( * this );
    argsV . push_back ( m_Value );
    if ( ! argsV . back () )
      throw error ( "Argument cannot be null" );
  }

  if (  calleeF -> getReturnType () -> isVoidTy () )
    m_Value = m_Builder . CreateCall ( calleeF, argsV );
  else
    m_Value = m_Builder . CreateCall ( calleeF, argsV, "callexpr" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitIdentifier     ( AST::CIdentifierNode    & n )
{
  m_Value = m_Builder . CreateLoad ( findByName ( n ), n . Name () . c_str() );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitNot            ( AST::CNotNode    & n )
{
  n . Child () -> Accept ( *this );
  m_Value = m_Builder . CreateNot ( m_Value, "not" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitPos            ( AST::CPosNode    & n )
{
  n . Child () -> Accept ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitNeg            ( AST::CNegNode    & n )
{
  n . Child () -> Accept ( *this );
  m_Value = m_Builder . CreateNeg ( m_Value, "neg" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitEqual          ( AST::CEqualNode  & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpEQ ( l, r , "eq" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );  
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitNotEq          ( AST::CNotEqNode  & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;
 
  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpNE ( l, r , "neq" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );  
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitGreat          ( AST::CGreatNode  & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpSGT ( l, r , "gt" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );  
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitGreatEq        ( AST::CGreatEqNode& n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;
  
  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpSGE ( l, r , "ge" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );   
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitLess           ( AST::CLessNode   & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpSLT ( l, r , "lt" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );  
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitLessEq         ( AST::CLessEqNode & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateIntCast ( m_Builder . CreateICmpSLE ( l, r , "le" ),
                                        llvm::Type::getInt32Ty ( m_Module -> getContext () ),
                                        false,
                                        "castToInt" );  
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitPlus           ( AST::CPlusNode   & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateAdd ( l, r , "add" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitMinus          ( AST::CMinusNode  & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateSub ( l, r , "sub" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitMul            ( AST::CMulNode    & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateMul ( l, r , "mul" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitDiv            ( AST::CDivNode    & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateSDiv ( l, r , "div" );       // The options are F, U, and S
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitOr             ( AST::COrNode     & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateOr ( l, r , "or" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitAnd            ( AST::CAndNode    & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateAnd ( l, r , "and" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitMod            ( AST::CModNode    & n )
{
  n . LeftChild () -> Accept ( *this );
  llvm::Value * l = m_Value;
  n . RightChild () -> Accept ( *this );
  llvm::Value * r = m_Value;

  m_Value = m_Builder . CreateSRem ( l, r , "mod" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitExprStmt       ( AST::CExprStmtNode & n )
{
  n . Child () -> Accept ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitWrtInt         ( AST::CWrtIntNode & n )
{
  n . Child () -> Accept ( *this );
  llvm::Function * function;
  function = m_Module -> getFunction ( "writeint" );
  m_Value = m_Builder . CreateCall ( function, m_Value , "wrtcall" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitWrtStr         ( AST::CWrtStrNode & n )
{
  n . Child () -> Accept ( *this );
  llvm::Function * function;
  function = m_Module -> getFunction ( "writestring" );
  m_Value = m_Builder . CreateCall ( function, m_Value, "wrtcall" );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitRdlStmt        ( AST::CRdlStmtNode& n )  
{
  m_Value = findByName ( *(dynamic_pointer_cast<AST::CIdentifierNode> ( n . Child () )));
  if ( ! m_Value )
    throw error ( "For now left hand side can be only identifier" );

  llvm::Function * function = m_Module -> getFunction ( "readln" );
  m_Value = m_Builder . CreateStore ( m_Builder . CreateCall ( function ) , m_Value );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitVar            ( AST::CVarNode    & n )
{
  m_Module -> getOrInsertGlobal ( llvm::StringRef ( n . Name () ), m_Builder . getInt32Ty () );
  llvm::GlobalVariable * global = m_Module -> getGlobalVariable ( llvm::StringRef ( n . Name () ));
  m_GlobalVars[n . Name ()] = global;
  global -> setLinkage ( llvm::GlobalValue::ExternalLinkage );                 // For some reason other linkage doesn't work
  global -> setInitializer ( llvm::ConstantInt::get ( m_Context, 
                                                      llvm::APInt ( INT_SIZE,
                                                                    0,
                                                                    false ) ) );
  m_Value = global;
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitVars           ( AST::CVarsNode   & n )
{
  for ( auto var : n . Vars () )
    var -> Accept ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitConst          ( AST::CConstNode  & n )
{
  m_Module -> getOrInsertGlobal ( llvm::StringRef ( n . Name () ), m_Builder . getInt32Ty () );
  llvm::GlobalVariable * constant = m_Module -> getGlobalVariable ( llvm::StringRef ( n . Name () ));
  m_Constants[n . Name ()] = constant;
  constant -> setLinkage ( llvm::GlobalValue::ExternalLinkage );               // For some reason other linkage doesn't work
  constant -> setInitializer ( llvm::ConstantInt::get ( m_Context, 
                                                        llvm::APInt ( INT_SIZE,
                                                                      n . Value (),
                                                                      false ) ) );
  constant -> setConstant ( true );
  m_Value = constant;
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitConsts         ( AST::CConstsNode & n )
{
  for ( auto con : n . Consts () )
    con -> Accept ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitBlock          ( AST::CBlockNode  & n )
{
  for ( auto stmt : n . Stmts () )
    stmt -> Accept ( *this );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitIf             ( AST::CIfNode     & n )
{
  llvm::Function   * parentFn;
  llvm::Value      * cond;
  llvm::BasicBlock * thenbb, * elsebb, * contbb;

  n . Condition () -> Accept ( *this );

  if ( ! m_Value )
    throw error ( "Ifstmt must have a condition" );

  cond = m_Builder . CreateICmpNE ( m_Value, 
                                    llvm::ConstantInt::get ( m_Context, 
                                                             llvm::APInt ( INT_SIZE, 0, false ) ),
                                    "ifcond" );

  parentFn = m_Builder . GetInsertBlock() -> getParent ();
  thenbb = llvm::BasicBlock::Create ( m_Context, "then",   parentFn );
  elsebb = llvm::BasicBlock::Create ( m_Context, "else",   parentFn );
  contbb = llvm::BasicBlock::Create ( m_Context, "ifcont", parentFn );

  m_Builder . CreateCondBr ( cond, thenbb, elsebb );

  m_Builder . SetInsertPoint ( thenbb );
  n . Then () -> Accept ( *this );

  if ( ! m_Value )
    throw error ( "Ifstmt must have a then block" );

  m_Builder . CreateBr ( contbb );
  m_Builder . SetInsertPoint ( elsebb );

  if ( n . Else () )
  {
    n . Else () -> Accept ( *this );
    if ( ! m_Value )
      throw error ( "Ifstmt else branch should not be null" );
  }

  m_Builder . CreateBr ( contbb );
  m_Builder . SetInsertPoint ( contbb );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitWhile          ( AST::CWhileNode  & n )
{
  llvm::Function * parentFn = m_Builder . GetInsertBlock() -> getParent ();
  llvm::Value    * condV;

  llvm::BasicBlock *condbb  = llvm::BasicBlock::Create ( m_Context, "whileCond", parentFn );
  llvm::BasicBlock *bodybb  = llvm::BasicBlock::Create ( m_Context, "whileBody", parentFn );
  llvm::BasicBlock *contbb  = llvm::BasicBlock::Create ( m_Context, "whileCont", parentFn );

  m_Builder . CreateBr ( condbb );
  m_Builder . SetInsertPoint ( condbb );

  n . Condition () -> Accept ( *this );
  if ( ! m_Value )
    throw error ( "Whilestmt must have a condition" );

  condV = m_Builder . CreateICmpNE ( m_Value, 
                                     llvm::ConstantInt::get ( m_Context, 
                                                              llvm::APInt ( INT_SIZE, 0, false ) ),
                                     "whileCond" );
  
  m_Scopes . push ( contbb ); 
  m_Builder . CreateCondBr ( condV, bodybb, contbb );
  
  m_Builder . SetInsertPoint ( bodybb );
  n . Body () -> Accept ( *this );
  if ( ! m_Value )
    throw error ( "Whilestmt must have a body" );

  m_Builder . CreateBr ( condbb );
  m_Builder . SetInsertPoint ( contbb );

  m_Scopes . pop ();
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitFunDecl        ( AST::CFunDeclNode& n )
{
  llvm::Function * function;
  if ( m_Module -> getFunction ( n . Name () ) )
    function = m_Module -> getFunction ( n . Name () );
  else
    function = prototype ( n );

  if ( ! function )
    throw error ( "Couldn't create the function" );
  if ( ! n . Body () )
    return;
  if ( ! function -> empty () )
    throw error ( "Function cannot be redefined" );

  llvm::BasicBlock * bb = llvm::BasicBlock::Create ( m_Context, "entry", function );
  m_Builder . SetInsertPoint ( bb );
  m_LocalVars . clear ();

  for ( auto & arg : function -> args () )
  {
    llvm::AllocaInst * alloca = createEntryBAlloca ( function, arg . getName() );
    m_Builder . CreateStore ( &arg, alloca );
    m_LocalVars[arg . getName()] = alloca;
  }

  for ( auto & var : n . Vars () -> Vars () )
  {
    llvm::AllocaInst * alloca = createEntryBAlloca ( function, var -> Name () );
    m_Builder . CreateStore ( llvm::ConstantInt::get ( m_Context, 
                                                      llvm::APInt ( INT_SIZE, 0, false ) ),
                              alloca );
    m_LocalVars[var -> Name()] = alloca; 
  }
  
  if ( n . ReturnType () == AST::CFunDeclNode::EType::INTEGER )
  {
    llvm::AllocaInst * alloca = createEntryBAlloca ( function, n . Name () );
    m_Builder . CreateStore ( llvm::ConstantInt::get ( m_Context, 
                                                      llvm::APInt ( INT_SIZE, 0, false ) ),
                              alloca );
    m_LocalVars[n . Name()] = alloca; 
  }

  n . Body () -> Accept ( *this );

  if ( n . ReturnType () == AST::CFunDeclNode::EType::INTEGER )
    m_Builder . CreateRet ( m_Builder . CreateLoad ( m_LocalVars [ n . Name () ], "loadRet" ) );
  else
    m_Builder . CreateRetVoid ();

  llvm::verifyFunction ( * function );
  m_Value = function;
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitExit           ( AST::CExitNode   & n )
{
  llvm::Function * function =  m_Builder . GetInsertBlock() -> getParent ();
  if ( function -> getReturnType () -> isVoidTy () )
    m_Builder . CreateRetVoid ();
  else
    m_Builder . CreateRet ( m_Builder . CreateLoad ( m_LocalVars [ function -> getName () ],
                            "loadRet" ) );
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::VisitBreak          ( AST::CBreakNode   & n )
{
  if ( ! m_Scopes . size () )
    throw error ( "Break statement does not have where to escape" );
  else
    m_Builder . CreateBr ( m_Scopes . top () ); 
                                       // but not pop -> when there are more breaks in the same scope
}
//-------------------------------------------------------------------------------------------------
const char                 * CCodeGen::CCodegenException::what ( void ) const noexcept
{
  return "codegen exception";
}
//-------------------------------------------------------------------------------------------------
CCodeGen::CCodegenException  CCodeGen::error               ( std::string       message )
{
  ::error ( message, -1, -1 );
  return CCodegenException ();
}
//-------------------------------------------------------------------------------------------------
void                         CCodeGen::createIO            ( void )
{
 //write int
  std::array<llvm::Type*, 1> argumentsI ( { llvm::Type::getInt32Ty ( m_Context ) } );
  llvm::FunctionType * functionTypeI = llvm::FunctionType::get ( llvm::Type::getInt32Ty ( m_Context ), argumentsI, false );
  llvm::Function     * functionI     = llvm::Function::Create ( functionTypeI,
                                                                llvm::Function::ExternalLinkage,
                                                                "writeint",
                                                                m_Module . get () );

  functionI -> args () . begin () -> setName ( "x" );
  
  //write string
  std::array<llvm::Type*, 1>  argumentsS ( { llvm::Type::getInt8PtrTy ( m_Context ) } );
  llvm::FunctionType * functionTypeS = llvm::FunctionType::get ( llvm::Type::getInt32Ty ( m_Context ), argumentsS, false );
  llvm::Function     * functionS     = llvm::Function::Create ( functionTypeS,
                                                                llvm::Function::ExternalLinkage,
                                                                "writestring",
                                                                m_Module . get () );
  functionS -> args () . begin () -> setName ( "x" );
  
  //readln
  llvm::FunctionType * functionTypeR = llvm::FunctionType::get ( llvm::Type::getInt32Ty ( m_Context ), false );
  llvm::Function     * functionR     = llvm::Function::Create ( functionTypeR,
                                                                llvm::Function::ExternalLinkage,
                                                                "readln",
                                                                m_Module . get () );
}
//-------------------------------------------------------------------------------------------------
llvm::AllocaInst           * CCodeGen::createEntryBAlloca  ( llvm::Function  * function,
                                                             const string    & name ) 
{
  llvm::IRBuilder<> tmpBuilder ( & function -> getEntryBlock (),
                                   function -> getEntryBlock () . begin() );

  return tmpBuilder . CreateAlloca ( llvm::Type::getInt32Ty ( m_Context ), 0, name . c_str () );
}
//-------------------------------------------------------------------------------------------------
llvm::Value                * CCodeGen::findByName          ( AST::CIdentifierNode & n )
{
  llvm::Value * value;
  value = m_LocalVars[n . Name ()];
  if ( ! value )
    value = m_GlobalVars[n . Name ()];
  if ( ! value )
    value = m_Constants[ n . Name ()];

  if ( ! value )
    throw error ( "Unknown identifier " + n . Name () );
  return value;
}
//-------------------------------------------------------------------------------------------------
llvm::Function             * CCodeGen::prototype           ( AST::CFunDeclNode & n )
{
  vector<llvm::Type*> ints ( n . Parameters () . size (), llvm::Type::getInt32Ty ( m_Context ) );
  
  llvm::Type* type;

  if ( n . ReturnType () == AST::CFunDeclNode::EType::INTEGER )
    type = llvm::Type::getInt32Ty ( m_Context );
  else 
    type = llvm::Type::getVoidTy ( m_Context );
  
  llvm::FunctionType * functionType;
  if ( n . Parameters () . size () )
    functionType = llvm::FunctionType::get ( type, ints, false );
  else 
    functionType = llvm::FunctionType::get ( type, false );

  llvm::Function * function = llvm::Function::Create ( functionType,
                                                       llvm::Function::ExternalLinkage, 
                                                       n . Name (),
                                                       m_Module . get () );
  uint32_t idx = 0;
  for ( auto & param : function -> args () )
    param . setName ( n . Parameters () [idx++] -> Name () );
  
  return function;
}
//=================================================================================================