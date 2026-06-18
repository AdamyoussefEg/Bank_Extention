#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct StUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enManageUsersMenueOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
    eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eExit = 8
};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, 
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

StUser Current_User;
string UsersFileName = "Users.txt";
const string ClientsFileName = "Clients.txt";

vector<StUser> LoadUsersDataFromFile(string FileName);
void login();
void ShowMainMenue();
void ShowTransactionsMenue();
void ShowMangersUsersMenue();
void ShowAddNewUserScreen();

void AccessDenied(){
    cout << "-----------------------------------\n";
    cout << "\tAccess Denied!";
    cout << "\nyou do not have permission to Do this,";
    cout << "\nPlease contact your Admin";
    cout << "\n-----------------------------------\n\n";
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);
    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}



void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
    if (Current_User.Permissions & enMainMenuePermissions::pListClients || Current_User.Permissions == enMainMenuePermissions::eAll) {
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
        cout << "\n_______________________________________________________________________\n" << endl;
        cout << "| " << left << setw(15) << "Accout Number";
        cout << "| " << left << setw(10) << "Pin Code";
        cout << "| " << left << setw(40) << "Client Name";
        cout << "| " << left << setw(12) << "Phone";
        cout << "| " << left << setw(12) << "Balance";
        cout << "\n_______________________________________________________________________\n" << endl;
        if (vClients.size() == 0)
            cout << "\t\t\t\tNo Clients Available In the System!";
        else
            for (sClient Client : vClients)
            {
                PrintClientRecordLine(Client);
                cout << endl;
            }
        cout << "\n_______________________________________________________________________\n" << endl;

    } else {
        AccessDenied();
    }
}

void ShowTotalBalances()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________________________\n" << endl;
    double TotalBalances = 0;
    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
    cout << "\n_______________________________________________________________________\n" << endl;
    cout << "\t\t\t\t\t Total Balances = " << TotalBalances;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number  : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New Client:\n";
        AddDataLineToFile(ClientsFileName, ConvertRecordToLine(ReadNewClient()));
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (AddMore == 'Y' || AddMore == 'y');
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.MarkForDelete = true;
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\nClient Deleted Successfully.";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\nClient Updated Successfully.";
            return true;
        }
    }
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
    return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteClientScreen()
{
    if (Current_User.Permissions & enMainMenuePermissions::pDeleteClient || Current_User.Permissions == enMainMenuePermissions::eAll) {
        cout << "\n-----------------------------------\n";
        cout << "\tDelete Client Screen";
        cout << "\n-----------------------------------\n";
        
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        string AccountNumber = ReadClientAccountNumber();
        DeleteClientByAccountNumber(AccountNumber, vClients);
    } else {
        AccessDenied();
    }
}

void ShowUpdateClientScreen()
{
    if (Current_User.Permissions & enMainMenuePermissions::pUpdateClients || Current_User.Permissions == enMainMenuePermissions::eAll) {
        cout << "\n-----------------------------------\n";
        cout << "\tUpdate Client Info Screen";
        cout << "\n-----------------------------------\n";
        
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        string AccountNumber = ReadClientAccountNumber();
        UpdateClientByAccountNumber(AccountNumber, vClients);
    } else {
        AccessDenied();
    }
}

void ShowAddNewClientsScreen()
{
    if (Current_User.Permissions & enMainMenuePermissions::pAddNewClient || Current_User.Permissions == enMainMenuePermissions::eAll) {
        cout << "\n-----------------------------------\n";
        cout << "\tAdd New Clients Screen";
        cout << "\n-----------------------------------\n";
        
        AddNewClients();
    } else {
        AccessDenied();
    }
}

void ShowFindClientScreen()
{
    
    if (Current_User.Permissions & enMainMenuePermissions::pFindClient || Current_User.Permissions == enMainMenuePermissions::eAll) {
        cout << "\n-----------------------------------\n";
        cout << "\tFind Client Screen";
        cout << "\n-----------------------------------\n";
        
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
            PrintClientCard(Client);
        else
            cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
    } else {
        AccessDenied();
    }
    
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }
    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}



void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::eWithdraw:
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::eShowMainMenue:
        ShowMainMenue();
        break;
    }
}

void ShowTransactionsMenue()
{
    if (Current_User.Permissions & enMainMenuePermissions::pTranactions || Current_User.Permissions == enMainMenuePermissions::eAll) {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tTransactions Menue Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] Deposit.\n";
        cout << "\t[2] Withdraw.\n";
        cout << "\t[3] Total Balances.\n";
        cout << "\t[4] Main Menue.\n";
        cout << "===========================================\n";
        PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
    } else {
        AccessDenied();
    }
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}



string ConvertUserRecordToLine(StUser Client, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += Client.UserName + Seperator;
    stUserRecord += Client.Password + Seperator;
    stUserRecord += to_string(Client.Permissions);
    return stUserRecord;
}

void showUserScreen(){
    vector<StUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________________________\n" << endl;
    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else
        for (StUser User : vUsers)
        {
            cout << "| " << setw(15) << left << User.UserName;
            cout << "| " << setw(10) << left << User.Password;
            cout << "| " << setw(40) << left << User.Permissions;
            cout << endl;
        }
    cout << "\n_______________________________________________________________________\n" << endl;
}

StUser ReadNewUser()
{
    StUser User;
    string userInput;

    cout << "Adding New User:\n";

    cout << "\nEnter User Name? ";
    getline(cin >> ws, User.UserName);
    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = 0;

    cout << "\nDo you want to give this user all permissions? (y/n)\n";
    cin >> userInput;
    if (userInput == "y" || userInput == "Y") 
    {
        User.Permissions = enMainMenuePermissions::eAll;
    }
    else 
    {
        cout << "\nDo you want to give this user the permission to List Clients? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pListClients;

        cout << "\nDo you want to give this user the permission to Add New Client? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pAddNewClient;

        cout << "\nDo you want to give this user the permission to Delete Client? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pDeleteClient;

        cout << "\nDo you want to give this user the permission to Update Client Info? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pUpdateClients;

        cout << "\nDo you want to give this user the permission to Find Client? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pFindClient;

        cout << "\nDo you want to give this user the permission to Transactions? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pTranactions;

        cout << "\nDo you want to give this user the permission to Manage Users? (y/n)\n";
        cin >> userInput;
        if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pManageUsers;
    }

    return User;
}
void TakeUserPermissionsInput(){
        string Addmore = "y";

        do {
            AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(ReadNewUser()));
            cout << "\nUser Add Successfully, do you want to add another user? (y/n)\n";
            cin >> Addmore;
        } while (Addmore == "y" || Addmore == "Y");
}

void DeleteUserScreen(){
    string UserName;
    vector<StUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    bool userFound = false;

    cout << "\nPlease enter the User Name to delete: ";
    cin >> UserName;

    for (StUser& User : vUsers)
    {
        if (User.UserName == UserName) {
            userFound = true;
            char Answer = 'n';
            cout << "\nAre you sure you want delete this user? y/n ? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y')
            {
                User.MarkForDelete = true;
                fstream MyFile;
                MyFile.open(UsersFileName, ios::out);
                if (MyFile.is_open())
                {
                    for (StUser U : vUsers)
                    {
                        if (!U.MarkForDelete)
                            MyFile << ConvertUserRecordToLine(U) << endl;
                    }
                    MyFile.close();
                }
                cout << "\nUser Deleted Successfully.";
            }
        }
    }

    if (!userFound)
    {
        cout << "\nUser with User Name [" << UserName << "] is not found!";
    }
}

void UpdateUserPermissionsInput(){
    string UserName;
    cout << "\nPlease Enter the User Name: ";
    cin >> UserName;

    vector<StUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out);
    if (MyFile.is_open())
    {
        for (StUser U : vUsers)
        {
            if (!U.MarkForDelete)
                MyFile << ConvertUserRecordToLine(U) << endl;
                break;
        }
        MyFile.close();
    }
    cout << "\nUser Permissions Updated Successfully.";

    bool userFound = false;

    for (StUser& User : vUsers) {
        if (User.UserName == UserName) {
            userFound = true;
            cout << "\nUpdating permissions for user: " << User.UserName << "\n";
            User.Permissions = 0;

            string userInput;
            cout << "\nDo you want to give this user all permissions? (y/n)\n";
            cin >> userInput;
            if (userInput == "y" || userInput == "Y") {
                User.Permissions = enMainMenuePermissions::eAll;
            } else {
                cout << "\nDo you want to give this user the permission to List Clients? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pListClients;

                cout << "\nDo you want to give this user the permission to Add New Client? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pAddNewClient;

                cout << "\nDo you want to give this user the permission to Delete Client? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pDeleteClient;

                cout << "\nDo you want to give this user the permission to Update Client Info? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pUpdateClients;

                cout << "\nDo you want to give this user the permission to Find Client? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pFindClient;

                cout << "\nDo you want to give this user the permission to Transactions? (y/n)\n";
                cin >> userInput;
                if (userInput == "y" || userInput == "Y") User.Permissions += enMainMenuePermissions::pTranactions;
            }
        }
    }
    if (!userFound) {
        cout << "\nUser with User Name [" << UserName << "] is not found!";
    }
}

void FindUserScreen(){
    string UserName;
    vector<StUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    bool userFound = false;
    
    cout << "\nPlease enter the User Name to find: ";
    cin >> UserName;

    cout << "\nThe following are the user details:\n";
    for (StUser User : vUsers)
    {
        if (User.UserName == UserName) {
            userFound = true;
            cout << "-----------------------------------";
            cout << "\nUser Name  : " << User.UserName;
            cout << "\nPassword   : " << User.Password;
            cout << "\nPermissions: " << User.Permissions;
            cout << "\n-----------------------------------\n";
        }
            
    }

    if (!userFound)
    {
        cout << "\nUser with User Name [" << UserName << "] is not found!";
    }
}



void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowMangersUsersMenue();
}

void ShowAllUsersScreen()
{
    showUserScreen();
}

void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    TakeUserPermissionsInput();
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    DeleteUserScreen();
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen";
    cout << "\n-----------------------------------\n";

    UpdateUserPermissionsInput();
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    FindUserScreen();
}

void PerfromMangersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption)
{
    switch (ManageUsersMenueOption)
    {
    case enManageUsersMenueOptions::eListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eMainMenue:
        ShowMainMenue();
        break;
    }
}

void ShowMangersUsersMenue()
{
    if (Current_User.Permissions & enMainMenuePermissions::pManageUsers || Current_User.Permissions == enMainMenuePermissions::eAll) {
        system("cls");
        int Choice;
        cout << "===========================================\n";
        cout << "\t\tManage Users Menue Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] List Users.\n";
        cout << "\t[2] Add New User.\n";
        cout << "\t[3] Delete User.\n";
        cout << "\t[4] Update User Info.\n";
        cout << "\t[5] Find User.\n";
        cout << "\t[6] Main Menue.\n";
        cout << "===========================================\n";
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> Choice;
        PerfromMangersMenueOption(enManageUsersMenueOptions(Choice));
    } else {
        AccessDenied();
    }
}



void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowMangersUsersMenue();
        break;
    case enMainMenueOptions::eExit:
        system("cls");
        // ShowEndScreen();
        login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Exit.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}



StUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    StUser User;
    vector<string> vUserData;
    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);
    return User;
}

vector <StUser> LoadUsersDataFromFile(string FileName)
{
    vector<StUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        StUser User;
        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUserNameAndPassword(string UserName, string Password, StUser& User)
{
    vector<StUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (StUser U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string UserName, string Password){
    if(FindUserByUserNameAndPassword(UserName, Password, Current_User)){
        return true;
    }
    else{
        return false;
    }
}

void login(){
    bool loginFailed = false;

    string userName, Password;
    do{
        cout << "\n----------------------\n";
        cout << "\tLogin Screen";
        cout << "\n----------------------\n";
        if (loginFailed){
            cout << "Invalid UserName/Password\n";}
        cout << "Enter UserName: ";
        cin >> userName;

        cout << "Enter Password: ";
        cin >> Password;

        loginFailed = !LoadUserInfo(userName, Password);
    } while(loginFailed);

    ShowMainMenue();
}

int main()
{
    login();
    system("pause>0");
    return 0;
}