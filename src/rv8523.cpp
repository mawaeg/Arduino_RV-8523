#include "rv8523.h"

RV8523::RV8523() {}

bool RV8523::begin(TwoWire &twoWire, bool is24Hours) {
    bool retVal = true;
    _twoWire = &twoWire;

    delay(1);

    if (!is24Hours) {
        retVal = set12Hours();
    }
    return retVal;
}

bool RV8523::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours,
                     uint8_t day, uint8_t weekday, uint8_t month,
                     uint8_t year) {
    /* TBD Check that the passed values are valid*/
    _time[TIME_SECONDS] = decToBcd(seconds);
    _time[TIME_MINUTES] = decToBcd(minutes);
    if (is12Hours()) {
        _time[TIME_HOURS] = decToBcd(hours);
    } else {
        _time[TIME_HOURS] = decToBcd(hours);
    }
    _time[TIME_DAY] = decToBcd(day);
    _time[TIME_WEEKDAY] = decToBcd(weekday);
    _time[TIME_MONTH] = decToBcd(month);
    _time[TIME_YEAR] = decToBcd(year);

    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setTime(uint8_t *time, uint8_t len) {
    if (len != TIME_ARRAY_LENGTH) {
        return false;
    }
    return writeMultipleRegisters(RV8523_SECONDS, time, len);
}

bool RV8523::setSeconds(uint8_t seconds) {
    _time[TIME_SECONDS] = decToBcd(seconds);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setMinutes(uint8_t minutes) {
    _time[TIME_MINUTES] = decToBcd(minutes);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setHours(uint8_t hours) {
    if (is12Hours()) {
        _time[TIME_HOURS] = decToBcd(hours);
    } else {
        _time[TIME_HOURS] = decToBcd(hours);
    }
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setDay(uint8_t day) {
    _time[TIME_DAY] = decToBcd(day);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setWeekday(uint8_t weekday) {
    _time[TIME_WEEKDAY] = decToBcd(weekday);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setMonth(uint8_t month) {
    _time[TIME_MONTH] = decToBcd(month);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::setYear(uint8_t year) {
    _time[TIME_YEAR] = decToBcd(year);
    return setTime(_time, TIME_ARRAY_LENGTH);
}

bool RV8523::updateTime() {
    return readMultipleRegisters(RV8523_SECONDS, _time, TIME_ARRAY_LENGTH);
}

char *RV8523::stringDateUSA() {
    static char date[11]; //Max of mm/dd/yyyy with \0 terminator
    sprintf(date, "%02hhu/%02hhu/20%02hhu", bcdToDec(_time[TIME_MONTH]),
            bcdToDec(_time[TIME_DAY]), bcdToDec(_time[TIME_YEAR]));
    return (date);
}

char *RV8523::stringDate() {
    static char date[11]; //Max of dd/mm/yyyy with \0 terminator
    sprintf(date, "%02hhu/%02hhu/20%02hhu", bcdToDec(_time[TIME_DAY]),
            bcdToDec(_time[TIME_MONTH]), bcdToDec(_time[TIME_YEAR]));
    return (date);
}

char *RV8523::stringTime() {
    static char time[11]; //Max of hh:mm:ssXM with \0 terminator

    if (is12Hours()) {
        char half = 'A';
        if (isPM()) {
            half = 'P';
        }

        sprintf(time, "%02hhu:%02hhu:%02hhu%cM", bcdToDec(_time[TIME_HOURS]),
                bcdToDec(_time[TIME_MINUTES]), bcdToDec(_time[TIME_SECONDS]),
                half);
    } else
        sprintf(time, "%02hhu:%02hhu:%02hhu", bcdToDec(_time[TIME_HOURS]),
                bcdToDec(_time[TIME_MINUTES]), bcdToDec(_time[TIME_SECONDS]));

    return (time);
}

char *RV8523::stringTimeStamp() {
    static char
        timeStamp[25]; //Max of yyyy-mm-ddThh:mm:ss.ss with \0 terminator

    if (is12Hours()) {
        char half = 'A';
        if (isPM()) {
            half = 'P';
        }

        sprintf(timeStamp, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu%cM",
                bcdToDec(_time[TIME_YEAR]), bcdToDec(_time[TIME_MONTH]),
                bcdToDec(_time[TIME_DAY]), bcdToDec(_time[TIME_HOURS]),
                bcdToDec(_time[TIME_MINUTES]), bcdToDec(_time[TIME_SECONDS]),
                half);
    } else
        sprintf(timeStamp, "20%02hhu-%02hhu-%02hhu  %02hhu:%02hhu:%02hhu",
                bcdToDec(_time[TIME_YEAR]), bcdToDec(_time[TIME_MONTH]),
                bcdToDec(_time[TIME_DAY]), bcdToDec(_time[TIME_HOURS]),
                bcdToDec(_time[TIME_MINUTES]), bcdToDec(_time[TIME_SECONDS]));

    return (timeStamp);
}

uint8_t RV8523::getSeconds() { return bcdToDec(_time[TIME_SECONDS]); }

uint8_t RV8523::getMinutes() { return bcdToDec(_time[TIME_MINUTES]); }

uint8_t RV8523::getHours() {
    if (is12Hours()) {
        return bcdToDec(_time[TIME_HOURS]);
    }
    return bcdToDec(_time[TIME_HOURS]);
}

uint8_t RV8523::getDay() { return bcdToDec(_time[TIME_DAY]); }

uint8_t RV8523::getWeekday() { return bcdToDec(_time[TIME_WEEKDAY]); }

uint8_t RV8523::getMonth() { return bcdToDec(_time[TIME_MONTH]); }

uint8_t RV8523::getYear() { return bcdToDec(_time[TIME_YEAR]); }

bool RV8523::is12Hours() {
    return readMask(RV8523_CONTROL1, RV8523_CONTROL1_12_24);
}

bool RV8523::isPM() {
    if (is12Hours()) {
        return readMask(RV8523_HOURS, RV8523_HOURS_12_AMPM);
    }
    return false;
}

bool RV8523::set12Hours() {
    return setMask(RV8523_CONTROL1, RV8523_CONTROL1_12_24);
}

bool RV8523::set24Hours() {
    return clearMask(RV8523_CONTROL1, RV8523_CONTROL1_12_24);
}

bool RV8523::setAlarm(uint8_t minute, uint8_t hour, uint8_t day,
                      uint8_t weekday, RV8523_ALARM_MODE mode) {
    bool retVal = disableAlarmInterrupt();
    if (retVal) {
        retVal = clearAlarmInterruptFlag();
    }

    if (retVal) {
        retVal = set24Hours();
    }

    if (retVal) {
        if (minute > 59) {
            return false;
        }
        if (hour > 23) {
            return false;
        }
        if (day < 1 || day > 31) {
            return false;
        }
        if (weekday > 6) {
            return false;
        }
    } else {
        return false;
    }

    /* 
        We want to disable all enable bits (logic 1) by default.
        We then check which alarms should by enabled (logic 0) using the mode.
    */
    uint8_t registers[4];
    registers[0] = RV8523_MINUTE_ALARM_AE_M | decToBcd(minute);
    registers[1] = RV8523_HOUR_ALARM_AE_H | decToBcd(hour);
    registers[2] = RV8523_DAY_ALARM_AE_D | decToBcd(day);
    registers[3] = RV8523_WEEKDAY_ALARM_AE_W | decToBcd(weekday);

    if (mode & 0b1) {
        registers[0] &= ~(RV8523_MINUTE_ALARM_AE_M);
    }
    if (mode & 0b10) {
        registers[1] &= ~(RV8523_HOUR_ALARM_AE_H);
    }
    if (mode & 0b100) {
        registers[2] &= ~(RV8523_DAY_ALARM_AE_D);
    }
    if (mode & 0b1000) {
        registers[3] &= ~(RV8523_WEEKDAY_ALARM_AE_W);
    }

    retVal = writeMultipleRegisters(RV8523_MINUTE_ALARM, registers, 4);
    if (retVal) {
        enableAlarmInterrupt();
    }

    return retVal;
}

bool RV8523::enableAlarmInterrupt() {
    return setMask(RV8523_CONTROL1, RV8523_CONTROL1_AIE);
}

bool RV8523::disableAlarmInterrupt() {
    return clearMask(RV8523_CONTROL1, RV8523_CONTROL1_AIE);
}

bool RV8523::readAlarmInterruptFlag() {
    return readMask(RV8523_CONTROL2, RV8523_CONTROL2_AF);
}

bool RV8523::clearAlarmInterruptFlag() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_AF);
}

bool RV8523::setTimerA(RV8523_TIMER_IRQ_MODE timer_irq_mode,
                       RV8523_SOURCE_CLOCK_FREQUENCY source_freq, uint8_t value,
                       bool set_interrupt, RV8523_TIMER_A_MODE mode) {
    bool retVal = true;

    retVal = setTimerAMode(RV8523_TIMER_A_MODE_DISABLED);
    if (retVal) {
        retVal = disableTimerAInterrupts();
    }
    if (retVal) {
        retVal = clearTimerAInterruptFlag();
    }

    if (retVal) {
        clearMask(RV8523_TIMER_A_CLOCK, RV8523_TIMER_A_CLOCK_TAQ);
    }
    if (retVal) {
        setMask(RV8523_TIMER_A_CLOCK, source_freq & RV8523_TIMER_A_CLOCK_TAQ);
    }

    if (retVal) {
        retVal = writeRegister(RV8523_TIMER_A, value);
    }
    if (retVal) {
        retVal = setTimerAIRQMode(timer_irq_mode);
    }
    if (retVal && set_interrupt) {
        if (mode == RV8523_TIMER_A_MODE_WATCHDOG) {
            retVal = enableTimerAWDInterrupt();
        }
        if (mode == RV8523_TIMER_A_MODE_COUNTDOWN) {
            retVal = enableTimerACDInterrupt();
        }
        /* TBD retVal = false if interrupt but timer set as disabled? */
    }
    if (retVal) {
        retVal = setTimerAMode(mode);
    }

    return retVal;
}

bool RV8523::setTimerAIRQMode(RV8523_TIMER_IRQ_MODE timer_mode) {
    bool retVal = true;

    switch (timer_mode) {
    case RV8523_TIMER_IRQ_MODE_PERMANENT:
        retVal = clearMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TAM);
        break;
    case RV8523_TIMER_IRQ_MODE_PULSED:
        retVal = setMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TAM);
        break;
    default:
        retVal = false;
        break;
    }

    return retVal;
}

bool RV8523::setTimerAMode(RV8523_TIMER_A_MODE mode) {
    bool retVal = true;

    /* Reset mode bits and then set the correct mode */
    retVal = clearMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TAC);
    if (retVal) {
        setMask(RV8523_TIMER_CLKOUT, mode << 1 & RV8523_TIMER_CLKOUT_TAC);
    }

    return retVal;
}

bool RV8523::disableTimerAInterrupts() {
    bool retVal = true;

    retVal = disableTimerACDInterrupt();
    if (retVal) {
        retVal = disableTimerAWDInterrupt();
    }

    return retVal;
}

bool RV8523::enableTimerAWDInterrupt() {
    return setMask(RV8523_CONTROL2, RV8523_CONTROL2_WTAIE);
}

bool RV8523::disableTimerAWDInterrupt() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_WTAIE);
}

bool RV8523::enableTimerACDInterrupt() {
    return setMask(RV8523_CONTROL2, RV8523_CONTROL2_CTAIE);
}

bool RV8523::disableTimerACDInterrupt() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_CTAIE);
}

bool RV8523::readTimerAWDInterruptFlag() {
    return readMask(RV8523_CONTROL2, RV8523_CONTROL2_WTAF);
}

bool RV8523::readTimerACDInterruptFlag() {
    return readMask(RV8523_CONTROL2, RV8523_CONTROL2_CTAF);
}

bool RV8523::clearTimerAInterruptFlag() {
    /* WTAF is read only - interrupt is resetted by reading the register.
       Therefore just clearing the CTAF is enough as we read the register in the process of clearing the mask.
       Moreover when clearing the Interrupt it was probably read before therefore it might already be cleaned.
    */
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_CTAF);
}

bool RV8523::setTimerB(RV8523_TIMER_IRQ_MODE timer_mode,
                       RV8523_SOURCE_CLOCK_FREQUENCY source_freq,
                       RV8523_TIMER_PULS_WIDTH pulse_width, uint8_t value,
                       bool set_interrupt, bool start_timer) {
    bool retVal = true;

    retVal = disableTimerB();
    if (retVal) {
        retVal = disableTimerBInterrupt();
    }
    if (retVal) {
        retVal = clearTimerBInterruptFlag();
    }

    uint8_t timer_b_clock = 0u;
    timer_b_clock |= (source_freq & RV8523_TIMER_B_CLOCK_TBQ);
    timer_b_clock |= (pulse_width << 4 & RV8523_TIMER_B_CLOCK_TBW);

    if (retVal) {
        retVal = writeRegister(RV8523_TIMER_B_CLOCK, timer_b_clock);
    }
    if (retVal) {
        retVal = writeRegister(RV8523_TIMER_B, value);
    }
    if (retVal) {
        retVal = setTimerBIRQMode(timer_mode);
    }
    if (retVal && set_interrupt) {
        retVal = enableTimerBInterrupt();
    }
    if (retVal && start_timer) {
        retVal = enableTimerB();
    }

    return retVal;
}

bool RV8523::setTimerBIRQMode(RV8523_TIMER_IRQ_MODE timer_mode) {
    bool retVal = true;

    switch (timer_mode) {
    case RV8523_TIMER_IRQ_MODE_PERMANENT:
        retVal = clearMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TBM);
        break;
    case RV8523_TIMER_IRQ_MODE_PULSED:
        retVal = setMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TBM);
        break;
    default:
        retVal = false;
        break;
    }

    return retVal;
}

bool RV8523::enableTimerB() {
    return setMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TBC);
}
bool RV8523::disableTimerB() {
    return clearMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_TBC);
}
bool RV8523::enableTimerBInterrupt() {
    return setMask(RV8523_CONTROL2, RV8523_CONTROL2_CTBIE);
}
bool RV8523::disableTimerBInterrupt() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_CTBIE);
}
bool RV8523::readTimerBInterruptFlag() {
    return readMask(RV8523_CONTROL2, RV8523_CONTROL2_CTBF);
}
bool RV8523::clearTimerBInterruptFlag() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_CTBF);
}

bool RV8523::enableSecondTimerInterrupt() {
    return setMask(RV8523_CONTROL1, RV8523_CONTROL1_SIE);
}

bool RV8523::disableSecondTimerInterrupt() {
    return clearMask(RV8523_CONTROL1, RV8523_CONTROL1_SIE);
}
bool RV8523::readSecondTimerInterruptFlag() {
    return readMask(RV8523_CONTROL2, RV8523_CONTROL2_SF);
}
bool RV8523::clearSecondTimerInterruptFlag() {
    return clearMask(RV8523_CONTROL2, RV8523_CONTROL2_SF);
}

RV8523_CLKOUT_FREQUENCY RV8523::getCLKOUTFrequency() {
    uint8_t value = readMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_COF);
    return (RV8523_CLKOUT_FREQUENCY)(value >> 3u);
}
bool RV8523::setCLKOUTFrequency(RV8523_CLKOUT_FREQUENCY frequency) {
    bool retVal = clearMask(RV8523_TIMER_CLKOUT, RV8523_TIMER_CLKOUT_COF);
    if (retVal) {
        retVal = setMask(RV8523_TIMER_CLKOUT,
                         frequency << 3 & RV8523_TIMER_CLKOUT_COF);
    }
    return retVal;
}

RV8523_BATTERY_SWITCHOVER RV8523::getBatterySwitchover() {
    uint8_t type = 0xFF;
    uint8_t val = readMask(RV8523_CONTROL3, RV8523_CONTROL3_PM);
    for (uint8_t i = 0; i < POWER_MANAGEMENT_LENGTH; i++) {
        if (val == RV8523_BATTERY_SWITCHOVER_VALUES[i]) {
            type = (RV8523_BATTERY_SWITCHOVER)i;
            break;
        }
    }
    /* In case RV8523_BATTERY_SWITCHOVER_DIS__BAT_LOW_EN is set with 0b011 which we don't catch in the for loop. */
    /* Only case where we would have 0xFF here.*/
    if (type == 0xFF) {
        type = RV8523_BATTERY_SWITCHOVER_DIS__BAT_LOW_EN;
    }
    return (RV8523_BATTERY_SWITCHOVER)type;
}

bool RV8523::setBatterySwitchover(RV8523_BATTERY_SWITCHOVER type) {
    return setMask(RV8523_CONTROL3, RV8523_BATTERY_SWITCHOVER_VALUES[type]);
}

bool RV8523::readBatterySwitchoverFlag() {
    return readMask(RV8523_CONTROL3, RV8523_CONTROL3_BSF);
}

bool RV8523::clearBatterySwitchoverFlag() {
    return clearMask(RV8523_CONTROL3, RV8523_CONTROL3_BSF);
}
bool RV8523::enableBatterySwitchoverInterrupt() {
    return setMask(RV8523_CONTROL3, RV8523_CONTROL3_BSIE);
}

bool RV8523::disableBatterySwitchoverInterrupt() {
    return clearMask(RV8523_CONTROL3, RV8523_CONTROL3_BSIE);
}

bool RV8523::readBatteryLowFlag() {
    return readMask(RV8523_CONTROL3, RV8523_CONTROL3_BLF);
}

bool RV8523::enableBatteryLowInterrupt() {
    return setMask(RV8523_CONTROL3, RV8523_CONTROL3_BLIE);
}

bool RV8523::disableBatteryLowInterrupt() {
    return clearMask(RV8523_CONTROL3, RV8523_CONTROL3_BLIE);
}

bool RV8523::setFrequencyOffsetMode(RV8523_FREQUENCY_OFFSET_MODE_TYPE mode) {
    if (mode == RV8523_FREQUENCY_OFFSET_MODE_TYPE_2HOURS) {
        return clearMask(RV8523_FREQUENCY_OFFSET, RV8523_FREQUENCY_OFFSET_MODE);
    }
    if (mode == RV8523_FREQUENCY_OFFSET_MODE_TYPE_1MIN) {
        return setMask(RV8523_FREQUENCY_OFFSET, RV8523_FREQUENCY_OFFSET_MODE);
    }
    return false;
}

RV8523_FREQUENCY_OFFSET_MODE_TYPE RV8523::getFrequencyOffsetMode() {
    return (
        RV8523_FREQUENCY_OFFSET_MODE_TYPE)(readMask(
                                               RV8523_FREQUENCY_OFFSET,
                                               RV8523_FREQUENCY_OFFSET_MODE) >>
                                           7);
}

bool RV8523::setFrequencyOffset(int8_t offset) {
    if (offset < -64 || offset > 63) {
        return false;
    }
    bool retVal =
        clearMask(RV8523_FREQUENCY_OFFSET, RV8523_FREQUENCY_OFFSET_OFFSET);
    if (retVal) {
        retVal = setMask(RV8523_FREQUENCY_OFFSET, offset);
    }
    return retVal;
}

int8_t RV8523::getFrequencyOffset() {
    return sevenBitSignedToDec(
        readMask(RV8523_FREQUENCY_OFFSET, RV8523_FREQUENCY_OFFSET_OFFSET));
}

bool RV8523::reset() {
    return writeRegister(RV8523_CONTROL1, RV8523_CONTROL1_RESET);
}

uint8_t RV8523::readRegister(uint8_t addr) {
    _twoWire->beginTransmission(RV8523_ADDR);
    _twoWire->write(addr);
    _twoWire->endTransmission();

    _twoWire->requestFrom(RV8523_ADDR, (uint8_t)1);
    if (_twoWire->available()) {
        return _twoWire->read();
    }
    return 0xFF;
}

bool RV8523::writeRegister(uint8_t addr, uint8_t val) {
    _twoWire->beginTransmission(RV8523_ADDR);
    _twoWire->write(addr);
    _twoWire->write(val);
    if (_twoWire->endTransmission() != 0) {
        return false;
    }
    return true;
}

bool RV8523::readMultipleRegisters(uint8_t addr, uint8_t *dest, uint8_t len) {
    _twoWire->beginTransmission(RV8523_ADDR);
    _twoWire->write(addr);
    if (_twoWire->endTransmission() != 0) {
        return false;
    }
    _twoWire->requestFrom(RV8523_ADDR, len);

    for (uint8_t i = 0; i < len; i++) {
        dest[i] = _twoWire->read();
    }
    return true;
}

bool RV8523::writeMultipleRegisters(uint8_t addr, uint8_t *val, uint8_t len) {
    _twoWire->beginTransmission(RV8523_ADDR);
    _twoWire->write(addr);

    for (int i = 0; i < len; i++) {
        _twoWire->write(val[i]);
    }

    if (_twoWire->endTransmission() != 0) {
        return false;
    }
    return true;
}

bool RV8523::setMask(uint8_t addr, uint8_t mask) {
    uint8_t value = readRegister(addr);
    value |= mask;
    return writeRegister(addr, value);
}

bool RV8523::clearMask(uint8_t addr, uint8_t mask) {
    uint8_t value = readRegister(addr);
    value &= ~mask;
    return writeRegister(addr, value);
}

int RV8523::readMask(uint8_t addr, uint8_t mask) {
    uint8_t value = readRegister(addr);
    return value & mask;
}

uint8_t RV8523::bcdToDec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8_t RV8523::decToBcd(uint8_t dec) { return ((dec / 10) << 4) | (dec % 10); }

int8_t RV8523::sevenBitSignedToDec(int8_t bin) {
    bin &= 0b1111111;
    if (bin & 0b1000000) {
        bin -= 128;
    }
    return bin;
}
