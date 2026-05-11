# Bank Management System in C

A simple **Bank Management System** developed in the C programming language using **random access file handling**. This project demonstrates core concepts of file handling, structures, authentication, searching, and account management.

---

## Features

* User Authentication System
* Add New Account
* Update Account Balance
* Delete Account
* Search Account by Name
* Transfer Money Between Accounts
* Display All Accounts
* Export Accounts to Text File
* Random Access Binary File Storage

---

## Technologies Used

* C Programming
* File Handling
* Structures
* Binary Files
* Random Access Files

---

## Project Structure

```text
Bank-Management-System-C/
│
├── main.c
├── credit.dat
├── accounts.txt
└── README.md
```

---

## Login Credentials

| Username | Password   |
| -------- | ---------- |
| user    | yalzo_here    |

---

## How to Run

### Compile the Program

```bash
gcc trans.c -o trans.exe
```

### Run the Program

```bash
./trans
```

For Windows:

```bash
trans.exe
```

---

## Menu Options

```text
1 - Store formatted accounts file
2 - Update an account
3 - Add a new account
4 - Delete an account
5 - Search account by name
6 - Transfer money
7 - Display all accounts
8 - End program
```

---

## File Handling

The project uses a binary file:

```text
credit.dat
```

to store account information using random access.

A text version of account records can also be generated:

```text
accounts.txt
```

---

## Concepts Demonstrated

* Structures in C
* Random Access Files
* fread() and fwrite()
* fseek() and rewind()
* Authentication Logic
* Searching Algorithms
* File-based Data Storage

---

## Future Improvements

* Hidden Password Input
* Transaction History
* PIN-based Authentication
* Admin/User Roles
* GUI Interface
* Database Integration
* Encryption for Passwords

---

## Author

Developed by SUDHARSHAN RAVIKUMAR 

---

## License

This project is created for educational and learning purposes.
