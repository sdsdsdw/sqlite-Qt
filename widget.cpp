// widget.cpp

#include "widget.h"

CarDatabase::CarDatabase(QWidget *parent) : QWidget(parent) {
    setupUi();
    createConnection();
    loadDatabase();
}

CarDatabase::~CarDatabase() {
    createConnection();
}

void CarDatabase::createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cars.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Failed to open the database");
        qApp->exit();
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS cars (model TEXT, brand TEXT, year INTEGER, price REAL)");
}

void CarDatabase::loadDatabase() {
    QSqlQuery query("SELECT * FROM cars");
    displayResults(query);
}

void CarDatabase::addCar() {
    QString model = modelLineEdit->text();
    QString brand = brandLineEdit->text();
    int year = yearSpinBox->value();
    double price = priceDoubleSpinBox->value();

    QSqlQuery query;
    query.prepare("INSERT INTO cars (model, brand, year, price) VALUES (:model, :brand, :year, :price)");
    query.bindValue(":model", model);
    query.bindValue(":brand", brand);
    query.bindValue(":year", year);
    query.bindValue(":price", price);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Car added to the database");
        loadDatabase();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add car to the database");
    }
}

void CarDatabase::removeCar() {
    int currentRow = carTableWidget->currentRow();
    if (currentRow != -1) {
        QString model = carTableWidget->item(currentRow, 0)->text();

        QSqlQuery query;
        query.prepare("DELETE FROM cars WHERE model = :model");
        query.bindValue(":model", model);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Car removed from the database");
            loadDatabase();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove car from the database");
        }
    } else {
        QMessageBox::warning(this, "Warning", "No car selected");
    }
}

void CarDatabase::searchCar() {
    QString searchParam = searchLineEdit->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM cars WHERE model = :searchParam OR brand = :searchParam OR year = :searchParam OR price = :searchParam");
    query.bindValue(":searchParam", searchParam);

    if (query.exec()) {
        if (query.next()) {
            carTableWidget->setRowCount(0);

            do {
                int row = carTableWidget->rowCount();
                carTableWidget->insertRow(row);

                for (int column = 0; column < 4; ++column) {
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(column).toString());
                    carTableWidget->setItem(row, column, item);

                    if (query.value(column).toString() == searchParam) {
                        item->setBackground(QBrush(QColor(Qt::darkCyan)));
                    }
                }
            } while (query.next());
        } else {
            QMessageBox::information(this, "Information", "No matching cars found in the database");
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to search for cars in the database");
    }
}


void CarDatabase::setupUi() {
    setFixedSize(800, 600);

    modelLineEdit = new QLineEdit;
    brandLineEdit = new QLineEdit;
    yearSpinBox = new QSpinBox;
    priceDoubleSpinBox = new QDoubleSpinBox;
    addButton = new QPushButton("Add Car");
    removeButton = new QPushButton("Remove Car");
    searchLineEdit = new QLineEdit;
    searchButton = new QPushButton("Search");

    carTableWidget = new QTableWidget;
    carTableWidget->setColumnCount(4);
    carTableWidget->setHorizontalHeaderLabels({"Model", "Brand", "Year", "Price"});

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Model:"));
    mainLayout->addWidget(modelLineEdit);
    mainLayout->addWidget(new QLabel("Brand:"));
    mainLayout->addWidget(brandLineEdit);
    mainLayout->addWidget(new QLabel("Year:"));
    mainLayout->addWidget(yearSpinBox);
    mainLayout->addWidget(new QLabel("Price:"));
    mainLayout->addWidget(priceDoubleSpinBox);
    mainLayout->addWidget(addButton);
    mainLayout->addWidget(removeButton);
    mainLayout->addWidget(new QLabel("Search:"));
    mainLayout->addWidget(searchLineEdit);
    mainLayout->addWidget(searchButton);
    mainLayout->addWidget(carTableWidget);

    setLayout(mainLayout);

    connect(addButton, &QPushButton::clicked, this, &CarDatabase::addCar);
    connect(removeButton, &QPushButton::clicked, this, &CarDatabase::removeCar);
    connect(searchButton, &QPushButton::clicked, this, &CarDatabase::searchCar);
}

void CarDatabase::displayResults(QSqlQuery& query) {
    carTableWidget->setRowCount(0);

    while (query.next()) {
        int row = carTableWidget->rowCount();
        carTableWidget->insertRow(row);

        for (int column = 0; column < 4; ++column) {
            carTableWidget->setItem(row, column, new QTableWidgetItem(query.value(column).toString()));
        }
    }
}
