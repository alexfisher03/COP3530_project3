// #include <cpprest/http_listener.h>
// #include <cpprest/json.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include "csv.h"
#include "Car.h"
#include "AVL.h"

using namespace std;
// using namespace web;
// using namespace web::http;
// using namespace web::http::experimental::listener;

vector<Car> readCSV(const string& filename) {
    vector<Car> cars;
    io::CSVReader<26, io::trim_chars<' ', '\t'>, io::no_quote_escape<','>, io::throw_on_overflow, io::no_comment> in(filename);

    in.read_header(io::ignore_extra_column,
                   "id", "url", "region", "region_url", "price", "year",
                   "manufacturer", "model", "condition",
                   "cylinders", "fuel", "odometer", "title_status", "transmission", "VIN", 
                   "drive", "size", "type", "paint_color", "image_url", "description", "county", "state", 
                   "lat", "long", "posting_date");

    string id, url, region, region_url, price, year, manufacturer, model, condition, cylinders, fuel, odometer, title_status, transmission, VIN;
    string drive, size, type, paint_color, image_url, description, county, state, lat, lon, posting_date;

    size_t line_number = 1;
    while (true) {
        try {
            if (!in.read_row(id, url, region, region_url, price, year, manufacturer, model, condition, cylinders, fuel, odometer, title_status, transmission, VIN, drive, size, type, paint_color, image_url, description, county, state, lat, lon, posting_date)) {
                break; // End of file
            }
            cars.push_back(Car{id, url, region, price, year, manufacturer, model, condition, odometer, VIN, drive, type, paint_color, image_url, state, posting_date});
            ++line_number;
        } catch (const io::error::too_many_columns& e) {
            // Skip this problematic row and continue
            ++line_number;
            continue;
        }
    }

    return cars;
}

void printUniqueMakes(const vector<Car>& cars, unordered_set<string>& makes) {
    for (const auto& car : cars){
        makes.insert(car.manufacturer);
    }

    cout << "\n\033[1;32m---------------------------------------------------------------------------\033[0m\n\n";
    for (const auto& make : makes){
        cout << make << " | ";
    }
    cout << "\n\033[1;32m---------------------------------------------------------------------------\033[1;32m\n" << endl;
}


void populateTreeByMake(AVLTree& tree, const vector<Car>& cars, const string& make) {
    for (const auto& car : cars){
        if (car.manufacturer == make){
            tree.insert(car);
        }
    }
}

void printUniqueModels(const AVLTree& tree){
    tree.printUniqueModels();
}

vector<Car> getCarsByModel(const vector<Car>& cars, const string& model){
    vector<Car> filteredCars;
    for(const auto& car : cars){
        if (car.model == model && car.odometer != "0" && !car.odometer.empty() && car.price != "0" && !car.price.empty()) {
            filteredCars.push_back(car);
        }
    }
    return filteredCars;
}
    
// Print Top 5 cars of a certain model
void printTopDeals(const vector<Car>& cars, const string& model) {
    vector<Car> filteredCars = getCarsByModel(cars, model);
    if (filteredCars.empty()) {
        cout << "Invalid model. No cars found for the specified model: " << model << endl;
        return;
    }

    sort(filteredCars.begin(), filteredCars.end(), [](const Car& a, const Car& b) {
        double priceA = stod(a.price);
        double odometerA = stod(a.odometer);
        double priceB = stod(b.price);
        double odometerB = stod(b.odometer);
        return (priceA / odometerA) < (priceB / odometerB);
    });

    cout << "\033[0;97m---------------------------------------------------------------------------\033[0;97m\n";
    cout << "\033[0;97mTop 5 best deals for model \033[0;97m" << model << ":\n";
    for (size_t i = 0; i < min(filteredCars.size(), size_t(5)); ++i) {
        const auto& car = filteredCars[i];
        cout << i + 1 << ". ID: " << car.id << ", Manufacturer: " << car.manufacturer << ", Model: " << car.model
             << ", Price: " << car.price << ", Odometer: " << car.odometer << endl;
    }
}

void printCarDetails(const Car& car) {
    cout << "\n------------------------------\n\tSELECTED CAR \n------------------------------\n";
    cout << "ID: " << car.id << " | "
         << "Region: " << car.region << " | "
         << "Price: " << car.price << " | "
         << "Year: " << car.year << " | "
         << "Manufacturer: " << car.manufacturer << " | "
         << "Model: " << car.model << " | "
         << "Condition: " << car.condition << " | "
         << "Odometer: " << car.odometer << " | "
         << "VIN: " << car.VIN << " | "
         << "Drive: " << car.drive << " | "
         << "Type: " << car.type << " | "
         << "Paint Color: " << car.paint_color << " | "
         << "State: " << car.state << " | "
         << "Posting Date: " << car.posting_date << " | \n";
}

bool isValidInput(const string& input) {
    regex pattern("^[a-zA-Z0-8 ]+$");
    return regex_match(input, pattern);
}

int main() {
    string ascii_art = R"(⠀⠀⠀⠀⠀                           ⢀⣀⡤⢤⣶⣶⣶⣶⣶⣒⣒⣀⣺⣿⣿⠿⢶⣶⣶⣶⣦⣤⣤⣤⣄⣀⣀⣀⣀⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠴⠚⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠛⠒⠲⠦⢤⣉⠙⣿⣿⣿⣟⢿⣿⠿⠿⠿⢿⣿
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣶⣷⡦⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⡇⠀⠈⠛⢿⣿⡀⠀⠀⠀⠻
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣤⠤⢴⣿⣉⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠇⠀⠀⠀⠀⠙⣿⣄⡀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣤⡶⠞⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠓⠒⠒⠢⠤⠤⣄⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⠀⢠⠤⠤⠤⣤⣾⠀⠙⢦⡀
⠀⠀⠀⠀⠀⠀⠀⠀⣠⠴⣾⣛⣩⢴⣿⠿⠶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⠒⠒⠒⠤⠤⢤⣤⣾⢿⣴⢏⣀⣀⣤⡼⠻⡆⠀⠈⢷
⠀⠀⠀⠀⠀⢀⣴⠋⠁⠀⠀⠀⠀⠙⠓⠲⠤⠬⠷⠀⠀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⠤⠞⠉⠀⠀⠹⡟⠀⠀⠀⠀⠀⢱⠀⢠⣼
⠀⠀⠀⣀⡴⠋⠈⢙⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠢⠤⢄⣀⣠⠴⠛⣋⣠⠴⠒⠉⠉⢉⣲⠶⠀⣀⡠⠤⠒⠊⠉⠁⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⠀⠀⠀⣼⢰⣿⣿
⡀⣠⣼⠏⠀⠀⣰⠟⠦⢤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠓⠒⠒⠒⠋⣉⠤⠒⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⢀⣯⣼⣿⣿
⡟⠋⠹⢤⣠⠞⠁⠀⠀⠀⠀⠈⠉⠐⠲⠤⢄⣀⡀⠀⠀⠀⠀⣀⠤⠤⠤⠤⠤⣄⣀⣀⠀⢀⡠⠖⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⢸⣿⣿⣿⣿
⢻⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⢒⡶⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⢦⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⣿⡝⣆⠀⠀⠀⠀⢸⠀⠀⣠⣴⠿⣻⣿⣿⠈
⢿⣿⣿⠶⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢨⠇⠀⠀⠀⠀⣀⣀⣤⣤⣾⣿⢿⡆⢸⣧⢹⠀⠀⣠⣴⣿⣴⠟⢋⣥⠴⢻⣿⠏⠀
⢸⣿⣿⡇⠀⠘⣿⣶⣦⣤⣀⡀⠀⠀⠀⠀⠀⠀⠈⠑⠲⠤⢤⣀⣀⡀⠀⠀⠀⠀⠀⠀⣀⣀⣞⣀⣠⣤⣾⣿⣿⣿⣿⣿⣿⢻⢸⣿⣾⣿⣾⣤⣾⣿⠿⢋⡡⠞⠉⠉⠉⠉⠀⠀⠀
⣼⡟⣿⡇⠀⠀⢹⣄⠈⠙⠒⠯⣽⣶⢶⣤⣤⣄⣀⣀⠀⠀⠀⠀⠈⠙⠻⠿⠿⠿⠿⠿⠿⠛⠛⠛⢉⣉⣽⠶⣿⠟⣿⣿⣧⣿⣾⣿⣿⡇⢹⡿⠋⣡⠖⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀
⣇⠳⣼⣧⠴⠛⠉⠉⠉⠒⣦⣤⣀⡀⠀⠀⠉⠙⢦⠀⠉⠉⢙⣷⣶⠒⠒⠒⠶⡶⠶⠶⢶⣤⠖⠚⠉⠁⠀⣰⣷⠾⣿⢿⢥⣼⣾⣿⣿⠃⣸⣠⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠈⠳⣌⣿⢦⣄⠀⢀⣴⣿⣿⣷⣿⡯⣗⠲⠤⣀⡈⣇⠀⠀⠻⡄⠈⣷⠀⠀⠀⡿⠒⠶⠾⢿⠀⠀⠀⢠⣾⡏⠁⣸⡇⢸⡸⢠⡌⢡⣿⠴⠿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠈⠙⠳⢿⣙⠺⢽⣿⣮⣿⣿⠗⠋⠀⠀⠀⠉⡿⠀⠀⣀⣹⣿⣯⣤⣶⣚⣛⣒⣛⣓⣿⠀⠀⠀⢽⣿⣶⣶⡏⢧⣼⣷⡿⢁⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠈⠙⠢⢬⣙⠛⠧⢤⣀⣀⠀⢀⣴⠃⠀⢀⣹⢦⣤⣉⣉⣯⣍⣹⣿⣿⣿⡃⣀⡤⠴⠛⠋⠁⠀⡇⠘⣜⣏⣠⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠓⠦⢤⣈⡉⠉⠙⠛⠛⠛⠲⠤⠤⠤⠴⣶⣶⣿⣿⢿⡿⣯⠀⠀⠀⠀⠀⢀⣠⡟⠤⠿⠟⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠒⠲⠤⢤⣄⣀⣀⣀⣤⣈⣤⣤⠤⣴⣿⣥⠤⠴⠒⠚⠋⠉⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀)";

    cout << "\n\033[0;96m----------------------------------------------------\033[0;96m\n";
    cout << "\033[0;96m---------------WELCOME TO CAR CACHE-----------------\033[0;96m\n";
    cout << ascii_art << '\n';
    cout << "\n\033[0;95m-----------------LOADING LISTINGS-------------------\033[0;95m\n";

    string filename = "./data/vehicles.csv";
    vector<Car> cars = readCSV(filename);


    unordered_set<string> makes;
    cout << "\n\033[\033[0;97m---------------SELECT A CAR MAKE ---------------\033[0;97m\n";
    printUniqueMakes(cars, makes);

    cout << "\n\033[0;96m---------------------------------------------\033[0;96m\n\033[0;93mEnter the Car Make You Want Details For: \033[0;93m";
    cout << "\033[0;96m\n-------------------------------------\033[0;96m\n\033[0;93m\033[0;93m";
    string make;
    cin >> make;
    while (!isValidInput(make) || makes.find(make) == makes.end()) {
        cout << "\033[0;91mInvalid input. Please, enter a valid car make from within the hash set: \033[0;91m";
        cin >> make;
    }


    AVLTree carTree;
    populateTreeByMake(carTree, cars, make);

    cout << "\n\033[0;96m-------Unique Car Models for Make: \033[0;96m" << make << " !\n";
    printUniqueModels(carTree);

    cout << "\033[0;96m\n-------------------------------------\033[0;96m\n\033[0;93mEnter the Car Model You Want Details For: \033[0;93m";
    cout << "\033[0;96m\n-------------------------------------\033[0;96m\n\033[0;93m\033[0;93m";
    string model;
    cin >> model;
    while (!isValidInput(model)) {
        cout << "\033[0;91mInvalid input. Please enter a valid car model: \033[0;91m";
        cin >> model;
    }

    printTopDeals(cars, model);

    cout << "\033[0;96mEnter the number (1-5) to see full details of the car: \033[0;96m";
    int choice;
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\033[0;91mInvalid input. Please enter a number between 1 and 5: \033[0;91m";
        cin >> choice;
    }

    vector<Car> filteredCars = getCarsByModel(cars, model);

    if (choice >= 1 && choice <= 5 && choice <= filteredCars.size()) {
        const auto& car = filteredCars[choice - 1];
        printCarDetails(car);
    } else {
        cout << "\033[0;91mInvalid choice.\033[0;91m\n";
    }

    return 0;
}