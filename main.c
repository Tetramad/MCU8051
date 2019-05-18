#include <c8051f330.h>

#define REGGET(reg, pos) (((reg) >> (pos)) & 0x01)
#define REGCLR(reg, pos) ((reg) &= ~(1 << (pos)))
#define REGSET(reg, pos) ((reg) |= 1 << (pos))

#define interrupt
#define INT0
#define sbit struct sbit_t {}
#define byte unsigned char
#define __void int
#define __return return 0;

union regiter
{
  sbit bit[8];
  byte reg;
};

void button_isr(void) interrupt INT0;

__void main(void)
{

  __return;
}

void button_isr(void) interrupt INT0
{

}
