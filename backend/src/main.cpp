// #include <cpprest/http_listener.h>
// #include <cpprest/json.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <windows.h>
#include <iomanip>
#include "csv.h"
#include "Car.h"
#include "AVL.h"

using namespace std;
// using namespace web;
// using namespace web::http;
// using namespace web::http::experimental::listener;


const size_t MAX_STRING_LENGTH = 100;

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

            // Trim the strings to the maximum length if necessary
            if (model.length() > MAX_STRING_LENGTH) model = model.substr(0, MAX_STRING_LENGTH);
            if (manufacturer.length() > MAX_STRING_LENGTH) manufacturer = manufacturer.substr(0, MAX_STRING_LENGTH);

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
    for (const auto& car : cars) {
        makes.insert(car.manufacturer);
    }

    // Calculate the number of columns based on the longest make and console width
    size_t max_length = 0;
    for (const auto& make : makes) {
        if (make.length() > max_length) {
            max_length = make.length();
        }
    }

    size_t console_width = 80;
    size_t columns = console_width / (max_length + 2);

    // Print makes in columns
    cout << "\n\033[1;32m---------------------------------------------------------------------------\033[0m\n";
    size_t count = 0;
    for (const auto& make : makes) {
        cout << "| " << make;
        // Add padding to align columns
        cout << string(max_length - make.length(), ' ') << " | ";
        count++;
        if (count % columns == 0) {
            cout << "\n";
        }
    }
    if (count % columns != 0) {
        cout << "\n";
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
        if (car.model.size() > 100 || car.model.empty()) {
            continue;
        }

        if (car.model == model && car.odometer != "0" && !car.odometer.empty() && car.price != "0" && !car.price.empty()) {
            filteredCars.push_back(car);
        }
    }
    return filteredCars;
}
    
// Print Top 5 cars of a certain model
bool printTopDeals(const vector<Car>& cars, const string& model) {
    vector<Car> filteredCars = getCarsByModel(cars, model);
    if (filteredCars.empty()) {
        cout << "Invalid model. No cars found for the specified model: \033[0;93m" << model << endl;
        cout << "\nWould you like to continue ? (y/n) : \033[0;93m";
            char choice2;
            cin >> choice2;
            if (choice2 == 'n' || choice2 == 'N') {
                exit(0);
            }
        return false;;
    }

    sort(filteredCars.begin(), filteredCars.end(), [](const Car& a, const Car& b) {
        double priceA = stod(a.price);
        double odometerA = stod(a.odometer);
        double priceB = stod(b.price);
        double odometerB = stod(b.odometer);
        return (priceA / odometerA) < (priceB / odometerB);
    });

    cout << "\n\033[0;97m---------------------------------------------------------------------------\033[0;97m\n";
    cout << "\033[0;97mTop 5 best deals for model \033[0;97m" << model << ":\n";
    for (size_t i = 0; i < min(filteredCars.size(), size_t(5)); ++i) {
        const auto& car = filteredCars[i];
        cout << i + 1 << ". ID: " << car.id << ", Manufacturer: " << car.manufacturer << ", Model: " << car.model
             << ", Price: " << car.price << ", Odometer: " << car.odometer << endl;
    }
    return true;
}

void printCarDetails(const Car& car) {
    const int labelWidth = 15;
    const int valueWidth = 30;

    cout << "\033[0;95m\n------------------------------\n\tSELECTED CAR \n------------------------------\033[0;95m\n";

    cout << left << setw(labelWidth) << "ID:" << setw(valueWidth) << car.id << "\n";
    cout << left << setw(labelWidth) << "Region:" << setw(valueWidth) << car.region << "\n";
    cout << left << setw(labelWidth) << "Price:" << setw(valueWidth) << car.price << "\n";
    cout << left << setw(labelWidth) << "Year:" << setw(valueWidth) << car.year << "\n";
    cout << left << setw(labelWidth) << "Manufacturer:" << setw(valueWidth) << car.manufacturer << "\n";
    cout << left << setw(labelWidth) << "Model:" << setw(valueWidth) << car.model << "\n";
    cout << left << setw(labelWidth) << "Condition:" << setw(valueWidth) << car.condition << "\n";
    cout << left << setw(labelWidth) << "Odometer:" << setw(valueWidth) << car.odometer << "\n";
    cout << left << setw(labelWidth) << "VIN:" << setw(valueWidth) << car.VIN << "\n";
    cout << left << setw(labelWidth) << "Drive:" << setw(valueWidth) << car.drive << "\n";
    cout << left << setw(labelWidth) << "Type:" << setw(valueWidth) << car.type << "\n";
    cout << left << setw(labelWidth) << "Paint Color:" << setw(valueWidth) << car.paint_color << "\n";
    cout << left << setw(labelWidth) << "State:" << setw(valueWidth) << car.state << "\n";
    cout << left << setw(labelWidth) << "Posting Date:" << setw(valueWidth) << car.posting_date << "\033[0;97m\n";
}

bool isValidInput(const string& input) {
    regex pattern("^[a-zA-Z0-9 -]+$");
    return regex_match(input, pattern);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    string ascii_title = R"( 
______________________________________________________________________________
|               ______              ______           __                       |
|              / ____/___ ______   / ____/___ ______/ /_  ___                 |
|             / /   / __ `/ ___/  / /   / __ `/ ___/ __ \/ _ \                |
|            / /___/ /_/ / /     / /___/ /_/ / /__/ / / /  __/                |
|            \____/\__,_/_/      \____/\__,_/\___/_/ /_/\___/                 |
|_____________________________________________________________________________|              
                                                 )";
    string ascii_art = R"(⠀⠀⠀⠀⠀     
                                         ⢀⣀⡤⢤⣶⣶⣶⣶⣶⣒⣒⣀⣺⣿⣿⠿⢶⣶⣶⣶⣦⣤⣤⣤⣄⣀⣀⣀⣀⡀⠀⠀
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
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠒⠲⠤⢤⣄⣀⣀⣀⣤⣈⣤⣤⠤⣴⣿⣥⠤⠴⠒⠚⠋⠉⡀
          
    )";

    cout << "\n\n\n\n\033[0;96m----------------------------------------------------\033[0;96m\n";
    cout << "\033[0;95m-----------     WELCOME TO CAR CACHE    ------------\033[0;95m\n";
    cout << "\033[0;96m----------------------------------------------------\033[0;96m\n\n\n\033[0;95m";
    cout << ascii_title << "\033[0;96m\n";
    cout << ascii_art << '\n';
    cout << "\n\033[0;95m----------------------------------------------------\033[0;95m";
    cout << "\n\033[0;95m-----------------LOADING LISTINGS-------------------\033[0;95m";
    cout << "\n\033[0;95m----------------------------------------------------\033[0;95m\n";
    cout << "\n\033[1;32m---------------------------------------------------------------------------\033[1;32m\n" << endl;

    string filename = "./data/vehicles.csv";
    vector<Car> cars = readCSV(filename);

    unordered_set<string> makes;
    while (true) {
        cout << "\n\033[0;93m----------------------------------------------------\033[0;93m";
        cout << "\n\033[0;93m-----------------SELECT A CAR MAKE------------------\033[0;93m";
        cout << "\n\033[0;93m----------------------------------------------------\033[0;93m";
        printUniqueMakes(cars, makes);

        cout << "\n\033[0;96m----------------------------------------\033[0;96m\n\033[0;93mEnter the Car Make You Want Details For: \033[0;97m";
        string make;
        cin >> make;
        
        while (!isValidInput(make) || makes.find(make) == makes.end()) {
            cout << "\033[0;91mInvalid input. Please, enter a valid car make from within the hash set: \033[0;91m\033[0;93m";
            cin >> make;
        }

        AVLTree carTree;
        populateTreeByMake(carTree, cars, make);

        cout << "\n\033[0;96m-------Unique Car Models for Make: \033[0;96m" << make << " !\n";
        printUniqueModels(carTree);

        cout << "\033[0;96m\n---------------------------------------\033[0;96m\n\033[0;93mEnter the Car Model You Want Details For: \033[0;97m";
        string model;
        cin >> model;
        while (!isValidInput(model)) {
            cout << "\033[0;91mInvalid input. Please enter a valid car model: \033[0;91m\033[0;93m";
            cin >> model;
        }

        vector<Car> filteredCars = getCarsByModel(cars, model);
        if (filteredCars.empty()) {
            cout << "\033[0;91mInvalid model. No cars found for the specified model: \033[0;93m" << model << "\033[0;93m";
            cout << "\nWould you like to continue ? (y/n) : \033[0;93m";
            char choice;
            cin >> choice;
            
            while(choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cin >> choice;
            }
            if (choice == 'n' || choice == 'N') {
                break;
            }

            continue;
        }


        bool anyCars = printTopDeals(cars, model);

        if (anyCars) {
            int choice = 0;
            do {
                cout << "\033[0;96mEnter the number (1-5) to see full details of the car: \033[0;96m";
                cin >> choice;
                while (cin.fail() || choice < 1 || choice > 5 || choice > filteredCars.size()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\033[0;91mInvalid input. Please enter a number between 1 and " << min(5, static_cast<int>(filteredCars.size())) << ": \033[0;93m";
                    cin >> choice;
                }
                const auto& car = filteredCars[choice - 1];
                printCarDetails(car);
            } while (choice < 1 || choice > 5 || choice > filteredCars.size());

            cout << "\nWould you like to continue ? (y/n) : \033[0;93m";
            char choice2;
            cin >> choice2;

            while(choice2 != 'y' && choice2 != 'Y' && choice2 != 'n' && choice2 != 'N') {
                cin >> choice2;
            }
            if (choice2 == 'n' || choice2 == 'N') {
                break;
            }

            continue;
        }
    }

    return 0;
}