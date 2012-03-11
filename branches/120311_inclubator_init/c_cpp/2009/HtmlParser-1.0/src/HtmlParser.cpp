////////////////////////////////////////////////////////////////////
///
///   HtmlParser.cpp 
///
///   Copyright (C) 2007 ScalingWeb.com
///   All rights reserved. 
/// 
///   Authors: Yuriy Soroka <ysoroka@scalingweb.com>
///	       Anton Fedoruk <afedoruk@scalingweb.com>
///
/// 
///////////////////////////////////////////////////////////////////


#include "HtmlParser.h"
#include "JBufferedInputStream.h"
#include "XmlEntity.h"
#include <QUrl>

const int ReserveNameCount = 64;
const int ReserveValueCount = 1024;

char brTag[3] = "br";
char trTag[3] = "tr";
char tdTag[3] = "td";
char pTag[2] = "p";
char blockquoteTag[11] = "blockquote";
char bslashBlockquoteTag[12] = "/blockquote";
char aTag[2] = "a";
char titleTag[6] = "title";
char bslashTitleTag[7] = "/title";
char ulTag[3] = "ul";
char liTag[3] = "li";
char bslashLiTag[4] = "/li";
char tableTag[6] = "table";
char scriptTag[7] = "script";
char bslashScriptTag[8] = "/script";
char styleTag[6] = "style";
char bslashStyleTag[7] = "/style";
char preTag[4] = "pre";
char bslashPreTag[5] = "/pre";
char metaTag[5] = "meta";
char frameTag[6] = "frame";
char imgTag[4] = "img";

//	==============================================
//	HtmlParser::HtmlParser

HtmlParser::HtmlParser()
:	extractLinks_( false ),
	decoder_( 0 ),
	codec_( 0 ),
	xmlEntityCodec_( 0 )
{
	init();
	xmlEntityCodec_ = QTextCodec::codecForName( "windows-1251" );

	if ( tags_.empty() )
	{
		// Init map
		JBufferString br( brTag, 3, true );
		tags_[ br ] =  &HtmlParser::parseBrTr;
		JBufferString tr( trTag, 3, true );
		tags_[ tr ] =  &HtmlParser::parseBrTr;

		JBufferString td( tdTag, 3, true );
		tags_[ td ] =  &HtmlParser::parseTd;

		JBufferString p( pTag, 2, true );
		tags_[ p ] =  &HtmlParser::parseBlockquote;
		JBufferString blockquote( blockquoteTag, 11, true );
		tags_[ blockquote ] =  &HtmlParser::parseBlockquote;
		JBufferString bslashBlockquote( bslashBlockquoteTag, 12, true );
		tags_[ bslashBlockquote ] =  &HtmlParser::parseBlockquote;

		JBufferString a( aTag, 2, true );
		tags_[ a ] =  &HtmlParser::parseA;

		JBufferString title( titleTag, 6, true );
		tags_[ title ] =  &HtmlParser::parseTitle;
		JBufferString bslashTitle( bslashTitleTag, 7, true );
		tags_[ bslashTitle ] =  &HtmlParser::parseBSlashTitle;

		JBufferString table( tableTag, 6, true );
		tags_[ table ] =  &HtmlParser::parseTableBSlashLiUl;

		JBufferString ul( ulTag, 3, true );
		tags_[ ul ] =  &HtmlParser::parseTableBSlashLiUl;

		JBufferString li( liTag, 3, true );
		tags_[ li ] =  &HtmlParser::parseLi;

		JBufferString bslashLi( bslashLiTag, 3, true );
		tags_[ bslashLi ] =  &HtmlParser::parseTableBSlashLiUl;

		JBufferString script( scriptTag, 7, true );
		tags_[ script ] =  &HtmlParser::parseScript;

		JBufferString bslashScript( bslashScriptTag, 8, true );
		tags_[ bslashScript ] =  &HtmlParser::parseBSlashScript;

		JBufferString style( styleTag, 6, true );
		tags_[ style ] =  &HtmlParser::parseStyle;

		JBufferString bslashStyle( bslashStyleTag, 7, true );
		tags_[ bslashStyle ] =  &HtmlParser::parseBSlashStyle;

		JBufferString pre( preTag, 4, true );
		tags_[ pre ] =  &HtmlParser::parsePre;

		JBufferString bslashPre( bslashPreTag, 5, true );
		tags_[ bslashPre ] =  &HtmlParser::parseBSlashPre;

		JBufferString meta( metaTag, 5, true );
		tags_[ meta ] =  &HtmlParser::parseMeta;

		JBufferString frame( frameTag, 6, true );
		tags_[ frame ] =  &HtmlParser::parseFrame;

		JBufferString img( imgTag, 4, true );
		tags_[ img ] =  &HtmlParser::parseFrame;
	}
}

//	==============================================
//	HtmlParser::~HtmlParser

HtmlParser::~HtmlParser()
{
	if ( decoder_ )
	{
		delete decoder_;
	}
}

//	==============================================
//	apply tag's methods

void HtmlParser::parseBrTr()
{
	if ( !intitle_ && !inscript_ && !instyle_ )
	{
		addNewLine( true );
	}
}

void HtmlParser::parseTd()
{
	text_.append( "\t" );
}

void HtmlParser::parseBlockquote()
{
	if ( !intitle_ && !inscript_ && !instyle_ )
	{
		if ( !text_.endsWith( "\r\n" ) )
		{
			// Another new line for paragraph
			addNewLine( true );
		}

		addNewLine( true );
	}
}

void HtmlParser::parseA()
{
	if ( !inscript_ && extractLinks_ )
	{
		addHref( attribute( "href" ) );
	}
}

void HtmlParser::parseTitle()
{
	intitle_ = true;
}

void HtmlParser::parseBSlashTitle()
{
	intitle_ = false;
}

void HtmlParser::parseTableBSlashLiUl()
{
	if ( !intitle_ && !inscript_ && !instyle_ )
	{
		addNewLine( false );
	}
}

void HtmlParser::parseLi()
{
	if ( !intitle_ && !inscript_ && !instyle_ )
	{
		addNewLine( true );
		text_.append( "       " );
	}
}

void HtmlParser::parseScript()
{
	inscript_ = true;
}

void HtmlParser::parseBSlashScript()
{
	inscript_ = false;
}

void HtmlParser::parseStyle()
{
	instyle_ = true;
}

void HtmlParser::parseBSlashStyle()
{
	instyle_ = false;
}

void HtmlParser::parsePre()
{
	inpre_ = true;
}

void HtmlParser::parseBSlashPre()
{
	inpre_ = true;
}

void HtmlParser::parseMeta()
{
	QByteArray httpEquiv = attribute( "http-equiv" );
	QByteArray content = attribute( "content" );
	QByteArray aName = attribute( "name" );

	aName = aName.toLower();
	httpEquiv = httpEquiv.toLower();

	if ( aName == "keywords" )
	{
		keywords_ = attribute("content" );
	}
	else if ( aName == "description" )
	{
		description_ = content;
	}
	else if ( aName == "author" )
	{
		author_ = content;
	}

	if ( !httpEquiv.isEmpty() )
	{
		int pos = content.indexOf( ';' );

		if ( pos != -1 )
		{
			QByteArray cset = content.mid( pos + 1, content.length() );

			pos = cset.indexOf( '=' );

			if ( pos != -1 )
			{
				charset_ = cset.mid( pos + 1, cset.length() );
				setCurrentCodec();
			}
		}
	}
}

void HtmlParser::parseFrame()
{
	addHref( attribute( "src" ) );
}

//	==============================================
//	HtmlParser::parse

bool HtmlParser::parse( JInputStream *htmlStream )
{
	if ( htmlStream->atEnd() )
	{
		return false;
	}

	JBufferedInputStream< JInputStream > html( htmlStream );
	html.initBuffer();

	stream_ = &html;

	// Check html for utf-16 encoding
	testUnicode();

	text_.reserve( stream_->size()/3 );

	while ( !html.atEnd() )
	{
		ushort curChar = readChar();

		switch ( curChar )
		{
		case '<':
			parseTag();
			break;
		//case '&':
		//	parseEntity();
		//	break;
		default:
			applyChar( curChar );
			break;
		}
	}

	// Trim all white space from meta data
	prepareAll();

	return true;
}

//	==============================================
//	HtmlParser::testUtf16

void HtmlParser::testUnicode()
{
	char bom[ 4 ];

	JBufferedInputStream< JInputStream >* bufStream = ( JBufferedInputStream< JInputStream >* ) stream_;
	bufStream->bufferRead( bom, 3 );

	if( uchar( bom[ 0 ] ) == 0xFF && uchar( bom[ 1 ] ) == 0xFE )
	{
		bufStream->read( bom, 2 );
		charset_ = "UTF-16";
		utf_ = 2;
		setCurrentCodec();
	}
	else if( uchar( bom[ 0 ] ) == 0xFE && uchar( bom[ 1 ] ) == 0xFF )
	{
		bufStream->read( bom, 2 );
		charset_ = "UTF-16";
		utf_ = 2;
		setCurrentCodec();
	}
	else if ( uchar( bom[ 0 ] ) == 0xEF && uchar( bom[ 1 ] ) == 0xBB && uchar( bom[ 2 ] ) == 0xBF )
	{
		bufStream->read( bom, 3 );
		charset_ = "UTF-8";
		setCurrentCodec();
	}
}

//	==============================================
//	HtmlParser::prepareAll

void HtmlParser::prepareAll()
{
	if ( 0 == decoder_ && !utf8native_ )
	{
		setCurrentCodec();
	}

	charset_ = "utf-8";
	title_ = removeEntities( title_ );
	keywords_ = removeEntities( keywords_ );
	description_ = removeEntities( description_ );
	author_ = removeEntities( author_ );
	text_ = removeEntities( text_ );

	title_ = title_.trimmed();
}

//	==============================================
//	HtmlParser::removeEntities

QByteArray HtmlParser::removeEntities( QByteArray &indata )
{
	QByteArray ret;
	ret.reserve( indata.size() );

	if ( decoder_ )
	{
		indata = decoder_->toUnicode( indata ).toUtf8();
	}

	int i = 0;
	while ( i < indata.size() )
	{
		if ( '&' == indata.at( i ) )
		{
			if ( i < indata.size() && '#' == indata.at( i + 1 ) )
			{
				// Skip '&'
				i++;
				// Skip '#'
				i++;

				int code = 0;
				char curChar = 0;
				while ( JBufferString::isDigit( curChar = indata.at( i ) ) )
				{
					code = code * 10 + ( curChar - '0' );
					i++;
				}

				QChar qchar( code );
				QString uniChar( qchar );

				ret += uniChar.toUtf8();
			}
			else
			{
				// Check for named xml entity
				const int maxLen = 10;
				char buf[ maxLen + 1 ];
				JBufferString entity( buf, maxLen);
				entity.clear();

				char curChar = 0;
				i++;

				while ( i < indata.size() && entity.size() < maxLen )
				{
					curChar = indata.at( i );

					if ( ';' == curChar || !JBufferString::isAlpha( curChar ) )
					{
						break;
					}

					entity.add( curChar );
					i++;
				}

				quint32 code = ( quint32 ) XmlEntity::entity( entity.data() );

				if ( 0 != code )
				{
					ret += xmlEntityCodec_->toUnicode( ( char* ) &code ).toUtf8();
				}
				else
				{
					ret += entity.data();
				}
			}
		}
		else
		{
			ret.append( indata.at( i ) );
		}

		i++;
	}

	return ret;
}

//	==============================================
//	HtmlParser::parse

bool HtmlParser::parse( JInputStream *htmlStream, QByteArray &contextURL )
{
	contextUrl_ = contextURL;
	extractLinks_ = true;
	return parse( htmlStream );
}

//	==============================================
//	HtmlParser::parseTag

void HtmlParser::parseTag()
{
	bool bTagName = true;
	bool endTag = false;

	char tagNameBuf[ 8192 + 1 ];

	JBufferString tagName( tagNameBuf, 8192 );
	tagName.clear();

	// Iterate stream_ chars
	while ( !stream_->atEnd() && !endTag )
	{
		ushort curChar = readChar();

		switch ( curChar )
		{
		case '!':
			if ( bTagName )
			{
				bool bComment = false;

				ushort firstChar = readChar();
				ushort secondChar = readChar();

				if ( '-' == firstChar && '-' == secondChar )
				{
					// Then it is comment
					bComment = true;
				}

				// It is comment or define entity - skip it
				while ( !stream_->atEnd() )
				{
					if ( bComment )
					{
						// Check for end comment (three 'if' because some compilers
						// can compare in different directions)
						if ( readChar() == '-' )
						{
							if ( readChar() == '-' )
							{
								if ( readChar() == '>' )
								{
									return;
								}
							}
						}
					}
					else
					{
						if ( readChar() == '>' )
						{
							// End of define (like DOCTYPE)
							return;
						}
					}
				}
				return;
			}
		case '?':
			if ( bTagName )
			{
				ushort prevChar = 0;
				while ( !stream_->atEnd() )
				{
					curChar = readChar();

					if ( prevChar == '?' && curChar == '>' )
					{
						return;
					}

					prevChar = curChar;
				}
				return;
			}
			break;
		case '>':
			endTag = true;
			break;
		default:
			{
				if ( bTagName )
				{
					if ( inscript_ && '/' != curChar )
					{
						return;
					}

					// Parse tag name
					bTagName = false;
					tagName.clear();
					tagName.add( curChar );

					if ( !JBufferString::isAlpha( curChar ) && 
						!JBufferString::isDigit( curChar ) && 
						'-' != curChar &&
						'/' != curChar )
					{
						if ( inpre_ )
						{
							text_.append( '<' );
							text_.append( curChar );
						}

						return;
					}

					endTag = parseTagName( tagName );
				}
				else
				{
					// Parse tag attr
					endTag = parseAttrs( curChar );
				}
			}
			break;
		}
	}

	// Apply tag
	if ( tagName.size() >= 2 && ( ( 'h' == tagName.at( 0 ) || 'H' == tagName.at( 0 ) ) &&
		JBufferString::isDigit( tagName.at( 1 ) ) ) || 
		( '/' == tagName.at( 0 ) && ( 'h' == tagName.at( 1 ) || 'H' == tagName.at( 1 ) ) && 
		JBufferString::isDigit( tagName.at( 2 ) ) ) )
	{
		addNewLine( false );
	}
	else
	{
        tagName.toLower();
		if( tags_.find( tagName ) != tags_.end() )
		{
			( this->*tags_[ tagName ])();
		}
	}
}

//	==============================================
//	HtmlParser::setCurrentCodec

void HtmlParser::setCurrentCodec()
{
	if ( 0 != decoder_ )
	{
		return;
	}

	if ( charset_.isEmpty() )
	{
		charset_ = "utf-8";		
	}


	if ( !utf8native_ && charset_.toLower() != "utf-8" && charset_.toLower() != "ascii" )
	{
		codec_ = QTextCodec::codecForName( charset_ );

		if ( codec_ )
		{
			decoder_ = new QTextDecoder( codec_ );
		}
	}
	else
	{
		utf8native_ = true;
	}


	if ( 0 != codec_ )
	{
		xmlEntityCodec_ = codec_;
	}
}

//	==============================================
//	HtmlParser::readChar

inline ushort HtmlParser::readChar()
{
	if ( decoder_ )
	{
		if ( 2 == utf_ )
		{
			ushort ret;
			stream_->read( ( char* ) &ret, sizeof( ret ) );
			return ret;
		}
	}

	return stream_->read();
}

//	==============================================
//	HtmlParser::applyChar

inline void HtmlParser::applyChar( ushort c )
{
	if ( intitle_ )
	{		
		if ( 2 == utf_ )
		{		
			title_.resize( title_.size() + 2 );
			::memcpy( ( title_.data() + title_.size() - 2 ), &c, 2 );
		}
		else
		{			
			title_.append( ( char ) c );
		}
	}
	else if ( inpre_ )
	{
		if ( decoder_ )
		{
			if ( 2 == utf_ )
			{			
				text_.resize( text_.size() + 2 );
				::memcpy( ( text_.data() + text_.size() - 2 ), &c, 2 );
			}
			else
			{			
				text_.append( ( char ) c );
			}
		}
		else
		{
			text_.append( c );
		}
	}
	else if ( !inscript_ && !instyle_ )
	{
		// Add normalized text
		bool bWS = JBufferString::isWhiteSpace( c );
		bool endsWS = text_.isEmpty() ? false : JBufferString::isWhiteSpace( text_.at( text_.size() - 1 ) );
		bool isNewLine = '\r' == c || '\n' == c;
	

		if ( isNewLine && !endsWS && !text_.isEmpty() )
		{
			c = ' ';

			if ( decoder_ )
			{
				if ( 2 == utf_ )
				{				
					text_.resize( text_.size() + 2 );
					::memcpy( ( text_.data() + text_.size() - 2 ), &c, 2 );
				}
				else
				{					
					text_.append( ( char ) c );
				}
			}
			else if ( utf8native_ )
			{
				text_.append( ( char ) c );
			}
			else
			{
				text_.append( ( char ) c );
			}
		}

		if ( !( bWS && endsWS ) && !isNewLine )
		{
			if ( decoder_ )
			{
				if ( 2 == utf_ )
				{					
					text_.resize( text_.size() + 2 );
					::memcpy( ( text_.data() + text_.size() - 2 ), &c, 2 );
				}
				else
				{					
					text_.append( ( char ) c );
				}
			}
			else
			{
				text_.append( c );
			}
		}
	}
}

//	==============================================
//	HtmlParser::parseAttrs

bool HtmlParser::parseAttrs( ushort firstChar )
{
	attrs_.clear();

	char nameBuf[ 1024 + 1 ];
	char valueBuf[ 8192 + 1 ];

	JBufferString name( nameBuf, 1024 );
	JBufferString value( valueBuf, 8192 );

	bool isName = true;
	char valueEnd = 0;
	char curChar = firstChar;

	// Iterate stream_ name chars
	while ( !stream_->atEnd() )
	{
		if ( JBufferString::isWhiteSpace( curChar ) )
		{
			curChar = ' ';
		}

		switch ( curChar )
		{
		case '>':
			if ( 0 == valueEnd && !isName )
			{
				addAttribute( name, value );
			}
			return true;
			break;
		case '=':
			if ( isName )
			{
				isName = false;
			}
			else
			{
				if ( value.size() >= 8192 )
				{
					return true;
				}

				value.add( curChar );
			}
			break;
		case ' ':
			if ( 0 == valueEnd && !isName )
			{
				// Add without brackets
				addAttribute( name, value );

				valueEnd = 0;
				isName = true;
				name.clear();
				value.clear();
			}
			break;

		case '"':
		case '\'':

			value.trim();

			if ( !isName && 0 == valueEnd && 0 == value.size() )
			{
				valueEnd = curChar;
			}
			else if ( !isName && curChar == valueEnd )
			{
				// Add without brackets
				addAttribute( name, value );
				valueEnd = 0;
				isName = true;

				name.clear();
				value.clear();
			}
			break;

		default:

			if ( isName )
			{
				if ( name.size() > 100 )
				{
					return false;
				}

				// Check for wrong attr name
				if ( !isCharAcceptable( curChar ) )
				{
					return false;
				}

				name.add( tolower( curChar ) );
			}
			else
			{
				if ( value.size() > 8192 )
				{
					return true;
				}

				value.add( curChar );
			}

			break;
		}

		curChar = readChar();
	}

	return true;
}

//	==============================================
//	HtmlParser::isCharAcceptable

bool HtmlParser::isCharAcceptable( ushort c ) const
{
	switch ( c )
	{
	case '<':
	case '>':
	case ')':
	case '(':
	case '$':
	case '*':
		return false;
		break;
	default:break;
	}

	return true;
}

//	==============================================
//	HtmlParser::addAttribute

void HtmlParser::addAttribute( JBufferString &name, JBufferString &value )
{
	name.trim();
	value.trim();

	if ( name.startsWith( '"' ) && name.endsWith( '"' ) ||
		name.startsWith( '\'' ) && name.endsWith( '\'' ) )
	{
		name.remove( 0, 1 );
		name.remove( name.size() - 1, 1 );
	}

	if ( value.startsWith( '"' ) && value.endsWith( '"' ) ||
		value.startsWith( '\'' ) && value.endsWith( '\'' ) )
	{
		value.remove( 0, 1 );
		value.remove( name.size() - 1, 1 );
	}
	
	attrs_.insert(name.data(), value.data());
}

//	==============================================
//	HtmlParser::parseTagName

bool HtmlParser::parseTagName( JBufferString &tagName )
{
	// Iterate stream_ name chars
	while ( !stream_->atEnd() )
	{
		ushort curChar = readChar();

		if ( JBufferString::isWhiteSpace( curChar ) )
		{
			return false;
		}
		else
		{
			// Check current character
			switch ( curChar )
			{
			case '>':
				return true;
				break;			
			default:
				{
					tagName.add( curChar );

					if ( !JBufferString::isAlpha( curChar ) && 
						!JBufferString::isDigit( curChar ) && curChar != ':' && curChar != '_'
						&& curChar != '/' && curChar != '-' )
					{
						text_ += tagName.data();

						return true;
					}
				}
				break;
			}
		}
	}

	return false;
}

HtmlParser::MethodMap HtmlParser::tags_;

//	==============================================
//	HtmlParser::addNewLine

void HtmlParser::addNewLine( bool bForce )
{
	if ( ( bForce && !text_.isEmpty() ) || 
		!text_.endsWith( "\r\n" ) &&
		!text_.endsWith( "\r\n\r\n" ) )
	{
		text_.append( "\r\n" );
	}
}

//	==============================================
//	HtmlParser::init

void HtmlParser::init()
{
	inscript_ = intitle_ = instyle_ = false;
	inpre_ = false;
	utf_ = 0;
	utf8native_ = false;

	title_.clear();
	keywords_.clear();
	description_.clear();
	author_.clear();
	text_.clear();
	charset_.clear();
	hrefs_.clear();

	attrs_.clear();
}

//	==============================================
//	HtmlParser::title

const QByteArray& HtmlParser::title() const
{
	return title_;
}

//	==============================================
//	HtmlParser::keywords

const QByteArray& HtmlParser::keywords() const
{
	return keywords_;
}

//	==============================================
//	HtmlParser::description

const QByteArray& HtmlParser::description() const
{
	return description_;
}

//	==============================================
//	HtmlParser::author

const QByteArray& HtmlParser::author() const
{
	return author_;
}

//	==============================================
//	HtmlParser::text

const QByteArray& HtmlParser::text() const
{
	return text_;
}

//	==============================================
//	HtmlParser::charset

const QByteArray& HtmlParser::charset() const
{
	return charset_;
}

//	==============================================
//	HtmlParser::listURLs

const QSet<QString>& HtmlParser::listURLs() const
{
	return hrefs_;
}

//	==============================================
//	HtmlParser::attribute

QByteArray HtmlParser::attribute( const char* name )
{
	return attrs_.value( name, "" );
}

//	==============================================
//	HtmlParser::addHref

void HtmlParser::addHref( const QByteArray &href )
{
	if ( !href.isEmpty() )
	{
		if ( href.indexOf( "javascript:" ) != -1 )
		{
			return;
		}

		QByteArray url = href;
		int pos = href.indexOf( "#" );
		if ( pos != -1 )
		{
			url = href.mid( 0, pos );
		}

		if ( !url.isEmpty() )
		{
			// Delete gaps
			url = url.trimmed();

			if ( isRelativeURL( url.data(), url.length() ) )
			{
				QUrl base(contextUrl_);
				QUrl ret = base.resolved( QUrl::fromEncoded(url) );

				url = ret.toString().toAscii();	
			}

			while ( url.endsWith( '/' ) ) url.remove( url.length() - 1, 1 );

			hrefs_.insert( remove2Dots( url ) );
		}
	}
}

//	==============================================
//	HtmlParser::remove2Dots

QByteArray HtmlParser::remove2Dots( QByteArray &path )
{
	return QUrl().resolved(QUrl(path)).toString().toAscii();
}

//	==============================================
//	HtmlParser::isRelativeURL

bool HtmlParser::isRelativeURL( const char *pc, long len ) const
{
	long pos = 0;

	if(pc[0] == '/')
	{
		return true;
	}

	while ( pos < len && pc[ pos ] != ':' ) pos++;

	// If the colon not found or protocol is too small ( drive 
	// letter on windows ) then url is relative
	if ( pos == len || len < 2 ) return true;

	return false;
}

//	==============================================
//	HtmlParser::setCodec

void HtmlParser::setCodec( QByteArray &charset )
{
	charset_ = charset;
	setCurrentCodec();
}

