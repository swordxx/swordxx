/******************************************************************************
 * osisbook.h - Canonical text information to be included by VerseKey2.cpp
 *
 * $Id: osisbook.h,v 1.1 2004/04/08 22:52:09 dglassey Exp $
 *
 * Copyright 2004 CrossWire Bible Society (http://www.crosswire.org)
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
 * [on]tbooks - initialize static instance for all canonical text names
 *		and chapmax
 *	taken from http://whi.wts.edu/OSIS/Projects/Markup/specs/BibleBookNames.html
 */

struct sbook
VerseKey2::osisbooks[] = {
//Old Testament
{"Old Testament", "OT"},
{"Genesis", "Gen"},
{"Exodus", "Exod"},
{"Leviticus", "Lev"},
{"Numbers", "Num"},
{"Deuteronomy", "Deut"},
{"Joshua", "Josh"},
{"Judges", "Judg"},
{"Ruth", "Ruth"},
{"1 Samuel", "1Sam"},
{"2 Samuel", "2Sam"},
{"1 Kings", "1Kgs"},
{"2 Kings", "2Kgs"},
{"1 Chronicles", "1Chr"},
{"2 Chronicles", "2Chr"},
{"Ezra", "Ezra"},
{"Nehemiah", "Neh"},
{"Esther", "Esth"},
{"Job", "Job"},
{"Psalms", "Pss"},
{"Proverbs", "Prov"},
{"Ecclesiastes", "Eccl"},		// 	Qohelot
{"Song of Solomon", "Song"}, 	// 	Canticle of Canticles
{"Isaiah", "Isa"},
{"Jeremiah", "Jer"},
{"Lamentations", "Lam"},
{"Ezekiel", "Ezek"},
{"Daniel", "Dan"},
{"Hosea", "Hos"},
{"Joel", "Joel"},
{"Amos", "Amos"},
{"Obadiah", "Obad"},
{"Jonah", "Jonah"},
{"Micah", "Mic"},
{"Nahum", "Nah"},
{"Habakkuk", "Hab"},
{"Zephaniah", "Zeph"},
{"Haggai", "Hag"},
{"Zechariah", "Zech"},
{"Malachi", "Mal"},
 
 
//New Testament
{"New Testament", "NT"},
{"Matthew", "Matt"},
{"Mark", "Mark"},
{"Luke", "Luke"},
{"John", "John"},
{"Acts", "Acts"},
{"Romans", "Rom"},
{"1 Corinthians", "1Cor"},
{"2 Corinthians", "2Cor"},
{"Galatians", "Gal"},
{"Ephesians", "Eph"},
{"Philippians", "Phil"},
{"Colossians", "Col"},
{"1 Thessalonians", "1Thess"},
{"2 Thessalonians", "2Thess"},
{"1 Timothy", "1Tim"},
{"2 Timothy", "2Tim"},
{"Titus", "Titus"},
{"Philemon", "Phlm"},
{"Hebrews", "Heb"},
{"James", "Jas"},
{"1 Peter", "1Pet"},
{"2 Peter", "2Pet"},
{"1 John", "1John"},
{"2 John", "2John"},
{"3 John", "3John"},
{"Jude", "Jude"},
{"Revelation", "Rev"},
 
 
//Roman Catholic Deuterocanon
{"Tobit", "Tob"},
{"Judith", "Jdt"},
{"Wisdom", "Wis"},			// 		Wisdom of Solomon
{"Sirach", "Sir"},			//  	Ecclesiasticus
{"Baruch", "Bar"},			//  	1 Baruch
{"Letter of Jeremiah", "EpJer"},
{"1 Esdras", "1Esd"},		//  	3Ezra 	Esdras A
{"2 Esdras", "2Esd"},		// 		4Ezra 	Esdras B
{"1 Maccabees", "1Macc"},
{"2 Maccabees", "2Macc"},
 
 
//Septuagint
{"3 Maccabees", "3Macc"},
{"4 Maccabees", "4Macc"},
{"Odes of Solomon", "OdesSol"},
{"Psalms of Solomon", "PssSol"},
 
 
//Vulgate
{"Epistle to the Laodiceans", "EpLao"},
 
 
//Orthodox Canon
{"1 Enoch", "1En"},		// 	Ethiopic Apocalypse of Enoch
{"Jubilees", "Jub"},
 
 
//Protestant Apocrypha
{"Additions to Esther", "AddEsth"},
{"Prayer of Azariah", "PrAzar"},	// 	Song of the Three Children
{"Susanna", "Sus"},
{"Bel and the Dragon", "Bel"},
{"Prayer of Manasses", "PrMan"},
{"Psalm 151", "Ps151"},

};

const struct abbrev
  VerseKey2::builtin_abbrevs[] = {
  {"1 C", 46},			//   1 Corinthians
  {"1 CHRONICLES", 13},		//   1 Chronicles
  {"1 CORINTHIANS", 46},	//   1 Corinthians
  {"1 JN", 62},			//    1 John
  {"1 JOHN", 62},		//    1 John
  {"1 KGS", 11},		//    1 Kings
  {"1 KINGS", 11},		//    1 Kings
  {"1 PETER", 60},		//    1 Peter
  {"1 PTR", 60},		//    1 Peter
  {"1 SAMUEL", 9},		//    1 Samuel
  {"1 THESSALONIANS", 52},	//   1 Thessalonians
  {"1 TIMOTHY", 54},		//   1 Timothy
  {"1C", 46},			//   1 Corinthians
  {"1CHRONICLES", 13},		//   1 Chronicles
  {"1CORINTHIANS", 46},		//   1 Corinthians
  {"1JN", 62},			//    1 John       
  {"1JOHN", 62},		//    1 John
  {"1KGS", 11},			// 1 Kings
  {"1KINGS", 11},		//    1 Kings
  {"1PETER", 60},		//    1 Peter
  {"1PTR", 60},			//    1 Peter
  {"1SAMUEL", 9},		//    1 Samuel
  {"1THESSALONIANS", 52},	//   1 Thessalonians
  {"1TIMOTHY", 54},		//   1 Timothy
  {"2 C", 47},			//   2 Corinthians
  {"2 CHRONICLES", 14},		//   2 Chronicles
  {"2 CORINTHIANS", 47},	//   2 Corinthians
  {"2 JN", 63},			//    2 John
  {"2 JOHN", 63},		//    2 John
  {"2 KGS", 12},		//    2 Kings
  {"2 KINGS", 12},		//    2 Kings
  {"2 PETER", 61},		//    2 Peter
  {"2 PTR", 61},		//    2 Peter
  {"2 SAMUEL", 10},		//    2 Samuel
  {"2 THESSALONIANS", 53},	//   2 Thessalonians
  {"2 TIMOTHY", 55},		//   2 Timothy
  {"2C", 47},			//   2 Corinthians
  {"2CHRONICLES", 14},		//   2 Chronicles
  {"2CORINTHIANS", 47},		//   2 Corinthians
  {"2JN", 63},			//    2 John    
  {"2JOHN", 63},		//    2 John
  {"2KGS", 12},			// 2 Kings
  {"2KINGS", 12},		//    2 Kings
  {"2PETER", 61},		//    2 Peter
  {"2PTR", 61},			//    2 Peter
  {"2SAMUEL", 10},		//    2 Samuel
  {"2THESSALONIANS", 53},	//   2 Thessalonians
  {"2TIMOTHY", 55},		//   2 Timothy
  {"3 JN", 64},			//    3 John
  {"3 JOHN", 64},		//    3 John
  {"3JN", 64},			//    3 John
  {"3JOHN", 64},		//    3 John
  {"ACTS", 44},			//     Acts
  {"AMOS", 30},			//    Amos
  {"APOCALYPSE OF ST. JOHN", 66},	//    Apocalypse of St. John (Rev.)
  {"C", 51},			//    Colossians
  {"CANTICLE OF CANTICLES", 22},	//    Canticle of Canticles (Song of S.)
  {"COLOSSIANS", 51},		//    Colossians
  {"D", 5},			//     Deuteronomy
  {"DANIEL", 27},		//    Daniel
  {"DEUTERONOMY", 5},		//    Deuteronomy
  {"E", 49},			//     Ephesians
  {"ECCLESIASTES", 21},		//    Ecclesiastes
  {"EPHESIANS", 49},		//    Ephesians
  {"ESTER", 17},		//    Esther
  {"ESTHER", 17},		//    Esther
  {"EXODUS", 2},		//    Exodus
  {"EZEKIEL", 26},		//   Ezekiel
  {"EZK", 26},		//   Ezekiel
  {"EZRA", 15},			//   Ezra
  {"G", 1},			//     Genesis
  {"GALATIANS", 48},		//    Galatians
  {"GENESIS", 1},		//    Genesis
  {"H", 58},			//     Hebrews
  {"HABAKKUK", 35},		//    Habakkuk
  {"HAGGAI", 37},		//   Haggai
  {"HEBREWS", 58},		//    Hebrews
  {"HOSEA", 28},		//    Hosea
  {"I C", 46},			//   1 Corinthians
  {"I CHRONICLES", 13},		//   1 Chronicles
  {"I CORINTHIANS", 46},	//   1 Corinthians
  {"I JN", 62},			//    1 John
  {"I JOHN", 62},		//    1 John
  {"I KGS", 11},		// 1 Kings
  {"I KINGS", 11},		//    1 Kings
  {"I PETER", 60},		//    1 Peter
  {"I PTR", 60},		//    1 Peter
  {"I SAMUEL", 9},		//    1 Samuel
  {"I THESSALONIANS", 52},	//   1 Thessalonians
  {"I TIMOTHY", 54},		//   1 Timothy
  {"IC", 46},			//   1 Corinthians
  {"ICHRONICLES", 13},		//   1 Chronicles
  {"ICORINTHIANS", 46},		//   1 Corinthians
  {"II C", 47},			//   2 Corinthians
  {"II CHRONICLES", 14},	//   2 Chronicles
  {"II CORINTHIANS", 47},	//   2 Corinthians
  {"II JN", 63},		//    2 John  
  {"II JOHN", 63},		//    2 John
  {"II KGS", 12},		// 2 Kings
  {"II KINGS", 12},		//    2 Kings
  {"II PETER", 61},		//    2 Peter
  {"II PTR", 61},		//    2 Peter
  {"II SAMUEL", 10},		//    2 Samuel
  {"II THESSALONIANS", 53},	//   2 Thessalonians
  {"II TIMOTHY", 55},		//   2 Timothy
  {"IIC", 47},			//   2 Corinthians
  {"IICHRONICLES", 14},		//   2 Chronicles
  {"IICORINTHIANS", 47},	//   2 Corinthians
  {"III JN", 64},		//    3 John 
  {"III JOHN", 64},		//    3 John
  {"IIIJN", 64},		//    3 John
  {"IIIJOHN", 64},		//    3 John
  {"IIJN", 63},			//    2 John
  {"IIJOHN", 63},		//    2 John
  {"IIKGS", 12},		// 2 Kings
  {"IIKINGS", 12},		//    2 Kings
  {"IIPETER", 61},		//    2 Peter
  {"IIPTR", 61},		//    2 Peter
  {"IISAMUEL", 10},		//    2 Samuel
  {"IITHESSALONIANS", 53},	//   2 Thessalonians
  {"IITIMOTHY", 55},		//   2 Timothy
  {"IJN", 62},			//    1 John
  {"IJOHN", 62},		//    1 John
  {"IKGS", 11},			// 1 Kings
  {"IKINGS", 11},		//    1 Kings
  {"IPETER", 60},		//    1 Peter
  {"IPTR", 60},			//    1 Peter
  {"ISA", 23},			//     Isaiah
  {"ISAIAH", 23},		//     Isaiah
  {"ISAMUEL", 9},		//    1 Samuel
  {"ITHESSALONIANS", 52},	//   1 Thessalonians
  {"ITIMOTHY", 54},		//   1 Timothy
  {"J", 43},			//     John
  {"JAMES", 59},		//    James
  {"JAS", 59},			//    James
  {"JDGS", 7},		//  Judges
  {"JEREMIAH", 24},		//    Jeremiah
  {"JHN", 43},			//    John
  {"JN", 43},			//    John
  {"JO", 43},			//    John
  {"JOB", 18},			//   Job
  {"JOEL", 29},			//   Joel
  {"JOHN", 43},			//   John
  {"JOL", 29},			//   Joel
  {"JONAH", 32},		//   Jonah
  {"JOSHUA", 6},		//   Joshua
  {"JUDE", 65},			//  Jude
  {"JUDGES", 7},		//  Judges
  {"L", 42},			//     Luke
  {"LAMENTATIONS", 25},		//    Lamentations
  {"LEVITICUS", 3},		//    Leviticus
  {"LK", 42},			//    Luke
  {"LUKE", 42},			//    Luke
  {"MA", 40},			//    Matthew
  {"MALACHI", 39},		//   Malachi
  {"MARK", 41},			//   Mark
  {"MATTHEW", 40},		//   Matthew
  {"MICAH", 33},		//    Micah
  {"MK", 41},			//    Mark
  {"MRK", 41},			//    Mark
  {"MT", 40},			//    Matthew
  {"N", 4},			//     Numbers
  {"NAHUM", 34},		//    Nahum
  {"NAM", 34},		//    Nahum
  {"NEHEMIAH", 16},		//    Nehemiah
  {"NUMBERS", 4},		//    Numbers
  {"OBADIAH", 31},		//     Obadiah
  {"P", 19},			//     Psalms
  {"PHIL", 50},			//    Philippians
  {"PHILEMON", 57},		// Philemon
  {"PHILIPPIANS", 50},		// Philippians
  {"PHLM", 57},		// Philemon
  {"PHM", 57},			//   Philemon
  {"PHP", 50},			//   Philippians
  {"PROVERBS", 20},		//    Proverbs
  {"PSALMS", 19},		//    Psalms
  {"PSM", 19},			// Psalms
  {"PSS", 19},			// Psalms
  {"QOHELETH", 21},              // Qohelet (Ecclesiastes)
  {"REVELATION OF JOHN", 66},	//     Revelation
  {"ROMANS", 45},		//    Romans
  {"RUTH", 8},			//    Ruth
  {"SNG", 22},	//     Song of Solomon
  {"SOLOMON", 22},	//     Song of Solomon
  {"SONG OF SOLOMON", 22},	//     Song of Solomon
  {"SONG OF SONGS", 22},	//     Song of Solomon
  {"SOS", 22},			//     Song of Solomon
  {"TITUS", 56},		//     Titus
  {"ZECHARIAH", 38},		//   Zechariah
  {"ZEPHANIAH", 36},		//   Zephaniah
  {"", -1}
};


/* The default versification scheme is KJV */
/*
  0, 1, 52, 93, 121, 158,
  193, 218, 240, 245, 277, 302, 325,
  351, 381, 418, 429, 443, 454, 497,
  648, 680, 693, 702, 769, 822, 828,
  877, 890, 905, 909, 919, 921, 926,
  934, 938, 942, 946, 949, 964
*/
/* We are allowing one extra chapter per book for summary */
/* And an extra chapter for each testament for testament summary */
/* These are for commentaries */

struct bkref
  VerseKey2::kjvbks[] = {
//Header
{0, 1},
	//Old Testament
{1, 50},	// Genesis", "Gen
{52, 40},	// Exodus", "Exod
{93, 27},	// Leviticus", "Lev
{121, 36},	// Numbers", "Num
{158, 34},	// Deuteronomy", "Deut
{193, 24},	// Joshua", "Josh
{218, 21},	// Judges", "Judg
{240, 4},	// Ruth", "Ruth
{245, 31},	// 1 Samuel", "1Sam
{277, 24},	// 2 Samuel", "2Sam
{301, 22},	// 1 Kings", "1Kgs
{325, 25},	// 2 Kings", "2Kgs
{351, 29},	// 1 Chronicles", "1Chr
{381, 36},	// 2 Chronicles", "2Chr
{418, 10},	// Ezra", "Ezra
{429, 13},	// Nehemiah", "Neh
{442, 10},	// Esther", "Esth
{454, 42},	// Job", "Job
{497, 150},	// Psalms", "Pss
{648, 31},	// Proverbs", "Prov
{680, 12},	// Ecclesiastes", "Eccl
{693, 8},	// Song of Solomon", "Song
{702, 66},	// Isaiah", "Isa
{769, 52},	// Jeremiah", "Jer
{822, 5},	// Lamentations", "Lam
{828, 48},	// Ezekiel", "Ezek
{877, 12},	// Daniel", "Dan
{890, 14},	// Hosea", "Hos
{905, 3},	// Joel", "Joel
{909, 9},	// Amos", "Amos
{919, 1},	// Obadiah", "Obad
{921, 4},	// Jonah", "Jonah
{926, 7},	// Micah", "Mic
{934, 3},	// Nahum", "Nah
{938, 3},	// Habakkuk", "Hab
{942, 3},	// Zephaniah", "Zeph
{946, 2},	// Haggai", "Hag
{949, 14},	// Zechariah", "Zech
{964, 4},	// Malachi", "Mal

/*
  0, 1, 30, 47, 72, 94,
  123, 140, 157, 171, 178, 185, 190,
  195, 201, 205, 212, 217, 221, 223,
  237, 243, 249, 253, 259, 261, 263,
  265
*/
 
//Header
{969, 1},
//New Testament
{970, 28},	// Matthew", "Matt
{999, 16},	// Mark", "Mark
{1016, 24},	// Luke", "Luke
{1041, 21},	// John", "John
{1063, 28},	// Acts", "Acts
{1092, 16},	// Romans", "Rom
{1109, 16},	// 1 Corinthians", "1Cor
{1126, 13},	// 2 Corinthians", "2Cor
{1140, 6},	// Galatians", "Gal
{1147, 6},	// Ephesians", "Eph
{1154, 4},	// Philippians", "Phil
{1159, 4},	// Colossians", "Col
{1164, 5},	// 1 Thessalonians", "1Thess
{1170, 3},	// 2 Thessalonians", "2Thess
{1174, 6},	// 1 Timothy", "1Tim
{1181, 4},	// 2 Timothy", "2Tim
{1186, 3},	// Titus", "Titus
{1190, 1},	// Philemon", "Phlm
{1192, 13},	// Hebrews", "Heb
{1206, 5},	// James", "Jas
{1212, 5},	// 1 Peter", "1Pet
{1218, 3},	// 2 Peter", "2Pet
{1222, 5},	// 1 John", "1John
{1228, 1},	// 2 John", "2John
{1230, 1},	// 3 John", "3John
{1232, 1},	// Jude", "Jude
{1232, 22},	// Revelation", "Rev
 
 
//Roman Catholic Deuterocanon
{0, 0},	// Tobit", "Tob
{0, 0},	// Judith", "Jdt
{0, 0},	// Wisdom", "Wis
{0, 0},	// Sirach", "Sir
{0, 0},	// Baruch", "Bar
{0, 0},	// Letter of Jeremiah", "EpJer
{0, 0},	// 1 Esdras", "1Esd
{0, 0},	// 2 Esdras", "2Esd
{0, 0},	// 1 Maccabees", "1Macc
{0, 0},	// 2 Maccabees", "2Macc
 
 
//Septuagint
{0, 0},	// 3 Maccabees", "3Macc
{0, 0},	// 4 Maccabees", "4Macc
{0, 0},	// Odes of Solomon", "OdesSol
{0, 0},	// Psalms of Solomon", "PssSol
 
 
//Vulgate
{0, 0},	// Epistle to the Laodiceans", "EpLao
 
 
//Orthodox Canon
{0, 0},	// 1 Enoch", "1En
{0, 0},	// Jubilees", "Jub
 
 
//Protestant Apocrypha
{0, 0},	// Additions to Esther", "AddEsth
{0, 0},	// Prayer of Azariah", "PrAzar
{0, 0},	// Susanna", "Sus
{0, 0},	// Bel and the Dragon", "Bel
{0, 0},	// Prayer of Manasses", "PrMan
{0, 0}	// Psalm 151", "Ps151
  
};

struct bkref
  VerseKey2::kjvcps[] = {
{0, 0}, // OT Header
{2, 0}, // Genesis:0
{3, 31}, // Genesis:1
{35, 25}, // Genesis:2
{61, 24}, // Genesis:3
{86, 26}, // Genesis:4
{113, 32}, // Genesis:5
{146, 22}, // Genesis:6
{169, 24}, // Genesis:7
{194, 22}, // Genesis:8
{217, 29}, // Genesis:9
{247, 32}, // Genesis:10
{280, 32}, // Genesis:11
{313, 20}, // Genesis:12
{334, 18}, // Genesis:13
{353, 24}, // Genesis:14
{378, 21}, // Genesis:15
{400, 16}, // Genesis:16
{417, 27}, // Genesis:17
{445, 33}, // Genesis:18
{479, 38}, // Genesis:19
{518, 18}, // Genesis:20
{537, 34}, // Genesis:21
{572, 24}, // Genesis:22
{597, 20}, // Genesis:23
{618, 67}, // Genesis:24
{686, 34}, // Genesis:25
{721, 35}, // Genesis:26
{757, 46}, // Genesis:27
{804, 22}, // Genesis:28
{827, 35}, // Genesis:29
{863, 43}, // Genesis:30
{907, 55}, // Genesis:31
{963, 32}, // Genesis:32
{996, 20}, // Genesis:33
{1017, 31}, // Genesis:34
{1049, 29}, // Genesis:35
{1079, 43}, // Genesis:36
{1123, 36}, // Genesis:37
{1160, 30}, // Genesis:38
{1191, 23}, // Genesis:39
{1215, 23}, // Genesis:40
{1239, 57}, // Genesis:41
{1297, 38}, // Genesis:42
{1336, 34}, // Genesis:43
{1371, 34}, // Genesis:44
{1406, 28}, // Genesis:45
{1435, 34}, // Genesis:46
{1470, 31}, // Genesis:47
{1502, 22}, // Genesis:48
{1525, 33}, // Genesis:49
{1559, 26}, // Genesis:50
{1586, 0}, // Exodus:0
{1587, 22}, // Exodus:1
{1610, 25}, // Exodus:2
{1636, 22}, // Exodus:3
{1659, 31}, // Exodus:4
{1691, 23}, // Exodus:5
{1715, 30}, // Exodus:6
{1746, 25}, // Exodus:7
{1772, 32}, // Exodus:8
{1805, 35}, // Exodus:9
{1841, 29}, // Exodus:10
{1871, 10}, // Exodus:11
{1882, 51}, // Exodus:12
{1934, 22}, // Exodus:13
{1957, 31}, // Exodus:14
{1989, 27}, // Exodus:15
{2017, 36}, // Exodus:16
{2054, 16}, // Exodus:17
{2071, 27}, // Exodus:18
{2099, 25}, // Exodus:19
{2125, 26}, // Exodus:20
{2152, 36}, // Exodus:21
{2189, 31}, // Exodus:22
{2221, 33}, // Exodus:23
{2255, 18}, // Exodus:24
{2274, 40}, // Exodus:25
{2315, 37}, // Exodus:26
{2353, 21}, // Exodus:27
{2375, 43}, // Exodus:28
{2419, 46}, // Exodus:29
{2466, 38}, // Exodus:30
{2505, 18}, // Exodus:31
{2524, 35}, // Exodus:32
{2560, 23}, // Exodus:33
{2584, 35}, // Exodus:34
{2620, 35}, // Exodus:35
{2656, 38}, // Exodus:36
{2695, 29}, // Exodus:37
{2725, 31}, // Exodus:38
{2757, 43}, // Exodus:39
{2801, 38}, // Exodus:40
{2840, 0}, // Leviticus:0
{2841, 17}, // Leviticus:1
{2859, 16}, // Leviticus:2
{2876, 17}, // Leviticus:3
{2894, 35}, // Leviticus:4
{2930, 19}, // Leviticus:5
{2950, 30}, // Leviticus:6
{2981, 38}, // Leviticus:7
{3020, 36}, // Leviticus:8
{3057, 24}, // Leviticus:9
{3082, 20}, // Leviticus:10
{3103, 47}, // Leviticus:11
{3151, 8}, // Leviticus:12
{3160, 59}, // Leviticus:13
{3220, 57}, // Leviticus:14
{3278, 33}, // Leviticus:15
{3312, 34}, // Leviticus:16
{3347, 16}, // Leviticus:17
{3364, 30}, // Leviticus:18
{3395, 37}, // Leviticus:19
{3433, 27}, // Leviticus:20
{3461, 24}, // Leviticus:21
{3486, 33}, // Leviticus:22
{3520, 44}, // Leviticus:23
{3565, 23}, // Leviticus:24
{3589, 55}, // Leviticus:25
{3645, 46}, // Leviticus:26
{3692, 34}, // Leviticus:27
{3727, 0}, // Numbers:0
{3728, 54}, // Numbers:1
{3783, 34}, // Numbers:2
{3818, 51}, // Numbers:3
{3870, 49}, // Numbers:4
{3920, 31}, // Numbers:5
{3952, 27}, // Numbers:6
{3980, 89}, // Numbers:7
{4070, 26}, // Numbers:8
{4097, 23}, // Numbers:9
{4121, 36}, // Numbers:10
{4158, 35}, // Numbers:11
{4194, 16}, // Numbers:12
{4211, 33}, // Numbers:13
{4245, 45}, // Numbers:14
{4291, 41}, // Numbers:15
{4333, 50}, // Numbers:16
{4384, 13}, // Numbers:17
{4398, 32}, // Numbers:18
{4431, 22}, // Numbers:19
{4454, 29}, // Numbers:20
{4484, 35}, // Numbers:21
{4520, 41}, // Numbers:22
{4562, 30}, // Numbers:23
{4593, 25}, // Numbers:24
{4619, 18}, // Numbers:25
{4638, 65}, // Numbers:26
{4704, 23}, // Numbers:27
{4728, 31}, // Numbers:28
{4760, 40}, // Numbers:29
{4801, 16}, // Numbers:30
{4818, 54}, // Numbers:31
{4873, 42}, // Numbers:32
{4916, 56}, // Numbers:33
{4973, 29}, // Numbers:34
{5003, 34}, // Numbers:35
{5038, 13}, // Numbers:36
{5052, 0}, // Deuteronomy:0
{5053, 46}, // Deuteronomy:1
{5100, 37}, // Deuteronomy:2
{5138, 29}, // Deuteronomy:3
{5168, 49}, // Deuteronomy:4
{5218, 33}, // Deuteronomy:5
{5252, 25}, // Deuteronomy:6
{5278, 26}, // Deuteronomy:7
{5305, 20}, // Deuteronomy:8
{5326, 29}, // Deuteronomy:9
{5356, 22}, // Deuteronomy:10
{5379, 32}, // Deuteronomy:11
{5412, 32}, // Deuteronomy:12
{5445, 18}, // Deuteronomy:13
{5464, 29}, // Deuteronomy:14
{5494, 23}, // Deuteronomy:15
{5518, 22}, // Deuteronomy:16
{5541, 20}, // Deuteronomy:17
{5562, 22}, // Deuteronomy:18
{5585, 21}, // Deuteronomy:19
{5607, 20}, // Deuteronomy:20
{5628, 23}, // Deuteronomy:21
{5652, 30}, // Deuteronomy:22
{5683, 25}, // Deuteronomy:23
{5709, 22}, // Deuteronomy:24
{5732, 19}, // Deuteronomy:25
{5752, 19}, // Deuteronomy:26
{5772, 26}, // Deuteronomy:27
{5799, 68}, // Deuteronomy:28
{5868, 29}, // Deuteronomy:29
{5898, 20}, // Deuteronomy:30
{5919, 30}, // Deuteronomy:31
{5950, 52}, // Deuteronomy:32
{6003, 29}, // Deuteronomy:33
{6033, 12}, // Deuteronomy:34
{6046, 0}, // Joshua:0
{6047, 18}, // Joshua:1
{6066, 24}, // Joshua:2
{6091, 17}, // Joshua:3
{6109, 24}, // Joshua:4
{6134, 15}, // Joshua:5
{6150, 27}, // Joshua:6
{6178, 26}, // Joshua:7
{6205, 35}, // Joshua:8
{6241, 27}, // Joshua:9
{6269, 43}, // Joshua:10
{6313, 23}, // Joshua:11
{6337, 24}, // Joshua:12
{6362, 33}, // Joshua:13
{6396, 15}, // Joshua:14
{6412, 63}, // Joshua:15
{6476, 10}, // Joshua:16
{6487, 18}, // Joshua:17
{6506, 28}, // Joshua:18
{6535, 51}, // Joshua:19
{6587, 9}, // Joshua:20
{6597, 45}, // Joshua:21
{6643, 34}, // Joshua:22
{6678, 16}, // Joshua:23
{6695, 33}, // Joshua:24
{6729, 0}, // Judges:0
{6730, 36}, // Judges:1
{6767, 23}, // Judges:2
{6791, 31}, // Judges:3
{6823, 24}, // Judges:4
{6848, 31}, // Judges:5
{6880, 40}, // Judges:6
{6921, 25}, // Judges:7
{6947, 35}, // Judges:8
{6983, 57}, // Judges:9
{7041, 18}, // Judges:10
{7060, 40}, // Judges:11
{7101, 15}, // Judges:12
{7117, 25}, // Judges:13
{7143, 20}, // Judges:14
{7164, 20}, // Judges:15
{7185, 31}, // Judges:16
{7217, 13}, // Judges:17
{7231, 31}, // Judges:18
{7263, 30}, // Judges:19
{7294, 48}, // Judges:20
{7343, 25}, // Judges:21
{7369, 0}, // Ruth:0
{7370, 22}, // Ruth:1
{7393, 23}, // Ruth:2
{7417, 18}, // Ruth:3
{7436, 22}, // Ruth:4
{7459, 0}, // I Samuel:0
{7460, 28}, // I Samuel:1
{7489, 36}, // I Samuel:2
{7526, 21}, // I Samuel:3
{7548, 22}, // I Samuel:4
{7571, 12}, // I Samuel:5
{7584, 21}, // I Samuel:6
{7606, 17}, // I Samuel:7
{7624, 22}, // I Samuel:8
{7647, 27}, // I Samuel:9
{7675, 27}, // I Samuel:10
{7703, 15}, // I Samuel:11
{7719, 25}, // I Samuel:12
{7745, 23}, // I Samuel:13
{7769, 52}, // I Samuel:14
{7822, 35}, // I Samuel:15
{7858, 23}, // I Samuel:16
{7882, 58}, // I Samuel:17
{7941, 30}, // I Samuel:18
{7972, 24}, // I Samuel:19
{7997, 42}, // I Samuel:20
{8040, 15}, // I Samuel:21
{8056, 23}, // I Samuel:22
{8080, 29}, // I Samuel:23
{8110, 22}, // I Samuel:24
{8133, 44}, // I Samuel:25
{8178, 25}, // I Samuel:26
{8204, 12}, // I Samuel:27
{8217, 25}, // I Samuel:28
{8243, 11}, // I Samuel:29
{8255, 31}, // I Samuel:30
{8287, 13}, // I Samuel:31
{8301, 0}, // II Samuel:0
{8302, 27}, // II Samuel:1
{8330, 32}, // II Samuel:2
{8363, 39}, // II Samuel:3
{8403, 12}, // II Samuel:4
{8416, 25}, // II Samuel:5
{8442, 23}, // II Samuel:6
{8466, 29}, // II Samuel:7
{8496, 18}, // II Samuel:8
{8515, 13}, // II Samuel:9
{8529, 19}, // II Samuel:10
{8549, 27}, // II Samuel:11
{8577, 31}, // II Samuel:12
{8609, 39}, // II Samuel:13
{8649, 33}, // II Samuel:14
{8683, 37}, // II Samuel:15
{8721, 23}, // II Samuel:16
{8745, 29}, // II Samuel:17
{8775, 33}, // II Samuel:18
{8809, 43}, // II Samuel:19
{8853, 26}, // II Samuel:20
{8880, 22}, // II Samuel:21
{8903, 51}, // II Samuel:22
{8955, 39}, // II Samuel:23
{8995, 25}, // II Samuel:24
{9021, 0}, // I Kings:0
{9022, 53}, // I Kings:1
{9076, 46}, // I Kings:2
{9123, 28}, // I Kings:3
{9152, 34}, // I Kings:4
{9187, 18}, // I Kings:5
{9206, 38}, // I Kings:6
{9245, 51}, // I Kings:7
{9297, 66}, // I Kings:8
{9364, 28}, // I Kings:9
{9393, 29}, // I Kings:10
{9423, 43}, // I Kings:11
{9467, 33}, // I Kings:12
{9501, 34}, // I Kings:13
{9536, 31}, // I Kings:14
{9568, 34}, // I Kings:15
{9603, 34}, // I Kings:16
{9638, 24}, // I Kings:17
{9663, 46}, // I Kings:18
{9710, 21}, // I Kings:19
{9732, 43}, // I Kings:20
{9776, 29}, // I Kings:21
{9806, 53}, // I Kings:22
{9860, 0}, // II Kings:0
{9861, 18}, // II Kings:1
{9880, 25}, // II Kings:2
{9906, 27}, // II Kings:3
{9934, 44}, // II Kings:4
{9979, 27}, // II Kings:5
{10007, 33}, // II Kings:6
{10041, 20}, // II Kings:7
{10062, 29}, // II Kings:8
{10092, 37}, // II Kings:9
{10130, 36}, // II Kings:10
{10167, 21}, // II Kings:11
{10189, 21}, // II Kings:12
{10211, 25}, // II Kings:13
{10237, 29}, // II Kings:14
{10267, 38}, // II Kings:15
{10306, 20}, // II Kings:16
{10327, 41}, // II Kings:17
{10369, 37}, // II Kings:18
{10407, 37}, // II Kings:19
{10445, 21}, // II Kings:20
{10467, 26}, // II Kings:21
{10494, 20}, // II Kings:22
{10515, 37}, // II Kings:23
{10553, 20}, // II Kings:24
{10574, 30}, // II Kings:25
{10605, 0}, // I Chronicles:0
{10606, 54}, // I Chronicles:1
{10661, 55}, // I Chronicles:2
{10717, 24}, // I Chronicles:3
{10742, 43}, // I Chronicles:4
{10786, 26}, // I Chronicles:5
{10813, 81}, // I Chronicles:6
{10895, 40}, // I Chronicles:7
{10936, 40}, // I Chronicles:8
{10977, 44}, // I Chronicles:9
{11022, 14}, // I Chronicles:10
{11037, 47}, // I Chronicles:11
{11085, 40}, // I Chronicles:12
{11126, 14}, // I Chronicles:13
{11141, 17}, // I Chronicles:14
{11159, 29}, // I Chronicles:15
{11189, 43}, // I Chronicles:16
{11233, 27}, // I Chronicles:17
{11261, 17}, // I Chronicles:18
{11279, 19}, // I Chronicles:19
{11299, 8}, // I Chronicles:20
{11308, 30}, // I Chronicles:21
{11339, 19}, // I Chronicles:22
{11359, 32}, // I Chronicles:23
{11392, 31}, // I Chronicles:24
{11424, 31}, // I Chronicles:25
{11456, 32}, // I Chronicles:26
{11489, 34}, // I Chronicles:27
{11524, 21}, // I Chronicles:28
{11546, 30}, // I Chronicles:29
{11577, 0}, // II Chronicles:0
{11578, 17}, // II Chronicles:1
{11596, 18}, // II Chronicles:2
{11615, 17}, // II Chronicles:3
{11633, 22}, // II Chronicles:4
{11656, 14}, // II Chronicles:5
{11671, 42}, // II Chronicles:6
{11714, 22}, // II Chronicles:7
{11737, 18}, // II Chronicles:8
{11756, 31}, // II Chronicles:9
{11788, 19}, // II Chronicles:10
{11808, 23}, // II Chronicles:11
{11832, 16}, // II Chronicles:12
{11849, 22}, // II Chronicles:13
{11872, 15}, // II Chronicles:14
{11888, 19}, // II Chronicles:15
{11908, 14}, // II Chronicles:16
{11923, 19}, // II Chronicles:17
{11943, 34}, // II Chronicles:18
{11978, 11}, // II Chronicles:19
{11990, 37}, // II Chronicles:20
{12028, 20}, // II Chronicles:21
{12049, 12}, // II Chronicles:22
{12062, 21}, // II Chronicles:23
{12084, 27}, // II Chronicles:24
{12112, 28}, // II Chronicles:25
{12141, 23}, // II Chronicles:26
{12165, 9}, // II Chronicles:27
{12175, 27}, // II Chronicles:28
{12203, 36}, // II Chronicles:29
{12240, 27}, // II Chronicles:30
{12268, 21}, // II Chronicles:31
{12290, 33}, // II Chronicles:32
{12324, 25}, // II Chronicles:33
{12350, 33}, // II Chronicles:34
{12384, 27}, // II Chronicles:35
{12412, 23}, // II Chronicles:36
{12436, 0}, // Ezra:0
{12437, 11}, // Ezra:1
{12449, 70}, // Ezra:2
{12520, 13}, // Ezra:3
{12534, 24}, // Ezra:4
{12559, 17}, // Ezra:5
{12577, 22}, // Ezra:6
{12600, 28}, // Ezra:7
{12629, 36}, // Ezra:8
{12666, 15}, // Ezra:9
{12682, 44}, // Ezra:10
{12727, 0}, // Nehemiah:0
{12728, 11}, // Nehemiah:1
{12740, 20}, // Nehemiah:2
{12761, 32}, // Nehemiah:3
{12794, 23}, // Nehemiah:4
{12818, 19}, // Nehemiah:5
{12838, 19}, // Nehemiah:6
{12858, 73}, // Nehemiah:7
{12932, 18}, // Nehemiah:8
{12951, 38}, // Nehemiah:9
{12990, 39}, // Nehemiah:10
{13030, 36}, // Nehemiah:11
{13067, 47}, // Nehemiah:12
{13115, 31}, // Nehemiah:13
{13147, 0}, // Esther:0
{13148, 22}, // Esther:1
{13171, 23}, // Esther:2
{13195, 15}, // Esther:3
{13211, 17}, // Esther:4
{13229, 14}, // Esther:5
{13244, 14}, // Esther:6
{13259, 10}, // Esther:7
{13270, 17}, // Esther:8
{13288, 32}, // Esther:9
{13321, 3}, // Esther:10
{13325, 0}, // Job:0
{13326, 22}, // Job:1
{13349, 13}, // Job:2
{13363, 26}, // Job:3
{13390, 21}, // Job:4
{13412, 27}, // Job:5
{13440, 30}, // Job:6
{13471, 21}, // Job:7
{13493, 22}, // Job:8
{13516, 35}, // Job:9
{13552, 22}, // Job:10
{13575, 20}, // Job:11
{13596, 25}, // Job:12
{13622, 28}, // Job:13
{13651, 22}, // Job:14
{13674, 35}, // Job:15
{13710, 22}, // Job:16
{13733, 16}, // Job:17
{13750, 21}, // Job:18
{13772, 29}, // Job:19
{13802, 29}, // Job:20
{13832, 34}, // Job:21
{13867, 30}, // Job:22
{13898, 17}, // Job:23
{13916, 25}, // Job:24
{13942, 6}, // Job:25
{13949, 14}, // Job:26
{13964, 23}, // Job:27
{13988, 28}, // Job:28
{14017, 25}, // Job:29
{14043, 31}, // Job:30
{14075, 40}, // Job:31
{14116, 22}, // Job:32
{14139, 33}, // Job:33
{14173, 37}, // Job:34
{14211, 16}, // Job:35
{14228, 33}, // Job:36
{14262, 24}, // Job:37
{14287, 41}, // Job:38
{14329, 30}, // Job:39
{14360, 24}, // Job:40
{14385, 34}, // Job:41
{14420, 17}, // Job:42
{14438, 0}, // Psalms:0
{14439, 6}, // Psalms:1
{14446, 12}, // Psalms:2
{14459, 8}, // Psalms:3
{14468, 8}, // Psalms:4
{14477, 12}, // Psalms:5
{14490, 10}, // Psalms:6
{14501, 17}, // Psalms:7
{14519, 9}, // Psalms:8
{14529, 20}, // Psalms:9
{14550, 18}, // Psalms:10
{14569, 7}, // Psalms:11
{14577, 8}, // Psalms:12
{14586, 6}, // Psalms:13
{14593, 7}, // Psalms:14
{14601, 5}, // Psalms:15
{14607, 11}, // Psalms:16
{14619, 15}, // Psalms:17
{14635, 50}, // Psalms:18
{14686, 14}, // Psalms:19
{14701, 9}, // Psalms:20
{14711, 13}, // Psalms:21
{14725, 31}, // Psalms:22
{14757, 6}, // Psalms:23
{14764, 10}, // Psalms:24
{14775, 22}, // Psalms:25
{14798, 12}, // Psalms:26
{14811, 14}, // Psalms:27
{14826, 9}, // Psalms:28
{14836, 11}, // Psalms:29
{14848, 12}, // Psalms:30
{14861, 24}, // Psalms:31
{14886, 11}, // Psalms:32
{14898, 22}, // Psalms:33
{14921, 22}, // Psalms:34
{14944, 28}, // Psalms:35
{14973, 12}, // Psalms:36
{14986, 40}, // Psalms:37
{15027, 22}, // Psalms:38
{15050, 13}, // Psalms:39
{15064, 17}, // Psalms:40
{15082, 13}, // Psalms:41
{15096, 11}, // Psalms:42
{15108, 5}, // Psalms:43
{15114, 26}, // Psalms:44
{15141, 17}, // Psalms:45
{15159, 11}, // Psalms:46
{15171, 9}, // Psalms:47
{15181, 14}, // Psalms:48
{15196, 20}, // Psalms:49
{15217, 23}, // Psalms:50
{15241, 19}, // Psalms:51
{15261, 9}, // Psalms:52
{15271, 6}, // Psalms:53
{15278, 7}, // Psalms:54
{15286, 23}, // Psalms:55
{15310, 13}, // Psalms:56
{15324, 11}, // Psalms:57
{15336, 11}, // Psalms:58
{15348, 17}, // Psalms:59
{15366, 12}, // Psalms:60
{15379, 8}, // Psalms:61
{15388, 12}, // Psalms:62
{15401, 11}, // Psalms:63
{15413, 10}, // Psalms:64
{15424, 13}, // Psalms:65
{15438, 20}, // Psalms:66
{15459, 7}, // Psalms:67
{15467, 35}, // Psalms:68
{15503, 36}, // Psalms:69
{15540, 5}, // Psalms:70
{15546, 24}, // Psalms:71
{15571, 20}, // Psalms:72
{15592, 28}, // Psalms:73
{15621, 23}, // Psalms:74
{15645, 10}, // Psalms:75
{15656, 12}, // Psalms:76
{15669, 20}, // Psalms:77
{15690, 72}, // Psalms:78
{15763, 13}, // Psalms:79
{15777, 19}, // Psalms:80
{15797, 16}, // Psalms:81
{15814, 8}, // Psalms:82
{15823, 18}, // Psalms:83
{15842, 12}, // Psalms:84
{15855, 13}, // Psalms:85
{15869, 17}, // Psalms:86
{15887, 7}, // Psalms:87
{15895, 18}, // Psalms:88
{15914, 52}, // Psalms:89
{15967, 17}, // Psalms:90
{15985, 16}, // Psalms:91
{16002, 15}, // Psalms:92
{16018, 5}, // Psalms:93
{16024, 23}, // Psalms:94
{16048, 11}, // Psalms:95
{16060, 13}, // Psalms:96
{16074, 12}, // Psalms:97
{16087, 9}, // Psalms:98
{16097, 9}, // Psalms:99
{16107, 5}, // Psalms:100
{16113, 8}, // Psalms:101
{16122, 28}, // Psalms:102
{16151, 22}, // Psalms:103
{16174, 35}, // Psalms:104
{16210, 45}, // Psalms:105
{16256, 48}, // Psalms:106
{16305, 43}, // Psalms:107
{16349, 13}, // Psalms:108
{16363, 31}, // Psalms:109
{16395, 7}, // Psalms:110
{16403, 10}, // Psalms:111
{16414, 10}, // Psalms:112
{16425, 9}, // Psalms:113
{16435, 8}, // Psalms:114
{16444, 18}, // Psalms:115
{16463, 19}, // Psalms:116
{16483, 2}, // Psalms:117
{16486, 29}, // Psalms:118
{16516, 176}, // Psalms:119
{16693, 7}, // Psalms:120
{16701, 8}, // Psalms:121
{16710, 9}, // Psalms:122
{16720, 4}, // Psalms:123
{16725, 8}, // Psalms:124
{16734, 5}, // Psalms:125
{16740, 6}, // Psalms:126
{16747, 5}, // Psalms:127
{16753, 6}, // Psalms:128
{16760, 8}, // Psalms:129
{16769, 8}, // Psalms:130
{16778, 3}, // Psalms:131
{16782, 18}, // Psalms:132
{16801, 3}, // Psalms:133
{16805, 3}, // Psalms:134
{16809, 21}, // Psalms:135
{16831, 26}, // Psalms:136
{16858, 9}, // Psalms:137
{16868, 8}, // Psalms:138
{16877, 24}, // Psalms:139
{16902, 13}, // Psalms:140
{16916, 10}, // Psalms:141
{16927, 7}, // Psalms:142
{16935, 12}, // Psalms:143
{16948, 15}, // Psalms:144
{16964, 21}, // Psalms:145
{16986, 10}, // Psalms:146
{16997, 20}, // Psalms:147
{17018, 14}, // Psalms:148
{17033, 9}, // Psalms:149
{17043, 6}, // Psalms:150
{17050, 0}, // Proverbs:0
{17051, 33}, // Proverbs:1
{17085, 22}, // Proverbs:2
{17108, 35}, // Proverbs:3
{17144, 27}, // Proverbs:4
{17172, 23}, // Proverbs:5
{17196, 35}, // Proverbs:6
{17232, 27}, // Proverbs:7
{17260, 36}, // Proverbs:8
{17297, 18}, // Proverbs:9
{17316, 32}, // Proverbs:10
{17349, 31}, // Proverbs:11
{17381, 28}, // Proverbs:12
{17410, 25}, // Proverbs:13
{17436, 35}, // Proverbs:14
{17472, 33}, // Proverbs:15
{17506, 33}, // Proverbs:16
{17540, 28}, // Proverbs:17
{17569, 24}, // Proverbs:18
{17594, 29}, // Proverbs:19
{17624, 30}, // Proverbs:20
{17655, 31}, // Proverbs:21
{17687, 29}, // Proverbs:22
{17717, 35}, // Proverbs:23
{17753, 34}, // Proverbs:24
{17788, 28}, // Proverbs:25
{17817, 28}, // Proverbs:26
{17846, 27}, // Proverbs:27
{17874, 28}, // Proverbs:28
{17903, 27}, // Proverbs:29
{17931, 33}, // Proverbs:30
{17965, 31}, // Proverbs:31
{17997, 0}, // Ecclesiastes:0
{17998, 18}, // Ecclesiastes:1
{18017, 26}, // Ecclesiastes:2
{18044, 22}, // Ecclesiastes:3
{18067, 16}, // Ecclesiastes:4
{18084, 20}, // Ecclesiastes:5
{18105, 12}, // Ecclesiastes:6
{18118, 29}, // Ecclesiastes:7
{18148, 17}, // Ecclesiastes:8
{18166, 18}, // Ecclesiastes:9
{18185, 20}, // Ecclesiastes:10
{18206, 10}, // Ecclesiastes:11
{18217, 14}, // Ecclesiastes:12
{18232, 0}, // Song of Solomon:0
{18233, 17}, // Song of Solomon:1
{18251, 17}, // Song of Solomon:2
{18269, 11}, // Song of Solomon:3
{18281, 16}, // Song of Solomon:4
{18298, 16}, // Song of Solomon:5
{18315, 13}, // Song of Solomon:6
{18329, 13}, // Song of Solomon:7
{18343, 14}, // Song of Solomon:8
{18358, 0}, // Isaiah:0
{18359, 31}, // Isaiah:1
{18391, 22}, // Isaiah:2
{18414, 26}, // Isaiah:3
{18441, 6}, // Isaiah:4
{18448, 30}, // Isaiah:5
{18479, 13}, // Isaiah:6
{18493, 25}, // Isaiah:7
{18519, 22}, // Isaiah:8
{18542, 21}, // Isaiah:9
{18564, 34}, // Isaiah:10
{18599, 16}, // Isaiah:11
{18616, 6}, // Isaiah:12
{18623, 22}, // Isaiah:13
{18646, 32}, // Isaiah:14
{18679, 9}, // Isaiah:15
{18689, 14}, // Isaiah:16
{18704, 14}, // Isaiah:17
{18719, 7}, // Isaiah:18
{18727, 25}, // Isaiah:19
{18753, 6}, // Isaiah:20
{18760, 17}, // Isaiah:21
{18778, 25}, // Isaiah:22
{18804, 18}, // Isaiah:23
{18823, 23}, // Isaiah:24
{18847, 12}, // Isaiah:25
{18860, 21}, // Isaiah:26
{18882, 13}, // Isaiah:27
{18896, 29}, // Isaiah:28
{18926, 24}, // Isaiah:29
{18951, 33}, // Isaiah:30
{18985, 9}, // Isaiah:31
{18995, 20}, // Isaiah:32
{19016, 24}, // Isaiah:33
{19041, 17}, // Isaiah:34
{19059, 10}, // Isaiah:35
{19070, 22}, // Isaiah:36
{19093, 38}, // Isaiah:37
{19132, 22}, // Isaiah:38
{19155, 8}, // Isaiah:39
{19164, 31}, // Isaiah:40
{19196, 29}, // Isaiah:41
{19226, 25}, // Isaiah:42
{19252, 28}, // Isaiah:43
{19281, 28}, // Isaiah:44
{19310, 25}, // Isaiah:45
{19336, 13}, // Isaiah:46
{19350, 15}, // Isaiah:47
{19366, 22}, // Isaiah:48
{19389, 26}, // Isaiah:49
{19416, 11}, // Isaiah:50
{19428, 23}, // Isaiah:51
{19452, 15}, // Isaiah:52
{19468, 12}, // Isaiah:53
{19481, 17}, // Isaiah:54
{19499, 13}, // Isaiah:55
{19513, 12}, // Isaiah:56
{19526, 21}, // Isaiah:57
{19548, 14}, // Isaiah:58
{19563, 21}, // Isaiah:59
{19585, 22}, // Isaiah:60
{19608, 11}, // Isaiah:61
{19620, 12}, // Isaiah:62
{19633, 19}, // Isaiah:63
{19653, 12}, // Isaiah:64
{19666, 25}, // Isaiah:65
{19692, 24}, // Isaiah:66
{19717, 0}, // Jeremiah:0
{19718, 19}, // Jeremiah:1
{19738, 37}, // Jeremiah:2
{19776, 25}, // Jeremiah:3
{19802, 31}, // Jeremiah:4
{19834, 31}, // Jeremiah:5
{19866, 30}, // Jeremiah:6
{19897, 34}, // Jeremiah:7
{19932, 22}, // Jeremiah:8
{19955, 26}, // Jeremiah:9
{19982, 25}, // Jeremiah:10
{20008, 23}, // Jeremiah:11
{20032, 17}, // Jeremiah:12
{20050, 27}, // Jeremiah:13
{20078, 22}, // Jeremiah:14
{20101, 21}, // Jeremiah:15
{20123, 21}, // Jeremiah:16
{20145, 27}, // Jeremiah:17
{20173, 23}, // Jeremiah:18
{20197, 15}, // Jeremiah:19
{20213, 18}, // Jeremiah:20
{20232, 14}, // Jeremiah:21
{20247, 30}, // Jeremiah:22
{20278, 40}, // Jeremiah:23
{20319, 10}, // Jeremiah:24
{20330, 38}, // Jeremiah:25
{20369, 24}, // Jeremiah:26
{20394, 22}, // Jeremiah:27
{20417, 17}, // Jeremiah:28
{20435, 32}, // Jeremiah:29
{20468, 24}, // Jeremiah:30
{20493, 40}, // Jeremiah:31
{20534, 44}, // Jeremiah:32
{20579, 26}, // Jeremiah:33
{20606, 22}, // Jeremiah:34
{20629, 19}, // Jeremiah:35
{20649, 32}, // Jeremiah:36
{20682, 21}, // Jeremiah:37
{20704, 28}, // Jeremiah:38
{20733, 18}, // Jeremiah:39
{20752, 16}, // Jeremiah:40
{20769, 18}, // Jeremiah:41
{20788, 22}, // Jeremiah:42
{20811, 13}, // Jeremiah:43
{20825, 30}, // Jeremiah:44
{20856, 5}, // Jeremiah:45
{20862, 28}, // Jeremiah:46
{20891, 7}, // Jeremiah:47
{20899, 47}, // Jeremiah:48
{20947, 39}, // Jeremiah:49
{20987, 46}, // Jeremiah:50
{21034, 64}, // Jeremiah:51
{21099, 34}, // Jeremiah:52
{21134, 0}, // Lamentations:0
{21135, 22}, // Lamentations:1
{21158, 22}, // Lamentations:2
{21181, 66}, // Lamentations:3
{21248, 22}, // Lamentations:4
{21271, 22}, // Lamentations:5
{21294, 0}, // Ezekiel:0
{21295, 28}, // Ezekiel:1
{21324, 10}, // Ezekiel:2
{21335, 27}, // Ezekiel:3
{21363, 17}, // Ezekiel:4
{21381, 17}, // Ezekiel:5
{21399, 14}, // Ezekiel:6
{21414, 27}, // Ezekiel:7
{21442, 18}, // Ezekiel:8
{21461, 11}, // Ezekiel:9
{21473, 22}, // Ezekiel:10
{21496, 25}, // Ezekiel:11
{21522, 28}, // Ezekiel:12
{21551, 23}, // Ezekiel:13
{21575, 23}, // Ezekiel:14
{21599, 8}, // Ezekiel:15
{21608, 63}, // Ezekiel:16
{21672, 24}, // Ezekiel:17
{21697, 32}, // Ezekiel:18
{21730, 14}, // Ezekiel:19
{21745, 49}, // Ezekiel:20
{21795, 32}, // Ezekiel:21
{21828, 31}, // Ezekiel:22
{21860, 49}, // Ezekiel:23
{21910, 27}, // Ezekiel:24
{21938, 17}, // Ezekiel:25
{21956, 21}, // Ezekiel:26
{21978, 36}, // Ezekiel:27
{22015, 26}, // Ezekiel:28
{22042, 21}, // Ezekiel:29
{22064, 26}, // Ezekiel:30
{22091, 18}, // Ezekiel:31
{22110, 32}, // Ezekiel:32
{22143, 33}, // Ezekiel:33
{22177, 31}, // Ezekiel:34
{22209, 15}, // Ezekiel:35
{22225, 38}, // Ezekiel:36
{22264, 28}, // Ezekiel:37
{22293, 23}, // Ezekiel:38
{22317, 29}, // Ezekiel:39
{22347, 49}, // Ezekiel:40
{22397, 26}, // Ezekiel:41
{22424, 20}, // Ezekiel:42
{22445, 27}, // Ezekiel:43
{22473, 31}, // Ezekiel:44
{22505, 25}, // Ezekiel:45
{22531, 24}, // Ezekiel:46
{22556, 23}, // Ezekiel:47
{22580, 35}, // Ezekiel:48
{22616, 0}, // Daniel:0
{22617, 21}, // Daniel:1
{22639, 49}, // Daniel:2
{22689, 30}, // Daniel:3
{22720, 37}, // Daniel:4
{22758, 31}, // Daniel:5
{22790, 28}, // Daniel:6
{22819, 28}, // Daniel:7
{22848, 27}, // Daniel:8
{22876, 27}, // Daniel:9
{22904, 21}, // Daniel:10
{22926, 45}, // Daniel:11
{22972, 13}, // Daniel:12
{22986, 0}, // Hosea:0
{22987, 11}, // Hosea:1
{22999, 23}, // Hosea:2
{23023, 5}, // Hosea:3
{23029, 19}, // Hosea:4
{23049, 15}, // Hosea:5
{23065, 11}, // Hosea:6
{23077, 16}, // Hosea:7
{23094, 14}, // Hosea:8
{23109, 17}, // Hosea:9
{23127, 15}, // Hosea:10
{23143, 12}, // Hosea:11
{23156, 14}, // Hosea:12
{23171, 16}, // Hosea:13
{23188, 9}, // Hosea:14
{23198, 0}, // Joel:0
{23199, 20}, // Joel:1
{23220, 32}, // Joel:2
{23253, 21}, // Joel:3
{23275, 0}, // Amos:0
{23276, 15}, // Amos:1
{23292, 16}, // Amos:2
{23309, 15}, // Amos:3
{23325, 13}, // Amos:4
{23339, 27}, // Amos:5
{23367, 14}, // Amos:6
{23382, 17}, // Amos:7
{23400, 14}, // Amos:8
{23415, 15}, // Amos:9
{23431, 0}, // Obadiah:0
{23432, 21}, // Obadiah:1
{23454, 0}, // Jonah:0
{23455, 17}, // Jonah:1
{23473, 10}, // Jonah:2
{23484, 10}, // Jonah:3
{23495, 11}, // Jonah:4
{23507, 0}, // Micah:0
{23508, 16}, // Micah:1
{23525, 13}, // Micah:2
{23539, 12}, // Micah:3
{23552, 13}, // Micah:4
{23566, 15}, // Micah:5
{23582, 16}, // Micah:6
{23599, 20}, // Micah:7
{23620, 0}, // Nahum:0
{23621, 15}, // Nahum:1
{23637, 13}, // Nahum:2
{23651, 19}, // Nahum:3
{23671, 0}, // Habakkuk:0
{23672, 17}, // Habakkuk:1
{23690, 20}, // Habakkuk:2
{23711, 19}, // Habakkuk:3
{23731, 0}, // Zephaniah:0
{23732, 18}, // Zephaniah:1
{23751, 15}, // Zephaniah:2
{23767, 20}, // Zephaniah:3
{23788, 0}, // Haggai:0
{23789, 15}, // Haggai:1
{23805, 23}, // Haggai:2
{23829, 0}, // Zechariah:0
{23830, 21}, // Zechariah:1
{23852, 13}, // Zechariah:2
{23866, 10}, // Zechariah:3
{23877, 14}, // Zechariah:4
{23892, 11}, // Zechariah:5
{23904, 15}, // Zechariah:6
{23920, 14}, // Zechariah:7
{23935, 23}, // Zechariah:8
{23959, 17}, // Zechariah:9
{23977, 12}, // Zechariah:10
{23990, 17}, // Zechariah:11
{24008, 14}, // Zechariah:12
{24023, 9}, // Zechariah:13
{24033, 21}, // Zechariah:14
{24055, 0}, // Malachi:0
{24056, 14}, // Malachi:1
{24071, 17}, // Malachi:2
{24089, 18}, // Malachi:3
{24108, 6}, // Malachi:4
{24115, 0}, // NT Header
{24117, 0}, // Matthew:0
{24118, 25}, // Matthew:1
{24144, 23}, // Matthew:2
{24168, 17}, // Matthew:3
{24186, 25}, // Matthew:4
{24212, 48}, // Matthew:5
{24261, 34}, // Matthew:6
{24296, 29}, // Matthew:7
{24326, 34}, // Matthew:8
{24361, 38}, // Matthew:9
{24400, 42}, // Matthew:10
{24443, 30}, // Matthew:11
{24474, 50}, // Matthew:12
{24525, 58}, // Matthew:13
{24584, 36}, // Matthew:14
{24621, 39}, // Matthew:15
{24661, 28}, // Matthew:16
{24690, 27}, // Matthew:17
{24718, 35}, // Matthew:18
{24754, 30}, // Matthew:19
{24785, 34}, // Matthew:20
{24820, 46}, // Matthew:21
{24867, 46}, // Matthew:22
{24914, 39}, // Matthew:23
{24954, 51}, // Matthew:24
{25006, 46}, // Matthew:25
{25053, 75}, // Matthew:26
{25129, 66}, // Matthew:27
{25196, 20}, // Matthew:28
{25217, 0}, // Mark:0
{25218, 45}, // Mark:1
{25264, 28}, // Mark:2
{25293, 35}, // Mark:3
{25329, 41}, // Mark:4
{25371, 43}, // Mark:5
{25415, 56}, // Mark:6
{25472, 37}, // Mark:7
{25510, 38}, // Mark:8
{25549, 50}, // Mark:9
{25600, 52}, // Mark:10
{25653, 33}, // Mark:11
{25687, 44}, // Mark:12
{25732, 37}, // Mark:13
{25770, 72}, // Mark:14
{25843, 47}, // Mark:15
{25891, 20}, // Mark:16
{25912, 0}, // Luke:0
{25913, 80}, // Luke:1
{25994, 52}, // Luke:2
{26047, 38}, // Luke:3
{26086, 44}, // Luke:4
{26131, 39}, // Luke:5
{26171, 49}, // Luke:6
{26221, 50}, // Luke:7
{26272, 56}, // Luke:8
{26329, 62}, // Luke:9
{26392, 42}, // Luke:10
{26435, 54}, // Luke:11
{26490, 59}, // Luke:12
{26550, 35}, // Luke:13
{26586, 35}, // Luke:14
{26622, 32}, // Luke:15
{26655, 31}, // Luke:16
{26687, 37}, // Luke:17
{26725, 43}, // Luke:18
{26769, 48}, // Luke:19
{26818, 47}, // Luke:20
{26866, 38}, // Luke:21
{26905, 71}, // Luke:22
{26977, 56}, // Luke:23
{27034, 53}, // Luke:24
{27088, 0}, // John:0
{27089, 51}, // John:1
{27141, 25}, // John:2
{27167, 36}, // John:3
{27204, 54}, // John:4
{27259, 47}, // John:5
{27307, 71}, // John:6
{27379, 53}, // John:7
{27433, 59}, // John:8
{27493, 41}, // John:9
{27535, 42}, // John:10
{27578, 57}, // John:11
{27636, 50}, // John:12
{27687, 38}, // John:13
{27726, 31}, // John:14
{27758, 27}, // John:15
{27786, 33}, // John:16
{27820, 26}, // John:17
{27847, 40}, // John:18
{27888, 42}, // John:19
{27931, 31}, // John:20
{27963, 25}, // John:21
{27989, 0}, // Acts:0
{27990, 26}, // Acts:1
{28017, 47}, // Acts:2
{28065, 26}, // Acts:3
{28092, 37}, // Acts:4
{28130, 42}, // Acts:5
{28173, 15}, // Acts:6
{28189, 60}, // Acts:7
{28250, 40}, // Acts:8
{28291, 43}, // Acts:9
{28335, 48}, // Acts:10
{28384, 30}, // Acts:11
{28415, 25}, // Acts:12
{28441, 52}, // Acts:13
{28494, 28}, // Acts:14
{28523, 41}, // Acts:15
{28565, 40}, // Acts:16
{28606, 34}, // Acts:17
{28641, 28}, // Acts:18
{28670, 41}, // Acts:19
{28712, 38}, // Acts:20
{28751, 40}, // Acts:21
{28792, 30}, // Acts:22
{28823, 35}, // Acts:23
{28859, 27}, // Acts:24
{28887, 27}, // Acts:25
{28915, 32}, // Acts:26
{28948, 44}, // Acts:27
{28993, 31}, // Acts:28
{29025, 0}, // Romans:0
{29026, 32}, // Romans:1
{29059, 29}, // Romans:2
{29089, 31}, // Romans:3
{29121, 25}, // Romans:4
{29147, 21}, // Romans:5
{29169, 23}, // Romans:6
{29193, 25}, // Romans:7
{29219, 39}, // Romans:8
{29259, 33}, // Romans:9
{29293, 21}, // Romans:10
{29315, 36}, // Romans:11
{29352, 21}, // Romans:12
{29374, 14}, // Romans:13
{29389, 23}, // Romans:14
{29413, 33}, // Romans:15
{29447, 27}, // Romans:16
{29475, 0}, // I Corinthians:0
{29476, 31}, // I Corinthians:1
{29508, 16}, // I Corinthians:2
{29525, 23}, // I Corinthians:3
{29549, 21}, // I Corinthians:4
{29571, 13}, // I Corinthians:5
{29585, 20}, // I Corinthians:6
{29606, 40}, // I Corinthians:7
{29647, 13}, // I Corinthians:8
{29661, 27}, // I Corinthians:9
{29689, 33}, // I Corinthians:10
{29723, 34}, // I Corinthians:11
{29758, 31}, // I Corinthians:12
{29790, 13}, // I Corinthians:13
{29804, 40}, // I Corinthians:14
{29845, 58}, // I Corinthians:15
{29904, 24}, // I Corinthians:16
{29929, 0}, // II Corinthians:0
{29930, 24}, // II Corinthians:1
{29955, 17}, // II Corinthians:2
{29973, 18}, // II Corinthians:3
{29992, 18}, // II Corinthians:4
{30011, 21}, // II Corinthians:5
{30033, 18}, // II Corinthians:6
{30052, 16}, // II Corinthians:7
{30069, 24}, // II Corinthians:8
{30094, 15}, // II Corinthians:9
{30110, 18}, // II Corinthians:10
{30129, 33}, // II Corinthians:11
{30163, 21}, // II Corinthians:12
{30185, 14}, // II Corinthians:13
{30200, 0}, // Galatians:0
{30201, 24}, // Galatians:1
{30226, 21}, // Galatians:2
{30248, 29}, // Galatians:3
{30278, 31}, // Galatians:4
{30310, 26}, // Galatians:5
{30337, 18}, // Galatians:6
{30356, 0}, // Ephesians:0
{30357, 23}, // Ephesians:1
{30381, 22}, // Ephesians:2
{30404, 21}, // Ephesians:3
{30426, 32}, // Ephesians:4
{30459, 33}, // Ephesians:5
{30493, 24}, // Ephesians:6
{30518, 0}, // Philippians:0
{30519, 30}, // Philippians:1
{30550, 30}, // Philippians:2
{30581, 21}, // Philippians:3
{30603, 23}, // Philippians:4
{30627, 0}, // Colossians:0
{30628, 29}, // Colossians:1
{30658, 23}, // Colossians:2
{30682, 25}, // Colossians:3
{30708, 18}, // Colossians:4
{30727, 0}, // I Thessalonians:0
{30728, 10}, // I Thessalonians:1
{30739, 20}, // I Thessalonians:2
{30760, 13}, // I Thessalonians:3
{30774, 18}, // I Thessalonians:4
{30793, 28}, // I Thessalonians:5
{30822, 0}, // II Thessalonians:0
{30823, 12}, // II Thessalonians:1
{30836, 17}, // II Thessalonians:2
{30854, 18}, // II Thessalonians:3
{30873, 0}, // I Timothy:0
{30874, 20}, // I Timothy:1
{30895, 15}, // I Timothy:2
{30911, 16}, // I Timothy:3
{30928, 16}, // I Timothy:4
{30945, 25}, // I Timothy:5
{30971, 21}, // I Timothy:6
{30993, 0}, // II Timothy:0
{30994, 18}, // II Timothy:1
{31013, 26}, // II Timothy:2
{31040, 17}, // II Timothy:3
{31058, 22}, // II Timothy:4
{31081, 0}, // Titus:0
{31082, 16}, // Titus:1
{31099, 15}, // Titus:2
{31115, 15}, // Titus:3
{31131, 0}, // Philemon:0
{31132, 25}, // Philemon:1
{31158, 0}, // Hebrews:0
{31159, 14}, // Hebrews:1
{31174, 18}, // Hebrews:2
{31193, 19}, // Hebrews:3
{31213, 16}, // Hebrews:4
{31230, 14}, // Hebrews:5
{31245, 20}, // Hebrews:6
{31266, 28}, // Hebrews:7
{31295, 13}, // Hebrews:8
{31309, 28}, // Hebrews:9
{31338, 39}, // Hebrews:10
{31378, 40}, // Hebrews:11
{31419, 29}, // Hebrews:12
{31449, 25}, // Hebrews:13
{31475, 0}, // James:0
{31476, 27}, // James:1
{31504, 26}, // James:2
{31531, 18}, // James:3
{31550, 17}, // James:4
{31568, 20}, // James:5
{31589, 0}, // I Peter:0
{31590, 25}, // I Peter:1
{31616, 25}, // I Peter:2
{31642, 22}, // I Peter:3
{31665, 19}, // I Peter:4
{31685, 14}, // I Peter:5
{31700, 0}, // II Peter:0
{31701, 21}, // II Peter:1
{31723, 22}, // II Peter:2
{31746, 18}, // II Peter:3
{31765, 0}, // I John:0
{31766, 10}, // I John:1
{31777, 29}, // I John:2
{31807, 24}, // I John:3
{31832, 21}, // I John:4
{31854, 21}, // I John:5
{31876, 0}, // II John:0
{31877, 13}, // II John:1
{31891, 0}, // III John:0
{31892, 14}, // III John:1
{31907, 0}, // Jude:0
{31908, 25}, // Jude:1
{31934, 0}, // Revelation of John:0
{31935, 20}, // Revelation of John:1
{31956, 29}, // Revelation of John:2
{31986, 22}, // Revelation of John:3
{32009, 11}, // Revelation of John:4
{32021, 14}, // Revelation of John:5
{32036, 17}, // Revelation of John:6
{32054, 17}, // Revelation of John:7
{32072, 13}, // Revelation of John:8
{32086, 21}, // Revelation of John:9
{32108, 11}, // Revelation of John:10
{32120, 19}, // Revelation of John:11
{32140, 17}, // Revelation of John:12
{32158, 18}, // Revelation of John:13
{32177, 20}, // Revelation of John:14
{32198, 8}, // Revelation of John:15
{32207, 21}, // Revelation of John:16
{32229, 18}, // Revelation of John:17
{32248, 24}, // Revelation of John:18
{32273, 21}, // Revelation of John:19
{32295, 15}, // Revelation of John:20
{32311, 27}, // Revelation of John:21
{32339, 21} // Revelation of John:22
};
