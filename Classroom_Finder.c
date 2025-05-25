#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 5000

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

void trim(char *str) {
    // Trim leading spaces
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
    // Trim trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

int isValidDay(const char *day) {
    char *days[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
    for (int i = 0; i < 6; i++) {
        if (strcmp(day, days[i]) == 0) return 1;
    }
    return 0;
}

int isValidTimeSlot(const char *timeSlot) {
    if (strlen(timeSlot) != 11 || timeSlot[2] != ':' || timeSlot[5] != '-' || timeSlot[8] != ':') return 0;
    for (int i = 0; i < 11; i++) {
        if (i == 2 || i == 5 || i == 8) continue;
        if (!isdigit((unsigned char)timeSlot[i])) return 0;
    }
    return 1;
}

int isDigitsOnly(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int isValidRoomNumber(const char *room) {
    if (strlen(room) != 4) return 0;

    for (int i = 0; i < 4; i++) {
        if (!isdigit(room[i])) return 0;
    }

    int building = room[0] - '0';
    int floor = room[1] - '0';
    int roomIndex = (room[2] - '0') * 10 + (room[3] - '0');

    if (building < 1 || building > 4) return 0;
    if (floor < 0 || floor > 4) return 0;
    if (roomIndex < 0 || roomIndex > 25) return 0;

    return 1;
}

int parseCSV(RoomRecord records[]) {
    FILE *file = fopen("college_timetable.csv", "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }
    char line[512];
    int count = 0;
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (!token) continue;

        strcpy(records[count].day, token);
        token = strtok(NULL, ",");
        strcpy(records[count].timeSlot, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(records[count].room, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(records[count].course, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(records[count].year, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(records[count].division, token ? token : "");
        token = strtok(NULL, ",");
        strcpy(records[count].subject, token ? token : "");
        token = strtok(NULL, ",\n");
        strcpy(records[count].teacher, token ? token : "");

        trim(records[count].day);
        trim(records[count].timeSlot);
        trim(records[count].room);
        trim(records[count].course);
        trim(records[count].year);
        trim(records[count].division);
        trim(records[count].subject);
        trim(records[count].teacher);

        count++;
    }
    fclose(file);
    return count;
}

int main() {
    RoomRecord records[MAX_RECORDS];
    int count = parseCSV(records);
    int choice;

    do {
        printf("\n \t=== Campus Room Tracker ===\n");
        printf("1. Search vacant rooms by day and time\n");
        printf("2. Show schedule by room and day\n");
        printf("3. View full timetable by building and floor\n");
        printf("4. Search individual room status\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 1) {
            char day[10], time[20];
            while (1) {
                printf("Enter day (e.g., Monday): ");
                fgets(day, sizeof(day), stdin);
                trim(day);
                toLowerCase(day);
                if (isValidDay(day)) break;
                printf("Invalid day. Please enter a valid weekday (Mon-Sat).\n");
            }

            while (1) {
                printf("Enter time (e.g., 09:00-10:00): ");
                fgets(time, sizeof(time), stdin);
                trim(time);
                if (isValidTimeSlot(time)) break;
                printf("Invalid time format. Use hh:mm-hh:mm.\n");
            }

            printf("\nVacant rooms on %s at %s:\n", day, time);
            int found = 0;
            for (int i = 0; i < count; i++) {
                char dayLower[10];
                strcpy(dayLower, records[i].day);
                toLowerCase(dayLower);

                if (strcmp(dayLower, day) == 0 &&
                    strcmp(records[i].timeSlot, time) == 0 &&
                    strlen(records[i].course) == 0) {
                    printf("%s\n", records[i].room);
                    found = 1;
                }
            }
            if (!found) printf("No vacant rooms found.\n");
        }

        else if (choice == 2) {
            char room[10], day[10];
           while (1) {
                printf("Enter room number (e.g., 3014): ");
                fgets(room, sizeof(room), stdin);
                trim(room);
                if (isValidRoomNumber(room)) break;
                else printf("Invalid room number. Please enter again.\n");
            }
                
            while (1) {
                printf("Enter day (e.g., Monday): ");
                fgets(day, sizeof(day), stdin);
                trim(day);
                toLowerCase(day);
                if (isValidDay(day)) break;
                printf("Invalid day. Please enter a valid weekday (Mon-Sat).\n");
            }

            printf("\nSchedule for Room %s on %s:\n", room, day);
            int found = 0;
            for (int i = 0; i < count; i++) {
                char dayLower[10];
                strcpy(dayLower, records[i].day);
                toLowerCase(dayLower);

                if (strcmp(records[i].room, room) == 0 && strcmp(dayLower, day) == 0) {
                    printf("%s - %s - %s\n", records[i].timeSlot,
                           strlen(records[i].course) == 0 ? "Vacant" : "Occupied",
                           strlen(records[i].course) == 0 ? "-" : records[i].course);
                    found = 1;
                }
            }
            if (!found) printf("No records found for this room on given day.\n");
        }

        else if (choice == 3) {
            int building, floor;

            printf("Enter building number (1-4): ");
            if (scanf("%d", &building) != 1 || building < 1 || building > 4) {
                printf("Invalid building number.\n");
                while (getchar() != '\n' && getchar() != EOF);
                continue;
            }

            printf("Enter floor number (0-4): ");
            if (scanf("%d", &floor) != 1 || floor < 0 || floor > 4) {
                printf("Invalid floor number.\n");
                while (getchar() != '\n' && getchar() != EOF);
                continue;
            }

            char matchedRooms[MAX_RECORDS][10];
            int matchedCount = 0;

            for (int i = 0; i < count; i++) {
                if ((records[i].room[0] - '0') == building && (records[i].room[1] - '0') == floor) {
                    int exists = 0;
                    for (int j = 0; j < matchedCount; j++) {
                        if (strcmp(matchedRooms[j], records[i].room) == 0) {
                            exists = 1;
                            break;
                        }
                    }
                    if (!exists) {
                        strcpy(matchedRooms[matchedCount], records[i].room);
                        matchedCount++;
                    }
                }
            }

            if (matchedCount == 0) {
                printf("\nNo rooms found on Building %d, Floor %d.\n", building, floor);
            } else {
                printf("\nTimetable for Building %d, Floor %d:\n", building, floor);
                for (int i = 0; i < matchedCount; i++) {
                    printf("\nRoom %s:\n", matchedRooms[i]);
                    int found = 0;
                    for (int j = 0; j < count; j++) {
                        if (strcmp(records[j].room, matchedRooms[i]) == 0) {
                            printf("%s %s - %s\n", records[j].day, records[j].timeSlot,
                                strlen(records[j].course) == 0 ? "Vacant" : "Occupied");
                            found = 1;
                        }
                    }
                    if (!found) printf("No records for this room.\n");
                }
            }
        }

        else if (choice == 4) {
            char room[10];
            while (1) {
                printf("Enter room number (e.g., 3014): ");
                fgets(room, sizeof(room), stdin);
                trim(room);

                if (isValidRoomNumber(room)) break;
                else printf("Invalid room number. Please enter again.\n");
            }

            printf("\nStatus for Room %s:\n", room);
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(records[i].room, room) == 0) {
                    printf("%s %s - %s\n", records[i].day, records[i].timeSlot,
                           strlen(records[i].course) == 0 ? "Vacant" : "Occupied");
                    found = 1;
                }
            }
            if (!found) printf("No matching records found for this room.\n");
        }

        else if (choice != 5) {
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);
    printf("Press Enter to exit...");
    getchar();
    return 0;
}
