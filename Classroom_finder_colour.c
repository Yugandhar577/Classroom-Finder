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

// Utility Functions
void trim(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
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
    const char *days[] = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
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

// CSV Parsing
int parseCSV(RoomRecord records[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }
    char line[512];
    int count = 0;
    fgets(line, sizeof(line), file);  // Skip header
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (!token) continue;

        strcpy(records[count].day, token);
        token = strtok(NULL, ","); strcpy(records[count].timeSlot, token ? token : "");
        token = strtok(NULL, ","); strcpy(records[count].room, token ? token : "");
        token = strtok(NULL, ","); strcpy(records[count].course, token ? token : "");
        token = strtok(NULL, ","); strcpy(records[count].year, token ? token : "");
        token = strtok(NULL, ","); strcpy(records[count].division, token ? token : "");
        token = strtok(NULL, ","); strcpy(records[count].subject, token ? token : "");
        token = strtok(NULL, ",\n"); strcpy(records[count].teacher, token ? token : "");

        trim(records[count].day); toLowerCase(records[count].day);
        trim(records[count].timeSlot); trim(records[count].room);
        trim(records[count].course); trim(records[count].year);
        trim(records[count].division); trim(records[count].subject); trim(records[count].teacher);

        count++;
    }
    fclose(file);
    return count;
}

// Binary search for vacant rooms
int binarySearchVacant(RoomRecord records[], int count, char *day, char *time) {
    int left = 0, right = count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmpDay = strcmp(records[mid].day, day);
        int cmpTime = strcmp(records[mid].timeSlot, time);
        if (cmpDay == 0 && cmpTime == 0) {
            // Scan around mid for all matches
            int found = 0;
            int i = mid;
            while (i >= 0 && strcmp(records[i].day, day) == 0 && strcmp(records[i].timeSlot, time) == 0) {
                if (strlen(records[i].course) == 0) {
                    printf("%s\n", records[i].room);
                    found = 1;
                }
                i--;
            }
            i = mid + 1;
            while (i < count && strcmp(records[i].day, day) == 0 && strcmp(records[i].timeSlot, time) == 0) {
                if (strlen(records[i].course) == 0) {
                    printf("%s\n", records[i].room);
                    found = 1;
                }
                i++;
            }
            return found;
        } else if (cmpDay < 0 || (cmpDay == 0 && cmpTime < 0)) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return 0;
}

// Main Menu
int main() {
    RoomRecord records[MAX_RECORDS];
    int count = parseCSV(records, "college_timetable_sorted.csv");
    int choice;
    do {
        printf("\n");
        printf("=============================================\n");
        printf("         \033[1;34m=== Classroom Finder ===\033[0m\n");
        printf("=============================================\n");
        printf("  \033[1;32m1.\033[0m Search vacant rooms by day and time\n");
        printf("  \033[1;32m2.\033[0m Show schedule by room and day\n");
        printf("  \033[1;32m3.\033[0m View full timetable by building and floor\n");
        printf("  \033[1;32m4.\033[0m Search individual room status\n");
        printf("  \033[1;32m5.\033[0m Exit\n");
        printf("---------------------------------------------\n");
        printf("Enter your choice (\033[1;33m1-5\033[0m): ");

        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("\033[1;31mInvalid choice.\033[0m\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 1) {
            char day[10], time[20];
            printf("\n\033[1;36m--- Vacant Room Search ---\033[0m\n");
            while (1) {
                printf("Enter day (e.g., Monday): ");
                fgets(day, sizeof(day), stdin);
                trim(day);
                toLowerCase(day);
                if (isValidDay(day)) break;
                printf("\033[1;31mInvalid day. Please enter a valid weekday (Mon-Sat).\033[0m\n");
            }
            while (1) {
                printf("Enter time (e.g., 09:00-10:00): ");
                fgets(time, sizeof(time), stdin);
                trim(time);
                if (isValidTimeSlot(time)) break;
                printf("\033[1;31mInvalid time slot format. Please enter as HH:MM-HH:MM.\033[0m\n");
            }

            printf("\n\033[1;35mVacant rooms on %s at %s:\033[0m\n", day, time);
            if (!binarySearchVacant(records, count, day, time)) {
                printf("\033[1;31mNo vacant rooms found.\033[0m\n");
            }
        }

        else if (choice == 2) {
            char room[10], day[10];
            printf("\n\033[1;36m--- Room Schedule Search ---\033[0m\n");
            while (1) {
                printf("Enter room number (e.g., 3014): ");
                fgets(room, sizeof(room), stdin);
                trim(room);
                if (isValidRoomNumber(room)) break;
                else printf("\033[1;31mInvalid room number. Please enter again.\033[0m\n");
            }
            while (1) {
                printf("Enter day (e.g., Monday): ");
                fgets(day, sizeof(day), stdin);
                trim(day);
                toLowerCase(day);
                if (isValidDay(day)) break;
                printf("\033[1;31mInvalid day. Please enter a valid weekday (Mon-Sat).\033[0m\n");
            }

            printf("\n\033[1;35mSchedule for Room %s on %s:\033[0m\n", room, day);
            printf("----------------------------------------------------------\n");
            printf("%-20s  %-20s  %-25s\n", "Time Slot", "Status", "Course");
            printf("----------------------------------------------------------\n");
            int found = 0;
            for (int i = 0; i < count; i++) {
                char dayLower[10];
                strcpy(dayLower, records[i].day);
                toLowerCase(dayLower);

                if (strcmp(records[i].room, room) == 0 && strcmp(dayLower, day) == 0) {
                    printf("%-20s  %-20s  %-25s\n", records[i].timeSlot,
                           strlen(records[i].course) ? "Occupied" : "Vacant",
                           strlen(records[i].course) ? records[i].course : "-");
                    found = 1;
                }
            }
            if (!found) printf("No records found for this room and day.\n");
        }

        else if (choice == 3) {
            // Timetable by Building & Floor
            char buildingStr[3], floorStr[3];
            int building, floor;
            printf("\n\033[1;36m--- Timetable by Building & Floor ---\033[0m\n");
            while (1) {
            printf("Enter building number (\033[1;33m1-4\033[0m): ");
            fgets(buildingStr, sizeof(buildingStr), stdin);
            trim(buildingStr);
            if (strlen(buildingStr) == 1 && buildingStr[0] >= '1' && buildingStr[0] <= '4') {
                building = buildingStr[0] - '0';
                break;
            }
            printf("\033[1;31mInvalid building number.\033[0m\n");
            }
            while (1) {
            printf("Enter floor number (\033[1;33m0-4\033[0m): ");
            fgets(floorStr, sizeof(floorStr), stdin);
            trim(floorStr);
            if (strlen(floorStr) == 1 && floorStr[0] >= '0' && floorStr[0] <= '4') {
                floor = floorStr[0] - '0';
                break;
            }
            printf("\033[1;31mInvalid floor number.\033[0m\n");
            }
            printf("press Enter to continue...\n");
            while (getchar() != '\n'); // Clear input buffer

            char matchedRooms[MAX_RECORDS][10];
            int matchedCount = 0;

            for (int i = 0; i < count; i++) {
                // Room numbering: first digit = building, second digit = floor
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
                printf("\n\033[1;31mNo rooms found on Building %d, Floor %d.\033[0m\n", building, floor);
            } else {
                printf("\n\033[1;35mTimetable for Building %d, Floor %d:\033[0m\n", building, floor);
                for (int i = 0; i < matchedCount; i++) {
                    printf("\n\033[1;34mRoom %s:\033[0m\n", matchedRooms[i]);
                    printf("---------------------------------------------------------\n");
                    printf("%-20s %-25s %-20s\n", "Day", "Time Slot", "Status");
                    printf("---------------------------------------------------------\n");
                    int found = 0;
                    for (int j = 0; j < count; j++) {
                        if (strcmp(records[j].room, matchedRooms[i]) == 0) {
                            printf("%-20s %-25s %-20s\n", records[j].day, records[j].timeSlot,
                                strlen(records[j].course) == 0 ? "Vacant" : "Occupied");
                            found = 1;
                        }
                    }
                    if (!found) printf("No records for this room.\n");
                }
            }
        }

        else if (choice == 4) {
            char room[10], day[10], time[20];
            printf("\n\033[1;36m--- Individual Room Status Search ---\033[0m\n");
            while (1) {
                printf("Enter room number (e.g., 3014): ");
                fgets(room, sizeof(room), stdin);
                trim(room);
                if (isValidRoomNumber(room)) break;
                else printf("\033[1;31mInvalid room number. Please enter again.\033[0m\n");
            }
            while (1) {
                printf("Enter day (e.g., Monday): ");
                fgets(day, sizeof(day), stdin);
                trim(day);
                toLowerCase(day);
                if (isValidDay(day)) break;
                printf("\033[1;31mInvalid day. Please enter a valid weekday (Mon-Sat).\033[0m\n");
            }
            while (1) {
                printf("Enter time (e.g., 09:00-10:00): ");
                fgets(time, sizeof(time), stdin);
                trim(time);
                if (isValidTimeSlot(time)) break;
                printf("\033[1;31mInvalid time slot format. Please enter as HH:MM-HH:MM.\033[0m\n");
            }

            printf("\n\033[1;35mStatus for room %s on %s at %s:\033[0m\n", room, day, time);
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(records[i].room, room) == 0 &&
                    strcmp(records[i].day, day) == 0 &&
                    strcmp(records[i].timeSlot, time) == 0) {
                    if (strlen(records[i].course)) {
                        printf("Occupied by %s (Course: %s)\n", records[i].teacher, records[i].course);
                    } else {
                        printf("Vacant\n");
                    }
                    found = 1;
                    break;
                }
            }
            if (!found) printf("No record found for this room at the specified time.\n");
        }

        else if (choice == 5) {
            printf("\n\033[1;32mThank you for using Classroom Finder! Exiting...\033[0m\n");
            break;
        } else {
            printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
        }
    } while (choice != 5);

    printf("\nPress Enter to exit...");
    getchar();
    return 0;
}
