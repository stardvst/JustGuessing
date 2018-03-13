#ifndef CATEGORIESPAGE_H
#define CATEGORIESPAGE_H

#include <QWidget>

// forward declarations
class JustGuessing;
class PropertiesBuilder;
class CategoryProperties;

class CategoriesPage : public QWidget
{
	Q_OBJECT
public:
	explicit CategoriesPage(JustGuessing *pGame, QWidget *parent = nullptr);
	~CategoriesPage();

	CategoryProperties *getCategoryProps() const;

signals:
	void canStartGame();
	void categoryChanged();

	private slots:
	void onBurgerBtnClicked();
	void onCompBtnClicked();
	void onCarsBtnClicked();
	void onMapBtnClicked();
private:
	void setCategoryProperties() const;
private:
	// the one game object
	JustGuessing * m_pGame;

	// category page properties
	PropertiesBuilder *m_pPropBuilder;
	CategoryProperties *m_pCategoryProps;
};

#endif // !CATEGORIESPAGE_H
