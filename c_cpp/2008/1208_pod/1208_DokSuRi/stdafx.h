// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>	// _tWinMain


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "WindowPosition.h"
#include "FileOpenDialogBox.h"
#include "RegistryManager.h"
#include "AloFileMonitor.h"
#include "PJLReader.h"
#include "UserInfoValidator.h"
#include "FileManager.h"
#include "FTPRequest.h"
#include "SingleInstance.h"