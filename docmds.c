#include	<types.h>#include	<quickdraw.h>#include	<windows.h>#include	<dialogs.h>#include	<controls.h>#include	<toolutils.h>#include	<menus.h>#include 	<Files.h>#include 	<packages.h>#include	<segload.h>#include	<textedit.h>#include    <scrap.h>#include	<Events.h>#include	<OSutils.h>#include	<resources.h>#include 	<String.h>#include 	<Strings.h>#include	<memory.h>#define _DOCMDS_C_#include 	"mstdio.h"#include	"iow.h"#include 	"window.h"#include 	"main.h"#include 	"macio.h"#include 	"docmds.h"#include	"launch.h"#include 	"asmain.h"#include	"aprintf.h"#include	"asinclude.h"#pragma segment DOCMDSextern	Boolean FirstTimeIF;   /* To jump the first passage in IFILE, asparse.c *//* To be initialised in this unit, come from asparse.c */extern	long pass1_count; extern	long pass2_count; M_FILE *mopen_local(char *ftype	/* types of files to display*/ );/******************************************************************* Open an existing file via the standard file box*/M_FILE *mopen_local(	char *ftype	/* types of files to display*/){struct Point 			dlgOrigin;			/* dialogbox top left corner*/static SFReply			theReply;			/* Data returned from dialogbox*/short 					num;				/* number of file types*/SFTypeList 				types;				/* file types*/char					*p;	dlgOrigin.h = DLGLEFT;	dlgOrigin.v = DLGTOP;				for (p = (char *) types, num = 0; *ftype && num < 16; num++ )		*p++ = *ftype++;					num /= 4;//	SFGetFile(dlgOrigin,(void *)"", (FileFilterProcPtr) 0, num, types, (DlgHookProcPtr)0, &theReply);	SFGetFile(dlgOrigin,(void *)"", 0, num, types, 0, &theReply);	if(theReply.good)	{			p2cstr(theReply.fName); 					return( mfopen (  (char *)&theReply.fName,  theReply.vRefNum, "r"    ) ) ;	}	return((M_FILE *)0);} /* mopen_local */void file_open(void){	if( (file_info = mopen( "TEXT" )) != 0 )	{			last_file_info = *file_info;		vax_assemble( 1, &file_info ,"this is a dummy file name which I will delete");		mclose(file_info ); 				}}#ifdef laurents_code/*******************************************************************  - Create a temporary file to deal with the include instruction.*  - Assemble this temporary file and obtain a result.*  - delete the temporary file.*/Boolean file_open(	char		*NameComplFile,  	short    *VRefNumReturn){M_FILE	*PtIdOrigFile;M_FILE	IdComplFile;   /* Complete file with all the include integrated  */char		NameOrigFile[64];Boolean	ResultFunct;int LePetitCompt;int PetitCompt;		/* Initialise 2 variable of asparse.c on this level */	/* because need to be done, before each compilation */	pass1_count = 0;	pass2_count = 0;		FirstTimeIF = True;	ResultFunct = False;	if( (PtIdOrigFile = mopen_local ( "TEXT" )) != 0 )	{			IntegrateTheInclude (&IdComplFile,PtIdOrigFile,NameComplFile);					file_info = mopen_na (NameComplFile,PtIdOrigFile->vrefnum);		*VRefNumReturn = PtIdOrigFile->vrefnum;		if  (file_info != NULL)		{		   ResultFunct = True;		   strcpy (NameOrigFile,PtIdOrigFile->name);			last_file_info = *file_info;					 	vax_assemble( 1, &file_info,NameOrigFile); 						if (mclose(file_info ) != 0) 				iowprintf ("docmds.c(file_open)  Can't close file_info (docmds.c)\n");							munlink (NameComplFile,PtIdOrigFile->vrefnum);     /* @@ */					}		else		  iowprintf ("%s %s %s", "Can't open : ", NameComplFile, "\n");		  		     	if  (mfclose (PtIdOrigFile) != 0)			iowprintf ("Can't close IdOrigFile (docmds.c)\n");																		}	} /* file_open */#endifvoid file_save(void){MFILE *file_info;extern TEHandle TextH;	if((file_info = mcreat("File Name","Output",'EDIT','TEXT')) != 0)	{		HLock((*TextH)->hText);		mwrite(file_info->fd,*(*TextH)->hText,(*TextH)->teLength);		HUnlock((*TextH)->hText);		mclose(file_info ); 				}}long got_abort_key(void){EventRecord 			myEvent;	if( GetNextEvent(everyEvent, &myEvent)		&& (myEvent.what ==  keyDown ||  myEvent.what == autoKey)		&& (myEvent.modifiers & cmdKey))	{		if((myEvent.message & charCodeMask) == '.')			return -1;	}	return 0;}void do_assemble(void){}void Link(void){}void exec_other(void){struct mac_args{	short message;	short count;	AppFile argv;} *arglist;Handle args;Handle *lmg = (Handle *)0xaec;struct Point 			dlgOrigin;			/* dialogbox top left corner*/static SFReply			theReply;			/* Data returned from dialogbox*/OSType	types[4];	dlgOrigin.h = DLGLEFT;	dlgOrigin.v = DLGTOP;	types[0] = 'APPL';//	sfgetfile(&dlgOrigin,"", (FileFilterProcPtr) 0, (short)1, types, (DlgHookProcPtr)0,	sfgetfile(&dlgOrigin,"",  0, (short)1, types, 0,			&theReply);				if(theReply.good)	{	p2cstr(theReply.fName);		SetZone(SystemZone());		DisposHandle(*lmg);		args = NewHandle(sizeof(struct mac_args));		HLock(args);		SetZone(ApplicZone());		arglist = (struct mac_args *)*args;		arglist->message = 0;		arglist->count = 0;		*lmg = args;		HUnlock(args);		exec_it((void *)&theReply.fName,theReply.vRefNum);	}}void exec_it(	char *name, /*name to launch + it better be right or boom*/	short vrefnum  /*zero for current directory*/){#ifdef C68Kstruct launch_params appl;OSErr	error;FInfo	finfo;	if(vrefnum)		setvol((char *)0,vrefnum);	if((error = getfinfo(name,(short)0,&finfo)) == noErr)	{		if(finfo.fdType != 'APPL')			aprintf("%s is not an application",name);		else		{			c2pstr(name);			appl.name = name;			appl.altsndbuff = (short)0;			EXEC(&appl);		}	}#else	aprintf("Not supported yet under PPC!");#endif}void exec_simul(void){struct mac_args{	short message;	short count;	AppFile argv;} *arglist;Handle args;Handle *lmg = (Handle *)0xaec;extern short appl_vrefnum;extern long	anyerrs;extern MFILE last_a_out_file;extern MFILE *a_out_file;FInfo	finfo;short vrefnum;		if(getfinfo("MacVAX",(short)0,&finfo) == noErr)		vrefnum = 0;	else		vrefnum = appl_vrefnum;	SetZone(SystemZone());	DisposHandle(*lmg);	args = NewHandle(sizeof(struct mac_args));	HLock(args);	SetZone(ApplicZone());	arglist = (struct mac_args *)*args;	arglist->message = 0;	if(a_out_file == 0 || anyerrs)		arglist->count = 0;	else	{		arglist->count = 1;		arglist->argv.vRefNum = last_a_out_file.vrefnum;		arglist->argv.fType = 'AOUT';		arglist->argv.versNum = 0;		arglist->argv.fName[0] = (unsigned char)strlen(last_a_out_file.name);		strcpy((char *)&arglist->argv.fName[1], last_a_out_file.name);	}	*lmg = args;	HUnlock(args);	exec_it("MacVAX",vrefnum);}void exec_edit(void){struct mac_args{	short message;	short count;	AppFile argv;} *arglist;Handle args;Handle *lmg = (Handle *)0xaec;extern short appl_vrefnum;extern long	anyerrs;extern MFILE *a_out_file;extern long num_files;FInfo	finfo;short vrefnum;extern TEHandle TextH;		if( getfinfo("edit",(short)0,&finfo) == noErr)		vrefnum = 0;	else		vrefnum = appl_vrefnum;	ZeroScrap();	HLock((*TextH)->hText);	PutScrap((*TextH)->teLength,'TEXT',*(*TextH)->hText);	HUnlock((*TextH)->hText);	UnloadScrap();	if( file_info == 0 && num_files)		exec_it("edit",vrefnum);	else	{		SetZone(SystemZone());		DisposHandle(*lmg);		args = NewHandle(sizeof(struct mac_args));		HLock(args);		SetZone(ApplicZone());		arglist = (struct mac_args *)*args;		arglist->message = 0;		if(file_info == 0)			arglist->count = 0;		else		{			arglist->count = 1;			arglist->argv.vRefNum = last_file_info.vrefnum;			arglist->argv.fType = 'AOUT';			arglist->argv.versNum = 0;			arglist->argv.fName[0] = (unsigned char)strlen(last_file_info.name);			strcpy((char *)&arglist->argv.fName[1], last_file_info.name);		}		*lmg = args;		HUnlock(args);		exec_it("edit",vrefnum);	}}