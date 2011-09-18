#ifndef _melder_h_
#define _melder_h_
/* melder.h
 *
 * Copyright (C) 1992-2011 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	#define strequ  ! strcmp
	#define strnequ  ! strncmp
	#define wcsequ  ! wcscmp
	#define wcsnequ  ! wcsncmp
	#define Melder_strequ  ! Melder_strcmp
	#define Melder_strnequ  ! Melder_strncmp
	#define Melder_wcsequ  ! Melder_wcscmp
	#define Melder_wcsnequ  ! Melder_wcsncmp
#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>
#ifdef __MINGW32__
	#define swprintf  _snwprintf
#endif
#include <stdbool.h>
#include <stdint.h>

typedef wchar_t wchar;
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

bool Melder_wcsequ_firstCharacterCaseInsensitive (const wchar *string1, const wchar *string2);

#include "enums.h"

#include "melder_enums.h"

#ifndef TRUE
	#define TRUE  1
#endif
#ifndef FALSE
	#define FALSE  0
#endif
#ifndef NULL
	#define NULL  ((void *) 0)
#endif

/********** NUMBER TO STRING CONVERSION **********/

/*
	The following routines return a static string, chosen from a circularly used set of 11 buffers.
	You can call at most 11 of them in one Melder_casual call, for instance.
*/
const wchar * Melder_integer (long value);
const wchar * Melder_bigInteger (double value);
const wchar * Melder_boolean (bool value);   // "yes" or "no"
const wchar * Melder_double (double value);   // "--undefined--" or something in the "%.15g", "%.16g", or "%.17g" formats
const wchar * Melder_single (double value);   // "--undefined--" or something in the "%.8g" format
const wchar * Melder_half (double value);   // "--undefined--" or something in the "%.4g" format
const wchar * Melder_fixed (double value, int precision);   // "--undefined--" or something in the "%.*f" format
const wchar * Melder_fixedExponent (double value, int exponent, int precision);
	/* if exponent is -2 and precision is 2:   67E-2, 0.00024E-2 */
const wchar * Melder_percent (double value, int precision);
	/* "--undefined--" or, if precision is 3: "0" or "34.400%" or "0.014%" or "0.001%" or "0.0000007%" */
const wchar * Melder_float (const wchar *number);
	/* turns 1e+4 into 10^^4, or -1.23456e-78 into -1.23456\.c10^^-78 */
const wchar * Melder_naturalLogarithm (double lnNumber);   // turns -10000 into "1.135483865315339e-4343"

/********** STRING TO NUMBER CONVERSION **********/

int Melder_isStringNumeric_nothrow (const wchar *string);
double Melder_atof (const wchar *string);
	/*
	 * "3.14e-3" -> 3.14e-3
	 * "15.6%" -> 0.156
	 * "fghfghj" -> NUMundefined
	 */

/********** CONSOLE **********/

void Melder_writeToConsole (const wchar *message, bool useStderr);

/********** MEMORY ALLOCATION ROUTINES **********/

/* These routines call malloc, free, realloc, and calloc. */
/* If out of memory, the non-f versions throw an error message (like "Out of memory"); */
/* the f versions open up a rainy day fund or crash Praat. */
/* These routines also maintain a count of the total number of blocks allocated. */

void Melder_alloc_init (void);   // to be called around program start-up
void * _Melder_malloc (unsigned long size);
#define Melder_malloc(type,numberOfElements)  (type *) _Melder_malloc ((numberOfElements) * sizeof (type))
void * _Melder_malloc_f (unsigned long size);
#define Melder_malloc_f(type,numberOfElements)  (type *) _Melder_malloc_f ((numberOfElements) * sizeof (type))
void * Melder_realloc (void *pointer, long size);
void * Melder_realloc_f (void *pointer, long size);
void * _Melder_calloc (long numberOfElements, long elementSize);
#define Melder_calloc(type,numberOfElements)  (type *) _Melder_calloc (numberOfElements, sizeof (type))
void * _Melder_calloc_f (long numberOfElements, long elementSize);
#define Melder_calloc_f(type,numberOfElements)  (type *) _Melder_calloc_f (numberOfElements, sizeof (type))
char * Melder_strdup (const char *string);
char * Melder_strdup_f (const char *string);
wchar * Melder_wcsdup (const wchar *string);
wchar * Melder_wcsdup_f (const wchar *string);
int Melder_strcmp (const char *string1, const char *string2);   // regards null string as empty string
int Melder_wcscmp (const wchar *string1, const wchar *string2);   // regards null string as empty string
int Melder_strncmp (const char *string1, const char *string2, unsigned long n);
int Melder_wcsncmp (const wchar *string1, const wchar *string2, unsigned long n);
wchar * Melder_wcstok (wchar *string, const wchar *delimiter, wchar **last);   // circumvents platforms where wcstok has only two arguments
wchar * Melder_wcsdecompose (const wchar *string);
wchar * Melder_wcsprecompose (const wchar *string);
wchar * Melder_wcsExpandBackslashSequences (const wchar *string);
wchar * Melder_wcsReduceBackslashSequences (const wchar *string);
void Melder_wcsReduceBackslashSequences_inline (const wchar *string);

/*
 * Text encodings.
 */
void Melder_textEncoding_prefs (void);
void Melder_setInputEncoding (enum kMelder_textInputEncoding encoding);
int Melder_getInputEncoding (void);
void Melder_setOutputEncoding (enum kMelder_textOutputEncoding encoding);
int Melder_getOutputEncoding (void);

/*
 * Some other encodings. Although not used in the above set/get functions,
 * these constants should stay separate from the above encoding constants
 * because they occur in the same fields of struct MelderFile.
 */
#define kMelder_textInputEncoding_FLAC  0x464C4143
#define kMelder_textOutputEncoding_ASCII  0x41534349
#define kMelder_textOutputEncoding_ISO_LATIN1  0x4C415401
#define kMelder_textOutputEncoding_FLAC  0x464C4143

typedef uint16 MelderUtf16;
typedef uint32 MelderUtf32;

bool Melder_isValidAscii (const wchar *string);
bool Melder_strIsValidUtf8 (const char *string);
bool Melder_isEncodable (const wchar *string, int outputEncoding);
extern wchar Melder_decodeMacRoman [256];
extern wchar Melder_decodeWindowsLatin1 [256];

long Melder_killReturns_inlineW (wchar *text);

unsigned long wcslen_utf8 (const wchar *wcs, bool expandNewlines);
unsigned long wcslen_utf16 (const wchar *wcs, bool expandNewlines);
unsigned long wcslen_utf32 (const wchar *wcs, bool expandNewlines);

void Melder_8bitToWcs_inline (const char *string, wchar *wcs, int inputEncoding);
	// errors: Text is not valid UTF-8.
wchar * Melder_8bitToWcs (const char *string, int inputEncoding);
	// errors: Out of memory; Text is not valid UTF-8.
wchar * Melder_utf8ToWcs (const char *string);
	// errors: Out of memory; Text is not valid UTF-8.

void Melder_wcsToUtf8_inline (const wchar *wcs, char *utf8);
char * Melder_wcsToUtf8 (const wchar *string);
	// errors: Out of memory.
void Melder_wcsTo8bitFileRepresentation_inline (const wchar *wcs, char *utf8);
void Melder_8bitFileRepresentationToWcs_inline (const char *utf8, wchar *wcs);
extern "C" wchar * Melder_peekUtf8ToWcs (const char *string);
extern "C" char * Melder_peekWcsToUtf8 (const wchar *string);
extern "C" const MelderUtf16 * Melder_peekWcsToUtf16 (const wchar *string);
const void * Melder_peekWcsToCfstring (const wchar *string);
void Melder_fwriteWcsAsUtf8 (const wchar *ptr, size_t n, FILE *f);

/*
 * Some often used characters.
 */
#define L_LEFT_SINGLE_QUOTE  L"\u2018"
#define L_RIGHT_SINGLE_QUOTE  L"\u2019"
#define L_LEFT_DOUBLE_QUOTE  L"\u201c"
#define L_RIGHT_DOUBLE_QUOTE  L"\u201d"
#define L_LEFT_GUILLEMET  L"\u00ab"
#define L_RIGHT_GUILLEMET  L"\u00bb"

#define Melder_free(pointer)  _Melder_free ((void **) & (pointer))
void _Melder_free (void **pointer);
/*
	Preconditions:
		none (*pointer may be NULL).
	Postconditions:
		*pointer == NULL;
*/

double Melder_allocationCount (void);
/*
	Returns the total number of successful calls to
	Melder_malloc, Melder_realloc (if 'ptr' is NULL), Melder_calloc, and Melder_strdup,
	since the start of the process. Mainly for debugging purposes.
*/

double Melder_deallocationCount (void);
/*
	Returns the total number of successful calls to Melder_free,
	since the start of the process. Mainly for debugging purposes.
*/

double Melder_allocationSize (void);
/*
	Returns the total number of bytes allocated in calls to
	Melder_malloc, Melder_realloc (if moved), Melder_calloc, and Melder_strdup,
	since the start of the process. Mainly for debugging purposes.
*/

double Melder_reallocationsInSituCount (void);
double Melder_movingReallocationsCount (void);

/********** FILES **********/

#if defined (_WIN32)
	#define Melder_DIRECTORY_SEPARATOR  '\\'
#else
	#define Melder_DIRECTORY_SEPARATOR  '/'
#endif

struct FLAC__StreamDecoder;
struct FLAC__StreamEncoder;

#define kMelder_MAXPATH 1023   /* excluding the null byte */

struct structMelderFile {
	FILE *filePointer;
	wchar path [kMelder_MAXPATH+1];
	bool openForReading, openForWriting, verbose, requiresCRLF;
	unsigned long outputEncoding;
	int indent;
	struct FLAC__StreamEncoder *flacEncoder;
};
typedef struct structMelderFile *MelderFile;

struct structMelderDir {
	wchar path [kMelder_MAXPATH+1];
};
typedef struct structMelderDir *MelderDir;

#if defined (macintosh)
	void Melder_machToFile (void *void_fsref, MelderFile file);
	void Melder_machToDir (void *void_fsref, MelderDir dir);
	void Melder_fileToMach (MelderFile file, void *void_fsref);
	void Melder_dirToMach (MelderDir dir, void *void_fsref);
#endif
const wchar * MelderFile_name (MelderFile file);
wchar * MelderDir_name (MelderDir dir);
void Melder_pathToDir (const wchar *path, MelderDir dir);
void Melder_pathToFile (const wchar *path, MelderFile file);
void Melder_relativePathToFile (const wchar *path, MelderFile file);
wchar * Melder_dirToPath (MelderDir dir);
	/* Returns a pointer internal to 'dir', like "/u/paul/praats" or "D:\Paul\Praats" */
wchar * Melder_fileToPath (MelderFile file);
void MelderFile_copy (MelderFile file, MelderFile copy);
void MelderDir_copy (MelderDir dir, MelderDir copy);
bool MelderFile_equal (MelderFile file1, MelderFile file2);
bool MelderDir_equal (MelderDir dir1, MelderDir dir2);
void MelderFile_setToNull (MelderFile file);
bool MelderFile_isNull (MelderFile file);
void MelderDir_setToNull (MelderDir dir);
bool MelderDir_isNull (MelderDir dir);
void MelderDir_getFile (MelderDir parent, const wchar *fileName, MelderFile file);
void MelderDir_relativePathToFile (MelderDir dir, const wchar *path, MelderFile file);
void MelderFile_getParentDir (MelderFile file, MelderDir parent);
void MelderDir_getParentDir (MelderDir file, MelderDir parent);
bool MelderDir_isDesktop (MelderDir dir);
void MelderDir_getSubdir (MelderDir parent, const wchar *subdirName, MelderDir subdir);
void Melder_rememberShellDirectory (void);
wchar * Melder_getShellDirectory (void);
void Melder_getHomeDir (MelderDir homeDir);
void Melder_getPrefDir (MelderDir prefDir);
void Melder_getTempDir (MelderDir tempDir);

bool MelderFile_exists (MelderFile file);
bool MelderFile_readable (MelderFile file);
long MelderFile_length (MelderFile file);
void MelderFile_delete (MelderFile file);

/* The following two should be combined with each other and with Windows extension setting: */
FILE * Melder_fopen (MelderFile file, const char *type);
#if defined (macintosh)
	void MelderFile_setMacTypeAndCreator (MelderFile file, long fileType, long creator);
	unsigned long MelderFile_getMacType (MelderFile file);
#else
	#define MelderFile_setMacTypeAndCreator(f,t,c)  (void) 0
#endif
void Melder_fclose (MelderFile file, FILE *stream);
void Melder_files_cleanUp (void);

/* So these will be the future replacements for the above, as soon as we rid of text files: */
MelderFile MelderFile_open (MelderFile file);
MelderFile MelderFile_append (MelderFile file);
MelderFile MelderFile_create (MelderFile file, const wchar *macType, const wchar *macCreator, const wchar *winExtension);
void * MelderFile_read (MelderFile file, long nbytes);
char * MelderFile_readLine (MelderFile file);
void MelderFile_writeCharacter (MelderFile file, wchar kar);
void MelderFile_write1 (MelderFile file, const wchar *s1);
void MelderFile_write2 (MelderFile file, const wchar *s1, const wchar *s2);
void MelderFile_write3 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3);
void MelderFile_write4 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void MelderFile_write5 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void MelderFile_write6 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void MelderFile_write7 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void MelderFile_write8 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void MelderFile_write9 (MelderFile file, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
void MelderFile_rewind (MelderFile file);
void MelderFile_seek (MelderFile file, long position, int direction);
long MelderFile_tell (MelderFile file);
void MelderFile_close (MelderFile file);
void MelderFile_close_nothrow (MelderFile file);

/* Read and write whole text files. */
wchar * MelderFile_readText (MelderFile file);
void MelderFile_writeText (MelderFile file, const wchar *text);
void MelderFile_appendText (MelderFile file, const wchar *text);

void Melder_createDirectory (MelderDir parent, const wchar *subdirName, int mode);

void Melder_getDefaultDir (MelderDir dir);
void Melder_setDefaultDir (MelderDir dir);
void MelderFile_setDefaultDir (MelderFile file);

/* Use the following functions to pass unchanged text or file names to Melder_* functions. */
/* Backslashes are replaced by "\bs". */
/* The trick is that they return one of 11 cyclically used static strings, */
/* so you can use up to 11 strings in a single Melder_* call. */
wchar * Melder_peekExpandBackslashes (const wchar *message);
const wchar * MelderFile_messageName (MelderFile file);   // Calls Melder_peekExpandBackslashes ().

/********** STRINGS **********/

/* These are routines for never having to check string boundaries again. */

typedef struct {
	unsigned long length;
	unsigned long bufferSize;
	wchar *string;   // a growing buffer, never shrunk (can only be freed by MelderString_free)
} MelderString;
typedef struct {
	unsigned long length;
	unsigned long bufferSize;
	MelderUtf16 *string;   // a growing buffer, never shrunk (can only be freed by MelderString16_free)
} MelderString16;

void MelderString_free (MelderString *me);   // frees the "string" attribute only (and sets other attributes to zero)
void MelderString16_free (MelderString16 *me);   // frees the "string" attribute only (and sets other attributes to zero)
void MelderString_empty (MelderString *me);   // sets to empty string (buffer not freed)
void MelderString16_empty (MelderString16 *me);   // sets to empty string (buffer not freed)
void MelderString_copy (MelderString *me, const wchar *source);
void MelderString_ncopy (MelderString *me, const wchar *source, unsigned long n);
void MelderString_append (MelderString *me, const wchar *s1);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4,
	const wchar *s5);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4,
	const wchar *s5, const wchar *s6);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4,
	const wchar *s5, const wchar *s6, const wchar *s7);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4,
	const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void MelderString_append (MelderString *me, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4,
	const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
#define MelderString_append1 MelderString_append
#define MelderString_append2 MelderString_append
#define MelderString_append3 MelderString_append
#define MelderString_append4 MelderString_append
#define MelderString_append5 MelderString_append
#define MelderString_append6 MelderString_append
#define MelderString_append7 MelderString_append
#define MelderString_append8 MelderString_append
#define MelderString_append9 MelderString_append
void MelderString_appendCharacter (MelderString *me, wchar character);
void MelderString16_appendCharacter (MelderString16 *me, wchar character);
void MelderString_get (MelderString *me, wchar *destination);   // performs no boundary checking
double MelderString_allocationCount (void);
double MelderString_deallocationCount (void);
double MelderString_allocationSize (void);
double MelderString_deallocationSize (void);

struct structMelderReadText {
	wchar *stringW, *readPointerW;
	char *string8, *readPointer8;
	unsigned long input8Encoding;
};
typedef struct structMelderReadText *MelderReadText;

MelderReadText MelderReadText_createFromFile (MelderFile file);
MelderReadText MelderReadText_createFromString (const wchar *string);
wchar MelderReadText_getChar (MelderReadText text);
wchar * MelderReadText_readLine (MelderReadText text);
long MelderReadText_getNumberOfLines (MelderReadText me);
const wchar * MelderReadText_getLineNumber (MelderReadText text);
void MelderReadText_delete (MelderReadText text);

const wchar * Melder_wcscat (const wchar *s1, const wchar *s2);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
const wchar * Melder_wcscat (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
#define Melder_wcscat2 Melder_wcscat
#define Melder_wcscat3 Melder_wcscat
#define Melder_wcscat4 Melder_wcscat
#define Melder_wcscat5 Melder_wcscat
#define Melder_wcscat6 Melder_wcscat
#define Melder_wcscat7 Melder_wcscat
#define Melder_wcscat8 Melder_wcscat
#define Melder_wcscat9 Melder_wcscat

/********** NUMBER AND STRING COMPARISON **********/

int Melder_numberMatchesCriterion (double value, int which_kMelder_number, double criterion);
int Melder_stringMatchesCriterion (const wchar *value, int which_kMelder_string, const wchar *criterion);

/********** STRING PARSING **********/

/*
	These functions regard a string as a sequence of tokens,
	separated (and perhaps preceded and followed) by white space.
	The tokens cannot contain spaces themselves (there are no escapes).
	Typical use:
		for (token = Melder_firstToken (string); token != NULL; token = Melder_nextToken ()) {
			... do something with the token ...
		}
*/

long Melder_countTokens (const wchar *string);
wchar *Melder_firstToken (const wchar *string);
wchar *Melder_nextToken (void);
wchar ** Melder_getTokens (const wchar *string, long *n);
void Melder_freeTokens (wchar ***tokens);
long Melder_searchToken (const wchar *string, wchar **tokens, long n);

/********** MESSAGING ROUTINES **********/

/* These functions are called like printf ().
	Default Melder does fprintf to stderr,
	except Melder_information, which does fprintf to stdout.
	These functions show generic, native, and mixed strings correctly,
	and perform quote conversion, if that flag is not off; see under "NON-ASCII CHARACTERS".
	The alphabet is Roman, so that symbols from the Symbol and Phonetic alphabets
	are not translated (by default, \mu is shown as \mu and \as as a).
*/

void Melder_casual (const char *format, ...);
void Melder_casual1 (const wchar *s1);
void Melder_casual2 (const wchar *s1, const wchar *s2);
void Melder_casual3 (const wchar *s1, const wchar *s2, const wchar *s3);
void Melder_casual4 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void Melder_casual5 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void Melder_casual6 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void Melder_casual7 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void Melder_casual8 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void Melder_casual9 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
/*
	Function:
		Sends a message without user interference.
	Behaviour:
		Writes to stderr on Unix, otherwise to a special window.
*/

/* Give information to stdout (batch), or to an "Info" window (interactive), or to a diverted string. */

void MelderInfo_open (void);   /* Clear the Info window in the background. */
void MelderInfo_write1 (const wchar *s1);   /* Write a string to the Info window in the background. */
void MelderInfo_write2 (const wchar *s1, const wchar *s2);   /* Write two strings to the Info window in the background. */
void MelderInfo_write3 (const wchar *s1, const wchar *s2, const wchar *s3);
void MelderInfo_write4 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void MelderInfo_write5 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void MelderInfo_write6 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void MelderInfo_write7 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void MelderInfo_write8 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void MelderInfo_write9 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
void MelderInfo_writeLine1 (const wchar *s1);   /* Write a string to the Info window in the background; add a new-line. */
void MelderInfo_writeLine2 (const wchar *s1, const wchar *s2);
void MelderInfo_writeLine3 (const wchar *s1, const wchar *s2, const wchar *s3);
void MelderInfo_writeLine4 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void MelderInfo_writeLine5 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void MelderInfo_writeLine6 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void MelderInfo_writeLine7 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void MelderInfo_writeLine8 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void MelderInfo_writeLine9 (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
void MelderInfo_close (void);   /* Flush the background info to the Info window. */

void Melder_information (const wchar *s1);
void Melder_information (const wchar *s1, const wchar *s2);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void Melder_information (const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
#define Melder_information1 Melder_information
#define Melder_information2 Melder_information
#define Melder_information3 Melder_information
#define Melder_information4 Melder_information
#define Melder_information5 Melder_information
#define Melder_information6 Melder_information
#define Melder_information7 Melder_information
#define Melder_information8 Melder_information
#define Melder_information9 Melder_information

void Melder_informationReal (double value, const wchar *units);   /* %.17g or --undefined--; units may be NULL */

void Melder_divertInfo (MelderString *buffer);   /* NULL = back to normal. */

void Melder_print (const wchar *s);
	/* Write formatted text to the Info window without clearing it, and without adding a new-line symbol at the end. */

void Melder_clearInfo (void);   /* Clear the Info window. */
const wchar * Melder_getInfo (void);
void Melder_help (const wchar *query);

void Melder_search (void);
	
void Melder_beep (void);

extern int Melder_debug;

/* The following trick uses Melder_debug only because it is the only plain variable known to exist at the moment. */
#define Melder_offsetof(klas,member) (char *) & ((klas) & Melder_debug) -> member - (char *) & Melder_debug

/********** ERROR **********/

class MelderError { };

typedef class structThing *Thing;
wchar *Thing_messageName (Thing me);
struct MelderArg {
	int type;
	union {
		const wchar *argW;
		const char *arg8;
	};
	MelderArg (const wchar *        arg) : type (1), argW (arg) { }
	MelderArg (const  char *        arg) : type (2), arg8 (arg) { }
	MelderArg (const double         arg) : type (1), argW (Melder_double          (arg)) { }
	MelderArg (const          long  arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (const unsigned long  arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (const          int   arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (const unsigned int   arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (const          short arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (const unsigned short arg) : type (1), argW (Melder_integer         (arg)) { }
	MelderArg (Thing                arg) : type (1), argW (Thing_messageName      (arg)) { }
	MelderArg (MelderFile           arg) : type (1), argW (MelderFile_messageName (arg)) { }
};
void Melder_throw (const MelderArg& arg1);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11);
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13 = L"");
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5,
	const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9, const MelderArg& arg10,
	const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15 = L"");
void Melder_throw (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12,
	const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15, const MelderArg& arg16,
	const MelderArg& arg17 = L"", const MelderArg& arg18 = L"", const MelderArg& arg19 = L"", const MelderArg& arg20 = L"");
void Melder_error_ (const MelderArg& arg1);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11);
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13 = L"");
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5,
	const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9, const MelderArg& arg10,
	const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15 = L"");
void Melder_error_ (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12,
	const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15, const MelderArg& arg16,
	const MelderArg& arg17 = L"", const MelderArg& arg18 = L"", const MelderArg& arg19 = L"", const MelderArg& arg20 = L"");
#define therror  if (Melder_hasError ()) { Melder_error_ ("PLEASE SEND THIS ENTIRE ERROR MESSAGE TO PAUL.BOERSMA@UVA.NL (file ", __FILE__, ", line ", __LINE__, ")."); throw MelderError (); }
#define Melder_throw(...)  do { Melder_error_ (__VA_ARGS__); throw MelderError (); } while (false)

void Melder_flushError (const char *format, ...);
	/* Send all deferred error messages to stderr (batch) or to an "Error" dialog, */
	/* including, if 'format' is not NULL, the error message generated by this routine. */

bool Melder_hasError ();
bool Melder_hasError (const wchar *partialError);
	/* Returns 1 if there is an error message in store, otherwise 0. */

void Melder_clearError (void);
	/* Cancel all stored error messages. */

wchar * Melder_getError (void);
	/* Returns the error string. Mainly used with wcsstr. */

int Melder_fatal (const char *format, ...);
	/* Give error message, abort program. */
	/* Should only be caused by programming errors. */

#ifdef NDEBUG
	#define Melder_assert(x)   ((void) 0)
#else
	#define Melder_assert(x)   ((x) ? (void) (0) : (void) _Melder_assert (#x, __FILE__, __LINE__))
#endif
int _Melder_assert (const char *condition, const char *fileName, int lineNumber);
	/* Call Melder_fatal with a message based on the following template: */
	/*    "Assertion failed in file <fileName> on line <lineNumber>: <condition>" */

/********** WARNING: ive warning to stderr (batch) or to a "Warning" dialog **********/

void Melder_warning (const MelderArg& arg1);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11);
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13 = L"");
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4, const MelderArg& arg5,
	const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8, const MelderArg& arg9, const MelderArg& arg10,
	const MelderArg& arg11, const MelderArg& arg12, const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15 = L"");
void Melder_warning (const MelderArg& arg1, const MelderArg& arg2, const MelderArg& arg3, const MelderArg& arg4,
	const MelderArg& arg5, const MelderArg& arg6, const MelderArg& arg7, const MelderArg& arg8,
	const MelderArg& arg9, const MelderArg& arg10, const MelderArg& arg11, const MelderArg& arg12,
	const MelderArg& arg13, const MelderArg& arg14, const MelderArg& arg15, const MelderArg& arg16,
	const MelderArg& arg17 = L"", const MelderArg& arg18 = L"", const MelderArg& arg19 = L"", const MelderArg& arg20 = L"");
#define Melder_warning1 Melder_warning
#define Melder_warning2 Melder_warning
#define Melder_warning3 Melder_warning
#define Melder_warning4 Melder_warning
#define Melder_warning5 Melder_warning
#define Melder_warning6 Melder_warning
#define Melder_warning7 Melder_warning
#define Melder_warning8 Melder_warning
#define Melder_warning9 Melder_warning

void Melder_warningOff (void);
void Melder_warningOn (void);

class autoMelderWarningOff {
public:
	autoMelderWarningOff () { Melder_warningOff (); }
	~autoMelderWarningOff () { Melder_warningOn (); }
};

/********** PROGRESS ROUTINES **********/

void Melder_progress (double progress, const wchar *s1);
void Melder_progress (double progress, const wchar *s1, const wchar *s2);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void Melder_progress (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
void Melder_progressOff (void);
void Melder_progressOn (void);
#define Melder_progress1 Melder_progress
#define Melder_progress2 Melder_progress
#define Melder_progress3 Melder_progress
#define Melder_progress4 Melder_progress
#define Melder_progress5 Melder_progress
#define Melder_progress6 Melder_progress
#define Melder_progress7 Melder_progress
#define Melder_progress8 Melder_progress
#define Melder_progress9 Melder_progress
/*
	Function:
		Show the progress of a time-consuming process.
	Arguments:
		Any of 's1' through 's9' may be NULL.
	Batch behaviour:
		Does nothing, always returns 1.
	Interactive behaviour:
		Shows the progress of a time-consuming process:
		- if 'progress' <= 0.0, show a window with text and a Cancel button, and return 1;
		- if 0.0 < 'progress' < 1.0, show text and a partially filled progress bar,
		  and return 0 if user interrupts, else return 1;
		- if 'progress' >= 1, hide the window.
	Usage:
		- call with 'progress' = 0.0 before the process starts:
		      (void) Melder_progress (0.0, "Starting work...");
		- at every turn in your loop, call with 'progress' between 0 and 1:
		      Melder_progress (i / (n + 1.0), L"Working on part ", i, L" out of ", n, L"...");
		  an exception is thrown if the user clicks Cancel; if you don't want that, catch it:
		      try {
		          Melder_progress (i / (n + 1.0), L"Working on part ", i, L" out of ", n, L"...");
		      } catch (MelderError) {
		          Melder_clearError ();
		          break;
		      }
		- after the process has finished, call with 'progress' = 1.0:
		      (void) Melder_progress (1.0, NULL);
		- the first and third steps can be automated by autoMelderProgress:
		      autoMelderProgress progress ("Starting work...");
*/
class autoMelderProgress {
public:
	autoMelderProgress (const wchar *message) {
		Melder_progress (0.0, message);
	}
	~autoMelderProgress () {
		Melder_progress (1.0, NULL);
	}
};

void * Melder_monitor (double progress, const wchar *s1);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8);
void * Melder_monitor (double progress, const wchar *s1, const wchar *s2, const wchar *s3, const wchar *s4, const wchar *s5, const wchar *s6, const wchar *s7, const wchar *s8, const wchar *s9);
#define Melder_monitor1 Melder_monitor
#define Melder_monitor2 Melder_monitor
#define Melder_monitor3 Melder_monitor
#define Melder_monitor4 Melder_monitor
#define Melder_monitor5 Melder_monitor
#define Melder_monitor6 Melder_monitor
#define Melder_monitor7 Melder_monitor
#define Melder_monitor8 Melder_monitor
#define Melder_monitor9 Melder_monitor
/*
	Function:
		Show the progress of a time-consuming process.
	Arguments:
		Any of 's1' through 's9' may be NULL.
	Batch behaviour:
		Does nothing, returns NULL if 'progress' <= 0.0 and a non-NULL pointer otherwise.
	Interactive behaviour:
		Shows the progress of a time-consuming process:
		- if 'progress' <= 0.0, show a window with text and a Cancel button and
		  room for a square drawing, and return a Graphics;
		- if 0.0 < 'progress' < 1.0, show text and a partially filled progress bar,
		  and return NULL if user interrupts, else return a non-NULL pointer;
		- if 'progress' >= 1, hide the window.
	Usage:
		- call with 'progress' = 0.0 before the process starts.
		- assign the return value to a Graphics:
		      Graphics graphics = Melder_monitor (0.0, L"Starting work...");
		- at every turn of your loop, draw something in the Graphics:
		      if (graphics) {   // always check; might be batch
		          Graphics_clearWs (graphics);   // only if you redraw all every time
		          Graphics_polyline (graphics, ...);
		          Graphics_text (graphics, ...);
		      }
		- immediately after this in your loop, call with 'progress' between 0 and 1:
		      Melder_monitor (i / (n + 1.0), L"Working on part ", Melder_integer (i), L" out of ", Melder_integer (n), L"...");
		  an exception is thrown if the user clicks Cancel; if you don't want that, catch it:
		      try {
		          Melder_monitor (i / (n + 1.0), L"Working on part ", Melder_integer (i), L" out of ", Melder_integer (n), L"...");
		      } catch (MelderError) {
		          Melder_clearError ();
		          break;
		      }
		- after the process has finished, call with 'progress' = 1.0:
		      (void) Melder_monitor (1.0, NULL);
		- the first and third steps can be automated by autoMelderMonitor:
		      autoMelderMonitor monitor ("Starting work...");
		      if (monitor.graphics()) {   // always check; might be batch
		          Graphics_clearWs (monitor.graphics());   // only if you redraw all every time
		          Graphics_polyline (monitor.graphics(), ...);
		          Graphics_text (monitor.graphics(), ...);
		      }
*/
typedef class structGraphics *Graphics;
class autoMelderMonitor {
	Graphics d_graphics;
public:
	autoMelderMonitor (const wchar *message) {
		d_graphics = (Graphics) Melder_monitor (0.0, message);
	}
	~autoMelderMonitor () {
		Melder_monitor1 (1.0, NULL);
	}
	Graphics graphics () { return d_graphics; }
};

/********** RECORD AND PLAY ROUTINES **********/

int Melder_publish (void *anything);

int Melder_record (double duration);
int Melder_recordFromFile (MelderFile fs);
void Melder_play (void);
void Melder_playReverse (void);
int Melder_publishPlayed (void);

/********** SYSTEM VERSION **********/

extern unsigned long Melder_systemVersion;
/*
	For Macintosh, this is set in the Motif emulator.
*/

/********** SCRATCH TEXT BUFFERS **********/

extern char Melder_buffer1 [30001], Melder_buffer2 [30001];
/*
	Every Melder routine uses both of these buffers:
	one for sprintfing the message,
	and one for translating this message to a native string.
	You can use these buffers,
	but be careful not to call any other routines that use them at the same time;
	the following routines are guaranteed not to call the Melder library:
	 - Mac Toolbox, XWindows, X Toolkit, Motif, and XVT routines,
		except those who dispatch events (XtDispatchEvent, xvt_process_pending_events).
	 - Longchar_*
	This means that you can use these buffers for reading and writing with
	the Longchar library.
*/

/********** ENFORCE INTERACTIVE BEHAVIOUR **********/

/* Procedures to enforce interactive behaviour of the Melder_XXXXXX routines. */

void MelderGui_create (/* GuiObject */ void *parent);
/*
	'parent' is the top-level widget returned by GuiAppInitialize.
*/

extern bool Melder_batch;   // true if run from the batch or from an interactive command-line interface
extern bool Melder_backgrounding;   /* True if running a script. */
extern bool Melder_consoleIsAnsi;
#ifndef CONTROL_APPLICATION
	extern void *Melder_topShell;   // GuiObject
#endif

/********** OVERRIDE DEFAULT BEHAVIOUR **********/

/* Procedures to override default message methods. */
/* They may chage the string arguments. */
/* Many of these routines are called by MelderMotif_create and MelderXvt_create. */

void Melder_setCasualProc (void (*casualProc) (const wchar *message));
void Melder_setProgressProc (int (*progressProc) (double progress, const wchar *message));
void Melder_setMonitorProc (void * (*monitorProc) (double progress, const wchar *message));
void Melder_setInformationProc (void (*informationProc) (const wchar *message));
void Melder_setHelpProc (void (*help) (const wchar *query));
void Melder_setSearchProc (void (*search) (void));
void Melder_setWarningProc (void (*warningProc) (const wchar *message));
void Melder_setErrorProc (void (*errorProc) (const wchar *message));
void Melder_setFatalProc (void (*fatalProc) (const wchar *message));
void Melder_setPublishProc (int (*publish) (void *));
void Melder_setRecordProc (int (*record) (double));
void Melder_setRecordFromFileProc (int (*recordFromFile) (MelderFile));
void Melder_setPlayProc (void (*play) (void));
void Melder_setPlayReverseProc (void (*playReverse) (void));
void Melder_setPublishPlayedProc (int (*publishPlayed) (void));

double Melder_stopwatch (void);

long Melder_killReturns_inline (char *text);
/*
	 Replaces all bare returns (Mac) or return / linefeed sequences (Win) with bare linefeeds (generic = Unix).
	 Returns new length of string (equal to or less than old length).
*/

/********** AUDIO **********/

#if defined (macintosh) || defined (_WIN32) || defined (linux)
	#define kMelderAudio_inputUsesPortAudio_DEFAULT  true
		// Mac: in order to have CoreAudio (so that rogue applications cannot attack our sample rate anymore)
		// Win: in order to allow recording for over 64 megabytes (paMME)
		// Linux: in order to use ALSA and therefore be compatible with Ubuntu 10.10 and later
#else
	#define kMelderAudio_inputUsesPortAudio_DEFAULT  false
#endif
void MelderAudio_setInputUsesPortAudio (bool inputUsesPortAudio);
bool MelderAudio_getInputUsesPortAudio (void);
#if defined (macintosh) || defined (linux)
	#define kMelderAudio_outputUsesPortAudio_DEFAULT  true
		// Mac: in order to have CoreAudio (so that rogue applications cannot attack our sample rate anymore)
		// Linux: in order to use ALSA and therefore be compatible with Ubuntu 10.10 and later
#else
	#define kMelderAudio_outputUsesPortAudio_DEFAULT  false
		// Win: in order to reduce the long latencies of paMME and to avoid the incomplete implementation of paDirectSound
#endif
void MelderAudio_setOutputUsesPortAudio (bool outputUsesPortAudio);
bool MelderAudio_getOutputUsesPortAudio (void);
#if 1
	#define kMelderAudio_outputSilenceBefore_DEFAULT  0.0
		// Mac: in order to switch off the BOING caused by the automatic gain control
#endif
void MelderAudio_setOutputSilenceBefore (double silenceBefore);
double MelderAudio_getOutputSilenceBefore (void);
#if defined (macintosh)
	#define kMelderAudio_outputSilenceAfter_DEFAULT  0.0
		// Mac: in order to reduce the BOING caused by the automatic gain control when the user replays immediately after a sound has finished
#else
	#define kMelderAudio_outputSilenceAfter_DEFAULT  0.5
		// Win: in order to get rid of the click on some cards
		// Linux: in order to get rid of double playing of a sounding buffer
#endif
void MelderAudio_setOutputSilenceAfter (double silenceAfter);
double MelderAudio_getOutputSilenceAfter (void);
void MelderAudio_setOutputUsesBlocking (bool outputUsesBlocking);
bool MelderAudio_getOutputUsesBlocking (void);
void MelderAudio_setUseInternalSpeaker (bool useInternalSpeaker);   // for HP-UX and Sun
bool MelderAudio_getUseInternalSpeaker (void);
void MelderAudio_setOutputMaximumAsynchronicity (enum kMelder_asynchronicityLevel maximumAsynchronicity);
enum kMelder_asynchronicityLevel MelderAudio_getOutputMaximumAsynchronicity (void);
long MelderAudio_getOutputBestSampleRate (long fsamp);

extern bool MelderAudio_isPlaying;
void MelderAudio_play16 (const int16_t *buffer, long sampleRate, long numberOfSamples, int numberOfChannels,
	bool (*playCallback) (void *playClosure, long numberOfSamplesPlayed),   // return true to continue, false to stop
	void *playClosure);
bool MelderAudio_stopPlaying (bool isExplicit);   // returns true if sound was playing
#define MelderAudio_IMPLICIT  false
#define MelderAudio_EXPLICIT  true
long MelderAudio_getSamplesPlayed (void);
bool MelderAudio_stopWasExplicit (void);

void Melder_audio_prefs (void);   // in init file

/********** AUDIO FILES **********/

/* Audio file types. */
#define Melder_AIFF  1
#define Melder_AIFC  2
#define Melder_WAV  3
#define Melder_NEXT_SUN  4
#define Melder_NIST  5
#define Melder_SOUND_DESIGNER_TWO  6
#define Melder_FLAC 7
#define Melder_MP3 8
#define Melder_NUMBER_OF_AUDIO_FILE_TYPES  8
const wchar * Melder_audioFileTypeString (int audioFileType);   /* "AIFF", "AIFC", "WAV", "NeXT/Sun", "NIST", "Sound Designer II", "FLAC", "MP3" */
const wchar * Melder_macAudioFileType (int audioFileType);   /* "AIFF", "AIFC", "WAVE", "ULAW", "NIST", "Sd2f", "FLAC", "MP3" */
const wchar * Melder_winAudioFileExtension (int audioFileType);   /* ".aiff", ".aifc", ".wav", ".au", ".nist", ".sd2", ".flac", ".mp3" */
/* Audio encodings. */
#define Melder_LINEAR_8_SIGNED  1
#define Melder_LINEAR_8_UNSIGNED  2
#define Melder_LINEAR_16_BIG_ENDIAN  3
#define Melder_LINEAR_16_LITTLE_ENDIAN  4
#define Melder_LINEAR_24_BIG_ENDIAN  5
#define Melder_LINEAR_24_LITTLE_ENDIAN  6
#define Melder_LINEAR_32_BIG_ENDIAN  7
#define Melder_LINEAR_32_LITTLE_ENDIAN  8
#define Melder_MULAW  9
#define Melder_ALAW  10
#define Melder_SHORTEN  11
#define Melder_POLYPHONE  12
#define Melder_IEEE_FLOAT_32_BIG_ENDIAN  13
#define Melder_IEEE_FLOAT_32_LITTLE_ENDIAN  14
#define Melder_FLAC_COMPRESSION 15
#define Melder_MPEG_COMPRESSION 16
int Melder_defaultAudioFileEncoding16 (int audioFileType);   /* BIG_ENDIAN, BIG_ENDIAN, LITTLE_ENDIAN, BIG_ENDIAN, LITTLE_ENDIAN, BIG_ENDIAN */
void MelderFile_writeAudioFileHeader16 (MelderFile file, int audioFileType, long sampleRate, long numberOfSamples, int numberOfChannels);
void MelderFile_writeAudioFile16 (MelderFile file, int audioFileType, const short *buffer, long sampleRate, long numberOfSamples, int numberOfChannels);

int MelderFile_checkSoundFile (MelderFile file, int *numberOfChannels, int *encoding,
	double *sampleRate, long *startOfData, long *numberOfSamples);
/* Returns information about a just opened audio file.
 * The return value is the audio file type, or 0 if it is not a sound file or in case of error.
 * The data start at 'startOfData' bytes from the start of the file.
 */
int Melder_bytesPerSamplePoint (int encoding);
void Melder_readAudioToFloat (FILE *f, int numberOfChannels, int encoding, double **buffer, long numberOfSamples);
/* Reads channels into buffer [ichannel], which are base-1.
 */
void Melder_readAudioToShort (FILE *f, int numberOfChannels, int encoding, short *buffer, long numberOfSamples);
/* If stereo, buffer will contain alternating left and right values.
 * Buffer is base-0.
 */
void MelderFile_writeFloatToAudio (MelderFile file, int numberOfChannels, int encoding, double **buffer, long numberOfSamples, int warnIfClipped);
void MelderFile_writeShortToAudio (MelderFile file, int numberOfChannels, int encoding, const short *buffer, long numberOfSamples);

void Melder_audioTrigger (void);

/********** QUANTITY **********/

#define MelderQuantity_NONE  0
#define MelderQuantity_TIME_SECONDS  1
#define MelderQuantity_FREQUENCY_HERTZ  2
#define MelderQuantity_FREQUENCY_BARK  3
#define MelderQuantity_DISTANCE_FROM_GLOTTIS_METRES  4
#define MelderQuantity_NUMBER_OF_QUANTITIES  4
const wchar * MelderQuantity_getText (int quantity);   // e.g. "Time"
const wchar * MelderQuantity_getWithUnitText (int quantity);   // e.g. "Time (s)"
const wchar * MelderQuantity_getLongUnitText (int quantity);   // e.g. "seconds"
const wchar * MelderQuantity_getShortUnitText (int quantity);   // e.g. "s"

/********** MISCELLANEOUS **********/

wchar * Melder_getenv (const wchar *variableName);
void Melder_system (const wchar *command);   // spawn a system command
double Melder_clock (void);   // seconds since 1969

struct autoMelderProgressOff {
	autoMelderProgressOff () { Melder_progressOff (); }
	~autoMelderProgressOff () { Melder_progressOn (); }
};

struct autoMelderString : MelderString {
	autoMelderString () { length = 0; bufferSize = 0; string = NULL; }
	~autoMelderString () { Melder_free (string); }
};

struct autoMelderReadText {
	MelderReadText text;
	autoMelderReadText (MelderReadText a_text) : text (a_text) {
		therror;
	}
	~autoMelderReadText () {
		if (text) MelderReadText_delete (text);
	}
	MelderReadText operator-> () const {   // as r-value
		return text;
	}
	MelderReadText peek () const {
		return text;
	}
	MelderReadText transfer () {
		MelderReadText tmp = text;
		text = NULL;
		return tmp;
	}
};

class autofile {
	FILE *ptr;
public:
	autofile (FILE *f) : ptr (f) {
		therror;
	}
	autofile () : ptr (NULL) {
	}
	~autofile () {
		if (ptr) fclose (ptr);   // no error checking, because this is a destructor, only called after a throw, because otherwise you'd use f.close(file)
	}
	operator FILE * () {
		return ptr;
	}
	void reset (FILE *f) {
		if (ptr) fclose (ptr);   // BUG: not a normal closure
		ptr = f;
		therror;
	}
	void close (MelderFile file) {
		if (ptr) {
			FILE *tmp = ptr;
			ptr = NULL;
			Melder_fclose (file, tmp); therror
		}
	}
};

class autoMelderFile {
	MelderFile file;
public:
	autoMelderFile (MelderFile a_file) : file (a_file) {
		therror;
	}
	~autoMelderFile () {
		if (file) MelderFile_close_nothrow (file);
	}
	void close () {
		if (file && file -> filePointer) {
			MelderFile tmp = file;
			file = NULL;
			MelderFile_close (tmp); therror
		}
	}
	MelderFile transfer () {
		MelderFile tmp = file;
		file = NULL;
		return tmp;
	}
};

class autoMelderSaveDefaultDir {
	structMelderDir saveDir;
public:
	autoMelderSaveDefaultDir () {
		Melder_getDefaultDir (& saveDir);
	}
	~autoMelderSaveDefaultDir () {
		Melder_setDefaultDir (& saveDir);
	}
};

class autoMelderSetDefaultDir {
	structMelderDir saveDir;
public:
	autoMelderSetDefaultDir (MelderDir dir) {
		Melder_getDefaultDir (& saveDir);
		Melder_setDefaultDir (dir);
	}
	~autoMelderSetDefaultDir () {
		Melder_setDefaultDir (& saveDir);
	}
};

class autoMelderFileSetDefaultDir {
	structMelderDir saveDir;
public:
	autoMelderFileSetDefaultDir (MelderFile file) {
		Melder_getDefaultDir (& saveDir);
		MelderFile_setDefaultDir (file);
	}
	~autoMelderFileSetDefaultDir () {
		Melder_setDefaultDir (& saveDir);
	}
};

class autoMelderTokens {
	wchar **tokens;
public:
	autoMelderTokens () {
		tokens = NULL;
	}
	autoMelderTokens (const wchar *string, long *n) {
		tokens = Melder_getTokens (string, n); therror
	}
	~autoMelderTokens () {
		if (tokens) Melder_freeTokens (& tokens);
	}
	wchar*& operator[] (long i) {
		return tokens [i];
	}
	wchar ** peek () const {
		return tokens;
	}
	void reset (const wchar *string, long *n) {
		if (tokens) Melder_freeTokens (& tokens);
		tokens = Melder_getTokens (string, n); therror
	}
};

template <class T>
class _autostring {
	T *ptr;
public:
	_autostring (T *string) : ptr (string) {
		//if (Melder_debug == 39) Melder_casual ("autostring: entering constructor from C-string %ld", ptr);
		therror;
		//if (Melder_debug == 39) Melder_casual ("autostring: leaving constructor from C-string");
	}
	_autostring () : ptr (0) {
		//if (Melder_debug == 39) Melder_casual ("autostring: zero constructor");
	}
	~_autostring () {
		//if (Melder_debug == 39) Melder_casual ("autostring: entering destructor ptr = %ld", ptr);
		if (ptr) Melder_free (ptr);
		//if (Melder_debug == 39) Melder_casual ("autostring: leaving destructor");
	}
	#if 0
	void operator= (T *string) {
		//if (Melder_debug == 39) Melder_casual ("autostring: entering assignment from C-string; old = %ld", ptr);
		if (ptr) Melder_free (ptr);
		ptr = string;
		therror;
		//if (Melder_debug == 39) Melder_casual ("autostring: leaving assignment from C-string; new = %ld", ptr);
	}
	#endif
	T& operator[] (long i) {
		return ptr [i];
	}
	T * peek () const {
		return ptr;
	}
	T ** operator& () {
		return & ptr;
	}
	T * transfer () {
		T *tmp = ptr;
		ptr = NULL;
		return tmp;
	}
	void reset (T *string) {
		if (ptr) Melder_free (ptr);
		ptr = string;
		therror;
	}
	void resize (long new_size) {
		T *tmp = (T *) Melder_realloc (ptr, new_size * sizeof (T));
		ptr = tmp;
	}
private:
	_autostring& operator= (const _autostring&);   // disable copy assignment
	//autostring (autostring &);   // disable copy constructor (trying it this way also disables good things like autostring s1 = wcsdup("hello");)
	template <class Y> _autostring (_autostring<Y> &);   // disable copy constructor
};

typedef _autostring <wchar> autostring;
typedef _autostring <char> autostring8;

/* End of file melder.h */
#endif
