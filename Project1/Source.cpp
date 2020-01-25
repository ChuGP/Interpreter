# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <cstdlib>
# include <iostream>
# include <vector>
# include <iomanip>




using namespace std;



struct Id_Data {
  string mid_ofName;
  double mid_ofValue; // ���ƥHdouble���O�h�s
  int mid_Oftype; // ���O
  bool muse = false; // �O�_�x�s���T���έ�
};

struct Tokeninformation {
  string mgpToken_data;
  int mgpToken_type; // ���O
  bool muse = false; // �O�_�P��k����(����)
};



static int uThreespace = 1; // �ť�tab \0
static int uUpdown = 2; // �[��
static int uMul_Divide = 3; // ����
static int uSemicolon = 4;  // ��������
static int uBrackets_L = 5; // ���A��
static int uBrackets_R = 6; // �k�A��
static int uDefine = 7; // ��l�w�q :=
static int uNo_useobj = 8;
static int uComment_line = 9; // ������
static int uComment_block = 10; // �϶����� �Ҧp"/**/"
static int uGp_ID = 11; // �r��A-Z �� A_
static int uGp_Int = 12; // ��ƼƦr 
static int uGp_Float = 13; // �B�I�ƼƦr 
static int uEnd_input = 14; // quit
static int uBool_ope = 15; // >= <= > < 
static int uEqual = 16; // =
static int uNomean = 17; // ��L�D�W�w�ӥX�{��

// 3��error
static int uNrecognized = 1;
static int uNexpected = 2;
static int uNdefined = 3;

Tokeninformation gNow_tokINfo;
Tokeninformation gNext_tokINfo;
vector<Id_Data> gTokenplace; // �sID

bool gError = false; // ������@��error
bool gUnrecognized = false;
bool gUndefined = false;
bool gUnexpected = false;
bool gDivide_zero = false; // ��¿�Xothererror
bool gFloat_num = false;
bool gInt = false;

double gNum_double = 0.0;
int gNum_int = 0;

void Skipline();
bool Garbageskip();
void GetTokenButton();
void Dealtoken();
void Gettoken();
void PeekTokenButton();
void Get_nextToken();
void Clear_now_next( bool now_next );
void Message_error();
void Print_error( int &gp );
void ArithExp(double &temp);

void Factor(double &num );
void Term(double &temp);

bool Deal_LME( string &gp, double &ans, double &temp, bool &flag );
void Printf_true_false( bool &flag );
void Test_Non_appear( bool &gp );
bool Test_In_Arith();
void ThreeERROR( bool &stop );
bool Command();
void Resset();

int main() {
  
  string testNum;
  cin >> testNum;
  cout << "Program starts..." << "\n";
  cout << "> ";
  bool succeed = false;
  while ( succeed == false ) {
    Garbageskip();

    succeed = Command();

    gNow_tokINfo.mgpToken_data.clear();
    gNext_tokINfo.mgpToken_data.clear();
    Resset();
  } // while

  cout << "Program exits..." << "\n";
  system( "pause" );
  
  /*
  while ( gNow_tokINfo.mgpToken_data != ";" ) {
    GetTokenButton();
    cout << gNow_tokINfo.mgpToken_data << endl;
    // cout << gNow_tokINfo.mgpToken_type;
  }
  
  system( "pause" );
  */

} // main()

// �H�U���p�u��

void Resset() {
  gDivide_zero = false;
  gError = false;
  gUnexpected = false;
  gUnrecognized = false;
  gUndefined = false;
  gFloat_num = false;
  gInt = false;
  gNum_double = 0.0;
  gNum_int = 0;
} // Resset()
void Skipline() {
  // cout << "in";
  char gp = cin.peek();
  while ( gp != '\n' ) {
    gp = cin.get();
    gp = cin.peek();
  } // while

  gp = cin.get();  // Ū������ 
  
} // Skipline()

bool Garbageskip() {
  bool out = false;
  bool succeed = false; // �ݦ��S�����\Ū���U�� 
  char test = cin.peek();

  while ( out == false ) {
    succeed = false;
    if ( test == ' ' ) {
      test = cin.get();
      test = cin.peek();
    } // if
    else if ( test == '\t' ) {
      test = cin.get();
      test = cin.peek();
    } // else if
    else if ( test == '\n' ) {
      test = cin.get();
      test = cin.peek();
    } // else if
    else {
      succeed = true; // �n��X��
      out = true; // ���X�j���
    } // else

  } // while

  
  return succeed; // �T�{�O�_���\Ū���U��
} // Garbageskip()

void SkipWhitespace( char &gp ) {
  // ���C����������Ӧn��
  //�ϥ�gp����temp�Ȼ~��

  bool succeed = false;
  int which = 0; // ����debug��

  while ( succeed == false ) {
    gNow_tokINfo.mgpToken_type = uThreespace;
    if ( gp == ' ' ) {
      which = 1;
      gp = cin.get();
      gp = cin.peek();

    } // if
    else if ( gp == '\t' ) {
      which = 2;
      gp = cin.get();
      gp = cin.peek();
    } // else if
    else if ( gp == '\0' ) {
      which = 3;
      gp = cin.get();
      gp = cin.peek();
    } // else if
    else succeed = true;
    // �䤣��i����if������Xwhich�Ӭ�

  } // while

  // cout << gp;
} // SkipWhitespace()

void Dealtoken() {
  // �M���t�d����next��now
  bool test = false;
  if ( gNext_tokINfo.mgpToken_data.empty() == false )
    test = true; // Nexttoken�����ťN��n���ʨéI�sclearnext

  gNow_tokINfo = gNext_tokINfo;
  Clear_now_next( test );

} // Dealtoken()

bool Test_ID_not( char &gp ) {
  // ���լO�_��ID
  bool ans = false;
  if ( ( gp >= 'a'&& gp <= 'z' ) ) {
    ans = true;    // �p�g
  } // if
  else if ( gp >= 'A'&& gp <= 'Z' ) {
    ans = true; // �j�g
  } // else if
  else ans = false; // �S�iif�N�O����˫O�I�_���A�]�@��false
  return ans;

} // Test_ID_not()

bool Test_number_not( char &gp ) {
  // ���լO�_���Ʀr
  bool ans = false;
  if ( gp >= '0' && gp <= '9' ) {
    ans = true;
  } // if

  return ans;
} // Test_number_not()

void Clear_now_next( bool now_next ) {
  // �t�d�Mnow next token
  if ( now_next == true ) {
    gNext_tokINfo.mgpToken_data.clear(); // �p�Gnext_token���F��n�M��
  } // if
  else {
    gNow_tokINfo.mgpToken_data.clear(); // �p�Gnow_token���F��n�M��
  } // else

} // Clear_now_next()

void Message_error() {
  int error_type = 0; // debug������
  if ( gUnrecognized == true ) {
    
    gError = true;
    Skipline(); // ��Ӧ�Ū��
    error_type = 1;
    Print_error( error_type );
    cout << "> "; // ����
  } // if
  else if ( gUnexpected == true ) {
    gError = true;
    Skipline(); // ��Ӧ�Ū��
    error_type = 2;
    Print_error( error_type );
    cout << "> "; // ����
  } // else if
  else if ( gUndefined == true ) {
    gError = true;
    Skipline(); // ��Ӧ�Ū��
    error_type = 3;
    Print_error( error_type );
    cout << "> "; // ����
  } // else if
  
  else {
    gError = true;
    Skipline(); // ��Lerror /0������
    error_type = 4;
    Print_error( error_type );
    cout << "> "; // ����
  } // else



} // Message_error()

void Print_error( int &gp ) {
  Tokeninformation temp = gNow_tokINfo;
  //���@��temp�Ӧs�������ϥΥ���ȭק��

  if ( gp == 1 ) {
    
    cout << "Unrecognized token with first char : " << "'";
    cout << temp.mgpToken_data;
    cout << "'" << "\n";
  } // if
  else if ( gp == 2 ) {
    cout << "Unexpected token : " << "'";
    cout << temp.mgpToken_data;
    cout << "'" << "\n";
  } // else if
  else if ( gp == 3 ) {
    cout << "Undefined identifier : " << "'";
    cout << temp.mgpToken_data;
    cout << "'" << "\n";
  } // else if
  else if ( gp == 4 ) {
    cout << "Error";
    cout << "\n";
  } // else if

} // Print_error()

void Set_Error( int& num ) {
  // �]3��error
  if ( num == 1 ) {
    
    gUnrecognized = true;
    GetTokenButton();
    Message_error();
    return;
  } // if
  else if ( num == 2 ) {
    gUnexpected = true;
    GetTokenButton();
    Message_error();
    return;
  } // else if
  else if ( num == 3 ) {
    gUndefined = true;
    GetTokenButton();
    Message_error();
    return;
  } // else if

} // Set_Error()

// ===============================================================
//�H�U��h if����P�_�ɶq���n�@���������bug

void GetTokenButton() {
  // �]�˦n��function ��next�N���ʨ�now�S���N����get�@�Ө�now
  gNow_tokINfo.mgpToken_data.clear();
  bool test = gNext_tokINfo.mgpToken_data.empty();
  if ( test == false ) {                 // �Ψӷ�getToken���}��
    Dealtoken(); //�I�s�M������next��now�����
    // cout << "���\";
  } // if
  else {
    Gettoken(); // ��token

  } // else
} // GetTokenButton()



void Gettoken() {

  bool test = false; // ���ڪ�Garbageskip
  test = Garbageskip(); // �⤣�n���r��Ū��
  bool run = false;
  char temp = cin.get(); // �s�@��char
  char skip = cin.peek();
  SkipWhitespace( skip ); // ���L�ť�3�S��
  char next_temp = cin.peek(); // ����temp���U�@��

  if ( temp == ';' ) {
    gNow_tokINfo.mgpToken_type = uSemicolon; // ����type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // if
  else if ( temp == '(' ) {
    gNow_tokINfo.mgpToken_type = uBrackets_L; // �A��type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == ')' ) {
    gNow_tokINfo.mgpToken_type = uBrackets_R; // �A��type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == '+' ) {
    gNow_tokINfo.mgpToken_type = uUpdown; // + -type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == '-' ) {
    gNow_tokINfo.mgpToken_type = uUpdown; // + -type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == ':' && next_temp == '=' ) {

    gNow_tokINfo.mgpToken_data = temp;
    next_temp = cin.get();
    gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next_temp;
    gNow_tokINfo.mgpToken_type = uDefine;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == ':' && next_temp != '=' ) {
    // ��� ':'
    gNow_tokINfo.mgpToken_type = uNomean; // ��':'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = false; // �P��k�L��!!!!!!!!
    run = true;
  } // else if
  else if ( temp == '/' && next_temp == '*' ) {
    gNow_tokINfo.muse = false; // �P��k�L��!!!!!!!!�¹L�o
    next_temp = cin.get();

    bool succeed = false;
    while ( succeed == false ) {
      temp = cin.get(); // �s�@��char
      skip = cin.peek();
      SkipWhitespace( skip ); // ���L�ť�3�S��
      char next_temp = cin.peek(); // ����temp���U�@��        
      if ( temp == '*' && next_temp == '/' ) {
        next_temp = cin.get();
        succeed = true;
      } // if
      else succeed = false;

    } // while     

    gNow_tokINfo.mgpToken_type = uComment_block; // ���Ѱ϶�
    GetTokenButton(); //���j�]

    return;

  } // else if
  else if ( temp == '/' && next_temp == '/' ) {
    gNow_tokINfo.muse = false; // �P��k�L��!!!!!!!!�¹L�o
    next_temp = cin.get();
    Skipline();
    gNow_tokINfo.mgpToken_type = uComment_line; // ���Ѥ@��
    GetTokenButton(); //���j�]

    return;
  } // else if
  else if ( temp == '/' && next_temp != '/' && next_temp != '*' ) {
    gNow_tokINfo.mgpToken_type = uMul_Divide; // ��'/'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == '*' ) {
    gNow_tokINfo.mgpToken_type = uMul_Divide; // ��'*'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( temp == '<' || temp == '>' ) {
    gNow_tokINfo.muse = true; // �P��k����
    gNow_tokINfo.mgpToken_type = uBool_ope;
    char next = cin.peek();
    if ( temp == '<' && next == '>' ) {
      gNow_tokINfo.mgpToken_data = temp;
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      return;
    } // if
    else if ( temp == '<' && next == '=' ) {
      gNow_tokINfo.mgpToken_data = temp;
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      return;
    } // else if
    else if ( temp == '>' && next == '=' ) {
      gNow_tokINfo.mgpToken_data = temp;
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      return;
    } // else if
    else {
      gNow_tokINfo.mgpToken_data = temp; // ��� > <
      return;
    } // else

  } // else if
  else if ( temp == '=' ) {
    gNow_tokINfo.mgpToken_type = uEqual; // ��'='
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // �P��k����
    return;
  } // else if
  else if ( Test_ID_not( temp ) == true ) {
    gNow_tokINfo.muse = true; // �P��k����
    gNow_tokINfo.mgpToken_data = temp;

    while ( Test_ID_not( next_temp ) == true || next_temp == '_' || Test_number_not( next_temp ) == true ) {
      next_temp = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next_temp;
      next_temp = cin.peek();
      //�]���o�짹��id
    } // while

    gNow_tokINfo.mgpToken_type = uGp_ID; // �]�����]��ID type

    if ( gNow_tokINfo.mgpToken_data == "quit" )
      gNow_tokINfo.mgpToken_type = uEnd_input; // �S���p�J��quit�n��type

    return;

  } // else if
  else if ( temp == '.' ) {
    gNow_tokINfo.muse = true; // �P��k����
    gNow_tokINfo.mgpToken_data = temp;
    char next = cin.peek();
    while ( Test_number_not( next ) == true ) {
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      next = cin.peek();
    } // while

    gNow_tokINfo.mgpToken_type = uGp_Float;
    return;
  } // else if
  else if ( Test_number_not( temp ) == true ) {
    gNow_tokINfo.muse = true; // �P��k����

    gNow_tokINfo.mgpToken_data = temp;
    bool float_point = false; // �O�_���B�I��
    char next = cin.peek();
    int in_while = 0; // �u��Ū��@��.
    while (  Test_number_not( next ) == true || ( next == '.'  && in_while < 1 ) ) {
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      if ( next == '.' ) {
        in_while++;
        float_point = true;
      } // if
      next = cin.peek();

    } // while

    gNow_tokINfo.mgpToken_type = uGp_Int; // ��l�]��int

    if ( float_point == true )
      gNow_tokINfo.mgpToken_type = uGp_Float; // ��dot�N�]��float

    return;

  } // else if
  else {
    gNow_tokINfo.muse = false; // �M��k�L��
    gNow_tokINfo.mgpToken_type = uNomean;  // ��l���S�N�q
    gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + temp;

    return;
  } // else




} // Gettoken()


void PeekTokenButton() {
  // ���󤺤��n�@��
  bool test = gNext_tokINfo.mgpToken_data.empty();
  if ( test == true )
    Get_nextToken();    // PeekToken �[�P�_����
} // PeekTokenButton()

void Get_nextToken() {
  // ��token���next
  bool empty_ornot = gNext_tokINfo.mgpToken_data.empty();  // ����true false
  if ( empty_ornot == true ) {
    bool test = false; // ���ڪ�Garbageskip
    test = Garbageskip(); // �⤣�n���r��Ū��
    bool run = false;
    char temp = cin.get(); // �s�@��char
    char skip = cin.peek();
    SkipWhitespace( skip ); // ���L�ť�3�S��
    char next_temp = cin.peek(); // ����temp���U�@��

    if ( temp == ';' ) {
      gNext_tokINfo.mgpToken_type = uSemicolon; // ����type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // if
    else if ( temp == '(' ) {
      gNext_tokINfo.mgpToken_type = uBrackets_L; // �A��type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == ')' ) {
      gNext_tokINfo.mgpToken_type = uBrackets_R; // �A��type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == '+' ) {
      gNext_tokINfo.mgpToken_type = uUpdown; // + -type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == '-' ) {
      gNext_tokINfo.mgpToken_type = uUpdown; // + -type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == ':' && next_temp == '=' ) {

      gNext_tokINfo.mgpToken_data = temp;
      next_temp = cin.get();
      gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next_temp;
      gNext_tokINfo.mgpToken_type = uDefine;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == ':' && next_temp != '=' ) {
      // ��� ':'
      gNext_tokINfo.mgpToken_type = uNomean; // ��':'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = false; // �P��k�L��!!!!!!!!
      run = true;
    } // else if
    else if ( temp == '/' && next_temp == '*' ) {
      gNext_tokINfo.muse = false; // �P��k�L��!!!!!!!!�¹L�o
      next_temp = cin.get();

      bool succeed = false;
      while ( succeed == false ) {
        temp = cin.get(); // �s�@��char
        skip = cin.peek();
        SkipWhitespace( skip ); // ���L�ť�3�S��
        char next_temp = cin.peek(); // ����temp���U�@��        
        if ( temp == '*' && next_temp == '/' ) {
          next_temp = cin.get();
          succeed = true;
        } // if
        else succeed = false;

      } // while     

      gNext_tokINfo.mgpToken_type = uComment_block; // ���Ѱ϶�
      PeekTokenButton();//���j�]

      return;

    } // else if
    else if ( temp == '/' && next_temp == '/' ) {
      gNext_tokINfo.muse = false; // �P��k�L��!!!!!!!!�¹L�o
      next_temp = cin.get();
      Skipline();
      gNext_tokINfo.mgpToken_type = uComment_line; // ���Ѥ@��
      PeekTokenButton(); //���j�]

      return;
    } // else if
    else if ( temp == '/' && next_temp != '/' && next_temp != '*' ) {
      gNext_tokINfo.mgpToken_type = uMul_Divide; // ��'/'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == '*' ) {
      gNext_tokINfo.mgpToken_type = uMul_Divide; // ��'*'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( temp == '<' || temp == '>' ) {
      gNext_tokINfo.muse = true; // �P��k����
      gNext_tokINfo.mgpToken_type = uBool_ope;
      char next = cin.peek();
      if ( temp == '<' && next == '>' ) {
        gNext_tokINfo.mgpToken_data = temp;
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        return;
      } // if
      else if ( temp == '<' && next == '=' ) {
        gNext_tokINfo.mgpToken_data = temp;
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        return;
      } // else if
      else if ( temp == '>' && next == '=' ) {
        gNext_tokINfo.mgpToken_data = temp;
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        return;
      } // else if
      else {
        gNext_tokINfo.mgpToken_data = temp; // ��� > <
        return;
      } // else

    } // else if
    else if ( temp == '=' ) {
      gNext_tokINfo.mgpToken_type = uEqual; // ��'='
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // �P��k����
      return;
    } // else if
    else if ( Test_ID_not( temp ) == true ) {
      gNext_tokINfo.muse = true; // �P��k����
      gNext_tokINfo.mgpToken_data = temp;

      while ( Test_ID_not( next_temp ) == true || next_temp == '_' || Test_number_not( next_temp ) == true ) {
        next_temp = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next_temp;
        next_temp = cin.peek();
        //�]���o�짹��id
      } // while

      gNext_tokINfo.mgpToken_type = uGp_ID; // �]�����]��ID type

      if ( gNext_tokINfo.mgpToken_data == "quit" )
        gNext_tokINfo.mgpToken_type = uEnd_input; // �S���p�J��quit�n��type

      return;

    } // else if
    else if ( temp == '.' ) {
      gNext_tokINfo.muse = true; // �P��k����
      gNext_tokINfo.mgpToken_data = temp;
      char next = cin.peek();
      while ( Test_number_not( next ) == true ) {
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        next = cin.peek();
      } // while

      gNext_tokINfo.mgpToken_type = uGp_Float;
      return;
    } // else if
    else if ( Test_number_not( temp ) == true ) {
      gNext_tokINfo.muse = true; // �P��k����

      gNext_tokINfo.mgpToken_data = temp;
      bool float_point = false; // �O�_���B�I��
      char next = cin.peek();
      int in_while = 0; // �u��Ū��@��.
      while ( Test_number_not( next ) == true || ( next == '.'   && in_while < 1 ) ) {
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        if ( next == '.' ) {
          float_point = true;
          in_while++;
        } // if
        next = cin.peek();

      } // while

      gNext_tokINfo.mgpToken_type = uGp_Int; // ��l�]��int

      if ( float_point == true )
        gNext_tokINfo.mgpToken_type = uGp_Float; // ��dot�N�]��float

      return;

    } // else if
    else {
      gNext_tokINfo.muse = false; // �M��k�L��
      gNext_tokINfo.mgpToken_type = uNomean;  // ��l���S�N�q
      gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + temp;
      return;
    } // else


  } // if
} // Get_nextToken()


// ===================================================
// �H�U����k
bool TestError() {
  // ���լO���Oerror
  bool ans = false;
  if ( gUnrecognized == true || gUndefined == true || gUnexpected == true ) {
    ans = true; // �з�3��error
  } // if
  else if ( gDivide_zero == true ) {
    ans = true; // ���H0�o��error

  } // else if

  return ans;
} // TestError() 

double Test_save( Tokeninformation &temp, bool &exist ) {
  //�o�Ӵ��ըS�s�L�N�|��XERROR
  int max = gTokenplace.size();
  int i = 0;

  int num = 0;
  while ( i < max && exist == false ) {
    // i < max �S���N�~��]���N���X
    if ( gTokenplace[ i ].mid_ofName == temp.mgpToken_data ) {
      exist = true;
      num = i; // �O�U�ĴX�ӬۦP
      
    } // if
    i++;
  } // while

  if ( exist == true ) {

    GetTokenButton(); // ����now��next
    if ( gTokenplace[ num ].mid_Oftype == uGp_Float ) {
      gFloat_num = true;
    } // if
    else gInt = true;

    return gTokenplace[ num ].mid_ofValue;
  } // if
  else {
    // �S�s�L�N��Xundefinederror
    Set_Error( uNdefined );
    return 0;
  } // else

} // Test_save()

void Isfloat() {
  if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
    gFloat_num = true;
  } // if
  else gInt = true;

} // Isfloat()

double StrToNum( Tokeninformation &temp ) {
  // �N�r����Ʀr
  double ans = 0.0;
  if ( temp.mgpToken_data.empty() == false ) {

    ans = atof( temp.mgpToken_data.c_str() ); // ���নconst*char �~��ϥ�atof

  } // if

  return ans;

} // StrToNum()



void Factor( double &num ) {
  // �^�Ǥ@�ӭ�
  // cout << "in";
  
  bool exist = false;
  if ( TestError() == true ) {
    return ;
  } // if

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;

  if ( gNext_tokINfo.mgpToken_type == uGp_ID ) {
    Tokeninformation temp = gNext_tokINfo;

    num = Test_save( temp, exist ); // �ݦ��S���s�L�F
    return;
  } // if
  else if ( gNext_tokINfo.mgpToken_data == "+" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next���ʨ�now
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 + buffer;
      
      Isfloat(); // �]gInt����true
      GetTokenButton(); // next���ʨ�now
      return;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 + buffer;
      
      Isfloat(); // �]gfloat
      GetTokenButton(); // next���ʨ�now
      return;
    } // else if
    else if ( gNext_tokINfo.muse == false ) {
      //�M��k�L�����ӥX�{��
      Set_Error( uNrecognized );
      return ;
    } // else if
    else {
      Set_Error( uNexpected );
      return ;
    } // else
  } // else if
  else if ( gNext_tokINfo.mgpToken_data == "-" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next���ʨ�now
    PeekTokenButton();
    
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {

      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 - buffer;
      
      Isfloat(); // �|�]gint = true
      GetTokenButton(); // next���ʨ�now
      return;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 - buffer;     
      Isfloat();
      GetTokenButton(); // next���ʨ�now
      return;
    } // else if
    else if ( gNext_tokINfo.muse == false ) {
      Set_Error( uNrecognized );
      return ;
    } // else if
    else {
      Set_Error( uNexpected );
      return ;
    } // else

  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {
    // cout << "in";
    double buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
    
    num = 0.0 + buffer;
    // cout << num;
    Isfloat(); // �]���|�Ngint �]��true
    GetTokenButton(); // next���ʨ�now
    return;
  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {

    double buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
    
    num = 0.0 + buffer;
    Isfloat();
    GetTokenButton(); // next���ʨ�now
    return;
  } // else if

  else if ( gNext_tokINfo.mgpToken_data == "(" ) {
    GetTokenButton();
    bool test = Test_In_Arith(); // �T�{�O�_�i�H�~�����Arith
    if ( test == true ) {
      double temp = 0.0;
      ArithExp(temp);
      if ( TestError() == true ) {
        return;
      } // if
      
      PeekTokenButton();
      if ( gNext_tokINfo.mgpToken_data != ")" ) {
        bool stop = false;
        ThreeERROR( stop ); // �t�d�h���T��error
        if ( stop == true ) return ;
      } // if
      else GetTokenButton(); // ��next���ʨ�now�]���C�@��function�I�s�ڳ��O����Peektoken����next

      num = temp;
    } // if
    else return ;

  } // else if
  else {
    // cout << "in";
    bool stop = false;
    ThreeERROR( stop ); // �t�d�h���T��error
    if ( stop == true ) return ;
  } // else 

  
} // Factor()

bool Test_In_Arith() {
  // ���կ�_�I�sArith
  if ( TestError() == true ) return false;
  bool ans = false;
  PeekTokenButton();
  if ( gNext_tokINfo.mgpToken_data == "+" || gNext_tokINfo.mgpToken_data == "-"
    || gNext_tokINfo.mgpToken_data == "*" || gNext_tokINfo.mgpToken_data == "/"
    || gNext_tokINfo.mgpToken_type == uGp_ID || gNext_tokINfo.mgpToken_type == uGp_Float
    || gNext_tokINfo.mgpToken_type == uGp_Int ) {
    // * / ���T�w
    return true;
  } // if
  else if ( gNext_tokINfo.muse == false  ) {
    Set_Error( uNrecognized );
    return false;

  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_ID ) {
    Set_Error( uNdefined );
    return false;
  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uEnd_input ) {
    Set_Error( uNdefined );
    return false;
  } // else if
  else if ( TestError() == false ) {
    Set_Error( uNexpected );
    return false;
  } // else if

  return false;
} // Test_In_Arith()

void Term( double &temp ) {
  bool out_while = false;
  // cout << gNow_tokINfo.mgpToken_data;
  
  Factor( temp );
  // cout << temp;

  if ( TestError() == true )
    return ; // ��error�N����

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "*" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Factor( temp_next );

      if ( TestError() == true )
        return ; // ��error�N����

      if ( gFloat_num == true ) {
        temp = temp * temp_next;
      } // if
      else {

        if ( gInt == true ) {
          int buffer1 = (int)temp;  // ���O�������ܭn�^���ˬd!!!!!!!!!!!!!!
          int buffer2 = (int)temp_next;
          temp = ( buffer1 * buffer2 );
        } // if        

      } // else

      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "/" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Factor( temp_next );

      if ( TestError() == true )
        return ; // ��error�N����

      if ( gFloat_num == true ) {

        if ( temp_next == 0 ) {
          gDivide_zero = true;
          Message_error(); // �LError!
          return ;
        } // if

        temp = temp / temp_next;
      } // if
      else {

        if ( gInt == true ) {
          int buffer1 = (int)temp;
          int buffer2 = (int)temp_next;

          if ( buffer2 == 0 ) {
            gDivide_zero = true;
            Message_error(); // �LError!
            return ;
          } // if

          temp = ( buffer1 / buffer2 ); // ���`���p

        } // if

      } // else

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  
} // Term()

void ArithExp( double &temp ) {
  
  Term( temp );

  if ( TestError() == true )
    return ; // ��error�N����

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  bool out_while = false;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "+" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Term( temp_next );
      if ( TestError() == true )
        return ; // ��error�N����
      temp = temp + temp_next;


      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "-" ) {
      
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Term( temp_next );
      // cout << temp_next;
      if ( TestError() == true )
        return ; // ��error�N����

      temp = temp - temp_next;

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  

} // ArithExp()


void ThreeERROR( bool &stop ) {
  // �ΦbBooleanOprator  and Factor 

  if ( gNext_tokINfo.muse == false ) {
    // �S�N�q��
    Set_Error( uNrecognized );
    stop = true;
  } // if
  else if ( gNext_tokINfo.mgpToken_type == uGp_ID ) {
    Set_Error( uNdefined );
    stop = true;
  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uEnd_input ) {
    Set_Error( uNdefined );
    stop = true;
  } // else if
  else {
    // ������X�{
    Set_Error( uNexpected );
    stop = true;
  } // else
} // ThreeERROR()



bool Deal_LME( string &gp, double &ans, double &temp, bool &flag ) {
  
  
  if ( gp == "=" ) {
    if ( ans == temp ) flag = true;
    else flag = false;
    return true;
  } // if
  else if ( gp == ">=" ) {
    if ( ans >= temp ) flag = true;
    else flag = false;
    return true;
  } // else if
  else if ( gp == "<=" ) {
    if ( ans <= temp ) flag = true;
    else flag = false;
    return true;
  } // else if
  else if ( gp == "<>" ) {
    if ( ans != temp ) flag = true;
    else flag = false;
    return true;
  } // else if
  else if ( gp == ">" ) {
    
    if ( ans > temp ) flag = true;
    else flag = false;
    
    return true;
  } // else if
  else if ( gp == "<" ) {
    if ( ans < temp ) flag = true;
    else flag = false;
    return true;
  } // else if

  return false;
} // Deal_LME()

void Printf_true_false( bool &flag ) {
  // �t�d�L����
  if ( flag == true ) {
    cout << "true"; // true�N�Ltrue
    cout << "\n";
  } // if   
  else {
    cout << "false"; // false �N�Lfalse
    cout << "\n";
  } // else

} // Printf_true_false()


void Twoerror( bool &gp ) {

  if ( gNext_tokINfo.muse == false ) {
    
    Set_Error( uNrecognized );
    gp = true;
  } // if
  else {
    
    Set_Error( uNexpected );
    gp = true;
  } // else 

} // Twoerror()


void NOT_ID_StartFactor( double &num ) {
  
  bool exist = false;
  if ( TestError() == true ) {
    return ;
  } // if

  PeekTokenButton();

  if ( gNext_tokINfo.mgpToken_data == "+" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next���ʨ�now
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 + buffer;
      
      Isfloat();
      GetTokenButton(); // next���ʨ�now
      return ;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 + buffer;
      
      Isfloat(); // �]gfloat
      GetTokenButton(); // next���ʨ�now
      return ;
    } // else if
    else if ( gNext_tokINfo.mgpToken_type == uNomean ) {
      Set_Error( uNrecognized );
      return ;
    } // else if
    else {
      Set_Error( uNexpected );
      return ;
    } // else
  } // if
  else if ( gNext_tokINfo.mgpToken_data == "-" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next���ʨ�now
    
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {

      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 - buffer;
      
      Isfloat();
      GetTokenButton(); // next���ʨ�now
      return ;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
      num = 0.0 - buffer;
      
      Isfloat();
      GetTokenButton(); // next���ʨ�now
      return ;
    } // else if
    else if ( gNext_tokINfo.mgpToken_type == uNomean ) {
      Set_Error( uNrecognized );
      return ;
    } // else if
    else {
      
      Set_Error( uNexpected );
      return ;
    } // else

  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {

    
    double buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr

    
    num = 0.0 + buffer;

    Isfloat(); // �]���|�Ngint �]��true
    GetTokenButton(); // next���ʨ�now
    
    return ;

  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {

    double buffer = StrToNum( gNext_tokINfo ); // ���I�s����ন�Ʀr
    
    num = 0.0 + buffer;
    Isfloat();
    GetTokenButton(); // next���ʨ�now
    return ;
  } // else if

  else if ( gNext_tokINfo.mgpToken_data == "(" ) {
    GetTokenButton();
    bool test = Test_In_Arith(); // �T�{�O�_�i�H�~�����Arith
    if ( test == true ) {
      double temp = 0.0;
      ArithExp(temp);
      if ( TestError() == true ) {
        return;
      } // if

      PeekTokenButton();
      if ( gNext_tokINfo.mgpToken_data != ")" ) {
        bool stop = false;
        ThreeERROR( stop ); // �t�d�h���T��error
        if ( stop == true ) return ;
      } // if
      else GetTokenButton(); // ��next���ʨ�now�]���C�@��function�I�s�ڳ��O����Peektoken����next

      num = temp;
      return ;
    } // if
    else return ;

  } // else if
  else {
    
    bool stop = false;
    ThreeERROR( stop ); // �t�d�h���T��error
    if ( stop == true ) return ;
  } // else 

  
} // NOT_ID_StartFactor()

void Test_Non_appear( bool &gp ) {
  // �t�d�ˬd���S���X�{���ӥX�{���Ÿ�
  if ( gNext_tokINfo.mgpToken_data != "+" && gNext_tokINfo.mgpToken_data != "-"
    &&  gNext_tokINfo.mgpToken_data != "*" && gNext_tokINfo.mgpToken_data != "/"
    && gNext_tokINfo.mgpToken_data != "<>" && gNext_tokINfo.mgpToken_data != ">"
    && gNext_tokINfo.mgpToken_data != "<" && gNext_tokINfo.mgpToken_data != "="
    && gNext_tokINfo.mgpToken_data != ">=" && gNext_tokINfo.mgpToken_data != "<="
    && gNext_tokINfo.mgpToken_data != ";" ) {
    
    bool test = false;
    Twoerror( test );
    if ( test == true ) {
      gp = true; // �N��X�{error
      return;
    } // if

  } // if
  return;
} // Test_Non_appear()

void NOT_ID_StartTerm(double &temp) {
  bool out_while = false;

 
  // GetTokenButton();
  NOT_ID_StartFactor( temp );

  if ( TestError() == true )
    return ; // ��error�N����



  bool gp = false;
  // GetTokenButton();
  PeekTokenButton();
  Test_Non_appear( gp );
  if ( gp == true ) return ;

  // GetTokenButton();


  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "*" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Factor(temp_next);
      if ( TestError() == true )
        return ; // ��error�N����

      if ( gFloat_num == true ) {
        temp = temp * temp_next;
      } // if
      else {
        int buffer1 = (int)temp;  // ���O�������ܭn�^���ˬd!!!!!!!!!!!!!!
        int buffer2 = (int)temp_next;
        temp = ( buffer1 * buffer2 );
      } // else

      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "/" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Factor( temp_next );
      if ( TestError() == true )
        return ; // ��error�N����

      // cout << "in";
      if ( gFloat_num == true ) {
        if ( temp_next == 0 ) {
          gDivide_zero = true;

          Message_error(); // �LError!
          return ;
        } // if
        temp = temp / temp_next; // ���`����

      } // if
      else {

        int buffer1 = (int)temp;
        int buffer2 = (int)temp_next;
        if ( buffer2 == 0 ) {
          gDivide_zero = true;
          Message_error(); // �LError!
          return ;
        } // if

        temp = ( buffer1 / buffer2 ); // ���`���p



      } // else

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  
} // NOT_ID_StartTerm()


void NOT_ID_StartArithExp(double &temp) {
  
  NOT_ID_StartTerm(temp);
  if ( TestError() == true )
    return ; // ��error�N����

  PeekTokenButton();

  bool gp = false;
  Test_Non_appear( gp ); // �ݦ��S���X�{���ӥX�{�Ÿ�
  if ( gp == true ) return ; // �X�{���ӥX�{�Nreturn 

  bool out_while = false;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "+" ) {
      GetTokenButton(); // ����next��now
      double temp_next = 0.0;
      Term( temp_next );
      if ( TestError() == true )
        return ; // ��error�N����
      temp = temp + temp_next;


      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "-" ) {
      GetTokenButton(); // ����next��now
      // cout << gNow_tokINfo.mgpToken_data;
      double temp_next = 0.0;
      Term( temp_next );
      // cout << temp_next;
      if ( TestError() == true )
        return ; // ��error�N����

      temp = temp - temp_next;

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  
} // NOT_ID_StartArithExp()




bool BooleanOprator() {
  bool ans = false;
  if ( gNext_tokINfo.mgpToken_data == "=" && gNext_tokINfo.mgpToken_type == uEqual
    || gNext_tokINfo.mgpToken_data == "<>" && gNext_tokINfo.mgpToken_type == uBool_ope
    || gNext_tokINfo.mgpToken_data == ">" && gNext_tokINfo.mgpToken_type == uBool_ope
    || gNext_tokINfo.mgpToken_data == "<" && gNext_tokINfo.mgpToken_type == uBool_ope
    || gNext_tokINfo.mgpToken_data == ">=" && gNext_tokINfo.mgpToken_type == uBool_ope
    || gNext_tokINfo.mgpToken_data == "<=" && gNext_tokINfo.mgpToken_type == uBool_ope ) {

    ans = true;

  } // if

  return ans;


} // BooleanOprator()




bool NOT_ID_StartArithExpOrBexp( bool &flag ) {
  // �^��true�N��i��j�p
  double temp = 0.0;
  // GetTokenButton();
  NOT_ID_StartArithExp( temp );

  bool gp = false;
  double ans = 0.0;
  string ope;
  if ( TestError() == true )
    return false;

  PeekTokenButton();

  ope = gNext_tokINfo.mgpToken_data;
  if ( BooleanOprator() == false ) {
    
    if ( gNext_tokINfo.mgpToken_data != ";" ) {
      // cout << "in";
      bool stop = false;
      ThreeERROR( stop );
      if ( stop == true ) return false;
    } // if
    
    
    if ( gInt == true ) {
      gNum_int = (int)temp;
      return false;
    } // if
    else if ( gFloat_num == true ) {
      gNum_double = temp;
      return false;
    } // else if
  } // if
  else {
    GetTokenButton();
    double num = 0.0;
    ArithExp( num );
    if ( TestError() == true )
      return false;
    PeekTokenButton();
    
    
    if ( gNext_tokINfo.mgpToken_data != ";" ) {
      // cout << "in";
      bool stop = false;
      ThreeERROR( stop );
      if ( stop == true ) return false;
    } // if
    
    if ( temp - num <= 0.0001 && temp > num ) {
      temp = num; // �B�z�~�t
      Deal_LME( ope, temp, num, flag);
      return true;
    } // if
    else if ( num - temp <= 0.0001 && num > temp ) {
      num = temp; // �B�z�~�t
      Deal_LME( ope, temp, num, flag );
      return true;
    } // else if
    else {
      // cout << temp;
      // cout << num;
      Deal_LME( ope, temp, num,flag );
      
      return true;
    } // else

    gp = true;

  } // else

  return gp;


} // NOT_ID_StartArithExpOrBexp()

bool IDlessArithExpOrBexp( double &ans, bool &flag ) {
  // �^��true�N��i��j�p
  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  bool gp = false;

  Test_Non_appear( gp ); // ���ӥX�{
  if ( gp == true )
    return false;

  bool out = false;
  string ope;
  
  while ( out == false ) {

    ope = gNext_tokINfo.mgpToken_data;
    
    if ( gNext_tokINfo.mgpToken_data == "+"  && gNext_tokINfo.mgpToken_type == uUpdown
      || gNext_tokINfo.mgpToken_data == "-"  && gNext_tokINfo.mgpToken_type == uUpdown ) {
      GetTokenButton();
      double next = 0.0;
      Term( next );
      if ( TestError() == true )
        return 0;

      if ( ope == "+" ) {
        ans = ans + next;
      } // if
      else if ( ope == "-" ) {
        ans = ans - next;
      } // else if

      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_data == "*" && gNext_tokINfo.mgpToken_type == uMul_Divide
      || gNext_tokINfo.mgpToken_data == "/" && gNext_tokINfo.mgpToken_type == uMul_Divide ) {
      GetTokenButton();
      double next = 0.0;
      Factor( next );
      
      if ( TestError() == true )
        return 0;

      if ( ope == "*" ) {
        
        ans = ans * next;
        
      } // if
      else if ( ope == "/" ) {
        if ( gInt == true ) {
          int buffer1 = (int)ans;
          int buffer2 = (int)next;
          ans = buffer1 / buffer2;
        } // if
      } // else if

      PeekTokenButton();
    } // else if

    else out = true;

  } // while

  // ========================================================== �H�W�Owhile����

  ope = gNext_tokINfo.mgpToken_data;
  // cout << ope;
  Test_Non_appear( gp );
  if ( gp == true )
    return false; // �X�{���ӥX�{��

  // cout << gNext_tokINfo.mgpToken_data;

  if ( BooleanOprator() == false ) {

    if ( gInt == true ) {
      gNum_int = (int)ans; //�ǻ��覡���I�M�I�X�{���D�n�^���ˬd

    } // if
    else if ( gFloat_num == true ) {
      gNum_double = ans;
    } // else if
  } // if
  else {
    
    double num = 0.0;
    // cout << "in";
    GetTokenButton();
    ArithExp( num );
    // cout << num;
    if ( TestError() == true )
      return false;
    PeekTokenButton();
    // cout << gNext_tokINfo.mgpToken_data;
    // �o�Ӥ����b�o�B�Q���O�ܦn �U���X���D�n�^���ˬd
    
    if ( gNext_tokINfo.mgpToken_data != ";" ) {
      
      bool stop = false;
      ThreeERROR( stop );
      if ( stop == true ) return false;
    } // if

    if ( ans - num <= 0.0001 && ans > num ) {
      ans = num; // �B�z�~�t
      Deal_LME( ope, ans, num, flag );

    } // if
    else if ( num - ans <= 0.0001 && num > ans ) {
      num = ans; // �B�z�~�t
      Deal_LME( ope, ans, num, flag );

    } // else if
    else {
     
      Deal_LME( ope, ans, num, flag );

    } // else

    
    
    
    gp = true;

  } // else

  return gp;

} // IDlessArithExpOrBexp()


bool Out_put() {
  bool ans = false;
  if ( gInt == true ) {
    cout << gNum_int;
    cout << "\n";
    ans = true;
    cout << "> ";
  } // if
  else if ( gFloat_num == true ) {
    if ( gNum_double > 0 ) {
      cout << fixed << setprecision( 3 ) << gNum_double;   // ��3��
      cout << "\n";
      cout << "> ";
    } // if
    else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

      cout << "0.000";   // �]����3�쳣�@��
      cout << "\n";
      cout << "> ";
    } // else if
    else {
      // �p��0�����p
      cout << fixed << setprecision( 3 ) << gNum_double;   // ��3��
      cout << "\n";
      cout << "> ";
    } // else

    ans = true;
  } // else if
  else ans = false;

  return ans;

} // Out_put()


bool Check_exist( string &name, int &num ) {

  int max = gTokenplace.size();
  int i = 0;
  bool exist = false;

  while ( i < max ) {
    if ( gTokenplace[ i ].mid_ofName == name ) {
      exist = true;
      num = i; // �O�U�ĴX�ӬۦP
      // GetTokenButton(); // ����next��now
    } // if
    i++;

  } // while

  if ( exist == true ) return true;
  else return false;

} // Check_exist




bool Out_put_twotype( bool &normal, Id_Data &out_put_data ) {
  bool ans = false;
  if ( normal == true ) {


    if ( gInt == true ) {
      cout << gNum_int;
      cout << "\n";
      ans = true;
      cout << "> ";
    } // if
    else if ( gFloat_num == true ) {
      if ( gNum_double > 0 ) {
        cout << fixed << setprecision( 3 ) << gNum_double;   // ��3��
        cout << "\n";
        cout << "> ";
      } // if
      else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

        cout << "0.000";   // �]����3�쳣�@��
        cout << "\n";
        cout << "> ";
      } // else if
      else {
        // �p��0�����p
        cout << fixed << setprecision( 3 ) << gNum_double;   // ��3��
        cout << "\n";
        cout << "> ";
      } // else

      
      ans = true;
    } // else if
    else ans = false;

  } // if
  else {
    if ( gInt == true ) {
      cout << (int)out_put_data.mid_ofValue;
      cout << "\n";
      cout << "> ";
      ans = true;
    } // if
    else if ( gFloat_num == true ) {
      if ( gNum_double > 0 ) {
        cout << fixed << setprecision( 3 ) << out_put_data.mid_ofValue;   // ��3��
        cout << "\n";
        cout << "> ";
      } // if
      else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

        cout << "0.000";   // �]����3�쳣�@��
        cout << "\n";
        cout << "> ";
      } // else if
      else {
        // �p��0�����p
        cout << fixed << setprecision( 3 ) << out_put_data.mid_ofValue;   // ��3��
        cout << "\n";
        cout << "> ";
      } // else

      
      ans = true;
    } // else if
    else ans = false;

  } // else


  return ans;

} // Out_put_twotype


double ID_exist( string &name, bool &exist ) {

  int max = gTokenplace.size();
  int i = 0;
  int num = 0;

  while ( i < max && exist == false ) {
    if ( gTokenplace[ i ].mid_ofName == name ) {
      exist = true;
      num = i; // �O�U�ĴX�ӬۦP
      
    } // if
    i++;

  } // while

  if ( exist == true ) {
    return gTokenplace[ num ].mid_ofValue;
  } // if
  else return 0;
  

} // ID_exist


bool Command() {

  Id_Data out_put_data;
  PeekTokenButton();

  if ( gNext_tokINfo.mgpToken_type == uGp_ID ) {
    
    string name = gNext_tokINfo.mgpToken_data;
    GetTokenButton(); // ID���ʨ�now
    PeekTokenButton();

    if ( gNext_tokINfo.mgpToken_data == ":=" && gNext_tokINfo.mgpToken_type == uDefine ) {
      
      int num = 0;
      bool exist = Check_exist( name, num );
      
      GetTokenButton();
      
      double ans = 0.0;
      
      ArithExp(ans);
      
      if ( TestError() == true )
        return false;

      // GetTokenButton();
      PeekTokenButton();
      // cout << gNext_tokINfo.mgpToken_data;

      if ( gNext_tokINfo.mgpToken_data != "+" && gNext_tokINfo.mgpToken_data != "-"
        && gNext_tokINfo.mgpToken_data != "*" && gNext_tokINfo.mgpToken_data != "/"
        && gNext_tokINfo.mgpToken_data != ";" ) {
        bool test = false;
        ThreeERROR( test ); // ��Xerror

      } // if
      else {
        if ( exist == true ) {
          gTokenplace[ num ].mid_ofValue = ans;
          out_put_data.mid_ofValue = ans; // �n��X����
          if ( gFloat_num == true ) {
            gTokenplace[ num ].mid_Oftype = uGp_Float;  //�M�w���O
          } // if
          else if ( gInt == true ) {
            gTokenplace[ num ].mid_Oftype = uGp_Int; //�M�w���O
          } // else if

        } // if
        else {
          out_put_data.mid_ofName = name;
          out_put_data.mid_ofValue = ans;
          if ( gFloat_num == true ) {
            out_put_data.mid_Oftype = uGp_Float; //�M�w���O
          } // if
          else if ( gInt == true ) {
            out_put_data.mid_Oftype = uGp_Int; //�M�w���O
          } // else if

          gTokenplace.push_back( out_put_data ); // �S�s�L�N�s�J
        } // else

        bool normal = false;
        Out_put_twotype( normal, out_put_data );
      } // else






      return false;
    } // if
    else {

      
      Tokeninformation gp = gNow_tokINfo;
      bool exist = false;
      double num = ID_exist( gp.mgpToken_data, exist );// num �OgTokenplace�W�r�Mgp�@�˪���

      if ( exist == false ) {
        gUndefined = true;
        gError = true;
        Skipline();
        cout << "Undefined identifier : " << "'";
        cout << gNow_tokINfo.mgpToken_data;
        cout << "'" << "\n";

        return false;
      } // if

      if ( gNext_tokINfo.mgpToken_type != uUpdown && gNext_tokINfo.mgpToken_type != uMul_Divide
        && gNext_tokINfo.mgpToken_type != uEqual && gNext_tokINfo.mgpToken_type != uBool_ope
        && gNext_tokINfo.mgpToken_type != uBrackets_L && gNext_tokINfo.mgpToken_type != uBrackets_R
        && gNext_tokINfo.mgpToken_type != uEqual && gNext_tokINfo.mgpToken_type != uDefine ) {
        bool test = false;
        
        Twoerror( test );
        if ( test == true ) return false;
      } // if

      
      

      Id_Data In; // ���F��ѼƦӫإߪ�
      In.mid_ofValue = num;

      if ( exist == false ) return false;
      else {
        PeekTokenButton();
        // cout << gNext_tokINfo.mgpToken_data;
        if ( gNext_tokINfo.mgpToken_data == ";" ) {
          bool normal = false; // normal�]false�N���O�n��X�@�몺gINT gfloat
          Out_put_twotype( normal, In );
        } // if

        bool flag = false;
        bool test = IDlessArithExpOrBexp( num, flag ); // test�Otrue���ܥN���i��j�p

        if ( TestError() == true ) return false;

        PeekTokenButton();
        if ( gNext_tokINfo.mgpToken_data == ";" ) {

          if ( test == true ) {
            Printf_true_false( flag );
            
          } // if
          else {
            In.mid_ofValue = num;
            bool normal = false; // normal�]false�N���O�n��X�@�몺gINT gfloat
            Out_put_twotype( normal, In );
            
          } // else

        } // if



      } // else

    } // else

    return false;
  } // if
  else if ( gNext_tokINfo.mgpToken_data == "quit" && gNext_tokINfo.mgpToken_type == uEnd_input ) {
    return true;

  } // else if
  else {
    
    bool flag = false;
    bool temp = false;
    // GetTokenButton();
    
    temp = NOT_ID_StartArithExpOrBexp( flag );
    
    if ( gNext_tokINfo.mgpToken_data == ";" ) {
      
      if ( temp == true ) {
        // �n��Xtrue false ;
        Printf_true_false( flag );
        cout << "> ";
        return false;
      } // if
      else {
        // ��X�ƭ�
        Out_put();
        // cout << "> ";
        return false;
      } // else

    } // if
    
  } // else


  return false;
} // Command()



