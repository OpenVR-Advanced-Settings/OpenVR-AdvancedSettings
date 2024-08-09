#pragma once
#include <QProcess>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <string>
#include <utility>
#include <vector>

namespace lh_con_util
{
struct RXTX_Pair
{
    QString RX_Serial = "";
    QString TX_Serial = "";
    bool Is_Init = false;
    bool Is_Paired = false;
};
class LHCUtil
{
private:
    QString path_;
    bool InPairs( std::string RXSerial );
    bool FindAllRX();
    bool FindAllTX();
    bool path_Init = false;
    std::vector<RXTX_Pair> RXTX_Pairs_;

public:
    explicit LHCUtil( QString path ) : path_( std::move( path ) ) {}
    bool FindAll();
    bool FindConnectedTX( QString RXSerial );
    QString GetLinkedTX( QString RXSerial );
    QString GetLinkedRX( QString TXSerial );
    RXTX_Pair GetConnectedTXRXPair();
    bool pairDevice( QString RXSerial );
};
} // namespace lh_con_util
