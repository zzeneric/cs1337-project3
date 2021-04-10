// Project 3: CS 1337 -- Raghav Pillai, RSP180007

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// Name of database file
const string database = "freeplay.dat";

// Function prototypes
bool validate(string type, string query);
void add(string linetext);
void search(string linetext);
void update(string linetext);
void remove(string linetext);

int main(){ // Main function
/* ################  DO NOT MODIFY CODE IN THIS BLOCK ###################### */
   string temp;  //variable for database filename
   string batch; //variable for batch filename

   cout<<"Enter Database Name: ";
   cin>>temp;

   ifstream infile(temp, ios::binary);
   ofstream outfile(database, ios::binary);
   string line;
   if (infile)
        while (infile.good())
        {
            getline(infile, line);
            if (line != "")
                outfile << line << "\n";
        }

   infile.close();
   outfile.close();
/* ################  DO NOT MODIFY CODE IN THIS BLOCK ###################### */

   //add your code for main here

   
    cout << "Enter Batch Name: "; // Asks for batch file name
    cin >> batch;
    cout << endl; // Add new line

    fstream ourfile;
    ourfile.open(batch, ios::in);

    string ourline;

    if (ourfile){
        while (ourfile.good()){
            getline(ourfile, ourline); // Gets line from databasefile
            if (ourline != ""){
                if (ourline[0] == '1'){ // If command char is add
                    add(ourline);
                }else if (ourline[0] == '2'){ // If command char is search
                    search(ourline);
                }else if (ourline[0] == '3'){ // If command char is update
                    update(ourline);
                }else if (ourline[0] == '4'){ // If command char is remove
                    //remove(ourline);
                }
            }
        }
    }else{
        cout << "Invalid input" << endl; // If none of the commands are fit
    }

    ourfile.close(); // Closes file

    // Removing all spare blank lines
    ourfile.open(database, ios::in); // Reopens file as databasefile now

    string tempstring;
    bool first = true; // Making sure first line doesn't create extra new line

    while(ourfile){
        getline(ourfile, ourline);
        if (ourline != ""){
            if (first == true){
                first = false;
            }else{
                tempstring = tempstring + "\n";
            }
            tempstring = tempstring + ourline;
        }
    }

    ourfile.close(); // Reopen file to output stored tempstring

    ourfile.open(database, ios::out);

    ourfile << tempstring << endl;

    ourfile.close(); // Closes file
    return 0;
}

bool validate(string type, string query){ // Global validate function (input the type of input and query)
    if (type == "name"){
        for (size_t i = 0; i < query.size(); i++){
            if (isalnum(query[i]) == 0){ // is not alphanumeric
                if (ispunct(query[i]) == 0){ // is not punctuation
                    if (isspace(query[i]) == 0){ // is not whitespace
                        return false;
                    }
                }
            }
        }
    }else if (type == "score"){
        for (size_t i = 0; i < query.size(); i++){
            if (isdigit(query[i]) == 0){ // is not digit
                return false;
            }
        }
        if (query.size() < 1 || query.size() > 9){ // if less than 1 or greater than 9
            return false;
        }
    }else if (type == "initials"){
        for (size_t i = 0; i < query.size(); i++){
            if (isalnum(query[i]) == 0){ // is alphanumeric
                if (ispunct(query[i]) == 0){ // is punctuation
                    return false;
                }
            }
        }
        if (query.size() != 3){ // if not 3 characters
            return false;
        }
    }else if (type == "plays"){
        for (size_t i = 0; i < query.size(); i++){
            if (isdigit(query[i]) == 0){ // is digit
                return false;
            }
        }
        if (query.size() < 1 || query.size() > 4){ // if less than 1 or greater than 4
            return false;
        }
    }else if (type == "revenue"){
        if (query[0] != '$'){ // Has $ sign
            return false;
        }
        if (query.find('.') == string::npos){ // If decimal point exists
            return false;
        }

        string left = query.substr(1, query.find(".") - 1); // gets characters left of decimal
        string right = query.substr(query.find(".") + 1, query.size()); // gets characters right of decimal

        if (right.size() != 2){ // Exactly 2 digits after decimal point
            return false;
        }
        if (left.size() < 1){ // Less than 1 digit between $ and .
            return false;
        }
        if (left.size() > 4){ // More than 4 digits between $ and .
            return false;
        }
    }
    return true;
}

void add(string linetext){ // Add function (input string of text that is batch line)
    ofstream outfile(database, ios::app); // Append to end of file
    
    // Find name by extracting string between quotes
    size_t firstquote = linetext.find("\"");
    string firstquotegone = linetext.substr(firstquote+1);
    size_t secondquote = firstquotegone.find("\"");
    string fullname = firstquotegone.substr(0,secondquote);   

    // Initializing variables for data extraction
    string data = firstquotegone.substr(secondquote + 2); // Get all characters right of second quote (adding 2 spaces for quote and space after)
    stringstream ss(data); // Stringstream to data to extract individual words
    int score, plays;
    string initials, revenue;
    string word;

    int progress = 1; // For loop
    bool valid = true; // Valid value, 1 if true, 0 if not valid

    while (ss >> word) // Loop to extract individual words
    {
        if (progress == 1){ // High score
            score = stoi(word);

        }else if (progress == 2){ // Initials
            initials = word;

        }else if (progress == 3){ // Plays
            plays = stoi(word);

        }else if (progress == 4){ // Revenue
            revenue = word;

        }
        progress++;
    }

    if (valid == true){ // If valid
        // Header output
        cout << "RECORD ADDED" << endl;

        // Output name
        cout << "Name: " << fullname << endl;
        outfile << fullname << ", ";

        // Output high score
        cout << "High Score: " << score << endl;
        outfile << score << ", ";

        // Output initials
        cout << "Initials: " << initials << endl;
        outfile << initials << ", ";

        // Revenue plays
        cout << "Plays: " << plays << endl;
        outfile << plays << ", ";

        // Output revenue
        cout << "Revenue: " << revenue << endl;

        string newrev = revenue;
        
        string firstpart = newrev.substr(1, newrev.find(".") - 1);
        string secondpart = newrev.substr(newrev.find(".") + 1, newrev.size());

        outfile << "$";
        outfile << firstpart << "." << secondpart << endl;

        cout << "\n";
    }

    outfile.close(); // Close outfile
}

void search(string linetext){ // Search function (input string of text that is batch line)
    ifstream infile(database, ios::binary);

    string searchquery = linetext.substr(2,linetext.size()); // Gets everything after command char and space
    string name = searchquery; // Temporary holding for search query
    string line, editline;
    
    transform(searchquery.begin(), searchquery.end(), searchquery.begin(), ::tolower); // Makes the search query lowercase for easier searching

    bool found = false; // To see if search query found

    if (infile){
        while (infile){
            getline(infile, line);
            size_t linecomma = line.find(","); // Finds first comma
            editline = line.substr(0,linecomma); // Gets line to be used
            transform(editline.begin(), editline.end(), editline.begin(), ::tolower); // Makes line lowercase for search

            if (editline.find(searchquery) != string::npos){ // See if search query is in line
                size_t commapos = line.find(","); // Find second comma
                int progress = 1; // Set string progress to 1
                
                string name = line.substr(0,commapos); // Get name
                string data = line.substr(commapos+2); // Get data

                cout << name << " FOUND" << endl;
                string word; // Binary
                found = true; // Found value is true now
                stringstream ss(data); // Convert data to string to extract individual words easier

                while (ss >> word) { // Loop through to collect all words
                    if (progress == 1){ // High score
                        word = word.erase(0, word.find_first_not_of('0'));
                        cout << "High Score: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 2){ // Initials
                        cout << "Initials: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 3){ // Plays
                        word = word.erase(0, word.find_first_not_of('0'));
                        cout << "Plays: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 4){ // Revenue
                        string firstpart = word.substr(1, word.find(".") - 1);
                        string secondpart = word.substr(word.find(".") + 1, word.size());

                        firstpart = firstpart.erase(0, firstpart.find_first_not_of('0'));
                        cout << "Revenue: $" << firstpart << "." << secondpart << endl;

                    }
                    progress++;
                }
                cout << endl; // Endl to make new line
            }
        }
    }

    if (found == false){ // If a value was not found
        cout << name << " NOT FOUND" << endl;
    }
}

void update(string linetext){ // Update function (input string of text that is batch line)
    ifstream dbfile(database, ios::binary);

    // Find name by extracting string between quotes
    size_t firstquote = linetext.find("\"");
    string firstquotegone = linetext.substr(firstquote+1);
    size_t secondquote = firstquotegone.find("\"");

    string fullname = firstquotegone.substr(0,secondquote);   

    // Initializing variables for data extraction
    string data = firstquotegone.substr(secondquote + 2); // Get all characters right of second quote (adding 2 spaces for quote and space after)
    stringstream ss(data); // Stringstream to data to extract individual words

    string word; // Word to store line from stringstream

    string toupdate, updatequery; // Get the item to update and the value to update to

    int progress = 1; // Initialize progress value to be used

    while (ss >> word) // Loop through stringstream to extract words
    {
        if (progress == 1){ // Update type
            toupdate = word;

        }else if (progress == 2){ // Update query
            updatequery = word;
        }

        progress++;
    }

    bool valid = true; // Input validation
    if (toupdate == "1"){ // High score
        if (validate("score",updatequery) == 0){
            valid = false;
        }
    }else if (toupdate == "2"){ // Initials
        if (validate("initials",updatequery) == 0){
            valid = false;
        }
    }else if (toupdate == "3"){ // Plays
        if (validate("plays",updatequery) == 0){
            valid = false;
        }
    }

    string line, tempstring;

    if (dbfile){
        bool updated = false; // Make update value and set to false, not updated yet

        while (dbfile.good()){
            getline(dbfile, line);
            if (line.find(fullname) != string::npos) { // If fullname was in line
                updated = true; // Updated value, to output later

                cout << fullname.substr(0, fullname.size()) << " UPDATED" << endl;
                cout << "UPDATE TO ";

                if (toupdate == "1"){ // High score
                    cout << "high score - VALUE " << updatequery << endl;
                }else if (toupdate == "2"){ // Initials
                    cout << "initials - VALUE " << updatequery << endl;
                }else if (toupdate == "3"){ // Plays
                    cout << "plays - VALUE " << updatequery << endl;
                }

                cout << "Name: " << fullname.substr(0, fullname.size()) << endl;

                progress = 1; // Set progress to 1 to use again
                word = ""; // Reusing word again
                
                string outputstring = fullname + ", ";

                size_t commapos = line.find(","); // Find second comma
                string newdata = line.substr(commapos+2); // Get data

                stringstream linestring(newdata); // Put line in stringstream to extract words
                double convertedrevenue = 0;

                while (linestring >> word) { // Loop through stringstream
                    if (progress == 1){ // High score
                        if (toupdate == "1"){
                            stringstream highscore;
                            highscore << updatequery;
                            word = highscore.str();

                            outputstring = outputstring + word + ", ";
                            cout << "High Score: " << updatequery << endl;
                        }else{
                            outputstring = outputstring + word + " ";

                            word = word.erase(0, word.find_first_not_of('0'));
                            cout << "High Score: " << word.substr(0, word.size()-1) << endl;
                        }

                    }else if (progress == 2){ // Initials
                        if (toupdate == "2"){
                            outputstring = outputstring + updatequery + ", ";
                            cout << "Initials: " << updatequery << endl;
                        }else{
                            outputstring = outputstring + word + " ";
                            cout << "Initials: " << word.substr(0, word.size()-1) << endl;
                        }                  

                    }else if (progress == 3){ // Plays
                        if (toupdate == "3"){
                            convertedrevenue  = stoi(updatequery); // Get integer of plays to recalculate revenue
                            convertedrevenue = convertedrevenue * 0.25; // Get the money from plays

                            cout << "Plays: " << updatequery << endl;

                            stringstream stream; // Get stringstream to convert integer to string
                            stream << stoi(updatequery);
                            string s = stream.str(); // Convert the stringstream to string

                            outputstring = outputstring + s + ", ";
                        }else{
                            outputstring = outputstring + word + " ";
                            word = word.erase(0, word.find_first_not_of('0'));
                            cout << "Plays: " << word.substr(0, word.size()-1) << endl;
                        }

                    }else if (progress == 4){ // Revenue
                        if (convertedrevenue != 0){
                            stringstream stream; // Get stringstream to convert integer to string
                            stream << convertedrevenue;
                            string s = stream.str(); // Convert the stringstream to string

                            outputstring = outputstring + "$" + s;
                            cout << "Revenue: $" << convertedrevenue << endl;
                        }else{
                            outputstring = outputstring + word;
                            string firstpart = word.substr(1, word.find(".") - 1);
                            string secondpart = word.substr(word.find(".") + 1, word.size());

                            firstpart = firstpart.erase(0, firstpart.find_first_not_of('0'));

                            cout << "Revenue: $" << firstpart << "." << secondpart << endl;
                        }

                        
                    }
                    progress++;
                }
                cout << endl;
                tempstring = tempstring + outputstring + "\n"; // Put the line string into tempstring
            }else{
                tempstring = tempstring + line + "\n"; // Cycle to next line
            }
        }

        dbfile.close();

        if (updated == true && valid == true){ // If file updated and everything is valid
            ofstream outfile(database, ios::binary);
            outfile << tempstring;

            outfile.close();
        }
        
    }
}

/*void remove(string linetext){ // Remove function (input string of text that is batch line)
    ifstream dbfile(database, ios::binary);

    string searchquery = linetext.substr(2,linetext.size()); // Get the search query
    string line;

    if (dbfile){
        string tempstring;
        while (dbfile.good()){
            getline(dbfile, line);
            size_t linecomma = line.find(","); // Find line comma to get name
            string linename = line.substr(0,linecomma); // Get the name of the line

            if (linename == searchquery) { // If direct match -- this will not copy the line for the overwrite (not copying to temp string)
                int progress = 1; // Set loop variable
                string name = line.substr(0,linecomma); // Get name of item
                string data = line.substr(linecomma+2); // Get data for console output

                string word; // Word for storing loop string
                stringstream ss(data); // Setup data for stringstream string

                cout << "RECORD DELETED" << endl;
                cout << "Name: " << name << endl;

                while (ss >> word) { // Loop through stringstream and extract words
                    if (progress == 1){ // High score
                        word = word.erase(0, word.find_first_not_of('0'));
                        cout << "High Score: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 2){ // Initials
                        cout << "Initials: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 3){ // Plays
                        word = word.erase(0, word.find_first_not_of('0'));
                        cout << "Plays: " << word.substr(0, word.size()-1) << endl;

                    }else if (progress == 4){ // Revenue
                        string firstpart = word.substr(1, word.find(".") - 1);
                        string secondpart = word.substr(word.find(".") + 1, word.size());

                        firstpart = firstpart.erase(0, firstpart.find_first_not_of('0'));
                        cout << "Revenue: $" << firstpart << "." << secondpart << endl;

                    }
                    progress++;
                }
                cout << endl;
            }else{
                tempstring = tempstring + line + "\n"; // Retain line for overwrite, not the line fit for removal
            }
        }
        
        dbfile.close();

        ofstream outfile(database, ios::binary); // Get output file ready for output, overwrite everything
        outfile << tempstring; // Outputting all lines (string includes every line but removal line)
        outfile.close();
    }
}*/