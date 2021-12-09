#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class DropDownList {
private:
	float keytime;
	float keytimeMax;

	sf::Font& font;
	Button* activeElement;
	std::vector<Button*> list;
	bool showList;

public:
	DropDownList(float x, float y, float width, float height,
		sf::Font& font, vector<string> list,
		unsigned nrOfElements, unsigned default_index = 0);
	~DropDownList();

	//Accessors
	const unsigned short& getActiveElementId() const;

	//Functions
	const bool getKeytime();
	void updateKeytime(const float& dt);
	void update(const sf::Vector2i& mousePosWindow, const float& dt);
	void render(sf::RenderTarget& target);
};

DropDownList::DropDownList(float x, float y, float width, float height,
	sf::Font& font, vector<string> list,
	unsigned nrOfElements, unsigned default_index)
	: font(font), showList(false), keytimeMax(1.f), keytime(0.f) {
	//unsigned nrOfElements = sizeof(list) / sizeof(std::string);

	this->activeElement = new Button(
		x, y, width, height,
		&this->font, list[default_index], 35,
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
		sf::Color(40, 87, 151, 255), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50)
	);

	for (unsigned i = 0; i < nrOfElements; i++) {
		this->list.push_back(new Button(x, y + ((i + 1) * height), width, height,
			&this->font, list[i], 35,
			sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
			i
		)
		);
	}
}

DropDownList::~DropDownList() {
	delete this->activeElement;
	for (size_t i = 0; i < this->list.size(); i++) {
		delete this->list[i];
	}
}

//Accessors
const bool DropDownList::getKeytime() {
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}

	return false;
}

const unsigned short& DropDownList::getActiveElementId() const {
	return this->activeElement->getId();
}

//Functions
void DropDownList::updateKeytime(const float& dt) {
	if (this->keytime < this->keytimeMax)
		this->keytime += 10.f * dt;
}

void DropDownList::update(const sf::Vector2i& mousePosWindow, const float& dt) {
	this->updateKeytime(dt);

	this->activeElement->update(mousePosWindow);

	//Show and hide the list
	if (this->activeElement->isPressed() && this->getKeytime()) {
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList) {
		for (auto& i : this->list) {
			i->update(mousePosWindow);

			if (i->isPressed() && this->getKeytime()) {
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setId(i->getId());
			}
		}
	}
}

void DropDownList::render(sf::RenderTarget& target) {
	this->activeElement->render(target);

	if (this->showList) {
		for (auto& i : this->list) {
			i->render(target);
		}
	}
}