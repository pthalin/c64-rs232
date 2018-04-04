#include <stdio.h>
#include <cbm.h>

void petscii_to_ascii (char *str);

char rs232_read_buf[512];
char rs232_write_buf[512];

char** RIBUF = (char**)0x00f7;
char** ROBUF = (char**)0x00f9;

//bypass cc65 Charter translation
static unsigned char us_name1200[] = {0x08, 0x00, 0x00}; //1200 8N1

char us_buffer[256];

void us_putc(char c)
{
  cbm_k_ckout(2);
  cbm_k_bsout(c);
}

char us_getc() {
  cbm_k_chkin(2);
  return cbm_k_getin();
}

void us_read(char* str, unsigned char len)
{
  char c = 0;
  char k = 0;
  cbm_k_chkin(2);
  while(k <= len)
  {
    c = cbm_k_getin();
    if(c)
    {
      str[++k] = c;
    }
  }
   
  return; 
}

void us_printf(const char* format, ... )
{
  char k=0; 
  char c;
  va_list args;
  va_start (args, format);
  vsprintf (us_buffer,format, args);
  cbm_k_ckout(2);

  while(k<255) 
  {
    c = us_buffer[k];
    if (c == NULL)
      break;
    cbm_k_bsout(c);
    ++k;
  }
  va_end (args);
  cbm_k_clrch();
}

void us_aprintf(const char* format, ... )
{
  char k=0; 
  char c;
  va_list args;
  va_start (args, format);
  vsprintf (us_buffer,format, args);
  cbm_k_ckout(2);
  
  petscii_to_ascii(us_buffer);
  
  while(k<255) 
  {
    c = us_buffer[k];
    if (c == NULL)
      break;
    cbm_k_bsout(c);
    ++k;
  }
  va_end (args);
  cbm_k_clrch();
  
  return;
}

void us_init()
{
  //unsigned char __fastcall__ (*cbm_k_getin)(void) = 0xffe4;

  *RIBUF = (char*)(((int)rs232_read_buf & 0xff00) + 256);
  *ROBUF = (char*)(((int)rs232_write_buf & 0xff00) + 256);

  // open rs232 channel
  cbm_k_setlfs (2,2,3);
  cbm_k_setnam (us_name1200);
  cbm_k_open ();
  

}

void petscii_to_ascii (char *str)
{
  unsigned char pos=0;
  char code;

  while ((str[pos]) && (pos < 255))
  {
    code = str[pos];

    if ((code >= 'A') && (code <= 'Z'))
    {
      code -= 128;
    } 
    else if ((code >= 'a') && (code <= 'z')) 
    {
      code += 32;
    }
    else if (code == 0xd)
      code = 0xa;

    str[pos] = code;
    ++pos;
  }
  return;
}


void ascii_to_petscii(char *str)
{
  unsigned char pos=0;
  char code;

  while ((str[pos]) && (pos < 255))
  {
    code = str[pos];

    if ((code >= 'A') && (code <= 'Z'))
    {
      code += 128;
    } 
    else if ((code >= 'a') && (code <= 'z')) 
    {
      code -= 32;
    }
    else if (code == 0xa)
      code = 0xd;

    str[pos] = code;
    ++pos;
  }
  return;
}
