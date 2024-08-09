#include "lh_console_util.h"
#include <QtLogging>
#include <QtDebug>
#include <QRegularExpression>

namespace lh_con_util
{
bool LHCUtil::FindAll()
{
    if ( !FindAllRX() )
    {
        qCritical() << "Find All Recievers Failed";
        return false;
    }
    if ( !FindAllTX() )
    {
        qCritical() << "Find All Transmitters Failed";
        return false;
    }
    return true;
}

bool LHCUtil::FindAllRX()
{
    QProcess* lhProcess = new QProcess();
    QString const program = "lighthouse_console.exe";
    lhProcess->start( path_, QStringList() << "" );
    lhProcess->waitForFinished();

    QString const output = QString( lhProcess->readAllStandardOutput() );
    QStringList const outputLines
        = output.split( QRegularExpression( "[\r\n]" ), Qt::SkipEmptyParts );
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
            //  qWarning()  << OutputLine << std::endl;
            RXTX_Pairs_.push_back( RXTX_Pair{ OutputLine, " ", true, false } );
        }
    }
    return !RXTX_Pairs_.empty();
}

bool LHCUtil::FindConnectedTX( QString RXSerial )
{
    qWarning() << RXSerial;
    QProcess* lhProcess = new QProcess();
    lhProcess->start( path_, QStringList() << "/serial" << RXSerial << "exit" );
    lhProcess->waitForFinished();

    QString const output = QString( lhProcess->readAllStandardOutput() );
    QStringList const outputLines
        = output.split( QRegularExpression( "[\r\n]" ), Qt::SkipEmptyParts );
    // std::vector<std::string> OutputLines
    //   = exec( ( "\"" + path_ + "\" /serial " + RXSerial + " exit" ).c_str()
    //   );
    bool lhConFound = false;
    for ( auto outputLine : outputLines )
    {
        //  qWarning()  << outputLine;
        if ( outputLine.contains( "Connected to receiver" ) )
        {
            auto splitList = outputLine.split( ":" );
            QString const TXSerial = splitList[0];
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
                    qWarning()
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
                qWarning() << "tx oops";
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
