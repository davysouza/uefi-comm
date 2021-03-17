# UEFI/OS Communication

A Python and C implementation of how to send data from UEFI to OS using UEFI variables


## Build and Running UEFI application
To build the application, run:
```
$ cd uefi-app
$ ./build.sh
```

Then, copy the `.efi` application to a FAT-32 flash media at the following path:
```
$FLASH_MEDIA/EFI/BOOT/BOOTX64.efi
```

## Running the Python application
The python application must be executed on a Linux system. To execute the application, just run the following commands:
```
$ cd os-app
$ sudo ueficomm.py
```
