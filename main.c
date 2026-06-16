/* 파일이름: main.c
   작 성 자: 남하연
   하 는 일: 스마트 급여 계산 및 근무 스케줄 관리 시스템 (V4.0 최종본)
   업데이트: 구조체 및 배열을 활용한 다중 사업장 통합 관리 시스템 구축
*/

#include <stdio.h>
#include <string.h>

#define MAX_STORES 5  // 최대 관리 가능한 알바 장소 개수

// [V4.0 핵심] 알바 장소별 데이터를 묶는 구조체 정의
typedef struct {
    char brand_initial;      // 사업장 이니셜
    int hourly_wage;         // 시급
    int total_hours;         // 이 사업장에서 근무한 총 시간
    float total_pay;         // 이 사업장에서 정산된 총 급여
} Store;

// --- 전역 변수 선언 ---
Store g_stores[MAX_STORES];  // [V4.0] 다중 사업장 관리를 위한 구조체 배열
int g_store_count = 0;       // 현재 등록된 사업장 개수
float g_total_monthly_pay = 0.0f;  // 전 사업장 총 누적 급여
int g_total_monthly_hours = 0;     // 전 사업장 총 누적 근무 시간

// --- 함수 프로토타입 선언 ---
int find_store(char brand);
void register_or_update_pay();
void print_monthly_report();
void reset_all_data();

int main() 
{
    int menu_choice = 0;

    while (1) {
        printf("\n=========================================\n");
        printf(" 📊 스마트 다중 알바 급여 관리 시스템 (V4.0)\n");
        printf("=========================================\n");
        printf(" 1. 당일 근무 입력 및 급여 정산\n");
        printf(" 2. 사업장별 월간 통합 리포트 조회\n");
        printf(" 3. 전체 데이터 초기화\n");
        printf(" 4. 프로그램 종료\n");
        printf("=========================================\n");
        printf("▶ 메뉴를 선택하세요: ");
        
        if (scanf("%d", &menu_choice) != 1) {
            printf("⚠️ 숫자로 입력해 주세요!\n");
            while (getchar() != '\n'); 
            continue;
        }
        while (getchar() != '\n'); 

        if (menu_choice == 4) {
            printf("\n프로그램을 종료합니다. 스마트한 경제 생활을 응원합니다! 👋\n");
            break;
        }

        switch (menu_choice) {
            case 1:
                register_or_update_pay();
                break;
            case 2:
                print_monthly_report();
                break;
            case 3:
                reset_all_data();
                break;
            default:
                printf("⚠️ 올바른 메뉴 번호(1~4)를 선택해 주세요.\n");
        }
    }

    return 0;
}

// 입력된 이니셜이 이미 등록된 사업장인지 검색하는 함수 (찾으면 인덱스 반환, 없으면 -1)
int find_store(char brand) {
    for (int i = 0; i < g_store_count; i++) {
        if (g_stores[i].brand_initial == brand) {
            return i;
        }
    }
    return -1;
}

// [V4.0] 1번 메뉴: 근무 정보를 입력받아 구조체 배열에 누적 및 정산하는 함수
void register_or_update_pay() {
    char brand;
    int wage, hours, weekly_hours, shift;
    float meal;
    
    printf("\n[정보 입력] 알바 장소의 영문 이니셜: ");
    scanf(" %c", &brand);

    int idx = find_store(brand);
    
    // 신규 사업장인데 이미 슬롯이 가득 찬 경우 예외 처리
    if (idx == -1 && g_store_count >= MAX_STORES) {
        printf("❌ [오류] 최대 %d개까지만 사업장을 등록할 수 있습니다.\n", MAX_STORES);
        while (getchar() != '\n');
        return;
    }

    // 기존 등록 매장인 경우 시급 정보를 가져오고, 신규면 새로 입력 받음
    if (idx != -1) {
        printf("💡 기등록된 사업장입니다. (기존 등록 시급: %d원)\n", g_stores[idx].hourly_wage);
        wage = g_stores[idx].hourly_wage;
    } else {
        printf("[정보 입력] 이 장소의 기본 시급(정수): ");
        scanf("%d", &wage);
    }

    printf("[정보 입력] 오늘 근무한 시간(정수): ");
    scanf("%d", &hours);

    printf("[정보 입력] 이번 주 누적 근무 시간(주휴수당 확인용): ");
    scanf("%d", &weekly_hours);

    printf("[정보 입력] 근무 시간대 (1:주간, 2:야간): ");
    scanf("%d", &shift);

    printf("[정보 입력] 오늘 지원받은 식대(실수): ");
    scanf("%f", &meal);
    while (getchar() != '\n'); 

    // 데이터 무결성 예외 처리 검증
    if (wage < 0 || hours < 0 || weekly_hours < 0 || (shift != 1 && shift != 2)) {
        printf("\n❌ [오류] 잘못된 값이 입력되었습니다. 메인 메뉴로 돌아갑니다.\n");
        return;
    }

    // --- 수당 및 급여 연산 ---
    float extra_pay = 0.0f;
    float night_premium = 0.0f;

    if (hours > 8) {
        extra_pay = (hours - 8) * wage * 0.5f;
    }
    if (shift == 2) {
        night_premium = hours * (wage * 0.5f);
    }

    float final_day_pay = (float)(wage * hours) + extra_pay + night_premium + meal;

    // --- 구조체 배열에 데이터 기록/업데이트 ---
    if (idx == -1) {
        // 완전히 새로운 알바 장소 추가
        g_stores[g_store_count].brand_initial = brand;
        g_stores[g_store_count].hourly_wage = wage;
        g_stores[g_store_count].total_hours = hours;
        g_stores[g_store_count].total_pay = final_day_pay;
        g_store_count++;
    } else {
        // 기존 알바 장소에 시간과 급여 누적 업데이트
        g_stores[idx].total_hours += hours;
        g_stores[idx].total_pay += final_day_pay;
    }

    // 전체 총합 변수에도 누적
    g_total_monthly_hours += hours;
    g_total_monthly_pay += final_day_pay;

    // 명세서 영수증 출력
    printf("\n==========================================\n");
    printf("       [%c] 사업장 당일 급여 정산 완료\n", brand);
    printf("------------------------------------------\n");
    printf(" 1. 기본 급여    : %d원\n", wage * hours);
    if (extra_pay > 0) printf(" 2. 연장 수당(+) : %.0f원 (8시간 초과)\n", extra_pay);
    if (night_premium > 0) printf(" 3. 야간 가산(+) : %.0f원 (야간 근무)\n", night_premium);
    printf(" 4. 식대 지원(+) : %.0f원\n", meal);
    printf("------------------------------------------\n");
    printf(" ▶ 당일 예상 일급: %.0f원\n", final_day_pay);
    printf("------------------------------------------\n");

    if (weekly_hours >= 15) {
        printf("📢 주 15시간 이상 근무 상태로 [주휴수당] 지급 대상입니다!\n");
    } else {
        printf("💡 주휴수당 기준(주 15시간) 미달 상태입니다.\n");
    }
    printf("==========================================\n");
}

// [V4.0] 2번 메뉴: 배열에 기록된 데이터를 순회하며 대시보드 리포트를 출력하는 함수
void print_monthly_report() {
    printf("\n==================================================\n");
    printf("       📊 이번 달 사업장별 통합 월간 리포트\n");
    printf("==================================================\n");
    
    if (g_store_count == 0) {
        printf(" 📭 아직 등록된 근무 데이터가 존재하지 않습니다.\n");
        printf("==================================================\n");
        return;
    }

    printf(" [매장]    [기본시급]    [누적시간]     [누적정산급여]\n");
    printf("--------------------------------------------------\n");
    
    // 구조체 배열 순회 처리
    for (int i = 0; i < g_store_count; i++) {
        printf("   %c       %6d원      %3d시간      %10.0f원\n", 
               g_stores[i].brand_initial, 
               g_stores[i].hourly_wage, 
               g_stores[i].total_hours, 
               g_stores[i].total_pay);
    }
    
    printf("--------------------------------------------------\n");
    printf(" 📅 총 합산 근무 시간 : %d 시간\n", g_total_monthly_hours);
    printf(" 💰 월간 총 예상 급여 : %.0f 원\n", g_total_monthly_pay);
    printf("==================================================\n");
}

// 3번 메뉴: 전체 메모리 구조체 초기화 함수
void reset_all_data() {
    g_total_monthly_pay = 0.0f;
    g_total_monthly_hours = 0;
    g_store_count = 0;
    
    // 구조체 배열 메모리 안전하게 밀어버리기
    memset(g_stores, 0, sizeof(g_stores));
    
    printf("\n♻️ 모든 사업장 정보 및 누적 통계 데이터가 클리어되었습니다.\n");
}
