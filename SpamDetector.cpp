
/*******************************************include********************************************************************/
#include <iostream>
#include <fstream>
#include "HashMap.hpp"

/***********************************************define*****************************************************************/
static const std::string USAGE_MSG = "Usage: SpamDetector <database path> <message path> <threshold>";
static const std::string IVALID_MSG = "Invalid input";
static const std::string SPAM_MSG = "SPAM";
static const std::string NOT_SPAM_MSG = "NOT_SPAM";

#define ARGS_NUM 4
#define SEPARATE ','
#define SEPARATE_AMOUNT 1
#define DATA_INDEX 1
#define MSG_INDEX 2
#define THRESHOLD_INDEX 3

/*************************************************methods**************************************************************/

/**
 * check if the args are valid
 * @param argsNum the nummber of correct args
 * @return true if so , false otherwise
 */
bool isValidArgs(const int argsNum)
{
    if(argsNum != ARGS_NUM)
    {
        std::cerr << USAGE_MSG << std::endl;
        return false;
    }
    return true;
}

/**
 * check if an int is valid
 * @param numToCheck the num to check
 * @param line the line he is at
 * @param threshold boolen if the trashoold is zero
 * @return true if valid ,false otherwise
 */
bool isValidInt(int *numToCheck, const std::string& line, bool threshold)
{
    for(int i = 0; i < (int) line.size(); ++i)
    {
        if(!isdigit(line[i]))
        {
            return false;
        }
    }
    try
    {
        *numToCheck = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        return false;
    }
    if(*numToCheck <= 0)
    {
        return !threshold;
    }
    return true;


}

/**
 * check if a file gets a valid input
 * @param file the path of the file
 */
void inValidInput(std::ifstream* file)
{
    if(*file)
    {
        file->close();
    }
    std::cerr << IVALID_MSG << std::endl;
}

/**
 * take a line and transform all the letters ther to lowercase
 * @param line the line we transform.
 */
void toLowerCase(std :: string& line)
{
    for(auto& word : line)
    {
        word = std::tolower(word);
    }
}

/**
 * the error handling if a line is not valid
 * @param database the data base file
 * @param msg the msg file
 */
bool notValidLine(std::ifstream* database, std::ifstream* msg)
{
    database->close();
    msg->close();
    std::cerr << IVALID_MSG << std::endl;
    return false;
}

/**
 * check if a line is valid
 * @param database he data base file
 * @param msg the msg file
 * @param line the line we check
 * @param map the hash map we add the value and keys to
 */
bool validLine(std::ifstream* database, std::ifstream* msg,  std :: string line,
               HashMap<std::string, int>* map)
{
    int count = (int) std::count(line.begin(), line.end(), SEPARATE);
    if(count != SEPARATE_AMOUNT)
    {
        return notValidLine(database, msg);
    }
    int separate_index = (int)line.find(SEPARATE);
    std :: string scoreStr =  line.substr((separate_index + 1), line.size());
    if((int) scoreStr.size() == 0 )
    {
        return notValidLine(database, msg);
    }
    int val;
    if(!isValidInt(&val, scoreStr, false))
    {
        return notValidLine(database, msg);
    }
    std:: string key = line.substr(0, separate_index);
    toLowerCase(key);
    map->insert(key,  val);
    return true;
}

/**
 * the funck parsese the msg file
 * @param msgFile the msg file
 * @param msg the string that parse the msg to
 */
void parseMsgFile(std::ifstream* msgFile, std :: string& msg)
{
    std :: string line;
    while(getline(*msgFile, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        toLowerCase(line);
        msg += line;
        msg += SEPARATE;
    }
}


/**
 * calc the score for one pair in the hash map
 * @param msg the msg
 * @param key the key of the pair
 * @param value the value of the pair
 * @return the calc score by the formula
 */
int scoreCalcForPair(const std::string& msg, const std::string& key, int value)
{
    int counter = 0;
    for(auto i = msg.find(key); i != std::string::npos; i = msg.find(key, i + key.length()))
    {
        counter++;
    }
    return counter * value;
}

/**
 * uptating the total score of the msg that decieds if its a spam or not
 * @param score the score of the msg
 * @param map the map we hold the values att
 * @param msg the msg we check
 */
void updateScore(int* score, HashMap<std::string, int>* map, const std::string& msg)
{
    for (HashMap<std :: string, int>::const_iterator p = map->begin(); p != map->end(); ++p)
    {
        *score += scoreCalcForPair(msg, p->first, p->second);
    }
}

/**
 * the funck parse the file
 * @param database the database file
 * @param msg the msg file
 * @param score the score of the msg
 * @param map the map that hold the values
 */
bool parse(std::ifstream* database, std::ifstream* msg, int* score , HashMap<std::string, int>* map)
{
    std::string line;
    while(getline(*database, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if(!validLine(database, msg, line, map))
        {
            return false;
        }
    }
    std:: string msgStr;
    parseMsgFile(msg, msgStr);
    updateScore(score, map, msgStr);
    return true;
}

/**
 * the main func of the program
 * @param argc the number of args
 * @param argv the aray of args
 * @return failure or success
 */
int main(int argc, char *argv[])
{
    std::ifstream database, msg;
    int threshold = 0;
    HashMap<std::string, int> map;
    if(!isValidArgs(argc))
    {
        return 1;
    }
    if(!isValidInt(&threshold,  std::string(argv[THRESHOLD_INDEX]), true))
    {
        inValidInput(&database);
        return 1;
    }
    database.open( argv[DATA_INDEX], std::ios::in);
    msg.open( argv[MSG_INDEX], std::ios::in);
    if(!database.good())
    {
        inValidInput(&database);
        return 1;
    }
    if(!msg.good())
    {
        if (database)
        {
            database.close();
        }
        inValidInput(&msg);
        return 1;
    }
    int score = 0;
    if(!parse(&database, &msg, &score, &map))
    {
        return 1;
    }
    if(threshold <= score)
    {
        std::cout << SPAM_MSG << std::endl;
    }
    else
    {
        std::cout << NOT_SPAM_MSG << std::endl;
    }
    database.close();
    msg.close();
    return 0;



}