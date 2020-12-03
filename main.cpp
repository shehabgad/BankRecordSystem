#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <windows.h>
using namespace std;
class BankRecord
{
    static fstream BankRecordFile;
private:
    int account_number;
    char first_name[30];
    char last_name[30];
    double balance;
public:
    BankRecord() {}
    void get_record()
    {
        cout << "Enter Account Number: "; cin >> account_number;
        cout << "Enter First Name: "; cin >> first_name;
        cout << "Enter Last Name: "; cin >> last_name;
        cout << "Enter Balance: "; cin >> balance;
    }
    void display_record()
    {
        cout << "Account Number: " << account_number;
        cout << "\nFirst name: " << first_name;
        cout << "\nLast name: " << last_name;
        cout << "\nBalance: " << balance;
        cout << "\n--------------------------------------" << endl;
    }
    static void add_record()
    {
        BankRecordFile.clear();
        BankRecord temp;
        temp.get_record();
        BankRecordFile.write(reinterpret_cast<char*>(&temp),sizeof(BankRecord));
        if(!BankRecordFile)
            cout << "Error opening the file\n";
    }
    static void show_data()
    {
        BankRecordFile.clear();
        BankRecordFile.seekg(0);
        BankRecord temp;
        while(!BankRecordFile.eof())
        {
            BankRecordFile.read(reinterpret_cast<char*>(&temp),sizeof(BankRecord));
            if(!BankRecordFile)
                break;
            temp.display_record(); 
        }
    }
    static void search_record()
    {
        BankRecordFile.clear();
        int number;
        BankRecordFile.seekg(0,fstream::end);
        int numberOfBytes = BankRecordFile.tellg();
        int total_Records = numberOfBytes / sizeof(BankRecord);
        cout << "Total number of records: " << total_Records << endl;
        cout << "Enter record number to be searched: "; cin >> number;
        BankRecordFile.seekg(0,fstream::beg);
        bool flag = true;
        while(!BankRecordFile.eof())
        {
          BankRecord temp;
          BankRecordFile.read(reinterpret_cast<char*>(&temp),sizeof(BankRecord));
          if(!BankRecordFile)
              break;
          if(temp.account_number == number)
          {
              temp.display_record();
              flag = false;
          }
        }
        if(flag)
            cout << "Error! Record is not found\n";
    }
    static void edit_record()
    {
        BankRecordFile.clear();
        int number;
        BankRecordFile.seekg(0,fstream::end);
        int numberOfBytes = BankRecordFile.tellg();
        int total_Records = numberOfBytes / sizeof(BankRecord);
        cout << "Total number of records: " << total_Records << endl;
        cout << "Enter record number to be edited: "; cin >> number;
        BankRecordFile.seekg(0,fstream::beg);
        bool flag = true;
        BankRecord temp;
        while(!BankRecordFile.eof())
        {
          BankRecordFile.read(reinterpret_cast<char*>(&temp),sizeof(BankRecord));
          if(!BankRecordFile)
              break;
          if(temp.account_number == number)
          {
              temp.display_record();
              flag = false;
              break;
          }
        }
        if(flag)
            cout << "Error! Record is not found\n";
        else
        {
            BankRecordFile.seekg(-72,fstream::cur);
            int pos = BankRecordFile.tellg();
            cout << "Enter the data to be modified: \n";
            temp.get_record();
            ofstream tempfile{"tempfile.DAT",ios::binary};
            BankRecordFile.seekg(0,fstream::beg);
            while(!BankRecordFile.eof())
            {
             BankRecord temp1;
             BankRecordFile.read(reinterpret_cast<char*>(&temp1), sizeof(BankRecord));
             if(!BankRecordFile)
                break;
             if(tempfile.tellp() == pos)
             {
                tempfile.write(reinterpret_cast<char*>(&temp), sizeof(BankRecord));
             }
             else
                 tempfile.write(reinterpret_cast<char*>(&temp1), sizeof(BankRecord));
             
            }
            tempfile.close();
            BankRecordFile.close();
            remove("BankRecordFile.DAT");
            rename("tempfile.DAT", "BankRecordFile.DAT");
            BankRecordFile.open("BankRecordFile.DAT",ios::in | ios::out | ios::app | ios::binary);
            
        }
    }
    static void delete_record()
    {
        BankRecord::show_data();
        BankRecordFile.clear();
        int number;
        cout << "Enter record number to be deleted: "; cin >> number;
        ofstream tempfile{"tempfile.DAT",ios::binary};
        bool flag = true;
        BankRecordFile.seekg(0);
        while(!BankRecordFile.eof())
        {
            BankRecord temp;
            BankRecordFile.read(reinterpret_cast<char*>(&temp), sizeof(BankRecord));
            if(!BankRecordFile)
                break;
            if(temp.account_number != number)
            {
                tempfile.write(reinterpret_cast<char*>(&temp), sizeof(BankRecord));
                
            }
            else
                flag = false;
        }
        if(flag)
        {
            cout << "Error! record is not found\n";
            tempfile.close();
            BankRecordFile.close();
            remove("BankRecordFile.DAT");
            rename("tempfile.DAT", "BankRecordFile.DAT");
            BankRecordFile.open("BankRecordFile.DAT",ios::in | ios::out | ios::app | ios::binary);
        }
        else
        {
            tempfile.close();
            BankRecordFile.close();
            remove("BankRecordFile.DAT");
            rename("tempfile.DAT", "BankRecordFile.DAT");
            BankRecordFile.open("BankRecordFile.DAT",ios::in | ios::out | ios::app | ios::binary);
        }
    }
};
fstream BankRecord::BankRecordFile {"BankRecordFile.DAT", ios::in | ios::out | ios::trunc | ios::binary};
int main()
{
    cout << "--Account Information System--" << endl;
    int choice;
    do
    {
      cout << "Select one option below" << endl;
      cout << "\t1-->Add record to file" << endl;
      cout << "\t2-->Show record from file" << endl;
      cout << "\t3-->Search Record from file" << endl;
      cout << "\t4-->Update Record" << endl;
      cout << "\t5-->Delete Record" << endl;
      cout << "\t6-->Quit" << endl;
      cout << "Enter your choice: "; cin >> choice;
      if(choice == 1)
          BankRecord::add_record();
      else if(choice == 2)
          BankRecord::show_data();
      else if(choice == 3)
          BankRecord::search_record();
      else if(choice == 4)
          BankRecord::edit_record();
      else if(choice == 5)
          BankRecord::delete_record();
      cout << endl;
    } while (choice != 6);
   
  return 0;
}
