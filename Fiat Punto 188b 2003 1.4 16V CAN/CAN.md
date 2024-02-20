Fiat Punto 188(b) 2003 1.4 16V

0x180 - Lights and convinience

length 6

byte 0 -

byte 1
    bit 0 - 
    bit 1 - rear fog light indicator
    bit 2 - front fog light indicator
    bit 3 - front beam lights height regulation
    bit 4 - high beam lights indicator
    bit 5,6 - position lights indicator
    bit 7 - 

byte 2
    bit 0 - left turning lights bulb malfunction
    bit 1 - right turning lights bulb malfunction
    bit 2 - 
    bit 3 - 
    bit 4 - heated rear window indicator
    bit 5 - right turning lights indicator
    bit 6 - left turning lights indicator
    bit 7 - 

byte 3
    bit 0 - 
    bit 1 - ? light bulb malfunction
    bit 2 - ? light bulb malfunction
    bit 3 - ? light bulb malfunction
    bit 4 - ? light bulb malfunction
    bit 5 - ? light bulb malfunction
    bit 6 - 
    bit 7 - 

byte 4
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - ? light bulb malfunction
    bit 4 - 
    bit 5 - 
    bit 6 - 
    bit 7 - 


byte 5 -


=============================================================================
0x282 - Power steering wheel

length 8

byte 0 -

byte 1 -

byte 2 -

byte 3 -

byte 4 -

byte 5 -

byte 6 -

byte 7 -


=============================================================================
0x281 - Misc.

length 8

byte 0 -

byte 1
    bit 0 - 
    bit 1 - cruise control
    bit 2 - 
    bit 3 - 
    bit 4 - 
    bit 5 - 
    bit 6 - 
    bit 7 - low oil pressure

byte 2
    bit 0 - high temperature and red light
    bit 1 - 
    bit 2 - 
    bit 3 - check engine indicator (blinking)
    bit 4 - check engine indicator
    bit 5 - 
    bit 6 - 
    bit 7 - 

byte 3 - engine coolant temperature

byte 4 -

byte 5 -

byte 6 - tachometer (rpm)

byte 7
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - 
    bit 4 - 
    bit 5 - disable tachometer (rpm)
    bit 6 - 
    bit 7 - 


=============================================================================
0x2A0 - Vehicle speed

length 4

byte 0 - speed high byte

byte 1 - speed low byte

byte 2 - 00 always

byte 3 - CA always (changing this value will update the odometer)


=============================================================================
0x380 - Vehicle electrics

length 8

byte 0
    bit 0 - 
    bit 1 -
    bit 2 - blinking display
    bit 3 - instrument backlight
    bit 4 - 
    bit 5 - parking brake indicator
    bit 6 - parking brake indicator
    bit 7 - 

byte 1
    bit 0 - 
    bit 1 -
    bit 2 - left, front door open
    bit 3 - right, front door open
    bit 4 - left, rear door open
    bit 5 - right, rear door open
    bit 6 - trunk door open
    bit 7 - door open (indicator only, without text on display)

byte 2
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - alternator light
    bit 4 - 
    bit 5 - 
    bit 6 - ignition on
    bit 7 - ignition on and instrument backlight on (force)

byte 3
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - 
    bit 4 - 
    bit 5 - 
    bit 6 - 
    bit 7 - FPS on

byte 4
    bit 0 - no fuel or fuel sensor malfunction (low fuel indicator blinking, gauge doesn't show fuel level)
    bit 1 - low fuel indicator
    bit 2 - 
    bit 3 - 
    bit 4 - 
    bit 5 - 
    bit 6 - 
    bit 7 -

byte 5 - fuel level in percentage (00 - empty, 64 - full)

byte 6
    bit 0 - fiat code indicator (blinking)
    bit 1 - fiat code indicator (shining)
    bit 2 - fiat code indicator (shining)
    bit 3 - fiat code indicator (shining)
    bit 4 - fiat code indicator
    bit 5 - 
    bit 6 - 
    bit 7 -

byte 7 - 


=============================================================================
0x382 Steering and City

length 1

byte 0
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - 
    bit 4 - 
    bit 5 - city indicator
    bit 6 - 
    bit 7 - steering indicator


=============================================================================
0x286 ABS

length 2

byte 0 -

byte 1
    bit 0 - 
    bit 1 - 
    bit 2 - 
    bit 3 - 
    bit 4 - ABS and brake indicator
    bit 5 - ABS indicator
    bit 6 - 
    bit 7 -