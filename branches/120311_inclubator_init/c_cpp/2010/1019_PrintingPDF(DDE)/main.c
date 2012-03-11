#include "PrintingPDF.h"
#include <stdio.h>
#include <stdlib.h>

void Usage()
{
	printf("Usage:\n");
	printf(" > PrintingPDF [.PDF]\n\n");
}

int main(int argc, char *argv[])
{
	if(argc == 2) {
		return ( PDFPrint(argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE );
	} else {
		Usage();
		return EXIT_SUCCESS;
	}
}