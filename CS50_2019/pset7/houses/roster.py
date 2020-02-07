from cs50 import SQL
import sys

if len(sys.argv) != 2:
    print("Usage: python roster.py <house>")
    exit(1)


def main():
    db = SQL("sqlite:///students.db")
    selection = \
        db.execute(f'SELECT first, middle, last, birth FROM students WHERE house="{sys.argv[1]}" ORDER BY last, first')

    for student in selection:
        print(f"{student['first']}", end=" ")
        if student['middle']:
            print(f"{student['middle']}", end=" ")
        print(f"{student['last']}, born {student['birth']}")


if __name__ == "__main__":
    main()
