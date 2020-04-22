/**
 * \file main.c
 *
 * \brief Main source file.
 *
 (c) 2019 Microchip Technology Inc. and its subsidiaries.
    Subject to your compliance with these terms, you may use this software and
    any derivatives exclusively with Microchip products. It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

#include <avr/io.h>

#define ON_TIME_START_WOA   0x3FF
#define ON_TIME_CYCLES_WOA  0x199
#define ON_TIME_START_WOB   0x3FF
#define ON_TIME_CYCLES_WOB  0x333

void CLK_Init(void);
void PORT_Init(void);
void TCD_Init(void);

void CLK_Init(void)
{
    /* Set OSCHF as main clock source */
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSCHF_gc);
    
    /* Wait for main clock oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }
    
    /* Change OSCHF frequency to 24 MHz */
    _PROTECTED_WRITE(CLKCTRL.OSCHFCTRLA, CLKCTRL_FREQSEL_24M_gc);
    
    /* Set the multiplication factor for PLL to 2x */
    _PROTECTED_WRITE(CLKCTRL.PLLCTRLA, CLKCTRL_MULFAC_2x_gc);
}

void PORT_Init(void)
{
    /* Configure PORT A PIN4 and PIN5 as output low */
    PORTA.DIRSET |= PIN4_bm | PIN5_bm;
    PORTA.OUTSET |= PIN4_bm | PIN5_bm;
}

void TCD_Init(void)
{
    /* Select the Two Ramp mode */
    TCD0.CTRLB |= TCD_WGMODE_TWORAMP_gc;
    
    /* Load the compare and clear registers */
    TCD0.CMPASET = ON_TIME_START_WOA;
    TCD0.CMPACLR = ON_TIME_START_WOA + ON_TIME_CYCLES_WOA;
    TCD0.CMPBSET = ON_TIME_START_WOB;
    TCD0.CMPBCLR = ON_TIME_START_WOB + ON_TIME_CYCLES_WOB;
    
    /* Enable the PWM channels */
    _PROTECTED_WRITE(TCD0.FAULTCTRL, TCD_CMPAEN_bm | TCD_CMPA_bm
                                   | TCD_CMPBEN_bm | TCD_CMPB_bm);
    
    /* Wait for TCD to be ready for enabling */
    while (!(TCD0.STATUS & TCD_ENRDY_bm))
    {
        ;
    }
    
    /* Select PLL as input clock and enable TCD */
    TCD0.CTRLA |= TCD_CLKSEL_PLL_gc | TCD_ENABLE_bm;
}

int main(void)
{
    CLK_Init();
    PORT_Init();
    TCD_Init();
    
    while (1) 
    {
        ;
    }
}
