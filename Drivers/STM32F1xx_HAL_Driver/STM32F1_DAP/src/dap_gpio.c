
#include "stm32f1xx_hal.h"
#include "DAP_config.h"
#include "DAP.h"

extern DAP_Data_t DAP_Data;
 void pin_out_init(GPIO_TypeDef* GPIOx, uint8_t pin_bit)
{
    if(pin_bit >= 8)
    {
        GPIOx->CRH &= ~(0x0000000F << ((pin_bit-8) << 2));
        GPIOx->CRH |= ( ((uint32_t)(0x00|0x03) & 0x0F) << ((pin_bit-8) << 2) );
    }
    else
    {
        GPIOx->CRL &= ~(0x0000000F << ((pin_bit) << 2));
        GPIOx->CRL |= ( ((uint32_t)(0x00|0x03) & 0x0F) << ((pin_bit) << 2) );
    }
}

 void pin_out_od_init(GPIO_TypeDef* GPIOx, uint8_t pin_bit)
{
    if(pin_bit >= 8)
    {
        GPIOx->CRH &= ~(0x0000000F << ((pin_bit-8) << 2));
        GPIOx->CRH |= ( ((uint32_t)(0x04|0x03) & 0x0F) << ((pin_bit-8) << 2) );
    }
    else
    {
        GPIOx->CRL &= ~(0x0000000F << ((pin_bit) << 2));
        GPIOx->CRL |= ( ((uint32_t)(0x04|0x03) & 0x0F) << ((pin_bit) << 2) );
    }
}
 void pin_in_init(GPIO_TypeDef* GPIOx, uint8_t pin_bit, uint8_t mode)
{
/**/
    uint8_t config;
    if(mode == 1)
        config = 0x08; //Up
    else if(mode == 2)
        config = 0x08; //down
    else
        config = 0x00; //GPIO_Mode_AIN

    if(pin_bit >= 8)
    {
        GPIOx->CRH &= ~(0x0000000F << ((pin_bit-8) << 2));
        GPIOx->CRH |= ( ((uint32_t)(config) & 0x0F) << ((pin_bit-8) << 2) );
        if(mode == 1)
            GPIOx->BSRR = (((uint32_t)0x01) << pin_bit);
        else if(mode == 2)
            GPIOx->BRR = (((uint32_t)0x01) << pin_bit);
    }
    else
    {
        GPIOx->CRL &= ~(0x0000000F << ((pin_bit) << 2));
        GPIOx->CRL |= ( ((uint32_t)(config) & 0x0F) << ((pin_bit) << 2) );
        if(mode == 1)
            GPIOx->BSRR = (((uint32_t)0x01) << pin_bit);
        else if(mode == 2)
            GPIOx->BRR = (((uint32_t)0x01) << pin_bit);
    }
}

/** Disable JTAG/SWD I/O Pins.
Disables the DAP Hardware I/O pins which configures:
 - TCK/SWCLK, TMS/SWDIO, TDI, TDO, nTRST, nRESET to High-Z mode.
*/
void PORT_OFF()
{
    pin_in_init(PIN_SWCLK_TCK_PORT, PIN_SWCLK_TCK_PIN, 0);
		pin_in_init(PIN_SWDIO_TMS_PORT, PIN_SWDIO_TMS_PIN, 0);
}

/** Setup SWD I/O pins: SWCLK, SWDIO, and nRESET.
Configures the DAP Hardware I/O pins for Serial Wire Debug (SWD) mode:
 - SWCLK, SWDIO, nRESET to output mode and set to default high level.
 - TDI, TMS, nTRST to HighZ mode (pins are unused in SWD mode).
*/
 void PORT_SWD_SETUP(void)
{
    // Set SWCLK HIGH
    pin_out_init(PIN_SWCLK_TCK_PORT, PIN_SWCLK_TCK_PIN);
    PIN_SWCLK_TCK_PORT->BSRR = PIN_SWCLK_TCK_MASK;
    // Set SWDIO HIGH
    pin_out_init(PIN_SWDIO_TMS_PORT, PIN_SWDIO_TMS_PIN);
    PIN_SWDIO_TMS_PORT->BSRR = PIN_SWDIO_TMS_MASK;

    //pin_in_init(SWDIO_IN_PIN_PORT, SWDIO_IN_PIN_Bit, 1);
    // Set RESET HIGH
//    pin_out_od_init(nRESET_PIN_PORT, nRESET_PIN_Bit);//TODO - fix reset logic
//    nRESET_PIN_PORT->BSRR = nRESET_PIN;
}

/** Setup JTAG I/O pins: TCK, TMS, TDI, TDO, nTRST, and nRESET.
Configures the DAP Hardware I/O pins for JTAG mode:
 - TCK, TMS, TDI, nTRST, nRESET to output mode and set to high level.
 - TDO to input mode.
*/
 void PORT_JTAG_SETUP(void)
{
#if (DAP_JTAG != 0)

#endif
}

//-----Soft reset + Hard reset-------------------------------------------------
#define PIN_SWCLK_SET PIN_SWCLK_TCK_SET
#define PIN_SWCLK_CLR PIN_SWCLK_TCK_CLR

#define RST_CLOCK_CYCLE()                \
  PIN_SWCLK_CLR();                       \
  PIN_DELAY();                           \
  PIN_SWCLK_SET();                       \
  PIN_DELAY()

#define RST_WRITE_BIT(bit)               \
  PIN_SWDIO_OUT(bit);                    \
  PIN_SWCLK_CLR();                       \
  PIN_DELAY();                           \
  PIN_SWCLK_SET();                       \
  PIN_DELAY()

#define RST_READ_BIT(bit)                \
  PIN_SWCLK_CLR();                       \
  PIN_DELAY();                           \
  bit = PIN_SWDIO_IN();                  \
  PIN_SWCLK_SET();                       \
  PIN_DELAY()

#define PIN_DELAY() PIN_DELAY_SLOW(DAP_Data.clock_delay)

uint8_t RST_Transfer(uint32_t request, uint32_t data)
{
  uint32_t ack;                                                                 \
  uint32_t bit;                                                                 \
  uint32_t val;                                                                 \
  uint32_t parity;                                                              \
  uint32_t n;                                                                   \
																																								\
  /* Packet Request */                                                          \
  parity = 0U;                                                                  \
  RST_WRITE_BIT(1U);                     /* Start Bit */                        \
  bit = request >> 0;                                                           \
  RST_WRITE_BIT(bit);                    /* APnDP Bit */                        \
  parity += bit;                                                                \
  bit = request >> 1;                                                           \
  RST_WRITE_BIT(bit);                    /* RnW Bit */                          \
  parity += bit;                                                                \
  bit = request >> 2;                                                           \
  RST_WRITE_BIT(bit);                    /* A2 Bit */                           \
  parity += bit;                                                                \
  bit = request >> 3;                                                           \
  RST_WRITE_BIT(bit);                    /* A3 Bit */                           \
  parity += bit;                                                                \
  RST_WRITE_BIT(parity);                 /* Parity Bit */                       \
  RST_WRITE_BIT(0U);                     /* Stop Bit */                         \
  RST_WRITE_BIT(1U);                     /* Park Bit */                         \
  \
  /* Turnaround */                                                              \
  PIN_SWDIO_OUT_DISABLE();                                                      \
  for (n = DAP_Data.swd_conf.turnaround; n; n--) {                              \
    RST_CLOCK_CYCLE();                                                          \
  }                                                                             \
  \
  /* Acknowledge response */                                                    \
  RST_READ_BIT(bit);                                                            \
  ack  = bit << 0;                                                              \
  RST_READ_BIT(bit);                                                            \
  ack |= bit << 1;                                                              \
  RST_READ_BIT(bit);                                                            \
  ack |= bit << 2;                                                              \
  \
  /* Data transfer */                                                           \
  /* Turnaround */                                                              \
  for (n = DAP_Data.swd_conf.turnaround; n; n--) {                              \
    RST_CLOCK_CYCLE();                                                          \
  }                                                                             \
  PIN_SWDIO_OUT_ENABLE();                                                       \
  /* Write data */                                                              \
  val = data;                                                                   \
  parity = 0U;                                                                  \
  for (n = 32U; n; n--) {                                                       \
    RST_WRITE_BIT(val);              /* Write WDATA[0:31] */                    \
    parity += val;                                                              \
    val >>= 1;                                                                  \
  }                                                                             \
  RST_WRITE_BIT(parity);             /* Write Parity Bit */                     \
  PIN_SWDIO_OUT_ENABLE();                                                       \
  PIN_SWDIO_OUT(1U);                                                            \
  return ((uint8_t)ack);                                                        \
}

void vResetTarget(uint8_t bit)
{
  uint32_t i;
  //soft-reset for Cortex-M
  RST_Transfer(0x00000CC5, 0xE000ED0C); //set AIRCR address
  for (i=0; i<100; i++);
  RST_Transfer(0x00000CDD, 0x05FA0007); //set RESET data
  for (i=0; i<100; i++);
  RST_Transfer(0x00000CC5, 0xE000ED0C); //repeat
  for (i=0; i<100; i++);
  RST_Transfer(0x00000CDD, 0x05FA0007);
  
    if (bit & 1)
        PIN_nRESET_PORT->BSRR = PIN_nRESET_MASK;
    else
        PIN_nRESET_PORT->BRR = PIN_nRESET_MASK;
}


//=============END=====================

