// �T���tab 
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <stdlib.h>
# include <iomanip>
using namespace std;

void Get_token( string command_line, vector<string>& token_group ) ;
// ���Xtoken�A�^��vector<string>�A�̭��񪺬Otoken 
int Token_type( string token ) ;
// �P�_token�����A�ñN�P�q�r�p #t t �� nil () #f �B�z�令�Τ@�r 
                             
int main() {
  char ch ;
  int lp = 0, rp = 0 ; // ���A�����ƶq �k�A�����ƶq 
  string command_line ; // �R�O���H 
  vector<string> token_group ; // �Q���ͪ��R�O = token 
  ch = cin.get() ;
  while ( !cin.eof() ) {
    if ( ch != '\n' ) command_line = command_line + ch ; // ���⴫��s�Jcommand_line 
    // �B�z�A�� �}�l! 
    if ( ch == '(' ) {
      lp++ ; 
      ch = cin.get() ; // �n��cin.get�~�౵����(����)(�ť�) 
      command_line = command_line + ch ;
      if ( ch == ')' ) lp-- ; // >>()<<  ���k�A���۳s�N��: nil  
      if ( ch == '(' ) lp++ ; // >>((<<  �o�Ӧr���]�O���A�� ��M�]�n��i�� 
    } // if()
    else if ( ch == ')' ) rp++ ;
    // �B�z�A�� ���� 
    
    if ( ( lp == rp ) && lp != 0  ) {
      cout << lp << rp << "\n" ;
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


// �J��" �A �N��᭱�����r��(���n���ť�)�A���p�G�b�I��U��"�r���e�A�N�J�W\n�F�A�h�L�Xerror
// �A���n����Ҧ���٪��A���~�൲���A�Ӥ��O�J�촫��N����

// ��str.at�Ӭݦr��̭����F��Afor( int i = 0 ; i < str.length ; i++ )

//  ' �n��X�� quote

// �i�Jget_token()�����case 
// case1 : Ū�촫��
// case2 : ��lp rp ��١A�B�Alp rp���O�s�b�@�_�h�i�Jget_token()
