/******************phExCT7816 - Example for Customer Demo ***********************************/
/* Features */
NonRTOS Based
Include PAL for CT
This Example Supports 7816 profile for S-Costa card for Ct Module.
Supports S-Costa card Activation, MF ,EF Creation and Selection also writes and reads binary to selected EF in Scosta card,
This example performs an ISO7816 ATR parsing.
It determines the protocol supported by the card and applies the protocol supported.

/* APDU Transactions*/
The following APDU's are sent after the activation of the card. If the card supports the following APDU's,
proper responses come from the card.
Create MF, Create EF, Select EF, Write Binary, Read Binary, Delete EF.


Note:
=====
The Example is applicable only for PN7462