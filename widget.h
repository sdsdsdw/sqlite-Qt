#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QSqlQuery>

class CarDatabase : public QWidget {
    Q_OBJECT

public:
    CarDatabase(QWidget *parent = nullptr);
    ~CarDatabase();
private slots:
    void addCar();
    void removeCar();
    void searchCar();

private:
    void createConnection();
    void loadDatabase();
    void setupUi();
    void displayResults(QSqlQuery& query);

    QLineEdit *modelLineEdit;
    QLineEdit *brandLineEdit;
    QSpinBox *yearSpinBox;
    QDoubleSpinBox *priceDoubleSpinBox;
    QPushButton *addButton;
    QPushButton *removeButton;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QTableWidget *carTableWidget;
};

#endif // WIDGET_H
