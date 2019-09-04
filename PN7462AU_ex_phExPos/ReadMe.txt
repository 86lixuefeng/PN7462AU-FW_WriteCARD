/******************phExPos - Example for Customer Demo ***********************************/
/* Features */
Includes Both CT and Clif
Includes PAL for both CT and Clif
Includes DiscLoop for Clif
RTOS Based


/* CLif Polling FEATURES */
1. Reader Modes- Supports TypeA, TypeB.
2. Never goes to Listen Mode.

The POS demo architecture is split into application layer(L2) and low level EMVCo compliant layer L which is hosted on the PN7462AU.

The application layer L2 commands are simulated in reference micro-controller board (LPC1769)
and L1 layer components are placed in PN7462AU.

The application APDU commands (L2) are communicated to PN7462AU through SPI host interface.

PN7462AU GPIO pin is used to synchronize command/response between LPC1769 and PN7462AU.

IRQ pin is used to notify valid ISO 14443-4 card to LPC1769.



/* CLif EMVCo FEATURES */
1. Supports Type A EMVCo Cards with application specific to MasterCard till generation of application cryptogram.
2. Supports Type B protocol.


/* CT EMVCo FEATURES */
Supports Credit,Maestro,Cirrus,Electron, Vpay,Visa,Amex card activation and their identification by sending Specific select AID for those cards. 


Note:
=====
The Example is applicable only for PN7462. Not Applicable for PN7362/PN7360.