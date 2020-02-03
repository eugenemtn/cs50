from cs50 import SQL
import csv
import sys

if len(sys.argv) != 2:
    print("Usage: python import.py <csv file>")
    exit(1)

raw_data = list()

def main():
    with open(sys.argv[1], "r") as csv_file:
        reader = csv.DictReader(csv_file, delimiter=",")
        for row in reader:
            raw_data.append(row)

    db = SQL("sqlite:///students.db")
    house = None
    birth = None
    name = {
        "first": None,
        "middle": None,
        "last": None,
    }

    for student in raw_data:
        split_name = student["name"].split(" ")
        name["first"] = split_name[0]
        name["middle"] = split_name[1] if len(split_name) == 3 else None
        name["last"] = split_name[2] if len(split_name) == 3 else split_name[1]
        house = student["house"]
        birth = student["birth"]
        db.execute(
            "INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
            name["first"],
            name["middle"],
            name["last"],
            house,
            birth,
        )

if __name__ == "__main__":
    main()
