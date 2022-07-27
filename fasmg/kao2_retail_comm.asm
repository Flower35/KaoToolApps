;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; "kao2_retail_comm.asm"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include 'cpu/80386.inc'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

COMM_FUNC_ADDR  EQU 0x005CC660
COMM_ARGS_ADDR  EQU 0x00641E00

COMM_ACT_SKIP      EQU 0
COMM_ACT_WAITING   EQU 1
COMM_ACT_CDECL     EQU 2
COMM_ACT_STDCALL   EQU 3
COMM_ACT_THISCALL  EQU 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

format binary as 'bin'
use32

org COMM_FUNC_ADDR

    push ebp
    mov ebp,esp

    push ebx
    push esi
    push edi

  .communicator_while_loop:

    ;; "COMM_ARG[0]", lower 3 bits: calling convention
    ;; "COMM_ARG[0]", upper bits: number of arguments
    ;; "COMM_ARG[1]" holds the call destination
    ;; "COMM_ARG[2]" holds the thiscall object or the 1st argument

    mov eax,dword[COMM_ARGS_ADDR + 0x04 * 0]
    mov ecx,eax
    and eax,7

    ;; while "COMM_ARG[0]" != COMM_ACT_SKIP
    cmp eax,COMM_ACT_SKIP
      je .back_to_the_game

    shr ecx,3
    mov ebx,ecx
    shl ebx,2

    xor edx,edx
    xor esi,esi

    cmp eax,COMM_ACT_CDECL
      je .copying_args_init

    cmp eax,COMM_ACT_STDCALL
      je .copying_args_init

    cmp eax,COMM_ACT_THISCALL
      jne .wait_for_release

    mov edx,dword[COMM_ARGS_ADDR + 0x04 * 2]
    add esi,0x04

  .copying_args_init:

    add esi,(COMM_ARGS_ADDR + 0x04 * 2)

    sub esp,ebx
    mov edi,esp

    ;; No stack clean-up for STDCALL and THISCALL
    cmp eax,COMM_ACT_CDECL
      je .copying_args_loop
      xor ebx,ebx

  .copying_args_loop:

    rep movsd

  .make_the_call:

    mov ecx,edx
    call dword[COMM_ARGS_ADDR + 0x04 * 1]
    add esp,ebx

    mov dword[COMM_ARGS_ADDR + 0x04 * 1],eax
    mov dword[COMM_ARGS_ADDR + 0x04 * 0],COMM_ACT_WAITING

  .wait_for_release:

    cmp dword[COMM_ARGS_ADDR + 0x04 * 0],COMM_ACT_WAITING
      je .wait_for_release

    jmp .communicator_while_loop

  .back_to_the_game:

    pop edi
    pop esi
    pop ebx

    pop ebp
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
