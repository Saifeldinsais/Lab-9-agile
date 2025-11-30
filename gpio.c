#include "gpio.h"
#include "common_macros.h"
#include "tm4c123gh6pm.h"
#include "std_types.h"

#ifndef GPIO_LOCK_KEY
#define GPIO_LOCK_KEY   0x4C4F434B
#endif

static const GpioRegsType gpioRegs[] =
{
    /* GPIO_PORT_A */
    {
        &GPIO_PORTA_DIR_R,
        &GPIO_PORTA_DEN_R,
        &GPIO_PORTA_PUR_R,
        &GPIO_PORTA_PDR_R,
        &GPIO_PORTA_LOCK_R,
        &GPIO_PORTA_CR_R,
        &GPIO_PORTA_DATA_R
    },
    /* GPIO_PORT_B */
    {
        &GPIO_PORTB_DIR_R,
        &GPIO_PORTB_DEN_R,
        &GPIO_PORTB_PUR_R,
        &GPIO_PORTB_PDR_R,
        &GPIO_PORTB_LOCK_R,
        &GPIO_PORTB_CR_R,
        &GPIO_PORTB_DATA_R
    },
    /* GPIO_PORT_C */
    {
        &GPIO_PORTC_DIR_R,
        &GPIO_PORTC_DEN_R,
        &GPIO_PORTC_PUR_R,
        &GPIO_PORTC_PDR_R,
        &GPIO_PORTC_LOCK_R,
        &GPIO_PORTC_CR_R,
        &GPIO_PORTC_DATA_R
    },
    /* GPIO_PORT_D */
    {
        &GPIO_PORTD_DIR_R,
        &GPIO_PORTD_DEN_R,
        &GPIO_PORTD_PUR_R,
        &GPIO_PORTD_PDR_R,
        &GPIO_PORTD_LOCK_R,
        &GPIO_PORTD_CR_R,
        &GPIO_PORTD_DATA_R
    },
    /* GPIO_PORT_E */
    {
        &GPIO_PORTE_DIR_R,
        &GPIO_PORTE_DEN_R,
        &GPIO_PORTE_PUR_R,
        &GPIO_PORTE_PDR_R,
        &GPIO_PORTE_LOCK_R,
        &GPIO_PORTE_CR_R,
        &GPIO_PORTE_DATA_R
    },
    /* GPIO_PORT_F */
    {
        &GPIO_PORTF_DIR_R,
        &GPIO_PORTF_DEN_R,
        &GPIO_PORTF_PUR_R,
        &GPIO_PORTF_PDR_R,
        &GPIO_PORTF_LOCK_R,
        &GPIO_PORTF_CR_R,
        &GPIO_PORTF_DATA_R
    }
};
void GPIO_InitPort(GpioPortType port,
                   uint8 directionMask,
                   uint8 pullUpMask,
                   uint8 pullDownMask)
{
    const GpioRegsType *regs = &gpioRegs[(uint8)port];

    /* Enable clock for the port */
    gpio_enable_clock(port);

    /* Unlock the port if required (for PF0, PD7, PC0–3 on some devices) */
    *(regs->LOCK) = GPIO_LOCK_KEY;
    *(regs->CR) = 0xFF;  // Allow all pins to be configurable

    /* Clear all existing pull configurations */
    *(regs->PUR) = 0x00;
    *(regs->PDR) = 0x00;

    /* Apply direction */
    *(regs->DIR) = directionMask;

    /* Apply pull-up / pull-down masks */
    *(regs->PUR) = pullUpMask;
    *(regs->PDR) = pullDownMask;

    /* Enable digital function */
    *(regs->DEN) = 0xFF;
}


static void gpio_enable_clock(GpioPortType port)
{
    SET_BIT(SYSCTL_RCGCGPIO_R, port);
    (void)SYSCTL_RCGCGPIO_R; // delay 3shan ye3atal el clock shwaya 3shan heya mesh betala3 2erayat in the same cycle
}

void GPIO_InitPin(GpioPortType port, uint8 pin,
                  GpioDirectionType dir, GpioPullType pull)
{
     const GpioRegsType *regs = &gpioRegs[(uint8)port];

    gpio_enable_clock(port);

    // Unlock port if needed (PF0, PD7, etc.)
    *(regs->LOCK) = GPIO_LOCK_KEY;
        SET_BIT(*(regs->CR), pin);


    /* Direction */
    if (dir == GPIO_DIR_OUTPUT)
    {
        SET_BIT(*(regs->DIR), pin);
    }
    else
    {
        CLEAR_BIT(*(regs->DIR), pin);
    }

    /* Pull-up / Pull-down configuration (only for input) */
    CLEAR_BIT(*(regs->PUR), pin);
    CLEAR_BIT(*(regs->PDR), pin);
    if (pull == GPIO_PULL_UP)
    {
        SET_BIT(*(regs->PUR), pin);
    }
    else if (pull == GPIO_PULL_DOWN)
    {
        SET_BIT(*(regs->PDR), pin);
    }

    /* Digital enable */
    SET_BIT(*(regs->DEN), pin);
}

void GPIO_WritePin(GpioPortType port, uint8 pin, uint8 value)
{
const GpioRegsType *regs = &gpioRegs[(uint8)port];

    if (value)
    {
        SET_BIT(*(regs->DATA), pin);
    }
    else
    {
        CLEAR_BIT(*(regs->DATA), pin);
    }
}

uint8 GPIO_ReadPin(GpioPortType port, uint8 pin)
{
    const GpioRegsType *regs = &gpioRegs[(uint8)port];
    return (uint8)BIT_IS_SET(*(regs->DATA), pin);
}

void GPIO_TogglePin(GpioPortType port, uint8 pin)
{
    const GpioRegsType *regs = &gpioRegs[(uint8)port];
    TOGGLE_BIT(*(regs->DATA), pin);
}
