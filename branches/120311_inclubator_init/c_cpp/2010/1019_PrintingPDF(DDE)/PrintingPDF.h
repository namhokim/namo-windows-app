/**************************************************************************

PrintingPDF.h

This file is furnished to you by Adobe Systems Incorporated
under the terms of the Acrobat (r) Plug-ins Software
Development Kit License Agreement.

Copyright (C) 1995-1999, Adobe Systems Inc.  All Rights Reserved.

PURPOSE

This implements a very simple DDE client that communicates with
the viewer to open a file. The viewer must be running in
order for the DDE transaction to take place.

Caveats:
None. 

***************************************************************************/

#pragma once
#ifndef	_PDF_PRINT_
#define	_PDF_PRINT_

/* Return Code */
#define	RETURN_SUCCESS	0
#define	RETURN_FAILED	1

int PDFPrint(char *szFilename);

#endif
