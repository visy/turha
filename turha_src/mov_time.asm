.486p
.model flat

_TEXT segment use32 dword public 'code'
assume cs:_TEXT, ds:_DATA

        public  mov_timer_init
        public  mov_timer_elapsed
        public  mov_timer

start   dd      17      dup      (0)
stop    dd      17      dup      (0)
start2  dd      0
stop2   dd      0


mov_timer_init proc    near
arg     system : dword
        push    ebp
        mov     ebp,    esp

        cmp     system, 0
        jne     mov_tim3
        mov     cx,     16
        mov     edi,    offset start
        mov     esi,    offset stop
        xor     ebx,    ebx
mov_tim1:
        mov     dword ptr edi[ebx],  0
        mov     dword ptr esi[ebx],  0
        add     ebx,    4
        dec     cx
        jnz     mov_tim1

        mov     cx,     16
        mov     edi,    offset start
        mov     eax,    0:[46ch]
        xor     ebx,    ebx
mov_tim2:
        mov     edi[ebx],  eax
        add     ebx,    4
        dec     cx
        jnz     mov_tim2
        jmp     mov_timend

mov_tim3:
        mov     eax,    system
        mov     edx,    4
        mul     dx
        mov     edi,    offset start
        add     edi,    eax
        mov     eax,    0:[46ch]
        mov     edi[0],  eax

mov_timend:

        mov     esp,    ebp
        pop     ebp
        ret
mov_timer_init endp

mov_timer_elapsed proc  near
arg     system1: dword
        push    ebp
        mov     ebp,    esp

        mov     eax,    system1
        mov     edx,    4
        mul     dx
        mov     esi,    offset start
        add     esi,    eax
        mov     eax,    esi[0]
        mov     start2, eax

        mov     eax,    0:[46ch]
        mov     stop2,  eax
        sub     eax,    start2

        mov     esp,    ebp
        pop     ebp
        ret
mov_timer_elapsed endp

mov_timer       proc    near
        mov     eax,    0:[46ch]
        ret
mov_timer       endp

_text ends
END
