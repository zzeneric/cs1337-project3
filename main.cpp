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

struct Node {
    string name;
    int score;
    string initials;
    int plays;
    double revenue;

    Node *next;
};

// Function prototypes
bool validate(string type, string query);
void add(string linetext, Node *&head);
void search(string linetext, Node *&head);
void update(string linetext, Node *&head);
void remove(string linetext, Node *&head);

void extract(string line, Node *&head){
    stringstream ss(line); // Stringstream to data to extract individual words

    int score, plays;
    string name, initials;
    double revenue;

    string newdata;
    size_t commaPos;

    commaPos = line.find(","); // Find second comma
    name = line.substr(0,commaPos);
    newdata = line.substr(commaPos+2); // Get data

    commaPos = newdata.find(",");
    score = stoi(newdata.substr(0,commaPos));
    newdata = newdata.substr(commaPos+2); // Get data

    commaPos = newdata.find(",");
    initials = newdata.substr(0,commaPos);
    newdata = newdata.substr(commaPos+2); // Get data

    commaPos = newdata.find(",");
    plays = stoi(newdata.substr(0,commaPos));
    newdata = newdata.substr(commaPos+2); // Get data

    commaPos = newdata.find(",");
    revenue = stod(newdata.substr(1,commaPos));
    newdata = newdata.substr(commaPos+2); // Get data


    Node *newNode = new Node; // Creates node
    newNode->next = NULL; // Temporarily assigns node no next value

    newNode->name = name;
    newNode->score = score;
    newNode->initials = initials;
    newNode->plays = plays;
    newNode->revenue = revenue;

    if(head == NULL){ // If previous node doesn't exist
        head = newNode;
    }else{ // If header node exists
        Node *temp = head;
        while(temp->next != NULL){ 
            temp = temp->next;
        }
        temp->next = newNode; // Puts node all the way in the back
    }
    
    /*cout << name << " ";
    cout << score << " ";
    cout << initials << " ";
    cout << plays << " ";
    cout << revenue << endl;*/
}

void ingest(string temp, Node *&head){
    ifstream infile(temp, ios::binary);
    string line;
    if (infile)
        while (infile.good())
        {
            getline(infile, line);
            if (line != "")
                extract(line,head);
        }

    infile.close();
}

void printList(Node *head){
    for ( ; head; head = head->next){
        cout << head->name << " ";
        cout << head->score << " ";
        cout << head->initials << " ";
        cout << head->plays << " ";
        cout << head->revenue << endl;
    }
}

void outputList(Node *head){
    for ( ; head; head = head->next){
        cout << head->name << " ";
        cout << head->score << " ";
        cout << head->initials << " ";
        cout << head->plays << " ";
        cout << head->revenue << endl;
    }

    fstream ourfile;
    string ourline;

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
}

int main(){ // Main function
/* ################  DO NOT MODIFY CODE IN THIS BLOCK ###################### */
    string temp;  //variable for database filename
    string batch; //variable for batch filename

    Node *head = NULL;

    //cout<<"Enter Database Name: ";
    //cin>>temp;
    temp = "sampledb.dat";
    
    ingest(temp,head);

    //cout << "Enter Batch Name: "; // Asks for batch file name
    //cin >> batch;
    batch = "batch2.txt";

    cout << endl; // Add new line

    fstream ourfile;
    ourfile.open(batch, ios::in);

    string ourline;

    if (ourfile){
        while (ourfile.good()){
            getline(ourfile, ourline); // Gets line from databasefile
            if (ourline != ""){
                if (ourline[0] == '1'){ // If command char is add
                    add(ourline,head);
                }else if (ourline[0] == '2'){ // If command char is search
                    //search(ourline,head);
                }else if (ourline[0] == '3'){ // If command char is update
                    update(ourline,head);
                }else if (ourline[0] == '4'){ // If command char is remove
                    //remove(ourline);
                }
            }
        }
    }else{
        cout << "Invalid input" << endl; // If none of the commands are fit
    }

    ourfile.close(); // Closes file
    
    //printList(head);

    //outputList(Node *head);
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

void add(string linetext, Node *&head){ // Add function (input string of text that is batch line)
    Node *newNode = new Node; // Creates node
    newNode->next = NULL; // Temporarily assigns node no next value
    
    if(head == NULL){ // If previous node doesn't exist
        head = newNode;
    }else{ // If header node exists
        Node *temp = head;
        while(temp->next != NULL){ 
            temp = temp->next;
        }
        temp->next = newNode; // Puts node all the way in the back
    }

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
        newNode->name = fullname;

        // Output high score
        cout << "High Score: " << score << endl;
        newNode->score = score;

        // Output initials
        cout << "Initials: " << initials << endl;
        newNode->initials = initials;

        // Revenue plays
        cout << "Plays: " << plays << endl;
        newNode->plays = plays;

        // Output revenue
        cout << "Revenue: " << revenue << endl;
        
        string firstpart = revenue.substr(1, revenue.find(".") - 1);
        string secondpart = revenue.substr(revenue.find(".") + 1, revenue.size());
        double newrev = stod(firstpart + "." + secondpart);

        newNode->revenue = newrev;

        cout << "\n";
    }

}

void search(string linetext, Node *&head){ // Search function (input string of text that is batch line)
    string searchQuery = linetext.substr(2,linetext.size()); // Gets everything after command char and space
    string storedName = searchQuery; // Temporary holding for search query
    string name;
    
    transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower); // Makes the search query lowercase for easier searching

    bool found = false; // To see if search query found

    for ( ; head; head = head->next){
        name = head->name;
        transform(name.begin(), name.end(), name.begin(), ::tolower); // Makes line lowercase for search

        if (name.find(searchQuery) != string::npos){ // See if search query is in line
            cout << head->name << " FOUND" << endl;
            found = true; // Found value is true now
            
            cout << "High Score: " << head->score << endl;
            cout << "Initials: " << head->initials << endl;
            cout << "Plays: " << head->plays << endl;
            cout << "Revenue: $" << head->revenue << endl;

            cout << endl; // Endl to make new line
        }
    }

    if (found == false){ // If a value was not found
        cout << storedName << " NOT FOUND" << endl;
    }
}

void update(string linetext, Node *&head){ // Update function (input string of text that is batch line)
    // Find name by extracting string between quotes
    size_t firstquote = linetext.find("\"");
    string firstquotegone = linetext.substr(firstquote+1);
    size_t secondquote = firstquotegone.find("\"");
    string fullname = firstquotegone.substr(0,secondquote);   

    // Initializing variables for data extraction
    string data = firstquotegone.substr(secondquote + 2); // Get all characters right of second quote (adding 2 spaces for quote and space after)
    stringstream ss(data); // Stringstream to data to extract individual words

    string word; // Word to store line from stringstream

    string toUpdate, updateQuery; // Get the item to update and the value to update to

    int progress = 1; // Initialize progress value to be used

    while (ss >> word) // Loop through stringstream to extract words
    {
        if (progress == 1){ // Update type
            toUpdate = word;

        }else if (progress == 2){ // Update query
            updateQuery = word;
        }

        progress++;
    }

    bool valid = true; // Input validation
    if (toUpdate == "1"){ // High score
        if (validate("score",updateQuery) == 0){
            valid = false;
        }
    }else if (toUpdate == "2"){ // Initials
        if (validate("initials",updateQuery) == 0){
            valid = false;
        }
    }else if (toUpdate == "3"){ // Plays
        if (validate("plays",updateQuery) == 0){
            valid = false;
        }
    }

    string line;

    bool updated = false;

    for ( ; head; head = head->next){
        line = head->name;

        if (line.find(fullname) != string::npos){ // If fullname was in line
            updated = true; // Updated value, to output later

            cout << line << " UPDATED" << endl;
            cout << "UPDATE TO ";

            if (toUpdate == "1"){ // High score
                head->score = stoi(updateQuery);
                cout << "high score - VALUE " << head->score << endl;
            }else if (toUpdate == "2"){ // Initials
                head->initials = updateQuery;
                cout << "initials - VALUE " << head->initials << endl;
            }else if (toUpdate == "3"){ // Plays
                head->plays = stoi(updateQuery);
                cout << "plays - VALUE " << head->plays << endl;
            }

            cout << "Name: " << head->name << endl;
            cout << "High Score: " << head->score << endl;
            cout << "Initials: " << head->initials << endl;
            cout << "Plays: " << head->plays << endl;
            cout << "Revenue: $" << head->revenue << endl;
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