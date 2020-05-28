/* memory.x -- Memory definition for a bootstrap program
   Copyright 2000, 2003 Free Software Foundation, Inc.
   Written by Stephane Carrez (stcarrez@nerim.fr)

This file is part of GTAM.

GTAM is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GTAM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GTAM; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* Defines the memory layout for a bootstrap program.
   Pretend there is no data section.  Everything is for the text.
   The page0 overlaps the text and we have to take care of that
   in the program (this is volunteered).  */

/*OUTPUT_FORMAT("elf32-m68hc11", "elf32-m68hc11",
            "elf32-m68hc11")
OUTPUT_ARCH(m68hc11)
ENTRY(_start)
SEARCH_DIR(C:\usr\lib\gcc-lib\m6811-elf\3.0.4\mshort)

MEMORY
{
  page0	(rwx)	: org = 0x0000, l = 1024 	// 0x0400
  ioports (!x)  : org = 0x1000, l = 0x400
  eeprom  (!i)  : org = 0xB600, l = 0x200
  data    (rwx) : org = 0x8800, l = 0x800
  text    (rx)  : org = 0x8000, l = 0x6000
}
PROVIDE (_stack = 0x3ff);
*/

MEMORY
{
  page0 (rwx) : ORIGIN = 0x00f0, LENGTH = 0x400-0xf0 /*15*/
  ioports (!x): ORIGIN = 0x1000, LENGTH = 0x400
  text  (rx)  : ORIGIN = 0x8000, LENGTH = 0x8000
  data        : ORIGIN = 0x0000, LENGTH = 1024
  eeprom (rwx): ORIGIN = 0xb600, LENGTH = 512    /* EEPROM 512 for 68hc11f1 */
}
/* Setup the stack on the top of the data internal ram (not used).*/
PROVIDE (_stack = 0x0400-1);
PROVIDE (_io_ports = 0x1000);
