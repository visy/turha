#define m_mouse_h

/*****************************************************************************\
*                                                                             *
* <ASM-ROUTINES "MOUSE">       /nolibs.                                       *
*                                                                             *
\*****************************************************************************/

extern "C"
{

/*----------------------------------------------------------------------------
-                                                                            -
- check_mouse                   out: 0 none, muuten nappulam„„r„             -
-                                                                            -
----------------------------------------------------------------------------*/

    char    check_mouse     ();




    /*------------------------------------------------------------------------
    -                                                                        -
    - Show / Hide_mouse_cursor                                               -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    show_mouse_cursor ();

    void    hide_mouse_cursor ();



    /*------------------------------------------------------------------------
    -                                                                        -
    - get_mouse_pos_and_but   return button count. 1=left. 2=right. 3=both.  -
    -                                                                        -
    ------------------------------------------------------------------------*/

    short   get_mouse_pos_and_but ( int &mouse_x, int &mouse_y);



    /*------------------------------------------------------------------------
    -                                                                        -
    - set_mouse_cursor_posi                                                  -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_mouse_cursor_posi ( short cur_x, short cur_y );




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_mouse_hor_minmax                                                   -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_mouse_hor_minmax ( short h_max, short h_min );
    void    set_mouse_ver_minmax ( short v_max, short v_min );




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_mouse_hor_minmax                               !! VOI KUSSA VIELŽ !!
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_mouse_gfx_cursor ( short hor_hotspot, short ver_hotspot,
                                   int pointer_to_16_byte_bitmap );




    // 0 = software cursor, 1 = hardware cursor, when you have to use those
    // last two variables.

    void    set_mouse_txt_cursor ( short what_type_of_cursor,
                                   short start_hardware, short end_hardware);




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_mouse_sensitivity                                                  -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_mouse_sensitivity ( short s_hor, short s_ver, short sdouble );

    void    get_mouse_sensitivity ( int &g_hor, int &g_ver, int &gdouble );




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_mouse_pixel_ratio                                                  -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_mouse_pixel_ratio ( short pix_hor, short pix_ver );




    /*------------------------------------------------------------------------
    -                                                                        -
    - get_driver_version                                                     -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    get_driver_version ( int &ver1, int &ver2, int &mtype, int &mirq );




    /*------------------------------------------------------------------------
    -                                                                        -
    - get_backup_piece                                                       -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    get_backup_piece ( int backup_dest, int backup_sour, int bkp_x,
                               int bkp_y );

    void    set_backup_piece ( int sbackup_dest, int sbackup_sour, int sbkp_x,
                               int sbkp_y );




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_screen_sizes                                                       -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_screen_sizes ( int scr_x, int scr_y, char mask_color );




    /*------------------------------------------------------------------------
    -                                                                        -
    - set_cursor_x_y_address                                                 -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    set_cursor_x_y_address ( int s_destination, int s_source,
                                     int curx_, int cury_, int cursx_,
                                     int cursy_ );




    /*------------------------------------------------------------------------
    -                                                                        -
    - draw_cursor1                                                           -
    -                                                                        -
    ------------------------------------------------------------------------*/

    void    draw_cursor1 ( int draw_desti, short draw_x2, short draw_y2 );

}



class   mouse_
{
    public:
        mouse_ ( int cursor_x, int cursor_y, int frames, char mask, int sx, int sy );
        update ();
        info_again ();
        cursor_on ();
        cursor_off ();
        set_mouse_position ( short x, short y );
        pixel_ratio ( short hor, short ver);
        set_sensitivity ( short hor, short ver, short sgdouble );
        set_box ( short x, short y, short x2, short y2 );
        set_cursor_gfx ( int source_adr );
        set_destination_address ( int adr1 );
        set_stuff_for_backup    ( int pages );
        set_bkp_in_use ();


    int mouse_available;        // 0 = no mouse, 1 = mouse available.
    int mouse_type;             // 1 = busmouse, 2 = serial mouse,
                                // 3 = inport mouse, 4 = ps/2 mouse
                                // 5 = hewlett packard mouse.
    int mouse_irq;              // 0 = ps/2, 2 = irq 2, 5 = irq 5, 7 = irq 7.

    int version_1;
    int version_2_decimal;

    int sensitivity_hor;
    int sensitivity_ver;
    int sensitivity_double;

    int current_x, current_y;

    int button;

    char *cursor_adr;
    int address_to_cursor;
    int frames;
    char mask_colour;
    int frame_count;

    int screen_x, screen_y;
    int cursori_y, cursori_x;
    int variable_adr1;

    char * backup_adr;
    int bkp_adr1;
    int bkp_adr2;
    int bkp_in_use;
    int page__;
    int last_x0, last_x1;
    int last_y0, last_y1;

    private:
};

mouse_::mouse_ ( int cursor_x, int cursor_y, int frames, char mask, int sx, int sy )
{
    if (check_mouse() == 0)
        mouse_available = 0;
    else
        {
        get_driver_version ( version_1, version_2_decimal, mouse_type, mouse_irq );
        get_mouse_sensitivity ( sensitivity_hor, sensitivity_ver, sensitivity_double );
        button = get_mouse_pos_and_but ( current_x, current_y );
        cursor_adr = (char *) malloc ((cursor_x * cursor_y)*frames);
        mask_colour = mask;
        frame_count = 0;
        set_screen_sizes ( sx, sy, mask );
        screen_x = sx;
        screen_y = sy;
        cursori_x = cursor_x;
        cursori_y = cursor_y;
        address_to_cursor = int (cursor_adr);
        bkp_in_use = 0;
        page__ = 0;
        }
}

mouse_::update ()
{
        button = get_mouse_pos_and_but ( current_x, current_y );
        current_x = (int) (float (current_x/2));
        if (current_x > (352-cursori_x))
            current_x = (352-cursori_x);
        if (bkp_in_use == 1)
            if (page__ == 0)
            {
                set_backup_piece (bkp_adr1, variable_adr1+((last_y0*352)+last_x0), cursori_x, cursori_y);
                get_backup_piece (bkp_adr1, variable_adr1+((current_y*352)+current_x), cursori_x, cursori_y);
                last_y0=current_y;
                last_x0=current_x;
                page__=1;
            }
            else
            {
                set_backup_piece (bkp_adr2, variable_adr1+((last_y1*352)+last_x1), cursori_x, cursori_y);
                get_backup_piece (bkp_adr2, variable_adr1+((current_y*352)+current_x), cursori_x, cursori_y);
                last_y1=current_y;
                last_x1=current_x;
                page__=0;
            }

        draw_cursor1 ( variable_adr1, current_x, current_y );
}

mouse_::info_again ()
{
        get_driver_version ( version_1, version_2_decimal, mouse_type, mouse_irq );
        get_mouse_sensitivity ( sensitivity_hor, sensitivity_ver, sensitivity_double );
        button = get_mouse_pos_and_but ( current_x, current_y );
}

mouse_::pixel_ratio ( short hor, short ver )
{
    set_mouse_pixel_ratio ( hor, ver );
}

mouse_::cursor_on ()
{
    show_mouse_cursor ();
}

mouse_::cursor_off ()
{
    hide_mouse_cursor ();
}

mouse_::set_mouse_position (short x, short y)
{
    set_mouse_cursor_posi ( x, y );
}

mouse_::set_sensitivity (short hor, short ver, short sgdouble)
{
    sensitivity_hor = hor;
    sensitivity_ver = ver;
    sensitivity_double = sgdouble;
    set_mouse_sensitivity (hor, ver, sgdouble);
}

mouse_::set_box ( short x, short y, short x2, short y2 )
{
    set_mouse_hor_minmax ( x, x2 );
    set_mouse_ver_minmax ( y, y2 );
}

mouse_::set_cursor_gfx ( int source )
{
    set_cursor_x_y_address ( address_to_cursor, source, cursori_x,
                             cursori_y, screen_x, screen_y );
}

mouse_::set_destination_address ( int adr1 )
{
    variable_adr1 = adr1;
}

mouse_::set_stuff_for_backup    ( int pages )
{
    backup_adr = (char *) malloc ( (cursori_x * cursori_y) * 4 );
    bkp_adr1 = int (backup_adr);
    bkp_adr2 = int (backup_adr+(cursori_x*cursori_y));
    get_backup_piece (bkp_adr1, variable_adr1+((current_y*352)+current_x), cursori_x, cursori_y);
    get_backup_piece (bkp_adr2, variable_adr1+((current_y*352)+current_x), cursori_x, cursori_y);
    last_x0=current_x;
    last_x1=current_x;
    last_y0=current_y;
    last_y1=current_y;
}

mouse_::set_bkp_in_use ()
{
    if (bkp_in_use == 1)
        bkp_in_use = 0;
    else
        bkp_in_use = 1;
}
