=============================================================================
0x180 - Lights and convinience

byte 0 -

byte 1 - x2 - rear fog light indicator

x8 - front beam lights height regualation

xA - front beam lights height regualation + rear fog light indicator

1x - high beam lighs indicator

6x - position lights indicator

7x - position + high beam lights indicators

byte 2 - 0x - nothing

1x - heated rear window indicator

2x - right turning lights indicator

3x - right turning lights + heated rear window indicators

4x - left turning lights indicator

5x - left turning lights + heated rear window indicators

6x - emergency lights (left + right turning lights indicators)

7x - emergency lights + heated rear window indicators

x0 - nothing

x1 - left turning lights bulb malfunction

x2 - right turning lights bulb malfunction

x3 - left + right turning lights bulbs malfunction

byte 3 - x1 - rear fog light bulb malfunction

1x - brake lights bulb malfunction

byte 4 - x9 - registration plate bulb malfunction

1x - left position light bulb malfunction (information on display, no indicator)

2x - right position light bulb malfunction (information on display, no indicator)

3x - left + right position light bulb malfunction (information on display, no indicator)

4x - left position light bulb malfunction (information on display, indicator on)

6x - left + right position light bulb malfunction (information on display, indicator on)

8x - right position light bulb malfunction (information on display, indicator on)

byte 5 -


=================================================================================================
0x282 - Power steering wheel

byte 0 -

byte 1 -

byte 2 -

byte 3 -

byte 4 -

byte 5 -

byte 6 -

byte 7 -


=============================================================================
0x281 - 

byte 0 -

byte 1 - 1x - glow plugs preheating indicator (diesel)

2x - clogged diesel fuel filter alarm

3x - glow plugs preheating indicator (diesel) + clogged diesel fuel filter alarm

4x - general malfunction (yellow exclamation mark in triangle - no alarm beep)

5x - glow plugs preheating indicator (diesel) + general malfunction

8x - low oil pressure alarm

byte 2 - 8x - engine running normal

9x - check engine indicator

byte 3 - engine coolan temperature

byte 4 - xA - glow plugs malfunction indicator (diesel)

byte 5 -

byte 6 - tachometer (rpm)

byte 7 - 0xAA DPF cleaning information + engine oil change alarm


=============================================================================
0x2A0 - Vehicle speed (works only together with rpm > 0 in msg 0x281)

byte 0 - speed high byte

byte 1 - speed low byte

byte 2 - 00 always

byte 3 - CA always


=============================================================================
0x380 - Vehicle electrics

byte 0 - 2x - parking brake indicator on

0x - parking brake indicator off

x1 - instruments backlight off

x9 - instruments backlight on

byte 1 - x4 - left, front door open

x8 - right, front door open

1x - left, rear door open

2x - right, rear door open

4x - trunk door open

8x - door open (indicator only, without text on display)

byte 2 - 0xC8 - alternator malfunction/not charging

0xC0 - alternator normal running

byte 3 -

byte 4 - x1 - no fuel or fuel sensor malfunction (low fuel indicator blinking, gauge doesn't show fuel level)

x2 - low fuel indicator, 

byte 5 - fuel level in percentage (00 - empty, 64 - full)

byte 6 - x1 - fiat code indicator (blinking)

x2 - fiat code indicator (shining)

byte 7 - x0 - city indicator off

x1 - city indicator on


=============================================================================
0x545 - rds/radio

byte 0 - 60 no card MP3

80 tape pause

C0 CD MP3 pause

byte 4 - 02 - normal, 03 - "Search",

0x545,0x43,0xEA,0x5F,0x00,0x00,0x00 - 999.9 MHz

0x565 - rds/radio - the eight characters are encoded in six bytes. I've even made a map for each letter and it's... somewhere.

0x565,0x02,0x24,0x17,0x39,0xA6,0x10,0x00 - " WELCOME"


=============================================================================