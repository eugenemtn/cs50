import sys
import csv

if len(sys.argv) != 3:
    print("Usage: python dna.py <database> <dna sequence>")
    exit(1)

database = list()
sequence = ''
sequence_counts = dict()


def main():
    str_list = list()

    with open(sys.argv[1], "r") as database_file:
        reader = csv.DictReader(database_file, delimiter=",")
        for row in reader:
            database.append(row)
        str_list = reader.fieldnames[1:len(reader.fieldnames)]

    with open(sys.argv[2], "r") as sequence_file:
        sequence = sequence_file.read()

    sequence_length = len(sequence)
    count_str = 0

    for str_type in str_list:
        counts_arr = [0] * (sequence_length)
        str_type_length = len(str_type)
        for i in range(sequence_length):
            for j in range(i, sequence_length, str_type_length):
                if str_type == sequence[j:j + str_type_length]:
                    count_str += 1
                else:
                    break
            counts_arr[i] = count_str
            count_str = 0
        sequence_counts[str_type] = max(counts_arr)
    for person in database:
        match = True
        for str_type in str_list:
            if int(person[str_type]) != sequence_counts[str_type]:
                match = False
                break
        if match:
            print(f"{person['name']}")
            return
    print("No match")


if __name__ == "__main__":
    main()
