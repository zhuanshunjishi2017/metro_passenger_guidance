#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
//#define RTC_CLOCK_SOURCE_LXTAL
#define BKP_VALUE    0x32F0
#define DEC2BCD(x) ((((x) / 10) << 4) | ((x) % 10))

#define RTC_CLOCK_SOURCE_IRC32K

rtc_parameter_struct  rtc_initpara;
rtc_alarm_struct  rtc_alarm;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;
uint32_t RTCSRC_FLAG = 0;

void rtc_setup(void);
void my_rtc_setup(void);
void rtc_pre_config(void);
void get_rtc_time_string(char *buf, size_t len);


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
        if(RESET != rcu_flag_get(RCU_FLAG_PORRST)); //power on reset occurred
        else if(RESET != rcu_flag_get(RCU_FLAG_EPRST)) ;//external reset occurred
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
    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = DEC2BCD(26);
    rtc_initpara.day_of_week = RTC_THURSDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = DEC2BCD(2);
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;
    rtc_initpara.hour = DEC2BCD(8);
    rtc_initpara.minute = DEC2BCD(0);
    rtc_initpara.second = DEC2BCD(0);

    /* current time input */
    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)) {
        return;
    } else {
        RTC_BKP0 = BKP_VALUE;
    }
}

