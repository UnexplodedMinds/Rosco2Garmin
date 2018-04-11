/*
Stratux AHRS Display
(c) 2018 Allen K. Lair, Unexploded Minds
*/

#include <QTimer>
#include <QDateTime>
#include <QNetworkDatagram>
#include <QTimerEvent>

#include "Translator.h"
#include "StreamReader.h"


// Setup minimal UI elements and make the connections
Translator::Translator()
    : QObject( 0 ),
      m_pStratuxStream( new StreamReader( this ) ),
      m_bAHRS( false ),
      m_bGPS( false ),
      m_bStratux( false ),
      m_bSendStream( false )
{
    m_lastStatusUpdate = QDateTime::currentDateTime();

    connect( m_pStratuxStream, SIGNAL( newSituation( StratuxSituation ) ), this, SLOT( situation( StratuxSituation ) ) );
    connect( m_pStratuxStream, SIGNAL( newStatus( bool, bool, bool ) ), this, SLOT( status( bool, bool, bool ) ) );
    connect( m_pStratuxStream, SIGNAL( newTraffic( int, StratuxTraffic ) ), this, SLOT( traffic( int, StratuxTraffic ) ) );
    m_pStratuxStream->connectStreams();

    m_roscoSocket.bind( QHostAddress( "192.168.10.1" ), 49010 );
    connect( &m_roscoSocket, SIGNAL( readyRead() ), this, SLOT( roscoMessage() ) );

    // We don't care what the ID is since it's the one and only timer for this class and never gets killed
    m_iRoutineTimer = startTimer( 1000 );
}


// Delete the stream reader
Translator::~Translator()
{
    delete m_pStratuxStream;
    m_pStratuxStream = 0;
}


// Status stream is received here instead of the canvas since here is where the indicators are
void Translator::status( bool bStratux, bool bAHRS, bool bGPS )
{
    m_bStratux = bStratux;
    m_bAHRS = bAHRS;
    m_bGPS = bGPS;
    m_lastStatusUpdate = QDateTime::currentDateTime();
}


void Translator::timerEvent( QTimerEvent *pEvent )
{
    if( pEvent == 0 )
        return;

    if( pEvent->timerId() == m_iRoutineTimer )
    {
        // If we're not connected or we haven't gotten a status update for over ten seconds, force a reconnect
        if( (!m_pStratuxStream->isConnected()) || (m_lastStatusUpdate.secsTo( QDateTime::currentDateTime() ) > 10) )
        {
            m_pStratuxStream->disconnectStreams();
            m_pStratuxStream->connectStreams();
            qDebug() << "Stratux local websockets reconnected";
        }
    }
}


// Situation update (includes GPS position and AHRS data)
void Translator::situation( StratuxSituation s )
{
    if( !m_bSendStream )
        return;

    if( m_bStratux && m_bAHRS && m_bGPS )
    {
        double    dMag = s.dAHRSMagHeading + 180.0;

        while( dMag > 360.0 )
            dMag -= 360.0;

        // GPS Position
        QString qsGPS = QString( "XGPS1,%1,%2,%3,%4,%5" )
                            .arg( s.dGPSlong, 0, 'f', 4 )
                            .arg( s.dGPSlat, 0, 'f', 4 )
                            .arg( s.dBaroPressAlt / 3.28084, 0, 'f', 1 )
                            .arg( dMag, 0, 'f', 1 )
                            .arg( s.dGPSGroundSpeed * 0.539957 * 100.0, 0, 'f', 4 );

        m_xplaneSocket.writeDatagram( qsGPS.toLatin1(), QHostAddress( "192.168.10.255" ), 49002 );
        m_xplaneSocket.waitForBytesWritten( 250 );

        // Attitude - the only things supported are heading, pitch, roll and slip/skid (not sure what the rest are)
        QString qsAtt = QString( "XATT1,%1,%2,%3,0.0,0.0,0.0,0.0,0.0,0.0,%4,0.0,0.0" )
                            .arg( s.dAHRSMagHeading, 0, 'f', 4 )
                            .arg( s.dAHRSpitch, 0, 'f', 4 )
                            .arg( s.dAHRSroll, 0, 'f', 4 )
                            .arg( s.dAHRSSlipSkid * 0.01, 0, 'f', 6 );

        m_xplaneSocket.writeDatagram( qsAtt.toLatin1(), QHostAddress( "192.168.10.255" ), 49002 );
        m_xplaneSocket.waitForBytesWritten( 250 );
    }
}


// Traffic update
void Translator::traffic( int iICAO, StratuxTraffic t )
{
    if( !m_bSendStream )
        return;

    QMapIterator<int, StratuxTraffic> it( m_trafficMap );

    Q_UNUSED( iICAO )

    QString qsTraffic = QString( "XTRA1,%1,%2,%3,%4,%5,%6,%7,%8,%9" )
                    .arg( t.iICAO )
                    .arg( t.dLat, 0, 'f', 8 )
                    .arg( t.dLong, 0, 'f', 8 )
                    .arg( t.dAlt, 0, 'f', 8 )
                    .arg( t.dVertSpeed, 0, 'f', 8 )
                    .arg( t.bOnGround ? 0 : 1 )
                    .arg( t.dBearing, 0, 'f', 8 )
                    .arg( t.dSpeed, 0, 'f', 8 )
                    .arg( t.qsTail );
    m_xplaneSocket.writeDatagram( qsTraffic.toLatin1(), QHostAddress( "192.168.10.255" ), 49002 );
    m_xplaneSocket.waitForBytesWritten( 250 );
}


// Message from the rosco app that can turn on/off GPS and AHRS data from the stratux
void Translator::roscoMessage()
{
    QNetworkDatagram msg = m_roscoSocket.receiveDatagram();
    QString qsMsg = msg.data();

    qDebug() << "Rosco Message Received";

    if( qsMsg == "EnableGarmin" )
    {
        m_bSendStream = (!m_bSendStream);
        qDebug() << "Garmin Stream:" << (m_bSendStream ? "Enabled" : "Disabled");
    }
}


