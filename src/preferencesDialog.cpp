#include "preferencesDialog.h"

void PreferencesDialog::setupContentsWidget()
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

    setupListWidgetItem(appearanceButton, "Appearance", QIcon(":appearanceButton.png"));
    setupListWidgetItem(gameButton, "Game", QIcon(":gameButton.png"));

    connect(contentsWidget, &QListWidget::currentItemChanged, this, &PreferencesDialog::changePage);
}

void PreferencesDialog::setupPagesWidget()
{
    appearancePage = new AppearancePage;
    connect(appearancePage, &AppearancePage::nameTagsChanged, this, &PreferencesDialog::nameTagsChanged);
    connect(appearancePage, &AppearancePage::gameResolutionChanged, this, &PreferencesDialog::gameResolutionChanged);

    gamePage = new GamePage;

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(appearancePage);
    pagesWidget->addWidget(gamePage);
}

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{
    setupContentsWidget();
    setupPagesWidget();

    closeButton = new QPushButton;
    closeButton->setText("Close");

    connect(closeButton, &QAbstractButton::clicked, this, &PreferencesDialog::onCloseButton);

    horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    // must be called after setting up horizontalLayout
    contentsWidget->setCurrentRow(0);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle("Preferences Dialog");
    setWindowIcon(QIcon(":rookicon.gif"));
}

void PreferencesDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
    {
        current = previous;
    }

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void PreferencesDialog::onCloseButton()
{
    QDialog::accept();
}
