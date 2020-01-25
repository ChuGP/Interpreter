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
  double mid_ofValue; // 全數以double型別去存
  int mid_Oftype; // 型別
  bool muse = false; // 是否儲存正確有用值
};

struct Tokeninformation {
  string mgpToken_data;
  int mgpToken_type; // 型別
  bool muse = false; // 是否與文法有關(有用)
};



static int uThreespace = 1; // 空白tab \0
static int uUpdown = 2; // 加減
static int uMul_Divide = 3; // 乘除
static int uSemicolon = 4;  // 結尾分號
static int uBrackets_L = 5; // 左括號
static int uBrackets_R = 6; // 右括號
static int uDefine = 7; // 初始定義 :=
static int uNo_useobj = 8;
static int uComment_line = 9; // 單行註解
static int uComment_block = 10; // 區塊註解 例如"/**/"
static int uGp_ID = 11; // 字母A-Z 或 A_
static int uGp_Int = 12; // 整數數字 
static int uGp_Float = 13; // 浮點數數字 
static int uEnd_input = 14; // quit
static int uBool_ope = 15; // >= <= > < 
static int uEqual = 16; // =
static int uNomean = 17; // 其他非規定該出現的

// 3種error
static int uNrecognized = 1;
static int uNexpected = 2;
static int uNdefined = 3;

Tokeninformation gNow_tokINfo;
Tokeninformation gNext_tokINfo;
vector<Id_Data> gTokenplace; // 存ID

bool gError = false; // 有任何一種error
bool gUnrecognized = false;
bool gUndefined = false;
bool gUnexpected = false;
bool gDivide_zero = false; // 單純輸出othererror
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

// 以下為小工具

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

  gp = cin.get();  // 讀掉換行 
  
} // Skipline()

bool Garbageskip() {
  bool out = false;
  bool succeed = false; // 看有沒有成功讀掉垃圾 
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
      succeed = true; // 要輸出的
      out = true; // 跳出迴圈用
    } // else

  } // while

  
  return succeed; // 確認是否成功讀掉垃圾
} // Garbageskip()

void SkipWhitespace( char &gp ) {
  // 條列式條件錯哪個好找
  //使用gp不用temp怕誤用

  bool succeed = false;
  int which = 0; // 之後debug用

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
    // 找不到進哪個if直接輸出which來看

  } // while

  // cout << gp;
} // SkipWhitespace()

void Dealtoken() {
  // 專門負責移動next到now
  bool test = false;
  if ( gNext_tokINfo.mgpToken_data.empty() == false )
    test = true; // Nexttoken不為空代表要移動並呼叫clearnext

  gNow_tokINfo = gNext_tokINfo;
  Clear_now_next( test );

} // Dealtoken()

bool Test_ID_not( char &gp ) {
  // 測試是否有ID
  bool ans = false;
  if ( ( gp >= 'a'&& gp <= 'z' ) ) {
    ans = true;    // 小寫
  } // if
  else if ( gp >= 'A'&& gp <= 'Z' ) {
    ans = true; // 大寫
  } // else if
  else ans = false; // 沒進if就保持原樣保險起見再設一次false
  return ans;

} // Test_ID_not()

bool Test_number_not( char &gp ) {
  // 測試是否有數字
  bool ans = false;
  if ( gp >= '0' && gp <= '9' ) {
    ans = true;
  } // if

  return ans;
} // Test_number_not()

void Clear_now_next( bool now_next ) {
  // 負責清now next token
  if ( now_next == true ) {
    gNext_tokINfo.mgpToken_data.clear(); // 如果next_token有東西要清掉
  } // if
  else {
    gNow_tokINfo.mgpToken_data.clear(); // 如果now_token有東西要清掉
  } // else

} // Clear_now_next()

void Message_error() {
  int error_type = 0; // debug直接看
  if ( gUnrecognized == true ) {
    
    gError = true;
    Skipline(); // 把該行讀掉
    error_type = 1;
    Print_error( error_type );
    cout << "> "; // 測試
  } // if
  else if ( gUnexpected == true ) {
    gError = true;
    Skipline(); // 把該行讀掉
    error_type = 2;
    Print_error( error_type );
    cout << "> "; // 測試
  } // else if
  else if ( gUndefined == true ) {
    gError = true;
    Skipline(); // 把該行讀掉
    error_type = 3;
    Print_error( error_type );
    cout << "> "; // 測試
  } // else if
  
  else {
    gError = true;
    Skipline(); // 其他error /0之類的
    error_type = 4;
    Print_error( error_type );
    cout << "> "; // 測試
  } // else



} // Message_error()

void Print_error( int &gp ) {
  Tokeninformation temp = gNow_tokINfo;
  //拿一個temp來存不直接使用全域怕修改錯

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
  // 設3種error
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
//以下原則 if條件判斷盡量不要一長串很難找bug

void GetTokenButton() {
  // 包裝好的function 有next就移動到now沒有就直接get一個到now
  gNow_tokINfo.mgpToken_data.clear();
  bool test = gNext_tokINfo.mgpToken_data.empty();
  if ( test == false ) {                 // 用來當getToken的開關
    Dealtoken(); //呼叫專門移動next到now的函示
    // cout << "成功";
  } // if
  else {
    Gettoken(); // 取token

  } // else
} // GetTokenButton()



void Gettoken() {

  bool test = false; // 接我的Garbageskip
  test = Garbageskip(); // 把不要的字元讀掉
  bool run = false;
  char temp = cin.get(); // 存一個char
  char skip = cin.peek();
  SkipWhitespace( skip ); // 跳過空白3兄弟
  char next_temp = cin.peek(); // 偷看temp的下一個

  if ( temp == ';' ) {
    gNow_tokINfo.mgpToken_type = uSemicolon; // 分號type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // if
  else if ( temp == '(' ) {
    gNow_tokINfo.mgpToken_type = uBrackets_L; // 括號type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == ')' ) {
    gNow_tokINfo.mgpToken_type = uBrackets_R; // 括號type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == '+' ) {
    gNow_tokINfo.mgpToken_type = uUpdown; // + -type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == '-' ) {
    gNow_tokINfo.mgpToken_type = uUpdown; // + -type
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == ':' && next_temp == '=' ) {

    gNow_tokINfo.mgpToken_data = temp;
    next_temp = cin.get();
    gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next_temp;
    gNow_tokINfo.mgpToken_type = uDefine;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == ':' && next_temp != '=' ) {
    // 單純 ':'
    gNow_tokINfo.mgpToken_type = uNomean; // 單':'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = false; // 與文法無關!!!!!!!!
    run = true;
  } // else if
  else if ( temp == '/' && next_temp == '*' ) {
    gNow_tokINfo.muse = false; // 與文法無關!!!!!!!!純過濾
    next_temp = cin.get();

    bool succeed = false;
    while ( succeed == false ) {
      temp = cin.get(); // 存一個char
      skip = cin.peek();
      SkipWhitespace( skip ); // 跳過空白3兄弟
      char next_temp = cin.peek(); // 偷看temp的下一個        
      if ( temp == '*' && next_temp == '/' ) {
        next_temp = cin.get();
        succeed = true;
      } // if
      else succeed = false;

    } // while     

    gNow_tokINfo.mgpToken_type = uComment_block; // 註解區塊
    GetTokenButton(); //遞迴跑

    return;

  } // else if
  else if ( temp == '/' && next_temp == '/' ) {
    gNow_tokINfo.muse = false; // 與文法無關!!!!!!!!純過濾
    next_temp = cin.get();
    Skipline();
    gNow_tokINfo.mgpToken_type = uComment_line; // 註解一行
    GetTokenButton(); //遞迴跑

    return;
  } // else if
  else if ( temp == '/' && next_temp != '/' && next_temp != '*' ) {
    gNow_tokINfo.mgpToken_type = uMul_Divide; // 單'/'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == '*' ) {
    gNow_tokINfo.mgpToken_type = uMul_Divide; // 單'*'
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( temp == '<' || temp == '>' ) {
    gNow_tokINfo.muse = true; // 與文法有關
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
      gNow_tokINfo.mgpToken_data = temp; // 單純 > <
      return;
    } // else

  } // else if
  else if ( temp == '=' ) {
    gNow_tokINfo.mgpToken_type = uEqual; // 單'='
    gNow_tokINfo.mgpToken_data = temp;
    gNow_tokINfo.muse = true; // 與文法有關
    return;
  } // else if
  else if ( Test_ID_not( temp ) == true ) {
    gNow_tokINfo.muse = true; // 與文法有關
    gNow_tokINfo.mgpToken_data = temp;

    while ( Test_ID_not( next_temp ) == true || next_temp == '_' || Test_number_not( next_temp ) == true ) {
      next_temp = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next_temp;
      next_temp = cin.peek();
      //跑完得到完整id
    } // while

    gNow_tokINfo.mgpToken_type = uGp_ID; // 跑完先設為ID type

    if ( gNow_tokINfo.mgpToken_data == "quit" )
      gNow_tokINfo.mgpToken_type = uEnd_input; // 特殊情況遇到quit要改type

    return;

  } // else if
  else if ( temp == '.' ) {
    gNow_tokINfo.muse = true; // 與文法有關
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
    gNow_tokINfo.muse = true; // 與文法有關

    gNow_tokINfo.mgpToken_data = temp;
    bool float_point = false; // 是否為浮點數
    char next = cin.peek();
    int in_while = 0; // 只能讀到一次.
    while (  Test_number_not( next ) == true || ( next == '.'  && in_while < 1 ) ) {
      next = cin.get();
      gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + next;
      if ( next == '.' ) {
        in_while++;
        float_point = true;
      } // if
      next = cin.peek();

    } // while

    gNow_tokINfo.mgpToken_type = uGp_Int; // 初始設為int

    if ( float_point == true )
      gNow_tokINfo.mgpToken_type = uGp_Float; // 有dot就設為float

    return;

  } // else if
  else {
    gNow_tokINfo.muse = false; // 和文法無關
    gNow_tokINfo.mgpToken_type = uNomean;  // 其餘都沒意義
    gNow_tokINfo.mgpToken_data = gNow_tokINfo.mgpToken_data + temp;

    return;
  } // else




} // Gettoken()


void PeekTokenButton() {
  // 條件內不要一串
  bool test = gNext_tokINfo.mgpToken_data.empty();
  if ( test == true )
    Get_nextToken();    // PeekToken 加判斷條件
} // PeekTokenButton()

void Get_nextToken() {
  // 取token放到next
  bool empty_ornot = gNext_tokINfo.mgpToken_data.empty();  // 先接true false
  if ( empty_ornot == true ) {
    bool test = false; // 接我的Garbageskip
    test = Garbageskip(); // 把不要的字元讀掉
    bool run = false;
    char temp = cin.get(); // 存一個char
    char skip = cin.peek();
    SkipWhitespace( skip ); // 跳過空白3兄弟
    char next_temp = cin.peek(); // 偷看temp的下一個

    if ( temp == ';' ) {
      gNext_tokINfo.mgpToken_type = uSemicolon; // 分號type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // if
    else if ( temp == '(' ) {
      gNext_tokINfo.mgpToken_type = uBrackets_L; // 括號type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == ')' ) {
      gNext_tokINfo.mgpToken_type = uBrackets_R; // 括號type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == '+' ) {
      gNext_tokINfo.mgpToken_type = uUpdown; // + -type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == '-' ) {
      gNext_tokINfo.mgpToken_type = uUpdown; // + -type
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == ':' && next_temp == '=' ) {

      gNext_tokINfo.mgpToken_data = temp;
      next_temp = cin.get();
      gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next_temp;
      gNext_tokINfo.mgpToken_type = uDefine;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == ':' && next_temp != '=' ) {
      // 單純 ':'
      gNext_tokINfo.mgpToken_type = uNomean; // 單':'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = false; // 與文法無關!!!!!!!!
      run = true;
    } // else if
    else if ( temp == '/' && next_temp == '*' ) {
      gNext_tokINfo.muse = false; // 與文法無關!!!!!!!!純過濾
      next_temp = cin.get();

      bool succeed = false;
      while ( succeed == false ) {
        temp = cin.get(); // 存一個char
        skip = cin.peek();
        SkipWhitespace( skip ); // 跳過空白3兄弟
        char next_temp = cin.peek(); // 偷看temp的下一個        
        if ( temp == '*' && next_temp == '/' ) {
          next_temp = cin.get();
          succeed = true;
        } // if
        else succeed = false;

      } // while     

      gNext_tokINfo.mgpToken_type = uComment_block; // 註解區塊
      PeekTokenButton();//遞迴跑

      return;

    } // else if
    else if ( temp == '/' && next_temp == '/' ) {
      gNext_tokINfo.muse = false; // 與文法無關!!!!!!!!純過濾
      next_temp = cin.get();
      Skipline();
      gNext_tokINfo.mgpToken_type = uComment_line; // 註解一行
      PeekTokenButton(); //遞迴跑

      return;
    } // else if
    else if ( temp == '/' && next_temp != '/' && next_temp != '*' ) {
      gNext_tokINfo.mgpToken_type = uMul_Divide; // 單'/'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == '*' ) {
      gNext_tokINfo.mgpToken_type = uMul_Divide; // 單'*'
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( temp == '<' || temp == '>' ) {
      gNext_tokINfo.muse = true; // 與文法有關
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
        gNext_tokINfo.mgpToken_data = temp; // 單純 > <
        return;
      } // else

    } // else if
    else if ( temp == '=' ) {
      gNext_tokINfo.mgpToken_type = uEqual; // 單'='
      gNext_tokINfo.mgpToken_data = temp;
      gNext_tokINfo.muse = true; // 與文法有關
      return;
    } // else if
    else if ( Test_ID_not( temp ) == true ) {
      gNext_tokINfo.muse = true; // 與文法有關
      gNext_tokINfo.mgpToken_data = temp;

      while ( Test_ID_not( next_temp ) == true || next_temp == '_' || Test_number_not( next_temp ) == true ) {
        next_temp = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next_temp;
        next_temp = cin.peek();
        //跑完得到完整id
      } // while

      gNext_tokINfo.mgpToken_type = uGp_ID; // 跑完先設為ID type

      if ( gNext_tokINfo.mgpToken_data == "quit" )
        gNext_tokINfo.mgpToken_type = uEnd_input; // 特殊情況遇到quit要改type

      return;

    } // else if
    else if ( temp == '.' ) {
      gNext_tokINfo.muse = true; // 與文法有關
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
      gNext_tokINfo.muse = true; // 與文法有關

      gNext_tokINfo.mgpToken_data = temp;
      bool float_point = false; // 是否為浮點數
      char next = cin.peek();
      int in_while = 0; // 只能讀到一次.
      while ( Test_number_not( next ) == true || ( next == '.'   && in_while < 1 ) ) {
        next = cin.get();
        gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + next;
        if ( next == '.' ) {
          float_point = true;
          in_while++;
        } // if
        next = cin.peek();

      } // while

      gNext_tokINfo.mgpToken_type = uGp_Int; // 初始設為int

      if ( float_point == true )
        gNext_tokINfo.mgpToken_type = uGp_Float; // 有dot就設為float

      return;

    } // else if
    else {
      gNext_tokINfo.muse = false; // 和文法無關
      gNext_tokINfo.mgpToken_type = uNomean;  // 其餘都沒意義
      gNext_tokINfo.mgpToken_data = gNext_tokINfo.mgpToken_data + temp;
      return;
    } // else


  } // if
} // Get_nextToken()


// ===================================================
// 以下為文法
bool TestError() {
  // 測試是不是error
  bool ans = false;
  if ( gUnrecognized == true || gUndefined == true || gUnexpected == true ) {
    ans = true; // 標準3種error
  } // if
  else if ( gDivide_zero == true ) {
    ans = true; // 除以0這種error

  } // else if

  return ans;
} // TestError() 

double Test_save( Tokeninformation &temp, bool &exist ) {
  //這個測試沒存過就會輸出ERROR
  int max = gTokenplace.size();
  int i = 0;

  int num = 0;
  while ( i < max && exist == false ) {
    // i < max 沒找到就繼續跑找到就跳出
    if ( gTokenplace[ i ].mid_ofName == temp.mgpToken_data ) {
      exist = true;
      num = i; // 記下第幾個相同
      
    } // if
    i++;
  } // while

  if ( exist == true ) {

    GetTokenButton(); // 移動now到next
    if ( gTokenplace[ num ].mid_Oftype == uGp_Float ) {
      gFloat_num = true;
    } // if
    else gInt = true;

    return gTokenplace[ num ].mid_ofValue;
  } // if
  else {
    // 沒存過就輸出undefinederror
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
  // 將字串轉數字
  double ans = 0.0;
  if ( temp.mgpToken_data.empty() == false ) {

    ans = atof( temp.mgpToken_data.c_str() ); // 先轉成const*char 才能使用atof

  } // if

  return ans;

} // StrToNum()



void Factor( double &num ) {
  // 回傳一個值
  // cout << "in";
  
  bool exist = false;
  if ( TestError() == true ) {
    return ;
  } // if

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;

  if ( gNext_tokINfo.mgpToken_type == uGp_ID ) {
    Tokeninformation temp = gNext_tokINfo;

    num = Test_save( temp, exist ); // 看有沒有存過了
    return;
  } // if
  else if ( gNext_tokINfo.mgpToken_data == "+" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next移動到now
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 + buffer;
      
      Isfloat(); // 設gInt等於true
      GetTokenButton(); // next移動到now
      return;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 + buffer;
      
      Isfloat(); // 設gfloat
      GetTokenButton(); // next移動到now
      return;
    } // else if
    else if ( gNext_tokINfo.muse == false ) {
      //和文法無關不該出現的
      Set_Error( uNrecognized );
      return ;
    } // else if
    else {
      Set_Error( uNexpected );
      return ;
    } // else
  } // else if
  else if ( gNext_tokINfo.mgpToken_data == "-" && gNext_tokINfo.mgpToken_type == uUpdown ) {
    GetTokenButton(); // next移動到now
    PeekTokenButton();
    
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {

      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 - buffer;
      
      Isfloat(); // 會設gint = true
      GetTokenButton(); // next移動到now
      return;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 - buffer;     
      Isfloat();
      GetTokenButton(); // next移動到now
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
    double buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
    
    num = 0.0 + buffer;
    // cout << num;
    Isfloat(); // 跑完會將gint 設成true
    GetTokenButton(); // next移動到now
    return;
  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {

    double buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
    
    num = 0.0 + buffer;
    Isfloat();
    GetTokenButton(); // next移動到now
    return;
  } // else if

  else if ( gNext_tokINfo.mgpToken_data == "(" ) {
    GetTokenButton();
    bool test = Test_In_Arith(); // 確認是否可以繼續執行Arith
    if ( test == true ) {
      double temp = 0.0;
      ArithExp(temp);
      if ( TestError() == true ) {
        return;
      } // if
      
      PeekTokenButton();
      if ( gNext_tokINfo.mgpToken_data != ")" ) {
        bool stop = false;
        ThreeERROR( stop ); // 負責去測三種error
        if ( stop == true ) return ;
      } // if
      else GetTokenButton(); // 把next移動到now因為每一個function呼叫我都是先用Peektoken先看next

      num = temp;
    } // if
    else return ;

  } // else if
  else {
    // cout << "in";
    bool stop = false;
    ThreeERROR( stop ); // 負責去測三種error
    if ( stop == true ) return ;
  } // else 

  
} // Factor()

bool Test_In_Arith() {
  // 測試能否呼叫Arith
  if ( TestError() == true ) return false;
  bool ans = false;
  PeekTokenButton();
  if ( gNext_tokINfo.mgpToken_data == "+" || gNext_tokINfo.mgpToken_data == "-"
    || gNext_tokINfo.mgpToken_data == "*" || gNext_tokINfo.mgpToken_data == "/"
    || gNext_tokINfo.mgpToken_type == uGp_ID || gNext_tokINfo.mgpToken_type == uGp_Float
    || gNext_tokINfo.mgpToken_type == uGp_Int ) {
    // * / 不確定
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
    return ; // 有error就結束

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "*" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Factor( temp_next );

      if ( TestError() == true )
        return ; // 有error就結束

      if ( gFloat_num == true ) {
        temp = temp * temp_next;
      } // if
      else {

        if ( gInt == true ) {
          int buffer1 = (int)temp;  // 型別有錯的話要回來檢查!!!!!!!!!!!!!!
          int buffer2 = (int)temp_next;
          temp = ( buffer1 * buffer2 );
        } // if        

      } // else

      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "/" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Factor( temp_next );

      if ( TestError() == true )
        return ; // 有error就結束

      if ( gFloat_num == true ) {

        if ( temp_next == 0 ) {
          gDivide_zero = true;
          Message_error(); // 印Error!
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
            Message_error(); // 印Error!
            return ;
          } // if

          temp = ( buffer1 / buffer2 ); // 正常情況

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
    return ; // 有error就結束

  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  bool out_while = false;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "+" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Term( temp_next );
      if ( TestError() == true )
        return ; // 有error就結束
      temp = temp + temp_next;


      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "-" ) {
      
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Term( temp_next );
      // cout << temp_next;
      if ( TestError() == true )
        return ; // 有error就結束

      temp = temp - temp_next;

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  

} // ArithExp()


void ThreeERROR( bool &stop ) {
  // 用在BooleanOprator  and Factor 

  if ( gNext_tokINfo.muse == false ) {
    // 沒意義的
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
    // 不期望出現
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
  // 負責印答案
  if ( flag == true ) {
    cout << "true"; // true就印true
    cout << "\n";
  } // if   
  else {
    cout << "false"; // false 就印false
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
    GetTokenButton(); // next移動到now
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 + buffer;
      
      Isfloat();
      GetTokenButton(); // next移動到now
      return ;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 + buffer;
      
      Isfloat(); // 設gfloat
      GetTokenButton(); // next移動到now
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
    GetTokenButton(); // next移動到now
    
    PeekTokenButton();
    double buffer = 0.0;
    if ( gNext_tokINfo.mgpToken_type == uGp_Int ) {

      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 - buffer;
      
      Isfloat();
      GetTokenButton(); // next移動到now
      return ;
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {
      buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
      num = 0.0 - buffer;
      
      Isfloat();
      GetTokenButton(); // next移動到now
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

    
    double buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字

    
    num = 0.0 + buffer;

    Isfloat(); // 跑完會將gint 設成true
    GetTokenButton(); // next移動到now
    
    return ;

  } // else if
  else if ( gNext_tokINfo.mgpToken_type == uGp_Float ) {

    double buffer = StrToNum( gNext_tokINfo ); // 先呼叫函示轉成數字
    
    num = 0.0 + buffer;
    Isfloat();
    GetTokenButton(); // next移動到now
    return ;
  } // else if

  else if ( gNext_tokINfo.mgpToken_data == "(" ) {
    GetTokenButton();
    bool test = Test_In_Arith(); // 確認是否可以繼續執行Arith
    if ( test == true ) {
      double temp = 0.0;
      ArithExp(temp);
      if ( TestError() == true ) {
        return;
      } // if

      PeekTokenButton();
      if ( gNext_tokINfo.mgpToken_data != ")" ) {
        bool stop = false;
        ThreeERROR( stop ); // 負責去測三種error
        if ( stop == true ) return ;
      } // if
      else GetTokenButton(); // 把next移動到now因為每一個function呼叫我都是先用Peektoken先看next

      num = temp;
      return ;
    } // if
    else return ;

  } // else if
  else {
    
    bool stop = false;
    ThreeERROR( stop ); // 負責去測三種error
    if ( stop == true ) return ;
  } // else 

  
} // NOT_ID_StartFactor()

void Test_Non_appear( bool &gp ) {
  // 負責檢查有沒有出現不該出現的符號
  if ( gNext_tokINfo.mgpToken_data != "+" && gNext_tokINfo.mgpToken_data != "-"
    &&  gNext_tokINfo.mgpToken_data != "*" && gNext_tokINfo.mgpToken_data != "/"
    && gNext_tokINfo.mgpToken_data != "<>" && gNext_tokINfo.mgpToken_data != ">"
    && gNext_tokINfo.mgpToken_data != "<" && gNext_tokINfo.mgpToken_data != "="
    && gNext_tokINfo.mgpToken_data != ">=" && gNext_tokINfo.mgpToken_data != "<="
    && gNext_tokINfo.mgpToken_data != ";" ) {
    
    bool test = false;
    Twoerror( test );
    if ( test == true ) {
      gp = true; // 代表出現error
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
    return ; // 有error就結束



  bool gp = false;
  // GetTokenButton();
  PeekTokenButton();
  Test_Non_appear( gp );
  if ( gp == true ) return ;

  // GetTokenButton();


  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "*" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Factor(temp_next);
      if ( TestError() == true )
        return ; // 有error就結束

      if ( gFloat_num == true ) {
        temp = temp * temp_next;
      } // if
      else {
        int buffer1 = (int)temp;  // 型別有錯的話要回來檢查!!!!!!!!!!!!!!
        int buffer2 = (int)temp_next;
        temp = ( buffer1 * buffer2 );
      } // else

      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uMul_Divide && gNext_tokINfo.mgpToken_data == "/" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Factor( temp_next );
      if ( TestError() == true )
        return ; // 有error就結束

      // cout << "in";
      if ( gFloat_num == true ) {
        if ( temp_next == 0 ) {
          gDivide_zero = true;

          Message_error(); // 印Error!
          return ;
        } // if
        temp = temp / temp_next; // 正常執行

      } // if
      else {

        int buffer1 = (int)temp;
        int buffer2 = (int)temp_next;
        if ( buffer2 == 0 ) {
          gDivide_zero = true;
          Message_error(); // 印Error!
          return ;
        } // if

        temp = ( buffer1 / buffer2 ); // 正常情況



      } // else

      PeekTokenButton();
    } // else if
    else out_while = true;


  } // while

  
} // NOT_ID_StartTerm()


void NOT_ID_StartArithExp(double &temp) {
  
  NOT_ID_StartTerm(temp);
  if ( TestError() == true )
    return ; // 有error就結束

  PeekTokenButton();

  bool gp = false;
  Test_Non_appear( gp ); // 看有沒有出現不該出現符號
  if ( gp == true ) return ; // 出現不該出現就return 

  bool out_while = false;
  while ( out_while == false ) {
    if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "+" ) {
      GetTokenButton(); // 移動next到now
      double temp_next = 0.0;
      Term( temp_next );
      if ( TestError() == true )
        return ; // 有error就結束
      temp = temp + temp_next;


      PeekTokenButton();
    } // if
    else if ( gNext_tokINfo.mgpToken_type == uUpdown && gNext_tokINfo.mgpToken_data == "-" ) {
      GetTokenButton(); // 移動next到now
      // cout << gNow_tokINfo.mgpToken_data;
      double temp_next = 0.0;
      Term( temp_next );
      // cout << temp_next;
      if ( TestError() == true )
        return ; // 有error就結束

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
  // 回傳true代表進比大小
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
      temp = num; // 處理誤差
      Deal_LME( ope, temp, num, flag);
      return true;
    } // if
    else if ( num - temp <= 0.0001 && num > temp ) {
      num = temp; // 處理誤差
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
  // 回傳true代表進比大小
  PeekTokenButton();
  // cout << gNext_tokINfo.mgpToken_data;
  bool gp = false;

  Test_Non_appear( gp ); // 不該出現
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

  // ========================================================== 以上是while部分

  ope = gNext_tokINfo.mgpToken_data;
  // cout << ope;
  Test_Non_appear( gp );
  if ( gp == true )
    return false; // 出現不該出現的

  // cout << gNext_tokINfo.mgpToken_data;

  if ( BooleanOprator() == false ) {

    if ( gInt == true ) {
      gNum_int = (int)ans; //傳遞方式有點危險出現問題要回來檢查

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
    // 這個分號在這處利不是很好 下次出問題要回來檢查
    
    if ( gNext_tokINfo.mgpToken_data != ";" ) {
      
      bool stop = false;
      ThreeERROR( stop );
      if ( stop == true ) return false;
    } // if

    if ( ans - num <= 0.0001 && ans > num ) {
      ans = num; // 處理誤差
      Deal_LME( ope, ans, num, flag );

    } // if
    else if ( num - ans <= 0.0001 && num > ans ) {
      num = ans; // 處理誤差
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
      cout << fixed << setprecision( 3 ) << gNum_double;   // 取3位
      cout << "\n";
      cout << "> ";
    } // if
    else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

      cout << "0.000";   // 因為取3位都一樣
      cout << "\n";
      cout << "> ";
    } // else if
    else {
      // 小於0的情況
      cout << fixed << setprecision( 3 ) << gNum_double;   // 取3位
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
      num = i; // 記下第幾個相同
      // GetTokenButton(); // 移動next到now
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
        cout << fixed << setprecision( 3 ) << gNum_double;   // 取3位
        cout << "\n";
        cout << "> ";
      } // if
      else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

        cout << "0.000";   // 因為取3位都一樣
        cout << "\n";
        cout << "> ";
      } // else if
      else {
        // 小於0的情況
        cout << fixed << setprecision( 3 ) << gNum_double;   // 取3位
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
        cout << fixed << setprecision( 3 ) << out_put_data.mid_ofValue;   // 取3位
        cout << "\n";
        cout << "> ";
      } // if
      else if ( gNum_double > -0.0001 && gNum_double < 0 ) {

        cout << "0.000";   // 因為取3位都一樣
        cout << "\n";
        cout << "> ";
      } // else if
      else {
        // 小於0的情況
        cout << fixed << setprecision( 3 ) << out_put_data.mid_ofValue;   // 取3位
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
      num = i; // 記下第幾個相同
      
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
    GetTokenButton(); // ID移動到now
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
        ThreeERROR( test ); // 輸出error

      } // if
      else {
        if ( exist == true ) {
          gTokenplace[ num ].mid_ofValue = ans;
          out_put_data.mid_ofValue = ans; // 要輸出之值
          if ( gFloat_num == true ) {
            gTokenplace[ num ].mid_Oftype = uGp_Float;  //決定型別
          } // if
          else if ( gInt == true ) {
            gTokenplace[ num ].mid_Oftype = uGp_Int; //決定型別
          } // else if

        } // if
        else {
          out_put_data.mid_ofName = name;
          out_put_data.mid_ofValue = ans;
          if ( gFloat_num == true ) {
            out_put_data.mid_Oftype = uGp_Float; //決定型別
          } // if
          else if ( gInt == true ) {
            out_put_data.mid_Oftype = uGp_Int; //決定型別
          } // else if

          gTokenplace.push_back( out_put_data ); // 沒存過就存入
        } // else

        bool normal = false;
        Out_put_twotype( normal, out_put_data );
      } // else






      return false;
    } // if
    else {

      
      Tokeninformation gp = gNow_tokINfo;
      bool exist = false;
      double num = ID_exist( gp.mgpToken_data, exist );// num 是gTokenplace名字和gp一樣的值

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

      
      

      Id_Data In; // 為了丟參數而建立的
      In.mid_ofValue = num;

      if ( exist == false ) return false;
      else {
        PeekTokenButton();
        // cout << gNext_tokINfo.mgpToken_data;
        if ( gNext_tokINfo.mgpToken_data == ";" ) {
          bool normal = false; // normal設false代表不是要輸出一般的gINT gfloat
          Out_put_twotype( normal, In );
        } // if

        bool flag = false;
        bool test = IDlessArithExpOrBexp( num, flag ); // test是true的話代表有進比大小

        if ( TestError() == true ) return false;

        PeekTokenButton();
        if ( gNext_tokINfo.mgpToken_data == ";" ) {

          if ( test == true ) {
            Printf_true_false( flag );
            
          } // if
          else {
            In.mid_ofValue = num;
            bool normal = false; // normal設false代表不是要輸出一般的gINT gfloat
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
        // 要輸出true false ;
        Printf_true_false( flag );
        cout << "> ";
        return false;
      } // if
      else {
        // 輸出數值
        Out_put();
        // cout << "> ";
        return false;
      } // else

    } // if
    
  } // else


  return false;
} // Command()



