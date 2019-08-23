#ifndef SETTINGS_H
#define SETTINGS_H

#include <map>
#include <QString>
#include <string>

#include "common.h"

using namespace std;

const QString ORGANIZATION_NAME = "OpenSourceSoftware";
const QString APPLICATION_NAME = "Rook";

namespace Settings
{
namespace Appearance
{
float readScaleFactor();
map<int, string> readPlayerNames();
bool readShowNameTags();
bool readShowPartnerToolTip();
int readScreenWidth();
int readScreenHeight();

void writeScaleFactor(float scaleFactor);
void writePlayerNames(const map<int, string> &playerNames);
void writeShowNameTags(bool showNameTags);
void writeShowPartnerToolTip(bool showPartnerToolTip);
void writeScreenWidth(int screenWidth);
void writeScreenHeight(int screenHeight);
} // namespace Appearance

namespace Game
{
int readCpuBidAggressionLevel();
int readNumCardsMiddleAllowed();
bool readPickSelfAsPartner();
bool readPickNestAsPartner();
int readCpuRandomnessLevel();

void writeCpuBidAggressionLevel(int cpuBidAggressionLevel);
void writeNumCardsMiddleAllowed(int numCardsAllowed);
void writePickSelfAsPartner(bool pickSelfAsPartner);
void writePickNestAsPartner(bool pickNestAsPartner);
void writeCpuRandomnessLevel(int cpuRandomnessLevel);
} // namespace Game
} // namespace Settings

#endif