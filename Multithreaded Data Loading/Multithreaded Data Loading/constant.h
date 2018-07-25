#ifndef CONSTANT_H
#define CONSTANT_H
// constant.h
// created: 2018-07-22
// modified:
// author: Artyom Panfutov

const short int DO_NOT_DISPLAY_INFO = 0; // The setting for disable displaying text on the screen 
const short int DISPLAY_INFO = 1; // The setting for enable displaying text on the screen 

// Restrictions on the maximum length
const short int SQL_USERNAME_MAX_LENGTH = 128;
const short int SQL_INSTANCE_MAX_LENGTH = 15;
const short int SQL_DB_MAX_LENGTH = 128;
const short int SQL_PASSWORD_MAX_LENGTH = 128;
const short int SQL_DRIVER_MAX_LENGTH = 128;

#endif // CONSTANT_H