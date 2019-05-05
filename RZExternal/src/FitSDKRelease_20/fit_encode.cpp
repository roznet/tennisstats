////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Garmin Canada Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2018 Garmin Canada Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 20.80Release
// Tag = production/akw/20.80.00-0-g64ad259
////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "fit_encode.hpp"
#include "fit_crc.hpp"

namespace fit
{
Encode::Encode()
    : Encode( ProtocolVersion::V10 ) {}

Encode::Encode( ProtocolVersion version )
    : dataSize( 0 )
      , file( nullptr )
      , version( version )
      , validator( version ) {}

void Encode::Open(std::iostream &file)
{
    this->file = &file;
    WriteFileHeader();
}

void Encode::Write( const MesgDefinition& mesgDef )
{
    if ( !file )
        return;

    if ( !validator.ValidateMesgDefn( mesgDef ) )
        return;

    dataSize += mesgDef.Write( *file );
    lastMesgDefinition[mesgDef.GetLocalNum()] = mesgDef;
}

void Encode::Write( const Mesg& mesg )
{
    MesgDefinition mesgDefinition( mesg );

    if ( !file )
        return;

    if ( !validator.ValidateMesg( mesg ) )
        return;

    if ( !lastMesgDefinition[mesg.GetLocalNum()].Supports( mesgDefinition ) )
        Write( mesgDefinition );

    dataSize += mesg.Write( *file, &( lastMesgDefinition[mesg.GetLocalNum()] ) );
}

void Encode::Write( const std::vector<Mesg>& mesgs )
{
    for ( std::vector<Mesg>::size_type i = 0; i < mesgs.size(); i++ )
        Write( mesgs[i] );
}

FIT_BOOL Encode::Close( void )
{
    FIT_UINT32 fileBytesLeft = FIT_FILE_HDR_SIZE + dataSize;
    FIT_UINT16 crc = 0;

    if ( !file )
        return FIT_FALSE;

    WriteFileHeader();

    file->seekg( 0, std::ios::beg );

    while ( fileBytesLeft > 0 )
    {
        FIT_UINT8 byte = ( FIT_UINT8 )file->get();

        if ( file->eof() )
            return FIT_FALSE; // File smaller than expected.

        crc = CRC::Get16( crc, byte );
        fileBytesLeft--;
    }

    file->seekp( 0, std::ios::end );
    file->put( crc & 0xFF );
    file->put( ( char )( crc >> 8 ) );

    return FIT_TRUE;
}

void Encode::OnMesg( Mesg& mesg )
{
    Write( mesg );
}

void Encode::OnMesgDefinition( MesgDefinition& mesgDef )
{
    Write( mesgDef );
}

void Encode::WriteFileHeader()
{
    FIT_FILE_HDR file_header;

    if ( !file )
        return;

    file_header.header_size = FIT_FILE_HDR_SIZE;
    file_header.profile_version = FIT_PROFILE_VERSION;
    file_header.protocol_version = fit::versionMap.at( version ).GetVersionByte();
    memcpy( ( FIT_UINT8 * )&file_header.data_type, ".FIT", 4 );
    file_header.data_size = dataSize;
    file_header.crc = CRC::Calc16( &file_header, FIT_STRUCT_OFFSET(crc, FIT_FILE_HDR) );

    file->seekp( 0, std::ios::beg );
    file->write( ( const char * )&file_header, FIT_FILE_HDR_SIZE );
}
} // namespace fit
