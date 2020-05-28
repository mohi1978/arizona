OUTPUT_FORMAT("elf32-m68hc11", "elf32-m68hc11",
            "elf32-m68hc11")
OUTPUT_ARCH(m68hc11)
ENTRY(_start)
/*SEARCH_DIR(C:\usr\lib\gcc-lib\m6811-elf\3.0.4\mshort)*/
MEMORY
{
  page0	(rwx)	: org = 0x0000, l = 1024	/* 0x0400 */
  ioports (!x)  : org = 0x1000, l = 0x60
  eeprom  (!i)  : org = 0xB600, l = 0x200
  data    (rwx) : org = 0x8800, l = 0x800
  text    (rx)  : org = 0x8000, l = 0x6000
}
PROVIDE (_stack = 0x3ff);
