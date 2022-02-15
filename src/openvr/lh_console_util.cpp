#include "lh_console_util.h"

namespace lh_con_util
{
bool LHCUtil::FindAll()
{
    if ( !FindAllRX() )
    {
        LOG( ERROR ) << "Find All Recievers Failed";
        return false;
    }
    if ( !FindAllTX() )
    {
        LOG( ERROR ) << "Find All Transmitters Failed";
        return false;
    }
    return true;
}

bool LHCUtil::FindAllRX()
{
    QProcess* lhProcess = new QProcess();
    QString program = "lighthouse_console.exe";
    lhProcess->start( path_, QStringList() << "" );
    lhProcess->waitForFinished();

    QString output = QString( lhProcess->readAllStandardOutput() );
    QStringList outputLines
        = output.split( QRegExp( "[\r\n]" ), QString::SkipEmptyParts );
    bool RecieverStart = false;

    for ( auto OutputLine : outputLines )
    {
        if ( OutputLine.contains( "Attached lighthouse receiver devices:" ) )
        {
            RecieverStart = true;
            continue;
        }
        if ( RecieverStart )
        {
            if ( OutputLine.contains( "lighthouse_console" ) )
            {
                break;
            }
            OutputLine.simplified().remove( " " );
            OutputLine.remove( 0, 1 );
            // LOG( WARNING ) << OutputLine << std::endl;
            RXTX_Pairs_.push_back( RXTX_Pair{ OutputLine, " ", true, false } );
        }
    }
    return !RXTX_Pairs_.empty();
}

bool LHCUtil::FindConnectedTX( QString RXSerial )
{
    LOG( WARNING ) << RXSerial;
    QProcess* lhProcess = new QProcess();
    lhProcess->start( path_, QStringList() << "/serial" << RXSerial << "exit" );
    lhProcess->waitForFinished();

    QString output = QString( lhProcess->readAllStandardOutput() );
    QStringList outputLines
        = output.split( QRegExp( "[\r\n]" ), QString::SkipEmptyParts );
    // std::vector<std::string> OutputLines
    //   = exec( ( "\"" + path_ + "\" /serial " + RXSerial + " exit" ).c_str()
    //   );
    bool lhConFound = false;
    for ( auto outputLine : outputLines )
    {
        // LOG( WARNING ) << outputLine;
        if ( outputLine.contains( "Connected to receiver" ) )
        {
            auto splitList = outputLine.split( ":" );
            QString TXSerial = splitList[0];
            if ( TXSerial == "lighthouse_console" )
            {
                lhConFound = true;
                continue;
            }
            for ( auto& rxtx : RXTX_Pairs_ )
            {
                if ( rxtx.RX_Serial == RXSerial )
                {
                    rxtx.TX_Serial = TXSerial;
                    rxtx.Is_Paired = true;
                    rxtx.Is_Init = true;
                    LOG( WARNING )
                        << "tx serial stored: " << TXSerial.toStdString();
                    return true;
                }
            }
            // TODO error handling
            return false;
        }
    }
    if ( lhConFound )
    {
        for ( auto rxtx : RXTX_Pairs_ )
        {
            if ( rxtx.RX_Serial == RXSerial )
            {
                rxtx.TX_Serial = "";
                rxtx.Is_Paired = false;
                rxtx.Is_Init = true;
                LOG( WARNING ) << "tx oops";
                return true;
            }
        }
    }
    // TODO error handling
    return false;
}

bool LHCUtil::FindAllTX()
{
    bool NoError = true;
    for ( auto rxtx : RXTX_Pairs_ )
    {
        if ( !FindConnectedTX( rxtx.RX_Serial ) )
        {
            NoError = false;
        }
    }
    return NoError;
}

QString LHCUtil::GetLinkedTX( QString RXSerial )
{
    for ( auto rxtx : RXTX_Pairs_ )
    {
        if ( rxtx.RX_Serial == RXSerial )
        {
            return rxtx.TX_Serial;
        }
    }
    return "";
}

QString LHCUtil::GetLinkedRX( QString TXSerial )
{
    for ( auto rxtx : RXTX_Pairs_ )
    {
        if ( rxtx.TX_Serial == TXSerial )
        {
            return rxtx.RX_Serial;
        }
    }
    return "";
}

} // namespace lh_con_util
