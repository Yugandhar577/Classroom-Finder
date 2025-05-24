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
        printf("\n \t=== Classroom Finder ===\n");
        printf("1. Search vacant rooms by day and time\n");
        printf("2. Show schedule by room and day\n");
        printf("3. View full timetable by building and floor\n");
        printf("4. Search room status by building and room\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            char day[10], time[20];
            printf("Enter day (e.g., Monday): ");
            fgets(day, sizeof(day), stdin);
            trim(day);
            toLowerCase(day);

            printf("Enter time (e.g., 09:00-10:00): ");
            fgets(time, sizeof(time), stdin);
            trim(time);

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
            printf("Enter room number (e.g., 3014): ");
            fgets(room, sizeof(room), stdin);
            trim(room);

            printf("Enter day (e.g., Monday): ");
            fgets(day, sizeof(day), stdin);
            trim(day);
            toLowerCase(day);

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
            scanf("%d", &building);
            printf("Enter floor number (1-4): ");   // Changed from 0-3 to 1-4
            scanf("%d", &floor);
            getchar();

            char matchedRooms[MAX_RECORDS][10];
            int matchedCount = 0;

            for (int i = 0; i < count; i++) {
                // Assuming room numbering format: first digit = building, second digit = floor
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
            printf("Enter room number (e.g., 3014): ");
            fgets(room, sizeof(room), stdin);
            trim(room);

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
