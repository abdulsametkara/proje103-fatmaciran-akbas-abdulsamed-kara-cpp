/**
 * @file hotels.h
 * 
 * @brief Provides functions for math. utilities
 */

#ifndef HOTELS_H
#define HOTELS_H

#include "../../utility/header/commonTypes.h"
#include <iostream>

using namespace std;

typedef struct {
    int id;
    char name[100];
    char surname[100];
    char nickname[100];
    char password[100];
} User;

typedef struct {
    int roomNumber;       // Oda numarasý
    char roomType[100];   // Oda tipi
    double price;         // Oda fiyatý
    int capacity;         // Oda kapasitesi
    char amenities[200]; // Bu alanda odanýn sunduðu imkanlarý tutacaðýz.

} Room;


int mainMenu(istream& in, ostream& out);
int RoomManagementMenu(istream& in, ostream& out);
int customerManagentMenu(istream& in, ostream& out);
int financialsMenu(istream& in, ostream& out);
int additionalServicesMenu(istream& in, ostream& out);
int integrationsMenu(istream& in, ostream& out);



int loadRooms(Room** rooms, const char* pathFileRooms);
int saveRooms(Room* rooms, int roomCount, const char* pathFileRooms, istream& in, ostream& out);
int addRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
int updateRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
int addRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
int updateRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
int deleteRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
int printRoomsByType(const char* pathFileRooms, const char* roomType, istream& in, ostream& out);
int categorizeRoomMenu(const char* pathFileRooms, istream& in, ostream& out);
#endif // HOTELS_H