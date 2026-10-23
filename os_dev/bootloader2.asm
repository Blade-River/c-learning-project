BITS 16
ORG 0x7C00

start:
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

gdt_start:
  db 0x00000000
  db 0x00000000
  
  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 0x10010010b
  db 0x11001111b
  db 0x00

  dw 0xFFFF
  dw 0x0000
  db 0x00
  db 0x0


times 510 - ($ - $$) db 0
dw 0xAA55
