/*
 * File: uart_freertos.c
 * Author: [작성자 이름]
 * Date: [작성일]
 *
 * Description:
 *   AVR ATmega 시리즈에서 FreeRTOS를 사용하여
 *   UART 인터럽트 기반 데이터 수신을 큐로 전달하고,
 *   별도의 태스크에서 처리하는 예제 코드입니다.
 *
 * Features:
 *   - UART RX 인터럽트에서 수신 데이터 큐잉
 *   - FreeRTOS 태스크에서 큐를 통해 데이터 처리
 *   - 예제: 수신된 문자를 printf로 출력
 *
 * Dependencies:
 *   - FreeRTOS (task, queue, semphr)
 *   - AVR-GCC, avr-libc
 *
 * Usage:
 *   1. UART 보드레이트 설정 (예: 9600bps)
 *   2. FreeRTOS 환경에서 빌드 및 실행
 *   3. 시리얼 터미널로 데이터 송신 시, 수신된 문자가 출력됨
 *
 * Note:
 *   - 큐 크기 및 태스크 스택 크기는 필요에 따라 조정 가능
 *   - printf 사용 시, UART 송신 함수 구현 필요
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <avr/io.h>
#include <avr/interrupt.h>

QueueHandle_t uartQueue;

ISR(USART_RX_vect) {
    char data = UDR0;  // UART 데이터 레지스터에서 읽기
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // 큐에 데이터 전달
    xQueueSendFromISR(uartQueue, &data, &xHigherPriorityTaskWoken);

    // 필요시 컨텍스트 스위치
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uartTask(void *pvParameters) {
    char rxData;
    while (1) {
        // 큐에서 데이터 수신 (블로킹)
        if (xQueueReceive(uartQueue, &rxData, portMAX_DELAY) == pdPASS) {
            // 받은 데이터 처리
            printf("수신: %c\n", rxData);
        }
    }
}

int main(void) {
    // UART 초기화 (예: 9600bps)
    UBRR0 = 103;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);

    // 큐 생성
    uartQueue = xQueueCreate(32, sizeof(char));

    // 태스크 생성
    xTaskCreate(uartTask, "UART Task", 256, NULL, 1, NULL);

    // RTOS 시작
    vTaskStartScheduler();

    while(1);
}
