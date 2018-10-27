# Universal Asynchronous Receiver/Transmitter (UART) Example

The following example displays how to use the USART3 peripheral on the STM32H743. It is configured to use pins PD8 as USART3_TX and PD9 as USART3_RX. 

This program waits until it receives 4 bytes on the USART3 peripheral, then prints back the same 4 bytes on the USART3 peripheral. This allows it to print directly to the serial console.

## UART Initialization settings

* **`huart3.Init.BaudRate`**: Defines the baudrate for the peripheral. Set to match the device used for communication. 
* **`huart3.Init.WordLength`**: Defines the number of data bits transmitted or received in a frame. Set to `UART_WORDLENGTH_8B` for an 8-bit long UART frame.
* **`huart3.Init.StopBits`**: Defines the number of stop bits to be used.
* **`huart3.Init.Parity`**: Defines parity mode.
* **`huart3.Init.Mode`**: Defines the UART transfer mode. Set to `UART_MODE_TX_RX` as we both receive and transmit in the example.
