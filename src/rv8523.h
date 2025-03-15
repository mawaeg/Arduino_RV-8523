/*!
 * @file rv8523.h
 *
 * A library to control the RV-8523 RTC.
 * 
 * RV-8523 is a CMOS RTC / calender optimized for low power consumption controlled via an I2C bus.
 *
 * This library was heavily inspired by the RV-3028_C7-Arduino_Library library by constiko (https://github.com/constiko/RV-3028_C7-Arduino_Library).
 * 
 * For more details on the RV-8523 check out it's official site: https://www.microcrystal.com/en/products/real-time-clock-rtc-modules/rv-8523-c3.
 * 
 * Library created by Marek Schmitt and licensed under MIT.
 */

#ifndef RV8523_H
#define RV8523_H

#include "Arduino.h"
#include <Wire.h>

#include <array>

#define RV8523_ADDR                        (uint8_t)0x68

#define RV8523_CONTROL1                    0x00
#define RV8523_CONTROL2                    0x01
#define RV8523_CONTROL3                    0x02
#define RV8523_SECONDS                     0x03
#define RV8523_MINUTES                     0x04
#define RV8523_HOURS                       0x05
#define RV8523_DAYS                        0x06
#define RV8523_WEEKDAYS                    0x07
#define RV8523_MONTHS                      0x08
#define RV8523_YEARS                       0x09
#define RV8523_MINUTE_ALARM                0x0A
#define RV8523_HOUR_ALARM                  0x0B
#define RV8523_DAY_ALARM                   0x0C
#define RV8523_WEEKDAY_ALARM               0x0D
#define RV8523_FREQUENCY_OFFSET            0x0E
#define RV8523_TIMER_CLKOUT                0x0F
#define RV8523_TIMER_A_CLOCK               0x10
#define RV8523_TIMER_A                     0x11
#define RV8523_TIMER_B_CLOCK               0x12
#define RV8523_TIMER_B                     0x13

#define RV8523_CONTROL1_CAP                0b1 << 7
#define RV8523_CONTROL1_N                  0b1 << 6
#define RV8523_CONTROL1_STOP               0b1 << 5
#define RV8523_CONTROL1_SR                 0b1 << 4
#define RV8523_CONTROL1_12_24              0b1 << 3
#define RV8523_CONTROL1_SIE                0b1 << 2
#define RV8523_CONTROL1_AIE                0b1 << 1
#define RV8523_CONTROL1_CIE                0b1
#define RV8523_CONTROL1_RESET              0b01011 << 3

#define RV8523_CONTROL2_WTAF               0b1 << 7
#define RV8523_CONTROL2_CTAF               0b1 << 6
#define RV8523_CONTROL2_CTBF               0b1 << 5
#define RV8523_CONTROL2_SF                 0b1 << 4
#define RV8523_CONTROL2_AF                 0b1 << 3
#define RV8523_CONTROL2_WTAIE              0b1 << 2
#define RV8523_CONTROL2_CTAIE              0b1 << 1
#define RV8523_CONTROL2_CTBIE              0b1

#define RV8523_CONTROL3_PM                 0b111 << 5
#define RV8523_CONTROL3_X                  0b1 << 4
#define RV8523_CONTROL3_BSF                0b1 << 3
#define RV8523_CONTROL3_BLF                0b1 << 2
#define RV8523_CONTROL3_BSIE               0b1 << 1
#define RV8523_CONTROL3_BLIE               0b1

#define RV8523_SECONDS_OS                  0b1 << 7
#define RV8523_SECONDS_SECONDS             0b1111111

#define RV8523_MINUTES_X                   0b1 << 7
#define RV8523_MINUTES_MINUTES             0b1111111

#define RV8523_HOURS_X                     0b11 << 6
#define RV8523_HOURS_12_AMPM               0b1 << 5
#define RV8523_HOURS_12_HOURS              0b11111
#define RV8523_HOURS_24_HOURS              0b111111

#define RV8523_DAYS_X                      0b11 << 6
#define RV8523_DAYS_DAYS                   0b111111

#define RV8523_WEEKDAYS_X                  0b11111 << 3
#define RV8523_WEEKDAYS_WEEKDAYS           0b111

#define RV8523_MONTHS_X                    0b111 << 5
#define RV8523_MONTHS_MONTHS               0b11111

#define RV8523_YEARS_YEARS                 0b11111111

#define RV8523_MINUTE_ALARM_AE_M           0b1 << 7
#define RV8523_MINUTE_ALARM_MINUTE_ALARM   0b1111111

#define RV8523_HOUR_ALARM_AE_H             0b1 << 7
#define RV8523_HOUR_ALARM_X                0b1 << 6
#define RV8523_HOUR_ALARM_12_AMPM          0b1 << 5
#define RV8523_HOUR_ALARM_12_HOUR_ALARM    0b11111
#define RV8523_HOUR_ALARM_24_HOUR_ALARM    0b111111

#define RV8523_DAY_ALARM_AE_D              0b1 << 7
#define RV8523_DAY_ALARM_X                 0b1 << 6
#define RV8523_DAY_ALARM_DAY_ALARM         0b111111

#define RV8523_WEEKDAY_ALARM_AE_W          0b1 << 7
#define RV8523_WEEKDAY_ALARM_X             0b1111 << 3
#define RV8523_WEEKDAY_ALARM_WEEKDAY_ALARM 0b111

#define RV8523_FREQUENCY_OFFSET_MODE       0b1 << 7
#define RV8523_FREQUENCY_OFFSET_OFFSET     0b1111111

#define RV8523_TIMER_CLKOUT_TAM            0b1 << 7
#define RV8523_TIMER_CLKOUT_TBM            0b1 << 6
#define RV8523_TIMER_CLKOUT_COF            0b111 << 3
#define RV8523_TIMER_CLKOUT_TAC            0b11 << 1
#define RV8523_TIMER_CLKOUT_TBC            0b1

#define RV8523_TIMER_A_CLOCK_X             0b11111 << 3
#define RV8523_TIMER_A_CLOCK_TAQ           0b111

#define RV8523_TIMER_A_TIMER_A             0b11111111

#define RV8523_TIMER_B_CLOCK_X1            0b1 << 7
#define RV8523_TIMER_B_CLOCK_TBW           0b111 << 4
#define RV8523_TIMER_B_CLOCK_X2            0b1 << 3
#define RV8523_TIMER_B_CLOCK_TBQ           0b111

#define RV8523_TIMER_B_TIMER_B             0b11111111

#define TIME_ARRAY_LENGTH                  7
#define POWER_MANAGEMENT_LENGTH            6

/*! Defines the order of values in the internal time array. */
enum time_order {
    TIME_SECONDS,
    TIME_MINUTES,
    TIME_HOURS,
    TIME_DAY,
    TIME_WEEKDAY,
    TIME_MONTH,
    TIME_YEAR,
};

// clang-format off
/*! Defines the different power management types that can be configured */
enum RV8523_BATTERY_SWITCHOVER {
    RV8523_BATTERY_SWITCHOVER_EN_STD__BAT_LOW_EN,   /*!< Battery switchover function is enabled in standard mode; Battery low detection function is enabled */
    RV8523_BATTERY_SWITCHOVER_EN_DRCT__BAT_LOW_EN,  /*!< Battery switchover function is enabled in direct switching mode; Battery low detection function is enabled*/
    RV8523_BATTERY_SWITCHOVER_DIS__BAT_LOW_EN,      /*!< Battery switchover function is disabled - only one power supply (VDD); Battery low detection function is enabled */
    RV8523_BATTERY_SWITCHOVER_EN_STD__BAT_LOW_DIS,  /*!< Battery switchover function is enabled in standard mode; Battery low detection function is disabled */
    RV8523_BATTERY_SWITCHOVER_EN_DRCT__BAT_LOW_DIS, /*!< Battery switchover function is enabled in direct switching mode; Battery low detection function is disabled */
    RV8523_BATTERY_SWITCHOVER_DIS__BAT_LOW_DIS,     /*!< Battery switchover function is disabled - only one power supply (VDD); Battery low detection function is disabled*/
};
// clang-format on

const std::array<uint8_t, 6> RV8523_BATTERY_SWITCHOVER_VALUES{
    0b0,
    0b001 << 5,
    0b010 << 5, /* Same as 011 according to datasheet*/
    0b100 << 5,
    0b101 << 5,
    /* Not allowed: 0b110 << 5, */
    0b111 << 5,
};

/*! Defines the source clock frequencies that can be configured */
enum RV8523_SOURCE_CLOCK_FREQUENCY {
    RV8523_SOURCE_CLOCK_FREQUENCY_4096KHZ,    /*!< 4.096 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_64HZ,       /*!< 64 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_1HZ,        /*!< 1 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_1_60HZ,     /*!< 1/60 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_1_3600HZ_1, /*!< 1/3600 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_1_3600HZ_2, /*!< 1/3600 Hz */
    RV8523_SOURCE_CLOCK_FREQUENCY_1_3600HZ_3  /*!< 1/3600 Hz */
};

/*! Defines the timer puls widths that can be configured*/
enum RV8523_TIMER_PULS_WIDTH {
    RV8523_TIMER_PULS_WIDTH_46_875_MS,  /*!< 46.875 ms */
    RV8523_TIMER_PULS_WIDTH_62_500_MS,  /*!< 62.500 ms */
    RV8523_TIMER_PULS_WIDTH_78_125_MS,  /*!< 78.125 ms */
    RV8523_TIMER_PULS_WIDTH_93_750_MS,  /*!< 93.750 ms */
    RV8523_TIMER_PULS_WIDTH_125_000_MS, /*!< 125.000 ms */
    RV8523_TIMER_PULS_WIDTH_156_250_MS, /*!< 156.250 ms */
    RV8523_TIMER_PULS_WIDTH_187_500_MS, /*!< 187.500 ms */
    RV8523_TIMER_PULS_WIDTH_218_750_MS  /*!< 218.750 ms */
};

/*! Defines the timer interrupt modes that can be configured */
enum RV8523_TIMER_IRQ_MODE {
    RV8523_TIMER_IRQ_MODE_PERMANENT, /*!< Permanent interrupt mode */
    RV8523_TIMER_IRQ_MODE_PULSED     /*!< Pulsed interrupt mode */
};

/*! Defines the modes the Timer A can be configured to */
enum RV8523_TIMER_A_MODE {
    RV8523_TIMER_A_MODE_DISABLED,  /*!< Timer A disabled */
    RV8523_TIMER_A_MODE_COUNTDOWN, /*!< Timer A configured to countdown mode */
    RV8523_TIMER_A_MODE_WATCHDOG   /*!< Timer A configured to watchdog mode */
};

/*! Defines the clkout frequencies that can be configured */
enum RV8523_CLKOUT_FREQUENCY {
    RV8523_CLKOUT_FREQUENCY_32_768,  /*!< 32'768 Hz */
    RV8523_CLKOUT_FREQUENCY_17_384,  /*!< 16'384 Hz */
    RV8523_CLKOUT_FREQUENCY_8_192,   /*!< 8'192 Hz */
    RV8523_CLKOUT_FREQUENCY_4_096,   /*!< 4'096 Hz */
    RV8523_CLKOUT_FREQUENCY_1_024,   /*!< 1'024 Hz */
    RV8523_CLKOUT_FREQUENCY_32,      /*!< 32 Hz */
    RV8523_CLKOUT_FREQUENCY_1,       /*!< 1 Hz */
    RV8523_CLKOUT_FREQUENCY_DISABLED /*!< clkout disabled */
};

// clang-format off
/*! Defines the alarm modes that can be configured*/
enum RV8523_ALARM_MODE {
    RV8523_ALARM_MODE_DISABLED,         /*!< Disables the alarm */
    RV8523_ALARM_MODE_MIN,              /*!< Enables minute alarm */
    RV8523_ALARM_MODE_HOUR,             /*!< Enables hour alarm */
    RV8523_ALARM_MODE_MIN_HOUR,         /*!< Enables minute and hour alarm */
    RV8523_ALARM_MODE_DAY,              /*!< Enables day alarm */
    RV8523_ALARM_MODE_MIN_DAY,          /*!< Enables minute and day alarm */
    RV8523_ALARM_MODE_HOUR_DAY,         /*!< Enable hour and day alarm */
    RV8523_ALARM_MODE_MIN_HOUR_DAY,     /*!< Enable minute, hour and day alarm */
    RV8523_ALARM_MODE_WDAY,             /*!< Enable weekday alarm */
    RV8523_ALARM_MODE_MIN_WDAY,         /*!< Enable minute and weekday alarm */
    RV8523_ALARM_MODE_HOUR_WDAY,        /*!< Enable hour and weekday alarm */
    RV8523_ALARM_MODE_MIN_HOUR_WDAY,    /*!< Enable minute, hour and weekday alarm */
    RV8523_ALARM_MODE_DAY_WDAY,         /*!< Enable day and weekday alarm */
    RV8523_ALARM_MODE_MIN_DAY_WDAY,     /*!< Enable minute, day and weekday alarm */
    RV8523_ALARM_MODE_HOUR_DAY_WDAY,    /*!< Enable hour, day and weekday alarm */
    RV8523_ALARM_MODE_MIN_HOUR_DAY_WDAY /*!< Enable minute, hour, day and weekday alarm */
};
// clang-format on

enum RV8523_FREQUENCY_OFFSET_MODE_TYPE {
    RV8523_FREQUENCY_OFFSET_MODE_TYPE_2HOURS,
    RV8523_FREQUENCY_OFFSET_MODE_TYPE_1MIN
};

/* TBD:
    ALARM 12 hours mode
*/

/*!
 * @brief Main class to interact with the RV-8523
 */
class RV8523 {
  public:
    RV8523();

    /*!
     * @brief Prepares the library and setup rtc
     * @param twoWire Reference to the TwoWire instance that should be used
     * @param is24Hours Whether the rtc should operate in 24hour or 12hour mode
     * @return true if the operation was successful, false otherwise
     */
    bool begin(TwoWire &twoWire = Wire, bool is24Hours = true);
    /*!
     * @brief Sets the time of the rtc
     * @param seconds The seconds that should be set (0-59)
     * @param minutes The minutes that should be set (0-59)
     * @param hours The hours that should be set (1-12 / 0-23)
     * @param day The day that should be set (1-31)
     * @param weekday The weekday that should be set (0-6) (0=Sunday)
     * @param month the month that should be set (1-12)
     * @param year The year that should be set (0-99)
     * @return true if the operation was successful, false otherwise
     */
    bool setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day,
                 uint8_t weekday, uint8_t month, uint8_t year);
    /*!
     * @brief Sets the time of the rtc (!NOTE! values have to be in bcd format)
     * @param time pointer to an array of uint8_t to set the time to (seconds, minutes, hours, day , weekday, month, year) (in bcd)
     * @return true if the operation was successful, false otherwise
     */
    bool setTime(uint8_t *time, uint8_t len);
    /*!
     * @brief Sets the seconds of the rtc
     * @param seconds The seconds that should be set (0-59)
     * @return true if the operation was successful, false otherwise
     */
    bool setSeconds(uint8_t seconds);
    /*!
     * @brief Sets the minutes of the rtc
     * @param minutes The minutes that should be set (0-59)
     * @return true if the operation was successful, false otherwise
     */
    bool setMinutes(uint8_t minutes);
    /*!
     * @brief Sets the hours of the rtc
     * @param hours The hours that should be set (1-12 / 0-23)
     * @return true if the operation was successful, false otherwise
     */
    bool setHours(uint8_t hours);
    /*!
     * @brief Sets the day of the rtc
     * @param day The day that should be set (1-31)
     * @return true if the operation was successful, false otherwise
     */
    bool setDay(uint8_t day);
    /*!
     * @brief Sets the weekday of the rtc
     * @param weekday The weekday that should be set (0-6) (0=Sunday)
     * @return true if the operation was successful, false otherwise
     */
    bool setWeekday(uint8_t weekday);
    /*!
     * @brief Sets the month of the rtc
     * @param month The month that should be set (1-12)
     * @return true if the operation was successful, false otherwise
     */
    bool setMonth(uint8_t month);
    /*!
     * @brief Sets the year of the rtc
     * @param year The year that should be set (0-99)
     * @return true if the operation was successful, false otherwise
     */
    bool setYear(uint8_t year);

    /*!
     * @brief Updates the local array. Needed before getting the time (string, or single number)
     * @return true if the operation was successful, false otherwise
     */
    bool updateTime();

    /*!
     * @brief Returns the current time in mm-dd-yyyy
     * @return The current time as formatted string
     */
    char *stringDateUSA();
    /*!
     * @brief Returns the current time in dd-mm-yyyy
     * @return The current time as formatted string
     */
    char *stringDate();
    /*!
     * @brief Returns the current time in hh:mm:ss with AM/PM if in 12 hour mode
     * @return The current time as formatted string
     */
    char *stringTime();
    /*!
     * @brief Returns the current timestamp in ISO 8601 (yyyy-mm-ddThh:mm:ss)
     * @return The current time as formatted string
     */
    char *stringTimeStamp();

    /*!
     * @brief Returns the current seconds
     * @return The seconds as uint8_8
     */
    uint8_t getSeconds();
    /*!
     * @brief Returns the current minutes
     * @return The minutes as uint8_8
     */
    uint8_t getMinutes();
    /*!
     * @brief Returns the current hours
     * @return The hours as uint8_8
     */
    uint8_t getHours();
    /*!
     * @brief Returns the current day
     * @return The day as uint8_8
     */
    uint8_t getDay();
    /*!
     * @brief Returns the current weekday
     * @return The weekday as uint8_8
     */
    uint8_t getWeekday();
    /*!
     * @brief Returns the current month
     * @return The month as uint8_8
     */
    uint8_t getMonth();
    /*!
     * @brief Returns the current year
     * @return The year as uint8_8
     */
    uint8_t getYear();

    /*!
     * @brief Checks if 12 hours mode is currently enabled
     * @return true if rtc is configured in 12 hours mode, false otherwise
     */
    bool is12Hours();
    /*!
     * @brief Checks if it is currently pm
     * @return true if pm, false otherwise and if 24 hours mode configured
     */
    bool isPM();
    /*!
     * @brief Sets the rtc to 12 hours mode
     * @return true if the operation was successful, false otherwise
     */
    bool set12Hours();
    /*!
     * @brief Sets the rtc to 24 hours mode
     * @return true if the operation was successful, false otherwise
     */
    bool set24Hours();

    /*!
     * @brief Configures the alarm of the rtc and enables the interrupt
     * @param minute The minute the alarm should be triggered
     * @param hour The hour the alarm should be triggered
     * @param day The day the alarm should be triggered
     * @param weekday The weekday the alarm should be triggered
     * @param mode The mode the alarm should be configured to
     * @return true if the operation was successful, false otherwise
     */
    bool setAlarm(uint8_t minute, uint8_t hour, uint8_t day, uint8_t weekday,
                  RV8523_ALARM_MODE mode);
    /*!
     * @brief Enables the interrupt for the alarm
     * @return true if the operation was successful, false otherwise
     */
    bool enableAlarmInterrupt();
    /*!
     * @brief Disables the interrupt for the alarm
     * @return true if the operation was successful, false otherwise
     */
    bool disableAlarmInterrupt();
    /*!
     * @brief Reads the interrupt flag for the alarm
     * @return true if the alarm interrupt flag is set, false otherwise
     */
    bool readAlarmInterruptFlag();
    /*!
     * @brief Clears the interrupt flag for the alarm
     * @return true if the operation was successful, false otherwise
     */
    bool clearAlarmInterruptFlag();

    /*!
     * @brief Configures the Timer A of the rtc
     * @param timer_irq_mode The interrupt mode the timer should be set to
     * @param source_freq The source frequency the timer should use
     * @param value The value the timer should be set to
     * @param set_interrupt Whether the interrupt of the timer should be enabled
     * @param mode The mode the timer should be set to
     * @return true if the operation was successful, false otherwise
     */
    bool setTimerA(RV8523_TIMER_IRQ_MODE timer_irq_mode,
                   RV8523_SOURCE_CLOCK_FREQUENCY source_freq, uint8_t value,
                   bool set_interrupt, RV8523_TIMER_A_MODE mode);
    /*!
     * @brief Configure the interrupt mode of Timer A
     * @param timer_irq_mode The interrupt mode the timer should be set to
     * @return true if the operation was successful, false otherwise
     */
    bool setTimerAIRQMode(RV8523_TIMER_IRQ_MODE timer_irq_mode);
    /*!
     * @brief Configure the operation mode of Timer A
     * @param mode The mode the timer should be set to
     * @return true if the operation was successful, false otherwise
     */
    bool setTimerAMode(RV8523_TIMER_A_MODE mode);
    /*!
     * @brief Disable the interrupt for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool disableTimerAInterrupts();
    /*!
     * @brief Enable the watchdog interrupt for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool enableTimerAWDInterrupt();
    /*!
     * @brief Disable the watchdog interrupt for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool disableTimerAWDInterrupt();
    /*!
     * @brief Enable the countdown interrupt for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool enableTimerACDInterrupt();
    /*!
     * @brief Disable the countdown interrupt for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool disableTimerACDInterrupt();
    /*!
     * @brief Reads the watchdog interrupt flag for Timer A
     * @return true if the Timer A watchdog interrupt flag is set, false otherwise
     */
    bool readTimerAWDInterruptFlag();
    /*!
     * @brief Reads the countdown interrupt flag for Timer A
     * @return true if the Timer A countdown interrupt flag is set, false otherwise
     */
    bool readTimerACDInterruptFlag();
    /*!
     * @brief Clears the interrupt flags for Timer A
     * @return true if the operation was successful, false otherwise
     */
    bool clearTimerAInterruptFlag();

    /*!
     * @brief Configures the Timer B of the rtc
     * @param timer_irq_mode The interrupt mode the timer should be set to
     * @param source_freq The source frequency the timer should use
     * @param pulse_width The pulse width that should be configured
     * @param value The value the timer should be set to
     * @param set_interrupt Whether the interrupt of the timer should be enabled
     * @param start_timer Whether the timer should be started (enabled)
     * @return true if the operation was successful, false otherwise
     */
    bool setTimerB(RV8523_TIMER_IRQ_MODE timer_irq_mode,
                   RV8523_SOURCE_CLOCK_FREQUENCY source_freq,
                   RV8523_TIMER_PULS_WIDTH pulse_width, uint8_t value,
                   bool set_interrupt, bool start_timer);
    /*!
     * @brief Configure the interrupt mode of Timer B
     * @param timer_irq_mode The interrupt mode the timer should be set to
     * @return true if the operation was successful, false otherwise
     */
    bool setTimerBIRQMode(RV8523_TIMER_IRQ_MODE timer_irq_mode);
    /*!
     * @brief Enables Timer B
     * @returns true if the operation was successful, false otherwise
     */
    bool enableTimerB();
    /*!
     * @brief Disables Timer B
     * @returns true if the operation was successful, false otherwise
     */
    bool disableTimerB();
    /*!
     * @brief Enable the interrupt for Timer B
     * @return true if the operation was successful, false otherwise
     */
    bool enableTimerBInterrupt();
    /*!
     * @brief Disable the interrupt for Timer B
     * @return true if the operation was successful, false otherwise
     */
    bool disableTimerBInterrupt();
    /*!
     * @brief Reads the interrupt flag for Timer B
     * @return true if the Timer B interrupt flag is set, false otherwise
     */
    bool readTimerBInterruptFlag();
    /*!
     * @brief Clears the interrupt flag for Timer B
     * @return true if the operation was successful, false otherwise
     */
    bool clearTimerBInterruptFlag();

    /*!
     * @brief Enables the second timer interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool enableSecondTimerInterrupt();
    /*!
     * @brief Disables the second timer interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool disableSecondTimerInterrupt();
    /*!
     * @brief Reads the second timer interrupt flag
     * @return true if the second timer interrupt flag is set, false otherwise
     */
    bool readSecondTimerInterruptFlag();
    /*!
     * @brief Clears the second timer interrupt flag
     * @return true if the operation was successful, false otherwise
     */
    bool clearSecondTimerInterruptFlag();

    /*!
     * @brief Get the current CLKOUT frequency
     * @return The currently configured CLKOUT frequency
     */
    RV8523_CLKOUT_FREQUENCY getCLKOUTFrequency();
    /*!
     * @brief Sets the CLKOUT frequency
     * @param frequency The frequency that should be set
     * @return true if the operation was successful, false otherwise
     */
    bool setCLKOUTFrequency(RV8523_CLKOUT_FREQUENCY frequency);

    /*!
     * @brief Get the current battery switchover configuration
     * @return The currently configured battery switchover
     */
    RV8523_BATTERY_SWITCHOVER getBatterySwitchover();
    /*!
     * @brief Sets the battery switchover configuration
     * @param frequency The battery switchover type that should be set
     * @return true if the operation was successful, false otherwise
     */
    bool setBatterySwitchover(RV8523_BATTERY_SWITCHOVER type);

    /*!
     * @brief Reads the battery switchover interrupt flag
     * @return true if the battery switchover interrupt flag is set, false otherwise
     */
    bool readBatterySwitchoverFlag();
    /*!
     * @brief Clears the battery switchover interrupt flag
     * @return true if the operation was successful, false otherwise
     */
    bool clearBatterySwitchoverFlag();
    /*!
     * @brief Enables the battery switchover interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool enableBatterySwitchoverInterrupt();
    /*!
     * @brief Disables the battery switchover interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool disableBatterySwitchoverInterrupt();
    /*!
     * @brief Reads the battery low interrupt flag
     * @return true if the battery low interrupt flag is set, false otherwise
     */
    bool readBatteryLowFlag();
    /*!
     * @brief Enables the battery low interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool enableBatteryLowInterrupt();
    /*!
     * @brief Disables the battery low interrupt
     * @return true if the operation was successful, false otherwise
     */
    bool disableBatteryLowInterrupt();

    /*!
     * @brief Sets the mode for the frequency offset
     * @param mode The mode that should be set where 
     * @return true if the operation was successful, false otherwise
     */
    bool setFrequencyOffsetMode(RV8523_FREQUENCY_OFFSET_MODE_TYPE mode);

    /*!
     * @brief Gets the currently configured frequency offset mode
     * @return The currently configured frequency offset mode
     */
    RV8523_FREQUENCY_OFFSET_MODE_TYPE getFrequencyOffsetMode();

    /*!
     * @brief Sets the frequency offset
     * @param offset The offset that should be configured (-64 to +63)
     * @return true if the operation was successful, false otherwise
     */
    bool setFrequencyOffset(int8_t offset);

    /*!
     * @brief Gets the currently configured frequency offset
     * @return The currently configured frequency offset
     */
    int8_t getFrequencyOffset();

    /*!
     * @brief Resets the rtc
     * @return true if the operation was successful, false otherwise
     */
    bool reset();

  private:
    /*!
     * @brief Reads a i2c address
     * @param addr The address it should be read from
     * @return The value of the registers as uint8_t
     */
    uint8_t readRegister(uint8_t addr);
    /*!
     * @brief Writes to a i2c address
     * @param addr The address it should be written to
     * @return true if the operation was successful, false otherwise
     */
    bool writeRegister(uint8_t addr, uint8_t val);
    /*!
     * @brief Reads from multiple i2c addresses
     * @param addr The start address it should be read from
     * @param dest The destination array, where the register values should be written to
     * @param len The amount of registers that should be read
     * @return true if the operation was successful, false otherwise
     */
    bool readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len);
    /*!
     * @brief Writes to multiple i2c addresses
     * @param addr The start address it should be written to
     * @param val The source array, that should be written to the registers
     * @param len The amount of registers that should be written
     * @return true if the operation was successful, false otherwise
     */
    bool writeMultipleRegisters(uint8_t addr, uint8_t *val, uint8_t len);

    /*!
     * @brief Applies a mask to a given i2c address
     * @param addr The addr the mask should be applied to
     * @param mask The mask that should be applied
     */
    bool setMask(uint8_t addr, uint8_t mask);
    /*!
     * @brief Clears a mask on a given i2c address
     * @param addr The addr the mask should be cleared from
     * @param mask The mask that should be cleared
     */
    bool clearMask(uint8_t addr, uint8_t mask);
    /*!
     * @brief Reads a mask from a given i2c address
     * @param addr The addr the mask should be read to
     * @param mask The mask that should be read
     */
    int readMask(uint8_t addr, uint8_t mask);

    /*!
     * @brief Converts a bcd value to dec
     * @param bcd The bcd value that should be converted
     * @return the converted value as dec
     */
    uint8_t bcdToDec(uint8_t bcd);
    /*!
     * @brief Converts a dec value to bcd
     * @param dec The dec value that should be converted
     * @return the converted value as bcd
     */
    uint8_t decToBcd(uint8_t dec);

    /*!
     * @brief Converts a binary value to a signed 8bit value where the 7th bit is the sign
     * @param bin The binary value that should be converted
     * @return The converted value
     */
    int8_t sevenBitSignedToDec(int8_t bin);

    TwoWire *_twoWire;
    uint8_t _time[TIME_ARRAY_LENGTH];
};

#endif
