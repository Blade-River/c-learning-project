BITS 16
ORG 0x7C00

start:
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov si, message

print_msg:
    lodsb
    cmp al, 0
    jz finished

    mov ah, 0x0E
    int 0x10
    jmp print_msg

finished:
    cli
    hlt
    
message:
    db 'hello world!', 0x0D, 0x0A, 0

times 510 - ($ - $$) db 0
dw 0xAA55
