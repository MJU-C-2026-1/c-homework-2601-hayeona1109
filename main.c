/* 파일이름: main.c
  작 성 자: 남하연
  하 는 일: 스마트 급여 계산 및 근무 스케줄 관리 시스템 (V3.0)
  업데이트: while(1) 무한 루프, switch-case 메뉴, 함수 분리(매개변수/반환값), 전역 변수 누적 기능 추가
*/

#include <stdio.h>

// [요구사항 5] 전역 변수 선언 (프로그램 전체에서 유지되며 함수 내부에서 업데이트됨)
float g_total_monthly_pay = 0.0f;  // 누적 월별 급여
int g_total_monthly_hours = 0;     // 누적 월별 근무 시간

// --- 함수 프로토타입 선언 ---
int input_data(char *brand, int *wage, int *hours, int *weekly_hours, int *shift, float *meal);
float calculate_day_pay(int wage, int hours, int shift, float *extra, float *night);
void print_and_accumulate(char brand, int wage, int hours, int weekly_hours, float extra, float night, float meal, float total_day);

int main() 
{
    int menu_choice;
    
    // 기능 1(입력)에 필요한 데이터를 담을 지역 변수들
    char brand_initial;
    int hourly_wage, working_hours, weekly_working_hours, shift_type;
    float meal_allowance;
    
    // 기능 2(연산)에 필요한 데이터를 담을 지역 변수들
    float extra_pay = 0.0f;
    float night_premium = 0.0f;
    float total_day_pay = 0.0f;

    // [요구사항 1] while(1) 무한 루프를 사용한 메인 메뉴 구성
    while (1) {
        printf("\n=========================================\n");
        printf("   스마트 알바 급여 관리 시스템 (V3.0)\n");
        printf("=========================================\n");
        printf(" 1. 당일 급여 입력 및 계산\n");
        printf(" 2. 이번 달 누적 급여/시간 조회\n");
        printf(" 3. 누적 데이터 초기화\n");
        printf(" 4. 프로그램 종료\n");
        printf("=========================================\n");
        printf("▶ 메뉴를 선택하세요: ");
        
        // 문자 입력 시 무한 루프 방지 처리
        if (scanf("%d", &menu_choice) != 1) {
            printf("⚠️ 숫자로 입력해 주세요!\n");
            while (getchar() != '\n'); // 입력 버퍼 비우기
            continue;
        }

        // [요구사항 1] 사용자가 '4. 종료'를 선택할 때만 break로 프로그램 종료
        if (menu_choice == 4) {
            printf("\n프로그램을 종료합니다. 한 달 동안 고생 많으셨습니다! 👋\n");
            break;
        }

        switch (menu_choice) {
            case 1:
                // [요구사항 2] 함수 호출 위주로 메인 로직을 간결하게 구성 (목차처럼!)
                // 입력 함수 호출 (포인터를 매개변수로 전달하여 값 변경)
                if (input_data(&brand_initial, &hourly_wage, &working_hours, &weekly_working_hours, &shift_type, &meal_allowance) == 0) {
                    break; // 유효성 검사 실패 시 메뉴로 리턴
                }
                
                // 연산 함수 호출 (매개변수 전달 및 반환값 접수)
                total_day_pay = calculate_day_pay(hourly_wage, working_hours, shift_type, &extra_pay, &night_premium);
                
                // 출력 및 전역 변수 누적 함수 호출
                print_and_accumulate(brand_initial, hourly_wage, working_hours, weekly_working_hours, extra_pay, night_premium, meal_allowance, total_day_pay);
                break;

            case 2:
                // 누적 조회 화면
                printf("\n=========================================\n");
                printf("        📊 이번 달 통합 누적 현황\n");
                printf("=========================================\n");
                printf(" 📅 총 누적 근무 시간 : %d 시간\n", g_total_monthly_hours);
                printf(" 💰 총 예상 누적 급여 : %.0f 원\n", g_total_monthly_pay);
                printf("=========================================\n");
                break;

            case 3:
                // 전역 변수 값 초기화 로직
                g_total_monthly_pay = 0.0f;
                g_total_monthly_hours = 0;
                printf("\n♻️ 이번 달 누적 데이터가 초기화되었습니다.\n");
                break;

            default:
                printf("⚠️ 올바른 메뉴 번호(1~4)를 선택해 주세요.\n");
        }
    }

    return 0;
}

// --- 함수 구체적 정의 [요구사항 2, 3, 4, 5] ---

/*
  1. input_data: 사용자로부터 근무 정보를 입력받는 함수
  - 주소(포인터)를 매개변수로 받아 main의 지역변수 값을 직접 바꿈
  - 유효성 검사 성공 시 1, 실패 시 0을 반환 (반환값 활용)
*/
int input_data(char *brand, int *wage, int *hours, int *weekly_hours, int *shift, float *meal) {
    printf("\n[정보 입력] 알바 장소의 영문 이니셜: ");
    scanf(" %c", brand);

    printf("[정보 입력] 현재 본인의 시급(정수): ");
    scanf("%d", wage);

    printf("[정보 입력] 오늘 근무한 시간(정수): ");
    scanf("%d", hours);

    printf("[정보 입력] 이번 주 근무한 총 시간(정수): ");
    scanf("%d", weekly_hours);

    printf("[정보 입력] 근무 시간대 (1:주간, 2:야간): ");
    scanf("%d", shift);

    printf("[정보 입력] 오늘 받은 식대(실수): ");
    scanf("%f", meal);

    // [조건 분기] 입력값 유효성 검사 (방어적 프로그래밍)
    if (*wage < 0 || *hours < 0 || *weekly_hours < 0 || (*shift != 1 && *shift != 2)) {
        printf("\n❌ [오류] 입력값이 유효하지 않습니다. 메인 메뉴로 돌아갑니다.\n");
        return 0; // 실패 반환
    }
    return 1; // 성공 반환
}

/*
  2. calculate_day_pay: 각종 수당을 계산하고 최종 일급을 반환하는 함수
  - 매개변수로 데이터값을 전달받음
  - return 문을 통해 최종 일급(float)을 정확히 명시하여 반환
*/
float calculate_day_pay(int wage, int hours, int shift, float *extra, float *night) {
    // 수당 초기화
    *extra = 0.0f;
    *night = 0.0f;

    // 연장 근로 수당 계산 (8시간 초과 시 0.5배 가산)
    if (hours > 8) {
        *extra = (hours - 8) * wage * 0.5f;
    }

    // 야간 근로 수당 계산 (야간 선택 시 0.5배 가산)
    if (shift == 2) {
        *night = hours * (wage * 0.5f);
    }

    // 최종 일급 연산 후 반환
    return (float)(wage * hours) + *extra + *night;
}

/*
  3. print_and_accumulate: 명세서를 출력하고 전역 변수를 업데이트하는 함수
  - 매개변수로 전달받은 결과를 화면에 가독성 높게 출력
  - 함수 내부에서 전역 변수(g_total_monthly_...)의 값을 직접 누적 변경함
*/
void print_and_accumulate(char brand, int wage, int hours, int weekly_hours, float extra, float night, float meal, float total_day) {
    // 최종 예상 일급 계산 (식대 포함)
    float final_pay_with_meal = total_day + meal;

    // [요구사항 5] 함수 내부에서 전역 변수의 값을 직접 변경(업데이트)하는 로직
    g_total_monthly_pay += final_pay_with_meal;
    g_total_monthly_hours += hours;

    printf("\n==========================================\n");
    printf("       [%c] 사업장 당일 급여 명세서\n", brand);
    printf("------------------------------------------\n");
    printf(" 1. 기본 급여    : %d원\n", wage * hours);
    
    if (extra > 0) 
        printf(" 2. 연장 수당(+) : %.0f원 (8시간 초과)\n", extra);
    
    if (night > 0) 
        printf(" 3. 야간 가산(+) : %.0f원 (야간 수당 적용)\n", night);
    
    printf(" 4. 식대 지원(+) : %.2f원\n", meal);
    printf("------------------------------------------\n");
    printf(" ▶ 당일 예상 일급: %.0f원 (식대 포함)\n", final_pay_with_meal);
    printf("------------------------------------------\n");

    // 주휴수당 및 격려 메시지 조건 분기
    if (weekly_hours >= 15) {
        printf("📢 주 15시간 이상 근무로 [주휴수당] 지급 대상입니다!\n");
    } else {
        printf("💡 주 15시간 미만 근무로 주휴수당 대상이 아닙니다.\n");
    }

    if (final_pay_with_meal >= 100000) {
        printf("💰 오늘 수익이 높네요! 맛있는 거 사 드세요.\n");
    } else {
        printf("🌱 오늘도 고생하셨습니다. 내일도 화이팅!\n");
    }
    printf("==========================================\n");
    printf("ℹ️ 당일 급여와 근무 시간이 이번 달 총액에 누적되었습니다.\n");
}