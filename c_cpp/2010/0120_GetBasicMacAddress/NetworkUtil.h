/**
 * 요약 : 네트워크 관련 라이브러리
 * 라이센스 : 현재 주석을 유지하는 전재하에 수정 및 배포를 자유롭게 할 수 있으며
				 수정한 코드를 다시 공개할 의무가 없습니다.
 * 면책조항 : 위 프로그램의 잘못된 작동으로 인한 책임을 지지 않습니다.
 * Copyright by Namo 2010.1.20
 */

#ifndef _NETWORK_NAMO_H_
#define _NETWORK_NAMO_H_
#include <string>

#define NO_INTERFACE	-1

std::string GetBasicMacAddress(const char* deliminator = "-");
#endif