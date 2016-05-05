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
const unsigned WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
const int FRAME_SWITCH_INTERVAL = 300;
}

CAppWindow::CAppWindow()
    : sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Prima Roads - RGR Sample", WINDOW_STYLE)
    , m_graph(new CBoostGraph)
{
    setFramerateLimit(60);
    setVerticalSyncEnabled(true);

    m_font.loadFromFile("res/Ubuntu-R.ttf");
    m_graph->SetStepHandler(std::bind(&CAppWindow::OnGraphAlgorithmStep, this, std::placeholders::_1));
}

CAppWindow::~CAppWindow()
{
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
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                OnClick();
            }
        }

        clear(sf::Color::Black);
        switch (m_state)
        {
        case State::WaitingInput:
            OnWaitingInput();
            break;
        case State::RunningDemo:
            OnRunningDemo();
            break;
        case State::WaitingOutput:
            OnWaitingOutput();
            break;
        }
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

void CAppWindow::OnClick()
{
    if (m_state == State::WaitingInput)
    {
        const char *filters[] = { "*.txt" };
        char const *result = tinyfd_openFileDialog("Select input file", "", 1, filters, "", false);
        // Пользователь отменил выбор файла.
        if (result == nullptr)
        {
            return;
        }
        std::ifstream in(result);
        if (!in.is_open() || !m_graph->ReadText(in))
        {
            tinyfd_messageBox("Error", "I/O error when reading input file", "ok", "error", 1);
        }
        else
        {
            RunAlgorithmDemo();
        }
    }
    else if (m_state == State::WaitingOutput)
    {
        const char *filters[] = { "*.txt" };
        char const *result = tinyfd_saveFileDialog("Select input file", "", 1, filters, "");
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
            close();
        }
    }
}

void CAppWindow::OnWaitingInput()
{
    sf::Text label("Click to select input file.", m_font);
    draw(label);
}

void CAppWindow::OnRunningDemo()
{
    if (m_clock.getElapsedTime().asMilliseconds() >= FRAME_SWITCH_INTERVAL)
    {
        if (!SwitchNextFrame())
        {
            m_state = State::WaitingOutput;
            return;
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

void CAppWindow::OnWaitingOutput()
{
    sf::Text label("Click to select file where to save output.", m_font);
    draw(label);
}

void CAppWindow::RunAlgorithmDemo()
{
    m_pendingFramePaths.clear();
    m_graph->RunPrima();
    m_state = State::RunningDemo;
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
