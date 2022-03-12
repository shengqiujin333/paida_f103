/*****************************************************************************
* Product: "DPP" example on STM32F4-Discovery board, cooperative QV kernel
* Last updated for version 6.9.3
* Last updated on  2021-03-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"  /* CMSIS-compliant header file for the MCU used */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void USART1_IRQHandler(void);
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Local-scope defines -----------------------------------------------------*/
#define LED_GPIO_PORT     GPIOA
#define LED3_PIN          GPIO_PIN_5

static uint32_t l_rnd;    /* random seed */

#ifdef Q_SPY
    static QSTimeCtr QS_tickTime_;
    static QSTimeCtr QS_tickPeriod_;

    /* QSpy source IDs */
    static QSpyId const l_SysTick = { 0U };

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        COMMAND_STAT
    };

#endif
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); 
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
     else if(huart->Instance==USART2)
   {
   /* USER CODE BEGIN USART2_MspInit 0 */

   /* USER CODE END USART2_MspInit 0 */
     /* Peripheral clock enable */
     __HAL_RCC_USART2_CLK_ENABLE();

     __HAL_RCC_GPIOA_CLK_ENABLE();
     /**USART2 GPIO Configuration
     PA2     ------> USART2_TX
     PA3     ------> USART2_RX
     */
     GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
     GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   /* USER CODE BEGIN USART2_MspInit 1 */
     HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(USART2_IRQn);

   /* USER CODE END USART2_MspInit 1 */
   }


}
 static void MX_USART2_UART_Init(void)
 {
 
   /* USER CODE BEGIN USART2_Init 0 */
 
   /* USER CODE END USART2_Init 0 */
 
   /* USER CODE BEGIN USART2_Init 1 */
 
   /* USER CODE END USART2_Init 1 */
   huart2.Instance = USART2;
   huart2.Init.BaudRate = 115200;
   huart2.Init.WordLength = UART_WORDLENGTH_8B;
   huart2.Init.StopBits = UART_STOPBITS_1;
   huart2.Init.Parity = UART_PARITY_NONE;
   huart2.Init.Mode = UART_MODE_TX_RX;
   huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart2.Init.OverSampling = UART_OVERSAMPLING_16;
   if (HAL_UART_Init(&huart2) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN USART2_Init 2 */
 
   /* USER CODE END USART2_Init 2 */
 
 }

 void HAL_MspInit(void)
 {
   /* USER CODE BEGIN MspInit 0 */
 
   /* USER CODE END MspInit 0 */
 
   __HAL_RCC_SYSCFG_CLK_ENABLE();
   __HAL_RCC_PWR_CLK_ENABLE();
 
   /* System interrupt init*/
 
   /* USER CODE BEGIN MspInit 1 */
 
   /* USER CODE END MspInit 1 */
 }


 static void MX_USART1_UART_Init(void)
 {
 
   /* USER CODE BEGIN USART1_Init 0 */
 
   /* USER CODE END USART1_Init 0 */
 
   /* USER CODE BEGIN USART1_Init 1 */
 
   /* USER CODE END USART1_Init 1 */
   huart1.Instance = USART1;
   huart1.Init.BaudRate = 115200;
   huart1.Init.WordLength = UART_WORDLENGTH_8B;
   huart1.Init.StopBits = UART_STOPBITS_1;
   huart1.Init.Parity = UART_PARITY_NONE;
   huart1.Init.Mode = UART_MODE_TX_RX;
   huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart1.Init.OverSampling = UART_OVERSAMPLING_16;
   if (HAL_UART_Init(&huart1) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN USART1_Init 2 */
 
   /* USER CODE END USART1_Init 2 */
 
 }


void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
 {
   if(huart->Instance==USART1)
   {
   /* USER CODE BEGIN USART1_MspDeInit 0 */

   /* USER CODE END USART1_MspDeInit 0 */
     /* Peripheral clock disable */
     __HAL_RCC_USART1_CLK_DISABLE();

     /**USART1 GPIO Configuration
     PA9     ------> USART1_TX
     PA10     ------> USART1_RX
     */
     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

     /* USART1 interrupt DeInit */
     HAL_NVIC_DisableIRQ(USART1_IRQn);
   /* USER CODE BEGIN USART1_MspDeInit 1 */

   /* USER CODE END USART1_MspDeInit 1 */
   }
  else if(huart->Instance==USART2)
   {
   /* USER CODE BEGIN USART2_MspDeInit 0 */
 
   /* USER CODE END USART2_MspDeInit 0 */
     /* Peripheral clock disable */
     __HAL_RCC_USART2_CLK_DISABLE();
 
     /**USART2 GPIO Configuration
     PA2     ------> USART2_TX
     PA3     ------> USART2_RX
     */
     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
     HAL_NVIC_DisableIRQ(USART2_IRQn);
   /* USER CODE BEGIN USART2_MspDeInit 1 */
 
   /* USER CODE END USART2_MspDeInit 1 */
   }

 }


/* ISRs used in this project ===============================================*/
void SysTick_Handler(void) {
    /* state of the button debouncing, see below */
    //static struct ButtonsDebouncing {
    //    uint32_t depressed;
    //    uint32_t previous;
    //} buttons = { 0U, 0U };
    //uint32_t current;
    uint32_t tmp;

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear SysTick_CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    QF_TICK_X(0U, &l_SysTick); /* process time events for tick rate 0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    //current = BTN_GPIO_PORT->IDR; /* read BTN GPIO */
    //tmp = buttons.depressed; /* save the debounced depressed buttons */
    //buttons.depressed |= (buttons.previous & current); /* set depressed */
    //buttons.depressed &= (buttons.previous | current); /* clear released */
    //buttons.previous   = current; /* update the history */
    //tmp ^= buttons.depressed;     /* changed debounced depressed */
    //if ((tmp & BTN_B1) != 0U) {  /* debounced B1 state changed? */
    //    if ((buttons.depressed & BTN_B1) != 0U) { /* is B1 depressed? */
    //        static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
    //        QF_PUBLISH(&pauseEvt, &l_SysTick);
    //    }
    //    else { /* the button is released */
    //        static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
    //        QF_PUBLISH(&serveEvt, &l_SysTick);
    //    }
    //}
    QV_ARM_ERRATUM_838869();
}

/*..........................................................................*/
#ifdef Q_SPY
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
* QK_ISR_EXIT and they cannot post or publish events.
*/
void USART2_IRQHandler(void) {
    if ((USART2->SR & USART_SR_RXNE) != 0) {
        uint32_t b = USART2->DR;
        QS_RX_PUT(b);
    }
    QV_ARM_ERRATUM_838869();
}
#else
void USART2_IRQHandler(void) {}
#endif

void BSP_randomSeed(uint32_t seed);

void Error_Handler(void)
 {
   /* USER CODE BEGIN Error_Handler_Debug */
   /* User can add his own implementation to report the HAL error return state */
   __disable_irq();
   while (1) 
   {
   }
   /* USER CODE END Error_Handler_Debug */
 }


 

void SystemClock_Config(void)
 {
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
 
   /** Configure the main internal regulator output voltage
   */
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
   /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
   RCC_OscInitStruct.LSIState = RCC_LSI_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 4;
   RCC_OscInitStruct.PLL.PLLN = 100;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 4;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
     Error_Handler();
   }
   /** Initializes the CPU, AHB and APB buses clocks
   */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 
   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
   {
     Error_Handler();
   }
 }
 

/* BSP functions ===========================================================*/
void BSP_init(void) {
    GPIO_InitTypeDef GPIO_struct;

    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    HAL_Init();
    SystemClock_Config();
    SystemCoreClockUpdate();
    //HAL_UART_MspInit(USART1);
    MX_USART2_UART_Init();
    uint32_t tempsysclock,temphclk,temppclk1,temppclk2;
    tempsysclock = HAL_RCC_GetSysClockFreq();
    temphclk = HAL_RCC_GetHCLKFreq();
    temppclk1 = HAL_RCC_GetPCLK1Freq();
    temppclk2 = HAL_RCC_GetPCLK2Freq();
    /* configure the FPU usage by choosing one of the options... */
#if 1
    /* OPTION 1:
    * Use the automatic FPU state preservation and the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in more than one task or
    * in any ISRs. This setting is the safest and recommended, but requires
    * extra stack space and CPU cycles.
    */
    FPU->FPCCR |= (1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos);
#else
    /* OPTION 2:
    * Do NOT to use the automatic FPU state preservation and
    * do NOT to use the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in ONE task only and not
    * in any ISR. This setting is very efficient, but if more than one task
    * (or ISR) start using the FPU, this can lead to corruption of the
    * FPU registers. This option should be used with CAUTION.
    */
    FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos));
#endif

    /* Initialize thr port for the LEDs */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* GPIO Configuration for the LEDs... */
    GPIO_struct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_struct.Pull = GPIO_PULLDOWN;
    GPIO_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_struct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    HAL_GPIO_WritePin(LED_GPIO_PORT,LED3_PIN,GPIO_PIN_SET);

    /* seed the random number generator */
    BSP_randomSeed(1234U);
    //printf("hello world\r\n");
    if (QS_INIT((void *)0) == 0U) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
void BSP_ledon(void) {
    HAL_GPIO_WritePin(LED_GPIO_PORT,LED3_PIN,GPIO_PIN_SET);
}
/*..........................................................................*/
void BSP_ledoff(void) {
    HAL_GPIO_WritePin(LED_GPIO_PORT,LED3_PIN,GPIO_PIN_RESET);
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    /* Some flating point code is to exercise the FPU... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

    return (rnd >> 8);
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}



/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assign all priority bits for preemption-prio. and none to sub-prio. */
    //NVIC_SetPriorityGrouping(0U);

    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly, see NOTE00.
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    /* kernel UNAWARE interrupts, see NOTE00 */
    //NVIC_SetPriority(USART2_IRQn,    0U);
    /* ... */

    /* kernel AWARE interrupts, see NOTE00 */
    //NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI);
    /* ... */

    /* enable IRQs... */
#ifdef Q_SPY
    //NVIC_EnableIRQ(USART2_IRQn); /* USART2 interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QV_onIdle(void) { /* CATION: called with interrupts DISABLED, NOTE01 */
    //LED_GPIO_PORT->BSRRL = LED6_PIN; /* turn LED on  */
    __NOP(); /* wait a little to actually see the LED glow */
    __NOP();
    __NOP();
    __NOP();
    //LED_GPIO_PORT->BSRRH = LED6_PIN; /* turn LED off */

#ifdef Q_SPY
    QF_INT_ENABLE();
    QS_rxParse();  /* parse all the received bytes */

    if ((USART2->SR & USART_FLAG_TXE) != 0) { /* TXE empty? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {  /* not End-Of-Data? */
            USART2->DR  = (b & 0xFFU);  /* put into the DR register */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    /* !!!CAUTION!!!
    * The WFI instruction stops the CPU clock, which unfortunately disables
    * the JTAG port, so the ST-Link debugger can no longer connect to the
    * board. For that reason, the call to __WFI() has to be used with CAUTION.
    *
    * NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    * reset the board, then connect with ST-Link Utilities and erase the part.
    * The trick with BOOT(0) is that it gets the part to run the System Loader
    * instead of your broken code. When done disconnect BOOT0, and start over.
    */
    //QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
    QF_INT_ENABLE(); /* just enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*1024]; /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[100];  /* buffer for QS-RX channel */
    GPIO_InitTypeDef GPIO_struct;
    USART_InitTypeDef USART_struct;

    (void)arg; /* avoid the "unused parameter" compiler warning */
    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* enable peripheral clock for USART2 */

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; /* to start the timestamp at zero */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {  /* NOTE: invoked with interrupts DISABLED */
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { /* not set? */
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { /* the rollover occured, but the SysTick_ISR did not run yet */
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;

    QF_INT_DISABLE();
    while ((b = QS_getByte()) != QS_EOD) { /* while not End-Of-Data... */
        QF_INT_ENABLE();
        while ((USART2->SR & USART_FLAG_TXE) == 0) { /* while TXE not empty */
        }
        USART2->DR = (b & 0xFFU); /* put into the DR register */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    NVIC_SystemReset();
}
/*..........................................................................*/
/*! callback function to execute a user command (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    void assert_failed(char const *module, int loc);
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
    QS_BEGIN_ID(COMMAND_STAT, 0U) /* app-specific record */
        QS_U8(2, cmdId);
        QS_U32(8, param1);
        QS_U32(8, param2);
        QS_U32(8, param3);
    QS_END()

    if (cmdId == 10U) {
        Q_ERROR();
    }
    else if (cmdId == 11U) {
        //assert_failed("QS_onCommand", 123);
    }
}


#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


PUTCHAR_PROTOTYPE  
{  
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);  //调用STM32的HAL库，发送一个字节
    return (ch);  
}  




#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE00:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
* are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF services. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE01:
* The QV_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QV_onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE02:
* One of the LEDs is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
