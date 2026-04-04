/* 
  파일이름: main.c
  작 성 자: 남하연, 26-04-05
  하 는 일: 스마트 급여 계신 및 근무 스케줄 관리 시스템 
*/


#include <stdio.h>
#include<stdilb.h>
int main()
{
system("chcp 65001");
//1. 변수 선언 (5개 이상, 3가지 자료형 사용)
chat brand_initial; // 알바 장소 이니셜 (chat)
int houly_wage;  // 기본 시급(int)
int working_hours; // 일일 근무 시간(int)
float meal_allowance; // 식대 지원비(float)
float total_day_pay: // 최종 예상 알급(계산결과)

//2. 프로그램 시작 화면 출력
printf("========================================\n");
printf("스마트 알바 급여 관리 시스템\n");
printf("=========================================\n\n");

//3. 사용자 입력 받기(scanf)
 printf( "알바 장소의 영문 이니셜을 입력하세요:");
 scanf(" %c", &brand_initial); // %c앞 공백 필수...

  printf("현재 본인의 시급(정수)를 입력하세요:");
  scanf("%d", &working_hours);

  printf("오늘 근무한 시간(정수)을 입력하세요:");
  scanf("%d", &working_hours);

  printf("오늘 받은 식대(실수)를 입력하세요:");
  scanf("%f", &meal_allowance);

//4. 간단한 연산 포함 (사칙연산 1회 이상)
  total_day_pay=(float)(hourly_wage*working_hours)+meal_allowance;

//5. 결과 출력
  printf("\n-- [%c] 사업장 근무 결과 ---\n", brand_initial);
  printf("기본 급여: %d원\n", hourly_wage*working_hours);
  printf("식대 지원: %.2f원\n", meal_allowance);
  printf("------------------------------------\n");
  printf("오늘의 최종 예상 일급은 [%.2f]원 입니다.\n", total_day_pay);
  printf("오늘도 고생 많으셨습니다!");
  printf("====================================\n");
  
  return0;
}

