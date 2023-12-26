#include "string.h"
#include "CP1252ToUTF8.h"
#include "MemoryAllocator.h"

const char* cp1252UTF8[128]
      = { u8"€", u8" ", u8"‚", u8"ƒ", u8"„", u8"…", u8"†", u8"‡", u8"ˆ", u8"‰", u8"Š", u8"‹", u8"Œ", u8" ", u8"Ž", u8" ",
      	  u8" ", u8"‘", u8"’", u8"“", u8"”", u8"•", u8"–", u8"—", u8"˜", u8"™", u8"š", u8"›", u8"œ", u8" ", u8"ž", u8"Ÿ",
          u8" ", u8"¡", u8"¢", u8"£", u8"¤", u8"¥", u8"¦", u8"§", u8"¨", u8"©", u8"ª", u8"«", u8"¬", u8" ", u8"®", u8"¯",
          u8"°", u8"±", u8"²", u8"³", u8"´", u8"µ", u8"¶", u8"·", u8"¸", u8"¹", u8"º", u8"»", u8"¼", u8"½", u8"¾", u8"¿",
          u8"À", u8"Á", u8"Â", u8"Ã", u8"Ä", u8"Å", u8"Æ", u8"Ç", u8"È", u8"É", u8"Ê", u8"Ë", u8"Ì", u8"Í", u8"Î", u8"Ï",
          u8"Ð", u8"Ñ", u8"Ò", u8"Ó", u8"Ô", u8"Õ", u8"Ö", u8"×", u8"Ø", u8"Ù", u8"Ú", u8"Û", u8"Ü", u8"Ý", u8"Þ", u8"ß",
          u8"à", u8"á", u8"â", u8"ã", u8"ä", u8"å", u8"æ", u8"ç", u8"è", u8"é", u8"ê", u8"ë", u8"ì", u8"í", u8"î", u8"ï",
          u8"ð", u8"ñ", u8"ò", u8"ó", u8"ô", u8"õ", u8"ö", u8"÷", u8"ø", u8"ù", u8"ú", u8"û", u8"ü", u8"ý", u8"þ", u8"ÿ" };

const char* convert1252ToUTF8(const char* c1252)
{
    int bufferlength = 0; 
    int i = 0;
    bool replacementNeeded = false;
    while(true)
    {
        char c = c1252[i++];
        if (c >= 128)
        {
            bufferlength += strlen(cp1252UTF8[c - 128]);
            replacementNeeded = true;
        }
        else
        {
            bufferlength++;
            if (c == 0)
                break;
        }
    }
    if (!replacementNeeded)
        return c1252;
    char* cUtf8 = (char*) HS_MALLOC(bufferlength);
    size_t bufferIndex = 0;
    i = 0;
    while(true)
    {
        char c = c1252[i++];
        if (c >= 128)
        {
            const char* utf8 = cp1252UTF8[c - 128];
            size_t utf8len = strlen(utf8);
            memcpy(cUtf8 + bufferIndex, utf8, utf8len);
            bufferIndex += utf8len;
        }
        else
        {
            cUtf8[bufferIndex++] = c; 
            if (c == 0)
                break;
        }
    }
    return cUtf8;
}
