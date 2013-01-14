#ifndef _ASINCLUDE_H_#define _ASINCLUDE_H_/*******************************************************************  File Name : asinclude.h**//* extra definitions (RTB) *//* include this for defn of MFILE */#include "macio.h"extern int include_stack_depth;/* must record not only info about the open file but info about the name of *//* the file and the last line number that was being processed when the input *//* was diverted by the include */typedef struct {	struct strdesc *name_ptr;	/* pointer into the string file which returns the filename */	MFILE *f;				/* file info */	long lineno;					/* the current line number */} INCLUDE_ENTRY;extern INCLUDE_ENTRY include_file[];#define True   -1#define False   0/* In use in asscan2.c and asparse.c *//* Max Index of the last possible .file and file name instruction */#define	IndxMaxIFN			100   /* written in two places */#define	SizeFileNameVI		64		/* Size of the file name stored inside */											/* VectInclFileName,  */#define	CmplDir_file	".file "#define	CmplDir_line	".line "/*******************************************************************/void  PrintIdFile(  M_FILE	*First);/*******************************************************************  Replace all the include instructions with the content of the*  pointed file. *  Return true if everythings are ok, or false if there is a*  problem*/Boolean  IntegrateTheInclude(  M_FILE  *IdCmplFile,       M_FILE  *IdOrgFile,  char    *NameCmplFile);/*******************************************************************	Write the File Name and Line Label inside the given file.*  FLL : File and Line Label.*  !! Load the global vector of .file name.*	*	M_FILE	*IdGivFile,    (given file to write in)*	char		*FNameLabel,	(given file name)*	int		FLineLabel		(given line label)*/void	WriteFNLL(	M_FILE	*IdGivFile,	char		*FNameLabel,	int		FLineLabel	);/*******************************************************************  Effect : **  - If the string is a complete comment (no instruction on*    the line)  by itself, the function will be False and no *    string will be returned.*  - If the string has a part of comment and a part of*    instruction, the function will be true and a string*    holding the instruction part will be returned.*  *	CurPS : Current purged string from any comment, returned.**  InsideComOn : True if we are inside a comment, in relation*			with the position of the character index of the string, for*			the outside, keep the value it had on the last character.**  InsideQuoOn : Idem than for comment but don't influence the result*  		of the function except for itself, in use for outside to detect*			an improbable include inside some quotation marks.** 	TouchQuo : True if we touched the InsideQuoOn on the line.**  CurOS : Current original string.**/Boolean  PartOutsideCom(	char  	*CurPS,	Boolean 	*InsideComOn,	Boolean 	*InsideQuoOn,	Boolean  *TouchQuo,	char    	*CurOS);/******************************************************************* True if a real include directive has been found inside. * We still have to care about the fact to be perhaps inside* some quotation mark.*   NewFName   : File name given with the include instruction *   CurPS   	: Current purged string free of any comment*	 QuotaOn		: In or out any quotation, to be able to eliminate*					  an include inside some ""*/Boolean  FoundIncludeLine(  char  *NewFName,  char  *CurPS,  Boolean *QuotaOn);/******************************************************************* True if the string2 is include inside the string1 and* give back the starting position of the substring.* Not working if s2 is NULL.*/Boolean  GiveStartSubS(	int   *StartSS,	char	*String1,	char 	*String2);#endif _ASINCLUDE_H_