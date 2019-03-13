# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;
string Get_token() ;
int Token_type( string&str ) ;
bool S_expression( string str ) ;
char separator = '\n' ;
 
int main() {
  string token ;
  while ( !cin.eof() ) {
    token = Get_token();
    cout << Token_type(Get_token()) << "\n" ;
  } // while()
} // main()

string Get_token() { // ¦^¶Ç¤@­Ótoken¦^¥h 
  string token ;
  char ch ;
  ch = separator ;
  while ( isspace(ch) && !cin.eof() ) { // ¸õ¹L©Ò¦³white-spaces¡Bª½¨ì¸I¨ì¡u«Dwhite-space¡v(©ÎEND-OF-FILE)¬°¤î
    ch = cin.get() ;
  } // while()
  while ( !isspace(ch) && ch != '(' && ch != ')' && ch != '\'' && ch != '"' && ch != ';' ) {
  // ±q¨º­Ó¡u«Dwhite-space¡v¶}©l "Ä~ÄòÅª¤J"¡Bª½¨ì¸I¨ì¡useparator-¦r¤¸¡v¬°¤î
    token = token + ch ;
    ch = cin.get() ;
  } // while()
  if ( token != "" ) {
    separator = ch ;
    return token ;
  } // if() 
  
  if ( ch == '(' || ch == ')' || ch == '\'' ) {
    separator = ' ' ;
    token = token + ch ;
    return token ;
  } // if() 
  else if ( ch == '"' ) { // 
    separator = ' ' ;
    token = token + ch ;
    ch = cin.get() ;
    while ( ch != '"' ) {
      if ( ch == '\\' ) {
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
      if ( ch == '\n' ) { // ¦r¦ê³B²zªºerror¦b³o¸Ì´N¥i¥H³B²z
        cout << "error\n" ;
        ch = cin.get() ;
        while ( ch != '\n' ) {
          ch = cin.get() ;
        }
      }
      if ( ch == '"' ) token = token + ch ;
    } // while() 
  } // if()
  else if ( ch == ';' ) {
    separator = ' ' ;
    ch = cin.get() ;
    while ( ch != '\n' ) {
      ch = cin.get() ;
    }
  } // if()
} // Get_token() 

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
    str == "#t" ;
    return 8 ;
  }
  else if ( str == "(" ) {
    separator = cin.get() ;
    if ( separator == ')' ) {
      str == "nil" ;
      separator = ' ' ;
      return 7 ;
    }
    else return 1 ;
  }
  else if ( str == "'" ) {
    str == "quote" ;
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

bool S_expression( string&str ) {
  string token ;
  int token_type = Token_type(str) ;
  if ( token_type == 3 || token_type == 4 || token_type == 5 || token_type == 6 || token_type == 7 || token_type == 8 || token_type == 10  ) {
    return true ;
  } 
  else if ( token_type == 1 ) {
    token = Get_token();
    
  }
}
