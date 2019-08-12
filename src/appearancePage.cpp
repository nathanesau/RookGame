#include <QSettings>
#include <vector>

#include "AppearancePage.h"
#include "mainWindow.h"
#include "settings.h"
#include "utils.h"

using namespace std;

// global functions
float getMaxScaleFactor()
{
    QRect maxSize = Utils::Ui::getScreenGeometry();

    float scaleFactor = min(maxSize.width() / 1200.0F, maxSize.height() / 850.0F);
    scaleFactor = scaleFactor * MAX_PROPORTION_SCREEN;
    scaleFactor = round(scaleFactor * 10) / 10.0F;

    return scaleFactor;
}

string getSizeAsString(const QSize &size)
{
    return to_string(size.width()) + "x" + to_string(size.height());
}

string getScaleFactorAsString(float scaleFactor)
{
    return to_string((int)scaleFactor) + "." + to_string(((int)(scaleFactor * 10)) % 10) + "x";
}

void AppearancePage::initializeResolutionGroup()
{
    resolutionLabel = new QLabel;
    resolutionLabel->setText("Game Resolution");

    resolutionComboBox = new QComboBox;

    float minScaleFactor = MIN_SCALE_FACTOR;
    float maxScaleFactor = getMaxScaleFactor();
    int nChoices = (maxScaleFactor - minScaleFactor) / 0.1F;

    for (int i = 0; i < nChoices; i++)
    {
        float scaleFactor = MIN_SCALE_FACTOR + i / 10.0F;
        indexScaleFactorMap[i] = scaleFactor;

        QSize resolution = QSize((int) 1200 * scaleFactor, (int) 850 * scaleFactor);
        string choice = getSizeAsString(resolution) + " (" + getScaleFactorAsString(scaleFactor) + ")";
        resolutionComboBox->addItem(QString::fromStdString(choice));
    }

    int currentIndex = round((Settings::Appearance::readScaleFactor() - MIN_SCALE_FACTOR) * 10.0F);
    resolutionComboBox->setCurrentIndex(currentIndex);

    resolutionGroup = new QGroupBox;
    resolutionGroup->setTitle("Resolution");

    resolutionLayout = new QHBoxLayout;
    resolutionLayout->addWidget(resolutionLabel);
    resolutionLayout->addWidget(resolutionComboBox);
    resolutionGroup->setLayout(resolutionLayout);
}

void AppearancePage::initializeNamesGroup()
{
    player1NameLabel = new QLabel;
    player1NameLabel->setText("Player 1");

    player2NameLabel = new QLabel;
    player2NameLabel->setText("Player 2");

    player3NameLabel = new QLabel;
    player3NameLabel->setText("Player 3");

    player4NameLabel = new QLabel;
    player4NameLabel->setText("Player 4");

    auto playerNames = Settings::Appearance::readPlayerNames();

    player1NameEdit = new QLineEdit;
    player1NameEdit->setText(QString::fromStdString(playerNames[PLAYER_1]));

    player2NameEdit = new QLineEdit;
    player2NameEdit->setText(QString::fromStdString(playerNames[PLAYER_2]));

    player3NameEdit = new QLineEdit;
    player3NameEdit->setText(QString::fromStdString(playerNames[PLAYER_3]));

    player4NameEdit = new QLineEdit;
    player4NameEdit->setText(QString::fromStdString(playerNames[PLAYER_4]));

    namesGroup = new QGroupBox;
    namesGroup->setTitle("Names");

    namesLayout = new QVBoxLayout;
    namesLayout->addWidget(player1NameLabel);
    namesLayout->addWidget(player1NameEdit);
    namesLayout->addWidget(player2NameLabel);
    namesLayout->addWidget(player2NameEdit);
    namesLayout->addWidget(player3NameLabel);
    namesLayout->addWidget(player3NameEdit);
    namesLayout->addWidget(player4NameLabel);
    namesLayout->addWidget(player4NameEdit);
    namesGroup->setLayout(namesLayout);
}

void AppearancePage::initializeHUDGroup()
{
    showNameTagsBox = new QCheckBox;
    showNameTagsBox->setText("Show name tags");
    showNameTagsBox->setChecked(Settings::Appearance::readShowNameTags());

    showPartnerToolTipBox = new QCheckBox;
    showPartnerToolTipBox->setText("Show partner tool tip");
    showPartnerToolTipBox->setChecked(Settings::Appearance::readShowPartnerToolTip());

    hudGroup = new QGroupBox;
    hudGroup->setTitle("HUD");

    hudLayout = new QVBoxLayout;
    hudLayout->addWidget(showNameTagsBox);
    hudLayout->addWidget(showPartnerToolTipBox);
    hudGroup->setLayout(hudLayout);
}

void AppearancePage::initializeScreenGroup()
{
    screenWidthLineEdit = new QLineEdit;
    screenWidthLineEdit->setText(QString::number(Settings::Appearance::readScreenWidth()));

    screenHeightLineEdit = new QLineEdit;
    screenHeightLineEdit->setText(QString::number(Settings::Appearance::readScreenHeight()));

    screenGroup = new QGroupBox;
    screenGroup->setTitle("Screen Dimensions");
    screenGroup->setToolTip("These are used to positions widgets on screen (defaults may need to be overriden for WSL, etc.)");

    screenLayout = new QVBoxLayout;
    screenLayout->addWidget(screenWidthLineEdit);
    screenLayout->addWidget(screenHeightLineEdit);
    screenGroup->setLayout(screenLayout);
}

AppearancePage::AppearancePage(MainWindow *pMainWindow, QWidget *parent) : mainWindow(pMainWindow),
                                                                           QWidget(parent)
{
    initializeResolutionGroup();
    initializeNamesGroup();
    initializeHUDGroup();
    initializeScreenGroup();

    applyButton = new QPushButton;
    applyButton->setText("Apply");
    QObject::connect(applyButton, &QAbstractButton::clicked, this, &AppearancePage::onApply);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(resolutionGroup);
    mainLayout->addWidget(namesGroup);
    mainLayout->addWidget(hudGroup);
    mainLayout->addWidget(screenGroup);
    mainLayout->addStretch(1);
    mainLayout->addWidget(applyButton);

    setLayout(mainLayout);
}

void AppearancePage::onApply()
{
    applyResolution();
    applyPlayerNames();
    applyHUD();
    applyScreenDimensions();
}

void AppearancePage::applyResolution()
{
    int index = resolutionComboBox->currentIndex();
    float scaleFactor = indexScaleFactorMap[index];

    if (scaleFactor == Settings::Appearance::readScaleFactor())
    {
        return;
    }

    Settings::Appearance::writeScaleFactor(scaleFactor);

    mainWindow->rescale();
}

void AppearancePage::applyPlayerNames()
{
    map<int, string> playerNames;

    playerNames[PLAYER_1] = player1NameEdit->text().toStdString();
    playerNames[PLAYER_2] = player2NameEdit->text().toStdString();
    playerNames[PLAYER_3] = player3NameEdit->text().toStdString();
    playerNames[PLAYER_4] = player4NameEdit->text().toStdString();
    Settings::Appearance::writePlayerNames(playerNames);

    mainWindow->updatePlayerNames(playerNames);
}

void AppearancePage::applyHUD()
{
    bool showNameTags = showNameTagsBox->isChecked();
    bool showPartnerToolTip = showPartnerToolTipBox->isChecked();

    Settings::Appearance::writeShowNameTags(showNameTags);
    Settings::Appearance::writeShowPartnerToolTip(showPartnerToolTip);
    mainWindow->updateNameTags(showNameTags);
}

void AppearancePage::applyScreenDimensions()
{
    int screenWidth = screenWidthLineEdit->text().toInt();
    int screenHeight = screenHeightLineEdit->text().toInt();

    // don't re-position existing widgets... use geometry to position future widgets
    Settings::Appearance::writeScreenWidth(screenWidth);
    Settings::Appearance::writeScreenHeight(screenHeight);
}