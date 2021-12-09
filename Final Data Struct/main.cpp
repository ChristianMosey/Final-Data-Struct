#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <random>
#include "Graph.h"
#include "Button.h"
#include <set>
#include <unordered_set>
#include "DropDownList.h"

using namespace std;
using namespace sf;

int main() {

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Schedule Builder");

	sf::Texture navBar;
	if (!navBar.loadFromFile("images/navBar.png")) return 0;
	sf::Sprite spNavBar;
	spNavBar.setTexture(navBar);

	sf::Texture box;
	if (!box.loadFromFile("images/box.png")) return 0;
	sf::Sprite spBox;
	spBox.setPosition(70, 210);

	sf::Texture leftButtonDisabled;
	leftButtonDisabled.loadFromFile("images/LeftButton_Disabled.png");
	sf::Sprite spLeftButtonDisabled;
	spLeftButtonDisabled.setTexture(leftButtonDisabled);

	sf::Texture rightButtonDisabled;
	rightButtonDisabled.loadFromFile("images/RightButton_Disabled.png");
	sf::Sprite spRightButtonDisabled;
	spRightButtonDisabled.setTexture(rightButtonDisabled);

	sf::Texture leftButtonEnabled;
	leftButtonEnabled.loadFromFile("images/LeftButton_Enabled.png");
	sf::Sprite spLeftButtonEnabled;
	spLeftButtonEnabled.setTexture(leftButtonEnabled);

	sf::Texture rightButtonEnabled;
	rightButtonEnabled.loadFromFile("images/RightButton_Enabled.png");
	sf::Sprite spRightButtonEnabled;
	spRightButtonEnabled.setTexture(rightButtonEnabled);

	spLeftButtonDisabled.setPosition(70, 959);
	spLeftButtonEnabled.setPosition(70, 959);
	spRightButtonDisabled.setPosition(927, 959);
	spRightButtonEnabled.setPosition(927, 959);

	sf::Font font;
	if (!font.loadFromFile("fonts/bahnschrift.ttf")) return 0;

	sf::Text output;
	output.setFont(font);
	output.setCharacterSize(31);
	output.setFillColor(Color(0, 0, 0, 255));
	output.setPosition(100, 230);
	output.setStyle(output.Bold);

	sf::Text totNumSemesters;
	totNumSemesters.setFont(font);
	totNumSemesters.setCharacterSize(31);
	totNumSemesters.setFillColor(Color(0, 0, 0, 255));
	totNumSemesters.setPosition(1250, 130);
	totNumSemesters.setString("Total semesters: 0");

	sf::Text creditsNeeded;
	creditsNeeded.setFont(font);
	creditsNeeded.setCharacterSize(31);
	creditsNeeded.setFillColor(Color(0, 0, 0, 255));
	creditsNeeded.setPosition(770, 130);
	creditsNeeded.setString("(0 Needed)");

	sf::Text time;
	time.setFont(font);
	time.setCharacterSize(31);
	time.setFillColor(Color(0, 0, 0, 255));
	time.setPosition(1110, 280);

	Button btnLoad(
		1560.0f, 120.0f, 270.0f, 55.0f,
		&font, "COMPUTE", 31,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
		sf::Color(40, 87, 151, 255), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	DropDownList drpDwnList(77.0f, 120.0f, 100.0f, 55.0f, font, vector<string>{"12", "13", "14", "15", "16", "17", "18"}, 7, 0);
	DropDownList drpDwnListMajors(220.0f, 120.0f, 500.0f, 55.0f, font, vector<string>{"Computer Engineering", "Computer Science", "Electrical Engineering", "Software Engineering", "Civil Engineering", "Aerospace Engineering", "Mechanical Engineering", "Studies of the Third Kind"}, 8, 0);
	bool arrow = false;
	int semester = 1;
	int semesterIndex = 0;
	vector<string> semesters;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {

			case sf::Event::Closed:
				window.close();
			case sf::Event::MouseMoved:
				btnLoad.update(Mouse::getPosition(window));
				drpDwnList.update(Mouse::getPosition(window), 2.0);
				drpDwnListMajors.update(Mouse::getPosition(window), 2.0);
				break;
			case sf::Event::MouseButtonPressed:
				btnLoad.update(Mouse::getPosition(window));
				drpDwnList.update(Mouse::getPosition(window), 2.0);
				drpDwnListMajors.update(Mouse::getPosition(window), 2.0);
				if (spRightButtonEnabled.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					if (semesterIndex + 2 < semesters.size()) {
						semesterIndex = semesterIndex + 2;
					}
					else if (!(semesterIndex + 1 == semesters.size() - 1))
						semesterIndex = semesters.size() - 1;
				}
				if (spLeftButtonEnabled.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					if (semesterIndex - 2 > 0) {
						semesterIndex = semesterIndex - 2;
					}
					else {
						semesterIndex = 0;
					}
				}
				if (btnLoad.isPressed()) {
					semesters.clear();
					arrow = true;
					semester = 1;
					semesterIndex = 0;
					int requiredCredit = 127;
					int totalCredit = 0;
					int maxCreditPerSemester = drpDwnList.getActiveElementId() + 12;
					int major = drpDwnListMajors.getActiveElementId();
					int semesterCredit = 0;

					Graph<Course> graph;
					random_device rd;
					mt19937 mt(rd());
					std::uniform_real_distribution<double> dist(1000, std::nextafter(9999, DBL_MAX));
					std::uniform_real_distribution<double> dist2(65, std::nextafter(90, DBL_MAX));

					int i = 0;
					switch (major) {
					case 0:
						graph[Course("COP3502C", "Joshua Fox", 3)].connectTo(Course("COP3503", "Mr. Fox", 4), 0.67);
						graph[Course("COP3502", "Joshua Fox", 3)].connectTo(Course("CDA3100", "Mr. Bosch", 3), 0.67);
						graph[Course("COP3503", "Mr. Fox", 4)].connectTo(Course("COP3530", "Mr. Bosch", 4), 0.67);
						graph[Course("COP3530", "Mr. Bosch", 4)].connectTo(Course("EEL3701C", "Mr. Bosch", 4), 0.67);
						graph[Course("CDA3100", "Mr. Bosch", 3)].connectTo(Course("EEL3701C", "Mr. Bosch", 4), 0.67);
						graph[Course("CDA3100", "Mr. Bosch", 3)].connectTo(Course("COP3503", "Mr. Fox", 4), 0.67);
						graph[Course("CDA3100", "Mr. Bosch", 3)].connectTo(Course("COP3530", "Mr. Bosch", 4), 0.67);
						graph[Course("CDA3100", "Mr. Bosch", 3)].connectTo(Course("CDA3101", "Mr. Bosch", 4), 0.67);
						graph[Course("CDA3101", "Mr. Bosch", 4)].connectTo(Course("EEL3701C", "Mr. Bosch", 4), 0.67);
						break;
					case 1:
						graph[Course("COP3502C", "Joshua Fox", 3)].connectTo(Course("COP3503C", "Mr. Fox", 4), 0.67);
						graph[Course("COP3502C", "Joshua Fox", 3)].connectTo(Course("COT3100", "Mr. Fox", 4), 0.67);
						graph[Course("COP3503C", "Joshua Fox", 3)].connectTo(Course("CDA3101", "Mr. Fox", 4), 0.67);
						graph[Course("COT3100", "Joshua Fox", 3)].connectTo(Course("CDA3101", "Mr. Fox", 4), 0.67);
						graph[Course("COP3503C", "Joshua Fox", 3)].connectTo(Course("COP3530", "Mr. Fox", 4), 0.67);
						graph[Course("COT3100", "Joshua Fox", 3)].connectTo(Course("COP3530", "Mr. Fox", 4), 0.67);
						graph[Course("COP3530", "Joshua Fox", 3)].connectTo(Course("CEN3031", "Mr. Fox", 4), 0.67);
						graph[Course("COP3503C", "Joshua Fox", 3)].connectTo(Course("CIS4301", "Mr. Fox", 4), 0.67);
						graph[Course("COT3100", "Joshua Fox", 3)].connectTo(Course("CIS4301", "Mr. Fox", 4), 0.67);
						graph[Course("COP3530", "Joshua Fox", 3)].connectTo(Course("COP4600", "Mr. Fox", 4), 0.67);
						graph[Course("CDA3101", "Joshua Fox", 3)].connectTo(Course("COP4600", "Mr. Fox", 4), 0.67);
						graph[Course("COP3530", "Joshua Fox", 3)].connectTo(Course("COP4020", "Mr. Fox", 4), 0.67);
						graph[Course("COP3530", "Joshua Fox", 3)].connectTo(Course("COP4533", "Mr. Fox", 4), 0.67);
						graph[Course("COP4600", "Joshua Fox", 3)].connectTo(Course("CNT4007C", "Mr. Fox", 4), 0.67);
						graph[Course("COP4533", "Joshua Fox", 3)].connectTo(Course("EGN4952", "Mr. Fox", 4), 0.67);
						break;
					case 7:
						while (graph.getVertexCount() < 100000) {
							string temp1 = "C";
							string temp2 = "C";
							temp1.push_back(char((int)dist2(mt)));
							temp1.push_back(char((int)dist2(mt)));
							temp2.push_back(char((int)dist2(mt)));
							temp2.push_back(char((int)dist2(mt)));
							i++;
							graph[Course(temp1 + to_string((int)dist(mt)), "Joshua Fox", 3 + (i % 2))].connectTo(Course(temp2 + to_string((int)dist(mt)), "Joshua Fox", 3 + (i % 2)), 1.0);
						}
						break;
					}

					map<int, Course> schedule;

					std::chrono::time_point<std::chrono::system_clock> start, end;
					start = std::chrono::system_clock::now();
					vector<Course> courses = graph.topsort();
					end = std::chrono::system_clock::now();
					string s = "elapsed time for queue BFS algorithm: \n     " + to_string(((std::chrono::duration<double>)(end - start)).count()) + "s\n";
					start = std::chrono::system_clock::now();
					graph.topsortBF();
					end = std::chrono::system_clock::now();
					s += "elapsed time for brute force BFS algorithm: \n     " + to_string(((std::chrono::duration<double>)(end - start)).count()) + "s\n\n";
					s += " " + to_string(graph.getVertexCount()) + " courses (vertices)";
					time.setString(s);
					semesters.push_back("SEMESTER 1\n\n");
					set<string> postreq;
					for (Course c : courses) { 
						if ((requiredCredit - totalCredit) < 0) break;
						if (semesterCredit + c.getCredits() > maxCreditPerSemester) {
							semester++;
							semesterCredit = 0;
							semesters.push_back(((semester % 2 == 0) ? "\nSEMESTER " : "SEMESTER ") + to_string(semester) + "\n\n");
						}
						semesters.back().append("   " + c.getId() + "(" + to_string(c.getCredits()) + " credits) taught by " + c.getInstructor() + "\n");
						schedule[semester] = c;
						totalCredit += c.getCredits();
						semesterCredit += c.getCredits();
					}
					cout << semesters.size();
					totNumSemesters.setString("Total semesters: " + to_string(semester));
					creditsNeeded.setString("(" + to_string(totalCredit) + " taken; " + to_string(((requiredCredit - totalCredit) < 0) ? 0 : (requiredCredit - totalCredit)) + " needed)");
				}
				if (btnLoad.isPressed() || spRightButtonEnabled.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) || spLeftButtonEnabled.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
					string out = semesters.at(semesterIndex);
					if (semesterIndex != semesters.size() - 1) {
						out.append(semesters.at(semesterIndex + 1));
					}
					output.setString(out);
					spBox.setTexture(box);
				}
				break;
			}
		}

		window.clear(sf::Color(242, 242, 242, 255));
		window.draw(spBox);
		window.draw(spNavBar);
		if (arrow) {
			if (semesterIndex > 0) {
				window.draw(spLeftButtonEnabled);
			}
			else
				window.draw(spLeftButtonDisabled);
			if (semesterIndex + 1 < semesters.size() - 1) {
				window.draw(spRightButtonEnabled);
			}
			else {
				window.draw(spRightButtonDisabled);
			}
		}
		window.draw(output);
		window.draw(totNumSemesters);
		window.draw(creditsNeeded);
		window.draw(time);
		btnLoad.render(window);
		drpDwnListMajors.render(window);
		drpDwnList.render(window);
		window.display();
	}

	return 0;
}
