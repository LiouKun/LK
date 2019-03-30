# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;
string Get_token( bool& ) ;
int Token_type( string&str ) ;
bool S_expression( string&str, vector<string>&all_tokens ) ; // paresr
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
int error_line = 1, error_column = 0 ;
bool enter = false ;
bool command_end = false ;
int count_mode = 0 ;
int main() {
  vector<string> all_tokens ;
  bool str_error ; // ¦r¦ê¿ù»~¡A¦b¦r¦ê¤º´«¦æªº¿ù»~ 
  string token = " " ; 
  while ( !cin.eof() ) {
    token = Get_token( str_error ) ;
    while( token == "" ) token = Get_token( str_error ) ; // ¤@©w­nget¨ì¤@­Ótoken
    command_end = false ;
    if ( !str_error && S_expression( token, all_tokens ) ) { 
      cout << "true\n" ;  // ¨S¦³¥ô¦ó¿ù»~
      int i = 0, space = 0 ;
      Fix_all_tokens( all_tokens ) ;
      PrettyPrint( all_tokens, i, space ) ;
    } // if()
    else {
      cout << "error  " << "LINE: " << error_line << "COLUMN: "  << error_column << "\n" ;
    } // else()
    error_line = 1 ;
    error_column = 0 ;
    all_tokens.clear() ;
    command_end = true ;
    if ( count_mode == 1 ) {
      if ( cin.peek() == '\n' ) char a = cin.get() ;
    }
    count_mode = 0 ;
  } // while()
} // main()
void GetChar( char&ch ) {
  ch = cin.get() ;
  if ( count_mode == 1 ) { // ¥ª¬A¸¹¶}ÀYªºªºcase ·|¥H¥k¬A¸¹µ²§ô 
    if ( ch == '\n' && !command_end ) {
      error_line++ ;
      error_column = 0 ;
    }
    else if ( ch == '\n' && command_end ) {
      error_line = 1 ;
      error_column = 0 ;
    }
    else {
      error_column++ ;
    }
  }
  else if ( count_mode == 2 ) { // «D¥ª¬A¸¹¶}ÀYªºcase ·|¥H´«¦æ¦r¤¸µ²§ô 
    if ( ch == '\n' ) enter = true ; // ²{¦b¹J¨ì´«¦æ¤F! ¦ý¼Æ¦r¥ý¤£°Ê¡A¤U¦¸getchar¦A°Ê 
    else if ( enter == true ) { // ­è­èªºchar¬O´«¦æ¡A³oÃä¸Ó°Ê¼Æ¦r¤F 
      enter = false ;
      error_line = 1 ; 
      error_column = 0 ;
      if ( cin.peek() == '\n' ) { // ³sÄò¨â­Ó´«¦æ! (2.0) (3,0) (4,0)... 
        ch = cin.get() ;
        while ( ch == '\n' ) {
          error_line++ ;
          ch = cin.get() ;
        }
      }
    }
    else {
      error_column++ ;
    }
  }
  else if ( count_mode == 0 ) {
    if ( ch == '\n' ) {
      error_line++ ;
    }
    else if ( ch == '(' ) {
      count_mode = 1 ;
      error_column++ ;
    }
    else {
      count_mode = 2 ;
      error_column++ ;
    }
  }
  // cout << "x: " << error_line << " y: " << error_column << "\n" ;
} // GetChar()
string Get_token( bool&str_error ) { // ¦^¶Ç¤@­Ótoken¦^¥h 
  str_error = false ; // ¦r¦ê¿ù»~ªì©l¬°false 
  string token ;
  char ch ;
  ch = separator ; // ±q¨º­Óseparator¦r¤¸¶}©l§PÂ_ 
  while ( isspace(ch) && !cin.eof() ) { 
  // ¸õ¹L©Ò¦³white-spaces¡Bª½¨ì¸I¨ì¡u«Dwhite-space¡v(©ÎEND-OF-FILE)¬°¤î
    GetChar(ch) ;
  } // while()
  while ( !isspace(ch) && ch != '(' && ch != ')' && ch != '\'' && ch != '"' && ch != ';' ) {
  // ±q¨º­Ó¡u«Dwhite-space¡v¶}©l "Ä~ÄòÅª¤J"¡Bª½¨ì¸I¨ì¡useparator-¦r¤¸¡v¬°¤î
    token = token + ch ;
    GetChar(ch) ;
  } // while()
  if ( token != "" ) { 
    separator = ch ;
    return token ;
  } // if() 
  
  // ¥H¤U¶}©l³B²z¤ÀÂ÷¦r¤¸ 
  if ( ch == '(' || ch == ')' || ch == '\'' ) {
    // ¸I¨ì³o¤T­Ó Â²³æ ª½±µreturn¦^¥h 
    separator = ' ' ;
    token = token + ch ;
    return token ;
  } // if() 
  else if ( ch == '"' ) { // ¦r¦êªº³B²z 
    separator = ' ' ;
    token = token + ch ;
    GetChar(ch) ;
    while ( ch != '"' ) {
      if ( ch == '\n' ) { // ¦r¦ê³B²zªºerror¦b³o¸Ì´N¥i¥H³B²z
        str_error = true ; // ¦r¦êerror
        token = "error" ;
        return token ;
      } // if()
      else if ( ch == '\\' ) { // ¸I¨ì'\' -> escape¦r¤¸³B²z 
        char back_slash = ch ; 
        GetChar(ch) ;
        if ( ch == 't' ) token = token + '\t' ;
        else if ( ch == '\\' ) token = token + '\\' ;
        else if ( ch == 'n' ) token = token + '\n' ;
        else if ( ch == '"' ) token = token + '\"' ;
        else token = token + back_slash ;
      } // else if()
      else  token = token + ch ;
      GetChar(ch) ;
      if ( ch == '"' ) token = token + ch ;
    } // while() 
  } // else if()
  else if ( ch == ';' ) { 
    separator = ' ' ;
    GetChar(ch) ;
    while ( ch != '\n' && !cin.eof() ) { // Åª±¼µù¸Ñ«áªº©Ò¦³ªF¦è 
      GetChar(ch) ;
    } // while()
  } // else if()
} // Get_token() 
void Cross_out() { // ¹º±¼¿ù»~«áªº©Ò¦³ªF¦è 
    char ch = separator ;
    separator = ' ' ; 
    while ( ch != '\n' ) {
      ch = cin.get() ;
      cout << "X" ;
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
    GetChar(separator) ;
    while ( isspace(separator) ) {
      GetChar(separator) ;
    } // while()
    if ( separator == ')' ) {
      str = "nil" ;
      separator = ' ' ;
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
bool S_expression( string&str, vector<string>&all_tokens ) { // §PÂ_¤åªk 
  string token ; 
  bool str_error ; //  ¦r¦êerror 
  int token_type = Token_type(str) ; 
  if ( token_type == 3 || token_type == 4 || token_type == 6 || token_type == 7 || token_type == 8 || token_type == 10  ) {
    all_tokens.push_back(str) ;
    return true ;
  } // if()
  else if ( token_type == 1 ) { // LP
    all_tokens.push_back(str) ;
    token = Get_token(str_error) ;
    while( token == "" ) token = Get_token( str_error ) ; // Åª¨ì¦³ªF¦è¬°¤î 
    if ( str_error || !S_expression(token, all_tokens) ) {
      return false ;
    } // if()
    else {
      token = Get_token(str_error) ;
      while( token == "" ) token = Get_token( str_error ) ;
      while ( token != "." && token != ")"  )  {
        if ( str_error || !S_expression(token, all_tokens) ) return false ;
        token = Get_token(str_error) ;
        while( token == "" ) token = Get_token( str_error ) ;
      }
      
      if ( token == ")" ) {
        all_tokens.push_back(token) ;
        return true ;
      } // if()
      else if ( token == "." ) {
        all_tokens.push_back(token) ;
        token = Get_token(str_error) ;
        while( token == "" ) token = Get_token( str_error ) ;
        if ( !str_error && S_expression(token, all_tokens) ) {
          token = Get_token(str_error) ;
          while( token == "" ) token = Get_token( str_error ) ;
          if ( token != ")" ) {
            Cross_out() ;
            return false ;
          } // if()
          else {
            all_tokens.push_back(token) ;
            return true ;
          }
        }  // if()
        else {
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
    if ( str_error || !S_expression(token, all_tokens) ) return false ;
    else {
      all_tokens.push_back(")") ;
      return true ;
    } // else
  } // else if()
  else { // DOT RP 
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
