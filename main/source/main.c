#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#define RTC_CLOCK_SOURCE_LXTAL
#define BKP_VALUE    0x32F0
#define DEC2BCD(x) ((((x) / 10) << 4) | ((x) % 10))

//#define RTC_CLOCK_SOURCE_IRC32K


rtc_parameter_struct  rtc_initpara;
rtc_alarm_struct  rtc_alarm;

__IO uint32_t prescaler_a = 0, prescaler_s = 0;
uint32_t RTCSRC_FLAG = 0;

void rtc_setup(void);
//void rtc_show_time(void);
void rtc_pre_config(void);


void display(void);
void gpio_init(void);
void my_rtc_init(void);
void get_rtc_time_string(char *buf, size_t len);

lv_obj_t *label;

int main(void)
{
	sys_init();
	
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

	display();
	
	label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "");                 
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0); 

    rtc_setup();

    delay_us(4000000);

    char time_buf[32];
    while(1)
	{
        get_rtc_time_string(time_buf, sizeof(time_buf));
        //lv_label_set_text(label, time_buf); // 更新显示
        lv_timer_handler();
        delay_us(40000);//刷新率25Hz
	}
}

void gpio_init(void)
{
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_bit_reset(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

// RTC 初始化函数
void my_rtc_init(void) 
{
    rcu_periph_clock_enable(RCU_PMU); 
    pmu_backup_write_enable(); 
    
    rcu_osci_on(RCU_LXTAL); 
    rcu_osci_stab_wait(RCU_LXTAL); 
    
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL); 
    rcu_periph_clock_enable(RCU_RTC); 
    
    rtc_parameter_struct rtc_init_struct; 

    rtc_init_struct.hour = 13; 
    rtc_init_struct.minute = 0; 
    rtc_init_struct.second = 0; 
    rtc_init_struct.year = 26; // 2000 + year 
    rtc_init_struct.month = 4; 
    rtc_init_struct.date = 2; 
    rtc_init_struct.day_of_week = RTC_SUNDAY; 
    rtc_init_struct.display_format = RTC_12HOUR;
    
    rtc_init_struct.factor_syn = 255; 
    rtc_init_struct.factor_asyn = 127; 
    
    
    rtc_init(&rtc_init_struct);
}


void get_rtc_time_string(char *buf, size_t len) 
{
    rtc_parameter_struct rtc_time;

    rtc_current_time_get(&rtc_time);

    snprintf(buf, len, "20%0.2x-%0.2x-%0.2x %0.2x:%0.2x:%0.2x",
            rtc_time.year,
             rtc_time.month,
             rtc_time.date,
             rtc_time.hour,
             rtc_time.minute,
             rtc_time.second);
}




/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void my_rtc_setup(void)
{


    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();
    /* get RTC clock entry selection */
    RTCSRC_FLAG = GET_BITS(RCU_BDCTL, 8, 9);
    rtc_pre_config();

    /* check if RTC has already been configured */
    if((BKP_VALUE != RTC_BKP0) || (0x00 == RTCSRC_FLAG)) {
        /* backup data register value is not correct or not yet programmed
            or RTC clock source is not configured (when the first time the program
            is executed or data in RCU_BDCTL is lost due to Vbat feeding) */
        rtc_setup();
    } else {
        /* detect the reset source */
        if(RESET != rcu_flag_get(RCU_FLAG_PORRST)) {
            lv_label_set_text(label,"power on reset occurred....\n\r");
        } else if(RESET != rcu_flag_get(RCU_FLAG_EPRST)) {
            lv_label_set_text(label,"external reset occurred....\n\r");
        }
        lv_label_set_text(label,"no need to configure RTC....\n\r");
    }

    rcu_all_reset_flag_clear();
}

/*!
    \brief      RTC configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_pre_config(void)
{
#if defined (RTC_CLOCK_SOURCE_IRC32K)
    rcu_osci_on(RCU_IRC32K);
    rcu_osci_stab_wait(RCU_IRC32K);
    rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);

    prescaler_s = 0x13F;
    prescaler_a = 0x63;
#elif defined (RTC_CLOCK_SOURCE_LXTAL)
    rcu_osci_on(RCU_LXTAL);
    rcu_osci_stab_wait(RCU_LXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    prescaler_s = 0xFF;
    prescaler_a = 0x7F;
#else
#error RTC clock source should be defined.
#endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

/*!
    \brief      use hyperterminal to setup RTC time and alarm
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_setup(void)
{
    /* setup RTC time value */
    //uint32_t tmp_year = 0xFF, tmp_month = 0xFF, tmp_day = 0xFF;
    //uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = DEC2BCD(26);
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = DEC2BCD(30);
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;
    rtc_initpara.hour = 0;
    rtc_initpara.minute = 0;
    rtc_initpara.second = 0;

    /* current time input */
    lv_label_set_text(label,"=======Configure RTC Time========\n\r");


    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)) {
        lv_label_set_text(label,"\n\r** RTC time configuration failed! **\n\r");
    } else {
        lv_label_set_text(label,"\n\r** RTC time configuration success! **\n\r");
        RTC_BKP0 = BKP_VALUE;
    }
}

/*!
    \brief      display the current time
    \param[in]  none
    \param[out] none
    \retval     none
*/
/*
void rtc_show_time(void)
{
    rtc_current_time_get(&rtc_initpara);

    printf("\r\nCurrent time: 20%0.2x-%0.2x-%0.2x", \
           rtc_initpara.year, rtc_initpara.month, rtc_initpara.date);

    printf(" : %0.2x:%0.2x:%0.2x \r\n", \
           rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
}
           */

/*!
    \brief      get the input character string and check if it is valid
    \param[in]  none
    \param[out] none
    \retval     input value in BCD mode
*/
/*
uint8_t usart_input_threshold(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while(index < 2) {
        while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_RBNE));
        tmp[index++] = usart_data_receive(EVAL_COM);
        if((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)) {
            printf("\n\r please input a valid number between 0 and 9 \n\r");
            index--;
        }
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    if(index > value) {
        printf("\n\r please input a valid number between 0 and %d \n\r", value);
        return 0xFF;
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) << 4);
    return index;
}
*/
/* retarget the C library printf function to the USART */
/*
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TC)) {
    }
    return ch;
}
*/