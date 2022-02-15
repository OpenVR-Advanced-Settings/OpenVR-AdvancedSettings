#pragma once
#include "openvr.h"
#include <vector>
#include <utility>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <easylogging++.h>

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

public:
    explicit LHCUtil( QString path )
    {
        path_ = path;
    }
    std::vector<RXTX_Pair> RXTX_Pairs_;
    bool FindAll();
    bool FindConnectedTX( QString RXSerial );
    QString GetLinkedTX( QString RXSerial );
    QString GetLinkedRX( QString TXSerial );
    RXTX_Pair GetConnectedTXRXPair();
    bool pairDevice( QString RXSerial );
};
} // namespace lh_con_util
