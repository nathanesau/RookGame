#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

#include <map>
#include <string>

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
void writePlayerNames(map<int, string> &playerNames);
void writeShowNameTags(bool showNameTags);
void writeShowPartnerToolTip(bool showPartnerToolTip);
void writeScreenWidth(int screenWidth);
void writeScreenHeight(int screenHeight);
} // namespace Appearance

namespace Game
{
int readNumCardsMiddleAllowed();

void writeNumCardsMiddleAllowed(int numCardsAllowed);
} // namespace Game
} // namespace Settings

#endif