#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

#include "player.h"
#include "settings.h"

namespace Settings
{
namespace Appearance
{
float readScaleFactor()
{
    float scaleFactor = 1.0F;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    scaleFactor = settings.value("scaleFactor", 1.0).toFloat();
    settings.endGroup();

    return scaleFactor;
}

map<int, string> readPlayerNames()
{
    map<int, string> playerNames;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    playerNames[PLAYER_1] = settings.value("Player1Name", "Player 1").toString().toStdString();
    playerNames[PLAYER_2] = settings.value("Player2Name", "Player 2").toString().toStdString();
    playerNames[PLAYER_3] = settings.value("Player3Name", "Player 3").toString().toStdString();
    playerNames[PLAYER_4] = settings.value("Player4Name", "Player 4").toString().toStdString();
    settings.endGroup();

    return playerNames;
}

bool readShowNameTags()
{
    bool showNameTags = true;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    showNameTags = settings.value("showNameTags", true).toBool();
    settings.endGroup();

    return showNameTags;
}

bool readShowPartnerToolTip()
{
    bool showPartnerToolTip = false;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    showPartnerToolTip = settings.value("showPartnerToolTip", false).toBool();
    settings.endGroup();

    return showPartnerToolTip;
}

int readScreenWidth()
{
    int screenWidth = QApplication::desktop()->screenGeometry().width();

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    screenWidth = settings.value("screenWidth", QApplication::desktop()->screenGeometry().width()).toInt();
    settings.endGroup();

    return screenWidth;
}

int readScreenHeight()
{
    int screenHeight = QApplication::desktop()->screenGeometry().height();

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    screenHeight = settings.value("screenHeight", QApplication::desktop()->screenGeometry().height()).toInt();
    settings.endGroup();

    return screenHeight;
}

void writeScaleFactor(float scaleFactor)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    settings.setValue("scaleFactor", scaleFactor);
    settings.endGroup();
}

void writePlayerNames(const map<int, string> &playerNames)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");

    auto getPlayerName = [&playerNames](int playerNum)
    {
        auto it = playerNames.find(playerNum);
        string playerName = (it != playerNames.end()) ? it->second : "";
        return QString::fromStdString(playerName);
    };

    settings.setValue("Player1Name", getPlayerName(PLAYER_1));
    settings.setValue("Player2Name", getPlayerName(PLAYER_2));
    settings.setValue("Player3Name", getPlayerName(PLAYER_3));
    settings.setValue("Player4Name", getPlayerName(PLAYER_4));
    settings.endGroup();
}

void writeShowNameTags(bool showNameTags)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    settings.setValue("showNameTags", showNameTags);
    settings.endGroup();
}

void writeShowPartnerToolTip(bool showPartnerToolTip)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    settings.setValue("showPartnerToolTip", showPartnerToolTip);
    settings.endGroup();
}

void writeScreenWidth(int screenWidth)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    settings.setValue("screenWidth", screenWidth);
    settings.endGroup();
}

void writeScreenHeight(int screenHeight)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Appearance");
    settings.setValue("screenHeight", screenHeight);
    settings.endGroup();
}
} // namespace Appearance

namespace Game
{
int readCpuBidAggressionLevel()
{
    int cpuBidAggressionLevel = 1; // mid

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    cpuBidAggressionLevel = settings.value("cpuBidAggressionLevel", 1).toInt();
    settings.endGroup();

    return cpuBidAggressionLevel;
}

int readNumCardsMiddleAllowed()
{
    int numCardsMiddleAllowed = 5;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    numCardsMiddleAllowed = settings.value("numCardsMiddleAllowed", 5).toInt();
    settings.endGroup();

    return numCardsMiddleAllowed;
}

bool readPickSelfAsPartner()
{
    bool pickSelfAsPartner = false;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    pickSelfAsPartner = settings.value("pickSelfAsPartner", false).toBool();
    settings.endGroup();

    return pickSelfAsPartner;
}

bool readPickNestAsPartner()
{
    bool pickNestAsPartner = false;

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    pickNestAsPartner = settings.value("pickNestAsPartner", false).toBool();
    settings.endGroup();

    return pickNestAsPartner;
}

int readCpuRandomnessLevel()
{
    int cpuRandomnessLevel = 1; // low (10%)

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    cpuRandomnessLevel = settings.value("cpuRandomnessLevel", cpuRandomnessLevel).toInt();
    settings.endGroup();

    return cpuRandomnessLevel;
}

void writeCpuBidAggressionLevel(int cpuBidAggressionLevel)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    settings.setValue("cpuBidAggressionLevel", cpuBidAggressionLevel);
    settings.endGroup();
}

void writeNumCardsMiddleAllowed(int numCardsAllowed)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    settings.setValue("numCardsMiddleAllowed", numCardsAllowed);
    settings.endGroup();
}

void writePickSelfAsPartner(bool pickSelfAsPartner)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    settings.setValue("pickSelfAsPartner", pickSelfAsPartner);
    settings.endGroup();
}

void writePickNestAsPartner(bool pickNestAsPartner)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    settings.setValue("pickNestAsPartner", pickNestAsPartner);
    settings.endGroup();
}

void writeCpuRandomnessLevel(int cpuRandomnessLevel)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("Game");
    settings.setValue("cpuRandomnessLevel", cpuRandomnessLevel);
    settings.endGroup();
}
} // namespace Game
} // namespace Settings