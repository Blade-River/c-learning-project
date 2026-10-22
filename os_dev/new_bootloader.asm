BITS 16
ORG 0x7C00

start:
  jmp main

main:
  mov ax, 0
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 0x7C00

  mov si,msg
  call put
  cli

halt:
  hlt 
  jmp halt

put:
  push si
  push ax

loop:
  lodsb
  cmp al,0
  jz end

  mov ah, 0x0E
  int 0x10

  jmp loop
  
end:
  pop ax
  pop si
  ret

msg db "hello world", 0x0D, 0x0A, 0

times 510 - ($ - $$) db 0
dw 0xAA55
