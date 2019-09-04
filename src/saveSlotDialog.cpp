#include "saveSlotDialog.h"

SaveSlotDialog::SaveSlotDialog(int &pSlotSelected, QWidget *parent) : slotSelected(pSlotSelected),
                                                                      QDialog(parent)
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);

    auto setupListWidgetItem = [this](QListWidgetItem *listWidgetItem, QString text, QIcon icon) {
        listWidgetItem = new QListWidgetItem;
        listWidgetItem->setIcon(icon);
        listWidgetItem->setText(text);
        listWidgetItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        contentsWidget->addItem(listWidgetItem);
    };

    setupListWidgetItem(saveSlot1Button, "File 1", QIcon(":saveSlot.png"));
    setupListWidgetItem(saveSlot2Button, "File 2", QIcon(":saveSlot.png"));
    setupListWidgetItem(saveSlot3Button, "File 3", QIcon(":saveSlot.png"));

    QObject::connect(contentsWidget, &QListWidget::currentItemChanged, this, &SaveSlotDialog::changePage);

    saveSlot1Page = new SaveSlotPage("slot1.db");
    saveSlot2Page = new SaveSlotPage("slot2.db");
    saveSlot3Page = new SaveSlotPage("slot3.db");

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(saveSlot1Page);
    pagesWidget->addWidget(saveSlot2Page);
    pagesWidget->addWidget(saveSlot3Page);

    contentsWidget->setCurrentRow(0);

    selectButton = new QPushButton;
    selectButton->setText("Select");

    QObject::connect(selectButton, &QAbstractButton::clicked, this, &SaveSlotDialog::onSelectButton);

    horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(selectButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle("File Select Dialog");
    setWindowIcon(QIcon(":rookicon.gif"));
}

void SaveSlotDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }

    slotSelected = contentsWidget->row(current);
    pagesWidget->setCurrentIndex(slotSelected);
}

void SaveSlotDialog::onSelectButton()
{
    QDialog::accept();
}
