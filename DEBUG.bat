@echo off
diskpart -s C:\OS\VHDScriptDetach.txt
cd C:\qemu\
C:\qemu\qemu-system-x86_64w.exe -hda C:\OS\Harddrive.vhd -L Bios  -vga std -name Hydra -m 512M -gdb tcp:127.0.0.1:1234
diskpart -s C:\OS\VHDScriptAttach.txt