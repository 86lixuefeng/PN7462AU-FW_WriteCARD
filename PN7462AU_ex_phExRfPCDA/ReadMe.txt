/***********************phExRfPCDA- Example for customer Demo*******************************/

This example demonstrates simple low level API usage to perform detection, anti-collision, activation,
authentication, activation and R/W operation on the Type A cards.

Application is using low level RF interface HAL implementation in flash.

There is limitation to only one card at the time.

Supports- TypeA cards are Type1 TOPAZ,MIFARE Ultralight, MIFARE Classic, MIFARE DESFire.
          In case of MIFARE Classic card also authentication with default key is demonstrated.
          In the case of MIFARE DESFire card L4 activation is demonstrated.
