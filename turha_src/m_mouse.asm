.586p
.model flat

_TEXT segment use32 dword public 'code'
assume cs:_TEXT, ds:_DATA

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;
;;      Public declarations !!
;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


                        public  check_mouse

                        public  show_mouse_cursor

                        public  hide_mouse_cursor

                        public  get_mouse_pos_and_but

                        public  set_mouse_cursor_posi

                        public  set_mouse_hor_minmax

                        public  set_mouse_ver_minmax

                        public  set_mouse_txt_cursor

                        public  set_mouse_sensitivity

                        public  get_mouse_sensitivity

                        public  set_mouse_pixel_ratio

                        public  get_driver_version

                        public  get_backup_piece

                        public  set_backup_piece

                        public  set_screen_sizes

                        public  set_cursor_x_y_address

                        public  draw_cursor1


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;
;;      Routines:
;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;variables

x_size_of_screen        dd      0
y_size_of_screen        dd      0
maski_col               db      0
cursor1                 dd      0
cursor1_size            dd      0
cursor1_x               dd      0
cursor1_y               dd      0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;
;;      check_mouse - out 0 if not found.
;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


display "check_mouse"
check_mouse             proc    near

        xor     eax,    eax
        xor     ebx,    ebx

        mov     ax,     00
        int     33h

        cmp     ax,     0                       ; 0 - not found, ffff found!
        je      check_mouse_end

        mov     eax,     ebx

check_mouse_end:
        ret
check_mouse             endp




show_mouse_cursor       proc    near

        mov     ax,     01
        int     33h
        ret
show_mouse_cursor       endp




hide_mouse_cursor       proc    near

        mov     ax,     02
        int     33h
        ret
hide_mouse_cursor       endp





get_mouse_pos_and_but   proc    near
arg     mouse_x : dword, mouse_y : dword

        push    ebp
        mov     ebp,    esp

        mov     ax,     03
        int     33h

        mov     ax,     bx

        mov     esi,    mouse_x                 ; (0..639)
        mov     [esi],  cx

        mov     esi,    mouse_y                 ; (0..199)
        mov     [esi],  dx

        mov     esp,    ebp
        pop     ebp

        ret
get_mouse_pos_and_but   endp



set_mouse_cursor_posi   proc    near
arg     cur_x : word, cur_y : word

        push    ebp
        mov     ebp,    esp

        mov     cx,     cur_x
        mov     dx,     cur_y

        mov     ax,     4
        int     33h

        mov     esp,    ebp
        pop     ebp

        ret
set_mouse_cursor_posi   endp




set_mouse_hor_minmax    proc    near
arg     h_max : word, h_min : word

        push    ebp
        mov     ebp,    esp

        mov     cx,     h_min
        mov     dx,     h_max

        mov     ax,     07
        int     33h

        mov     esp,    ebp
        pop     ebp

        ret
set_mouse_hor_minmax    endp




set_mouse_ver_minmax    proc    near
arg     v_max : word, v_min : word

        push    ebp
        mov     ebp,    esp

        mov     cx,     v_min
        mov     dx,     v_max

        mov     ax,     08
        int     33h

        mov     esp,    ebp
        pop     ebp

        ret
set_mouse_ver_minmax    endp



set_mouse_gfx_cursor    proc    near
arg     hor_hotspot : word, ver_hotspot : word, pointer_to_16_byte_bitmap : dword
        push    ebp
        mov     ebp,    esp

        mov     bx,     hor_hotspot
        mov     cx,     ver_hotspot

        mov     edx,    pointer_to_16_byte_bitmap
        mov     ax,     dx
        shr     edx,    4
        and     ax,     0fh
        mov     es,     dx
        mov     dx,     ax

        mov     ax,     09
        int     33h

        mov     esp,    ebp
        pop     ebp
        ret
set_mouse_gfx_cursor    endp




set_mouse_txt_cursor    proc    near
arg     what_type_of_cursor : word, start_hardware : word, end_hardware : word
        push    ebp
        mov     ebp,    esp

        mov     bx,     what_type_of_cursor
        mov     cx,     start_hardware
        mov     dx,     end_hardware

        mov     ax,     0ah
        int     33h

        mov     esp,    ebp
        pop     ebp
        ret
set_mouse_txt_cursor    endp




set_mouse_sensitivity   proc    near
arg     s_hor : word, s_ver : word, sdouble : word
        push    ebp
        mov     ebp,    esp

        mov     bx,     s_hor
        mov     cx,     s_ver
        mov     dx,     sdouble

        mov     ax,     1ah
        int     33h
        

        mov     esp,    ebp
        pop     ebp
        ret
set_mouse_sensitivity   endp




get_mouse_sensitivity   proc    near
arg     g_hor : dword, g_ver : dword, gdouble : dword
        push    ebp
        mov     ebp,    esp

        mov     ax,     1bh
        int     33h
        
        mov     esi,    g_hor
        mov     [esi],  bx

        mov     esi,    g_ver
        mov     [esi],  cx

        mov     esi,    gdouble
        mov     [esi],  dx

        mov     esp,    ebp
        pop     ebp
        ret
get_mouse_sensitivity   endp




set_mouse_pixel_ratio   proc    near
arg     pix_hor : word, pix_ver : word

        push    ebp
        mov     ebp,    esp

        mov     cx,     pix_hor         ; default 8  .. 1-32767
        mov     dx,     pix_ver         ; default 16 .. 1-32767

        mov     ax,     0fh
        int     33h

        mov     esp,    ebp
        pop     ebp

        ret
set_mouse_pixel_ratio   endp



get_driver_version      proc    near
arg     ver1 : dword, ver2 : dword, mtype : dword, mirq : dword

        push    ebp
        mov     ebp,    esp

        mov     ax,     24h
        int     10h

        mov     esi,    ver1
        mov     [esi],  bh

        mov     esi,    ver2
        mov     [esi],  bl

        mov     esi,    mtype
        mov     [esi],  ch

        mov     esi,    mirq
        mov     [esi],  cl

        mov     esp,    ebp
        pop     ebp

        ret
get_driver_version      endp



get_backup_piece        proc    near
arg     backup_dest : dword, backup_sour : dword, bkp_x : dword, bkp_y : dword
        push    ebp
        mov     ebp,    esp

        mov     edi,    backup_dest
        mov     esi,    backup_sour

        mov     eax,    bkp_x
        mov     edx,    bkp_y
        mul     dx

        xor     edx,    edx

        mov     ecx,    eax

get_backup_piece_loop1:
        mov     al,    [esi]

;        cmp     al,    maski_col
;        je      get_backup_piece_mask

        mov     byte ptr [edi], al

;get_backup_piece_mask:
        inc     edi
        inc     esi

        inc     edx
        cmp     edx,    bkp_x
        jne     get_backup_piece_not_enough

        add     esi,    x_size_of_screen
        xor     edx,    edx
        sub     esi,    bkp_x

get_backup_piece_not_enough:
        dec     ecx
        jz      get_backup_piece_end

        jmp     get_backup_piece_loop1

get_backup_piece_end:

        mov     esp,    ebp
        pop     ebp
        ret
get_backup_piece        endp




set_backup_piece        proc    near
arg     sbackup_dest : dword, sbackup_sour : dword, sbkp_x : dword, sbkp_y : dword
        push    ebp
        mov     ebp,    esp

        mov     esi,    sbackup_dest
        mov     edi,    sbackup_sour

        mov     eax,    sbkp_x
        mov     edx,    sbkp_y
        mul     dx

        xor     edx,    edx

        mov     ecx,    eax

set_backup_piece_loop1:

        mov     al,     [esi]

;        cmp     al,     0
;        je      set_backup_piece_mask
        mov     byte ptr [edi],  al

;set_backup_piece_mask:
        inc     esi
        inc     edi

        inc     edx
        cmp     edx,    sbkp_x
        jne     set_backup_piece_not_enough

        add     edi,    x_size_of_screen
        xor     edx,    edx
        sub     edi,    sbkp_x

set_backup_piece_not_enough:
        dec     ecx
        jz      set_backup_piece_end

        jmp     set_backup_piece_loop1

set_backup_piece_end:

        mov     esp,    ebp
        pop     ebp
        ret
set_backup_piece        endp





set_screen_sizes        proc    near
arg     scr_x : dword, scr_y : dword, mask_color : byte

        push    ebp
        mov     ebp,    esp

        mov     eax,    scr_x
        mov     ebx,    scr_y
        mov     x_size_of_screen, eax
        mov     y_size_of_screen, ebx
        mov     al,     mask_color
        mov     maski_col, al

        mov     esp,    ebp
        pop     ebp
        ret

set_screen_sizes        endp



set_cursor_x_y_address  proc    near
arg     s_destination : dword, s_source : dword, curx_ : dword, cury_ : dword, cursx_ : dword, cursy_ : dword

        push    ebp
        mov     ebp,    esp

        mov     edi,    s_destination
        mov     cursor1, edi                    ; tee lis„yksi„ joskus.
        mov     esi,    s_source

        mov     eax,    cury_
        mov     edx,    curx_

        mov     cursor1_y, eax
        mov     cursor1_x, edx

        mul     edx


        mov     ecx,    eax
        mov     cursor1_size, ecx

        xor     edx,    edx

set_cursor_x_y_address_loop1:
        mov     al,     [esi]
        mov     byte ptr [edi],  al

        inc     edi
        inc     esi

        inc     edx
        cmp     edx,    curx_
        jne     set_cursor_x_y_address_not_enough

        add     esi,    cursx_
        xor     edx,    edx
        sub     esi,    curx_

set_cursor_x_y_address_not_enough:
        dec     ecx
        jz      set_cursor_x_y_address_end

        jmp     set_cursor_x_y_address_loop1

set_cursor_x_y_address_end:
        mov     esp,    ebp
        pop     ebp
        ret

set_cursor_x_y_address  endp




draw_cursor1            proc    near
arg     draw_desti : dword, draw_x2 : word, draw_y2 : word

        cld

        push    ebp
        mov     ebp,    esp

        mov     edi,    draw_desti
        mov     esi,    cursor1

        xor     eax,    eax
        xor     edx,    edx
        xor     ebx,    ebx
        mov     bx,     draw_x2

        mov     ax,     draw_y2
        mov     edx,    x_size_of_screen
        mul     edx
        add     edi,    eax
        add     edi,    ebx

        xor     edx,    edx

        mov     ecx,    cursor1_size

draw_cursor1_loop1:

        mov     al,     [esi]
        cmp     al,     maski_col
        je      draw_cursor1_not_now
        mov     [edi],         al

draw_cursor1_not_now:
        inc     esi
        inc     edi

        inc     edx
        cmp     edx,    cursor1_x
        jne     draw_cursor1_not_enough

        xor     edx,    edx
        add     edi,    x_size_of_screen
        sub     edi,    cursor1_x

draw_cursor1_not_enough:
        dec     cx
        jz      draw_cursor1_end

        jmp     draw_cursor1_loop1

draw_cursor1_end:
        mov     esp,    ebp
        pop     ebp
        ret
draw_cursor1            endp


_TEXT ENDS
end
