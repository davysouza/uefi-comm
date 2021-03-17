#include <Uefi.h>

#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

// UEFI Variable definition
#define MY_VAR_NAME L"MyVar"
#define MY_VAR_GUID { 0xF299EF14, 0x61D1, 0x4BF0, { 0xBF, 0xBC, 0x56, 0x5A, 0xF8, 0x8D, 0xF0, 0xC9 } }

#define MAX_SIZE 32

EFI_STATUS
EFIAPI
EntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE* SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  EFI_GUID MyVarGuid = MY_VAR_GUID;
  CHAR8    MyVarValue[MAX_SIZE] = {0};
  UINT64   MyVarSize = 12;

  Print(L"UEFI/OS Communication App\n\n");

  // Get variable
  Print(L"Reading variable...\n");
  Status = gRT->GetVariable(MY_VAR_NAME, &MyVarGuid, NULL, &MyVarSize, MyVarValue);
  if(EFI_ERROR(Status)) {
    if(Status == EFI_NOT_FOUND) {
      Print(L"Variable not found.\n");
    } else {
      Print(L"Error to get UEFI variable: %r\n", Status);
      goto FINISH;
    }
  }

  // Print result
  if(!EFI_ERROR(Status)) {
    CHAR16 MyVarValueUnicode[MAX_SIZE] = {0};
    AsciiStrToUnicodeStrS(MyVarValue, MyVarValueUnicode, MyVarSize);
    Print(L"MyVar: %s\n\n", MyVarValueUnicode);
  }

  // Set variable
  Print(L"Writing variable...\n");

  MyVarSize = 10;
  AsciiStrCpyS(MyVarValue, MyVarSize, "Hello OS!\0");
  Status = gRT->SetVariable(MY_VAR_NAME, 
                            &MyVarGuid, 
                            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                            MyVarSize,
                            MyVarValue);
  if(EFI_ERROR(Status)) {
    Print(L"Error to set UEFI variable: %r\n", Status);
    goto FINISH;
  }

FINISH:
  return Status;
}
