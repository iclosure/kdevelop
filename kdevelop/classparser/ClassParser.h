/********************************************************************
* Name    : Definition of tha classparser.                          *
* ------------------------------------------------------------------*
* File    : ClassParser.h                                           *
* Author  : Jonas Nordin(jonas.nordin@cenacle.se)                   *
* Date    : Mon Mar 15 13:14:44 CET 1999                            *
*                                                                   *
* ------------------------------------------------------------------*
* Purpose :                                                         *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
* Usage   :                                                         *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
* Macros:                                                           *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
* Types:                                                            *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
* Functions:                                                        *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
* Modifications:                                                    *
*                                                                   *
*                                                                   *
*                                                                   *
* ------------------------------------------------------------------*
*********************************************************************/
#ifndef _CLASSPARSER_H_INCLUDED
#define _CLASSPARSER_H_INCLUDED

#include <qstring.h>
#include <qlist.h>
#include <qstack.h>
#include <FlexLexer.h>
#include <fstream.h>
#include "tokenizer.h"
#include "ClassStore.h"
/** */
class CClassParser
{
public: // Constructor & Destructor

  CClassParser();
  ~CClassParser();

private: // Private classes
  /** This class represents a lexema. Used to store lexem in a stack. */
  class CParsedLexem
  {
    public: // Constructor & destructor
    
    CParsedLexem( int aType, const char *aText) { type = aType; text = aText; }
    ~CParsedLexem() {}
    
    public: // Public attributes
    
    /** The lexem text. */
    QString text;
    
    /** Type of lexem. */
    int type;
  };

public: // Public attributes

  /** Store for the classes. */
  CClassStore store;

public: // Public Methods

  /** Parse the two files and return the parsed classes found. */
  bool parse( const char *file = NULL );

  /** Remove all parsed classes */
  void wipeout();

  /** Output this object as text on stdout */
  void out()     { store.out(); }

private: // Private attributes

  /** Lexer used to fetch lexical patterns */
  yyFlexLexer *lexer;

  /** Current lexem */
  int lexem;

  /** The name of the parsed file. */
  QString currentFile;
  
  /** The scope in which the attributes and metods being defined. */
  PIExport declaredScope;

  /** Stack of lexems currently parsed. */
  QStack<CParsedLexem> lexemStack;
 
  /** Type of method. 0 = NORMAL else QTSIGNAL/QTSLOT. */
  int methodType;

  /** Tells if this is an static declaration. */
  bool isStatic;

private: // Private methods

  /** Get the next lexem from the lexer. */
  void getNextLexem()        { lexem = lexer->yylex(); }

  /** Fetch the current text from the lexer and return it. */
  const char *getText()      { return lexer->YYText(); }
  
  /** Fetch the current linenumber from the lexer and return it. */
  //int getLineno()            { debug("%d",lexer->lineno());return lexer->lineno() - 1;  }
  int getLineno()            { return lexer->lineno() - 1;  }

  /** Remove all elements from the stack. */
  void emptyStack();

  /** Parse all structure declarations.
   * @param aStruct The struct that holds the declarations.
   */
  void parseStructDeclarations( CParsedStruct *aStruct);

  /** Parse a structure using header declaration from stack.
   * @param aContainer Container to store the parsed struct in.
   */
  void fillInParsedStruct( CParsedContainer *aContainer );

  /** Parse a structure. 
   * @param aContainer Container to store the parsed struct in.
   */
  void parseStruct( CParsedContainer *aContainer );

  /** Parse an enumeration. */
  void parseEnum();

  /** Parse an union. */
  void parseUnion();

  /** Skip all lexems between '{' and '}'. */
  void skipBlock();

  /** Skip a template declaration. */
  void parseTemplate();

  /** Create a type using the arguments on the stack. */
  void fillInParsedType(QString &type);

  /** Tells if the current lexem is the end of a variable declaration. */
  bool isEndOfVarDecl();

  /** Initialize a attribute, except the type, using the arguments on
   *   the stack. */
  void fillInParsedVariableHead( CParsedAttribute *anAttr );

  /** Initialize a attribute using the arguments on the stack. */
  void fillInParsedVariable( CParsedAttribute *anAttr );

  /** Take what's on the stack and return as a list of variables. 
   *   Works for variable declarations like int foo, bar, baz.... */
  void fillInMultipleVariable( CParsedContainer *aContainer );

  /** Parse a variable declaration. */
  CParsedAttribute *parseVariable();

  /** Parse and add all arguments to a function. */
  void parseFunctionArgs( CParsedMethod *method );

  /** Initialize a method using the arguments on the stack. */
  void fillInParsedMethod(CParsedMethod *aMethod, bool isOperator=false);

  /** Parse a method declaration. */
  CParsedMethod *parseMethodDeclaration();

  /** Parse a method implementation. */
  void parseMethodImpl(bool isOperator);

  /** Push lexems on the stack until we find something we know and 
   *   return what we found. */
  int checkClassDecl();

  /** Parse the inheritance clause of a class declaration. */
  void parseClassInheritance( CParsedClass *aClass );

  /** Parse a class header, i.e find out classname and possible parents. */
  CParsedClass *parseClassHeader();

  /** Parse the declarations of a class. */
  void parseClassDeclarations( CParsedClass *aClass );
  
  /** Parse a class declaration. */
  CParsedClass *parseClass();

  /** Parse all methods and attributes and add them to the container. */
  void parseMethodAttributes( CParsedContainer *aContainer );

  /** Parse toplevel statements */
  void parseToplevel();

  /** Remove all items in the store with references to the file. */
  void removeWithReferences( const char *aFile );

  /** Parse a file */
  void parseFile( ifstream &file );

  /** Reset the internal variables */
  void reset();
};

#endif
