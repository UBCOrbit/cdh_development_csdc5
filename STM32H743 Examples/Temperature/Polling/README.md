# Temperature Sensor Example - Polling

The following example displays how to use the temperature sensor on the STM32H743. The temperature sensor is accessed through ADC3 on channel 18. 
It follows [this procedure](https://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/c9/a3/76/fa/55/46/45/fa/DM00314099/files/DM00314099.pdf/jcr:content/translations/en.DM00314099.pdf#page=973) for reading data from the ADC and calculating the temperature.


Additional information can be found in: 
[ADC Characteristics](https://www.st.com/resource/en/datasheet/stm32h743zi.pdf#page=160)
and
[Temperature Sensor Characteristics](https://www.st.com/resource/en/datasheet/stm32h743zi.pdf#page=170)

This program starts off by printing the calibration values of the temperature sensor at 30°C and 110°C. It then polls the ADC and prints the converted raw sensor value and calculated temperature in a loop.

The clock rate and sampling cycles must be selected such that the sampling time is greater than the minimum sampling time for the ADC (9µs). 

## ADC Initialization Settings
* **`hadc3.Instance`**: Defines the register base address. Set to `ADC3` to access the temperature sensor.
* **`hadc3.Init.ClockPrescaler`**: Defines the clock source and prescaling to be used for the ADC.
* **`hadc3.Init.BoostMode`**: Must be `ENABLED` if the ADC clock rate is >= 20 MHz. It should be set automatically by STM32CubeMX when configuring the clock rate.

* **`HAL_ADC_Init(&hadc3)`**: Initializes the ADC peripheral.

## Channel Configuration

* **`sConfig.Channel`**: Selects the channel. Set to `ADC_CHANNEL_TEMPSENSOR` (channel 18).
* **`sConfig.SamplingTime`**: Defines the number of sampling cycles. Options enumerated in STM32CubeMX or in `stm32h7xx_hal_adc_ex.h`.

* **`HAL_ADC_ConfigChannel(&hadc3, &sConfig)`**: Configures the channel.
* **`HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED)`**: Performs an ADC automatic self-calibration.