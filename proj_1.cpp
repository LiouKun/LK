// 禁止用tab 
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;
//////

void Get_token( string command_line, vector<string>& token_group ) ;
// 切出token，回傳vector<string>，裡面放的是token 
int Token_type( string token ) ;
// 判斷token類型，並將同義字如 #t t 或 nil () #f 處理改成統一字 
                             
int main() {
  char ch ;
  int lp = 0, rp = 0 ; // 左括號的數量 右括號的數量 
  string command_line ; // 命令本人 
  vector<string> token_group ; // 被分屍的命令 = token 
  ch = cin.get() ;
  while ( !cin.eof() ) {
    if ( ch != '\n' ) command_line = command_line + ch ; // 不把換行存入command_line 
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
    else if ( ch == '\n' && lp == 0 && rp == 0 ) {
      lp = 0 ;
      rp = 0 ;
      cout << "into Get_token \n" ; 
      cout << "commandline: " << command_line << "\n" ;
      command_line = "" ;
    } // else if()

    ch = cin.get() ;
  } // while()

} // main()


// 遇到" ， 就把後面視為字串(不要跳空白)，但如果在碰到下個"字元前，就遇上\n了，則印出error
// 括號要收到所有對稱的括號才能結束，而不是遇到換行就結束

// 用str.at來看字串裡面的東西，for( int i = 0 ; i < str.length ; i++ )

//  ' 要輸出成 quote

// 進入get_token()的兩種case 
// case1 : 讀到換行
// case2 : 當lp rp 對稱，且，lp rp不是連在一起則進入get_token()
