#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;

const int WIDTH  = 600;
const int HEIGHT = 800;

const int BAR_COUNT = 32;

vector<int> values(BAR_COUNT);

int compareA = -1;
int compareB = -1;
int sortedFrom = BAR_COUNT;

bool sorting = false;
bool finished = false;

// ----------------------------------------------------
// Generate random bar heights
// ----------------------------------------------------
void generateValues()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(80, 560);

    for (int &value : values)
        value = dist(gen);

    compareA = -1;
    compareB = -1;
    sortedFrom = BAR_COUNT;
    finished = false;
}

// ----------------------------------------------------
// Draw the bars
// ----------------------------------------------------
void drawBars(sf::RenderWindow& window)
{
    float chartLeft   = 35.0f;
    float chartRight  = 565.0f;
    float chartTop    = 150.0f;
    float chartBottom = 700.0f;

    float chartWidth = chartRight - chartLeft;
    float barWidth = chartWidth / BAR_COUNT;

    for (int i = 0; i < BAR_COUNT; i++)
    {
        float height = static_cast<float>(values[i]);

        sf::RectangleShape bar;

        bar.setSize(
            sf::Vector2f(
                barWidth - 3.0f,
                height
            )
        );

        bar.setPosition(
            chartLeft + i * barWidth,
            chartBottom - height
        );

        // Default rainbow-like color
        int red   = 60 + (i * 17) % 195;
        int green = 80 + (i * 31) % 175;
        int blue  = 120 + (i * 43) % 135;

        bar.setFillColor(
            sf::Color(red, green, blue)
        );

        // Bars currently being compared
        if (i == compareA)
            bar.setFillColor(sf::Color(255, 80, 90));

        if (i == compareB)
            bar.setFillColor(sf::Color(255, 220, 50));

        // Sorted portion
        if (i >= sortedFrom)
            bar.setFillColor(sf::Color(40, 240, 150));

        // Glow-like outline
        bar.setOutlineThickness(1.0f);
        bar.setOutlineColor(sf::Color(255,255,255,120));

        window.draw(bar);
    }
}

// ----------------------------------------------------
// Bubble sort animation
// ----------------------------------------------------
void bubbleSort(sf::RenderWindow& window)
{
    sorting = true;
    finished = false;

    for (int pass = 0; pass < BAR_COUNT - 1; pass++)
    {
        bool swapped = false;

        for (int i = 0; i < BAR_COUNT - pass - 1; i++)
        {
            compareA = i;
            compareB = i + 1;

            // Draw animation frame
            window.clear(sf::Color(10, 10, 30));

            drawBars(window);

            window.display();

            this_thread::sleep_for(
                chrono::milliseconds(45)
            );

            if (values[i] > values[i + 1])
            {
                swap(values[i], values[i + 1]);
                swapped = true;

                // Show the swap
                window.clear(sf::Color(10, 10, 30));

                drawBars(window);

                window.display();

                this_thread::sleep_for(
                    chrono::milliseconds(55)
                );
            }

            sf::Event event;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
            }
        }

        sortedFrom = BAR_COUNT - pass - 1;

        if (!swapped)
            break;
    }

    sortedFrom = 0;
    compareA = -1;
    compareB = -1;

    sorting = false;
    finished = true;
}

// ----------------------------------------------------
// Main
// ----------------------------------------------------
int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WIDTH, HEIGHT),
        "Bubble Sort Visualizer"
    );

    window.setFramerateLimit(60);

    generateValues();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // SPACE = start sorting
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space &&
                !sorting)
            {
                bubbleSort(window);
            }

            // R = randomize
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::R &&
                !sorting)
            {
                generateValues();
            }
        }

        window.clear(sf::Color(10, 10, 30));

        drawBars(window);

        window.display();
    }

    return 0;
}