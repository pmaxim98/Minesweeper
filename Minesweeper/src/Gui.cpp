#include "Gui.h"

#include "ResourceManager.h"
#include "Minefield.h"
#include "Difficulty.h"

#include <TGUI/Loading/Theme.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/RadioButton.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Animation.hpp>

#include <algorithm>

namespace minesweeper {

	Gui::Gui(Minefield & minefield, Difficulty & difficulty)
	{
		window.create(sf::VideoMode({std::max(minefield.getSize().x + 25u, 200u), std::max(minefield.getSize().y + 100u, 200u)}), "Minesweeper", sf::Style::Close);
		window.setIcon(64, 64, ResourceManager::getInstance().getIconPtr());
		window.setFramerateLimit(60);

		tgui::Gui::setWindow(window);
		tgui::Gui::setFont(ResourceManager::getInstance().getFont("Start"));

		auto theme = tgui::Theme::create("res/gui.txt");
		auto windowWidth = tgui::bindWidth(*this);
		auto windowHeight = tgui::bindHeight(*this);

		tgui::EditBox::Ptr minesLeftEditBox = theme->load("EditBox");
		minesLeftEditBox->setSize(40, 30);
		minesLeftEditBox->setPosition(windowWidth / 2 - minesLeftEditBox->getSize().x - 3, 10);
		minesLeftEditBox->setMaximumCharacters(4);
		minesLeftEditBox->setAlignment(tgui::EditBox::Alignment::Center);
		minesLeftEditBox->setTextSize(8);
		minesLeftEditBox->setInputValidator();
		minesLeftEditBox->setCaretWidth(0.0f);
		tgui::Gui::add(minesLeftEditBox, "minesLeftEditBox");

		tgui::EditBox::Ptr timerEditBox = theme->load("EditBox");
		timerEditBox->setSize(40, 30);
		timerEditBox->setPosition(windowWidth / 2 + 3, 10);
		timerEditBox->setMaximumCharacters(4);
		timerEditBox->setAlignment(tgui::EditBox::Alignment::Center);
		timerEditBox->setTextSize(8);
		timerEditBox->setInputValidator();
		timerEditBox->setCaretWidth(0.0f);
		tgui::Gui::add(timerEditBox, "timerEditBox");

		tgui::Picture::Ptr minesPicture = tgui::Picture::create(ResourceManager::getInstance().getTexture("Mine"));
		minesPicture->setSize(30, 30);
		minesPicture->setPosition(tgui::bindLeft(minesLeftEditBox) - minesPicture->getSize().x - 5, 10);
		tgui::Gui::add(minesPicture);

		tgui::Picture::Ptr clockPicture = tgui::Picture::create(ResourceManager::getInstance().getTexture("Clock"));
		clockPicture->setSize(30, 30);
		clockPicture->setPosition(tgui::bindRight(timerEditBox) + 5, 10);
		tgui::Gui::add(clockPicture);
	
		tgui::Button::Ptr buttonNewGame = theme->load("Button");
		buttonNewGame->setSize(90, 30);
		buttonNewGame->setPosition(windowWidth / 2 - buttonNewGame->getSize().x - 3, windowHeight - buttonNewGame->getSize().y - 10);
		buttonNewGame->setText("Reset");
		buttonNewGame->setTextSize(8);
		tgui::Gui::add(buttonNewGame);

		buttonNewGame->connect("pressed", [&, windowWidth, windowHeight] () { 
			minefield = Minefield(difficulty);

			clock.restart();
			window.setSize({std::max(minefield.getSize().x + 25u, 200u), std::max(minefield.getSize().y + 100u, 200u)});
			window.setView(sf::View(static_cast<sf::Vector2f>(window.getSize()) * 0.5f, static_cast<sf::Vector2f>(window.getSize())));
			minefield.center(getCenter());
			tgui::Gui::setView(window.getView());

			auto parentWindow = tgui::Gui::get("difficultyWindow");
			parentWindow->hide();
			parentWindow->setPosition(windowWidth / 2 - parentWindow->getFullSize().x / 2, windowHeight / 2 - parentWindow->getFullSize().y / 2);
		});

		tgui::Button::Ptr buttonDifficulty = theme->load("Button");
		buttonDifficulty->setSize(90, 30);
		buttonDifficulty->setPosition(windowWidth / 2 + 3, windowHeight - buttonDifficulty->getSize().y - 10);
		buttonDifficulty->setText("Difficulty");
		buttonDifficulty->setTextSize(8);
		tgui::Gui::add(buttonDifficulty);

		tgui::ChildWindow::Ptr diffWindow = theme->load("ChildWindow");
		diffWindow->setSize(170, 135);
		diffWindow->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Center);
		diffWindow->setPosition(windowWidth / 2 - diffWindow->getSize().x / 2, windowHeight / 2 - diffWindow->getSize().y / 2);
		diffWindow->hide();
		diffWindow->connect("closed", [=] () { diffWindow->hide(); });
		tgui::Gui::add(diffWindow, "difficultyWindow");

		tgui::RadioButton::Ptr radioButtonEasy = theme->load("RadioButton");
		radioButtonEasy->setSize(15, 15);
		radioButtonEasy->setPosition("15", "parent.height * 1 / 5 - height / 2");
		radioButtonEasy->setText("Easy");
		radioButtonEasy->setTextSize(8);
		radioButtonEasy->connect("checked", [&] () { difficulty = Difficulty(Difficulty::Type::Easy); });

		tgui::RadioButton::Ptr radioButtonMedium = theme->load("RadioButton");
		radioButtonMedium->setSize(15, 15);
		radioButtonMedium->setPosition("15", "parent.height * 2 / 5 - height / 2");
		radioButtonMedium->setText("Medium");
		radioButtonMedium->setTextSize(8);
		radioButtonMedium->connect("checked", [&] () { difficulty = Difficulty(Difficulty::Type::Medium); });

		tgui::RadioButton::Ptr radioButtonHard = theme->load("RadioButton");
		radioButtonHard->setSize(15, 15);
		radioButtonHard->setPosition("15", "parent.height * 3 / 5 - height / 2");
		radioButtonHard->setText("Hard");
		radioButtonHard->setTextSize(8);
		radioButtonHard->connect("checked", [&] () { difficulty = Difficulty(Difficulty::Type::Hard); });

		tgui::RadioButton::Ptr radioButtonCustom = theme->load("RadioButton");
		radioButtonCustom->setSize(15, 15);
		radioButtonCustom->setPosition("15", "parent.height * 4 / 5 - height / 2");
		radioButtonCustom->setText("Custom");
		radioButtonCustom->setTextSize(8);
		radioButtonCustom->connect("checked", [&, theme, diffWindow] () { 
			tgui::EditBox::Ptr rowsTextBox = theme->load("EditBox");
			rowsTextBox->setSize(65, 22);
			rowsTextBox->setPosition("parent.width - width - 15", "parent.height * 1 / 5 - height / 2");
			rowsTextBox->setMaximumCharacters(4);
			rowsTextBox->setTextSize(8);
			rowsTextBox->setInputValidator(tgui::EditBox::Validator::UInt);
			rowsTextBox->setDefaultText("Rows");

			tgui::EditBox::Ptr columnsTextBox = theme->load("EditBox");
			columnsTextBox->setSize(65, 22);
			columnsTextBox->setPosition("parent.width - width - 15", "parent.height * 2 / 5 - height / 2");
			columnsTextBox->setMaximumCharacters(4);
			columnsTextBox->setTextSize(8);
			columnsTextBox->setInputValidator(tgui::EditBox::Validator::UInt);
			columnsTextBox->setDefaultText("Columns");

			tgui::EditBox::Ptr minesTextBox = theme->load("EditBox");
			minesTextBox->setSize(65, 22);
			minesTextBox->setPosition("parent.width - width - 15", "parent.height * 3 / 5 - height / 2");
			minesTextBox->setMaximumCharacters(4);
			minesTextBox->setTextSize(8);
			minesTextBox->setInputValidator(tgui::EditBox::Validator::UInt);
			minesTextBox->setDefaultText("Mines");

			tgui::Button::Ptr applyButton = theme->load("Button");
			applyButton->setSize(65, 22);
			applyButton->setPosition("parent.width - width - 15", "parent.height * 4 / 5 - height / 2");
			applyButton->setText("Apply");
			applyButton->setTextSize(8);
			applyButton->connect("pressed", [&, rowsTextBox, columnsTextBox, minesTextBox, applyButton] () {
				const unsigned finalRows = rowsTextBox->getText().isEmpty() ? 0 : std::stoul(rowsTextBox->getText().toAnsiString());
				const unsigned finalColumns = columnsTextBox->getText().isEmpty() ? 0 : std::stoul(columnsTextBox->getText().toAnsiString());
				const unsigned finalMines = minesTextBox->getText().isEmpty() ? 0 : std::stoul(minesTextBox->getText().toAnsiString());

				difficulty = Difficulty(finalRows, finalColumns, finalMines);

				rowsTextBox->setText(std::to_string(difficulty.getRows()));
				columnsTextBox->setText(std::to_string(difficulty.getColumns()));
				minesTextBox->setText(std::to_string(difficulty.getMines()));

				applyButton->getParent()->hide();
			});

			diffWindow->add(rowsTextBox, "rowsTextBox");
			diffWindow->add(columnsTextBox, "columnsTextBox");
			diffWindow->add(minesTextBox, "minesTextBox");
			diffWindow->add(applyButton, "applyButton");
		});
		radioButtonCustom->connect("unchecked", [=] () { 
			diffWindow->remove(diffWindow->get("rowsTextBox"));
			diffWindow->remove(diffWindow->get("columnsTextBox"));
			diffWindow->remove(diffWindow->get("minesTextBox"));
			diffWindow->remove(diffWindow->get("applyButton"));
		});

		buttonDifficulty->connect("pressed",
								  [=] () {
			if (!diffWindow->isVisible()) {
				diffWindow->add(radioButtonEasy, "easy");
				diffWindow->add(radioButtonMedium, "medium");
				diffWindow->add(radioButtonHard, "hard");
				diffWindow->add(radioButtonCustom, "custom");
				diffWindow->showWithEffect(tgui::ShowAnimationType::Scale, sf::Time::Time(sf::milliseconds(100)));
			}
		});
	}

	void Gui::updateFlagsCounter(int flags)
	{
		tgui::EditBox::Ptr minesLeftEditBox = tgui::Gui::get<tgui::EditBox>("minesLeftEditBox");
		minesLeftEditBox->setText(std::to_string(flags));
	}

	void Gui::updateClock()
	{
		tgui::EditBox::Ptr minesLeftEditBox = tgui::Gui::get<tgui::EditBox>("timerEditBox");
		minesLeftEditBox->setText(std::to_string(static_cast<int>(clock.getElapsedTime().asSeconds())));
	}

	void Gui::showWinImage()
	{
		auto winImage = tgui::Gui::get("winImage");
		if (winImage)
			tgui::Gui::remove(winImage);
		winImage = tgui::Picture::create(ResourceManager::getInstance().getTexture("Like"));
		winImage->setSize(48, 48);
		winImage->setOpacity(0.8f);
		winImage->setPosition(tgui::bindWidth(*this) / 2 - winImage->getSize().x / 2, tgui::bindHeight(*this) / 2 - winImage->getSize().y / 2);
		tgui::Gui::add(winImage, "winImage");

		winImage->showWithEffect(tgui::ShowAnimationType::Scale, sf::Time::Time(sf::milliseconds(100)));
		winImage->hideWithEffect(tgui::ShowAnimationType::Fade, sf::Time::Time(sf::milliseconds(1000)));
	}

	sf::Vector2f Gui::getCenter() const
	{
		return static_cast<sf::Vector2f>(window.getSize()) * 0.5f;
	}

}