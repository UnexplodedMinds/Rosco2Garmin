/*
Stratux AHRS Display
(c) 2018 Allen K. Lair, Unexploded Minds
*/

#ifndef __STREAMREADER_H__
#define __STREAMREADER_H__

#include <QObject>
#include <QWebSocket>

#include "StratuxStreams.h"


class QCoreApplication;


class StreamReader : public QObject
{
    Q_OBJECT

public:
    explicit StreamReader( QObject *parent );
    ~StreamReader();

    void connectStreams();
    void disconnectStreams();
    bool isConnected() { return m_bConnected; }

    void initSituation( StratuxSituation &situation );
    void initStatus( StratuxStatus &status );
    void initTraffic( StratuxTraffic &traffic );

private:
    bool          m_bHaveMyPos;
    bool          m_bAHRSStatus;
    bool          m_bStratuxStatus;
    bool          m_bGPSStatus;
    QWebSocket    m_stratuxSituation;
    QWebSocket    m_stratuxStatus;
    QWebSocket    m_stratuxTraffic;
    double        m_dMyLat;
    double        m_dMyLong;
    bool          m_bConnected;

private slots:
    void situationUpdate( const QString &qsMessage );
    void statusUpdate( const QString &qsMessage );
    void trafficUpdate( const QString &qsMessage );
    void stratuxConnected();
    void stratuxDisconnected();

signals:
    void newSituation( StratuxSituation );
    void newStatus( bool, bool, bool );         // Stratux available, AHRS available, GPS available, Traffic available, Weather available
    void newTraffic( int, StratuxTraffic );     // ICAO, Rest of traffic struct
};

#endif // __STREAMREADER_H__
