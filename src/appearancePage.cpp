#include "appearancePage.h"
#include "mainWindow.h"
#include "settings.h"
#include "utils.h"

// global functions
float getMaxScaleFactor()
{
    auto maxSize = Utils::Ui::getScreenGeometry();
    auto defaultSize = MAIN_WINDOW_SIZE;

    float scaleFactor = std::min(maxSize.width() / (float)defaultSize.width(),
                                 maxSize.height() / (float)defaultSize.height());
    scaleFactor = scaleFactor * MAX_PROPORTION_SCREEN;

    // round to nearest increment
    float maxScaleFactor = MIN_SCALE_FACTOR;
    while (maxScaleFactor < scaleFactor)
    {
        maxScaleFactor += SCALE_FACTOR_INCR;
    }

    return maxScaleFactor - SCALE_FACTOR_INCR;
}

std::string getSizeAsString(const QSize &size)
{
    return std::to_string(size.width()) + "x" + std::to_string(size.height());
}

std::string getScaleFactorAsString(float scaleFactor)
{
    // two decimal places
    int decimalPart = (int)(scaleFactor * 100) % 100;
    std::string decimalStr = (decimalPart < 10) ? (".0" + std::to_string(decimalPart)) : ("." + std::to_string(decimalPart));
    return std::to_string((int)scaleFactor) + decimalStr + "x";
}

int getNumScaleFactorChoices(float minScaleFactor, float maxScaleFactor, float incr)
{
    float scaleFactor = minScaleFactor;

    int nChoices = 0;
    while (scaleFactor <= maxScaleFactor)
    {
        scaleFactor += incr;
        nChoices++;
    }

    return nChoices;
}

void AppearancePage::initializeResolutionGroup()
{
    resolutionLabel = new QLabel;
    resolutionLabel->setText("Game Resolution");

    resolutionComboBox = new QComboBox;

    const float MAX_SCALE_FACTOR = getMaxScaleFactor(); // cannot be determined at compile time

    int nChoices = getNumScaleFactorChoices(MIN_SCALE_FACTOR, MAX_SCALE_FACTOR, SCALE_FACTOR_INCR);

    for (int i = 0; i < nChoices; i++)
    {
        float scaleFactor = MIN_SCALE_FACTOR + i * SCALE_FACTOR_INCR;
        indexScaleFactorMap[i] = scaleFactor;

        auto resolution = QSize(MAIN_WINDOW_SIZE.width() * scaleFactor,
                                MAIN_WINDOW_SIZE.height() * scaleFactor);

        std::string choice = getSizeAsString(resolution) + " (" + getScaleFactorAsString(scaleFactor) + ")";
        resolutionComboBox->addItem(QString::fromStdString(choice));
    }

    float currentScaleFactor = Settings::Appearance::readScaleFactor();
    int currentIndex = 0;

    for (auto &it : indexScaleFactorMap)
    {
        if (abs(it.second - currentScaleFactor) < 1e-06) // equal operator not reliable for float
        {
            currentIndex = it.first;
            break;
        }
    }

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

AppearancePage::AppearancePage(QWidget *parent) : QWidget(parent)
{
    initializeResolutionGroup();
    initializeNamesGroup();
    initializeHUDGroup();
    initializeScreenGroup();

    applyButton = new QPushButton;
    applyButton->setText("Apply");
    connect(applyButton, &QAbstractButton::clicked, this, &AppearancePage::onApply);

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

    emit gameResolutionChanged();
}

void AppearancePage::applyPlayerNames()
{
    std::map<int, std::string> playerNames;

    playerNames[PLAYER_1] = player1NameEdit->text().toStdString();
    playerNames[PLAYER_2] = player2NameEdit->text().toStdString();
    playerNames[PLAYER_3] = player3NameEdit->text().toStdString();
    playerNames[PLAYER_4] = player4NameEdit->text().toStdString();
    Settings::Appearance::writePlayerNames(playerNames);
}

void AppearancePage::applyHUD()
{
    bool showNameTags = showNameTagsBox->isChecked();
    bool showPartnerToolTip = showPartnerToolTipBox->isChecked();

    Settings::Appearance::writeShowNameTags(showNameTags);
    Settings::Appearance::writeShowPartnerToolTip(showPartnerToolTip);
    
    emit nameTagsChanged();
}

void AppearancePage::applyScreenDimensions()
{
    int screenWidth = screenWidthLineEdit->text().toInt();
    int screenHeight = screenHeightLineEdit->text().toInt();

    // don't re-position existing widgets... use geometry to position future widgets
    Settings::Appearance::writeScreenWidth(screenWidth);
    Settings::Appearance::writeScreenHeight(screenHeight);
}
