import csv

# Prompt CSV file
title = input("Title: ")

# Open CSV file
with open("shows2.csv", "r") as input:
    reader = csv.DictReader(input)

    # Read each row
    for row in reader:
        if title.lower() == row["primaryTitle"].lower():
            print(row["primaryTitle"], row["startYear"], row["genres"], sep=" | ")
        
