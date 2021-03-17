# 
# This code was a simple C program which demonstrates
# how to use efivarfs to read, write or delete an EFI 
# NVRAM variable implemented by Finnbarr P. Murphy.
# 
# The original implementation can be found at:
# https://blog.fpmurphy.com/2012/12/efivars-and-efivarfs.html
#

import os
import array
from fcntl import ioctl

SYS_FS_EFIVARS_PATH   = '/sys/firmware/efi/efivars/'
SYS_FS_IMMUTABLE_FLAG = 0x00000010
SYS_FS_GET_FLAG       = 0x80086601
SYS_FS_SET_FLAG       = 0x40086602

MY_VAR_NAME = 'MyVar'
MY_VAR_GUID = 'f299ef14-61d1-4bf0-bfbc-565af88df0c9'

def read_variable(name, guid):
    var_path = SYS_FS_EFIVARS_PATH + name + '-'  + guid

    attr = []
    data = []

    if os.path.exists(var_path):
        with open(var_path, 'rb') as fd:
            buffer = fd.read()

            attr = buffer[:4]
            data = buffer[4:]

    return [attr, data]

def write_variable(name, guid, attr, data):
    var_path = SYS_FS_EFIVARS_PATH + name + '-'  + guid
    if os.path.exists(var_path):
        delete_variable(name, guid)

    buffer = bytes(attr) + bytes(data)
    with open(var_path, 'wb') as fd:
        fd.write(buffer)

def delete_variable(name, guid):
    var_path = SYS_FS_EFIVARS_PATH + name + '-'  + guid

    if os.path.exists(var_path):
        with open(var_path, 'rb') as fd:
            buffer = array.array('h', [0])

            # removing immutable flag from efi variable file
            ioctl(fd, SYS_FS_GET_FLAG, buffer)
            buffer[0] = buffer[0] & ~SYS_FS_IMMUTABLE_FLAG
            ioctl(fd, SYS_FS_SET_FLAG, buffer)

        os.remove(var_path)

def main():
    print('UEFI/OS Communication App\n')

    print('Reading variable...')
    [attr, data] = read_variable(MY_VAR_NAME, MY_VAR_GUID)
    
    print('MyVar: %s\n' % data.decode('utf-8'))

    print('Writing variable...')
    write_variable(MY_VAR_NAME, MY_VAR_GUID, attr, 'Hello UEFI!\0'.encode('utf-8'))
    print('success')

if __name__ == '__main__':
    main()
