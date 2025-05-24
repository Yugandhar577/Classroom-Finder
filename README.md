# 🏢 Room Management System

A simple C program to manage and query classroom or meeting room usage records. It reads from a CSV file containing room booking information and offers a menu-based interface to view, filter, and analyze room availability across different buildings, floors, and dates.

---

## 📁 Project Structure

```
Classroom_Finder.c     --> Main source code file
college_timetable.csv      --> CSV file containing room usage data
README.md             --> This documentation file
```

---

## 📊 CSV File Format

The `college_timetable.csv` file contains structured data in the following format:

```
Day, TimeSlot,	Room,	Course,	Year,	Division,	Subject, Teacher

```

### Example:

```
Monday,2024-03-20,10:00,A1,1006,Occupied
Tuesday,2024-03-21,12:00,A1,1006,Free
```

---

## 🔧 Program Features

The system provides the following options:

1. **Display All Records**

   * Shows every entry in the CSV file.

2. **Filter by Day**

   * Displays records only for a specific day (e.g., "Monday").

3. **Floor-wise Timetable**

   * Shows full room usage data for all rooms on a specific floor within a building.

4. **Room Status Lookup**

   * Displays the complete usage schedule of a specific room on different dates and times.

5. **Exit**

   * Closes the program.

---

## 🧱 Data Structure

Room data is stored using the following structure:

```c
typedef struct {
    char day[10];
    char timeSlot[20];
    char room[10];
    char course[50];
    char year[5];
    char division[5];
    char subject[50];
    char teacher[50];
} RoomRecord;
```

Using `char[]` for all fields provides flexibility to handle alphanumeric room names, building codes, and formatting requirements.

---

Ensure the `college_timetable.csv` file is present in the same directory before running the program.

---

## 📌 Notes

* All data is loaded from the CSV file at runtime. No persistent data changes are made.
* All string comparisons are **case-sensitive**.
* Make sure your CSV is properly formatted (no missing commas or empty lines).
* Room and building codes should be entered **exactly as they appear** in the CSV.

---

## 🛠️ Future Improvements

* Add case-insensitive search
* Support real-time updates to the CSV file
* GUI-based interface using a graphics library (e.g., GTK)
* Logging of user actions

---

## 👨‍💻 Author

* Yugandhar Paulbudhe
* Year: 1st
* Course: B.Tech in Computer Science Engineering (AI)

---
