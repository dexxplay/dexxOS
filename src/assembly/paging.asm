; paging

bits 32
global paging_load_directory
global paging_enable
; Sets the given paging directory to be used by the CPU
paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    ret
; Set paging to be used by CPU
paging_enable:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    pop ebp
    ret

