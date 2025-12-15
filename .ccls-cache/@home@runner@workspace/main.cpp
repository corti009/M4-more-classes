#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <random>
#include <chrono>

class Car {
private:
    std::string make;
    std::string model;
    std::string color;
    std::string licensePlate;

public:

    Car(std::string mk, std::string md, std::string clr, std::string lp)
        : make(mk), model(md), color(clr), licensePlate(lp) {}

    std::string getMake() const { return make; }
    std::string getModel() const { return model; }
    std::string getColor() const { return color; }
    std::string getLicensePlate() const { return licensePlate; }

    void setMake(const std::string& mk) { make = mk; }
    void setModel(const std::string& md) { model = md; }
    void setColor(const std::string& clr) { color = clr; }
    void setLicensePlate(const std::string& lp) { licensePlate = lp; }
};

class ParkingMeter {
private:
    double timePurchased; 
    double ratePerMinute; 

public:

    ParkingMeter(double time, double rate)
        : timePurchased(time), ratePerMinute(rate) {}

    double getTimePurchased() const { return timePurchased; }
    double getRatePerMinute() const { return ratePerMinute; }

    void setTimePurchased(double time) { timePurchased = time; }
    void setRatePerMinute(double rate) { ratePerMinute = rate; }
    void purchaseTime(double minutes) {
        timePurchased += minutes;
        std::cout << "[Meter] Driver purchased an additional " << minutes << " minutes.\n";
    }
};

class ParkingTicket {
private:
    double fineAmount;
    std::string licensePlate;
    double timeIn;  
    double timeOut; 

public:

    ParkingTicket(double fine, std::string lp, double ti, double to)
        : fineAmount(fine), licensePlate(lp), timeIn(ti), timeOut(to) {}


    double getFineAmount() const { return fineAmount; }
    std::string getLicensePlate() const { return licensePlate; }
    double getTimeIn() const { return timeIn; }
    double getTimeOut() const { return timeOut; }

    void setFineAmount(double fine) { fineAmount = fine; }
    void setLicensePlate(const std::string& lp) { licensePlate = lp; }
    void setTimeIn(double ti) { timeIn = ti; }
    void setTimeOut(double to) { timeOut = to; }
};

class PoliceOfficer {
private:
    std::string officerName;
    int badgeNumber;

public:

    PoliceOfficer(std::string name, int badge)
        : officerName(name), badgeNumber(badge) {}


    std::string getOfficerName() const { return officerName; }
    int getBadgeNumber() const { return badgeNumber; }


    void setOfficerName(const std::string& name) { officerName = name; }
    void setBadgeNumber(int badge) { badgeNumber = badge; }


    bool checkCar(const ParkingMeter& meter, double minutesParked) const {
        double timePurchased = meter.getTimePurchased();
        return minutesParked > timePurchased;
    }


    ParkingTicket* issueTicket(const Car& car, const ParkingMeter& meter, double minutesParked) const {
        if (!checkCar(meter, minutesParked)) {
            return nullptr; 
        }

        double timePurchased = meter.getTimePurchased();
        double minutesOverdue = minutesParked - timePurchased;
        double fine = 25.00;
        double overdueHours = minutesOverdue / 60.0;
        if (overdueHours > 1.0) {
            int additionalHours = static_cast<int>(std::ceil(overdueHours - 1.0));
            fine += (additionalHours * 10.00);
        }

        std::cout << "\n*** VIOLATION DETECTED ***\n";
        std::cout << "Car was overdue by " << std::fixed << std::setprecision(2) << minutesOverdue << " minutes.\n";
        std::cout << "Fine calculated: $" << fine << "\n";

        return new ParkingTicket(
            fine, 
            car.getLicensePlate(), 
            timePurchased, 
            minutesParked
        );
    }
};

void runSimulation(const Car& car, const ParkingMeter& meter, const PoliceOfficer& officer, double minutesParked) {
    std::cout << "\n======================================================\n";
    std::cout << "SIMULATION SCENARIO\n";
    std::cout << "======================================================\n";
    std::cout << "  Vehicle: " << car.getMake() << " " << car.getModel() << " (" << car.getColor() << ")\n";
    std::cout << "  License: " << car.getLicensePlate() << "\n";
    std::cout << "  Paid Time: " << meter.getTimePurchased() << " minutes\n";
    std::cout << "  Time Parked: " << minutesParked << " minutes\n";
    std::cout << "  Officer: " << officer.getOfficerName() << " (Badge #" << officer.getBadgeNumber() << ")\n";
    std::cout << "------------------------------------------------------\n";

    ParkingTicket* ticket = officer.issueTicket(car, meter, minutesParked);

    if (ticket) {
        std::cout << "\n[TICKET ISSUED]\n";
        std::cout << "  License Plate: " << ticket->getLicensePlate() << "\n";
        std::cout << "  Violation Duration: " << ticket->getTimeOut() - ticket->getTimeIn() << " minutes\n";
        std::cout << "  Total Fine: $" << std::fixed << std::setprecision(2) << ticket->getFineAmount() << "\n";
        delete ticket; 
    } else {
        std::cout << "\n[NO VIOLATION]\n";
        std::cout << "The vehicle was legally parked.\n";
    }
    std::cout << "======================================================\n";
}

int main() {

    PoliceOfficer officer("Agent Smith", 4201);
    Car parkedCar("Honda", "Civic", "Red", "XYZ-456");


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);


    std::uniform_real_distribution<> purchasedTimeDist(30.0, 180.0);
    double purchasedTime = std::round(purchasedTimeDist(generator));

    ParkingMeter meter(purchasedTime, 0.01); 
    double parkedTimeA = purchasedTime - 15.0; 
    std::cout << "--- Running Test Case A: No Violation ---\n";
    runSimulation(parkedCar, meter, officer, parkedTimeA);

    double parkedTimeB = purchasedTime + 45.0;
    std::cout << "\n--- Running Test Case B: Minor Violation (< 1 hour overdue) ---\n";
    runSimulation(parkedCar, meter, officer, parkedTimeB);

    double parkedTimeC = purchasedTime + 150.0; 
    std::cout << "\n--- Running Test Case C: Major Violation (> 2 hours overdue) ---\n";
    runSimulation(parkedCar, meter, officer, parkedTimeC);

    return 0;
}