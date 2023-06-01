#ifndef _PROJECT__H_
#define _PROJECT__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * \brief A nation in the Olympics
 */

typedef struct {
	char name[900]; //!< The name of the nation
	int gold; 	//!< The number of gold medals of the nation
	int silver; //!< The number of silver medals of the nation
    int bronze; //!< The number of bronze medals of the nation
} Nation;

/**
 * \brief Returns a comparison value obtained by comparing the number of bronze medals of Nation 
 *        objects pointed by arguments a and b.
 * 
 * \details The bronze field of a Nation is an integer.
 * 
 * \param a The first object to compare
 * \param b The second object to compare
 * 
 * \returns a negative number if a should be placed before b
 * \returns 0 if numbers of bronze medals are the same.
 * \returns a positive number if b should be placed before a
 */
int compareBronze(const void* a, const void* b);

/**
 * \brief Returns a comparison value obtained by comparing the number of silver medals of Nation 
 *        objects pointed by arguments a and b.
 * 
 * \details The silver field of a Nation is an integer. If numbers of silver medals are equal, nations 
 *          with higher number of bronze medals should be placed before the ones with lower number 
 *          of bronze medals.
 * 
 * \param a The first object to compare
 * \param b The second object to compare
 * 
 * \returns a negative number if a should be placed before b
 * \returns 0 if numbers of silver and bronze medals are the same.
 * \returns a positive number if b should be placed before a
 */
int compareSilver(const void* a, const void* b);

/**
 * \brief Returns a comparison value obtained by comparing the number of gold medals of Nation 
 *        objects pointed by arguments a and b.
 * 
 * \details The gold field of a Nation is an integer. If numbers of gold medals are equal, nations 
 *          with higher number of silver medals should be placed before the ones with lower number 
 *          of silver medals.
 * 
 * \param a The first object to compare
 * \param b The second object to compare
 * 
 * \returns a negative number if a should be placed before b
 * \returns 0 if numbers of medals are the same.
 * \returns a positive number if b should be placed before a
 */
int compareGold(const void* a, const void* b);

/**
 * \brief Returns a comparison value obtained by comparing the names of Nation 
 *        objects pointed by arguments a and b.
 * 
 * \details The name field of a Nation is a string.
 * 
 * \param a The first object to compare
 * \param b The second object to compare
 * 
 * \returns a negative number if a should be placed before b
 * \returns 0 if a and b have the same name.
 * \returns a positive number if b should be placed before a
 */
int compareName(const void* a, const void* b);

/**
 * \brief Returns the number of nations in a database. 
 * 
 * \param database The pointer to the database.
 * 
 * \return The number of nations in the database that have non-empty names. 
 */
int countNations(Nation* database);

/**
 * \brief Handles the command "A", add nation command.
 *
 * \details Parses the entry and adds a nation to the database, the newly added nation has 0 medals. If the 
 *          nation with the specified name is already in the database, the function prints an error message.
 * 
 * \param database The pointer to the database.
 * \param entry The pointer to the command from the user.
 *
 * \return The pointer to a new dynamically allocated database if the nation supposed to be added 
 *         is not in the database. 
 * \return A NULL pointer if the nation is already in the database.
 */
Nation* handleA(Nation* database, char* entry);

/**
 * \brief Handles the command "M", adding medals command.
 *
 * \details Parses the entry and adds the specified medals to a nation. If the given nation does 
 *          not exist in the database, an error message is printed.
 * 
 * \param database The pointer to the database.
 * \param entry The pointer to the command from the user.
 */
void handleM(Nation* database, char* entry);

/**
 * \brief A common print function that is used as a basis for print and save functions.
 *
 * \details Print the stored nations on a separate line as follows <name> <gold> <silver> <bronze>.
 *          The database entries are printed in the following order.
 *              The nations with higher number of gold medals should be printed first.
 *              If number of gold medals are equal, nations with higher number of silver medals should be printed above the others.
 *              If number of gold and silver medals are equal, the nations with higher number of bronze medals should be printed before the others.
 *              If their number of medals are the same, the nations can be printed in an arbitrary order.
 *          The functions prints the entries to standard output stream if the f argument is `stdout`, else prints the entries to the file 
 *          pointed by pointer f.
 *
 * \param database The pointer to the database.
 * \param f The File pointer. 
 */
void commonPrint(Nation* database, FILE* f);

/**
 * \brief Handles the command "L", displaying the database content.
 *
 * \details Passes the pointer to the database and a pointer to `stdout` to the common print function and let it handle the rest.
 * 
 * \param database The pointer to the database.
 *
 */
void handleL(Nation* database);

/**
 * \brief Handles the command "W", saving a database to a text file.
 *
 * \details Passes the pointer to the database and a pointer to `stdout` to the common print function and let it handle the rest.
 *          Prints error message if any error occurs.
 * 
 * \param database The pointer to the database.
 * \param entry The pointer to the entry.
 *
 */
void handleW(Nation* database, char* entry);

/**
 * \brief Returns the number of lines in a file. 
 * 
 * \param filename The name of the file.
 * \return The number of lines in the file, or -1 if an error occurs. 
 */
int line_count(const char* filename);

/**
 * \brief An helper function used to get the nation's name from a line used by funtion handleO.
 *
 * \details Parses the line and return the nation's name.
 * 
 * \param line The pointer to the line.
 *
 * \return A pointer to a dynamically allocated string of the nation's name.
 */
char* extractNationName(char* line);

/**
 * \brief An helper function used to get the file name.
 *
 * \details Parses the entry and return the file name.
 * 
 * \param entry The pointer to the entry.
 *
 * \return A pointer to a dynamically allocated string of the file name.
 */
char* extractFileName(char* entry);

/**
 * \brief Handles the command "O", loading nations from a text file saved using Save to file command.
 *
 * \details Opens the file for read and read line by line. For each line, parses the line and then adds the nation
 *			to the database.
 * 
 * \param entry The pointer to the entry.
 * 
 * \return The pointer to a dynamically allocated database.
 *
 */
Nation* handleO(char* entry);

#endif //! _PROJECT__H_