////////////////////////////////////////////////////////////////////////////
//
// nrMaterial.cpp
//
// A class for materials.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrMaterial.h"

#include "nrChannel.h"
#include "nrChannelColor.h"
#include "nrChannelMarble.h"
#include "nrLibrary.h"
#include "nrNoise.h"
#include "nrParser.h"


////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

nrLibrary<nrMaterial> nrMaterial::m_Library;


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrMaterial::nrMaterial( void )
{
    m_Name = 0;
    m_Ambient = 0;
    m_Diffuse = 0;
    m_Specular = 0;
}

////////////////////////////////////////////////////////////////////////////

nrMaterial::~nrMaterial( void )
{
    delete [] m_Name;
	delete m_Ambient;
	delete m_Diffuse;
	delete m_Specular;
}

////////////////////////////////////////////////////////////////////////////

void nrMaterial::SetName( const char* name )
{
    delete [] m_Name;
    m_Name = 0;
    
    if ( name )
    {
        m_Name = new char[ strlen( name ) + 1 ];
        strcpy( m_Name, name );
    }
}

////////////////////////////////////////////////////////////////////////////

const char* nrMaterial::GetName( void ) const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////

nrColor nrMaterial::Ambient( const nrVector3& point ) const
{
	if ( m_Ambient != 0 )
	{
		return m_Ambient->Color( point );
	}
	else
	{
		return nrColor( 0, 0, 0 );
	}
}

////////////////////////////////////////////////////////////////////////////

nrColor nrMaterial::Diffuse( const nrVector3& point ) const
{
	if ( m_Diffuse != 0 )
	{
		return m_Diffuse->Color( point );
	}
	else
	{
		return nrColor( 1, 1, 1 );
	}
}

////////////////////////////////////////////////////////////////////////////

nrColor nrMaterial::Specular( const nrVector3& point ) const
{
	if ( m_Specular != 0 )
	{
		return m_Specular->Color( point );
	}
	else
	{
		return nrColor( 0, 0, 0 );
	}
}

////////////////////////////////////////////////////////////////////////////

nrMaterial* nrMaterial::ParseColor( nrParser& parser )
{
    nrColor color;
    
    parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
    color.r = parser.ReadFloat();
    color.g = parser.ReadFloat();
    color.b = parser.ReadFloat();
    parser.ReadToken( nrParser::TOKEN_LIST_END );
    
    if ( parser.Error() )
    {
        return 0;
    }
    
    nrMaterial* material = new nrMaterial();
	material->m_Ambient = new nrChannelColor( color );
	material->m_Diffuse = new nrChannelColor( color );
    
    m_Library.Add( material->m_Name, material );
    
    return material;
}

////////////////////////////////////////////////////////////////////////////

nrMaterial* nrMaterial::Parse( nrParser& parser )
{
    nrMaterial* material = new nrMaterial;
	if ( material == 0 )
	{
		parser.ParseError( "out of memory" );
		return 0;
	}
	
    if ( parser.NextToken() == nrParser::TOKEN_STRING )
    {
		const char* name = parser.ReadString();
		if ( name )
		{
			nrMaterial* m = m_Library.Find( name );
			if ( m )
			{
				return m;
			}
			
			material->SetName( name );
			m_Library.Add( name, material );
		}
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "ambient" ) )
        {
			if ( parser.NextToken() == nrParser::TOKEN_AGGREGATE_BEGIN )
			{
				material->m_Ambient = nrChannel::Parse( parser );
			}
			else
			{
				parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
                float r = parser.ReadFloat();
                float g = parser.ReadFloat();
                float b = parser.ReadFloat();
				material->m_Ambient = new nrChannelColor( nrColor( r, g, b ) );
				parser.ReadToken( nrParser::TOKEN_LIST_END );
			}
        }
        else if ( parser.KeyMatches( key, "diffuse" ) )
        {
			if ( parser.NextToken() == nrParser::TOKEN_AGGREGATE_BEGIN )
			{
				material->m_Diffuse = nrChannel::Parse( parser );
			}
			else
			{
				parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
                float r = parser.ReadFloat();
                float g = parser.ReadFloat();
                float b = parser.ReadFloat();
				material->m_Diffuse = new nrChannelColor( nrColor( r, g, b ) );
				parser.ReadToken( nrParser::TOKEN_LIST_END );
			}
        }
        else if ( parser.KeyMatches( key, "specular" ) )
        {
			if ( parser.NextToken() == nrParser::TOKEN_AGGREGATE_BEGIN )
			{
				material->m_Specular = nrChannel::Parse( parser );
			}
			else
			{
				parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
                float r = parser.ReadFloat();
                float g = parser.ReadFloat();
                float b = parser.ReadFloat();
				material->m_Specular = new nrChannelColor( nrColor( r, g, b ) );
				parser.ReadToken( nrParser::TOKEN_LIST_END );
			}
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
    
    if ( parser.Error() )
    {
        return 0;
    }
    
    return material;
}

////////////////////////////////////////////////////////////////////////////
