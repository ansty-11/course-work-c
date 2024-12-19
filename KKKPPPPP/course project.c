#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define POROG 500
#define WARNING_THRESHOLD 0.05 * POROG
#define MAX_TANKS 10 // ������������ ���������� �����������
#define MAX_FILENAME_SIZE 20 // ���������� ���������� �������� ��� �������� �����
#define MAX_RECORD_COUNT 100 // ������������ ���������� ��������������� �������

// ��������� ��� �������� ������ �������� � ������� ����������
typedef struct {
    float level; // ������� �������� � ����������
    int index; // ������ ����������
} Record;

/**
 * ������� ���������� ������ � ������� �������� � ����.
 *
 * @param filename ��� ����� ��� ������ ������.
 * @param records ������ ������� � ������� � ������� ��������.
 * @param count ���������� ������� ��� ������.
 */
int data(const char* filename, Record* records, int count);

/*
 * ������� ����������� ������ �� ����� ��� ���������� ����������.
 */
int analyz_data();

/**
 * ������� �������� ������ � ������� �������� �� ������������.
 */
int collection_date();

/*
 * ������� ���������� ��������� ������ ������� �������� ��� �����������.
 * @param records ��������� �� ������ ������� ��� �������� ��������������� ������.
 * @param count ��������� �� ���������� ��� �������� ���������� �������.
 */
int generate_random_data(Record** records, int* count);

/*
 * ������� ��������� ������ �� �������� ����������� � ������� ��������.
 *
 * @param records ������ ������� ��� ����������.
 * @param count ���������� ������� ��� ����������.
 */
int sort_records(Record* records, int count);

/*
 * ������� ���������� ���� ������ � ������ �������� � ����.
 * @param file ��������� �� ���� ��� ������ ������.
 * @param record ������ � ������� � ������ ��������.
 */
int write_record_to_file(FILE* file, Record record);

/*
 * ������� ������������ ���� ������ �� ������������ � ��������� �� � ������ �������.
 * @param records ������ ������� ��� ���������� ��������� ������.
 * @param count ��������� �� ���������� ��� �������� ���������� �������.
 */
int handle_user_input(Record* records, int* count);

/*
 * ������� ��������� ������������ ������� ����� ������ �������� � ������� ����������.
 * @param input ������ � ���������� ������� �� ������������.
 * @param record ��������� �� ������ ��� ���������� ���������� ������.
 */
int validate_input(const char* input, Record* record);

/*
 * ������� ��������� ������������ ������� �������� ��� ������ ����������,
 * ������� �������������� ��� ��������� ������������.
 * @param records ������ ������� � ������� � ������� ��������.
 * @param count ���������� ������� ��� �������� ������������.
 */
int check_monotonicity(Record* records, int count);

/*
 * ������� ������� ������� ������� �������� �� �����.
 * @param total ����� ������� �������� ��� ������� �������� ��������.
 * @param count ���������� ������� ��� ������� �������� ��������.
 */
int print_average_level(float total, int count);

/*
 * ������� ������� ������������ � ����������� ������ �������� �� �����.
 * @param max ������������ ������� ��������.
 * @param min ����������� ������� ��������.
 */
int print_max_min_levels(float max, float min);

int main() {
    setlocale(LC_ALL, "RUS"); //�����������
    int choice;
    int monitoring = 0;

    while (1) {
        // ������� ����
        printf("1. �������� ����� �����������\n");
        printf("2. ��������� ����� �����������\n");
        printf("3. ������ ������ �� �����\n");
        printf("4. �����\n");
        printf("�������� �����: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: 
            monitoring = 1;
            while (monitoring) {
                // ������� ������ �����������
                printf("1. ���� ������ �������\n");
                printf("2. �������������� ��������� ������\n");
                printf("3. ��������� � ������� ����\n");
                printf("�������� �����: ");
                scanf("%d", &choice);
                switch (choice) {
                case 1: // ���� ������ �������
                    collection_date();
                    break;
                case 2: { // �������������� ��������� ������
                    Record* records = 0;
                    int count = 0;
                    generate_random_data(&records, &count);
                    free(records); // ������������ ������ ����� �������������
                    break;
                }
                case 3: // ������� � ������� ����
                    monitoring = 0;
                    break;
                default:
                    printf("�������� �����. ���������� ��� ���.\n");
                }
            }
            break;
        case 2: // ���������� ������ �����������
            monitoring = 0;
            printf("����� ����������� ��������.\n");
            break;
        case 3: // ������ ������ �� �����
            analyz_data();
            break;
        case 4: // ����� �� ���������
            exit(0);
        default:
            printf("�������� �����. ���������� ��� ���.\n");
        }
    }
    return 0;
}

int collection_date() {
    Record* records = malloc(sizeof(Record) * MAX_RECORD_COUNT); // ��������� ������ ��� ������ �������
    if (!records) {
        printf("������ ��������� ������.\n");
        return;
    }

    int count = 0;
    handle_user_input(records, &count); // ��������� ����� ������ �� ������������
    data("data.txt", records, count); // ������ ��������� ������ � ����
    free(records); // ������������ ������ ����� �������������
}

int handle_user_input(Record* records, int* count) {
    while (*count < MAX_RECORD_COUNT) { // ����������� �� ���������� �������� �������
        char input[MAX_RECORD_COUNT]; //����� ��� ����� ������ �������������
        printf("������� ������� �������� � ������� '[��������] �, [������ ����������]' (��� 'exit' ��� ������): ");
        scanf(" %[^\n]s", input); // �������

        if (strcmp(input, "exit") == 0) { // �������� ������� ������ �� �����
            printf("����� �� ������ �����������.\n");
            break; // ����� �� ����� �����
        }

        Record record;
        if (validate_input(input, &record)) { // �������� ������������ �����
            records[*count] = record; // ���������� ������
            (*count)++;
            check_monotonicity(records, *count); // �������� ������������ ������� �������� ����� ���������� ����� ������
        }
        else {
            printf("������ ������� �����. ���������� ��� ���.\n");
        }
    }
}

int validate_input(const char* input, Record* record) {
    char* token = strtok((char*)input, "�, ");
    if (token != NULL) {
        record->level = atof(token); // �������������� ������� �������� � ������� �������� (float)
        token = strtok(NULL, "�, ");
        if (token != NULL) {
            record->index = atoi(token); // �������������� ������� �������� � ������ ���������� (int)
            if (record->level >= 0 && record->level <= POROG) {
                return 1; // �������� ���������
            }
        }
    }
    return 0; // ������ ���������
}

int check_monotonicity(Record* records, int count) { 
    // ��������, ��� ���� ��� ������� 2 ������ � ��� ������� ����������� ���������
    if (count > 1 && records[count - 1].index == records[count - 2].index) { 
        // �������� ������� ������������ ������ ��������
        if ((records[count - 1].level < records[count - 2].level - 1) || 
            // ������� �������� ��������� ������ �� ������ ���������� �� ������ ���������� ������ ����� ��� �� 1 �������
            (records[count - 1].level > records[count - 2].level + 1)) { 
            printf("������: �������� ������������ ������ �������� ��� ���������� %d!\n", records[count - 1].index);
        }
    }
    return 0;
}

int generate_random_data(Record** records, int* count) {
    srand(time(NULL)); // ������������� ���������� ��������� �����
    *count = MAX_RECORD_COUNT; // ��������� ���������� �������

    *records = malloc(sizeof(Record) * (*count));
    if (*records == NULL) {
        printf("������ ��������� ������.\n");
        return 1;
    }

    float previous_level[MAX_TANKS] = { 0 }; // ������ ��� �������� ���������� ������� �� �����������
    for (int i = 0; i < MAX_TANKS; i++) {
        previous_level[i] = rand() % (POROG + 1); // ������������� ��������� �������
    }

    for (int i = 0; i < *count; i++) {
        float new_level;
        int tank_index = rand() % MAX_TANKS + 1; // ��������� ������� ���������� �� 1 �� MAX_TANKS

        do {
            new_level = previous_level[tank_index - 1] + ((rand() % 3) - 1); // ��������� ������ ������
            new_level = new_level < 0 ? 0 : new_level > POROG ? POROG : new_level; // ����������� �� ��������
        } while ((new_level < previous_level[tank_index - 1] - 1) ||
            (new_level > previous_level[tank_index - 1] + 1));

        (*records)[i].level = new_level;
        (*records)[i].index = tank_index;

        // ������ ������ � ��������������� ���� ����������
        char filename[MAX_FILENAME_SIZE];
        sprintf(filename, "tank_%d.txt", tank_index); // ������������ ����� �����
        FILE* tank_file = fopen(filename, "a"); // �������� ����� ��� ���������� ������
        if (tank_file != NULL) {
            write_record_to_file(tank_file, (*records)[i]);
            fclose(tank_file);
        }
        else {
            printf("������ �������� ����� %s ��� ������.\n", filename);
        }
        previous_level[tank_index - 1] = new_level; // ���������� ����������� ������ ��� ����������
    }

    printf("������ �������� � ����� �����������.\n");
    sort_records(*records, *count); // ���������� ������� ���� �����������
    return 0;
}

int write_record_to_file(FILE* file, Record record) {
    fprintf(file, "%.2f %d\n", record.level, record.index);
}

int sort_records(Record* records, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if ((records[i].index > records[j].index) ||
                (records[i].index == records[j].index && records[i].level > records[j].level)) {
                Record temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }
    data("data.txt", records, count); // ������ ���� ������ � ����� ����
}

int data(const char* filename, Record* records, int count) {
    FILE* file = fopen(filename, "a");
    if (file == 0) {
        printf("������ �������� ����� ��� ������.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        write_record_to_file(file, records[i]);
    }

    fclose(file);
    printf("������ �������� � ���� %s.\n", filename);
    return 0;
}

int analyz_data() {
    int tank_index;

    // ������ ������� ���������� � ������������
    printf("������� ����� ���������� ��� ������� (�� 1 �� %d): ", MAX_TANKS);
    scanf("%d", &tank_index);

    if (tank_index < 1 || tank_index > MAX_TANKS) { // �������� �� ������������ ���������� ������� ���������
        printf("������: �������� ������ ����������.\n");
        return;
    }

    char filename[MAX_FILENAME_SIZE];
    sprintf(filename, "tank_%d.txt", tank_index); // ������������ ����� ����� �� ������ ������� ����������

    FILE* file = fopen(filename, "r"); // �������� ����� ��� ������
    if (file == 0) {
        printf("������ �������� ����� %s ��� �������.\n", filename);
        return;
    }

    float total = 0; // ���������� ��� �������� ������ ������
    float max_level = -1; // ������������� ������������ ������
    float min_level = POROG + 1; // ������������� ������������� ������ �� ������� �������� ��� POROG
    int count = 0; // ������� ������� � �����

    while (!feof(file)) { // ������ ������ �� �����, ���� �� ��������� ����� �����
        Record record;
        fscanf(file, "%f %d", &record.level, &record.index); // ������ ������ � �������
        total += record.level; // ������������ ������

        if (record.level > max_level) {
            max_level = record.level; // ���������� ������������� ������
        }

        if (record.level < min_level) {
            min_level = record.level; // ���������� ������������ ������
        }

        count++;
    }

    fclose(file); // �������� �����

    print_average_level(total, count); // ������ �������� ������ ��������

    if (count > 0) {
        print_max_min_levels(max_level, min_level); // ������ ������������� � ������������ ������� ��������
    }
    else {
        printf("��� ���������� ������ ��� �������.\n");
    }
}

int print_average_level(float total, int count) {
    if (count > 0) { // �������� �� ������������ ���������� �������
        float average_level = total / count; // ���������� �������� ������
        printf("������� ������� ��������: %.2f �\n", average_level);
    }
    else {
        printf("��� ���������� ������ ��� �������.\n");
    }
}

int print_max_min_levels(float max, float min) {
    if (max >= 0 && min <= POROG) { // �������� �� ���������� �������� ������� ��������
        printf("������������ ������� ��������: %.2f �\n", max); // ������ ������������� ������
        printf("����������� ������� ��������: %.2f �\n", min); // ������ ������������ ������
    }
    else {
        printf("��� ���������� ������ ��� �������.\n");
    }
}