/*==================================================================================*/
/*-- <выражение>::= <слагаемое> {+ <слагаемое>} | <слагаемое> {- <слагаемое>}     --*/
/*-- <слагаемое>::= <множитель^> {* <множитель^>} | <множитель^> {/ <множитель^>} --*/
/*-- <множитель^>::= <множитель> {^ <множитель>}                                  --*/
/*-- <множитель>::= 0|1|2|3|4|5|6|7|8|9| (<выражение>)                            --*/
/*==================================================================================*/

#include<stdio.h>
#include<setjmp.h>

jmp_buf begin;
char curlex;

void getlex(void);
int expr(void);
int add(void);
int mult(void);
void error();
int mult_s(void);
int in_step(int a, int b);

/*=====================================================*/
/*----Выделяет из входного потока очередную лексему----*/
/*=====================================================*/
void getlex()
  {
    while ( (curlex=getchar()) == ' ');
  }

/*=====================================================*/
/*--Сообщает об ошибке и передает управление на begin--*/
/*=====================================================*/
void error(void)
  {
    printf("\nОШИБКА!\n");
    while (getchar()!='\n');
    longjmp(begin,1);
  }

/*=====================================================*/
/*----Распознает выражение и вычисляет его значение----*/
/*=====================================================*/
int expr()
  {
    int e=add();
    while (curlex == '+' || curlex == '-')
      {
        if (curlex == '+')
          {
            getlex();
            e+=add();
          }
        else
          {
            getlex();
            e-=add();
          }
      }
    return e;
  }

/*=====================================================*/
/*----Распознает слагаемое и вычисляет его значение----*/
/*=====================================================*/
int add()
{
  int a=mult_s(), d=0;
  while (curlex == '*' || curlex == '/')
    {
      if (curlex == '*')
        {
          getlex();
          a*=mult_s();
        }
      else
        {
          getlex();
          d=mult_s();
          if (d != 0)
            a/=d;
          else
            error();
        }
    }
  return a;
}

/*=====================================================*/
/*-----------------Выдает а в степени b----------------*/
/*=====================================================*/
int in_step(int a, int b)
  {
    int i=0, res=1;
    if (a==0 && b==0)
      error();
    for (;i<b;i++)
      res*=a;
    return res;
  }

/*=====================================================*/
/*-----Распознает степень и вычисляет его значение-----*/
/*=====================================================*/
int mult_s()
  {
    int s=mult();
    while (curlex == '^')
      {
        getlex();
        s=in_step(s, mult_s());
      }
    return s;
  }

/*=====================================================*/
/*----Распознает множитель и вычисляет его значение----*/
/*=====================================================*/
int mult()
{
  int m;
  switch(curlex){

    case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': m=curlex-'0'; break;

    case '(': getlex(); m=expr();
              if (curlex == ')') break;

    default : error();
  }
  getlex();
  return m;
}

/*=====================================================*/
/*------------------------Main-------------------------*/
/*=====================================================*/
int main()
{
  int result;
  setjmp(begin);
  printf("==>");

  getlex();
  result=expr();
  if (curlex != '\n')
    error();
  printf("\n%d\n",result);
  return 0;
}
