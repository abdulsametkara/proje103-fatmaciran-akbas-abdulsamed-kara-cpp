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

int mainMenu(istream& in, ostream& out);
int RoomManagementMenu(istream& in, ostream& out);
int customerManagentMenu(istream& in, ostream& out);
int financialsMenu(istream& in, ostream& out);
int additionalServicesMenu(istream& in, ostream& out);
int integrationsMenu(istream& in, ostream& out);

#endif // HOTELS_H