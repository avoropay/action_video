#include "systeminfo.h"

#define  RAMEND_LIMIT  2000
//************************************************************************

extern UTFT myGLCD;
extern uint8_t SmallFont[];

#ifdef _AVR_CPU_NAME_
const char  gTextMsg_CPU_Name[]      = _AVR_CPU_NAME_;
#else
const char  gTextMsg_CPU_Name[]      = "UNKNOWN";
#endif

const char  gTextMsg_COMPILED_ON[]     = "Compiled on = ";
const char  gTextMsg_CPU_Type[]      = "CPU Type    = ";
const char  gTextMsg_AVR_ARCH[]      = "__AVR_ARCH__= ";
const char  gTextMsg_AVR_LIBC[]      = "AVR LibC Ver= ";
const char  gTextMsg_GCC_VERSION[]     = "GCC Version = ";
const char  gTextMsg_CPU_SIGNATURE[]   = "CPU ID      = ";
const char  gTextMsg_FUSE_BYTE_LOW[]   = "Low fuse    = ";
const char  gTextMsg_FUSE_BYTE_HIGH[]  = "High fuse   = ";
const char  gTextMsg_FUSE_BYTE_EXT[]   = "Ext fuse    = ";
const char  gTextMsg_FUSE_BYTE_LOCK[]  = "Lock fuse   = ";
const char  gTextMsg_GCC_DATE_STR[]    = __DATE__;
const char  gTextMsg_AVR_LIBC_VER_STR[]  = __AVR_LIBC_VERSION_STRING__;
const char  gTextMsg_GCC_VERSION_STR[]   = __VERSION__;
const char  gTextMsg_VECTOR_HEADER[]   = "V#   ADDR   op code     instruction addr   Interrupt";
const char  gTextMsg_noVector[]      = "no vector";
const char  gTextMsg_rjmp[]        = "rjmp  ";
const char  gTextMsg_jmp[]         = "jmp ";
const char  gTextMsg_WHAT_PORT[]     = "What port:";
const char  gTextMsg_PortNotSupported[]  = "Port not supported";
const char  gTextMsg_MustBeLetter[]    = "Must be a letter";
const char  gTextMsg_SPACE[]       = " ";
const char  gTextMsg_WriteToEEprom[]   = "Writting EE";
const char  gTextMsg_ReadingEEprom[]   = "Reading EE";
const char  gTextMsg_EEPROMerrorCnt[]  = "EE err cnt=";
const char  gTextMsg_PORT[]        = "PORT";

int  PrintDecInt(int theNumber, int digitCnt)
{
  int theChar;
  int myNumber;

  myNumber  = theNumber;

  if ((myNumber > 100) || (digitCnt >= 3))
  {
    theChar   = 0x30 + myNumber / 100;
    return theChar;
  }

  if ((myNumber > 10) || (digitCnt >= 2))
  {
    theChar = 0x30  + ((myNumber % 100) / 10 );
    return theChar;
  }
  theChar = 0x30 + (myNumber % 10);
  return theChar;
}

void  Ram_TableDisplay(void)
{
  char stack = 1;
  extern char *__data_start;
  extern char *__data_end;

  extern char *__bss_start;
  extern char *__bss_end;
  //extern char *__brkval;
  extern char *__heap_start;
  extern char *__heap_end;
  //extern char *__malloc_heap_end;
  //extern size_t __malloc_margin;


  int data_size = (int)&__data_end - (int)&__data_start;
  int bss_size  = (int)&__bss_end - (int)&__data_end;
  int heap_end  = (int)&stack - (int)&__malloc_margin;
  //  int heap_size = (int)__brkval - (int)&__bss_end;
  int heap_size = heap_end - (int)&__bss_end;
  int stack_size  = RAMEND - (int)&stack + 1;
  int available = (RAMEND - (int)&__data_start + 1);

  available -=  data_size + bss_size + heap_size + stack_size;

  Serial.print("+----------------+  __data_start  =");  Serial.println((int)&__data_start);
  Serial.print("+      data      +");           Serial.println();
  Serial.print("+    variables   +  data_size     =");  Serial.println(data_size);
  Serial.print("+                +");           Serial.println();
  Serial.print("+----------------+  __data_end    =");  Serial.println((int)&__data_end);
  Serial.print("+----------------+  __bss_start   =");  Serial.println((int)&__bss_start);
  Serial.print("+       bss      +");           Serial.println();
  Serial.print("+    variables   +  bss_size      =");  Serial.println(bss_size);
  Serial.print("+                +");           Serial.println();
  Serial.print("+----------------+  __bss_end     =");  Serial.println((int)&__bss_end);
  Serial.print("+----------------+  __heap_start  =");  Serial.println((int)&__heap_start);
  Serial.print("+                +");           Serial.println();
  Serial.print("+       heap     +  heap_size     =");  Serial.println(heap_size);
  Serial.print("+                +");           Serial.println();
  Serial.print("+----------------+  heap_end      =");  Serial.println(heap_end);
  Serial.print("+----------------+  Current STACK =");  Serial.println((int)&stack);
  Serial.print("+                +");           Serial.println();
  Serial.print("+      stack     +  stack_size    =");  Serial.println(stack_size);
  Serial.print("+                +");           Serial.println();
  Serial.print("+----------------+  RAMEND        =");  Serial.println(RAMEND);

  //  Serial.print("__brkval      =");
  //  Serial.println((int)__brkval);

  Serial.print("available =");
  Serial.println(available);

  Serial.println();
  Serial.println();
}

void PrintCPUstats()
{
  unsigned char fuseByte;
  unsigned long memorySize;
  unsigned long upSeconds;
  int shiftY, numRow = 0;
  char textString[50];
  uint8_t cFontYsize;

  myGLCD.setFont(SmallFont);
  cFontYsize = myGLCD.getFontYsize();

  Serial.print(gTextMsg_COMPILED_ON);
  Serial.println(gTextMsg_GCC_DATE_STR);
  myGLCD.print(gTextMsg_COMPILED_ON, LEFT, shiftY);
  myGLCD.print(gTextMsg_GCC_DATE_STR, RIGHT, shiftY);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  
  Serial.print(gTextMsg_CPU_Type);
  Serial.println(gTextMsg_CPU_Name);

  myGLCD.print(gTextMsg_CPU_Type, LEFT, shiftY);
  myGLCD.print(gTextMsg_CPU_Name, RIGHT, shiftY);


  Serial.print(gTextMsg_AVR_ARCH);
  Serial.println(PrintDecInt(__AVR_ARCH__, 1));

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(gTextMsg_AVR_ARCH, LEFT, shiftY);
  myGLCD.printNumI(PrintDecInt(__AVR_ARCH__, 1), RIGHT, shiftY);


  Serial.print(gTextMsg_GCC_VERSION);
  Serial.println(gTextMsg_GCC_VERSION_STR);
  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(gTextMsg_GCC_VERSION, LEFT, shiftY);
  myGLCD.print(gTextMsg_GCC_VERSION_STR, RIGHT, shiftY);

  //* these can be found in avr/version.h
  Serial.print(gTextMsg_AVR_LIBC);
  Serial.println(gTextMsg_AVR_LIBC_VER_STR);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(gTextMsg_AVR_LIBC, LEFT, shiftY);
  myGLCD.print(gTextMsg_AVR_LIBC_VER_STR, RIGHT, shiftY);

#if defined(SIGNATURE_0)
  Serial.print(gTextMsg_CPU_SIGNATURE);
  //* these can be found in avr/iomxxx.h
  Serial.print(SIGNATURE_0, HEX);
  Serial.print(SIGNATURE_1, HEX);
  Serial.print(SIGNATURE_2, HEX);
  Serial.println();
  
  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(gTextMsg_CPU_SIGNATURE, LEFT, shiftY);
  sprintf(textString, "%02X%02X%02X", SIGNATURE_0, SIGNATURE_1, SIGNATURE_2);
  myGLCD.print(textString, RIGHT, shiftY);
  
#endif

#if !defined(__MWERKS__) && defined(GET_LOW_FUSE_BITS) &&  (RAMEND > RAMEND_LIMIT)
  //* fuse settings
  fuseByte  = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
  sprintf(textString, "Low fuse byte = %02X", fuseByte);
  Serial.println(textString);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(textString, LEFT, shiftY);


  fuseByte  = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
  sprintf(textString, "High fuse byte= %02X", fuseByte);
  Serial.println(textString);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(textString, LEFT, shiftY);

  fuseByte  = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
  sprintf(textString, "Ext  fuse byte= %02X", fuseByte);
  Serial.println(textString);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(textString, LEFT, shiftY);


  fuseByte  = boot_lock_fuse_bits_get(GET_LOCK_BITS);
  sprintf(textString, "Lock fuse byte= %02X", fuseByte);
  Serial.println(textString);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print(textString, LEFT, shiftY);

#endif


#if defined(RAMEND) && (RAMEND > 1000)
  //* these can be found in avr/iomxxx.h
  memorySize  = RAMEND - RAMSTART + 1;
  Serial.print("RAM size: ");
  Serial.println(memorySize / 1024);
  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print("RAM size: ", LEFT, shiftY);
  myGLCD.printNumI(memorySize / 1024, RIGHT, shiftY);

#endif


#if defined(FLASHEND) && (RAMEND > 1000)
  memorySize  = (long)FLASHEND;
  memorySize  +=  1;
  Serial.print("FLASH size: ");
  Serial.println(memorySize / 1024);
  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print("FLASH size: ", LEFT, shiftY);
  myGLCD.printNumI(memorySize / 1024, RIGHT, shiftY);
#endif

#if defined(E2END) && (RAMEND > 1000)
  Serial.print("eeprom: ");
  Serial.println(E2END + 1);
  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print("eeprom size: ", LEFT, shiftY);
  myGLCD.printNumI(E2END + 1 / 1024, RIGHT, shiftY);
#endif

#ifdef _ENABLE_XTENDED_MEMORY_
  XtendedMemoryCheck(true); //* true means print results
#endif

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  myGLCD.print("Free size: ", LEFT, shiftY);
  myGLCD.printNumI(freeMemory(), RIGHT, shiftY);

  upSeconds = millis() / 1000;
  //  Serial.println(upSeconds);
  sprintf(textString, "%02ld:%02ld:%02ld", (upSeconds / 3600), ((upSeconds / 60) % 60), (upSeconds % 60));
  Serial.println(textString);

  numRow++;
  shiftY = cFontYsize*numRow + 1;
  //myGLCD.print(textString, LEFT, shiftY);

  Serial.println();

  Ram_TableDisplay();
}

