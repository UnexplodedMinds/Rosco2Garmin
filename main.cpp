/*
Rosco2Garmin
(c) 2018 Allen K. Lair, Unexploded Minds
*/

#include <QCoreApplication>

#include "Translator.h"


int main( int argc, char *argv[] )
{
    QCoreApplication coreApp( argc, argv );
    Translator       trans;

    // Not really but it prevents IDE warnings
    Q_UNUSED( trans )

    QCoreApplication::setOrganizationName( "Unexploded Minds" );
    QCoreApplication::setOrganizationDomain( "unexplodedminds.com" );
    QCoreApplication::setApplicationName( "Rosco2Garmin" );

    qDebug() << "\nRosco2Garmin (c) 2018 Unexploded Minds\n";

    return coreApp.exec();
}
