@echo off
"C:\Program Files\SEGGER\JLink_V932\JLink.exe" -CommanderScript "C:\Users\Esteban\Documents\GitHub\i-mop-qorvo-uci-fw\flash_dwm3001cdk.jlink" > flash_output.txt 2>&1
type flash_output.txt
