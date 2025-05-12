#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 200

typedef struct {
    char day[10];
    char date[15];
    char time[6];
    char building[10];
    char room[10];
    char status[10];
} RoomRecord;

// Trim leading/trailing whitespace and newline characters
void trim(char *str) {
    int i = 0, j = 0;
    while (str[i] == ' ' || str[i] == '\t') i++;
    while (str[i]) str[j++] = str[i++];
    str[j] = '\0';

    j = strlen(str) - 1;
    while (j >= 0 && (str[j] == ' ' || str[j] == '\n' || str[j] == '\r')) str[j--] = '\0';
}

int parseCSV(const char *filename, RoomRecord records[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open the file.\n");
        return 0;
    }

    char line[200];
    int count = 0;
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(records[count].day, token); trim(records[count].day);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(records[count].date, token); trim(records[count].date);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(records[count].time, token); trim(records[count].time);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(records[count].building, token); trim(records[count].building);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(records[count].room, token); trim(records[count].room);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(records[count].status, token); trim(records[count].status);

        count++;
    }

    fclose(file);
    return count;
}

void option1(RoomRecord records[], int count) {
    char date[15], time[6];
    int found = 0;
    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", date);
    printf("Enter time (HH:MM): ");
    scanf("%s", time);

    printf("\nVacant rooms on %s at %s:\n", date, time);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].date, date) == 0 &&
            strcmp(records[i].time, time) == 0 &&
            strcmp(records[i].status, "Vacant") == 0) {
            printf("Room %s in Building %s\n", records[i].room, records[i].building);
            found = 1;
        }
    }
    if (!found) printf("No vacant rooms found.\n");
}

void option2(RoomRecord records[], int count) {
    char day[10], time[6];
    int found = 0;
    printf("Enter day (e.g., Monday): ");
    scanf("%s", day);
    printf("Enter time (HH:MM): ");
    scanf("%s", time);

    printf("\nVacant rooms on %s at %s:\n", day, time);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].day, day) == 0 &&
            strcmp(records[i].time, time) == 0 &&
            strcmp(records[i].status, "Vacant") == 0) {
            printf("Room %s in Building %s\n", records[i].room, records[i].building);
            found = 1;
        }
    }
    if (!found) printf("No vacant rooms found.\n");
}

void option3(RoomRecord records[], int count) {
    char room[10], date[15];
    int found = 0;
    printf("Enter room number (e.g., 101): ");
    scanf("%s", room);
    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", date);

    printf("\nSchedule for Room %s on %s:\n", room, date);
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i].room, room) == 0 &&
            strcmp(records[i].date, date) == 0) {
            printf("%s at %s - %s\n", records[i].day, records[i].time, records[i].status);
            found = 1;
        }
    }
    if (!found) printf("No records found for this room on given date.\n");
}

int main() {
    RoomRecord records[MAX_RECORDS];
    int count = parseCSV("room_schedule.csv", records);

    if (count == 0) return 1;

    int choice;
    printf("\nCampus Room Tracker\n");
    printf("1. Search by Date and Time\n");
    printf("2. Search by Day and Time\n");
    printf("3. Search by Room Number and Date\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: option1(records, count); break;
        case 2: option2(records, count); break;
        case 3: option3(records, count); break;
        default: printf("Invalid choice.\n"); break;
    }

    return 0;
}