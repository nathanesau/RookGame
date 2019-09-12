#include "cpu.h"
#include "gameData.h"
#include "mainWidget.h"
#include "messageBox.h"
#include "roundSummaryDialog.h"
#include "settings.h"
#include "utils.h"

PlayerNameLabel::PlayerNameLabel(const Qt::AlignmentFlag pAlign, QWidget *parent) : align(pAlign),
                                                                                        QWidget(parent)
{
    scaleFactor = Settings::Appearance::readScaleFactor();
    prevScaleFactor = scaleFactor;

    // font should be private (padding logic is specific to this font combination)
    setFont(QFont("Times", 18, QFont::Weight::Bold));
}

void PlayerNameLabel::rescale()
{
    updateScaleFactor();
    setFont(font);
    resize(width(), height());
    move({x(), y()});
}

// functions related to size or position
void PlayerNameLabel::setFont(const QFont &pFont)
{
    font = pFont;
    font.setPointSizeF(font.pointSizeF() * scaleFactor);
}

void PlayerNameLabel::setGeometry(const QRect &rect)
{
    auto scaledRect = QRect(rect.x() * scaleFactor, rect.y() * scaleFactor,
                            rect.width() * scaleFactor, rect.height() * scaleFactor);

    QWidget::setGeometry(scaledRect);
}

void PlayerNameLabel::resize(int w, int h)
{
    QWidget::resize(w * scaleFactor, h * scaleFactor);
}

void PlayerNameLabel::move(const QPoint &pos)
{
    QWidget::move(pos * scaleFactor);
}

void PlayerNameLabel::setText(const QString &pText)
{
    text = pText;
}

void PlayerNameLabel::updateScaleFactor()
{
    float newScaleFactor = Settings::Appearance::readScaleFactor();
    scaleFactor = newScaleFactor / prevScaleFactor;
    prevScaleFactor = newScaleFactor;
}

QPoint PlayerNameLabel::getFontSubPos() const
{
    const float globalScaleFactor = Settings::Appearance::readScaleFactor();
    const int xPadAmt = 10 * globalScaleFactor;
    const int yPadAmt = 10 * globalScaleFactor;
    const int textWidth = width() * text.length() / MAX_NAME_CHAR;

    QPoint fontSubPos;

    switch (align)
    {
    case Qt::AlignRight:
        fontSubPos.setX(std::max(width() - textWidth - xPadAmt, xPadAmt));
        fontSubPos.setY((int) 25 * globalScaleFactor);
        break;
    case Qt::AlignLeft:
        fontSubPos.setX(std::min(width() - textWidth, xPadAmt));
        fontSubPos.setY((int) 25 * globalScaleFactor);
        break;
    case Qt::AlignTop:
        fontSubPos.setX(0);
        fontSubPos.setY((int) 25 * globalScaleFactor + yPadAmt);
        break;
    case Qt::AlignBottom:
        fontSubPos.setX(0);
        fontSubPos.setY((int) 25 * globalScaleFactor + yPadAmt);
        break;
    }

    return fontSubPos;
}

void PlayerNameLabel::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);
    painter->setRenderHint(QPainter::Antialiasing);
    QPainterPath *ppath = new QPainterPath;

    QPoint fontSubPos = getFontSubPos();
    ppath->addText(fontSubPos, this->font, this->text);
    painter->drawPath(*ppath);
}

MainWidgetData::MainWidgetData()
{
}

MainWidget::MainWidget(QWidget *parent) : ScaledQDialog(false, parent)
{
    player1Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_BOTTOM, SIZE_NORMAL, this);
    connect(player1Cards, &ClickableCardArray::clicked, this, &MainWidget::onCardClicked);
#ifdef CPU_DEBUG
    player2Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_LEFT, SIZE_SMALL, this);
    player3Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_TOP, SIZE_SMALL, this);
    player4Cards = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_RIGHT, SIZE_SMALL, this);
#endif

    player1CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_BOTTOM, SIZE_NORMAL, this);
    player2CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_LEFT, SIZE_NORMAL, this);
    player3CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_TOP, SIZE_NORMAL, this);
    player4CardPlayed = new ClickableCardArray(DRAW_POSITION_MAIN_WIDGET_CENTER_RIGHT, SIZE_NORMAL, this);

    auto playerNames = Settings::Appearance::readPlayerNames();

    auto setupLabel = [this](PlayerNameLabel *label, const QString &text, const QPoint &pos) {
        label->setParent(this);
        label->setText(text);
        label->move(pos);
        label->resize(100, 60);
    };

    player1NameLabel = new PlayerNameLabel(Qt::AlignTop);
    setupLabel(player1NameLabel, QString::fromStdString(playerNames[PLAYER_1]), {550, 770});

    player2NameLabel = new PlayerNameLabel(Qt::AlignLeft);
    setupLabel(player2NameLabel, QString::fromStdString(playerNames[PLAYER_2]), {0, 425});

    player3NameLabel = new PlayerNameLabel(Qt::AlignBottom);
    setupLabel(player3NameLabel, QString::fromStdString(playerNames[PLAYER_3]), {550, 130});

    player4NameLabel = new PlayerNameLabel(Qt::AlignRight);
    setupLabel(player4NameLabel, QString::fromStdString(playerNames[PLAYER_4]), {1100, 425});

    infoWidget = new GameInfoWidget;
    infoWidget->setParent(this);
    infoWidget->move(QPoint(0, 0));

    menuWidget = new GameMenuWidget;

    connect(menuWidget, &GameMenuWidget::newGameButtonPressed, this, &MainWidget::newGameButtonPressed);
    connect(menuWidget, &GameMenuWidget::newRoundButtonPressed, this, &MainWidget::newRoundButtonPressed);
    connect(menuWidget, &GameMenuWidget::saveGameButtonPressed, this, &MainWidget::saveGameButtonPressed);
    connect(menuWidget, &GameMenuWidget::loadGameButtonPressed, this, &MainWidget::loadGameButtonPressed);
    connect(menuWidget, &GameMenuWidget::quitGameButtonPressed, this, &MainWidget::quitGameButtonPressed);

    menuWidget->setParent(this);
    menuWidget->move(QPoint(400, 300));

    resize(MAIN_WIDGET_SIZE);
    // no window title, etc.
}

void MainWidget::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for (auto clickableCardArray : std::vector<ClickableCardArray *>{player1CardPlayed, player2CardPlayed,
                                                                player3CardPlayed, player4CardPlayed, player1Cards
#ifdef CPU_DEBUG
                                                                ,
                                                                player2Cards, player3Cards, player4Cards
#endif
         })
        clickableCardArray->rescale();

    for (auto label : std::vector<PlayerNameLabel *>{player1NameLabel, player2NameLabel,
                                                player3NameLabel, player4NameLabel})
        label->rescale();

    for (auto widget : std::vector<GameInfoWidget *>{infoWidget})
        widget->rescale();

    for (auto widget : std::vector<GameMenuWidget *>{menuWidget})
        widget->rescale();
}

void MainWidget::finishExistingHand(Card player1Card)
{
    playCard(player1Card, PLAYER_1);

    player1Cards->showCards(gamedata.playerArr[PLAYER_1].cardArr);

    showCardPlayed(player1Card, PLAYER_1);

    for (auto playerNum : std::vector<int>{PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (gamedata.handInfo.cardPlayed[playerNum].isUndefined()) // cpu hasn't played yet
        {
            auto &player = gamedata.playerArr[playerNum];

            playCard(player.cpu->getCardToPlay(), playerNum);

#ifdef CPU_DEBUG
            if (playerNum == PLAYER_2)
                player2Cards->showCards(gamedata.playerArr[PLAYER_2].cardArr);
            else if (playerNum == PLAYER_3)
                player3Cards->showCards(gamedata.playerArr[PLAYER_3].cardArr);
            else if (playerNum == PLAYER_4)
                player4Cards->showCards(gamedata.playerArr[PLAYER_4].cardArr);
#endif

            showCardPlayed(gamedata.handInfo.cardPlayed[playerNum], playerNum);
        }
    }

    gamedata.roundInfo.updateScores(gamedata.handInfo);

    showPartnerCardIfApplicable();

    // refresh playerScores, teamScores
    infoWidget->refreshWidget(gamedata);

    showHandResult();

    for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        getCardPlayedWidget(playerNum)->hideCards();
    }
}

void MainWidget::startNewHand(int startingPlayerNum)
{
    if (startingPlayerNum != PLAYER_1)
    {
        // sleep before starting hand
        repaint();
        QThread::msleep(500);
    }

    gamedata.handInfo.clear();

    int playerNum = startingPlayerNum;

    while (playerNum != PLAYER_1)
    {
        auto &player = gamedata.playerArr[playerNum];

        playCard(player.cpu->getCardToPlay(), playerNum);

        showCardPlayed(gamedata.handInfo.cardPlayed[playerNum], playerNum);

        playerNum = gamedata.playerArr[playerNum].getNextPlayerNum();
    }

    // wait for card click to finish hand
}

bool MainWidget::validateCard(ClickableCard *clickableCard)
{
    CardVector playableCards = gamedata.playerArr[PLAYER_1].cardArr.getPlayableCards(gamedata.handInfo);

    if (!playableCards.hasCard(clickableCard->data)) // invalid card
    {
        emit invalidCardPlayed(clickableCard);

        return false;
    }

    return true;
}

void MainWidget::onCardClicked(ClickableCard *clickableCard)
{
    if (!validateCard(clickableCard))
    {
        return;
    }

    finishExistingHand(clickableCard->data);

    if (isRoundOver())
    {
        gamedata.roundInfo.addPointsMiddleToScores(gamedata.handInfo);

        // refresh playerScores, teamScores
        infoWidget->refreshWidget(gamedata);

        showNestResult();

        gamedata.overallInfo.updatePlayerScores(gamedata.roundInfo);

        // refresh overallScores
        infoWidget->refreshWidget(gamedata);

        emit roundSummary();

        gamedata.scoreHistory[gamedata.overallInfo.roundNum] = gamedata.roundInfo.getRoundScores();
        gamedata.clearRoundSpecificInfo();

        infoWidget->refreshWidget(gamedata);

        // show game menu
        menuWidget->show();
    }
    else
    {
        startNewHand(gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo).playerNum);
    }
}

void MainWidget::refreshCardWidgets(GameData &pData)
{
    for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        if (data.cardPlayed[playerNum] != pData.handInfo.cardPlayed[playerNum])
        {
            data.cardPlayed[playerNum] = pData.handInfo.cardPlayed[playerNum];

            CardVector cardsToDisplay;

            if (!data.cardPlayed[playerNum].isUndefined())
            {
                cardsToDisplay.push_back({data.cardPlayed[playerNum]});
            }

            getCardPlayedWidget(playerNum)->showCards(cardsToDisplay);
        }
    }

    auto areCardVectorsEqual = [](CardVector &cardArr1, CardVector &cardArr2) {
        if (cardArr1.size() != cardArr2.size())
        {
            return false;
        }

        for (int i = 0; i < cardArr1.size(); i++)
        {
            if (cardArr1[i] != cardArr2[i])
            {
                return false;
            }
        }

        return true;
    };

    if (!areCardVectorsEqual(data.player1Cards, pData.playerArr[PLAYER_1].cardArr))
    {
        data.player1Cards = pData.playerArr[PLAYER_1].cardArr;
        player1Cards->showCards(data.player1Cards);
    }

#ifdef CPU_DEBUG
    if (!areCardVectorsEqual(data.player2Cards, pData.playerArr[PLAYER_2].cardArr))
    {
        data.player2Cards = pData.playerArr[PLAYER_2].cardArr;
        player2Cards->showCards(data.player2Cards);
    }

    if (!areCardVectorsEqual(data.player3Cards, pData.playerArr[PLAYER_3].cardArr))
    {
        data.player3Cards = pData.playerArr[PLAYER_3].cardArr;
        player3Cards->showCards(data.player3Cards);
    }

    if (!areCardVectorsEqual(data.player4Cards, pData.playerArr[PLAYER_4].cardArr))
    {
        data.player4Cards = pData.playerArr[PLAYER_4].cardArr;
        player4Cards->showCards(data.player4Cards);
    }
#endif
}

void MainWidget::refreshInfoWidget(GameData &pData)
{
    infoWidget->refreshWidget(pData);
}

void MainWidget::setMenuWidgetVisible(bool visible)
{
    menuWidget->setVisible(visible);
}

void MainWidget::refreshNameTags(bool showNameTags)
{
    auto playerNames = Settings::Appearance::readPlayerNames();

    for (auto playerNum : std::vector<int>{PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4})
    {
        auto label = getPlayerNameLabel(playerNum);
        label->setText(QString::fromStdString(playerNames[playerNum]));
        label->setVisible(showNameTags);
        label->repaint();
    }
}

void MainWidget::showCardPlayed(const Card &card /*NOT_USED*/, int playerNum /*NOT_USED*/)
{
    // refresh current players card played
    refreshCardWidgets(gamedata);

    bool sleep = []() {
        return true; // sleep after any card is played
    }();

    if (sleep)
    {
        repaint();
        QThread::msleep(500);
    }
}

void MainWidget::showPartnerCardIfApplicable()
{
    for (auto it = gamedata.handInfo.cardPlayed.begin(); it != gamedata.handInfo.cardPlayed.end(); it++)
    {
        auto currentCard = (*it).second;

        if (currentCard == gamedata.roundInfo.partnerCard)
        {
            // refresh partner card, teams
            infoWidget->refreshWidget(gamedata);

            emit partnerMessage();

            return;
        }
    }
}

void MainWidget::showHandResult()
{
    auto winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);

    emit handResultMessage(winningPair);
}

void MainWidget::showNestResult()
{
    auto winningPair = gamedata.handInfo.getWinningPlayerCardPair(gamedata.roundInfo);

    emit nestResultMessage(winningPair);
}

ClickableCardArray *MainWidget::getCardPlayedWidget(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return player1CardPlayed;
    case PLAYER_2:
        return player2CardPlayed;
    case PLAYER_3:
        return player3CardPlayed;
    case PLAYER_4:
        return player4CardPlayed;
    default:
        return nullptr;
    }
}

PlayerNameLabel *MainWidget::getPlayerNameLabel(int playerNum)
{
    switch (playerNum)
    {
    case PLAYER_1:
        return player1NameLabel;
    case PLAYER_2:
        return player2NameLabel;
    case PLAYER_3:
        return player3NameLabel;
    case PLAYER_4:
        return player4NameLabel;
    default:
        return nullptr;
    }
}

bool MainWidget::isRoundOver()
{
    return std::all_of(gamedata.playerArr.begin(), gamedata.playerArr.end(), [](auto &p) { return p.cardArr.empty(); });
}

void MainWidget::playCard(Card cardPlayed, int playerNum)
{
    if (gamedata.handInfo.startingPlayerNum == PLAYER_UNDEFINED) // first card played
    {
        gamedata.handInfo.startingPlayerNum = playerNum;
        gamedata.handInfo.suit = cardPlayed.suit;
    }

    gamedata.handInfo.cardPlayed[playerNum] = cardPlayed;
    gamedata.handInfo.points += cardPlayed.getPointValue();

    if (cardPlayed == gamedata.roundInfo.partnerCard) // update partner and teams
    {
        gamedata.roundInfo.partnerPlayerNum = playerNum;
        gamedata.roundInfo.updateTeams();
    }

    gamedata.playerArr[playerNum].cardArr.remove({cardPlayed});

    // detailed round info
    gamedata.detailRoundInfo.suitCardsPlayed[cardPlayed.suit].append({cardPlayed});
}