#pragma once

void RunTestAll();
// ->
void Test_GetDefaultPrinter_Param_AllNull();
void Test_GetDefaultPrinter_Param_Buffer1024_Null();
void Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT();
void Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_0();
void Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_1024();
void Test_GetDefaultPrinter_Param_Buffer1024_INIT_WITH_1024();
void Test_GetDefaultPrinter_Param_Buffer2_INIT_WITH_1024();