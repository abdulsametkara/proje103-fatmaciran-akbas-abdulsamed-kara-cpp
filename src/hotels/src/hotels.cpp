#define _CRT_SECURE_NO_WARNINGS
#include "../header/hotels.h"
#include <stdexcept>
#include <regex>
#include <fstream>
#include <stdio.h>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>

using namespace std;

User activeUser;


const char* pathFileUsers = "Users.bin";
const char* pathFileRooms = "Rooms.bin";


void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//Waits for the user to press enter to continue.
bool enterToContinue(istream& in, ostream& out) {
    out << "\nPress enter to continue";
    while (in.get() != '\n');
    return true;
}

/*getchar() fonksiyonu ile buffer'dan bir karakter okunur ve c deðiþkenine atanýr.
Eðer okunan karakter yeni satýr karakteri (\n) veya dosya sonu karakteri (EOF) deðilse, 
döngü devam eder ve buffer temizlenene kadar karakterler okunur.*/

bool handleInputError(istream& in, ostream& out) {
    int c;
    while ((c = in.get()) != '\n' && c != EOF);
    out << "Invalid input. Please enter a number." << std::endl;
    return true;
}

/**
 * @brief Reads an integer input from the input stream.
 *
 * This function reads a line from the input stream and attempts to convert it to an integer.
 * If the input is not a valid integer, it returns an error code.
 *
 * @param in Input stream to read from.
 * @return int The integer input if valid, -1 if the input stream fails, -2 if the input is not a valid integer.
 */
int getInput(istream& in) {
    int choice;
    in >> choice;

    // Eðer girdi geçerli bir tam sayý deðilse hata kodu döner
    if (in.fail()) {
        in.clear(); // Hata bayraðýný temizle
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Geçersiz giriþi temizle
        return -2; // Geçersiz giriþ hatasý
    }

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Kalan karakterleri temizle
    return choice; // Baþarýlý giriþ
}

int getNewUserId(User users[], int userCount) {
    if (userCount == 0) {
        return 1; // Eðer kullanýcý yoksa, ilk kimlik 1 olur
    }

    int maxId = users[0].id;

    // Tüm kullanýcýlarýn kimliklerini kontrol ederek en yüksek kimliði bul
    for (int i = 1; i < userCount; ++i) {
        if (users[i].id > maxId) {
            maxId = users[i].id;
        }
    }

    // Yeni kullanýcý kimliði olarak en yüksek kimliðin bir fazlasýný döner
    return maxId + 1;
}

int activedUser(User loginUser, const char* pathFileUsers, istream& in, ostream& out) {
    ifstream file(pathFileUsers, ios::binary);
    if (!file.is_open()) {
        out << "Failed to open user file." << endl;
        return 0;
    }

    int userCount = 0;
    file.read(reinterpret_cast<char*>(&userCount), sizeof(int));
    if (userCount == 0) {
        out << "No users registered." << endl;
        file.close();
        enterToContinue(in, out);
        return 0;
    }

    User userFromFile;
    for (int i = 0; i < userCount; i++) {
        file.read(reinterpret_cast<char*>(&userFromFile), sizeof(User));
        if (strcmp(userFromFile.nickname, loginUser.nickname) == 0 && strcmp(userFromFile.password, loginUser.password) == 0) {
            out << "Login successful. Welcome " << endl;
            file.close();
            enterToContinue(in, out);
            activeUser = userFromFile;
            return 1;
        }
    }

    out << "Incorrect  or password." << endl;
    file.close();
    enterToContinue(in, out);
    return 0;
}


int activeUserMenu(const char* pathFileUsers, istream& in, ostream& out) {
    clearScreen();
    User activeUsers;

    out << "Enter nickname: ";
    in.getline(activeUsers.nickname, sizeof(activeUsers.nickname));

    out << "Enter password: ";
    in.getline(activeUsers.password, sizeof(activeUsers.password));

    return activedUser(activeUsers, pathFileUsers, in, out);
}

bool printMainMenu(ostream& out) {
    clearScreen();
    out << "1. Login\n";
    out << "2. Register\n";
    out << "3. Guest Mode\n";
    out << "4. Exit\n";
    return true;
}

bool printGuestMenu(ostream& out) {
    out << "1. View Categories\n";
    out << "2. Exit";
    return true;
}

bool printUserMenu(ostream& out) {
    clearScreen();
    out << "Hello " << activeUser.name << " " << activeUser.surname << " Please Enter Your Choice: \n";
    out << "1. Room Management\n";
    out << "2. Customer Reservation\n";
    out << "3. Financials\n";
    out << "4. Additional Services\n";
    out << "5. Integrations\n";  // New menu option
    out << "6. Exit\n";
    return true;
}

bool printRoomManagementMenu(ostream& out) {
    clearScreen();
    out << "1. Add Room\n";
    out << "2. Categorize Room\n";
    out << "3. Update Room\n";
    out << "4. Delete Room\n";
    out << "5. Amenities Listing\n";
    out << "6. Exit\n";
    return true;
}

bool printCustomerReservationMenu(ostream& out) {
    clearScreen();
    out << "1. Booking\n";
    out << "2. Check-in Process\n";
    out << "3. Check-out Process\n";
    out << "4. Guest Feedback\n";
    out << "5. Exit\n";
    return true;
}

bool printFinancialsMenu(ostream& out) {
    clearScreen();
    out << "1. Invoive Generation\n";
    out << "2. Payment Tracking\n";
    out << "3. Promotional Offers\n";
    out << "4. Exit\n";
    return true;
}

bool printAdditionalServicesMenu(ostream& out) {
    clearScreen();
    out << "1. Room Service\n";
    out << "2. Event Booking\n";
    out << "3. Transportation Services\n";
    out << "4. Exit\n";
    return true;
}

bool printIntegrationsMenu(ostream& out) {
    clearScreen();
    out << "1. Travel Agencies\n";
    out << "2. Customer Review Platforms\n";
    out << "3. Exit\n";
    return true;
}

int registerUser(User user, const char* pathFileUser, istream& in, ostream& out) {
    fstream file(pathFileUser, ios::binary | ios::in | ios::out);
    int userCount = 0;
    User* users = nullptr;

    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&userCount), sizeof(int));
        if (userCount > 0) {
            users = new User[userCount];
            file.read(reinterpret_cast<char*>(users), sizeof(User) * userCount);

            for (int i = 0; i < userCount; ++i) {
                if (strcmp(users[i].nickname, user.nickname) == 0) {
                    out << "User already exists." << endl;
                    file.close();
                    delete[] users;
                    enterToContinue(in, out);
                    return 0;
                }
            }
        }
        file.seekg(0, ios::beg);
    }
    else {
        file.open(pathFileUser, ios::binary | ios::out);
    }

    user.id = getNewUserId(users, userCount);
    userCount++;
    User* updatedUsers = new User[userCount];
    if (users) {
        copy(users, users + (userCount - 1), updatedUsers);
        delete[] users;
    }
    updatedUsers[userCount - 1] = user;

    file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
    file.write(reinterpret_cast<const char*>(updatedUsers), sizeof(User) * userCount);

    out << "User registered successfully " << endl;

    delete[] updatedUsers;
    file.close();
    enterToContinue(in, out);
    return 1;
}


int usersOperations(istream& in, ostream& out) {
    int choice;

    while (true) {
        printUserMenu(out);
        choice = getInput(in);

        if (in.fail()) {
            handleInputError(in, out);
            continue;
        }

        switch (choice) {
        case 1:
            RoomManagementMenu(in, out);
            break;

        case 2:
            //deadlineSettingsMenu(in, out);
            break;

        case 3:
            //reminderSystemMenu(in, out);
            break;

        case 4:
            //taskPrioritizationMenu(in, out);
            break;

        case 5:
            //flowAlgorithmsMenu(in, out);
            break;

        case 6:
            return 0;

        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int registerUserMenu(const char* pathFileUsers, istream& in, ostream& out) {
    clearScreen();
    User newUser;

    out << "Enter Name: ";
    in.getline(newUser.name, sizeof(newUser.name));

    out << "Enter Surname: ";
    in.getline(newUser.surname, sizeof(newUser.surname));

    out << "Enter nickname: ";
    in.getline(newUser.nickname, sizeof(newUser.nickname));

    out << "Enter password: ";
    in.getline(newUser.password, sizeof(newUser.password));

    if (registerUser(newUser, pathFileUsers, in, out)) {
        return 1;
    }
    else {
        out << "Registration failed." << endl;
    }

    enterToContinue(in, out);
    return 1;
}

int guestOperation(istream& in, ostream& out) {
    int choice;

    while (true) {
        printGuestMenu(out);
        choice = getInput(in);

        if (in.fail()) {
            handleInputError(in, out);
            continue;
        }

        switch (choice) {
        case 1:
            clearScreen();
            out << "1. Room" << endl;
            out << "2. Services" << endl;
            out << "3. a" << endl;
            out << "4. b" << endl;
            enterToContinue(in, out);
            break;

        case 2:
            return 0;

        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int mainMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printMainMenu(out);
        choice = getInput(in);

        if (in.fail()) {
            handleInputError(in, out);
            continue;
        }

        switch (choice) {
        case 1:
            clearScreen();
            if (activeUserMenu(pathFileUsers, in, out)) {
                usersOperations(in, out);
            }
            break;

        case 2:
            clearScreen();
            registerUserMenu(pathFileUsers, in, out);
            break;

        case 3:
            clearScreen();
            out << "Guest Operations\n";
            guestOperation(in, out);
            break;

        case 4:
            clearScreen();
            out << "Exit Program\n";
            return 0;

        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}


int RoomManagementMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printRoomManagementMenu(out);
        choice = getInput(in);

        if (in.fail()) { handleInputError(in, out); continue; }

        switch (choice) {
        case 1:
            //addRoom
            break;
        case 2:
            //Categorize Room
            break;
        case 3:
            //Update Room
            break;
        case 4:
            //Delete Room
            break;
        case 5:
            //Amenities Listing
            break;

        case 6:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}