# README

## Required components
- Visual Studio Code
- [Arm Keil Stuido Pack (MDK v6)](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack)
- ST-Link/V2 flasher software (e.g. `sudo apt-get install stlink-tools`)

## Build
1. Install Visual Studio Code, and the Arm Keil Studio Pack extensions
2. In Visual Studio Code, run "CMSIS: Build Solution", or 

Alternatively, you can run `cbuild ./project.csolution.yml --context-set --packs` in a terminal
window.

## Flash
`st-flash --format ihex write ./out/project/Flash/project.hex`
