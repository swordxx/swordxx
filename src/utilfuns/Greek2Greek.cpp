//*****************************************************************************
// Author      : William Dicks                                              ***
// Date Created: 10 February 1998                                           ***
// Purpose     : Implementation for Greek to b-Greek conversion and vice    ***
//             : versa.                                                     ***
// File Name   : Greek2Greek.cpp                                            ***
//                                                                          ***
// Author info : ---------------------------------------------------------- ***
//     Address : 23 Tieroogpark                                             ***
//             : Hoewe Str                                                  ***
//             : Elarduspark X3                                             ***
//             : 0181                                                       ***
//             : South Africa                                               ***
//     Home Tel: +27 (0)12 345 3166                                         ***
//     Cell No : +27 (0)82 577 4424                                         ***
//     e-mail  : wd@isis.co.za                                              ***
// Church WWW  : http://www.hatfield.co.za                                  ***
//                                                                          ***
// Bugfix info : ---------------------------------------------------------- ***
// Bug #1      : Greek Font character 197 converted to b-Greek "6"          ***
// Date Fixed  : 23 February 1998                                           ***
//*****************************************************************************

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "Greek2Greek.h"
#include "GreekChars.h"

//*****************************************************************************
// Used to convert a string created by using the Greek font supplied with the
// Sword Project to a string that conforms to the b-Greek discussion list 
// method of transliteration.
//*****************************************************************************

SWORD_NAMESPACE_START

unsigned char Greek2bGreek(
         unsigned char *sResult, 
         unsigned char *sGreekText, 
         int nMaxResultBuflen)
{
   char error;

   unsigned int NoOfChars = ParseGreek(sResult, sGreekText, nMaxResultBuflen);

   if (NoOfChars < strlen((char *)sGreekText))
      error = 1;
   else
      error = 0;

   return error;
}

//*****************************************************************************
// Used to convert a string created by using the b-Greek method of 
// transliteration to a string that can be converted to a Greek-font readable 
// string.
//*****************************************************************************

unsigned char bGreek2Greek(
         unsigned char *sResult, 
         unsigned char *sGreekText, 
         int nMaxResultBuflen)
{
   unsigned char error;

   unsigned int NoOfChars = ParsebGreek(sResult, sGreekText, nMaxResultBuflen);

   if (NoOfChars < strlen((char *)sGreekText))
      error = 1;
   else
      error = 0;

   return error;
}

//*****************************************************************************
// Parse a Greek font created string and return the b-Greek equivalent
//*****************************************************************************

int ParseGreek(
         unsigned char *sResult, 
         unsigned char *sGreekText, 
         int nMaxResultBuflen)
{
   int characters = 0;
   int index = 0;
   unsigned char tmp;
   bool iota;        // true = IOTA subscript; false = No IOTA
   bool breathing;   // true = add breathing; false = no breathing
   bool rough;       // true = rough breathing; false = smooth

   // While text is not equal to NULL pointer

   while (sGreekText[index] && characters < nMaxResultBuflen)
   {
      iota = breathing = rough = false;
      tmp = Font2char(sGreekText[index++], iota, breathing, rough);

      if (breathing)
      {
         if (rough)  // Rough breathing
         {
            sResult[characters++] = ROUGH;      // Add rough breathing "h"
            sResult[characters++] = tmp;        // Insert char
         }
         else
            sResult[characters++] = tmp;           // Insert char
      }
      else
      {
         if (iota)   // IOTA subscript
         {
            sResult[characters++] = tmp;        // Insert char
            sResult[characters++] = IOTA_SUB;   // Add Iota subscript
         }
         else
            sResult[characters++] = tmp;           // Insert char
      }
   }
   sResult[characters] = 0;					// Terminate the string

   return index;
}

//*****************************************************************************
// Parse a b-Greek string and return the Greek font equivalent
//*****************************************************************************
int ParsebGreek(
         unsigned char *sResult, 
         unsigned char *sGreekText, 
         int nMaxResultBuflen)
{
   int characters = 0;
   int index = 0;
   bool iota = false;        // true = IOTA subscript; false = No IOTA
   bool breathing = false;   // true = add breathing; false = no breathing
   bool rough = false;       // true = rough breathing; false = smooth
   bool fSigma = false;      // Final sigma flag
   bool nChar = true;        // New char flag

   // While text is not equal to NULL pointer

   while (*sGreekText || characters < nMaxResultBuflen)
   {
      if (nChar)
      {
         if (*sGreekText == (unsigned char)ROUGH)
         {
            rough = true;
            breathing = true;
         }
         else
         {
            rough = false;
            breathing = true;
         }
         
         nChar = false;
      }
      else if (isPunctSpace(*(sGreekText + 1)))
      {
         fSigma = true;
      }
      else if (*(sGreekText + 1) == (unsigned char)IOTA_SUB)
      {
         iota = true;
      }
      
      if (*sGreekText != (unsigned char)IOTA_SUB)
      {
         if (*sGreekText == (unsigned char)' ')
         {
            nChar = true;
         }

         if (breathing)
         {
            if (rough)
            {
               // When we read a rough breather we want to increment the pointer 
               // to the right character before char2Font is called.

               sResult[index++] = 
                  char2Font(*++sGreekText, fSigma, iota, breathing, rough);

               sGreekText++;
               characters++;
            }
            else
            {
               sResult[index++] = 
                  char2Font(*sGreekText++, fSigma, iota, breathing, rough);
               characters++;
            }
         }
         else
         {
            sResult[index++] = 
               char2Font(*sGreekText++, fSigma, iota, breathing, rough);
            characters++;
         }
      }
      else
      {
         sGreekText++;
         characters++;
      }

      fSigma = iota = breathing = rough = false;
   }

   sResult[index] = 0;					// Terminate the string

   return characters;
}


//*****************************************************************************
// Convert a character to a GREEK font character
//*****************************************************************************
unsigned char char2Font(
               unsigned char letter,      // bGreek letter to convert to Font letter
               bool finalSigma,  // Is it a final SIGMA
               bool iota,        // true = IOTA subscript; false = No IOTA
               bool breathing,   // true = add breathing; false = no breathing
               bool rough)       // true = rough breathing; false = smooth
{
   unsigned char charFont = 0;

   switch (letter)
   {
      case ALPHA:            // A
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_ALPHA;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_ALPHA;
         }
         else
         {
            if (iota)
            {
               charFont = (unsigned char)gIOTA_ALPHA;
            }
            else
               charFont = (unsigned char)gALPHA;
         }

      break;

      case BETA:             // B
         charFont = (unsigned char)gBETA;

      break;

      case CHI:              // C
         charFont = (unsigned char)gCHI;

      break;

      case DELTA:            // D
         charFont = (unsigned char)gDELTA;

      break;

      case EPSILON:          // E
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_EPSILON;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_EPSILON;
         }
         else
         {
            charFont = (unsigned char)gEPSILON;
         }

      break;

      case PHI:              // F
         charFont = (unsigned char)gPHI;

      break;

      case GAMMA:            // G
         charFont = (unsigned char)gGAMMA;

      break;

      case ETA:              // H
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_ETA;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_ETA;
         }
         else
         {
            if (iota)
            {
               charFont = (unsigned char)gIOTA_ETA;
            }
            else
               charFont = (unsigned char)gETA;
         }

      break;

      case IOTA:             // I
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_IOTA;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_IOTA;
         }
         else
         {
            charFont = (unsigned char)gIOTA;
         }

      break;

      case KAPPA:            // K
         charFont = (unsigned char)gKAPPA;

      break;

      case LAMBDA:           // L
         charFont = (unsigned char)gLAMBDA;

      break;

      case MU:               // M
         charFont = (unsigned char)gMU;

      break;

      case NU:               // N
         charFont = (unsigned char)gNU;

      break;

      case OMICRON:          // O
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_OMICRON;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_OMICRON;
         }
         else
         {
            charFont = (unsigned char)gOMICRON;
         }

      break;

      case PI:               // P
         charFont = (unsigned char)gPI;

      break;

      case THETA:            // Q
         charFont = (unsigned char)gTHETA;

      break;

      case RHO:              // R
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_RHO;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_RHO;
         }
         else
         {
            charFont = (unsigned char)gRHO;
         }

      break;

      case SIGMA:            // S
         if (finalSigma)
            charFont = (unsigned char)gSIGMA_END;
         else
            charFont = (unsigned char)gSIGMA;

      break;

      case TAU:              // T
         charFont = (unsigned char)gTAU;

      break;

      case UPSILON:          // U
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_UPSILON;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_UPSILON;
         }
         else
         {
            charFont = (unsigned char)gUPSILON;
         }

      break;

      case OMEGA:            // W
         if (breathing)
         {
            if (rough)
            {
               charFont = (unsigned char)gROUGH_OMEGA;
            }
            else
               charFont = (unsigned char)gNON_ROUGH_OMEGA;
         }
         else
         {
            if (iota)
            {
               charFont = (unsigned char)gIOTA_OMEGA;
            }
            else
               charFont = (unsigned char)gOMEGA;
         }

      break;

      case XI:               // X
         charFont = (unsigned char)gXI;

      break;

      case PSI:              // Y
         charFont = (unsigned char)gPSI;

      break;

      case ZETA:             // Z
         charFont = (unsigned char)gZETA;

      break;

      default:
         if (ispunct(letter) || isspace(letter))
         {
            charFont = getGreekPunct(letter);
         }

         if (isdigit(letter))
            charFont = letter;

      break;
   }

   return charFont;
}


//*****************************************************************************
// Convert a GREEK font character to a character
//*****************************************************************************
unsigned char Font2char(
               unsigned char letter,       // bGreek letter to convert to Font letter
               bool &iota,        // true = IOTA subscript; false = No IOTA
               bool &breathing,   // true = add breathing; false = no breathing
               bool &rough)       // true = rough breathing; false = smooth
{
   unsigned char character = 0;

   if (getSpecialChar(letter, letter))
   {
      switch (letter)
      {
         case gROUGH_ALPHA:         // hA
         case gIOTA_ALPHA:          // Ai
         case gNON_ROUGH_ALPHA:     // hA
            character = ALPHA;

            if (letter == gIOTA_ALPHA)
               iota = true;
            else
               iota = false;

            if (letter == gROUGH_ALPHA)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_EPSILON:       // hE
         case gNON_ROUGH_EPSILON:   // hE
            character = EPSILON;
            iota = false;

            if (letter == gROUGH_EPSILON)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_ETA:           // hH
         case gIOTA_ETA:            // Ei
         case gNON_ROUGH_ETA:       // hH
            character = ETA;

            if (letter == gIOTA_ETA)
               iota = true;
            else
               iota = false;

            if (letter == gROUGH_ETA)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_IOTA:          // hH
         case gNON_ROUGH_IOTA:      // hH
            character = IOTA;
            iota = false;

            if (letter == gROUGH_IOTA)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_OMICRON:       // hH
         case gNON_ROUGH_OMICRON:   // hH
            character = OMICRON;
            iota = false;

            if (letter == gROUGH_OMICRON)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_RHO:           // hR
         case gNON_ROUGH_RHO:       // hR
            character = RHO;
            iota = false;

            if (letter == gROUGH_RHO)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_UPSILON:       // hU
         case gNON_ROUGH_UPSILON:   // hU
            character = UPSILON;
            iota = false;

            if (letter == gROUGH_UPSILON)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;

         case gROUGH_OMEGA:         // hW
         case gIOTA_OMEGA:          // Wi
         case gNON_ROUGH_OMEGA:     // hW
            character = OMEGA;

            if (letter == gIOTA_OMEGA)
               iota = true;
            else
               iota = false;

            if (letter == gROUGH_OMEGA)
            {
               breathing = true;
               rough = true;
            }
            else
            {
               breathing = false;
               rough = false;
            }

            break;
      }
   }  // if (letter > SPECIAL_GREEK)
   else
   {
      if (letter == gSIGMA_END)
      {
         character = SIGMA;
      }
      else if (ispunct(letter) || isspace(letter))
      {
         character = getbGreekPunct(letter);
      }
      else if (isdigit(letter))
      {
         character = letter;
      }
      else
      {
         character = letter - 32;
      }
   }

   return character;
}

//*****************************************************************************
// Identify and return a bGreek letter from a special font char
//*****************************************************************************
bool getSpecialChar(unsigned char Font, unsigned char &letter)
{
   bool Yes = false;
   letter = Font;

   if (Font >= 133 && Font <= 144)
   {
      letter = gIOTA;
      Font = gIOTA;
   }

   if (Font >= 154 && Font <= 159)
   {
      letter = gEPSILON;
      Font = gEPSILON;
   }

   if (Font >= 163 && Font <= 171)
   {
      letter = gALPHA;
      Font = gALPHA;
   }

   if (Font >= 172 && Font <= 182)
   {
      letter = gIOTA_ALPHA;
      Font = gIOTA_ALPHA;
      Yes = true;
   }

   if (Font >= 187 && Font <= 195)
   {
      letter = gETA;
      Font = gETA;
   }
 
   if (Font >= 197 && Font <= 207)
   {
      letter = gIOTA_ETA;
      Font = gIOTA_ETA;
      Yes = true;
   }

   if ((Font >= 210 && Font <= 215) || Font == 253)
   {
      letter = gOMICRON;
      Font = gOMICRON;
   }

   if (Font >= 218 && Font <= 229)
   {
      letter = gUPSILON;
      Font = gUPSILON;
   }

   if (Font >= 232 && Font <= 240)
   {
      letter = gOMEGA;
      Font = gOMEGA;
   }

   if (Font >= 241 && Font <= 251)
   {
      letter = gIOTA_OMEGA;
      Font = gIOTA_OMEGA;
      Yes = true;
   }

   Yes = SpecialGreek(Font);

   return Yes;
}


//*****************************************************************************
// true if the font character is a special character; false it isn't
//*****************************************************************************

bool SpecialGreek(unsigned char Font)
{
   bool res = false;

   switch (Font)
   {
      case gROUGH_ALPHA:
      case gROUGH_EPSILON:
      case gROUGH_ETA:
      case gROUGH_IOTA:
      case gROUGH_OMICRON:
      case gROUGH_RHO:
      case gROUGH_UPSILON:
      case gROUGH_OMEGA:
      case gIOTA_ALPHA:
      case gIOTA_ETA:
      case gIOTA_OMEGA:
      case gNON_ROUGH_ALPHA:
      case gNON_ROUGH_EPSILON:
      case gNON_ROUGH_ETA:
      case gNON_ROUGH_IOTA:
      case gNON_ROUGH_OMICRON:
      case gNON_ROUGH_RHO:
      case gNON_ROUGH_UPSILON:
      case gNON_ROUGH_OMEGA:
         res = true;

         break;
   }

   return res;
}


//*****************************************************************************
// Return Greek font puntuation from bGreek punstuation
//*****************************************************************************

unsigned char getGreekPunct(unsigned char bGreek)
{
   unsigned char Font;

   switch (bGreek)
   {
      case COMMA:
         Font = gCOMMA;
      break;

      case STOP:
         Font = gSTOP;
      break;

      case SEMI_COLON:
         Font = gSEMI_COLON;
      break;

      case QUESTION:
         Font = gQUESTION;
      break;

      default:
         Font = ' ';
      break;
   }

   return Font;
}


//*****************************************************************************
// Return bGreek puntuation from Greek font punstuation
//*****************************************************************************

unsigned char getbGreekPunct(unsigned char Greek)
{
   unsigned char character;

   switch (Greek)
   {
      case gCOMMA:
         character = COMMA;
      break;

      case gSTOP:
         character = STOP;
      break;

      case gSEMI_COLON:
         character = SEMI_COLON;
      break;

      case gQUESTION:
         character = QUESTION;
      break;

      default:
         character = ' ';
      break;   
   }

   return character;
}


//*****************************************************************************
// Is the character punctuation or a space: true it is, false it isn't
//*****************************************************************************

bool isPunctSpace(unsigned char c)
{
   return (ispunct(c) || isspace(c) || c == 0) ? true : false;
}

#ifdef __TEST

int main()
{
   unsigned char *sGreekText = (unsigned char *)
      "1„£kwboj qeoà kaˆ kur…ou „hsoà cristoà doàloj ta‹j dèdeka fula‹j ta‹j ™n tÍ diaspor´ ca…rein.";
   unsigned char *sResult = new unsigned char[100];

   char result = Greek2bGreek(
                  sResult, 
                  sGreekText, 
                  100);

   strset((char *)sResult, 0);
   strset((char *)sGreekText, 0);

   sGreekText = (unsigned char *)"18 EIS AFESIN TWN hAMARTWN hUMWN?";
   result = bGreek2Greek(
                  sResult, 
                  sGreekText, 
                  33);

   //delete[] sGreekText;
   delete[] sResult;
}

#endif // __TEST

SWORD_NAMESPACE_END
