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
Room rooms;
int roomCount;



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

/*getchar() fonksiyonu ile buffer'dan bir karakter okunur ve c de�i�kenine atan�r.
E�er okunan karakter yeni sat�r karakteri (\n) veya dosya sonu karakteri (EOF) de�ilse, 
d�ng� devam eder ve buffer temizlenene kadar karakterler okunur.*/

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

    // E�er girdi ge�erli bir tam say� de�ilse hata kodu d�ner
    if (in.fail()) {
        in.clear(); // Hata bayra��n� temizle
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ge�ersiz giri�i temizle
        return -2; // Ge�ersiz giri� hatas�
    }

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Kalan karakterleri temizle
    return choice; // Ba�ar�l� giri�
}

int getNewUserId(User users[], int userCount) {
    if (userCount == 0) {
        return 1; // E�er kullan�c� yoksa, ilk kimlik 1 olur
    }

    int maxId = users[0].id;

    // T�m kullan�c�lar�n kimliklerini kontrol ederek en y�ksek kimli�i bul
    for (int i = 1; i < userCount; ++i) {
        if (users[i].id > maxId) {
            maxId = users[i].id;
        }
    }

    // Yeni kullan�c� kimli�i olarak en y�ksek kimli�in bir fazlas�n� d�ner
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
    out << "5. Exit\n";
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
            customerManagentMenu(in, out);
            break;

        case 3:
            financialsMenu(in, out);
            break;

        case 4:
            additionalServicesMenu(in, out);
            break;

        case 5:
            integrationsMenu(in, out);
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
            addRoomMenu(pathFileRooms, in, out);
            break;
        case 2:
            categorizeRoomMenu(pathFileRooms, in, out);
            break;
        case 3:
            updateRoomMenu(pathFileRooms, in, out);
            break;
        case 4:
            deleteRoomMenu(pathFileRooms, in, out);
            break;
        case 5:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int customerManagentMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printCustomerReservationMenu(out);
        choice = getInput(in);

        if (in.fail()) { handleInputError(in, out); continue; }

        switch (choice) {
        case 1:
            //Booking
            break;
        case 2:
            //Check-in Process
            break;
        case 3:
            //Check-out Process
            break;
        case 4:
            //Guest Feedback
            break;
        case 5:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int financialsMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printFinancialsMenu(out);
        choice = getInput(in);

        if (in.fail()) { handleInputError(in, out); continue; }

        switch (choice) {
        case 1:
            //Invoive Generation
            break;
        case 2:
            //Payment Tracking
            break;
        case 3:
            //Promotional Offers
            break;
        case 4:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int additionalServicesMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printAdditionalServicesMenu(out);
        choice = getInput(in);

        if (in.fail()) { handleInputError(in, out); continue; }

        switch (choice) {
        case 1:
            //Room Service
            break;
        case 2:
            //Event Booking
            break;
        case 3:
            //Transportation Services
            break;
        case 4:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int integrationsMenu(istream& in, ostream& out) {
    int choice;

    while (true) {
        printIntegrationsMenu(out);
        choice = getInput(in);

        if (in.fail()) { handleInputError(in, out); continue; }

        switch (choice) {
        case 1:
            //Travel Agencies
            break;
        case 2:
            //Customer Review Platforms
            break;
        case 3:
            return 0;
        default:
            out << "\nInvalid choice. Please try again.\n";
            enterToContinue(in, out);
            break;
        }
    }
}

int loadRooms(Room** rooms, const char* pathFileRooms) {
    FILE* file = fopen(pathFileRooms, "rb");    // Dosyay� ikili modda a�
    if (!file) {
        return 0;   // Dosya a��lamazsa 0 d�n
    }

    fseek(file, 0, SEEK_END);                   // Dosyan�n sonuna git
    int fileSize = ftell(file);                 // Dosya boyutunu al
    rewind(file);                               // Dosya ba��na geri d�n

    int roomCount = fileSize / sizeof(Room);    // Dosyadaki oda say�s�n� hesapla
    *rooms = (Room*)malloc(roomCount * sizeof(Room));  // Bellekte yer ay�r

    if (fread(*rooms, sizeof(Room), roomCount, file) != roomCount) {
        fclose(file);                           // Okuma hatas� varsa dosyay� kapat
        free(*rooms);                           // Ayr�lan belle�i serbest b�rak
        *rooms = NULL;                          // ��aret�iyi NULL yap
        return 0;                               // 0 d�n
    }

    fclose(file);                               // Dosyay� kapat
    return roomCount;                           // Oda say�s�n� d�n
}

int saveRooms(Room* rooms, int roomCount, const char* pathFileRooms, istream& in, ostream& out) {
    FILE* file = fopen(pathFileRooms, "wb");    // Dosyay� yazma modunda a�
    if (file) {
        fwrite(rooms, sizeof(Room), roomCount, file);  // Odalar� dosyaya yaz
        fclose(file);                           // Dosyay� kapat
    }
    return 1;
}

int addRoomMenu(const char* pathFileRooms, istream& in, ostream& out) {
    clearScreen();                              // Ekran� temizle
    Room newRoom;                               // Yeni bir oda olu�tur

    out << "Enter room number: ";               // Oda numaras�n� iste
    in >> newRoom.roomNumber;
    in.ignore();

    out << "Enter room type (Suit, Single, King Room, Family Room, Deluxe Room): ";                 // Oda tipini iste
    in.getline(newRoom.roomType, sizeof(newRoom.roomType));

    out << "Enter room price: ";                // Oda fiyat�n� iste
    in >> newRoom.price;
    in.ignore();

    out << "Enter room capacity: ";             // Oda kapasitesini iste
    in >> newRoom.capacity;
    in.ignore();

    out << "Enter room amenities (Wi-Fi, TV, Air Conditioning, Minibar, Terrace): "; // Oda imkanlar�n� iste
    in.getline(newRoom.amenities, sizeof(newRoom.amenities));

    Room* rooms = NULL;                         // Mevcut odalar� y�klemek i�in i�aret�i
    int roomCount = loadRooms(&rooms, pathFileRooms);  // Mevcut odalar� y�kle

    Room* newRooms = (Room*)malloc((roomCount + 1) * sizeof(Room));  // Yeni oda dahil bellek ay�r
    if (rooms) {
        memcpy(newRooms, rooms, roomCount * sizeof(Room));  // Mevcut odalar� kopyala
        free(rooms);                       // Eski odalar� serbest b�rak
    }
    newRooms[roomCount] = newRoom;         // Yeni oday� ekle
    saveRooms(newRooms, roomCount + 1, pathFileRooms, in, out);  // G�ncellenmi� odalar� kaydet
    free(newRooms);                        // Belle�i serbest b�rak

    out << "Room added successfully.\n";   // Ba�ar� mesaj�
    enterToContinue(in, out);              // Devam etmek i�in bekle
    return 1;                              // Ba�ar�yla tamamland�
}

int updateRoomMenu(const char* pathFileRooms, istream& in, ostream& out) {
    clearScreen();                          // Ekran� temizle
    int roomNumber;

    out << "Enter room number to update: "; // G�ncellenecek oda numaras�n� iste
    in >> roomNumber;
    in.ignore();

    Room* rooms = NULL;                     // Mevcut odalar� y�klemek i�in i�aret�i
    int roomCount = loadRooms(&rooms, pathFileRooms);  // Mevcut odalar� y�kle

    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {  // Oda numaras�n� bul
            out << "Enter new room type: ";  // Yeni oda tipini iste
            in.getline(rooms[i].roomType, sizeof(rooms[i].roomType));

            out << "Enter new room price: "; // Yeni oda fiyat�n� iste
            in >> rooms[i].price;
            in.ignore();

            out << "Enter new room capacity: ";  // Yeni oda kapasitesini iste
            in >> rooms[i].capacity;
            in.ignore();
            
            out << "Enter new room Amenities: ";  // Yeni oda kapasitesini iste
            in >> rooms[i].amenities;
            in.ignore();

            saveRooms(rooms, roomCount, pathFileRooms, in, out);  // G�ncellenmi� odalar� kaydet
            free(rooms);                    // Belle�i serbest b�rak
            out << "Room updated successfully.\n";  // Ba�ar� mesaj�
            enterToContinue(in, out);       // Devam etmek i�in bekle
            return 1;                       // Ba�ar�yla tamamland�
        }
    }

    free(rooms);                            // Oda bulunamazsa belle�i serbest b�rak
    out << "Room not found.\n";             // Hata mesaj�
    enterToContinue(in, out);               // Devam etmek i�in bekle
    return 0;                               // Ba�ar�s�z
}

int deleteRoomMenu(const char* pathFileRooms, istream& in, ostream& out) {
    clearScreen();                          // Ekran� temizle
    int roomNumber;

    out << "Enter room number to delete: "; // Silinecek oda numaras�n� iste
    in >> roomNumber;
    in.ignore();

    Room* rooms = NULL;                     // Mevcut odalar� y�klemek i�in i�aret�i
    int roomCount = loadRooms(&rooms, pathFileRooms);  // Mevcut odalar� y�kle

    int newRoomCount = 0;                   // Yeni oda say�s�n� tut
    Room* newRooms = (Room*)malloc(roomCount * sizeof(Room));  // Yeni odalar i�in bellek ay�r

    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber != roomNumber) {  // Silinecek oda d���ndaki odalar� kopyala
            newRooms[newRoomCount++] = rooms[i];
        }
    }

    saveRooms(newRooms, newRoomCount, pathFileRooms, in, out);  // G�ncellenmi� odalar� kaydet
    free(rooms);                        // Eski odalar� serbest b�rak
    free(newRooms);                     // Yeni odalar� serbest b�rak

    out << "Room deleted successfully.\n";  // Ba�ar� mesaj�
    enterToContinue(in, out);               // Devam etmek i�in bekle
    return 1;                               // Ba�ar�yla tamamland�
}

int printRoomsByType(const char* pathFileRooms, const char* roomType, istream& in, ostream& out) {
    Room* rooms = NULL;                         // Mevcut odalar� y�klemek i�in i�aret�i
    int roomCount = loadRooms(&rooms, pathFileRooms);  // Mevcut odalar� y�kle

    bool found = false;                         // Belirli bir oda tipi bulunup bulunmad���n� takip eder

    for (int i = 0; i < roomCount; i++) {       // T�m odalar� dola�
        if (strcmp(rooms[i].roomType, roomType) == 0) { // Oda tipi e�le�irse
            found = true;                       // Oda tipi bulundu
            out << "Room Number: " << rooms[i].roomNumber << "\n";
            out << "Room Type: " << rooms[i].roomType << "\n";
            out << "Price: " << rooms[i].price << "\n";
            out << "Capacity: " << rooms[i].capacity << "\n";
            out << "Amenities: " << rooms[i].amenities << "\n";
            out << "-------------------------\n";
        }
    }

    if (!found) {
        out << "No rooms found of type: " << roomType << "\n";  // Hi� oda bulunamazsa mesaj ver
    }

    free(rooms);                               // Belle�i serbest b�rak
    return 1;
}

int categorizeRoomMenu(const char* pathFileRooms, istream& in, ostream& out) {
    clearScreen();                             // Ekran� temizle
    char roomType[100];

    out << "Enter room type to list: ";        // Listelenecek oda tipini iste
    in.getline(roomType, sizeof(roomType));

    printRoomsByType(pathFileRooms, roomType, in, out); // Belirli oda tipine g�re odalar� listele

    enterToContinue(in, out);                  // Devam etmek i�in bekle
    return 1;                                  // Ba�ar�yla tamamland�
}