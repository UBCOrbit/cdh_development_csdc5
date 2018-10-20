# Controller Area Network (CAN) Peripheral Example

The following example displays how to use the FDCAN peripheral on the STM32H743. The peripheral is set up to run in [internal loop back mode](https://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/c9/a3/76/fa/55/46/45/fa/DM00314099/files/DM00314099.pdf/jcr:content/translations/en.DM00314099.pdf#page=2440), meaning that it will treat any messages it transmits as messages it receives. This mode is useful for testing the CAN peripheral without requiring a second micro-controller to act as the receiver. Due to this setup, no external connections need to be made to a separate micro-controller, and any connections made will not work. It is also important to note that this example employs the classic CAN protocol, and does not make use of any of the FD CAN features.

This program transmits the message "Trillium" on the FDCAN1 peripheral, waits for receival of the message, and transmits that message through USART3 for debugging.

## CAN Initialization Settings
The biggest catch to working with the CAN peripheral is configuring its large amount of settings. The important settings for this example are:
* **`hfdcan1.Init.Mode`**: Defines the test mode of the CAN peripheral, set to `FDCAN_MODE_INTERNAL_LOOPBACK` for this test. To enable communication with an external device, set this configuration to `FDCAN_MODE_NORMAL`. 
* **`hfdcan1.Init.StdFiltersNbr`**: Defines the number of standard message ID filters that the RX handler will choose to accept. Set to 1 for this example, because we are only transmitting one type of message.
* **`hfdcan1.Init.RxFifo0ElmtsNbr`**: Specifies the number of elements that the RX FIFO0 can hold. Set to 1, because one message is transmitted at a time.
* **`hfdcan1.Init.RxFifo0ElmtsSize`**: Specifies the data size of the message stored in the RX FIFO0.
* **`hfdcan1.Init.TxFifoQueueElmtsNbr`**: Specifies the number of TX buffers used for the FIFO. To use the FIFO capability, this setting must be set to at least 1 according to the [reference manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/c9/a3/76/fa/55/46/45/fa/DM00314099/files/DM00314099.pdf/jcr:content/translations/en.DM00314099.pdf#page=2451).
* **`hfdcan1.Init.TxFifoQueueMode`**: Specifies if the FIFO or the queue will be used for TX transactions. This example uses the FIFO, so this setting is set to `FDCAN_TX_FIFO_OPERATION`.
* **`hfdcan1.Init.TxElmtSize`**: Specifies the data size of the messages to transmit.

## TX Header Configuration
Prior to transmitting the first message, a TX Header must be initialized and configured. This header is passed to any of the transmit functions and defines the format in which the data will be sent. Each message type will require its own TX Header, with a unique identifier. This allows the receiver to filter for only the message types that it is interested in. It is important that these settings match what was defined in the peripheral intialization:
* **`TxHeader.DataLength`**: The amount of bytes that will be sent in the frame. For classic CAN protocol, this value can only go up to `FDCAN_DLC_BYTES_8`. This value is can be less than or equal to `hfdcan1.Init.TxElmtSize`.
* **`TxHeader.FDFormat`**: This field must match the mode set in `hfdcan1.Init.FrameFormat`. The two options are either `FDCAN_CLASSIC_CAN` or `FDCAN_FD_CAN`.

## RX Filter Configuration
Filtering is started after the complete identifier has been received. It will accept the message if the filter and Tx identifiers match. Configuration details are outlined below:
* **`sFilterConfig.IdType`**: Specify if you are using `FDCAN_STANDARD_ID` or `FDCAN_EXTENDED_ID`. We are using a standard FDCAN in this example so we will use `FDCAN_STANDARD_ID`.
* **`sFilterConfig.FilterIndex`**: Specifies the filter which will be initialized. In this example we are using filter RxFIFO0 so we set the index to `0`.
* **`sFilterConfig.FilterType`**: Specify which type of filter you are using. Options include `FDCAN_FILTER_DUAL`, `FDCAN_FILTER_RANGE`, `FDCAN_FILTER_MASK`, `FDCAN_FILTER_RANGE_NO_EIDM`. Any of these will work for this example.
* **`sFilterConfig.FilterConfig`**: Configure the location that will store Rx. In this example we are using `FDCAN_FILTER_TO_RXFIFO0`.
* **`sFilterConfig.FilterID1`**: Specifies the filter identification. This should be the same as `TxHeader.Identifier`.
