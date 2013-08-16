			sudo losetup -o32256 /dev/loop0 /mnt/hgfs/OS/hard.img
			sudo mount -tvfat /dev/loop0 /media/hd0
			
			sudo cp /mnt/hgfs/OS/menu.lst /media/hd0/boot/grub
			
			sudo umount /media/hd0
			sudo losetup -d /dev/loop0