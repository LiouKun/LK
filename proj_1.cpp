# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;
string Get_token( bool& ) ;
int Token_type( string&str ) ;
bool S_expression( string&str, vector<string>&all_tokens, int&error_type, string&error_token ) ; // parser
void Cross_out() ; // ¥X¿ù«á ®ø¤Æ±¼«á­±ªº¿é¤J 
void GetChar( char&ch ) ; 
void PrettyPrint( vector<string> vec, int&i, int space ) ;
void Fix_all_tokens( vector<string>&vec ) ; // ­×¤@¤Uvector 
struct node {   
  string atom ;           
	node *left;      // Link to left child
	node *right;     // Link to right child   
};
char separator = ' ' ;  // ¦³«Ý§ï¶iªº³]­p ¥i¥H§ï¥Îcin.peek 
int line = 1, column = 0 ;
int pre_line = 1, pre_column = 1 ;
bool command_end = false ;
int main() {
  int gTestNum ;
  cin >> gTestNum ;
  cout << "Welcome to OurScheme!\n"
  vector<string> all_tokens ;
  bool str_error = false ; // ¦r¦ê¿ù»~¡A¦b¦r¦ê¤º´«¦æªº¿ù»~ 
  string token = " " ;
  int error_type ;
  string error_token ;
  while ( !cin.eof() ) {
    token = Get_token( str_error ) ;
    while( token == "" ) token = Get_token( str_error ) ; // ¤@©w­nget¨ì¤@­Ótoken
    if ( !str_error && S_expression( token, all_tokens, error_type, error_token ) ) { 
      cout << "true\n" ;  // ¨S¦³¥ô¦ó¿ù»~
      int i = 0, space = 0 ;
      Fix_all_tokens( all_tokens ) ;
      PrettyPrint( all_tokens, i, space ) ;
      cout << "\n" ;
    } // if()
    else {
      if ( error_type == 1 ) {
        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << line ;
        cout << " Column " << column << " is >>" << error_token << "<<\n" ;
      }
      else if ( error_type == 2 ) {
        cout << "ERROR (unexpected token) : ')' expected when token at Line " << line << " Column "<< column ;
        cout << " is >>" << error_token << "<<\n" ;
      }
      else if ( error_type == 3 || str_error ) {
        cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << line ;
        cout <<  " Column "<< column+1 << "\n" ;
      }
      else { // eof
      }
    } // else()
    line = 0 ;
    column = 0 ;
    all_tokens.clear() ;
    command_end = true ;
  } // while()
} // main()

void GetChar( char&ch ) {
  ch = cin.get() ;
  if ( ch == '\n' ) {
    line++ ;
    column = 0 ;
  }
  else {
    if ( !isspace(ch) && line == 0 ) line = 1 ;
    column++ ;
  }
} // GetChar()
string Get_token(bool&str_error) { 
  str_error = false ;
  string token ;
  char ch ;
  ch = cin.peek() ;
  while ( isspace(ch) && !cin.eof() ) { //get all white-space
    GetChar(ch) ;
    ch = cin.peek() ; // if this is not white space, don't get this ch ,and end loop
  } // while()
  // start to get token!!
  while ( !isspace(ch) && ch != '(' && ch != ')' && ch != '\'' && ch != '"' && ch != ';' ) {
    GetChar(ch) ;
    token = token + ch ;
    ch = cin.peek() ;
  } // while()
  
  if ( token != "" ) { // cut down by 'isspace', and token is done, bye
    return token ;
  } // if() 
  else { // time to cut sepatator char, token is empty
    if ( cin.peek() != ';' ) GetChar(ch) ; // get token is end
    else {
      ch = cin.peek() ;
      while ( ch != '\n' ) {
        ch = cin.get() ;
        ch = cin.peek() ;
      }
      string str = Get_token(str_error) ;
      return str ;
    } // else if()
  } // else() 
  
  // start to handle......
  // !!!separator char!!! cut down by 'rule'
  if ( ch == '(' || ch == ')' || ch == '\'' ) { // LP RP QUOTE
    token = token + ch ;
    return token ; // bye
  } // if() 
  else if ( ch == '"' ) { // this time, 'first double quote' has been cut down
    token = token + ch ;
    ch = cin.peek() ; 
    while ( ch != '"' && ch != '\n' ) {
      GetChar(ch) ;
      if ( ch == '\\' ) {
        char back_slash = ch ;
        ch = cin.peek() ; // peek next char
        if ( ch == 't' ) {
          GetChar(ch) ;
          token = token + '\t' ;
        } // if()
        else if ( ch == '\\' ) {
          GetChar(ch) ;
          token = token + '\\' ;
        } // else if
        else if ( ch == 'n' ) {
          GetChar(ch) ;
          token = token + '\n' ;
        } // else if
        else if ( ch == '"' ) {
          GetChar(ch) ;
          token = token + '\"' ;
        } // else if
        else token = token + back_slash ;
      }
      else  token = token + ch ;
      
      ch = cin.peek() ;
      if ( ch == '\n' ) {
        str_error = true ;
        return token ;
      }
      if ( ch == '"' ) {
        GetChar(ch) ;
        token = token + ch ; // second double quote, and string token is done
        return token ;
      }
    } // while() 
    if ( ch == '\n' ) {
       str_error = true ;
       return token ;
    }
  } // if()
} // Get_token() 
void Cross_out() { // ¹º±¼¿ù»~«áªº©Ò¦³ªF¦è 
    char ch = cin.peek() ;
    while ( ch != '\n' ) {
      ch = cin.get() ;
      ch = cin.peek() ;
    } // while()
} // Cross_out()
int Token_type ( string&str ) {   // §PÂ_type ¶¶«K³B²znil #t () 
  /*
  1 LEFT-PAREN       // '('
  2 RIGHT-PAREN      // ')'
  3 INT              // e.g., '123', '+123', '-123'
  4 STRING           // "string's (example)." (strings do not extend across lines)
  5 DOT              // '.'
  6 FLOAT            // '123.567', '123.', '.567', '+123.4', '-.123'
  7 NIL              // 'nil' or '#f', but not 'NIL' nor 'nIL'
  8 T                // 't' or '#t', but not 'T' nor '#T'
  9 QUOTE            // '
  10 SYMBOL    */
  if ( str[0] == '"' && str[str.length()-1] == '"' ) { 
    return 4 ;
  } // if()
  else if ( str == "nil" || str == "#f" ) {
    str = "nil" ;
    return 7 ;
  } // else if()
  else if ( str == "t" || str == "#t" ) {
    str = "#t" ;
    return 8 ;
  } // else if()
  else if ( str == "(" ) {
    char ch = cin.peek() ;
    while ( isspace(ch) ) {
      GetChar(ch) ;
      ch = cin.peek() ;
    } // while()
    if ( ch == ')' ) {
      GetChar(ch) ;
      str = "nil" ;
      return 7 ;
    } // if()
    else return 1 ;
  } // else if()
  else if ( str == "'" ) {
    str = "quote" ;
    return 9 ;
  } // else if()
  else if ( str == "." ) return 5 ;
  else if ( str == ")" ) return 2 ;
  
  int dot = 0, plus = 0, minus = 0 ;
  bool isNum = false ;
  for ( int i = 0 ; i < str.length() ; i++ ) {
    if ( str[i] == '.' ) dot++ ;
    else if ( str[i] == '+' ) plus++ ;
    else if ( str[i] == '-' ) minus++ ;
    else if ( str[i] >= 48 && str[i] <= 57 ) isNum = true ;
    else {
      isNum = false ;
      break ;
    }
  } // for()
  if ( isNum && ( ( plus == 1 && minus == 0 ) || ( plus == 0 && minus == 1 ) || ( plus == 0 && plus == 0 ) ) && ( dot == 0 || dot == 1 ) ) {
    if ( dot == 1 ) return 6 ;
    else return 3 ;
  } // if()
  else return 10 ;
} // Token_type()
bool S_expression( string&str, vector<string>&all_tokens, int&error_type, string&error_token ) { // §PÂ_¤åªk 
  string token ; 
  bool str_error ; //  ¦r¦êerror
  error_type = 0 ;
  error_token = str ;
  int token_type = Token_type(str) ; 
  if ( token_type == 3 || token_type == 4 || token_type == 6 || token_type == 7 || token_type == 8 || token_type == 10  ) {
    all_tokens.push_back(str) ;
    return true ;
  } // if()
  else if ( token_type == 1 ) { // LP
    all_tokens.push_back(str) ;
    token = Get_token(str_error) ;
    while( token == "" ) token = Get_token( str_error ) ; // Åª¨ì¦³ªF¦è¬°¤î
    if ( str_error ) error_type = 3 ;
    if ( str_error || !S_expression(token, all_tokens, error_type, error_token ) ) return false ;
    else {
      token = Get_token(str_error) ;
      while( token == "" ) token = Get_token( str_error ) ;
      if ( str_error ) error_type = 3 ;
      while ( token != "." && token != ")"  )  {
        if ( str_error || !S_expression( token, all_tokens, error_type, error_token ) ) return false ;
        token = Get_token(str_error) ;
        while( token == "" ) token = Get_token( str_error ) ;
        if ( str_error ) error_type = 3 ;
      }
      
      if ( token == ")" ) {
        all_tokens.push_back(token) ;
        return true ;
      } // if()
      else if ( token == "." ) {
        all_tokens.push_back(token) ;
        token = Get_token(str_error) ;
        while( token == "" ) token = Get_token( str_error ) ;
        if ( str_error ) error_type = 3 ;
        if ( !str_error && S_expression(token, all_tokens, error_type, error_token ) ) {
          token = Get_token(str_error) ;
          while( token == "" ) token = Get_token( str_error ) ;
          if ( token != ")" ) {
            error_type = 2 ;
            Cross_out() ;
            return false ;
          } // if()
          else {
            all_tokens.push_back(token) ;
            return true ;
          }
        }  // if()
        else {
          error_type = 1 ;
          return false ;
        } // else
      } // else if
    } // else

  }
  else if ( token_type == 9 ) { // quote
    all_tokens.push_back("(") ;
    all_tokens.push_back("quote") ; 
    token = Get_token(str_error) ;
    while( token == "" ) token = Get_token( str_error ) ;
    if ( str_error ) error_type = 3 ;
    if ( str_error || !S_expression(token, all_tokens, error_type, error_token ) ) return false ;
    else {
      all_tokens.push_back(")") ;
      return true ;
    } // else
  } // else if()
  else { // DOT RP
    error_type = 1 ;
    Cross_out() ;
    return false ;
  } // else
} // S_expression()
void PrettyPrint( vector<string> vec, int&i, int space ) {
  while ( i < vec.size() )  { 
    if ( vec[i] == "(" ) {
      if ( i > 0 ) {
        if ( vec[i-1] != "(" ) {
          for( int x = 0 ; x < space ; x++ ) {
            cout << " " ;
          } // for()
        } // if()
      } // if()
      cout << vec[i] << " " ;
      i++ ;
      PrettyPrint( vec, i , space+2 ) ;
    } // if()
    else if ( vec[i] == ")" ) {
      for( int x = 0 ; x < space-2 ; x++ ) {
        cout << " " ;
      } // for()
      cout << vec[i] << "\n" ;
      i++ ;
      return ;
    } // else if()
    else {
      if ( i > 0 ) {
        if ( vec[i-1] == "(" ) {
          if ( Token_type( vec[i] ) == 3 ) {
            cout << atoi( vec[i].c_str() ) << "\n" ;
          }
          else if ( Token_type( vec[i] ) == 6 ) {
            cout << fixed  <<  setprecision(3) << atof( vec[i].c_str() )  << "\n" ;
            cout.unsetf( ios::fixed );
          }
          else {
            cout << vec[i] << "\n" ;
          }
        } // if()
        else {
          for( int x = 0 ; x < space ; x++ ) {
            cout << " " ;
          } // for()
          if ( Token_type( vec[i] ) == 3 ) {
            cout << atoi( vec[i].c_str() ) << "\n" ;
          }
          else if ( Token_type( vec[i] ) == 6 ) {
            cout << fixed  <<  setprecision(3) << atof( vec[i].c_str() )  << "\n" ;
            cout.unsetf( ios::fixed );
          }
          else {
            cout << vec[i] << "\n" ;
          }
        }
      } // if()
      else {
        if ( Token_type( vec[i] ) == 3 ) {
          cout << atoi( vec[i].c_str() ) << "\n" ;
        }
        else if ( Token_type( vec[i] ) == 6 ) {
          cout << fixed  <<  setprecision(3) << atof( vec[i].c_str() )  << "\n" ;
          cout.unsetf( ios::fixed );
        }
        else {
          cout << vec[i] << "\n" ;
        }
      } // else
      i++ ;
    } // else()
  } // while()
} // PrettyPrint()
void Fix_all_tokens( vector<string>&vec ) {
  int lp = 0  ;
  int rp = 0 ;
  for ( int i = 0 ; i < vec.size() ; i++) {
    if ( i != vec.size()-1 ) {
      if ( vec[i] == "." && vec[i+1] == "(" ) {
        vec.erase( vec.begin()+i ) ;
        vec.erase( vec.begin()+i ) ;
        for ( int x = i ; x < vec.size() ; x++ ) {
          if ( vec[x] == "(" ) lp++ ;
          if ( vec[x] == ")" ) rp++ ;
          if ( rp-lp == 1 ) {
            lp = 0 ;
            rp = 0 ;
            vec.erase( vec.begin()+x ) ;
            break ;
          } // if()
        } // for()
      } // if()
      else if ( vec[i] == "." && vec[i+1] == "nil" ) {
        vec.erase( vec.begin()+i ) ;
        vec.erase( vec.begin()+i ) ;
      }
    } // if()
  } // for()
} // Fix_all_tokens()
