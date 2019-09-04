/******************phExCTEMVCo - Example for Customer Demo ***********************************/

/* Features */
NON-RTOS Based
Include PAL for CT
This example performs an EMVCo activation and EMVCo ATR parsing.
It also determines the protocol supported by the card and applies the protocol supported.

/* CT EMVCo FEATURES */
This Example Supports EMVCo profile for Ct Module.
Supports Credit,Maestro,Cirrus,Electron, Vpay,Visa,Amex card activation  and their identification by sending Specific EF file for those cards. .
*/


/* APDU Transactions */
The example sends select commands for nine pre-selected types of EMVCO cards after successful activation
1.Master card:Credit or Debit  2.Visa card: Credit or Debit  3.Master card:Maestro  4.Master card:Cirrus
5.Master card:Maestro UK       6.Visa card:Electron card     7.Visa card:V PAY card  8.Visa card:VISA Plus card
9.Amex card.

Note:
=====
The Example is applicable only for PN7462