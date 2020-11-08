// Adapted from https://github.com/ehayon/BeagleBone-GPIO/blob/master/src/gpio.c

#include "GPIOInterface.h"

#include <iostream>
#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


/* Definitions for GPIO */
#define MMAP_OFFSET (0x44C00000)
#define MMAP_SIZE   (0x481AEFFF-MMAP_OFFSET)

/* GPIO Memory Registers */
#define GPIO_REGISTER_SIZE (4)

#define GPIO0 	(0x44E07000)
#define GPIO1	(0x4804C000)
#define GPIO2	(0x481AC000)
#define GPIO3	(0x481AE000)

#define GPIO_CLEARDATAOUT (0x190)
#define GPIO_SETDATAOUT   (0x194)
#define GPIO_OE			  (0x134)
#define GPIO_DATAOUT      (0x13C)
#define GPIO_DATAIN       (0x138)

/* ***** Define GPIO Pins ***** */
const GPIOInterface::Pin GPIOInterface::USR0  = { "GPIO1_21",    GPIO1,     0,   21,   "",				 0};
const GPIOInterface::Pin GPIOInterface::USR1  = { "GPIO1_22",    GPIO1,     0,   22,   "",				 0};
const GPIOInterface::Pin GPIOInterface::USR2  = { "GPIO1_23",    GPIO1,     0,   23,   "",				 0};
const GPIOInterface::Pin GPIOInterface::USR3  = { "GPIO1_24",    GPIO1,     0,   24,   "",				 0};
const GPIOInterface::Pin GPIOInterface::P8_3  = { "GPIO1_6",	 GPIO1,     38, 	6,	 "gpmc_ad6",        26};
const GPIOInterface::Pin GPIOInterface::P8_4  = { "GPIO1_7",     GPIO1,     39, 	7,	 "gpmc_ad7",        27};
const GPIOInterface::Pin GPIOInterface::P8_5  = { "GPIO1_2",     GPIO1,     34, 	2,	 "gpmc_ad2",        22};
const GPIOInterface::Pin GPIOInterface::P8_6  = { "GPIO1_3",     GPIO1,     35, 	3,	 "gpmc_ad3",        23};
const GPIOInterface::Pin GPIOInterface::P8_7  = { "TIMER4",      GPIO2,     66, 	2, 	 "gpmc_advn_ale",   41};
const GPIOInterface::Pin GPIOInterface::P8_8  = { "TIMER7",      GPIO2,     67, 	3,	 "gpmc_oen_ren",    44};
const GPIOInterface::Pin GPIOInterface::P8_9  = { "TIMER5",      GPIO2,     69, 	5,	 "gpmc_ben0_cle",   42};
const GPIOInterface::Pin GPIOInterface::P8_10 = { "TIMER6",      GPIO2,     68, 	4,	 "gpmc_wen",        43};
const GPIOInterface::Pin GPIOInterface::P8_11 = { "GPIO1_13",    GPIO1,     45, 	13,	 "gpmc_ad13",       29};
const GPIOInterface::Pin GPIOInterface::P8_12 = { "GPIO1_12",    GPIO1,     44, 	12,	 "gpmc_ad12",       28};
const GPIOInterface::Pin GPIOInterface::P8_13 = { "EHRPWM2B",    GPIO0,     23, 	23,	 "gpmc_ad9",        15};
const GPIOInterface::Pin GPIOInterface::P8_14 = { "GPIO0_26",    GPIO0,     26, 	26,	 "gpmc_ad10",       16};
const GPIOInterface::Pin GPIOInterface::P8_15 = { "GPIO1_15",    GPIO1,     47, 	15,	 "gpmc_ad15",       31};
const GPIOInterface::Pin GPIOInterface::P8_16 = { "GPIO1_14",    GPIO1,     46, 	14,	 "gpmc_ad14",       30};
const GPIOInterface::Pin GPIOInterface::P8_17 = { "GPIO0_27",    GPIO0,     27, 	27,	 "gpmc_ad11",       17};
const GPIOInterface::Pin GPIOInterface::P8_18 = { "GPIO2_1",     GPIO2,     65, 	1,	 "gpmc_clk",        40};
const GPIOInterface::Pin GPIOInterface::P8_19 = { "EHRPWM2A",    GPIO0,     22, 	22,	 "gpmc_ad8",        14};
const GPIOInterface::Pin GPIOInterface::P8_20 = { "GPIO1_31",    GPIO1,     63, 	31,	 "gpmc_csn2",       39};
const GPIOInterface::Pin GPIOInterface::P8_21 = { "GPIO1_30",    GPIO1,     62, 	30,	 "gpmc_csn1",       38};
const GPIOInterface::Pin GPIOInterface::P8_22 = { "GPIO1_5",     GPIO1,     37, 	5,	 "gpmc_ad5",        25};
const GPIOInterface::Pin GPIOInterface::P8_23 = { "GPIO1_4",     GPIO1,     36, 	4,	 "gpmc_ad4",        24};
const GPIOInterface::Pin GPIOInterface::P8_24 = { "GPIO1_1",     GPIO1,     33, 	1,	 "gpmc_ad1",        21};
const GPIOInterface::Pin GPIOInterface::P8_25 = { "GPIO1_0",     GPIO1,     32, 	0,	 "gpmc_ad0",        20};
const GPIOInterface::Pin GPIOInterface::P8_26 = { "GPIO1_29",    GPIO1,     61, 	29,	 "gpmc_csn0",       37};
const GPIOInterface::Pin GPIOInterface::P8_27 = { "GPIO2_22",    GPIO2,     86, 	22,	 "lcd_vsync",       57};
const GPIOInterface::Pin GPIOInterface::P8_28 = { "GPIO2_24",    GPIO2,     88, 	24,	 "lcd_pclk",        59};
const GPIOInterface::Pin GPIOInterface::P8_29 = { "GPIO2_23",    GPIO2,     87, 	23,	 "lcd_hsync",       58};
const GPIOInterface::Pin GPIOInterface::P8_30 = { "GPIO2_25",    GPIO2,     89, 	25,	 "lcd_ac_bias_en",  60};
const GPIOInterface::Pin GPIOInterface::P8_31 = { "UART5_CTSN",  GPIO0,     10, 	10,	 "lcd_data14",       7};
const GPIOInterface::Pin GPIOInterface::P8_32 = { "UART5_RTSN",  GPIO0,     11, 	11,	 "lcd_data15",       8};
const GPIOInterface::Pin GPIOInterface::P8_33 = { "UART4_RTSN",  GPIO0,     9,  	9,	 "lcd_data13",       6};
const GPIOInterface::Pin GPIOInterface::P8_34 = { "UART3_RTSN",  GPIO2,     81, 	17,	 "lcd_data11",      56};
const GPIOInterface::Pin GPIOInterface::P8_35 = { "UART4_CTSN",  GPIO0,     8,  	8,	 "lcd_data12",       5};
const GPIOInterface::Pin GPIOInterface::P8_36 = { "UART3_CTSN",  GPIO2,     80, 	16,	 "lcd_data10",      55};
const GPIOInterface::Pin GPIOInterface::P8_37 = { "UART5_TXD",   GPIO2,     78, 	14,	 "lcd_data8",       53};
const GPIOInterface::Pin GPIOInterface::P8_38 = { "UART5_RXD",   GPIO2,     79, 	15,	 "lcd_data9",       54};
const GPIOInterface::Pin GPIOInterface::P8_39 = { "GPIO2_12",    GPIO2,     76, 	12,	 "lcd_data6",       51};
const GPIOInterface::Pin GPIOInterface::P8_40 = { "GPIO2_13",    GPIO2,     77, 	13,	 "lcd_data7",       52};
const GPIOInterface::Pin GPIOInterface::P8_41 = { "GPIO2_10",    GPIO2,     74, 	10,	 "lcd_data4",       49};
const GPIOInterface::Pin GPIOInterface::P8_42 = { "GPIO2_11",    GPIO2,     75, 	11,	 "lcd_data5",       50};
const GPIOInterface::Pin GPIOInterface::P8_43 = { "GPIO2_8",     GPIO2,     72, 	8,	 "lcd_data2",       47};
const GPIOInterface::Pin GPIOInterface::P8_44 = { "GPIO2_9",     GPIO2,     73, 	9,	 "lcd_data3",       48};
const GPIOInterface::Pin GPIOInterface::P8_45 = { "GPIO2_6",     GPIO2,     70, 	6,	 "lcd_data0",       45};
const GPIOInterface::Pin GPIOInterface::P8_46 = { "GPIO2_7",     GPIO2,     71, 	7,	 "lcd_data1",       46};

const GPIOInterface::Pin GPIOInterface::P9_11 = { "UART4_RXD",   GPIO0,     30,  30,  "gpmc_wait0",      18};
const GPIOInterface::Pin GPIOInterface::P9_12 = { "GPIO1_28",    GPIO1,     60,  28,  "gpmc_ben1",       36};
const GPIOInterface::Pin GPIOInterface::P9_13 = { "UART4_TXD",   GPIO0,     31,  31,  "gpmc_wpn",        19};
const GPIOInterface::Pin GPIOInterface::P9_14 = { "EHRPWM1A",    GPIO1,     50,  18,  "gpmc_a2",         34};
const GPIOInterface::Pin GPIOInterface::P9_15 = { "GPIO1_16",    GPIO1,     48,  16,  "mii1_rxd3",       32};
const GPIOInterface::Pin GPIOInterface::P9_16 = { "EHRPWM1B",    GPIO1,     51,  19,  "gpmc_a3",         35};
const GPIOInterface::Pin GPIOInterface::P9_17 = { "I2C1_SCL",    GPIO0,      5,   5,  "spi0_cs0",         3};
const GPIOInterface::Pin GPIOInterface::P9_18 = { "I2C1_SDA",    GPIO0,      4,   4,  "spi0_d1",          2};
const GPIOInterface::Pin GPIOInterface::P9_19 = { "I2C2_SCL",    GPIO0,     13,  13,  "uart1_rtsn",       9};
const GPIOInterface::Pin GPIOInterface::P9_20 = { "I2C2_SDA",    GPIO0,     12,  12,  "uart1_ctsn",      10};
const GPIOInterface::Pin GPIOInterface::P9_21 = { "UART2_TXD",   GPIO0,      3,   3,  "spi0_d0",          1};
const GPIOInterface::Pin GPIOInterface::P9_22 = { "UART2_RXD",   GPIO0,      2,   2,  "spi0_sclk",        0};
const GPIOInterface::Pin GPIOInterface::P9_23 = { "GPIO1_17",    GPIO1,     49,  17,  "gpmc_a1",         33};
const GPIOInterface::Pin GPIOInterface::P9_24 = { "UART1_TXD",   GPIO0,     15,  15,  "uart1_txd",       12};
const GPIOInterface::Pin GPIOInterface::P9_25 = { "GPIO3_21",    GPIO3,    117,  21,  "mcasp0_ahclkx",   66};
const GPIOInterface::Pin GPIOInterface::P9_26 = { "UART1_RXD",   GPIO0,     14,  14,  "uart1_rxd",       11};
const GPIOInterface::Pin GPIOInterface::P9_27 = { "GPIO3_19",    GPIO3,    115,  19,  "mcasp0_fsr",      64};
const GPIOInterface::Pin GPIOInterface::P9_28 = { "SPI1_CS0",    GPIO3,    113,  17,  "mcasp0_ahclkr",   63};
const GPIOInterface::Pin GPIOInterface::P9_29 = { "SPI1_D0",     GPIO3,    111,  15,  "mcasp0_fsx",      61};
const GPIOInterface::Pin GPIOInterface::P9_30 = { "SPI1_D1",     GPIO3,    112,  16,  "mcasp0_axr0",     62};
const GPIOInterface::Pin GPIOInterface::P9_31 = { "SPI1_SCLK",   GPIO3,    110,  14,  "mcasp0_aclkx",    65};
const GPIOInterface::Pin GPIOInterface::P9_33 = { "AIN4",        0,          4,   4,  "",                71};
const GPIOInterface::Pin GPIOInterface::P9_35 = { "AIN6",        0,          6,   6,  "",                73};
const GPIOInterface::Pin GPIOInterface::P9_36 = { "AIN5",        0,          5,   5,  "",                72};
const GPIOInterface::Pin GPIOInterface::P9_37 = { "AIN2",        0,          2,   2,  "",                69};
const GPIOInterface::Pin GPIOInterface::P9_38 = { "AIN3",        0,          3,   3,  "",                70};
const GPIOInterface::Pin GPIOInterface::P9_39 = { "AIN0",        0,          0,   0,  "",                67};
const GPIOInterface::Pin GPIOInterface::P9_40 = { "AIN1",        0,          1,   1,  "",                68};
const GPIOInterface::Pin GPIOInterface::P9_41 = { "CLKOUT2",     GPIO0,     20,  20,  "xdma_event_intr1",13};
const GPIOInterface::Pin GPIOInterface::P9_42 = { "GPIO0_7",     GPIO0,      7,   7,  "ecap0_in_pwm0_out",4};

GPIOInterface::GPIOInterface(DataStore& dataStore)
    : SimObject(dataStore, "GPIO Interface", 1)  // 1Hz
{
    int fd = open("/dev/mem", O_RDWR);
    if (fd == -1) {
        std::cerr << "[GPIO Interface] Unable to open /dev/mem" << std::endl;
        return;
    }

    // perform memory map
    m_map = (uint32_t*)mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
    if (m_map == MAP_FAILED) {
        close(fd);
        std::cerr << "[GPIO Interface] Unable to map /dev/mem" << std::endl;
        return;
    }

    if (pthread_mutex_init(&m_lock, NULL) == -1) {
        std::cerr << "[GPIO Interface] Unable to create mutex" << std::endl;
        return;
    }
}

void GPIOInterface::init()
{
    std::cout << "[" << getName() << "] init" << std::endl;
    // No variables to be initialized
}

void GPIOInterface::step(unsigned long)
{
    // Nothing to be done here for this sim device
}

void GPIOInterface::initPin(const Pin* pin, Direction direction)
{
    int pin_data = 0;
	FILE *fp = NULL;
	char muxfile[64];

    // set the mux mode
	// pin_data |=  mux;
    pin_data |= 7;  // mux=7 => GPIO

	// set up the pull up/down resistors
	// if(pull == gpio_disabled) pin_data |= 1 << 3;
	// if(pull == gpio_pullup)   pin_data |= 1 << 4;

    // set up the pin direction
	// pin_data |= direction << 5; 
    if (direction == GPIO_IN)
    {
        pin_data |= 1 << 5;
    }

	// write the pin_data
	sprintf(muxfile, "/sys/kernel/debug/omap_mux/%s", pin->mux);
	// open the file
	if((fp = fopen(muxfile, "w")) == NULL) 
    {
		perror("[GPIO Interface] Cannot set pin mode");
		return;
	}
	fprintf(fp, "%x", pin_data);
	fclose(fp);
}

void GPIOInterface::write(const Pin* pin, bool value)
{
    pthread_mutex_lock(&m_lock);
    m_map[(pin->gpio_bank-MMAP_OFFSET+GPIO_OE)/4] &= ~(1<<pin->bank_id);

	if(value == true) {
        m_map[(pin->gpio_bank-MMAP_OFFSET+GPIO_DATAOUT)/4] |= 1<<pin->bank_id;
    } else {
        m_map[(pin->gpio_bank-MMAP_OFFSET+GPIO_DATAOUT)/4] &= ~(1<<pin->bank_id);
    }

    pthread_mutex_unlock(&m_lock);
}

bool GPIOInterface::read(const Pin* pin)
{
    bool value;

    pthread_mutex_lock(&m_lock);
    value = (m_map[(pin->gpio_bank-MMAP_OFFSET+GPIO_DATAIN)/4] & (1<<pin->bank_id))>>pin->bank_id;
    pthread_mutex_unlock(&m_lock);

    return value;
}