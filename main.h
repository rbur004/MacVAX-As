#ifndef __MAIN_H_#define __MAIN_H_#define TRUE -1#define FALSE 0/* * Resource ID constants. */#define	appleMenuid		128			 #define	appleMenu 		0			/* MyMenus[] array indexes */#define	aboutMeCommand	1#define	fileMenuid		129#define	fileMenu 		1 #define openCommand		1/*-*/#define saveCommand		3#define Clear_All		4/*-*/#define	quitCommand 	6#define	editMenuid		130#define	editMenu		2#define	undoCommand 	1/*-*/#define	cutCommand		3#define	copyCommand 	4#define	pasteCommand	5#define TransferMenuid		131#define TransferMenu		3#define	Simulator		 	1#define	Edit				2/*-*/#define Other				4#define menuCount		4extern short menuids[];/* * For the About... DLOG */#define	aboutMeDLOG		128#define	okButton		1#define	authorItem		2			/* For SetIText */#define	languageItem	3			/* For SetIText *//* * C programs typically use macros for simple expressions which * must be function calls in Pascal.  Here are a couple of examples: *//* * Inline SetRect() macro, efficient when (rectp) is a constant. * Must not be used if (rectp) has side effects. * * We could do an InsetRect() macro in a similar vein. */#define SETRECT(rectp, _left, _top, _right, _bottom)	\	(rectp)->left = (_left), (rectp)->top = (_top), 	\	(rectp)->right = (_right), (rectp)->bottom = (_bottom)/* * HIWORD and LOWORD macros, for readability. */#define HIWORD(aLong)		(((aLong) >> 16) & 0xFFFF)#define LOWORD(aLong)		((aLong) & 0xFFFF)void setupMenus(void);void doCommand(	long	mResult);#endif