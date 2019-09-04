/******************phExCt - Example for Customer Demo ***********************************/

/* Features */
NonRTOS Based
On top of Bare Metal HAL
This(PN7462AU\phExCt) uses only the CT HAL APIs to demonstrate the CT functionality.
SELECT master card APDU is sent depending on the protocol supported by the card and expects RAPDU 0x90 0x00.
This example performs EMVCo activation and EMVCo ATR praising.
It also determines the protocol supported by the card.


/* CT Specifications */
This file Indicates Use of Hal api for Ct Module.
Supports Activation for all class ,transmission and reception of apdu using Hal Apis.
This application can be run on MasterCard. 


Note:
=====
The Example is applicable only for PN7462.