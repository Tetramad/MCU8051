/*
 * MCU : C8051F330 (Silicon labs)
 * IDE : Keil μVision5
 * Describes : Control H-bridge(L293D) using PWM.
 */
#include <c8051F330.h>

#define INT0 0

void switch_isr(void) interrupt INT0
{
  if (P1 & 0x10) // &[0001 0000]
  {
    // STOP
    P1 &= 0xF3; // MID/MAX LED off
    PCA0CN &= 0xBF; // PCA disable
    PCA0L = 0x00;
    PCA0H = 0x00; // [0000 0000 0000 0000]
    PCA0CPL0 = 0x00;
    PCA0CPH0 = 0x00; // [0000 0000 0000 0000]
    PCA0CN |= 0x40; // PCA enable
  }
  else if (P1 & 0x20) // &[0010 0000]
  {
    // MID SPEED
    P1 &= 0xF7; // MAX LED off
    P1 |= 0x04; // MID LED on
    PCA0CN &= 0xBF; // PCA disable
    PCA0L = 0x00;
    PCA0H = 0x00; // [0000 0000 0000 0000]
    PCA0CPL0 = 0x00;
    PCA0CPH0 = 0x80; // [1000 0000 0000 0000]
    PCA0CN |= 0x40; // PCA enable
    
  }
  else if (P1 & 0x40) // &[0100 0000]
  {
    // MAX SPEED
    P1 &= 0xFB; // MID LED off
    P1 |= 0x08; // MAX LED on
    PCA0CN &= 0xBF; // PCA disable
    PCA0L = 0x00;
    PCA0H = 0x00; // [0000 0000 0000 0000]
    PCA0CPL0 = 0xFF;
    PCA0CPH0 = 0xFF; // [1000 0000 0000 0000]
    PCA0CN |= 0x40; // PCA enable
  }
}

void main(void)
{
  PCA0MD = 0x08; // =[0000 1000] disable Watchdog timer and PCA0 use system clock
  PCA0CN = 0x00; // =[0000 0000] initialize PCA0
  PCA0L = 0x00;
  PCA0H = 0x00;
  PCA0CPL0 = 0x00;
  PCA0CPH0 = 0x00;
  PCA0CPM0 = 0x42; // =[0100 0010] PCA becomes 8bit PWM
  
  P0MDIN = 0xFF; // =[1111 1111] set P0.n not analog input
  P1MDIN = 0xFF; // =[1111 1111] set P1.n not analog input
  P0MDOUT = 0xFF; // =[1111 1111] set P0.n push-pull output
  P1MDOUT = 0xFF; // =[1111 1111] set P1.n push-pull output
  P0SKIP = 0xFF; // =[1111 1111] skip for aligning P1.7 to CEX0
  P1SKIP = 0x7F; // =[0111 1111] skip for aligning P1.7 to CEX0
  P0 = 0x00; // =[0000 0000] initialize P0.n
  P1 = 0x82; // =[1000 0010] initialize P1.n
  
  XBR1 = 0x41; // =[0100 0001] crossbar enable and CEX0 routed to Port pin
  
  IT01CF = 0x09; // =[0000 1001] /INT0 becomes active high using P0.1
  TCON = 0x01; // =[0000 0001] /INT0 becomes edge triggered
  IE = 0x81; // =[1000 0001] enable external interrupt 0
  
  while (1)
  {
    PCON |= 0x01; // idle
  }
}
