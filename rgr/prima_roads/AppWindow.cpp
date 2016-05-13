#include "stdafx.h"
#include "AppWindow.h"
#include "Utils.h"
#include "BoostGraph.h"
#include "tinyfiledialogs.h"
#include <fstream>

namespace
{
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;
const unsigned MENU_BAR_HEIGHT = 20;
const unsigned WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
const int FRAME_SWITCH_INTERVAL = 1000;
}

CAppWindow::CAppWindow()
    : sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Prima Roads - RGR Sample", WINDOW_STYLE)
{
    m_menu = std::make_unique<CAppMenu>("File");
    m_menu->SetFrame(sf::FloatRect(0, 0, float(WINDOW_WIDTH), float(MENU_BAR_HEIGHT)));
    m_openActionId = m_menu->AddAction("Open...", std::bind(&CAppWindow::AskOpenInput, this));
    m_saveActionId = m_menu->AddAction("Save...", std::bind(&CAppWindow::AskSaveOutput, this));

    setFramerateLimit(60);
    setVerticalSyncEnabled(true);
    SetState(State::WaitingInput);

    m_font.loadFromFile("res/Ubuntu-R.ttf");
}

CAppWindow::~CAppWindow()
{
    m_menu.reset();
}

void CAppWindow::EnterLoop()
{
    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                close();
                return;
            }
            m_menu->OnEvent(event);
        }

        clear(sf::Color::Black);
        if (m_state != State::WaitingInput)
        {
            OnRunningDemo();
        }
        draw(*m_menu);
        display();
    }
}

void CAppWindow::OnGraphAlgorithmStep(const std::string &dotCode)
{
    std::string tempPath = "graph.dot";
    std::string nextPath = "frame_" + std::to_string(m_pendingFramePaths.size()) + ".png";
    std::ofstream tempDot(tempPath);
    tempDot << dotCode;
    tempDot.close();

    if (CUtils::RunProcess("dot -Tpng -o" + nextPath + " " + tempPath))
    {
        m_pendingFramePaths.push_back(nextPath);
    }
}

void CAppWindow::SetState(CAppWindow::State state)
{
    m_state = state;
    m_menu->SetActionEnabled(m_openActionId, state != State::RunningDemo);
    m_menu->SetActionEnabled(m_saveActionId, state == State::WaitingOutput);
}

void CAppWindow::OnRunningDemo()
{
    if (m_state == State::RunningDemo && m_clock.getElapsedTime().asMilliseconds() >= FRAME_SWITCH_INTERVAL)
    {
        if (!SwitchNextFrame())
        {
            SetState(State::WaitingOutput);
        }
    }
    sf::Sprite sprite;
    sprite.setTexture(m_activeFrame);

    const sf::Vector2f spriteSize = { float(sprite.getTextureRect().width), float(sprite.getTextureRect().height)};
    const sf::Vector2f windowSize = { float(getSize().x), float(getSize().y) };
    const float scale = std::min(windowSize.x / spriteSize.x, windowSize.y / spriteSize.y);
    sprite.setOrigin(spriteSize * 0.5f);
    sprite.setPosition(windowSize * 0.5f);
    sprite.setScale(scale, scale);

    draw(sprite);
}

void CAppWindow::RunAlgorithmDemo()
{
    m_pendingFramePaths.clear();
    m_graph->RunPrima();
    SetState(State::RunningDemo);
    SwitchNextFrame();
}

bool CAppWindow::SwitchNextFrame()
{
    m_clock.restart();
    if (m_pendingFramePaths.empty())
    {
        return false;
    }

    m_activeFrame.loadFromFile(m_pendingFramePaths.front());
    m_pendingFramePaths.pop_front();

    return true;
}

void CAppWindow::AskOpenInput()
{
    const char *filters[] = { "*.txt" };
    char const *result = tinyfd_openFileDialog("Select input file", "", 1, filters, "", false);
    // Пользователь отменил выбор файла.
    if (result == nullptr)
    {
        return;
    }
    m_graph = std::make_unique<CBoostGraph>();
    m_graph->SetStepHandler(std::bind(&CAppWindow::OnGraphAlgorithmStep, this, std::placeholders::_1));

    std::ifstream in(result);
    if (!in.is_open() || !m_graph->ReadText(in))
    {
        m_graph.reset();
        tinyfd_messageBox("Error", "I/O error when reading input file", "ok", "error", 1);
    }
    else
    {
        RunAlgorithmDemo();
    }
}

void CAppWindow::AskSaveOutput()
{
    const char *filters[] = { "*.txt" };
    char const *result = tinyfd_saveFileDialog("Select output file", "", 1, filters, "");
    // Пользователь отменил выбор файла.
    if (result == nullptr)
    {
        return;
    }
    std::ofstream out(result);
    if (!out.is_open() || !m_graph->PrintResults(out))
    {
        tinyfd_messageBox("Error", "I/O error when writing output file", "ok", "error", 1);
    }
    else
    {
        tinyfd_messageBox("Success", "File saved OK", "ok", "info", 1);
    }
}
