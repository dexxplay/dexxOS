; dexxOS bootloader

org 0x7C00 ; the origin of are lable's offset
bits 16

; -------- FAT12 Descripter Table ---------
jmp short start
nop ; make start up slower by pausing a sec (allowing system to set up)

bdb_oem:                    db 'MSWIN4.1'           ; 8 bytes long
bdb_bytes_per_sector:       dw 512                  ; bytes per sector
bdb_sectors_per_cluster:    db 1                    ; number 
bdb_reserved_sectors:       dw 1
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880                 ; 2880 * 512 = 1.44MB
bdb_media_descriptor_type:  db 0F0h                 ; F0 = 3.5" floppy disk
bdb_sectors_per_fat:        dw 9                    ; 9 sectors/fat
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

; extended boot record
boot_drive:                 db 0                    ; 0x00 floppy, 0x80 hdd,
                            db 0                    ; reserved
ebr_signature:              db 29h
ebr_volume_id:              db 12h, 34h, 56h, 78h   ; serial number (value doesn't matter)
ebr_volume_label:           db 'dexxOS     '        ; 11 bytes, padded with spaces
ebr_system_id:              db 'FAT12   '           ; 8 bytes

start:
; Set segment regesterys
 xor ax, ax ; ax == 0
 mov ds, ax
 mov ss, ax
 mov es, ax
 mov sp, 0x7C00

; Enable A20 line
 in al, 0x92
 or al, 0x2
 out 0x92, al

; Clear screen by switching to text mode (while we still have BIOS)
 mov ah, 0x0
 mov al, 0x3 ; text mode = 0x3 VGA mode = 0x13
 int 0x10

load_dexxos:
 mov byte [boot_drive], dl

; Get memory map
 mov ax, 0x0100
 mov es, ax
 mov di, 0
 
 mov ebx, 0
 mov cx, 50
 .get_memory_entry: 
 push cx
  mov eax, 0xE820
  mov ecx, 24
  mov edx, 0x534D4150 ;signature
  int 15h
 ;jc error_memory (will add a case later)
 add di, 24
 pop cx
 cmp ebx, 0
 je .last_entry
 loop .get_memory_entry
 .last_entry:

; Load dexxOS
 mov dword [0xF008], 1 ; first sector of dexxOS code on disk
 mov edx, 10
 mov cx, 12 ;load 12x64 sectors = 384 KB
 mov ax, 0x10
 .load_dexxos_from_hard_disk:
  mov word [0xF000], 0x0010 ;signature
  mov word [0xF002], 64
  mov bx, ax
  shl bx, 12
  mov word [0xF004], bx ;memory offset
  mov bx, ax
  shl bx, 8
  and bx, 0xF000
  mov word [0xF006], bx ;segment
  mov dword [0xF00C], 0
  pusha ;some hardware change values in registers during int 13h so we have to save everything
  mov ah, 0x42
  mov si, 0xF000
  mov dl, byte [boot_drive]
  int 13h
  popa
  ;jc error_loading
  
  add edx, 64
  mov dword [0xF008], edx
  add ax, 0x8
 loop .load_dexxos_from_hard_disk

; Enter Protected Mode 32-bit
 cli ; Clear/Disable Interrupts
 lgdt [gdt_descriptor] ; set GDT to be used
 mov eax, cr0
 or eax, 0x1  ; set PE (Protected Enable) bit in CR0
 mov cr0, eax
 jmp 0x0008:load32 ; far jump to clear current cpu instructions
 
bits 32
load32:
 ; Set Regesterys
 mov ax, 0x0010 ; DATA_SEG
 mov ds, ax
 mov es, ax
 mov fs, ax
 mov gs, ax
 mov ss, ax

 mov esp, 0xFFF0 ;set stack pointer
 jmp 0x10000 ;execute dexxOS

; GDT (Flat memory Model)
gdt_start:
gdt_null:
    dq 0 ;first item is null

; offset 0x8
gdt_code:     ; [cs] should point to this
    dw 0xFFFF ; Segment limit first 0-15 bits 
    dw 0      ; Base first 0-15 bits
    db 0      ; Base first 0-15 bits
    db 10011010b   ; Access byte
    db 11001111b ; High 4 bit flag and the low 4 bit flags
    db 0      ; Base 24-31 bits
; offset 0x10
gdt_data:     ; linked to [ds], [ss], [es], [fs], [gs]
    dw 0xFFFF ; Segment limit first 0-15 bits 
    dw 0      ; Base first 0-15 bits
    db 0      ; Base first 0-15 bits
    db 10010010b   ; Access byte
    db 11001111b ; High 4 bit flag and the low 4 bit flags
    db 0      ; Base 24-31 bits
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

; Padding & Magic BIOS Number.
times 506-($-$$) db 0
db 'D','E','X','X' ; not required but it helps when viewing dexxos.img as hex
dw 0xAA55