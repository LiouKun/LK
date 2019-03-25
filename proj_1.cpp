# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;
string Get_token( bool& ) ;
int Token_type( string&str ) ;
bool S_expression( string&str ) ;
void Cross_out() ;

struct node {   
  string atom ;           
	node *left;      // Link to left child
	node *right;     // Link to right child
};
char separator = ' ' ;  // 有待改進的設計 可以改用cin.peek 
int main() {
  bool str_error ; // 字串錯誤，在字串內換行的錯誤 
  string token = " " ; 
  while ( !cin.eof() ) {
    token = Get_token( str_error ) ;
    while( token == "" ) token = Get_token( str_error ) ; // 一定要get到一個token 
    if ( !str_error && S_expression( token ) )  cout << "true\n" ;  // 沒有任何錯誤 
    else cout << "error\n" ;
  } // while()
} // main()

string Get_token( bool&str_error ) { // 回傳一個token回去 
  str_error = false ; // 字串錯誤初始為false 
  string token ;
  char ch ;
  ch = separator ; // 從那個separator字元開始判斷 
  while ( isspace(ch) && !cin.eof() ) { 
  // 跳過所有white-spaces、直到碰到「非white-space」(或END-OF-FILE)為止
    ch = cin.get() ;
  } // while()
  while ( !isspace(ch) && ch != '(' && ch != ')' && ch != '\'' && ch != '"' && ch != ';' ) {
  // 從那個「非white-space」開始 "繼續讀入"、直到碰到「separator-字元」為止
    token = token + ch ;
    ch = cin.get() ;
  } // while()
  if ( token != "" ) { 
    separator = ch ;
    return token ;
  } // if() 
  
  // 以下開始處理分離字元 
  if ( ch == '(' || ch == ')' || ch == '\'' ) {
    // 碰到這三個 簡單 直接return回去 
    separator = ' ' ;
    token = token + ch ;
    return token ;
  } // if() 
  else if ( ch == '"' ) { // 字串的處理 
    separator = ' ' ;
    token = token + ch ;
    ch = cin.get() ;
    while ( ch != '"' ) {
      if ( ch == '\n' ) { // 字串處理的error在這裡就可以處理
        str_error = true ; // 字串error
        token = "error" ;
        return token ;
      }
      else if ( ch == '\\' ) { // 碰到'\' -> escape字元處理 
        char back_slash = ch ; 
        ch = cin.get() ;
        if ( ch == 't' ) token = token + '\t' ;
        else if ( ch == '\\' ) token = token + '\\' ;
        else if ( ch == 'n' ) token = token + '\n' ;
        else if ( ch == '"' ) token = token + '\"' ;
        else token = token + back_slash ;
      }
      else  token = token + ch ;
      ch = cin.get() ;
      if ( ch == '"' ) token = token + ch ;
    } // while() 
  } // if()
  else if ( ch == ';' ) { 
    separator = ' ' ;
    ch = cin.get() ;
    while ( ch != '\n' && !cin.eof() ) { // 讀掉註解後的所有東西 
      ch = cin.get() ;
    }
  } // if()
} // Get_token() 
void Cross_out() { // 劃掉錯誤後的所有東西 
    char ch = separator ;
    separator = ' ' ; 
    while ( ch != '\n' ) {
      ch = cin.get() ;
      cout << "X" ;
    } 
}
int Token_type ( string&str ) {   
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
    str[0] = '\0' ;
    str[str.length()-1] = '\0' ;
    return 4 ;
  }
  else if ( str == "nil" || str == "#f" ) {
    str = "nil" ;
    return 7 ;
  }
  else if ( str == "t" || str == "#t" ) {
    str = "#t" ;
    return 8 ;
  }
  else if ( str == "(" ) {
    separator = cin.get() ;
    while ( isspace(separator) ) {
      separator = cin.get() ;
    }
    if ( separator == ')' ) {
      str = "nil" ;
      separator = ' ' ;
      return 7 ;
    }
    else return 1 ;
  }
  else if ( str == "'" ) {
    str = "quote" ;
    return 9 ;
  }
  else if ( str == "." ) return 5 ;
  else if ( str == ")" ) return 2 ;
  
  int dot = 0, plus = 0, minus = 0 ;
  bool isNum = true ;
  for ( int i = 0 ; i < str.length() ; i++ ) {
    if ( str[i] == '.' ) dot++ ;
    else if ( str[i] == '+' ) plus++ ;
    else if ( str[i] == '-' ) minus++ ;
    else if ( str[i] < 48 || str[i] > 57 ) isNum = false ;
  }
  if ( isNum && ( ( plus == 1 && minus == 0 ) || ( plus == 0 && minus == 1 ) || ( plus == 0 && plus == 0 ) ) && ( dot == 0 || dot == 1 ) ) {
    if ( dot == 1 ) return 6 ;
    else return 3 ;
  }
  else return 10 ;
  
}
bool S_expression( string&str ) { // 判斷文法 
  string token ;
  bool str_error ; 
  int token_type = Token_type(str) ; 
  if ( token_type == 3 || token_type == 4 || token_type == 6 || token_type == 7 || token_type == 8 || token_type == 10  ) {
    return true ;
  }
  else if ( token_type == 1 ) { // LP
    token = Get_token(str_error) ;
    while( token == "" ) token = Get_token( str_error ) ; // 讀到有東西為止 
    if ( str_error || !S_expression(token) ) {
      return false ;
    }
    else {
      token = Get_token(str_error) ;
      while( token == "" ) token = Get_token( str_error ) ;
      while ( token != "." && token != ")"  )  {
        if ( str_error || !S_expression(token) ) return false ;
        token = Get_token(str_error) ;
      }
      
      if ( token == ")" ) {
        return true ;
      }
      else if ( token == "." ) {
        token = Get_token(str_error) ;
        while( token == "" ) token = Get_token( str_error ) ;
        if ( !str_error && S_expression(token) ) {
          token = Get_token(str_error) ;
          while( token == "" ) token = Get_token( str_error ) ;
          if ( token != ")" ) {
            Cross_out() ;
            return false ;
          }
          else {
            return true ;
          }
        }
        else {
          return false ;
        }
      }
    }

  }
  else if ( token_type == 9 ) { // quote 
    token = Get_token(str_error) ;
    while( token == "" ) token = Get_token( str_error ) ;
    if ( str_error || !S_expression(token) ) return false ;
    else return true ;
  }
  else { // DOT RP 
    Cross_out() ;
    return false ;
  }
} // S_expression()
