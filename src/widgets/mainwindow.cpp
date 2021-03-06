#include "widgets/mainwindow.hpp"
#include "channelmanager.hpp"
#include "colorscheme.hpp"
#include "settingsmanager.hpp"
#include "widgets/chatwidget.hpp"
#include "widgets/notebook.hpp"
#include "widgets/settingsdialog.hpp"

#include <QDebug>
#include <QPalette>
#include <QShortcut>
#include <QVBoxLayout>
#include <boost/foreach.hpp>

#ifdef USEWINSDK
#include "Windows.hpp"
#endif

namespace chatterino {
namespace widgets {

MainWindow::MainWindow(ChannelManager &_channelManager, ColorScheme &_colorScheme)
    : BaseWidget(_colorScheme, nullptr)
    , channelManager(_channelManager)
    , colorScheme(_colorScheme)
    , notebook(this->channelManager, this)
    , windowGeometry("/windows/0/geometry")
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // add titlebar
    //    if (SettingsManager::getInstance().useCustomWindowFrame.get()) {
    //        layout->addWidget(&_titleBar);
    //    }

    layout->addWidget(&this->notebook);
    setLayout(layout);

    // set margin
    //    if (SettingsManager::getInstance().useCustomWindowFrame.get()) {
    //        layout->setMargin(1);
    //    } else {
    layout->setMargin(0);
    //    }

    this->refreshTheme();

    if (this->windowGeometry->isFilled()) {
        // Load geometry from settings file
        this->setGeometry(this->windowGeometry.getValueRef());
    } else {
        // Set default geometry
        // Default position is in the middle of the current monitor or the primary monitor
        this->resize(1280, 800);
    }

    // Initialize program-wide hotkeys
    {
        // CTRL+P: Open Settings Dialog
        auto shortcut = new QShortcut(QKeySequence("CTRL+P"), this);
        connect(shortcut, &QShortcut::activated, []() {
            SettingsDialog::showDialog();  //
        });
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::layoutVisibleChatWidgets(Channel *channel)
{
    auto *page = this->notebook.getSelectedPage();

    if (page == nullptr) {
        return;
    }

    const std::vector<ChatWidget *> &widgets = page->getChatWidgets();

    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        ChatWidget *widget = *it;

        if (channel == nullptr || channel == widget->getChannel().get()) {
            widget->layoutMessages();
        }
    }
}

void MainWindow::repaintVisibleChatWidgets(Channel *channel)
{
    auto *page = this->notebook.getSelectedPage();

    if (page == nullptr) {
        return;
    }

    const std::vector<ChatWidget *> &widgets = page->getChatWidgets();

    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        ChatWidget *widget = *it;

        if (channel == nullptr || channel == widget->getChannel().get()) {
            widget->layoutMessages();
        }
    }
}

void MainWindow::repaintGifEmotes()
{
    auto *page = this->notebook.getSelectedPage();

    if (page == nullptr) {
        return;
    }

    const std::vector<ChatWidget *> &widgets = page->getChatWidgets();

    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        ChatWidget *widget = *it;

        widget->updateGifEmotes();
    }
}

void MainWindow::load(const boost::property_tree::ptree &tree)
{
    this->notebook.load(tree);

    loaded = true;
}

boost::property_tree::ptree MainWindow::save()
{
    boost::property_tree::ptree child;

    child.put("type", "main");

    this->notebook.save(child);

    return child;
}

void MainWindow::loadDefaults()
{
    this->notebook.loadDefaults();

    loaded = true;
}

bool MainWindow::isLoaded() const
{
    return loaded;
}

Notebook &MainWindow::getNotebook()
{
    return this->notebook;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Save closing window position
    this->windowGeometry = this->geometry();
}

void MainWindow::refreshTheme()
{
    QPalette palette;
    palette.setColor(QPalette::Background, this->colorScheme.TabPanelBackground);
    this->setPalette(palette);
}

}  // namespace widgets
}  // namespace chatterino
