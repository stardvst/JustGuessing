#include "categoriesPage.h"
#include "../JustGuessing.h"
#include "../strings.h"
#include "../appGuiUtil.h"

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>


// builders
#include "../builders/burgerBuilder.h"
#include "../builders/compBuilder.h"
#include "../builders/carsBuilder.h"
#include "../builders/mapBuilder.h"

CategoriesPage::CategoriesPage(JustGuessing *pGame, QWidget *parent /* = nullptr */) 
	: QWidget(parent)
	, m_pGame(pGame)
	, m_pPropBuilder(nullptr)
	, m_pCategoryProps(nullptr) {

	// Back button
	const auto backBtn = new QPushButton { this };
	backBtn->setIcon(QIcon { ":/controls/back_button" });
	backBtn->setStyleSheet("background: rgba(0, 0, 0, 0); margin-top: 4px;");
	backBtn->setCursor(Qt::PointingHandCursor);
	backBtn->setMaximumSize(backBtn->minimumSizeHint());
	connect(backBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showHomePage);

	// "Pick Category" text label
	const auto pickLabel = new QLabel { sPickLabel };
	pickLabel->setStyleSheet("font: 24px; color: white");

	// Burger button
	const auto burgerBtn = new CategoryButton { ":/burger/burger_0", "Burger", sBurgerToolTip, this };
	connect(burgerBtn, &QAbstractButton::clicked, this, &CategoriesPage::onBurgerBtnClicked);
	connect(burgerBtn, &QAbstractButton::clicked, this, &CategoriesPage::canStartGame);
	connect(burgerBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showGamePage);

	// Computer button
	const auto compBtn = new CategoryButton { ":/computer/computer_0", "Computers", sCompToolTip, this };
	connect(compBtn, &QAbstractButton::clicked, this, &CategoriesPage::onCompBtnClicked);
	connect(compBtn, &QAbstractButton::clicked, this, &CategoriesPage::canStartGame);
	connect(compBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showGamePage);

	// Cars button
	const auto carsBtn = new CategoryButton { ":/car/tachka_0", "Cars", sCarToolTip, this };
	connect(carsBtn, &QAbstractButton::clicked, this, &CategoriesPage::onCarsBtnClicked);
	connect(carsBtn, &QAbstractButton::clicked, this, &CategoriesPage::canStartGame);
	connect(carsBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showGamePage);

	// Map button
	const auto mapBtn = new CategoryButton { ":/map/map_0", "Geography", sMapToolTip, this };
	connect(mapBtn, &QAbstractButton::clicked, this, &CategoriesPage::onMapBtnClicked);
	connect(mapBtn, &QAbstractButton::clicked, this, &CategoriesPage::canStartGame);
	connect(mapBtn, &QAbstractButton::clicked, m_pGame, &JustGuessing::showGamePage);

	// create grid layout for category buttons
	const auto gridLayout = new QGridLayout;
	gridLayout->addWidget(burgerBtn, 0, 1);
	gridLayout->addWidget(compBtn, 0, 2);
	gridLayout->addWidget(carsBtn, 1, 1);
	gridLayout->addWidget(mapBtn, 1, 2);
	gridLayout->setColumnStretch(0, 1);
	gridLayout->setColumnStretch(3, 1);

	// create the main layout
	const auto pageLayout = new QVBoxLayout;
	pageLayout->addWidget(backBtn);
	pageLayout->addStretch(1);
	pageLayout->addWidget(pickLabel, 0, Qt::AlignHCenter);
	pageLayout->addSpacerItem(new QSpacerItem { 0, 25 });
	pageLayout->addLayout(gridLayout);
	pageLayout->addStretch(2);
	pageLayout->setSpacing(15);

	setLayout(pageLayout);
}

CategoriesPage::~CategoriesPage() {
	delete m_pPropBuilder;
	delete m_pCategoryProps;
}

CategoryProperties* CategoriesPage::getCategoryProps() const {
	return m_pCategoryProps;
}

void CategoriesPage::onBurgerBtnClicked() {
	if(m_pPropBuilder) delete m_pPropBuilder;
	//if(m_pCategoryProps) delete m_pCategoryProps;
	m_pPropBuilder = new BurgerBuilder;
	setCategoryProperties();
	m_pCategoryProps = dynamic_cast<BurgerBuilder *>(m_pPropBuilder)->getPropertiesPtr();
}

void CategoriesPage::onCompBtnClicked() {
	if(m_pPropBuilder) delete m_pPropBuilder;
	//if(m_pCategoryProps) delete m_pCategoryProps;
	m_pPropBuilder = new CompBuilder;
	setCategoryProperties();
	m_pCategoryProps = dynamic_cast<CompBuilder *>(m_pPropBuilder)->getPropertiesPtr();
}

void CategoriesPage::onCarsBtnClicked() {
	if(m_pPropBuilder) delete m_pPropBuilder;
	//if(m_pCategoryProps) delete m_pCategoryProps;
	m_pPropBuilder = new CarsBuilder;
	setCategoryProperties();
	m_pCategoryProps = dynamic_cast<CarsBuilder *>(m_pPropBuilder)->getPropertiesPtr();
}

void CategoriesPage::onMapBtnClicked() {
	if(m_pPropBuilder) delete m_pPropBuilder;
	//if(m_pCategoryProps) delete m_pCategoryProps;
	m_pPropBuilder = new MapBuilder;
	setCategoryProperties();
	m_pCategoryProps = dynamic_cast<MapBuilder *>(m_pPropBuilder)->getPropertiesPtr();
}

void CategoriesPage::setCategoryProperties() const {
	m_pPropBuilder->setPropertyName();
	m_pPropBuilder->setWordsFilePath();
	m_pPropBuilder->setIconPaths();
}
