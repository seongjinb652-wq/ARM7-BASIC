#include "2450addr.h"

void busyWait(int num) {
  int j = num;
  while (j) {

    j -= 1;
  }

  j = num;
}

void Display_LED(int *num) {

  int i = 0;
  for (i = 4; i < 8; i++) {
    if (num[i] > 0) {
      rGPGDAT &= ~(1 << i); // 4 LED ON
    }
  }

  busyWait(10000);

  for (i = 4; i < 8; i++) {
    if (num[i] > 0) {
      rGPGDAT |= (1 << i); // 4 LED OFF
    }
  }
}

void pressed(int flag) {

  // GPFDAT 2~6  == 1

  // if 0 presssed
  // 4 pressed
  int arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  if (flag == 1) {
    // 전류가 흐르면 rGPFDAT7 이 1이 됍니다.
    // sw 4~ 8번

    if (!(rGPFDAT & (0x1 << 2))) {
      // rGPFDAT == 0 이면
      // sw == 4
      arr[7] = 1;
    }

    if (!(rGPFDAT & (0x1 << 3))) {
      // rGPFDAT == 0 이면
      arr[6] = 1;
    }

    if (!(rGPFDAT & (0x1 << 4))) {
      // rGPFDAT == 0 이면
      arr[7] = 1;
      arr[6] = 1;
    }

    if (!(rGPFDAT & (0x1 << 5))) {
      // rGPFDAT == 0 이면
      arr[5] = 1;
    }

    if (!(rGPFDAT & (0x1 << 6))) {
      // rGPFDAT == 0 이면
      arr[5] = 1;
      arr[7] = 1;
    }
  } else {
    // sw 9~ 13번
    // 전류가 흐르면 rGPFDAT7 이 1이 됍니다.
    // sw 4~ 8번

    if (!(rGPFDAT & (0x1 << 2))) {
      // rGPFDAT == 0 이면
      // sw == 4
      arr[5] = arr[6] = 1;
    }

    if (!(rGPFDAT & (0x1 << 3))) {
      // rGPFDAT == 0 이면
      arr[5] = arr[6] = arr[7] = 1;
    }

    if (!(rGPFDAT & (0x1 << 4))) {
      // rGPFDAT == 0 이면
      arr[4] = 1;
    }

    if (!(rGPFDAT & (0x1 << 5))) {
      // rGPFDAT == 0 이면
      arr[4] = arr[7] = 1;
    }

    if (!(rGPFDAT & (0x1 << 6))) {
      // rGPFDAT == 0 이면
      arr[4] = arr[6] = 1;
    }
  }

  Display_LED(arr);
}

void Init_key() {

  // LED
  rGPGDAT |= 0xf0;
  // rGPGCON = 0;     // 0 setting
  // rGPGCON |= 0x50; // output mode
  // rGPGDAT = 0;     // 0 setting
  // rGPGDAT |= 0xf0; // 4~7 bit setting

  // BUTTON
  // input mode
  rGPFCON &= ~(0xf << 14); //  15:14
  rGPFCON &= ~(0xf << 12); // 13:12
  rGPFCON &= ~(0xf << 10); // 11:10
  rGPFCON &= ~(0xf << 8);  // 9:8
  rGPFCON &= ~(0xf << 6);  // 7:6
  rGPFCON &= ~(0xf << 4);

  rGPFCON |= (0x1 << 14); // GPFCON7 을 1로 설정
  rGPGCON |= (0x1);          //  GPGCON0  을 1로 설정
}

void Main() {
  Init_key();

  int flag = 1;
  // int num[] = {0,0,0,0,1,0,1,0};
  // Display_LED(num);
  while (1) {

    pressed(flag);

    if (flag == 1) {
      rGPFDAT |= (1 << 7);
      rGPGDAT &= ~(0x1);
      flag = 0;
    } else {
      rGPFDAT &= ~(1 << 7);
      rGPGDAT |= 0x1;
      flag = 1;
    }
  }
}