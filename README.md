# MINIDB
A lightweight CLI-based database engine built in C++ that supports dynamic schemas, file-based storage, and basic query operations.

<hr>

## 🚀 Features
- Create and manage tables
- Insert, update, and delete records (soft delete)
- Multi-condition query support (AND queries)
- Schema-aware query validation
- Limit results using "LIMIT"
- View table schema using "DESCRIBE"
- List all tables using "SHOW TABLES"
- Drop tables ("DROP TABLE")
- Formatted table output

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

## 📌 Supported Commands
- create tableName column1 column2 ...
- insert tableName value1 value2...
- find tableName column1 value1 column2 value2...
- update tableName id newValue1 newValue2...
- delete tableName id
- describe tableName
- showTables
- drop tableName
- count tableName

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



