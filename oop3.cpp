#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

class GraphicalElement {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void move(int x, int y) = 0;
    virtual void setColor(sf::Color color) = 0;
    virtual void setSize(int size) = 0;
    virtual void setSizeWithDimensions(int length, int width) = 0; // Уникальное имя
    virtual void setVisible(bool visible) = 0;
    virtual sf::Color getColor() = 0;
    virtual int getSize() = 0;
};

class LineElement : public GraphicalElement {
public:
    LineElement(int x, int y, float length, float width) : line(sf::Vector2f(length, width)) {
        line.setPosition(x, y);
        line.setFillColor(sf::Color::Transparent);
        line.setOutlineColor(sf::Color::White);
        line.setOutlineThickness(1);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(line);
    }

    void move(int x, int y) override {
        line.move(x, y);
    }

    void setColor(sf::Color color) override {
        // Не викристовується для лінії
    }

    void setSize(int size) override {
        line.setSize(sf::Vector2f(size, 1));
    }

    void setSizeWithDimensions(int length, int width) override {
        line.setSize(sf::Vector2f(length, width));
    }

    void setVisible(bool visible) override {
        line.setOutlineColor(visible ? sf::Color::White : sf::Color::Transparent);
    }

    sf::Color getColor() override {
        return line.getOutlineColor();
    }

    int getSize() override {
        return static_cast<int>(line.getSize().x);
    }

private:
    sf::RectangleShape line;
};

class CircularElement : public GraphicalElement {
public:
    CircularElement(int x, int y, int radius) : circle(radius) {
        circle.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(circle);
    }

    void move(int x, int y) override {
        circle.move(x, y);
    }

    void setColor(sf::Color color) override {
        circle.setFillColor(color);
    }

    void setSize(int size) override {
        circle.setRadius(size);
    }

    void setSizeWithDimensions(int length, int width) override {
        // Для круга можно рассмотреть вариант, когда width игнорируется
        circle.setRadius(length);
    }

    void setVisible(bool visible) override {
        circle.setFillColor(visible ? sf::Color::White : sf::Color::Transparent);
    }

    sf::Color getColor() override {
        return circle.getFillColor();
    }

    int getSize() override {
        return circle.getRadius();
    }

private:
    sf::CircleShape circle;
};

class DiamondElement : public GraphicalElement {
public:
    DiamondElement(int x, int y, int size) {
        diamond.setPointCount(4);
        diamond.setRadius(size);
        diamond.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(diamond);
    }

    void move(int x, int y) override {
        diamond.move(x, y);
    }

    void setColor(sf::Color color) override {
        diamond.setFillColor(color);
    }

    void setSize(int size) override {
        diamond.setRadius(size);
    }

    void setSizeWithDimensions(int length, int width) override {
        diamond.setRadius(sqrt(length * length + width * width) / 2);
    }

    void setVisible(bool visible) override {
        diamond.setFillColor(visible ? sf::Color::White : sf::Color::Transparent);
    }

    sf::Color getColor() override {
        return diamond.getFillColor();
    }

    int getSize() override {
        return static_cast<int>(2 * diamond.getRadius());
    }

private:
    sf::CircleShape diamond;
};

class CombinedElement : public GraphicalElement {
public:
    CombinedElement(std::unique_ptr<GraphicalElement> elem1, std::unique_ptr<GraphicalElement> elem2)
        : elem1(std::move(elem1)), elem2(std::move(elem2)) {
        size = (this->elem1->getSize() + this->elem2->getSize()) / 2;
    }

    void draw(sf::RenderWindow& window) override {
        elem1->draw(window);
        elem2->draw(window);
    }

    void move(int x, int y) override {
        elem1->move(x, y);
        elem2->move(x, y);
    }

    void setColor(sf::Color color) override {
        elem1->setColor(color);
        elem2->setColor(color);
    }

    void setSize(int size) override {
        elem1->setSize(size);
        elem2->setSize(size);
        this->size = size;
    }

    void setSizeWithDimensions(int length, int width) override {
        int newSize = (elem1->getSize() + elem2->getSize()) / 2;
        elem1->setSize(newSize);
        elem2->setSize(newSize);
        this->size = newSize;
    }

    void setVisible(bool visible) override {
        elem1->setVisible(visible);
        elem2->setVisible(visible);
    }

    sf::Color getColor() override {
        return elem1->getColor();
    }

    int getSize() override {
        return size;
    }

private:
    std::shared_ptr<GraphicalElement> elem1;
    std::shared_ptr<GraphicalElement> elem2;
    int size;
};

int main() {
    AllocConsole();
    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    std::srand(std::time(0));
    sf::RenderWindow window(sf::VideoMode(800, 800), "Daria");

    std::vector<std::unique_ptr<GraphicalElement>> elements;
    elements.push_back(std::make_unique<CircularElement>(100, 100, 50));

    int activeElement = 0;
    std::cout << "Press K to print help" << std::endl;;

    while (window.isOpen()) {
        sf::Event event;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::K) {
                        std::cout << "K: Show help \nArrow keys: Move\nR: Change color\n Z: Increase size\n X: Decrease size\n Num1-Num9: Switch objects\n T: Create Circle\n D: Create Square\n L: Create Line\n Q: Combine objects\n W: Move object \n E: Delete Object" << std::endl;
                    }
                    else if (event.key.code == sf::Keyboard::Up) {
                        elements[activeElement]->move(0, -5);
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        elements[activeElement]->move(-5, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        elements[activeElement]->move(0, 5);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        elements[activeElement]->move(5, 0);
                    }
                    else if (event.key.code == sf::Keyboard::R) {
                        sf::Color currentColor = elements[activeElement]->getColor();
                        sf::Color newColor;

                        if (currentColor == sf::Color::Red) {
                            newColor = sf::Color::White;
                        }
                        else if (currentColor == sf::Color::White) {
                            newColor = sf::Color::Blue;
                        }
                        else if (currentColor == sf::Color::Blue) {
                            newColor = sf::Color::Green;
                        }
                        else if (currentColor == sf::Color::Green) {
                            newColor = sf::Color::Red;
                        }
                        else {
                            newColor = sf::Color::White;
                        }

                        elements[activeElement]->setColor(newColor);
                    }
                    else if (event.key.code == sf::Keyboard::Z) {
                        elements[activeElement]->setSize(elements[activeElement]->getSize() + 1);
                    }
                    else if (event.key.code == sf::Keyboard::X) {
                        int newSize = std::max(1, elements[activeElement]->getSize() - 1);
                        elements[activeElement]->setSize(newSize);
                    }
                    else if (event.key.code == sf::Keyboard::T) {
                        elements.push_back(std::make_unique<CircularElement>(100, 100, 50));
                        activeElement = elements.size() - 1;
                    }
                    else if (event.key.code == sf::Keyboard::L) {
                        elements.push_back(std::make_unique<LineElement>(100, 100,100, 10));
                        activeElement = elements.size() - 1;
                    }
                    else if (event.key.code == sf::Keyboard::D) {
                        elements.push_back(std::make_unique<DiamondElement>(200, 200, 50));
                        activeElement = elements.size() - 1;
                    }
                }
                else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                    int index = event.key.code - sf::Keyboard::Num1;
                    if (index < elements.size()) {
                        activeElement = index;
                    }
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    if (elements.size() >= 2) {
                        std::unique_ptr<GraphicalElement> combinedElement = std::make_unique<CombinedElement>(std::move(elements[elements.size() - 2]), std::move(elements[elements.size() - 1]));
                        elements.erase(elements.end() - 2, elements.end());
                        elements.push_back(std::move(combinedElement));
                        activeElement = elements.size() - 1;
                    }
                    else {
                        std::cout << "Not enough elements to combine.\n";
                    }
                }
                else if (event.key.code == sf::Keyboard::E) {
                    if (!elements.empty()) {
                        elements.erase(elements.begin() + activeElement);
                        if (activeElement >= elements.size()) {
                            activeElement = elements.size() - 1;
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::W) {
                    for (int i = 0; i < 4; ++i) {
                        int moveAmount = 70;
                        switch (i) {
                        case 0: // Move to the right
                            elements[activeElement]->move(moveAmount, 0);
                            break;
                        case 1: // Move down
                            elements[activeElement]->move(0, moveAmount);
                            break;
                        case 2: // Move to the left
                            elements[activeElement]->move(-moveAmount, 0);
                            break;
                        case 3: // Move up
                            elements[activeElement]->move(0, -moveAmount);
                            break;
                        }

                        sf::sleep(sf::milliseconds(200));
                        window.clear();
                        for (auto& element : elements) {
                            element->draw(window);
                        }
                        window.display();
                    }
                }
            }

            window.clear();
            for (auto& element : elements) {
                element->draw(window);
            }
            window.display();
        }
    }

    return 0;
}
