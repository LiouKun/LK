// 禁止用tab 
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;

int main() {
  char ch ;
  int lp = 0, rp = 0 ; // 左括號的數量 右括號的數量 
  string command_line ; // 命令本人 
  vector<string> token_group ; // 被分屍的命令 = token 
  ch = cin.get() ;
  while ( !cin.eof() ) {
    if ( ch != ';' ) command_line = command_line + ch ;
    // 處理括號 開始! 
    if ( ch == '(' ) {
      lp++ ; 
      ch = cin.get() ; // 要用cin.get才能接受到(換行)(空白) 
      command_line = command_line + ch ;
      if ( ch == ')' ) lp-- ; // >>()<<  左右括號相連代表: nil  
      if ( ch == '(' ) lp++ ; // >>((<<  這個字元也是左括號 當然也要算進來 
    } // if()
    else if ( ch == ')' ) rp++ ;
    // 處理括號 結束 

    if ( ( lp == rp ) && lp != 0  ) {
      lp = 0 ;
      rp = 0 ;
      cout << "into Get_token \n" ;
      cout << "commandline: " << command_line << "\n" ;
      ch = cin.get() ;
      command_line = "" ;
      if ( ch == '\n' ) command_line = "" ;
      else command_line = command_line + ch ;
    } // if()
    else if ( ( ch == '\n' && lp == 0 && rp == 0 ) || ( ch == ';' && command_line != "" ) ) {
      lp = 0 ;
      rp = 0 ;
      cout << "into Get_token \n" ; 
      cout << "commandline: " << command_line << "\n" ;
      command_line = "" ;
      if ( ch == ';' ) {
        ch = cin.get() ;
        while ( ch != '\n' ) {
          ch = cin.get() ;
        } 
      }
    } // else if()
    ch = cin.get() ; 
  } // while()
} // main()
