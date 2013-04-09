#include "StdAfx.h"
#include "ToyTokenizer.h"

ToyTokenizer::ToyTokenizer(void)
{
}

ToyTokenizer::~ToyTokenizer(void)
{
}

void ToyTokenizer::setProg(const char* prog)
{
	m_prog = prog;
}

bool ToyTokenizer::getToken(std::string& token, int& type)
{
	return false;
}