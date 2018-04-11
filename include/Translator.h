/*
Stratux AHRS Display
(c) 2018 Allen K. Lair, Unexploded Minds
*/

#ifndef __AHRSMAINWIN_H__
#define __AHRSMAINWIN_H__

#include <QObject>
#include <QUdpSocket>
#include <QDateTime>

#include "StratuxStreams.h"


class StreamReader;


class Translator : public QObject
{
    Q_OBJECT

public:
    explicit Translator();
    ~Translator();

protected:
    void timerEvent( QTimerEvent *pEvent );

private:
    StreamReader             *m_pStratuxStream;
    bool                      m_bAHRS;
    bool                      m_bGPS;
    bool                      m_bStratux;
    QUdpSocket                m_xplaneSocket;
    QUdpSocket                m_roscoSocket;
    QDateTime                 m_lastStatusUpdate;
    QMap<int, StratuxTraffic> m_trafficMap;
    bool                      m_bSendStream;
    int                       m_iRoutineTimer;

private slots:
    void situation( StratuxSituation s );
    void status( bool bStratux, bool bAHRS, bool bGPS );
    void traffic( int iICAO, StratuxTraffic t );
    void roscoMessage();
};

#endif // __AHRSMAINWIN_H__
