/*
Stratux AHRS Display
(c) 2018 Allen K. Lair, Unexploded Minds
*/

#ifndef __STRATUXSTREAMS_H__
#define __STRATUXSTREAMS_H__

#include <QDateTime>
#include <QString>


struct StratuxSituation
{
    double    dLastGPSFixSinceMidnight;
    double    dGPSlat;
    double    dGPSlong;
    int       iGPSFixQuality;
    double    dGPSHeightAboveEllipsoid;
    double    dGPSGeoidSep;
    int       iGPSSats;
    int       iGPSSatsTracked;
    int       iGPSSatsSeen;
    double    dGPSHorizAccuracy;
    int       iGPSNACp;
    double    dGPSAltMSL;
    double    dGPSVertAccuracy;
    double    dGPSVertSpeed;
    QDateTime lastGPSFixTime;
    double    dGPSTrueCourse;
    double    dGPSTurnRate;
    double    dGPSGroundSpeed;
    QDateTime lastGPSGroundTrackTime;
    QDateTime gpsDateTime;
    QDateTime lastGPSTimeStratuxTime;
    QDateTime lastValidNMEAMessageTime;
    QString   qsLastNMEAMsg;
    int       iGPSPosSampleRate;
    double    dBaroTemp;
    double    dBaroPressAlt;
    double    dBaroVertSpeed;
    QDateTime lastBaroMeasTime;
    double    dAHRSpitch;
    double    dAHRSroll;
    double    dAHRSGyroHeading;
    double    dAHRSMagHeading;
    double    dAHRSSlipSkid;
    double    dAHRSTurnRate;
    double    dAHRSGLoad;
    double    dAHRSGLoadMin;
    double    dAHRSGLoadMax;
    QDateTime lastAHRSAttTime;
    int       iAHRSStatus;
};


// Traffic struct
// NOTE ICAO is deliberately missing since it's used in a map in a higher level class to differentiate aircraft
struct StratuxTraffic
{
    int       iICAO;
    QString   qsReg;
    double    dSigLevel;
    int       iSquawk;
    bool      bOnGround;
    double    dLat;
    double    dLong;
    bool      bPosValid;
    double    dAlt;
    double    dTrack;
    double    dSpeed;
    double    dVertSpeed;
    QString   qsTail;
    QDateTime lastSeen;
    QDateTime timestamp;
    int       iLastSource;
    double    dBearing;
    double    dDist;
    double    dAge;
    bool      bHasADSB;
};


struct StratuxStatus
{
    int  iUATTrafficTracking;
    int  iESTrafficTracking;
    int  iGPSSatsLocked;
    bool bGPSConnected;
    int  iUATMETARTotal;
    int  iUATTAFTotal;
    int  iUATNEXRADTotal;
    int  iUATSIGMETTotal;
    int  iUATPIREPTotal;
};


struct StratuxWeather
{
    QString   qsType;
    QString   qsLocation;
    QDateTime prodTime;
    QString   qsData;
    QString   qsLastMessage;    // This is for testing only
};

#endif // __STRATUXSTREAMS_H__
