#include "strings.h"

//
// App specific strings
//
const QString sAppName = "Just Guessing";
const QString sAppDesc = "This game will get you a burger!\nJust try to guess the secret word!";
//
// Home page
//
const QString sStart = "Start";
const QString sHighScores = "High Scores";
const QString sHelp = "Help";
const QString sSettings = "Settings";
const QString sQuit = "Quit";
const QString sBack = "Back";

//
// Categories page
//
const QString sPickLabel = "Pick a Category";
const QString sBurgerToolTip = "This category contains words related to burgers and such stuff";
const QString sCompToolTip = "This category contains words related to computers, programming languages, etc.";
const QString sCarToolTip = "This category contains words about cars and related stuff";
const QString sMapToolTip = "This category contains words related to countries, cities, and other geographical objects";

//
// Player Username page
//
const QString sPlayerUsername = "Player's username:";
const QString sPressEnterToLeaveEmpty = "(or press Enter to play anonymously " + QString::fromWCharArray(L"\u2013") + " your points will still be saved)";
const QString sUsernameExists = "The username already exists. Please choose another one.";

//
// High Scores page
//
const QString sNHighestScores = "%1 Highest Scores";

//
// Settings page
//
const QString sSounds = "Sounds";

//
// Correct guess page
//
const QString sWikiBtnToolTip = "<font size=\"15px\">Open the Wikipedia page about %1</font>";
const QString sChooseCategory = "Choose Another Category";
const QString sNextPuzzle = "Next Puzzle";

//
// Help Page
//
const QString sGameInstructions = "Game Instructions";
const QString sInstructionsText = "This is a modified version of the Hangman game... A whole new version, actually.\n\n"
"This game is aimed to reward you with something you choose. BUT... In order to get it you have "
"to guess the secret word :) Just guess.\n\nWhen the game starts, you have your item. Each time the game starts, a secret word "
"is generated. You need to select one of the available letters to unlock it. If you choose a letter that the secret word "
"doesn't contain, you lose a part of your item. So, your goal is to guess the word as quickly as possible "
"to have as much of the item as possible.\n\nSo, good luck!";
const QString sLetsPlay = "Let's Play!";
