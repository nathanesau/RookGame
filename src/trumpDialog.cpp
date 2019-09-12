#include "card.h"
#include "trumpDialog.h"

TrumpDialog::TrumpDialog(int &pSuitSelected, QWidget *parent) : suitSelected(pSuitSelected), ScaledQDialog(true, parent)
{
    QFont labelFont;
    labelFont.setPointSize(12);    

    redLabel = new ClickableLabel;
    redLabel->setStyleSheet("background-color: red");
    redLabel->setText("Red");
    redLabel->setAlignment(Qt::AlignCenter);
    redLabel->setFont(labelFont);

    blackLabel = new ClickableLabel;
    blackLabel->setStyleSheet("background-color: black; color: white");
    blackLabel->setText("Black");
    blackLabel->setAlignment(Qt::AlignCenter);
    blackLabel->setFont(labelFont);

    greenLabel = new ClickableLabel;
    greenLabel->setStyleSheet("background-color: green");
    greenLabel->setText("Green");
    greenLabel->setAlignment(Qt::AlignCenter);
    greenLabel->setFont(labelFont);

    yellowLabel = new ClickableLabel;
    yellowLabel->setStyleSheet("background-color: yellow");
    yellowLabel->setText("Yellow");
    yellowLabel->setAlignment(Qt::AlignCenter);
    yellowLabel->setFont(labelFont);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(redLabel);
    mainLayout->addWidget(blackLabel);
    mainLayout->addWidget(greenLabel);
    mainLayout->addWidget(yellowLabel);

    resize(TRUMP_DIALOG_SIZE);
    setLayout(mainLayout);
    setWindowTitle("Choose Trump Suit...");
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void TrumpDialog::rescale()
{
    // do nothing
}

void TrumpDialog::reject()
{
    // do nothing
}

void TrumpDialog::onTrumpLabelClicked(ClickableLabel *label)
{
    using SuitMap = std::map<std::string, int>;

    SuitMap suitMap = { {"Black", SUIT_BLACK},
                        {"Green", SUIT_GREEN},
                        {"Red", SUIT_RED},
                        {"Yellow", SUIT_YELLOW}};

    suitSelected = suitMap[label->text().toStdString()];

    QDialog::accept();
}
