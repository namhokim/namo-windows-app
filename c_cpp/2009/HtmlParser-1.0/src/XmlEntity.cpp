////////////////////////////////////////////////////////////////////
///
///   XmlEntity.cpp 
///
///   Copyright (C) 2007 ScalingWeb.com
///   All rights reserved. 
/// 
///   Authors: Yuriy Soroka <ysoroka@scalingweb.com>
///	       Anton Fedoruk <afedoruk@scalingweb.com>
///
/// 
///////////////////////////////////////////////////////////////////


#include "XmlEntity.h"
#include <QtAlgorithms>
#include <string>
#include <QTextCodec>
#include <QString>
#include <QFile>

///
/// Class StringComparator
///

class StringComparator
{
public:

	StringComparator( const char *string_line );

	// String data

	const char *data() const;

private:

	const char *m_string;
};

static const StringComparator name_table[] = 
{
	"AElig",
	"Aacute",
	"Acirc",
	"Agrave",
	"Aring",
	"Atilde",
	"Auml",
	"Ccedil",
	"ETH",
	"Eacute",
	"Ecirc",
	"Egrave",
	"Euml",
	"Iacute",
	"Icirc",
	"Igrave",
	"Iuml",
	"Nbsp",
	"Ntilde",
	"Oacute",
	"Ocirc",
	"Ograve",
	"Oslash",
	"Otilde",
	"Ouml",
	"THORN",
	"Uacute",
	"Ucirc",
	"Ugrave",
	"Uuml",
	"Yacute",
	"aacute",
	"acirc",
	"acute",
	"aelig",
	"agrave",
	"amp",
	"aring",
	"atilde",
	"auml",
	"brvbar",
	"ccedil",
	"cedil",
	"cent",
	"copy",
	"curren",
	"deg",
	"divide",
	"eacute",
	"ecirc",
	"egrave",
	"eth",
	"euml",
	"frac12",
	"frac14",
	"frac34",
	"gt",
	"hibar",
	"iacute",
	"icirc",
	"iexcl",
	"igrave",
	"iquest",
	"iuml",
	"laquo",
	"lt",
	"micro",
	"middot",
	"nbsp",
	"not",
	"ntilde",
	"oacute",
	"ocirc",
	"ograve",
	"ordf",
	"ordm",
	"oslash",
	"otilde",
	"ouml",
	"para",
	"plusmn",
	"pound",
	"quot",
	"raquo",
	"reg",
	"sect",
	"shy",
	"sup1",
	"sup2",
	"sup3",
	"szlig",
	"thorn",
	"times",
	"trade",
	"uacute",
	"ucirc",
	"ugrave",
	"uml",
	"uuml",
	"yacute",
	"yen",
	"yuml"
};

static const ushort entity_table[] = 
{
	198,
	193,
	194,
	192,
	197,
	195,
	196,
	199,
	208,
	201,
	202,
	200,
	203,
	204,
	206,
	205,
	207,
	32,
	209,
	211,
	212,
	210,
	216,
	213,
	214,
	222,
	218,
	219,
	217,
	220,
	221,
	225,
	226,
	180,
	230,
	224,
	38,
	229,
	227,
	228,
	166,
	231,
	184,
	162,
	169,
	164,
	176,
	247,
	233,
	234,
	232,
	240,
	235,
	189,
	188,
	190,
	62,
	175,
	237,
	238,
	161,
	236,
	191,
	239,
	171,
	60,
	181,
	183,
	32,
	172,
	241,
	243,
	244,
	242,
	170,
	186,
	248,
	245,
	246,
	182,
	177,
	163,
	34,
	187,
	174,
	167,
	173,
	185,
	178,
	179,
	223,
	254,
	215,
	153,
	250,
	251,
	249,
	168,
	252,
	253,
	165,
	255
};


const int NumEntries = sizeof(name_table) / sizeof(name_table[0]);

bool operator<(const StringComparator& entry, const StringComparator& s)
{
	if(entry.data() && s.data())
	{
		return ( strcmp( entry.data(), s.data() ) < 0 );
	}
	else
	{
		return false;
	}
}

//	===============================================================
//	StringComparator::StringComparator

StringComparator::StringComparator( const char *string_line )
{
	m_string = string_line;
}

//	===============================================================
//	StringComparator::data

const char * StringComparator::data() const
{
	return m_string;
}

//	===============================================================
//	XmlEntity::entity

int XmlEntity::entity( const char *title )
{
	// Getting number value of string entity
	const StringComparator* name = qBinaryFind( name_table, name_table + NumEntries, title );
	int index = name - name_table;
    if ( index == NumEntries )
	{
        return ' ';
	}
	else
	{
		return entity_table[ index ];
	}
}
