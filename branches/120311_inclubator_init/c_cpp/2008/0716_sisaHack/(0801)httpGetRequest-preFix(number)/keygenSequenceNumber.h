#include "common.h"
#include "httpModule.h"		// need to
#include "logfileModule.h"

extern char g_id[MAX_ID];				// httpModule���� �˻��� id�� ���(�⺻��: nam17845)
extern char savingFilename[MAX_PATH];	// log ���� ����� ������ �̸�

void SeqNumberEntry(char * targetId, char * prefixString);										// 3
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber);						// 4
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber, int endNumber);		// 5
void SeqNumberEntry(char * prefixString);														// 2
void SeqNumberEntry(char * prefixString, int beginNumber);									// 3
void SeqNumberEntry(char * prefixString, int beginNumber, int endNumber);					// 4