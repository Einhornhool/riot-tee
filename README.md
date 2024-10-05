# RIOT-TEE

> Note: Very much work in progress, probably not usable for anyone else

> Warning, since someone has already forked this: In the current state I can make no guarantees for security. As of now it is completely untested. Please do not use this for anything safety or security related.

Secure Firmware for [RIOT OS](https://github.com/RIOT-OS/RIOT), for platforms with Trustzone-M support.

Currently only usable on nRF9160.

![design](out/design/object-diagram/object-diagram.png)

# Dependencies
## CryptoService API and Library
- Access currently restricted
- https://github.com/LP-HAW/CryptoService/tree/feature/nrf9160dk-target-integration

## CMSIS 5
- Provides drivers for Arm Cortex-M devices
- https://github.com/ARM-software/CMSIS_5.git

## NRFX Lib
- Provides drivers and peripherals for Nordic microcontrollers
- https://github.com/NordicSemiconductor/nrfx.git

## Trusted Firmware-M (Fork)
- Provides low-level drivers for CryptoCell 312
- This fork modfies the driver code to be compatible with Cryptocell 310
- https://github.com/Einhornhool/trusted-firmware-m/tree/main

## SDK-NRFXLIB
- Provides crypto libraries for Nordic microcontrollers
- https://github.com/nrfconnect/sdk-nrfxlib.git
