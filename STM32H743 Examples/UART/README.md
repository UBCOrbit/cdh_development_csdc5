# Universal Asynchronous Receiver/Transmitter (UART) Example

The following example displays how to use the USART3 peripheral on the STM32H743. While using the Nucleo-144 development board, this example can be used to print directly to a serial console through a USB connection to the board. Note that this will ONLY work if pins PD8 and PD9 are used for the USART3 peripheral, other pin configurations will not enable printing to a serial console.

This program waits until it receives 4 bytes on the USART3 peripheral, then transmits back the same 4 bytes on the USART3 peripheral.

## UART Initialization settings

* **`huart3.Init.BaudRate`**: Defines the speed of transfer on the peripheral. Receiving and transmitting devices should be initialized with the same baud rate to allow for proper communication.
* **`huart3.Init.WordLength`**: Defines the number of data bits transmitted or received in a frame. Set to `UART_WORDLENGTH_8B` for an 8-bit long UART frame.
* **`huart3.Init.StopBits`**: Defines the number of stop bits to be used.
* **`huart3.Init.Parity`**: Defines whether or not parity will be used in the frame. The addition of parity bits can be used for data validation that a message was properly transmitted over UART.
* **`huart3.Init.Mode`**: Defines the UART transfer mode. Set to `UART_MODE_TX_RX`, as we both receive and transmit in the example.
