/******************************************************************************
 * apocrypha.h - Apocryphal text information to be included by VerseKey.cpp
 *
 * $Id: apocrypha.h,v 1.2 2003/04/21 01:33:37 chrislit Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

/******************************************************************************
 * atbooks - initialize static instance for all apocryphal text names
 *		and chapmax
 */

#include <defs.h>
//SWORD_NAMESPACE_START

//Data based on NRSVA unless otherwise noted; this book ordering is not necessarily optimal.

struct sbook
VerseKey::atbooks[] = {
  //Catholic Deuterocanon
  {"Tobit", "Tob", 14},                        //67
  {"Judith", "Jdt", 16},                       //68
  {"Wisdom", "Wis", 19},                       //69
  {"Sirach", "Sir", 51},                       //70   //51 or 52? count prologue as separate book or just chapter?
  {"Baruch", "Bar", 5},                        //71
  {"Letter of Jeremiah", "EpJer", 1},          //72   //1 or 6?  EpJer is ch6 of Baruch
  {"1 Esdras", "1Esd", 9},                     //73
  {"2 Esdras", "2Esd", 16},                    //74
  {"1 Maccabees", "1Macc", 16},                //75
  {"2 Maccabees", "2Macc", 15},                //76

  //LXX
  {"3 Maccabees", "3Macc", 7},                 //77
  {"4 Maccabees", "4Macc", 18},                //78
  {"Odes", "Odes", 14},                        //79   //based on LXX (Rahlf's)
  {"Psalms of Solomon", "PssSol", 18},         //80   //based on LXX, compared with Charlesworth

  //Protestant Apocrypha
  {"Additions to Esther", "AddEsth", 6},       //81   //based on Charles   //6 or F or 16?  If you're Catholic, you have a 16 chapter canonical book called Esther (ie Greek Esther); if you're Protestant you have a 10 chapter canonical book called Esther (ie Hebrew Esther) plus a 6 "chapter" set of "Additions" in an apocryphal book called Additions to Esther, which are "numbered" A through F.
  {"Prayer of Azariah", "PrAzar", 1},          //82
  {"Susanna", "Sus", 1},                       //83
  {"Bel and the Dragon", "Bel", 1},            //84
  {"Prayer of Manasses", "PrMan", 1},          //85
  {"Psalm 151", "Ps151", 1},                   //86

  //Orthodox Canon
  {"1 Enoch", "1En", 108},                     //87   //based on Charlesworth, compared with Charles
  {"Jubilees", "Jub", 50},                     //88   //based on Charles, compared with Charlesworth

  //Vulgate
  {"Epistle to the Laodiceans", "EpLao", 1},   //89   //based on Vulgate, compared with James
};

/******************************************************************************
 *	Abbreviations - MUST be in alphabetical order & by PRIORITY
 *		RULE: first match of entire key
 *			(e.g. key: "1CH"; match: "1CHRONICLES")
 */
/*
const struct abbrev
  VerseKey::builtin_abbrevs[] = {

  {"", -1}
};
*/

/******************************************************************************
 *	Maximum verses per chapter
 */

int
VerseKey::vm[] = {
  //Catholic Deuterocanon
  //Tobit 14                        //67
  22, 14, 17, 21, 21, 17, 18, 21, 6, 12, 19, 22, 18, 15,
  //Judith 16                       //68
  16, 28, 10, 15, 24, 21, 32, 36, 14, 23, 23, 20, 20, 19, 13, 25,
  //Wisdom 19                       //69
  16, 24, 19, 20, 23, 25, 30, 21, 18, 21, 26, 27, 19, 31, 19, 29, 21, 25, 22,
  //Sirach 51                       //70
  30, 18, 31, 31, 15, 37, 36, 19, 18, 31, 34, 18, 26, 27, 20, 30, 32, 33, 30, 32, 28, 27, 27, 34, 26, 29, 30, 26, 28, 25, 31, 24, 31, 26, 20, 26, 31, 34, 35, 30, 23, 25, 33, 23, 26, 20, 25, 25, 16, 29, 30,
  //Baruch 5                        //71
  21, 35, 37, 37, 9,
  //Letter of Jeremiah 1            //72
  73,
  //1 Esdras 9                      //73
  58, 30, 24, 63, 73, 34, 15, 96, 55,
  //2 Esdras 16                     //74
  40, 48, 36, 52, 56, 59, 140, 63, 47, 59, 46, 51, 58, 48, 63, 78,
  //1 Maccabees 16                  //75
  64, 70, 60, 61, 68, 63, 50, 32, 73, 89, 74, 53, 53, 49, 41, 24,
  //2 Maccabees 15                  //76
  36, 32, 40, 50, 27, 31, 42, 36, 29, 38, 38, 45, 26, 46, 39,

  //LXX
  //3 Maccabees 7                   //77
  29, 33, 30, 21, 51, 41, 23,
  //4 Maccabees 18                  //78
  35, 24, 21, 26, 38, 35, 23, 29, 32, 21, 27, 19, 27, 20, 32, 25, 24, 24,
  //Odes of Solomon 14              //79
  19, 43, 10, 19, 20, 10, 45, 88, 79, 9, 20, 15, 32, 46,
  //Psalms of Solomon 18            //80
  8, 37, 12, 25, 19, 6, 10, 34, 11, 8, 9, 6, 12, 10, 13, 15, 46, 12,

  //Protestant Apocrypha
  //Additions to Esther 6           //81
  17, 7, 30, 16, 24, 11
  //Prayer of Azariah 1             //82
  68,
  //Susanna 1                       //83
  64,
  //Bel and the Dragon 1            //84
  42,
  //Prayer of Manasses 1            //85
  15,
  //Psalm 151 1                     //86
  7,

  //Orthodox Canon
  //1 Enoch 108                     //87   //based on Charlesworth, compared with Charles
  9, 3, 1, 1, 10, 8, 6, 4, 11, 22, 2, 6, 10, 25, 12, 4, 8, 16, 3, 8, 10, 14, 4, 6, 7, 6, 5, 3, 2, 3, 3, 6, 4, 3, 1, 4, 5, 6, 14, 10, 9, 3, 4, 1, 6, 8, 4, 10, 4, 5, 5, 9, 7, 10, 4, 8, 3, 6, 3, 25, 13, 16, 12, 2, 12, 3, 13, 5, 29, 4, 17, 37, 8, 17, 9, 14, 9, 17, 6, 8, 10, 20, 11, 6, 10, 6, 4, 3, 77, 42, 19, 5, 14, 11, 7, 8, 10, 16, 16, 13, 9, 11, 15, 13, 2, 19, 3, 15,
  //Jubilees 50                     //88
  29, 33, 35, 33, 32, 38, 39, 30, 15, 36, 24, 31, 29, 24, 34, 31, 18, 19, 31, 13, 26, 30, 32, 33, 23, 35, 27, 30, 20, 26, 32, 34, 23, 21, 27, 24, 25, 24, 18, 13, 28, 25, 24, 34, 16, 16, 12, 19, 23, 13

  //Vulgate
  //Epistle to the Laodiceans 1     //89
  20,
};


long
  VerseKey::atbks[] = {
0, 1, 16, 33, 53, 105, 111, 113, 123, 140, 157, 173, 181, 200, 215, 234, 241, 243, 245, 247, 249, 251, 360, 411
};

long
  VerseKey::atcps[] = {
0, 2, 3, 26, 41, 59, 81, 103, 121, 140, 162, 169, 182, 202, 225, 244, 261, 278, 307, 318, 334, 359, 381, 414, 451, 466, 490, 514, 535, 556, 576, 590, 617, 634, 659, 679, 700, 724, 750, 781, 803, 822, 844, 871, 899, 919, 951, 971, 1001, 1023, 1049, 1073, 1104, 1123, 1155, 1187, 1203, 1241, 1278, 1298, 1317, 1349, 1384, 1403, 1430, 1458, 1479, 1510, 1543, 1577, 1608, 1641, 1670, 1698, 1726, 1761, 1788, 1818, 1849, 1876, 1905, 1931, 1963, 1988, 2020, 2047, 2068, 2095, 2127, 2162, 2198, 2229, 2253, 2279, 2313, 2337, 2364, 2385, 2411, 2437, 2454, 2484, 2516, 2538, 2574, 2612, 2650, 2661, 2736, 2795, 2826, 2851, 2915, 2989, 3024, 3040, 3137, 3194, 3235, 3284, 3321, 3374, 3431, 3491, 3632, 3696, 3744, 3804, 3851, 3903, 3962, 4011, 4075, 4155, 4220, 4291, 4352, 4414, 4483, 4547, 4598, 4631, 4705, 4795, 4870, 4924, 4978, 5028, 5070, 5096, 5133, 5166, 5207, 5258, 5286, 5318, 5361, 5398, 5428, 5467, 5506, 5552, 5579, 5626, 5667, 5697, 5731, 5762, 5784, 5836, 5878, 5903, 5939, 5964, 5986, 6013, 6052, 6088, 6112, 6142, 6175, 6197, 6225, 6245, 6273, 6294, 6327, 6353, 6378, 6404, 6424, 6468, 6479, 6499, 6520, 6531, 6577, 6666, 6746, 6756, 6777, 6793, 6826, 6874, 6883, 6921, 6934, 6960, 6980, 6987, 6998, 7033, 7045, 7054, 7064, 7071, 7084, 7095, 7109, 7125, 7172, 7186, 7256, 7322, 7366, 7383, 7392, 7402, 7406, 7408, 7410, 7421, 7430, 7437, 7442, 7454, 7477, 7480, 7487, 7498, 7524, 7537, 7542, 7551, 7568, 7572, 7581, 7592, 7607, 7612, 7619, 7627, 7634, 7640, 7644, 7647, 7651, 7655, 7662, 7667, 7671, 7673, 7678, 7684, 7691, 7706, 7717, 7727, 7731, 7736, 7738, 7745, 7754, 7759, 7770, 7775, 7781, 7787, 7797, 7805, 7816, 7821, 7830, 7834, 7841, 7845, 7871, 7885, 7902, 7915, 7918, 7931, 7935, 7949, 7955, 7985, 7990, 8008, 8046, 8055, 8073, 8083, 8098, 8108, 8126, 8133, 8142, 8153, 8174, 8186, 8193, 8204, 8211, 8216, 8220, 8298, 8341, 8361, 8367, 8382, 8394, 8402, 8411, 8422, 8439, 8456, 8470, 8480, 8492, 8508, 8522, 8525, 8545, 8549, 8566
};

//SWORD_NAMESPACE_END
