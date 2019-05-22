#include <c8051f330.h>

#define REGGET(reg, pos) (((reg) >> (pos)) & 0x01)
#define REGCLR(reg, pos) ((reg) &= ~(1 << (pos)))
#define REGSET(reg, pos) ((reg) |= 1 << (pos))

#define _SWITCH_INTERRUPT_FIXED_PORT_0_PIN_0_

#define LED_LVL1_PORT 0
#define LED_LVL1_PIN 1
#define LED_LVL2_PORT 0
#define LED_LVL2_PIN 2
#define LED_LVL3_PORT 0
#define LED_LVL3_PIN 3
#define LED_POWER_PORT 0
#define LED_POWER_PIN 4
#define LED_RF_PORT 0
#define LED_RF_PIN 5
#define SW_LVL1_PORT 1
#define SW_LVL1_PIN 1
#define SW_LVL2_PORT 1
#define SW_LVL2_PIN 2
#define SW_LVL3_PORT 1
#define SW_LVL3_PIN 3

#define _PORT(port) P##port
#define _PORTIN(port) P##port##MDIN
#define _PORTOUT(port) P##port##MDOUT
#define PORT(port) _PORT(port)
#define PORTIN(port) _PORTIN(port)
#define PORTOUT(port) _PORTOUT(port)

#define interrupt
#define INT0
#define __void int
#define __return return 0;

unsigned char switch_register = 0x00;

void button_isr(void) interrupt INT0;

__void main(void)
{
  // Interrupt Setting
  REGCLR(IE, 7); // disable all interrupt sources.
  REGCLR(IE, 0); // disable /INT0 interrupt.
  REGSET(IT01CF, 3); // /INT0 interrupt set active high.
  IT01CF &= ~0x03; // /INT0 Port Pin reset, set P0.0
  REGSET(IE, 0); // enable /INT0 interrupt.
  
  // I/O Port Setting
  REGCLR(XBR1, 6); // crossbar disable
  P0 = 0x00; // Set all pin in P0 low
  P1 = 0x00; // Set all pin in P1 low
  REGSET(PORTIN(LED_LVL1_PORT), LED_LVL1_PIN); // Input mode for digital input.
  REGSET(PORTIN(LED_LVL2_PORT), LED_LVL2_PIN);
  REGSET(PORTIN(LED_LVL3_PORT), LED_LVL3_PIN);
  REGSET(PORTIN(LED_POWER_PORT), LED_POWER_PIN);
  REGSET(PORTIN(LED_RF_PORT), LED_RF_PIN);
  REGSET(PORTIN(SW_LVL1_PORT), SW_LVL1_PIN);
  REGSET(PORTIN(SW_LVL2_PORT), SW_LVL2_PIN);
  REGSET(PORTIN(SW_LVL3_PORT), SW_LVL3_PIN);
  REGSET(PORTOUT(LED_LVL1_PORT), LED_LVL1_PIN); // Output mode for push-pull.
  REGSET(PORTOUT(LED_LVL2_PORT), LED_LVL2_PIN);
  REGSET(PORTOUT(LED_LVL3_PORT), LED_LVL3_PIN);
  REGSET(PORTOUT(LED_POWER_PORT), LED_POWER_PIN);
  REGSET(PORTOUT(LED_RF_PORT), LED_RF_PIN);
  REGCLR(PORTOUT(SW_LVL1_PORT), SW_LVL1_PIN); // Output mode for open-drain.
  REGCLR(PORTOUT(SW_LVL2_PORT), SW_LVL2_PIN);
  REGCLR(PORTOUT(SW_LVL3_PORT), SW_LVL3_PIN);
  REGSET(XBR1, 6); // crossbar enable

  REGSET(PORT(LED_POWER_PORT), LED_POWER_PIN); // initializing done.
  switch_register = 1;
  REGSET(PORT(LED_LVL1_PORT), LED_LVL1_PIN); // set lvl1
  REGSET(IE, 7); // enable interrupt sources.

  while (1); // main looping

  __return;
}

void button_isr(void) interrupt INT0
{
  if (REGGET(PORT(SW_LVL1_PORT), SW_LVL1_PIN))
  {
    switch_register = 1
    REGSET(PORT(LED_LVL1_PORT), LED_LVL1_PIN);
    REGCLR(PORT(LED_LVL2_PORT), LED_LVL2_PIN);
    REGCLR(PORT(LED_LVL3_PORT), LED_LVL3_PIN);
  }
  else if (REGGET(PORT(SW_LVL2_PORT), SW_LVL2_PIN))
  {
    switch_register = 2
    REGCLR(PORT(LED_LVL1_PORT), LED_LVL1_PIN);
    REGSET(PORT(LED_LVL2_PORT), LED_LVL2_PIN);
    REGCLR(PORT(LED_LVL3_PORT), LED_LVL3_PIN);
  }
  else if (REGGET(PORT(SW_LVL3_PORT), SW_LVL3_PIN))
  {
    switch_register = 3
    REGCLR(PORT(LED_LVL1_PORT), LED_LVL1_PIN);
    REGCLR(PORT(LED_LVL2_PORT), LED_LVL2_PIN);
    REGSET(PORT(LED_LVL3_PORT), LED_LVL3_PIN);
  }
  else
  {
    switch_register = 0
    REGCLR(PORT(LED_LVL1_PORT), LED_LVL1_PIN);
    REGCLR(PORT(LED_LVL2_PORT), LED_LVL2_PIN);
    REGCLR(PORT(LED_LVL3_PORT), LED_LVL3_PIN);
  }
}
