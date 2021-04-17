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

struct Node{
    string name;
    int score;
    string initials;
    int plays;
    double revenue;

    Node *next;
};

// Function prototypes
void add(string linetext, Node *&head);
void search(string linetext, Node *&head);
void update(string linetext, Node *&head);
void remove(string linetext, Node *&head);
void sort(string linetext, Node *&head);

// Vanity functions
void ingest(string temp, Node *&head);
void printList(Node *head);
void outputList(Node *head);

int main(){ // Main function
    string temp;  //variable for database filename
    string batch; //variable for batch filename

    Node *head = NULL;

    cout<<"Enter Database Name: ";
    cin >> temp;
    //temp = "sampledb.dat";
    
    ingest(temp,head);

    cout << "Enter Batch Name: "; // Asks for batch file name
    cin >> batch;
    cout << endl;
    //batch = "batch2.txt";

    fstream ourfile;
    ourfile.open(batch, ios::in);

    string ourline;

    if (ourfile){
        while(getline(ourfile,ourline)){
            if (ourline != ""){
                if (ourline[0] == '1'){ // If command char is add
                    add(ourline,head);
                }else if (ourline[0] == '2'){ // If command char is search
                    search(ourline,head);
                }else if (ourline[0] == '3'){ // If command char is update
                    update(ourline,head);
                }else if (ourline[0] == '4'){ // If command char is remove
                    remove(ourline, head);
                }else if (ourline[0] == '5'){ // If command char is sort
                    sort(ourline, head);
                }
                cout << "\n\n"; // Add new line
            }
        }
    }

    ourfile.close(); // Closes file
    
    //printList(head);

    outputList(head);
    return 0;
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
        
    string firstpart = revenue.substr(1, revenue.find(".") - 1);
    string secondpart = revenue.substr(revenue.find(".") + 1, revenue.size());
    double newrev = stod(firstpart + "." + secondpart);

    newNode->revenue = newrev;

    cout << "Revenue: " << fixed << setprecision(2) << revenue << endl;

    cout << "\n";
}

void search(string linetext, Node *&head){ // Search function (input string of text that is batch line)
    string searchQuery = linetext.substr(2,linetext.size()); // Gets everything after command char and space
    string storedName = searchQuery; // Temporary holding for search query
    string name;
    
    transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower); // Makes the search query lowercase for easier searching

    bool found = false; // To see if search query found

    Node *temp = head;

    for ( ; head; head = head->next){
        name = head->name;
        transform(name.begin(), name.end(), name.begin(), ::tolower); // Makes line lowercase for search

        if (name.find(searchQuery) != string::npos){ // See if search query is in line
            cout << head->name << " FOUND" << endl;
            found = true; // Found value is true now
            
            cout << "High Score: " << head->score << endl;
            cout << "Initials: " << head->initials << endl;
            cout << "Plays: " << head->plays << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << head->revenue << endl;

            cout << endl; // Endl to make new line
        }
    }
    head = temp;

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

    while (ss >> word){ // Loop through stringstream to extract words
        if (progress == 1){ // Update type
            toUpdate = word;
        }else if (progress == 2){ // Update query
            updateQuery = word;
        }
        progress++;
    }

    string name;

    Node *current;
	current = head;

    for ( ; head; head = head->next){ // Loop through from head to end
        name = head->name;

        if (name.find(fullname) != string::npos){ // If fullname was in line
            cout << name << " UPDATED" << endl;
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
            cout << "Revenue: $" << fixed << setprecision(2) << head->revenue << endl;
        }
    }
    head = current;
    //delete temp;
}

void functionDelete(Node *head, Node *n){ // Specific delete function
    if(head == n){ // If head is the node to be deleted
        head->name = head->next->name; 
        head->score = head->next->score; 
        head->initials = head->next->initials; 
        head->plays = head->next->plays; 
        head->revenue = head->next->revenue; 

        n = head->next; 
  
        head->next = head->next->next; 
        return; 
    } 
  
    Node *prev = head; // Temporary node
    while(prev->next != NULL && prev->next != n){
        prev = prev->next; 
    }

    if(prev->next == NULL){ 
        return;
    }else{
        prev->next = prev->next->next; 
    }
}

void remove(string linetext, Node *&head){ // Remove function (input string of text that is batch line)
    string searchquery = linetext.substr(2,linetext.size()); // Get the search query
    string line;
    string name;

    Node *current;
	current = head;

	while (head != NULL){ // If head isn't null
        name = head->name;
        if (name == searchquery) {
            cout << "RECORD DELETED" << endl;
            cout << "Name: " << name << endl;
            cout << "High Score: " << head->score << endl;
            cout << "Initials: " << head->initials << endl;
            cout << "Plays: " << head->plays << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << head->revenue << endl;

            functionDelete(current,head);
        }
		head = head->next;
    }
    head = current; // Put head back to the top
}

void sort(string linetext, Node *&head){ // Selection sort data switching
    string searchQuery = linetext.substr(2,linetext.size());
    Node* temp = head;
  
    while (temp){
        Node* min = temp;
        Node* r = temp->next;
  
        while (r) {
            if(searchQuery == "name"){
                if (min->name > r->name){
                    min = r;
                }
            }else if (searchQuery == "plays"){
                if (min->plays > r->plays){
                    min = r;
                }
            }
            

            r = r->next;
        }
        // Data switching
        string tempName = min->name;
        int tempScore = min->score;
        string tempInitials = min->initials;
        int tempPlays = min->plays;
        double tempRevenue = min->revenue;

        min->name = temp->name;
        min->score = temp->score;
        min->initials = temp->initials;
        min->plays = temp->plays;
        min->revenue = temp->revenue;

        temp->name = tempName;
        temp->score = tempScore;
        temp->initials = tempInitials;
        temp->plays = tempPlays;
        temp->revenue = tempRevenue;

        temp = temp->next;
    }
    cout << "RECORDS SORTED BY " << searchQuery << endl;

    Node * current;
	current = head;
	while (current != nullptr){
        cout << current->name << ", ";
        cout << current->score << ", ";
        cout << current->initials << ", ";
        cout << current->plays << ", $";
        cout << current->revenue << endl;
		current = current->next;
	}
}

// Vanity functions

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
}

void ingest(string temp, Node *&head){ // Ingests each line for extraction
    ifstream infile(temp, ios::binary);
    string line;
    if (infile)
        while (infile.good())
        {
            getline(infile, line); // Gets line
            if (line != "")
                extract(line,head); // Extracts data out of each line
        }

    infile.close();
}

void printList(Node *head){ // Prints all the data from node header
    Node * current;
	current = head;
	while (current != nullptr){
        cout << current->name << " ";
        cout << current->score << " ";
        cout << current->initials << " ";
        cout << current->plays << " ";
        cout << current->revenue << endl;
		current = current->next;
	}
}

void outputList(Node *head){ // Outputs the linked list to database file
    string tempstring;

    Node * current;
	current = head;

    for ( ; current; current = current->next){ // Loop through nodes
        ostringstream oss;
        oss << fixed << setprecision(2) << current->revenue;
        string str = oss.str();

        string format = current->name + ", " + to_string(current->score) + ", " + current->initials + ", " + to_string(current->plays) +  ", $" + str;
        tempstring = tempstring + format + "\n";
    }

    fstream ourfile;
    
    // Removing all spare blank lines
    ourfile.open(database, ios::out);

    ourfile << tempstring;

    ourfile.close(); // Closes file
}