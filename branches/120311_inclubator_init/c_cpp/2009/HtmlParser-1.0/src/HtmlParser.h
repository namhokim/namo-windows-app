////////////////////////////////////////////////////////////////////
///
///   HtmlParser.h
///
///   Copyright (C) 2007 ScalingWeb.com
///   All rights reserved. 
/// 
///   Authors: Yuriy Soroka <ysoroka@scalingweb.com>
///	       Anton Fedoruk <afedoruk@scalingweb.com>
///
/// 
///////////////////////////////////////////////////////////////////

#ifndef __HtmlParser_h__
#define __HtmlParser_h__

#include <map>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QTextDecoder>
#include <QHash>
#include <QSet>

#include "JInputStream.h"
#include "JBufferString.h"

typedef QHash< QByteArray, QByteArray > AttrsMap;

/// Class HtmlParser

class HtmlParser
{
public:

	HtmlParser();
	virtual ~HtmlParser();

	///
	/// Parses html document and sets internal parameters like
	/// title, keywords, charset and text.
	/// @return true if success, or false otherwise.

	bool parse( JInputStream *htmlStream );

	///
	/// Parses html document and sets internal parameters like
	/// title, keywords, charset and text. Extracts all links.
	/// Prepends to the extracted relative links contextUrl.
	/// @return true if success, or false otherwise.

	bool parse( JInputStream *htmlStream, QByteArray &contextURL );

	// Return document params

	const QByteArray& title() const;
	const QByteArray& keywords() const;
	const QByteArray& description() const;
	const QByteArray& author() const;
	const QByteArray& text() const;
	const QByteArray& charset() const;

	///
	/// Returns links harvested from the parsed document

	const QSet<QString>& listURLs() const;

	///
	/// Set codec for parsing html, meta data charset will
	/// be ignored

	void setCodec( QByteArray &charset );

protected:

	inline void parseTag();
	bool parseAttrs( ushort firstChar );
	bool parseTagName( JBufferString &tagName );
	inline void applyChar( ushort c );
	void addAttribute( JBufferString &name, JBufferString &value );

	QByteArray attribute( const char* name );
	void addHref( const QByteArray &href );
	QByteArray remove2Dots( QByteArray &path );
	bool isRelativeURL( const char *pc, long len ) const;
	bool isCharAcceptable( ushort c ) const;
	void testUnicode();
	inline ushort readChar();

	void addNewLine( bool bForce );
	void init();
	void prepareAll();
	void setCurrentCodec();	
	QByteArray removeEntities( QByteArray &indata );

	void parseBrTr();
	void parseTd();
	void parseBlockquote();
	void parseA();
	void parseTitle();
	void parseBSlashTitle();
	void parseTableBSlashLiUl();
	void parseLi();
	void parseScript();
	void parseBSlashScript();
	void parseStyle();
	void parseBSlashStyle();
	void parsePre();
	void parseBSlashPre();
	void parseMeta();
	void parseFrame();

	typedef void ( HtmlParser::*VoidMethodType )( void );
	typedef std::map< JBufferString, VoidMethodType > MethodMap;

	static MethodMap tags_;

	JInputStream *stream_;

	bool inscript_, intitle_, instyle_;
	bool inpre_;
	bool extractLinks_;
	int utf_;
	bool utf8native_;

	QByteArray contextUrl_;

	QByteArray title_;
	QByteArray keywords_;
	QByteArray description_;
	QByteArray author_;
	QByteArray text_;
	QByteArray charset_;
	QSet<QString> hrefs_;

	AttrsMap attrs_;

	QTextCodec *codec_;
	QTextCodec *xmlEntityCodec_;	
	QTextDecoder *decoder_;

};

#endif // __HtmlParser_h__

