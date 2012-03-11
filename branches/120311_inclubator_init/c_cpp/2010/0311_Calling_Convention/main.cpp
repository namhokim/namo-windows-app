#include <stdio.h>

int Sum(int a, int b);

int __cdecl main()
{
    Sum(1, 2);
	
    return 0;
}

//_cdecl
//_fastcall
//_stdcall
int Sum(int a, int b)
{
	int i = a + b;
	return i;
}

/*
_cdecl

.text:00401000 ; int __cdecl main(int argc, const char **argv, const char **envp)
.text:00401000 _main           proc near               ; CODE XREF: ___tmainCRTStartup+F6p
.text:00401000
.text:00401000 argc            = dword ptr  8
.text:00401000 argv            = dword ptr  0Ch
.text:00401000 envp            = dword ptr  10h
.text:00401000
.text:00401000                 push    ebp
.text:00401001                 mov     ebp, esp
.text:00401003                 push    2
.text:00401005                 push    1
.text:00401007                 call    sub_401020
.text:0040100C                 add     esp, 8
.text:0040100F                 xor     eax, eax
.text:00401011                 pop     ebp
.text:00401012                 retn
.text:00401012 _main           endp
.text:00401012
.text:00401012 ; ---------------------------------------------------------------------------
.text:00401013                 align 10h
.text:00401020
.text:00401020 ; =============== S U B R O U T I N E =======================================
.text:00401020
.text:00401020 ; Attributes: bp-based frame
.text:00401020
.text:00401020 sub_401020      proc near               ; CODE XREF: _main+7p
.text:00401020
.text:00401020 var_4           = dword ptr -4
.text:00401020 arg_0           = dword ptr  8
.text:00401020 arg_4           = dword ptr  0Ch
.text:00401020
.text:00401020                 push    ebp
.text:00401021                 mov     ebp, esp
.text:00401023                 push    ecx
.text:00401024                 mov     eax, [ebp+arg_0]
.text:00401027                 add     eax, [ebp+arg_4]
.text:0040102A                 mov     [ebp+var_4], eax
.text:0040102D                 mov     eax, [ebp+var_4]
.text:00401030                 mov     esp, ebp
.text:00401032                 pop     ebp
.text:00401033                 retn
.text:00401033 sub_401020      endp



_stdcall

.text:00401000 ; int __cdecl main(int argc, const char **argv, const char **envp)
.text:00401000 _main           proc near               ; CODE XREF: ___tmainCRTStartup+F6p
.text:00401000
.text:00401000 argc            = dword ptr  8
.text:00401000 argv            = dword ptr  0Ch
.text:00401000 envp            = dword ptr  10h
.text:00401000
.text:00401000                 push    ebp
.text:00401001                 mov     ebp, esp
.text:00401003                 push    2
.text:00401005                 push    1
.text:00401007                 call    sub_401010
.text:0040100C                 xor     eax, eax
.text:0040100E                 pop     ebp
.text:0040100F                 retn
.text:0040100F _main           endp
.text:0040100F
.text:00401010
.text:00401010 ; =============== S U B R O U T I N E =======================================
.text:00401010
.text:00401010 ; Attributes: bp-based frame
.text:00401010
.text:00401010 sub_401010      proc near               ; CODE XREF: _main+7p
.text:00401010
.text:00401010 var_4           = dword ptr -4
.text:00401010 arg_0           = dword ptr  8
.text:00401010 arg_4           = dword ptr  0Ch
.text:00401010
.text:00401010                 push    ebp
.text:00401011                 mov     ebp, esp
.text:00401013                 push    ecx
.text:00401014                 mov     eax, [ebp+arg_0]
.text:00401017                 add     eax, [ebp+arg_4]
.text:0040101A                 mov     [ebp+var_4], eax
.text:0040101D                 mov     eax, [ebp+var_4]
.text:00401020                 mov     esp, ebp
.text:00401022                 pop     ebp
.text:00401023                 retn    8
.text:00401023 sub_401010      endp




_fastcall

.text:00401000 ; int __cdecl main(int argc, const char **argv, const char **envp)
.text:00401000 _main           proc near               ; CODE XREF: __tmainCRTStartup+F6p
.text:00401000
.text:00401000 argc            = dword ptr  8
.text:00401000 argv            = dword ptr  0Ch
.text:00401000 envp            = dword ptr  10h
.text:00401000
.text:00401000                 push    ebp
.text:00401001                 mov     ebp, esp
.text:00401003                 mov     edx, 2          ; b
.text:00401008                 mov     ecx, 1          ; a
.text:0040100D                 call    ?Sum@@YIHHH@Z   ; Sum(int,int)
.text:00401012                 xor     eax, eax
.text:00401014                 pop     ebp
.text:00401015                 retn
.text:00401015 _main           endp
.text:00401015
.text:00401015 ; ---------------------------------------------------------------------------
.text:00401016                 align 10h
.text:00401020
.text:00401020 ; =============== S U B R O U T I N E =======================================
.text:00401020
.text:00401020 ; Attributes: bp-based frame
.text:00401020
.text:00401020 ; int __fastcall Sum(int a, int b)
.text:00401020 ?Sum@@YIHHH@Z   proc near               ; CODE XREF: _main+Dp
.text:00401020
.text:00401020 b               = dword ptr -0Ch
.text:00401020 a               = dword ptr -8
.text:00401020 i               = dword ptr -4
.text:00401020
.text:00401020                 push    ebp
.text:00401021                 mov     ebp, esp
.text:00401023                 sub     esp, 0Ch
.text:00401026                 mov     [ebp+b], edx
.text:00401029                 mov     [ebp+a], ecx
.text:0040102C                 mov     eax, [ebp+a]
.text:0040102F                 add     eax, [ebp+b]
.text:00401032                 mov     [ebp+i], eax
.text:00401035                 mov     eax, [ebp+i]
.text:00401038                 mov     esp, ebp
.text:0040103A                 pop     ebp
.text:0040103B                 retn
.text:0040103B ?Sum@@YIHHH@Z   endp



참고 : 좋은향기 호출규약 - http://thepassion.tistory.com/?page=14

 */