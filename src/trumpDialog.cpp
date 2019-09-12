#include "card.h"
#include "trumpDialog.h"

TrumpDialogLabel::TrumpDialogLabel(TrumpDialog *parent) : ScaledQLabel(parent)
{
}

void TrumpDialogLabel::mousePressEvent(QMouseEvent *event)
{
    TrumpDialog *parentDialog = dynamic_cast<TrumpDialog *>(parent ());

    parentDialog->onTrumpLabelClicked(this);
}

TrumpDialog::TrumpDialog(int &pSuitSelected, QWidget *parent) : suitSelected(pSuitSelected), ScaledQDialog(true, parent)
{
    auto setupTrumpLabel = [this](TrumpDialogLabel *label, QString text, QString style, QPoint pos) {
        label->setFont(QFont("Times", 12));
        label->setText(text);
        label->setStyleSheet(style);
        label->setParent(this);
        label->move(pos);
        label->setAlignment(Qt::AlignCenter);
    };

    redLabel = new TrumpDialogLabel;
    setupTrumpLabel(redLabel, "Red", "background-color: red", QPoint(25, 25));

    blackLabel = new TrumpDialogLabel;
    setupTrumpLabel(blackLabel, "Black", "background-color: black; color: white", QPoint(25, 75));

    greenLabel = new TrumpDialogLabel;
    setupTrumpLabel(greenLabel, "Green", "background-color: green", QPoint(25, 125));

    yellowLabel = new TrumpDialogLabel;
    setupTrumpLabel(yellowLabel, "Yellow", "background-color: yellow", QPoint(25, 175));

    resize(TRUMP_DIALOG_SIZE);
    setWindowTitle("Choose Trump Suit...");
    setWindowIcon(QIcon(":rookicon.gif"));
    setStyleSheet("background-color: white");
}

void TrumpDialog::rescale()
{
    updateScaleFactor();
    setGeometry(geometry());

    for(auto label : std::vector<ScaledQLabel *>{blackLabel, greenLabel, redLabel, yellowLabel})
        label->rescale();
}

void TrumpDialog::reject()
{
    // do nothing
}

void TrumpDialog::onTrumpLabelClicked(TrumpDialogLabel *label)
{
    std::string text = label->text().toStdString();

    if (text == "Black")
    {
        suitSelected = SUIT_BLACK;
    }
    else if (text == "Green")
    {
        suitSelected = SUIT_GREEN;
    }
    else if (text == "Red")
    {
        suitSelected = SUIT_RED;
    }
    else if (text == "Yellow")
    {
        suitSelected = SUIT_YELLOW;
    }

    QDialog::accept();
}
