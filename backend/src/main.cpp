#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <vector>
#include <string>
#include "csv.h"
#include "Car.h"
#include "AVL.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;


/**
 * Reads a CSV file and returns a vector of Car objects.
 *
 * @param filename The name of the CSV file to read, always gonna be "vehicles.csv".
 *
 * @return A vector of Car objects, where each Car object represents a row in the CSV file.
 *
 * @throws None
 */
vector<Car> readCSV(const string& filename){
    vector<Car> cars;
    io::CSVReader<15> in(filename); // CSVReader object<number of columns>

    in.read_header(io::ignore_extra_column, 
                   "id", "region", "price", "year",
                   "manufacturer", "model", "condition",
                   "odometer", "VIN", "drive", "type",
                   "paint_color", "state", "posting_date");

    string id, region, price, year, manufacturer, model, condition, odometer, VIN, drive, type, paint_color, state, posting_date;

    while (in.read_row(id, region, price, year, manufacturer, model, condition, odometer, VIN, drive, type, paint_color, state, posting_date)) {
        cars.push_back(Car{id, region, price, year, manufacturer, model, condition, odometer, VIN, drive, type, paint_color, state, posting_date});
    }
    return cars;
}

/**
 * DEBUG: Prints the details of each car in the given vector of cars. 
 *
 * @param cars A const reference to a vector of Car objects.
 *
 * @throws None
 */
void printCars(const vector<Car>& cars) {
    for (const auto& car : cars) {
        cout << "ID: " << car.id << ", Manufacturer: " << car.manufacturer << ", Model: " << car.model << ", Price: " << car.price << endl;
    }
}

/**
 * Handles a GET request by simulating fetching data from the AVL tree.
 *
 * @param request The HTTP request object containing the details of the request.
 *
 * @return void
 *
 * @throws None
 */
void handle_get(http_request request) {
    // Simulate fetching data from AVL tree for now
    json::value jsonArray = json::value::array();
    jsonArray[0][U("id")] = json::value::string(U("1"));
    jsonArray[0][U("manufacturer")] = json::value::string(U("Toyota"));
    jsonArray[0][U("model")] = json::value::string(U("Camry"));
    jsonArray[0][U("price")] = json::value::string(U("20000"));

    request.reply(status_codes::OK, jsonArray);
}

int main() {
    string filename = "./data/vehicles.csv";
    vector<Car> cars = readCSV(filename);
    printCars(cars);

    AVLTree carTree;
    for (const auto& car : cars) {
        carTree.insert(car);
    }
    carTree.printInOrder();

    uri_builder uri(U("http://localhost:8080"));
    auto addr = uri.to_uri().to_string();
    http_listener listener(addr);
    listener.support(methods::GET, handle_get);

    try {
        listener.open().then([&listener, &addr]() {
            wcout << L"Listening at: " << addr << endl;
        }).wait();
        wcout << L"Press ENTER to exit." << endl;
        string line;
        getline(cin, line);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}