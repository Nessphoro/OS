@echo off
cd C:\OS\
fwimage app BOOTX64.dll BOOTX64.efi
copy BOOTX64.efi G:\BOOTX64.EFI
diskpart -s C:\OS\VHDScriptDetach.txt
cd C:\qemu\
C:\qemu\qemu-system-x86_64w.exe -hda C:\OS\Harddrive.vhd -L EFIBios -vga cirrus -name Hydra -m 512M
diskpart -s C:\OS\VHDScriptAttach.txt