# MINIDB
A lightweight CLI-based database engine built in C++ that supports dynamic schemas, file-based storage, and basic query operations.

<hr>

## 🚀 Overview
MiniDB is a simple database engine that supports:
- Dynamic table schemas
- Insert, delete, and query operations
- Multi-condition filtering (AND queries)
- File-based persistent storage
- Custom CLI interface

It is designed to demonstrate core database concepts like indexing, schema validation, and storage handling.

<hr>

## ✨ Features
- Dynamic schema support (no fixed struct)
- Query by column with multiple conditions (AND)
- Soft delete (isActive flag)
- Formatted table output
- File-based storage (.db + .meta)
- Basic indexing for faster lookup
- CLI-based interaction

<hr>

## 🛠️ Tech Stack
- C++
- File handling (text-based storage)
- STL (vectors, maps, stringstream)

<hr>

## 📂 Storage Design
Each table is stored as:

- ".db" file → stores records
- ".meta" file → stores schema

Format:
id | isActive | column1 | column2 | ...

<hr>

## 💻 CLI Commands

### Create Table
create students name age city

### Insert Record
insert students kapil 20 delhi

### List Records
list students

### Find by Column
find students name kapil

### Multi-condition Find (AND)
find students name kapil age 20

### Delete Record
delete students 3

### Describe Table
describe students

### Show Tables
show tables

### Drop Table
drop students

<hr>

## ⚙️ How It Works
1. Queries are parsed via CLI
2. Schema validation is performed
3. Data is read from files
4. Operations are executed (filter, update, etc.)
5. Results are formatted and displayed

<hr>

## 🚧 Limitations
- Linear scan (O(n)) for queries
- No indexing (e.g., B+ Trees)
- No transactions or rollback
- No concurrency support

<hr>

## 🔮 Future Improvements

- Add indexing (B+ Trees)
- Implement transaction support
- Query optimization
- Binary storage for performance

<hr>

## 📖 What I Learned

- How databases handle schema and storage
- Query parsing and execution flow
- Importance of validation and modular design
- Trade-offs between simplicity and performance

<hr>

## 🛠 How to Run

### 1. Clone the repo
git clone https://github.com/your-username/mini-db-engine-cpp.git  
cd mini-db-engine-cpp  

### 2. Compile
g++ -std=c++17 src/*.cpp -o miniDB

### 3. Run
./miniDB

<hr>

## 👤 Author
Kapil Tadkase



